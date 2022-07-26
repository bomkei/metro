from calendar import c
import os
import glob
from collections import defaultdict
from colors import *

def com_line_count(folders: list, ext: list):
  for folder in folders:
    total = 0
    path = f"./{folder}/"
    files = glob.glob(f"{path}*")
    for file in files:
      if os.path.splitext(file)[1][1:] in ext:
        count = sum([1 for _ in open(file)])
        total += count
        print(f"{os.path.basename(file)}: {count}")
    print(f"\ntotal: {total}")
  return