# -*- coding: utf-8 -*-
# @Author: orres
# @Email:  jasonjin22@gmail.com
# @Date:   2019-04-24 14:09:41
# @Last Modified by:   orres
# @Last Modified time: 2019-04-27 00:24:35
import intervals as interval
import copy

def get_BWT(X):
	bwt = []
	for i in range(len(X)):
		bwt.append(X)
		X = X[1:] + X[0]
		bwt.sort()
	return bwt

def get_B(bwt):
	b = ''
	for i in bwt:
		b += i[-1]
	return b

def is_subsequence(seq, X):
	w = ''
	r1 = []
	r2 = []
	if (len(seq) == 0):
		return True
	for i in range(len(seq)+1):
		r1.append(0)
		r2.append(0)
		w = seq[len(seq) - 1 - i] + w
	r1[0] = 1
	r2[0] = len(X) - 1
	for i in range(1, len(seq)+1):
		a = seq[len(seq) - i]
		r1[i] = c(a) + o(a, r1[i-1]-1) + 1
		r2[i] = c(a) + o(a, r2[i-1])
	return r1[-1] <= r2[-1]

def calculateD(W, X, b):
	z = 0
	j = 0
	D = []
	for i in range(len(W)):
		if not is_subsequence(W[j:i+1], X):
			z = z + 1
			j = i + 1
		D.append(z)
	return D

def my_union(tuple1, tuple2):
	if (tuple1[0] < tuple2[0]):
		if (tuple1[1] < tuple2[1]):
			return (tuple1, tuple2)
def c(a):
	result = 0
	for i in X[:-1]:
		if (i < a):
			result += 1
	return result

def o(a, i):
	if (i == 0):
		return 0
	result = 0
	for s in b[:i+1]:
		if (a == s):
			result += 1
	return result

def inex_recur(W, i, z, k, l):
	if z < D[i]:
		return interval.empty()
	if i < 0:
		return interval.closed(k, l)
	I = interval.empty()
	# suppose W[i] does not match
	I = I | inex_recur(W, i - 1, z - 1, k, l)
	my_k = copy.deepcopy(k)
	my_l = copy.deepcopy(l)
	for s in ['A', 'C', 'G', 'T']:
		k = c(s) + o(s, my_k - 1) + 1
		l = c(s) + o(s, my_l)
		if (k <= l):
			I = I | inex_recur(W, i, z - 1, k, l)
			if (s == W[i]):
				I = I | inex_recur(W, i - 1, z, k, l)
			else:
				I = I | inex_recur(W, i - 1, z - 1, k, l)
	return I

def calculateC(X):
	result = {}
	result['A'] = c('A')
	result['C'] = c('C')
	result['G'] = c('G')
	result['T'] = c('T')
	return result

def print_indexedBWT(bwt):
	index = 0
	for i in bwt:
		print(index, i)
		index += 1


if __name__ == '__main__':
	file = open("ref.txt")
	X = ''
	line = file.readline()
	while (line):
		line = file.readline()
		X += line[:-1]
	seq = 'AACGCAGCTCCGCCCTCGCGGTGCTCTCCGGGTCTGTGCTGAGGAGAACGCAACTCCGCC'
	X += '$'
	bwt = get_BWT(X)
	b = get_B(bwt)
	D = calculateD(seq, X, b)
	C = calculateC(X)
	z = 2
	# print(b)
	# print_indexedBWT(bwt)
	result = inex_recur(seq, len(seq) - 1, z, 1, len(X) - 1)
	print(result)
	for i in result:
		for j in range(i.lower, i.upper + 1):
			print(bwt[j][:len(seq)])
	

	