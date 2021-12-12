#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Dec 12 19:16:06 2021

@author: danw
"""

import networkx as nx
from enum import Enum
from itertools import chain

class CaveType(Enum):
    START_CAVE = 1
    END_CAVE = 2
    SMALL_CAVE = 3
    BIG_CAVE = 4


def get_cavetp(name: str) -> CaveType:
    if name == "start":
        return CaveType.START_CAVE
    elif name == "end":
        return CaveType.END_CAVE
    elif name.isupper():
        return CaveType.BIG_CAVE
    return CaveType.SMALL_CAVE

def load_graph(fname: str) -> nx.Graph:
    with open(fname, "r") as f:
        connections = [line.strip().split("-") for line in f.readlines()]
    
    G = nx.Graph()
    
    nodes = set(chain(*connections))
    for nodename in nodes:
        tp = get_cavetp(nodename)
        weight = 1 if tp == CaveType.SMALL_CAVE else 0
        G.add_node(nodename, weight=weight, type=tp)
    
    for edge in connections:
        G.add_edge(edge[0], edge[1])
    
    return G

def walk_all_paths(graph, n, previsited=[]):
    path = previsited + [n]
    for neigh in graph.neighbors(n):
        neightp = graph.nodes[neigh]["type"]
        if neigh in path and neightp == CaveType.SMALL_CAVE or neightp == CaveType.START_CAVE:
                continue
        elif neightp == CaveType.END_CAVE:
            yield path + [neigh]
            continue
        yield from walk_all_paths(graph, neigh, path)

def walk_all_paths_part2(graph, n, previsited=[], small_cave_revisited = False):
    path = previsited + [n]
    for neigh in graph.neighbors(n):
        neightp = graph.nodes[neigh]["type"]
        if neightp == CaveType.START_CAVE:
            continue
        if neigh in path and neightp == CaveType.SMALL_CAVE:
            if small_cave_revisited:
                continue
            else:
                yield from walk_all_paths_part2(graph, neigh, path, True)
                continue
        elif neightp == CaveType.END_CAVE:
            yield path + [neigh]
            continue
        yield from walk_all_paths_part2(graph, neigh, path, small_cave_revisited)


if __name__  == "__main__":
    graph = load_graph("input.txt")

    it = walk_all_paths(graph, "start")
    
    i = 0
    for path in it:
        i+=1
    print("number of paths: %d" % i)
        
    it2 = walk_all_paths_part2(graph, "start")
    i = 0
    for path in it2:
        i += 1
    print("number of paths part 2: %d" % i)
    
    
    
