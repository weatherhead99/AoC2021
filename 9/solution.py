#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np
from scipy import ndimage

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


if __name__ == "__main__":
    
    example_grid = load_grid("example_input.txt")
    
    local_minima = find_minima(example_grid)
    risk_level = np.sum(local_minima * (example_grid + 1))
    print("example risk level: %d" % risk_level)
    
    grid = load_grid("input.txt")
    local_minima2 = find_minima(grid)
    risk_level2 = np.sum(local_minima2 * (grid+1))
    print("part 1 risk level: %d" % risk_level2)
    
    
    
    
    
    
    
    
