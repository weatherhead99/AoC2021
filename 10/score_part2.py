import numpy as np

with open("completions_part2.txt") as f:
    comps = [_.strip() for _ in f.readlines()]


def score_string(s: str) -> int:
    sm = 0
    adddct = { ")" : 1,
               "]" : 2,
               "}" : 3,
               ">" : 4}
    for c in s:
        sm = (sm*5) + adddct[c]
    return sm
    
scores = []

for c in comps:
    score = score_string(c)
    scores.append(score)
#    print("completion: %s, score: %d" % (c, score))
    

scores = sorted(scores)
med = np.median(scores)
print(scores)
print("median: %d" % med)
