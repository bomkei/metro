import os
import glob
from collections import defaultdict
from colors import *

def check_duplication(path: str) -> list:
  if path[-1] == '/':
    path += '**/*'
  else:
    path += '/**/*'

  all_files = glob.glob(path, recursive=True)
  dictvec = defaultdict(list)
  ret = [ ]

  for file in all_files:
    s = os.path.basename(file)

    dictvec[s].append(file)

  for k in dictvec.keys():
    if len(dictvec[k]) > 1:
      ret.append(dictvec[k])

  return ret

def com_check_duplicate(folders: list):
  for folder in folders:
    print(f'{COL_CYAN}check filename duplication in {COL_GREEN}"{folder}" {COL_CYAN}...{COL_DEFAULT}')

    if not os.path.isdir(folder):
      print(f'{COL_MAGENTA}folder not found')
      continue

    res = check_duplication(folder)

    if len(res) != 0:
      print(COL_RED + 'file name duplicated in folder ' + COL_YELLOW + '"' + folder + COL_DEFAULT + \
        '":\n  ' + '\n'.join(['\n  '.join(v) for v in res]))
    else:
      print('no duplcicated files.')