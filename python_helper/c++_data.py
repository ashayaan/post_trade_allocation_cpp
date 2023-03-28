# -*- coding: utf-8 -*-
# @Author: shayaan
# @Date:   2020-06-15 10:03:00
# @Last Modified by:   shayaan
# @Last Modified time: 2020-06-15 10:07:05

import pandas as pd
import numpy as np



if __name__ == '__main__':
	f = open('out.txt')
	x = []
	for l in f:
		temp = np.array(l.split()[1:], dtype=np.int64)
		x.append(temp)

	df = pd.DataFrame(data=x,columns=['Account1','Account2','Account3','Account4','Account5','Account6'])
	df.to_csv('C++_position.csv',index=False)