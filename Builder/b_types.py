from email.parser import Parser
from enum import Enum
import re

class ErrorInfo(BaseException):
  def __init__(self, msg: str):
    self.msg = msg

class ParserElement:
  class Kind(Enum):
    Item = 0
    Flag = 1
    Properties = 2

  def __init__(self, kind: Kind):
    self.kind = kind
    self.name = ''
    self.value = ''
    self.elems = [ ]

  def find(self, name: str):
    for i in self.elems:
      if i.name == name:
        return i

    return 0
  
  def to_str(self) -> str:
    if self.kind == ParserElement.Kind.Item:
      return f'{{Item: {self.name}{" = " + self.value if self.value != "" else ""}}}'
    elif self.kind == ParserElement.Kind.Flag:
      return f'{{Flag: {self.name}}}'
    elif self.kind == ParserElement.Kind.Properties:
      s = ' '.join([i.to_str() for i in self.elems])
      return f'{{Properties: {self.name}: {s}}}'

    return ''

class BuilderContext:
  def __init__(self):
    self.variables = { }
    self.flags = { }

    self.folders = {
      'includes': [ ],
      'sources': [ ]
    }

    self.output = {
      'binary': 'a.out',
      'objects': 'build'
    }

    self.options = {
      'sources': {
        'use_glob': False,
        'no_overwrite': False
      }
    }

  def get_source_files(self) -> list:
    return [ ]
