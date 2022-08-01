import re
from enum import Enum

class Node:
  class Kind(Enum):
    Item        = 0
    Let         = 1
    Flag        = 2
    Properties  = 3

  def __init__(self, kind: Kind):
    self.kind = kind
    self.name = ''
    self.value = 0

  def __repr__(self):
    if self.kind == Node.Kind.Item:
      return f'(Item {self.name})'
    elif self.kind == Node.Kind.Let:
      return f'(let-node {self.name} = {self.value})'
    elif self.kind == Node.Kind.Flag:
      return f'(flag-node .{self.name} = {self.value})'
    elif self.kind == Node.Kind.Properties:
      return f'(properties-node {self.name} {{ {" ".join([str(n) for n in self.value])} }})'

    return ''

class Parser:
  class ParseError(Exception):
    pass

  def __init__(self, data: str):
    self.data = data
    self.position = 0
    self.proplist = [ ]

  def check(self) -> bool:
    return self.position < len(self.data)

  def pass_space(self):
    while self.check() and self.peek().isspace():
      self.position += 1

  def peek(self) -> str:
    return self.data[self.position]

  def eat(self, s: str) -> bool:
    self.pass_space()

    if self.position + len(s) <= len(self.data) and self.data[self.position: self.position + len(s)] == s:
      self.position += len(s)
      return True

    return False

  def expect(self, s: str):
    if not self.eat(s):
      raise Parser.ParseError()

  def ident(self) -> str:
    s = ''

    self.pass_space()

    while self.check() and (c := self.peek()).isidentifier():
      s += c
      self.position += 1

    return s

  # dont include new line character '\n'
  def read_to_endline(self) -> str:
    s = ''

    self.pass_space()

    while self.check() and (c := self.peek()) != '\n':
      s += c
      self.position += 1
    
    return s

  def expect_ident(self) -> str:
    s = self.ident()

    if s == '':
      raise Parser.ParseError()

    return s

  def prs_item(self) -> Node:
    x = Node(Node.Kind.Let)

    if self.eat('.'):
      x.kind = Node.Kind.Flag
      x.name = self.expect_ident()

      self.expect('=')
      x.value = self.read_to_endline()
    elif (name := self.ident()) != '':
      x.name = name

      self.pass_space()

      if self.eat('='):
        x.kind = Node.Kind.Let
        x.value = [ ]

        items = re.split(' +', self.read_to_endline())

        for item in items:
          if item.startswith('$'):
            found = False
            prop = self.proplist[-1]

            symlist = item[1:].split('.')

            for i, sym in enumerate(symlist[:-1]):
              for ix in prop.value:
                if ix.kind == Node.Kind.Properties and ix.name == sym:
                  prop = ix

            for ix in prop.value:
              if ix.name == symlist[-1]:
                x.value.extend(ix.value)
                found = True
                break

            if not found:
              raise Parser.ParseError()
          else:
            x.value.append(item)
        
        #x.value = ' '.join(x.value)
      elif self.eat('{'):
        x.kind = Node.Kind.Properties
        x.value = [ ]

        self.proplist.append(x)

        while True:
          x.value.append(self.prs_item())
          if self.eat('}'): break
        
        self.proplist.pop()
      else:
        x.kind = Node.Kind.Item
    else:
      raise Parser.ParseError()

    return x

  def parse(self) -> list:
    ret = [ ]

    while self.check():
      ret.append(self.prs_item())

    return ret