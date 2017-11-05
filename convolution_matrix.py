#!/usr/bin/python
from __future__ import division,print_function

import sys


if __name__ == '__main__':
	
	count=int(sys.argv[1])
	
	if count % 2 != 1:
		raise ValueError("Vector length must be an odd number")
	
	base=[0]*count
	currVal=1
	for i in range(0,count//2+1):
		base[i]=currVal
		base[-(i+1)]=currVal
		currVal+=1
		
	print("Convolution vector:")
	print(str(base))
	
	cMat=[0]*count*count
	
	for a in range(count):
		for b in range(count):
			ind=a*count + b
			
			cMat[ind]=base[a]*base[b]
	
	print("convolution matrix:")
	for i in range(count):
		print(str(cMat[i*count:(i+1)*count]))
		
	tot=0
	for v in cMat:
		tot+=v
		
	print("Normalize: ",str(tot))
	
	print("C Declaration:")
	outFormat=',{0}./{1}.'
	sys.stdout.write('{'+outFormat.format(base[0],tot)[1:])
	for b in base[1:]:
		sys.stdout.write(outFormat.format(b,tot))
	sys.stdout.write('};\n')
	