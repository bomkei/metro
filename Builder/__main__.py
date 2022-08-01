import os
import sys
from builder import *

# ---------------------------
#  BuildPy
# ---------------------------
def main():
  argv = sys.argv

  try:
    builder = Builder()

    return builder.run(argv)
  except FileNotFoundError as e:
    print(f'cannot open file "{e.filename}"')
#  except:
#    print('unhandled exception have been occurred')

  return 1

# ------

if __name__ == '__main__':
  main()