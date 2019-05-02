# -*- coding: utf-8 -*-
# @Author: orres
# @Email:  jasonjin22@gmail.com
# @Date:   2019-05-01 20:34:40
# @Last Modified by:   orres
# @Last Modified time: 2019-05-02 21:49:50
import copy
import math

# return the suffix array of the input
def getSA(T_segment):
	sorting_array = []
	sa = []
	# T_segment += '$'
	for i in range(len(T_segment)):
		suffix = T_segment[i:] + T_segment[0:i]
		sorting_array.append((suffix, i))
	sorting_array.sort()
	sa = [x[1] for x in sorting_array]
	return sa

# return the inverse of the suffix array of the input suffix array
def get_inv_SA(sa):
	sa_length = len(sa)
	inv_sa = []
	for i in range(sa_length):
		inv_sa.append(0)
	for i in range(sa_length):
		inv_sa[sa[i]] = i
	return inv_sa

# return compressed suffix array psi from sa and inv_sa
# where psi[i] = inv_sa[sa[i] + 1] (i = 1 ... end)
def get_psi(sa, inv_sa):
	psi_length = len(sa)
	psi = []
	for i in range(psi_length):
		psi.append(0)
	psi[0] = inv_sa[0]
	for i in range(1, psi_length):
		psi[i] = inv_sa[sa[i] + 1]
	return psi

def brute_force_init_phi(T_segment):
	# maybe can be improved with Larsson and Sadakane
	sa = getSA(T_segment)
	inv_sa = get_inv_SA(sa)
	psi = get_psi(sa, inv_sa)
	return psi


# the rank of A_k among LS(A) when only the first l characters are considered
# in the paper, it is said use Larsson and Sadakane, required time is O(l*logl)
############################################
################ TO IMPROVE ################
############################################
def get_P(seg, new_seg):
	two_chunks = new_seg + seg
	P = []
	l = len(seg)
	sorting_array = []
	for i in range(l):
		P.append(0)
	for i in range(l):
		sorting_array.append((two_chunks[i:i + l], i))
	# print(sorting_array)
	sorting_array.sort()
	# print(sorting_array)
	for i in range(l):
		P[sorting_array[i][1]] = i
	return P

def get_Q(psi_b, k):
	Q = []
	for i in range(k):
		Q.append(0)
	index = psi_b[0]
	for i in range(k):
		Q[i] = index
		index = psi_b[index]
	return Q

# compute M which stores the rank of long suffixes among themselves
def get_M(psi_b, seg, new_seg):
	# P stores the rank of A_k among LS(A) when only first l characters are considered
	M = []
	l = len(seg)
	for i in range(l):
		M.append(0)
	P = get_P(seg, new_seg)
	# Q stores the rank of B_k among S(B), which equals to invSA_b[k]
	Q = get_Q(psi_b, l)
	sorting_array = []
	for i in range(l):
		sorting_array.append((P[i], Q[i], i))
	sorting_array.sort()
	for i in range(l):
		M[sorting_array[i][2]] = i
	return M

# return the number of suffixes of B starting with 'c'
def gamma(B, c):
	result = 0
	for i in B:
		if i == c:
			result += 1
	return result

# return the number of suffixes of B whose start char is smaller than 'c'
def alpha(B, c):
	result = 0
	for i in B:
		if i < c:
			result +=1
	return result

# compute L which stores the rank of long suffixes among S(B)
############################################
################ TO IMPROVE ################
############################################
def get_L(B, psi_b, seg, new_seg):
	L = []
	l = len(new_seg)
	L = [0 for i in range(l)]
	k = l - 1
	# print('psi_b',psi_b)
	while k >= 0:
		c = new_seg[k]
		RBc = range(alpha(B, c), alpha(B, c) + gamma(B, c))
		# print('c:', c, 'RBc', RBc)
		# print(B, c)
		if (k == l - 1):
			benchmark = psi_b[0]
		else:
			benchmark = L[k + 1]
		# print('benchmark:', benchmark)
		tem = []
		for i in RBc:
			if psi_b[i] < benchmark:
				tem.append(i)
		if (len(tem) == 0):
			# does not exist the r
			# print('alpha')
			L[k] = alpha(B, c)
		else:
			# print('max')
			L[k] = max(tem) + 1
		k -= 1
	return L

