#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  3 11:50:11 2021

@author: danw
"""

from typing import List

def load_numbers(fname: str):
    with open(fname, "r") as f:
        numstr = [_.strip() for _ in f.readlines()]
    return numstr


def load_numbers_transform(fname: str):
    numstrs = load_numbers(fname)
    
    outs = []
    for i in range(len(numstrs[0])):
        outs.append([int(_[i]) for _ in numstrs])
    return outs

def count_bits(num: List[int]):
    ones = sum( _ for _ in num if _ == 1)
    zeros = len(num) - ones
    return zeros, ones    

def dec_num_from_bits(bits):
    return int("".join(str(_) for _ in bits),base=2)


def get_gamma_epsilon(nums):
    bits_gamma = []
    bits_epsilon = []
    for num in nums:
        count_zeros, count_ones = count_bits(num)
        if count_zeros > count_ones:
            bits_gamma.append(0)
            bits_epsilon.append(1)
        else:
            bits_gamma.append(1)
            bits_epsilon.append(0)
    
    return dec_num_from_bits(bits_gamma), dec_num_from_bits(bits_epsilon)


def filter_numbers(numbers, criterion, bit_pos):
    bits_to_consider = [int(_[bit_pos]) for _ in numbers]
    return [n for n in numbers if criterion(bits_to_consider, bit_pos, n)]
    
def oxygen_criterion(bits, pos, n):
    nzeros, nones = count_bits(bits)
    if nzeros > nones:
        if int(n[pos]) == 0:
            return True
        return False
    else:
        if int(n[pos]) == 1:
            return True
        return False

def co2_criterion(bits, pos, n):
    return not oxygen_criterion(bits, pos, n)


def find_number(nums, criterion):
    n = nums.copy()
    for i in range(len(nums[0])):
        n = filter_numbers(n, criterion, i)
        if len(n) == 1:
            break
    else:
        raise ValueError("couldn't filter list!")
        
    
    return dec_num_from_bits(n[0])


if __name__ == "__main__":
    nums = load_numbers_transform("example_input.txt")
    
    gamma, epsilon = get_gamma_epsilon(nums)
    
    ans = gamma * epsilon
    print("test result: %d" % ans)
    
    
    nums2 = load_numbers_transform("input.txt")
    gamma, epsilon = get_gamma_epsilon(nums2)
    ans = gamma * epsilon
    print("test result 2: %d " %ans)
    
    
    nums = load_numbers("example_input.txt")
    
    oxygen_test = find_number(nums, oxygen_criterion)
    co2_test = find_number(nums, co2_criterion)
    
    result = oxygen_test* co2_test
    print("example result part 2: %d" % result)
    
    
    nums = load_numbers("input.txt")
    oxygen = find_number(nums, oxygen_criterion)
    co2 = find_number(nums, co2_criterion)
    
    result = oxygen * co2
    print("final result part 2 : %d" % result)
    
    
    
        
    
    