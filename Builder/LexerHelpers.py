import string

def trimmer(s: str) -> list:
  x = set(s).intersection(' ' + string.punctuation)
  ret = [s]

  for y in x:
    for i in range(len(ret)):
      s = ret[i]
      if (k := s.find(y)) != -1:
        l = s[:k]
        r = s[k + 1:]
        xb = ret[:i]
        if l != '': xb.append(l)
        xb.append(y)
        if r != '': xb.append(r)
        xb.extend(ret[i + 1:])
        ret = xb

  return ret
