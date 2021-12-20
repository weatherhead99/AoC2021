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
from scipy.linalg import svd

import matplotlib.pyplot as plt


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

        neighdist, neighind = tree1.query(c,k=n_neighbours, p=1)
        for c2 in coords2:
            if tuple(c2) in registry.values():
                continue
            nd2, ni2 = tree2.query(c2, k=n_neighbours, p=1)
            if np.all(neighdist == nd2):
                registry[tuple(c)] = tuple(c2)
    return registry


def get_rotation_translation(registered_points, pr=False):
    pts_from = np.vstack(list(registered_points.keys()))
    pts_to = np.vstack(list(registered_points.values()))

    cent_from = np.matrix((np.sum(pts_from, axis=0) / pts_from.shape[0]))
    cent_to = np.matrix((np.sum(pts_to, axis=0) / pts_to.shape[0]))

    if(pr):
        print(cent_from)
        print(cent_to)

    #find rotation via SVD
    R, err = Rotation.align_vectors(pts_from - cent_from, pts_to-cent_to)
    print("registration error: %2.2f" % err)

    #find translation
    t = cent_to - R.apply(cent_from)

    if(pr):
        print(R.as_matrix())
        print(err)
        print(t)
    return R, t


if __name__ == "__main__":
    scanners = read_scanners("example_input.txt")

    all_beacons = [tuple(_) for _ in scanners[0]]


    for ind,scanner in enumerate(scanners[1:]):
        print("scanner: %d" % (ind + 1))
        registry = register_points(scanner, all_beacons, 4)
        print("registry length: %d" % len(registry))
        if len(registry) > 0:
            print(registry)
            R, t = get_rotation_translation(registry)
            translated_beacons = np.round(R.apply(scanner) + t,0).astype(np.int64)
            print("translation: %s" % t)
            for tb in translated_beacons:
                tbtup = tuple(np.ravel(tb))
                if tbtup not in all_beacons:
                    all_beacons.append(tbtup)
            print("all beacons length: %d" % len(all_beacons))