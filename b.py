import os
import glob
import re

def cmd(s):
  print(s)
  return os.system(s)

srcfiles = glob.glob('src/**/*.cc', recursive=True)

cmd(
  f'clang++ -std=c++20 -Wno-switch -Wimplicit-fallthrough '
    + '-DMETRO_DEBUG '
    + f'-g -O0 -Iinclude -Wl,--gc-sections -pthread '
    + f'{" ".join(srcfiles)} -o metro'
)

