import os
import glob
from collections import defaultdict

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