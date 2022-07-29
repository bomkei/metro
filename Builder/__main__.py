import os
import sys
from b_parser import BuildpyParser
from b_types import *
from builder import BuildPy

argv = sys.argv

if len(argv) <= 1:
  print('no input files')
  os._exit(1)

def main():
  try:
    if len(argv) == 1:
      print('no input file')
      return

    builder = BuildPy(argv[1])

    builder.parse()

    builder.init_ctx()

    builder.build()

  except FileNotFoundError as e:
    print(f'cannot open file \'{e.filename}\'')
  #except:
  #  print('oh,, unhandled exception.')


if __name__ == '__main__':
  main()