#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec 19 20:12:43 2021

@author: danw
"""

import numpy as np
from typing import List
from scipy.spatial.transform import Rotation
from itertools import combinations
from scipy.spatial import KDTree

def read_scanners(fname: str):
    scanners = []
    thisscanner = []
    with open(fname, "r") as f:
        for line in  f:
            if len(line.strip()) == 0:
                continue
            if "---" in line:
                scanners.append(thisscanner.copy())
                thisscanner.clear()
                continue
            coords = [int(_) for _ in line.strip().split(",")]
            thisscanner.append(coords)
    scanners.append(thisscanner.copy())
    return scanners[1:]

def centroid_scanner(scanner: List[List[int]]):
    out = np.array(scanner)
    centroid = np.sum(out, axis=0) / out.shape[0]
    return out-centroid, centroid


def register_points(coords1, coords2, n_neighbours):
    tree1 = KDTree(coords1)
    tree2 = KDTree(coords2)
    registry = {}

    for c in coords1:
        if tuple(c) in registry:
            continue

        neighdist, neighind = tree1.query(c,k=n_neighbours, p=2)
        for c2 in coords2:
            if tuple(c2) in registry.values():
                continue
            nd2, ni2 = tree2.query(c2, k=n_neighbours, p=2)
            if np.all(neighdist == nd2):
                registry[tuple(c)] = tuple(c2)
    return registry




if __name__ == "__main__":
    scanners = read_scanners("example_input.txt")

    centroids = []
    cent_coords = []
    for sc in scanners:
        coords, cent = centroid_scanner(sc)
        centroids.append(cent)
        cent_coords.append(coords)

    registry = register_points(cent_coords[1], cent_coords[0], 2)
    toalign1 = np.vstack(list(registry.keys()))
    toalign2 = np.vstack(list(registry.values()))

    r, err = Rotation.align_vectors(toalign1, toalign2)