import os
import sys
from b_parser import BuildpyParser
from b_types import *

class BuildPy:
  def __init__(self, path: str):
    with open(path, 'r', encoding='utf-8') as fs:
      self.source = fs.readlines()

    self.context = BuilderContext()
    self.parser = BuildpyParser(self.source)
    self.parsed_elements = 0

  def parse(self):
    self.parsed_elements = self.parser.parse()

  def init_ctx(self):
    for elem in self.parsed_elements:
      if elem.name == 'includes':
        for i in elem.elems:
          if i.kind == ParserElement.Kind.Item:
            if i.value != '':
              print(f'warn: includes.{i.name}: value is not be used')
            self.context.folders['includes'].append(i.name)
      elif elem.name == 'sources':
        for i in elem.elems:
          if i.kind == ParserElement.Kind.Flag:
            if not i.name in self.context.options['sources'].keys():
              print(f'warn: \'{i.name}\' doesn\'t exists in options of \'sources\'')
            else:
              self.context.options['sources'][i.name] = True

  def compile(self, file):
    pass

  def build(self) -> bool:
    pass