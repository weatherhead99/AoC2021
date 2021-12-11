#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Dec 11 05:25:09 2021

@author: danw
"""

import numpy as np
from scipy.ndimage import binary_dilation, generate_binary_structure, generic_filter, convolve

def load_grid(fname: str) -> np.ndarray:
    with open(fname, "r") as f:
        dat = [ [int(_) for _ in line.strip()] for line in f.readlines()]
    return np.array(dat, dtype=np.uint8)

def n_neighbour_flashes(flashmap: np.ndarray):
    w = np.array([[1,1,1],
                  [1,0,1],
                  [1,1,1]], dtype=np.uint8)
    n = convolve(flashmap.astype(np.uint8), w, mode="constant", cval=0.0)
    return n


def step_grid(grid: np.ndarray) -> np.ndarray:
    #first increase all energy levels by 1
    all_flashes = np.zeros_like(grid)
    
    grid += 1
    
    flashes = grid > 9
    all_flashes = np.logical_or(all_flashes, flashes)
    #cascade flashes for any octopus above 9
    while np.any(flashes):
        #get n_neighbour_flashes
        neighflash = n_neighbour_flashes(flashes)
        #increase neighbours power level by 1
        grid += neighflash
        
        flashes = (grid * np.logical_not(all_flashes)) > 9
        all_flashes = np.logical_or(flashes,all_flashes)
    
    grid[all_flashes] = 0
    
    return all_flashes

def count_total_flashes(grid: np.ndarray, nsteps: int) -> int:
    total_flashes = 0
    for i in range(nsteps):
        flashes = step_grid(grid)
        total_flashes += np.count_nonzero(flashes)
    return total_flashes

def find_all_flash_step(grid: np.ndarray) -> int:
    step = 0
    while True:
        flashes = step_grid(grid)
        step +=1
        if np.all(flashes):
            break
    return step


if __name__  == "__main__":
    grid = load_grid("input.txt")
    
    grid_part1 = grid.copy()
    total_flashes = count_total_flashes(grid_part1, 100)
    print("total flashes: %d" % total_flashes)
    
    grid_part2 = grid.copy()
    all_flash_step = find_all_flash_step(grid_part2)
    print("all flash step: %d" % all_flash_step)