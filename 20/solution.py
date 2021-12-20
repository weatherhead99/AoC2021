#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 20 20:20:01 2021

@author: danw
"""

import numpy as np
from scipy.ndimage import generic_filter
import pyqtgraph as pg

def load_problem(fname: str):
    rulestr = []
    imagestrs = []
    with open(fname, "r") as f:
        b : bool = False
        for line in f:
            if len(line.strip()) == 0:
                b = True
                continue
            if not b:
                rulestr.append(line.strip())
            else:
                imagestrs.append(line.strip())

    rulestr = "".join(rulestr)
    rulearr = [1 if _ == "#" else 0 for _ in rulestr]
    image = []
    for iis in imagestrs:
        out = [1 if _ == "#" else 0 for _ in iis]
        image.append(out)

    return rulearr, image


def filter_image(image: np.ndarray, rule=None):
    def filtfun(v):
        combnums = [_ for _ in v]
        combstr = "".join(str(int(_)) for _ in combnums)
        if len(combstr) !=9 :
            raise ValueError
        idx = int(combstr, 2)
        if rule is None:
            return idx
        return rule[idx]
    return generic_filter(image, filtfun, size=(3,3), mode="constant", cval=0)



if __name__ == "__main__":
    rule, image = load_problem("input.txt")

    image = np.array(image,dtype=np.uint16)
    print(image.shape)
    image = np.pad(image, 3)
    print(image.shape)
    image2 = filter_image(image, rule)
    image2 = np.pad(image2, 3)

    image3 = filter_image(image2, rule)



    print("lit pixels: %d"%  np.count_nonzero(image))