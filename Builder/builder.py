import os
import glob
from enum import Enum
from parser import *

class BuilderContext:
  class NoOverwrite(Enum):
    FolderCopy = 0
    NameMangle = 1

  def __init__(self):
    self.target = ''
    self.script_path = ''
    self.script = ''
    self.fastmode = False
    self.no_overwrite = False

    self.include  = [ ]
    self.source   = [ ]
    self.objects  = ''

class Builder:
  class InvalidArgument(Exception):
    pass

  class ElementNotFound(Exception):
    def __init__(self, name):
      self.name = name

  def __init__(self):
    self.context = BuilderContext()
    self.ndlist = [ ]

  def parse_argv(self, argv) -> bool:
    i = 1

    try:
      while i < len(argv):
        arg = argv[i]

        if arg.startswith('-'):
          arg = arg[1:]

          if arg == 'fast':
            self.context.fastmode = True
            i += 1
          else:
            raise Builder.InvalidArgument()
        else:
          if self.context.script_path == '':
            self.context.script_path = arg + '.buildpy'

            with open(self.context.script_path, mode='r') as fs:
              self.context.script = ''.join(fs.readlines())
            
            i += 1
          else:
            print('script file already specified')
            raise Builder.InvalidArgument()
    except:
      return False

    return True
  
  def get_elem(self, sym) -> str:
    li = sym.split('.')
    ret = -1

    for nd in self.ndlist:
      if nd.name == li[0]:
        ret = nd
        break

    if ret == -1:
      raise Builder.ElementNotFound()

    for sym in li[1:]:
      found = False

      for nd in ret.value:
        if nd.name == sym:
          ret = nd
          found = True
          break

      if not found:
        raise Builder.ElementNotFound()

    return ret

  def detect_lang(self, ext):
    extlist = self.get_elem('build.extensions').value

    for x in extlist:
      if ext in x.value:
        return x.name

    return 0

  def mangle(self, path: str) -> str:
    return path[path.find('/') + 1:].replace('/', '@')

  def init_context(self) -> BuilderContext:
    ret = BuilderContext()

    extensions = [ ]

    for ext in self.get_elem('build.extensions'):
      

  def compile(self, com, file, destpath, flags):
    line = f'{com} {flags} {file} -o {destpath}'

    print(file)

    os.system(line)

  def execute(self) -> int:
    use_glob = self.get_elem('build.use_glob').value == 'true'
    extlist = self.get_elem('build.extensions').value
    src_files = [ ]

    for fol in self.get_elem('folders.source').value:
      for ext in extlist:
        for e in ext.value:
          if use_glob:
            src_files.extend(glob.glob(f'{fol}/**/*.{e.name}', recursive=True))
          else:
            src_files.extend(glob.glob(f'{fol}/*.{e.name}'))

    for src in src_files:
      pass

  def run(self, argv) -> int:
    if not self.parse_argv(argv):
      print('invalid argument')
      return 1

    if self.context.script_path == '':
      print('no build-script file')
      return 1

    try:
      prs = Parser(self.context.script)

      self.ndlist = prs.parse()

      self.execute()

      return 0
    except Parser.ParseError as e:
      print('parse error!')
    except Builder.ElementNotFound as e:
      print(f'element {e.name} is not defined!')

    return 1