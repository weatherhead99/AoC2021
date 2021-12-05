#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec  5 19:02:13 2021

@author: danw
"""

import numpy as np
from math import atan2, radians, sqrt

def load_line_segments(fname: str):
    outsegs = []
    with open(fname,"r") as f:
        for line in f.readlines():
            fromtup, totup = [ _.split(",") for _ in line.split(" -> ")]
            fromtup = [int(_.strip()) for _ in fromtup]
            totup = [int(_.strip()) for _ in totup]
            outsegs.append([fromtup,  totup])

    return outsegs    

def lineseg_vert_horiz(lineseg) -> bool:
    return lineseg[0][0] == lineseg[1][0] or lineseg[0][1] == lineseg[1][1]


def get_theta(lineseg):
    return atan2(lineseg[1][1]-lineseg[0][1], lineseg[1][0] - lineseg[0][0])

def get_direction(lineseg):
    deltaY = lineseg[1][1] - lineseg[0][1]
    deltaX = lineseg[1][0] - lineseg[0][0]
    mag = sqrt(deltaY**2 + deltaX**2)
    return int(round(deltaX / mag)), int(round(deltaY / mag))

def draw_line_on_grid(grid, lineseg, limit: int = -1):
    startpos = lineseg[0][0], lineseg[0][1]
    endpos = lineseg[1][0], lineseg[1][1]
    dr = get_direction(lineseg)
    
    grid[startpos[0], startpos[1]] += 1
    curpos = startpos
    while curpos != endpos:
        curpos = int(curpos[0] + dr[0]), int(curpos[1] + dr[1])
        grid[curpos[0], curpos[1]] += 1


def process(fname, part1: bool = True):
    segs = load_line_segments(fname)
    #consider only horizontal or vertical segments
    if part1: 
        segs = [_ for _ in segs if lineseg_vert_horiz(_)]
    gridsize = np.max(segs) + 1
    grid = np.zeros((gridsize, gridsize), dtype=np.uint8)
    for line in segs:
        draw_line_on_grid(grid, line)
    
    return np.count_nonzero(grid >= 2)

if __name__ == "__main__":
    example_part1 = process("example_input.txt")
    print("part 1 example answer: %d" % example_part1)
    
    real_part1 = process("input.txt")
    print("part 1 real answer: %d" % real_part1)
#%%
    example_part2 = process("example_input.txt", False)
    print("part 2 example answer: %d" % example_part2)
    
    real_part2 = process("input.txt", False)
    print("part 2 real answer: %d" % real_part2)
    
    