def rank0(V, i):
	result = 0
	j = 0
	while j < i:
		if (V[j] == 0):
			result += 1
		j += 1
	return result

def rank1(V, i):
	result = 0
	j = 0
	while j < i:
		if (V[j] == 1):
			result += 1
		j += 1
	return result

def select0(V, j):
	c = copy.deepcopy(j)
	for i in range(len(V)):
		if (V[i] == 0):
			c -= 1
		if (c == 0):
			return i

# compute psi_a from psi_b
# psi_b is the computed psi from last iteration
def next_psi(psi_b, seg, new_seg, B):
	l = len(new_seg)
	m = len(new_seg + B)
	# M stores the rank of long suffixes among themselves
	M = get_M(psi_b, seg, new_seg)
	# L stores the rank of long suffixes among S(B)
	L = get_L(B, psi_b, seg, new_seg)
	inv_M = [0 for i in range(len(M))]
	# print('M', M)
	# print('L', L)
	for i in range(len(M)):
		inv_M[M[i]] = i
	# print('inv_M', inv_M)

	V = [0 for i in range(m)]
	sorted_L = sorted(L)
	V_index = 0
	for i in range(sorted_L[0]):
		V[V_index] = 0
		V_index += 1
	V[V_index] = 1
	V_index += 1
	for i in range(l - 1):
		for j in range(sorted_L[i+1] - sorted_L[i]):
			V[V_index] = 0
			V_index += 1
		V[V_index] = 1
		V_index += 1
	while V_index < (len(V) - 1):
		V[V_index] = 0
		V_index += 1
	psi_a = [0 for i in range(m)]
	psi_a[0] = M[0] + L[0]
	for r in range(1, m):
		if V[r] == 0:
			r_prime = rank0(V, r)
			p = psi_b[r_prime]
			psi_a[r] = select0(V, p + 1)
		else:
			r_prime = rank1(V, r)
			k = inv_M[r_prime]
			if k < l - 1:
				psi_a[r] = M[k + 1] + L[k + 1]
			else:
				p = psi_b[0]
				psi_a[r] = select0(V, p + 1)
	return psi_a
def test(input):
	T = input
	B = ''

	seg_length = 4
	seg = T[len(T) - seg_length:]
	B = seg + B
	pre_psi = brute_force_init_phi(seg)
	psi_a = pre_psi
	iteration = int(len(T) / seg_length)
	for i in range(1, iteration):
		new_seg = T[len(T) - (i+1)*seg_length : len(T) - (i)*seg_length]
		psi_a = next_psi(pre_psi, seg, new_seg, B)
		pre_psi = psi_a
		seg = new_seg
		B = seg + B
	a = psi_a
	b = brute_force_init_phi(T)
	print(a == b)

def main():
	# T = 'assaqweqacg$'
	T = '''CCTAACCCTAACCCTAACCCTAACCCCAACC$'''
	# test(T)
	B = ''
	seg_length = 4
	seg = T[len(T) - seg_length:]
	B = seg + B
	pre_psi = brute_force_init_phi(seg)
	psi_a = pre_psi
	iteration = int(len(T) / seg_length)
	for i in range(1, iteration):
		# print('######################')
		new_seg = T[len(T) - (i+1)*seg_length : len(T) - (i)*seg_length]
		psi_a = next_psi(pre_psi, seg, new_seg, B)
		pre_psi = psi_a
		seg = new_seg
		B = seg + B
	print(psi_a)
	b = brute_force_init_phi(T)
	print(brute_force_init_phi(T))
	print(psi_a == b)
	




if __name__ == '__main__':
	main()

