#!/usr/bin/python

import sys

def f(i, j):
	return 1.0 / (i + j + 1)

def g(i, j):
	return 1 + max(i, j)

def main():
	if len(sys.argv) < 2:
		print "Matrix size must be specified"
		return 1
	current_function = g # Change to change generation formula
	size = int(sys.argv[1])
	print size
	for i in range(size):
		free_term = 0
		for j in range(size):
			print "%f\t" % (current_function(i, j), ),
			free_term = free_term + current_function(i, j)
		print "%f" % (free_term, )

if __name__ == '__main__':
	main()
