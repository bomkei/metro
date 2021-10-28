import glob

files = []
total = 0

files.extend(glob.glob("*.cc"))
files.extend(glob.glob("*.h"))

for i in files:
  with open(i) as file:
    s = file.readlines()
    print(f"{i}: {len(s)}")
    total += len(s)

print(f"\ntotal: {total}")

