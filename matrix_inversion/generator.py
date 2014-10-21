#!/usr/bin/python

import sys

def f(i, j):
	return 1.0 / (i + j + 1)

def main():
	if len(sys.argv) < 2:
		print "Matrix size must be specified"
		return 1
	size = int(sys.argv[1])
	print size
	for i in range(size):
		for j in range(size):
			print f(i, j),
		print

if __name__ == '__main__':
	main()
