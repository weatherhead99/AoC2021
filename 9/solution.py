#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np
from scipy import ndimage
from functools import reduce


def load_grid(fname: str):
    with open(fname, "r") as f:
        grid = [[int(_) for _ in line.strip()]  for line in f.readlines() ]
    
    return np.array(grid, dtype=np.int8)


def find_minima(grid: np.ndarray) -> np.ndarray:
    fprint_neighbours = np.array([[0,1,0],
                                  [1,0,1],
                                  [0,1,0]], dtype=bool)
    filtered = ndimage.minimum_filter(grid, footprint=fprint_neighbours,
                                      mode="constant", cval=9)
    return grid < filtered

def region_sizes_naive(grid: np.ndarray):
    thresh = grid < 9
    labelled_thresh, nregions = ndimage.label(thresh)
    szes = []
    for i in range(1,nregions+1):
        sz = np.count_nonzero(labelled_thresh == i)
        szes.append(sz)
    return list(reversed(sorted(szes)))



if __name__ == "__main__":
    
    example_grid = load_grid("example_input.txt")
    
    local_minima = find_minima(example_grid)
    risk_level = np.sum(local_minima * (example_grid + 1))
    print("example risk level: %d" % risk_level)
    
    grid = load_grid("input.txt")
    local_minima2 = find_minima(grid)
    risk_level2 = np.sum(local_minima2 * (grid+1))
    print("part 1 risk level: %d" % risk_level2)
    
    region_sizes_example = region_sizes_naive(example_grid)
    
    answer_part2_example = reduce(lambda a, b: a*b, region_sizes_example[:3])
    print("answer part 2 example: %d" % answer_part2_example)
    
    region_sizes = region_sizes_naive(grid)
    answer_part2 = reduce(lambda a,b : a*b, region_sizes[:3])
    print("answer part 2: %d" % answer_part2)
    
    
    
    
    
    
    
    
