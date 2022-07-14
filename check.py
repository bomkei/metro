import os
import glob
import collections

source = 'src'
total_lines  = 0

src_files = [ ]

for ext in ['c', 'cc']:
  src_files += glob.glob(f'{source}/**/*.{ext}', recursive=True)

# line count
for i in src_files:
  with open(i, mode='r') as f:
    ll = len(f.readlines())
    total_lines += ll
    print(f'{i}: {ll}')

print(f'total lines = {total_lines}\n')

# to lower
src_files = list(map(lambda name: os.path.basename(name).lower(), src_files))

if len(src_files) != len(list(set(src_files))):
  dup_dict = collections.Counter(src_files)
  asserted = { }

  for key in src_files:
    if dup_dict[key] > 1 and not asserted.get(key, False):
      print(f'`{key}` is duplicated: {" ".join(glob.glob(f"{source}/**/{key}", recursive=True))}')
      asserted[key] = True

  exit(1)
else:
  print('no duplicated files.')
