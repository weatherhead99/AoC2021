#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Dec  4 18:58:03 2021

@author: danw
"""


import numpy as np

class BingoBoard:
    def __init__(self, numbers: np.ndarray):
        self.board = numbers
        self.mask = np.zeros((5,5), dtype=bool)
        self.winning_number = None
        self.winning_score = None
        self.winning_mask = None
        
        
    def apply_called_number(self, num: int):
        self.mask += self.board == num
        
    def reset(self):
        self.mask = np.zeros((5,5), dtype=bool)
    
    @property
    def is_winner(self):
        for row in self.mask:
            if np.sum(row) == 5:
                return True
        for column in self.mask.T:
            if np.sum(column) == 5:
                return True
        return False
    
    def score(self, num_just_called: int):
        sum_unmarked = np.sum(self.board * np.logical_not(self.mask))
        return sum_unmarked * num_just_called


def parse_input_file(fname: str):
    with open(fname, "r") as f:
        called_numbers = f.readline()
        raw_boards = f.readlines()
    
    
    cnums = [int(_) for _ in called_numbers.split(",")]
    
    boards = []
    for line in raw_boards:
        if line == "\n":
            thisboard = []
            boards.append(thisboard)
        else:
            thisboard.append([int(_) for _ in line.split()])
    
    return cnums, boards


def iterate_winning_boards(boards, numbers):
    b2 = boards.copy()
    for num in numbers:
        if len(b2) > 0:
            to_remove = []
            for board in b2:
                board.apply_called_number(num)
                if board.is_winner:
                    to_remove.append(board)
                    yield num, board
            for brd in to_remove:
                b2.remove(brd)


def run_boards_part1(boards, numbers):    
    for num, board in iterate_winning_boards(boards, numbers):    
        return num, board

def run_boards_part2(boards, numbers):
    b2 = boards.copy()
    it = iterate_winning_boards(b2, numbers)
    
    for num, board in it:
        pass
    
    return board, num
    
        

if __name__ == "__main__":
    called_numbers, raw_boards = parse_input_file("example_input.txt")
    
    boards = [BingoBoard(np.array(_,dtype=np.uint16)) for _ in raw_boards]
    
    winnum, winboard = run_boards_part1(boards, called_numbers)
    print("winning test score is: %d" % winboard.score(winnum))
    
    cnum2, rb2 = parse_input_file("input.txt")
    boards2 = [BingoBoard(np.array(_, dtype=np.uint16)) for _ in rb2]    
    winnum2, winboard2= run_boards_part1(boards2, cnum2)
    
    print("winning score is: %d" % winboard2.score(winnum2))
    
    lwb, n = run_boards_part2(boards, called_numbers)
    print("test win num part 2: %d" % n)
    print("test score part 2: %d" % lwb.score(n))
    
    lwb2, n2 = run_boards_part2(boards2, cnum2)
    
    print("score part2: %d" % lwb2.score(n2))
    
    
    
    