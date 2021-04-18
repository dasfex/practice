def knut_moris_pratt(hs, nd) -> int:
  ind = -1
  for i in range(len(hs) - len(nd) + 1):
      good = True
      for j in range(len(nd)):
          if nd[j] != hs[i+j]:
              good = False
              break
      if good:
          ind = i
          break
  return ind
