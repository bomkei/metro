from concurrent.futures import ThreadPoolExecutor
from collections import defaultdict
from curses import use_default_colors
from email.policy import default
import os
import glob
import subprocess
from enum import Enum
from parser import *

class BuilderContext:
  class OverwriteProtector(Enum):
    FolderCopy = 0
    NameMangle = 1

  def __init__(self):
    self.target = ''
    self.script_path = ''
    self.script = ''

    # flags
    self.fastmode = False
    self.use_glob = False
    self.no_overwrite = BuilderContext.OverwriteProtector.FolderCopy

    # source file flags
    self.src_flags = defaultdict(str)

    # directories
    self.include  = [ ]
    self.source   = [ ]
    self.object_outdir  = 'build'
    self.object_subdir  = [ ]

    # compilers
    self.compilers = { }

    # files
    self.src_files = [ ]
    self.obj_files = [ ]

    self.extensions = defaultdict(list)

class Builder:
  class InvalidArgument(Exception):
    pass

  class ElementNotFound(Exception):
    def __init__(self, name):
      self.name = name

  def __init__(self):
    self.context = BuilderContext()
    self.ndlist = [ ]

  # ==== Parse arguments passed to application ====
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
      return 0

    for sym in li[1:]:
      found = False

      for nd in ret.value:
        if nd.name == sym:
          ret = nd
          found = True
          break

      if not found:
        return 0

    return ret

  def detect_lang(self, file):
    ext = file[file.rfind('.') + 1:]

    for key in self.context.extensions.keys():
      if ext in self.context.extensions[key]:
        return key

    return 0

  def mangle(self, path: str) -> str:
    return path[path.find('/') + 1:].replace('/', '@')

  def init_context(self) -> BuilderContext:
    ret = BuilderContext()

    ret.include = self.get_elem('folders.include').value
    ret.source = self.get_elem('folders.source').value

    # === builder flags ===
    if (e := self.get_elem('build.fast')) != 0:
      ret.fastmode = e.value == 'true'

    if (e := self.get_elem('build.use_glob')) != 0:
      ret.use_glob = e.value == 'true'

    # ===== Extensions =====
    for prop in self.get_elem('build.extensions').value:
      for item in prop.value:
        ret.extensions[prop.name].append(item.name)

    # ===== Flags =====
    for item in self.get_elem('build.flags').value:
      ret.src_flags[item.name] = ' '.join(item.value)

    # ===== Compilers =====
    for clist in self.get_elem('build.compiler').value:
      ret.compilers[clist.name] = clist.value[0]

    # ===== Source files =====
    for fol in ret.source:
      for extkey in ret.extensions.keys():
        for ext in ret.extensions[extkey]:
          if ret.use_glob:
            #print(1231231232)
            #print(fol, ext)
            ret.src_files.extend(glob.glob(f'{fol}/**/*.{ext}', recursive=True))
          else:
            ret.src_files.extend(glob.glob(f'{fol}/*.{ext}'))

    # ===== Object files =====
    ret.obj_files = [self.

    # ===== builder flag: no_overwrite =====
    if (e := self.get_elem('build.no_overwrite')) != 0:
      if e.value == 'copy_folder':
        ret.no_overwrite = BuilderContext.OverwriteProtector.FolderCopy

        for src in ret.src_files:
          if not (objdir := src[src.find('/') + 1 : src.rfind('/')]) in ret.object_subdir:
            ret.object_subdir.append(objdir)
      elif e.value == 'mangle':
        ret.no_overwrite = BuilderContext.OverwriteProtector.NameMangle

    #print(f'source files = {ret.src_files}')
    #print(ret.compilers)

    return ret

  def to_output_path(self, file):
    if self.context.no_overwrite == BuilderContext.OverwriteProtector.FolderCopy:
      return self.context.object_outdir + file[file.find('/') : file.rfind('.')] + '.o'
    else:
      return self.context.object_outdir + file[file.find('/') + 1:].replace('/', '@')

  def compile(self, file):
    lang = self.detect_lang(file)

    com = self.context.compilers[lang]
    destpath = self.to_output_path(file)

    flags = self.context.src_flags[lang]
    incl_flag = ' '.join(['-I' + x for x in self.context.include])

    line = f'{com} {flags} {incl_flag} {file} -c -o {destpath}'

    print(file)

    return subprocess.run(line, shell=True)

  def execute(self) -> int:
    self.context = self.init_context()

    # == create subdirs in objects folder ==
    for sub in self.context.object_subdir:
      os.system(f'mkdir -p {self.context.object_outdir}/{sub}')

    # == Compile ==
    if self.context.fastmode:
      pass
    else:
      for src in self.context.src_files:
        res = self.compile(src)

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