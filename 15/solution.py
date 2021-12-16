#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 15 11:41:57 2021

@author: danw
"""

import networkx as nx
import numpy as np

def load_grid(fname: str) -> np.ndarray:
    out = []
    with open(fname, "r") as f:
        for line in f:
            out.append([int(_) for _ in line.strip()])
    return np.array(out, dtype=np.uint8)

def construct_graph(grid: np.ndarray) -> nx.Graph:

    graph = nx.Graph()
    with np.nditer(grid, flags=["multi_index"]) as it:
        for x in it:
            graph.add_node(it.multi_index, risk=x)
            neighbours = []
            if it.multi_index[0] < grid.shape[0]-1 :
                neighbours.append( (it.multi_index[0]+1, it.multi_index[1]))
            if it.multi_index[1] < grid.shape[1]-1:
                neighbours.append( (it.multi_index[0], it.multi_index[1]+1))
            for n in neighbours:
                graph.add_edge(it.multi_index, n)
    return graph

def path_total_risk(path, graph):
    #don't count risk of first node
    total_risk = 0
    for node in path[1:]:
        total_risk += graph.nodes[node]["risk"]
    return total_risk

def expand_grid(grid: np.ndarray) -> np.ndarray:
    #expand down 5 times
    column = np.hstack( (grid, grid+1, grid+2, grid+3, grid+4))
    full_grid = np.vstack((column, column+1, column+2, column+3, column+4))
    cond = full_grid > 9
    full_grid[cond] = full_grid[cond]%9
    return full_grid

def shortest_path_find(grid, graph):
    tgt_node = (grid.shape[0]-1, grid.shape[1]-1)
    weightfun = lambda fr, to, attrs: graph.nodes[to]["risk"]
    paths = nx.shortest_simple_paths(graph, source=(0,0), target=tgt_node, weight=weightfun)
    shortest_path = next(paths)
    return shortest_path

if __name__ == "__main__":
    #grid = load_grid("example_input.txt")
    grid = load_grid("input.txt")
    graph = construct_graph(grid)
    shortest_path = shortest_path_find(grid, graph)
    print("shortest path : %s" % shortest_path)
    risk = path_total_risk(shortest_path, graph)
    print("risk of shortest path: %d" % risk)

    expanded_grid = expand_grid(grid)
    expanded_graph = construct_graph(expanded_grid)
    shortest_expanded_path = shortest_path_find(expanded_grid, expanded_graph)

    expanded_risk = path_total_risk(shortest_expanded_path, expanded_graph)
    print("risk of shortest expanded path: %d" % expanded_risk)