import re
import string

from pygls import IS_PYODIDE
import LexerHelpers
from b_types import *

class BuildpyParser:
  def __init__(self, source: list):
    self.source = source
    self.tokens = [ ]
    self.ctx = BuilderContext()
    self.indent = 0
    self.index = 0
    self.ate = 0
    self.cur_prop_elem = 0

  # -------------------
  #  Lex
  # -------------------
  def lex(self) -> list:
    ret = [ ]

    for i in range(len(self.source)):
      line = self.source[i]

      if len(line) == 0 or line.isspace():
        ret.append('\n')
        continue

      if (leftc := len(line) - len(line.lstrip())) != 0:
        ret.append(leftc * ' ')

      line = line.strip()
      tmp = re.split(' +', line)
      
      for j, tok in enumerate(tmp):
        if tok == '=':
          ret.extend(tmp[j:])
          break
        elif not tok[0] in '.':
          ret.extend(LexerHelpers.trimmer(tok))
        else:
          ret.append(tok)

      #ret.extend(tmp)
      ret.append('\n')

    while len(ret) != 0 and ret[-1] == '\n':
      ret = ret[:-1]

    return ret

  # -------------------
  #  Parser helpers
  # -------------------
  def check(self) -> bool:
    return self.index < len(self.tokens)

  def cur(self) -> str:
    return self.tokens[self.index]

  def eat(self, s: str) -> bool:
    if self.cur() == s:
      self.ate = self.index
      self.index += 1
      return True
    
    return False

  def eat_indent(self) -> bool:
    return self.eat('  ' * self.indent)

  def expect(self, s: str):
    if not self.eat(s):
      raise ErrorInfo(f'expected \'{s}\' cur={self.cur()}')
  
  def expect_indent(self, addval: int = 0):
    self.indent += addval
    self.expect('  ' * self.indent)

  def expect_ident(self):
    if not self.cur().isidentifier():
      raise ErrorInfo(f'{self.cur()} expected identifier')

    self.index += 1

  def newblock(self):
    self.expect(':')
    self.expect('\n')
    self.indent += 1

  # -------------------
  #  Parser
  # -------------------
  def read_list(self) -> str:
    val = ''

    while self.check():
      if self.eat('\n'):
        break

      tok = self.cur()

      if tok.startswith('$'):
        find = self.cur_prop_elem.find(tok[1:])

        if find == 0:
          raise ErrorInfo('undefined')
        
        val += find.value
      else:
        val += tok
      
      val += ' '
      self.index += 1

    return val

  def prs_element(self) -> ParserElement:
    elem = ParserElement(ParserElement.Kind.Item)
    cur = self.cur()

    if cur.startswith('.'):
      elem.kind = ParserElement.Kind.Flag
      elem.name = cur[1:]

      if not elem.name.isidentifier():
        raise ErrorInfo('aiui')

      self.index += 1
      self.expect('\n')
      return elem
    elif self.index < len(self.tokens) - 1 and self.tokens[self.index + 1] == ':':
      return self.prs_properties()

    elem.name = self.cur()
    self.expect_ident()

    if self.eat('='):
      elem.value = self.read_list()
    else:
      self.expect('\n')

    return elem

  def prs_properties(self) -> ParserElement:
    ret = ParserElement(ParserElement.Kind.Properties)
    self.cur_prop_elem = ret

    ret.name = self.cur()
    self.expect_ident()

    self.newblock()

    while self.check():
      if not self.eat_indent():
        self.indent -= 1
        break

      ret.elems.append(self.prs_element())

      while self.check() and self.eat('\n'):
        pass

    return ret

  def parse(self) -> list:
    self.tokens = self.lex()
    self.index = 0

    ret = [ ]

    while self.check():
      ret.append(self.prs_properties())

    return ret