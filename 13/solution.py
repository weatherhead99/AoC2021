#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 13 05:42:38 2021

@author: danw
"""

import numpy as np
from typing import Sequence

def parse_instructions(fname: str):
    dot_coords = []
    fold_instrs = []
    reached_folds : bool = False
    with open(fname, "r") as f:
        for line in f.readlines():
            if len(line.strip()) == 0:
                reached_folds = True
                continue
            if reached_folds:
                instr = line.strip().split("fold along ")[1]
                direction, num = instr.split("=")
                if direction not in ["x", "y"]:
                    print("ERROR: direction %s" % direction)
                    raise ValueError("invalid direction found")
                
                axis = 0 if direction == "x" else 1
                num = int(num)                
                fold_instrs.append((axis, num))
            else:
                dot_coords.append(tuple(int(_) for _ in line.strip().split(",")))
                
    return dot_coords, fold_instrs


def generate_array(coords: Sequence[Sequence[int]]) -> np.ndarray:
    xmax = max(_[0] for _ in coords)
    ymax = max(_[1] for _ in coords)
    out = np.zeros((xmax+1, ymax+1), dtype=bool)
    
    for coord in coords:
        out[coord[0], coord[1]] = True    
    return out

def print_arr(grid: np.ndarray, **kwargs):
    return np.array2string(grid, formatter={"bool" : lambda b: '#' if b else '.'}, **kwargs)
    

def reflect_collapse(arr: np.ndarray, direction: int, location: int) -> np.ndarray:
    arr1, arr2 = np.split(arr, [location], axis=direction)
    
    #array2 contains the extra row
    allslc = slice(None)
    cutslc = slice(1,None,None)
    
    if direction == 0:
        arr2 = arr2[cutslc,allslc][::-1, :]
    else:
        arr2 = arr2[allslc, cutslc][:,::-1]
    
    return arr1 + arr2
    

def reflect_collapse2(arr: np.ndarray, direction: int, location: int) -> np.ndarray:
    arr1, foldline,  arr2 = np.split(arr, [location, location+1], axis=direction)
    
    if direction == 0:
        arr2 = arr2[::-1, :]
    else:
        arr2 = arr2[:,::-1]
        
    padwidth = abs(arr1.shape[direction] - arr2.shape[direction])
    padarr = [[0,0], [0,0]]
    padarr[direction] = [padwidth,0]
    
    arr_to_pad = "arr1"  if arr.shape[direction] < arr2.shape[direction] else "arr2"
    padded_arr = np.pad(locals()[arr_to_pad], padarr)
    
    if arr_to_pad == "arr1":
        arr1 = padded_arr
    else:
        arr2 = padded_arr
    
    assert(arr1.shape == arr2.shape)
    return arr1 + arr2

def part1(fname: str):
    coords, instrs = parse_instructions("input.txt")
    grid = generate_array(coords)
    print(print_arr(grid))
            
    anext = reflect_collapse2(grid, *instrs[0])
    print("first fold...")
    print(print_arr(anext))
    
    anext2 = reflect_collapse2(anext, *instrs[1])
    print("second fold...")
    print(print_arr(anext2))
    
    folds_visible_step1 = np.count_nonzero(anext)
    print("dots visible after first fold: %d" % folds_visible_step1)    


if __name__  == "__main__":
    coords, instrs = parse_instructions("input.txt")
    grid = generate_array(coords)
    for instr in instrs:
        grid = reflect_collapse2(grid, *instr)
        
    to_write = print_arr(grid.T, max_line_width=200)
    with open("output.txt", "w") as f:
        f.write(to_write)
        f.write("\n")
    
    
    
    
    
    
    
    
    
    