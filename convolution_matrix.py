#!/usr/bin/python
from __future__ import division,print_function

import sys

def pascalPass(count,theVals):

	if count==0:
		return
	
	pascalPass(count-1,theVals)
	
	theVals[0]=1
	prev=1
	for i in range(1,count-1):
		oldVal=theVals[i]
		theVals[i]+=prev
		prev=oldVal
	theVals[count-1]=1


if __name__ == '__main__':
	
	count=int(sys.argv[1])
	
	if count % 2 != 1:
		raise ValueError("Vector length must be an odd number")
	
	base=[0]*count
	pascalPass(count,base)
		
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
		
	norm=tot**0.5
	print("Normalize: ",str(tot))
	print("  for vector: ",str(norm))
	for i in range(count):
		print(str([x/tot for x in cMat[i*count:(i+1)*count]]))
	
	
	print("C Declaration:")
	outFormat=',{0}./{1}'
	sys.stdout.write('{'+outFormat.format(base[0],norm)[1:])
	for b in base[1:]:
		sys.stdout.write(outFormat.format(b,norm))
	sys.stdout.write('};\n')
	