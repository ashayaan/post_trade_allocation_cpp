# -*- coding: utf-8 -*-
# @Author: shayaan
# @Date:   2020-06-16 12:00:39
# @Last Modified by:   shayaan
# @Last Modified time: 2020-06-18 23:55:27
import pandas as pd
import numpy as np


if __name__ == '__main__':
	py = open('temp.txt')
	matlab = open('position.txt')
	c = []
	m = []
	for l in py:
		temp = l[1:-2].split()
		temp = np.array(temp,dtype=np.float64)
		c.append(list(temp))

	for l in matlab:
		temp = l.strip('\n').split(',')
		temp = np.array(temp,dtype=np.float64)
		m.append(list(temp))

	for i,j in zip(c,m):
		print(i==j)