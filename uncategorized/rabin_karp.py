def rabin_karp(hs, nd):
    h = hash(nd)
    for i in range(len(hs) - len(nd)):
        hss = h 
        if i > 0:
          hss -= hash(hs[i - 1])
          hss += hash(hs[i + len(nd) - 1])
        if hss == h and hs[i : i + len(nd)] == nd:
            return i
    return -1
  
