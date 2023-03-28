# -*- coding: utf-8 -*-
# @Author: shayaan
# @Date:   2020-06-15 10:14:49
# @Last Modified by:   shayaan
# @Last Modified time: 2020-06-15 14:55:48

import pandas as pd
import numpy as np


def compare(df1,df2,df3):
	length = min(len(df1),min(len(df2),len(df3)))
	x = []
	for i in range(length):
		temp = list(df1.iloc[i] == df2.iloc[i])
		temp.extend((df3.iloc[i]['CALENDAR DATE'],df3.iloc[i]['CALENDAR TIME'],df3.iloc[i]['LAST PRICE'],df3.iloc[i]['LAST SHARES'],df3.iloc[i]['SIDE']))
		temp.extend(list(df1.iloc[i]))
		temp.extend(list(df2.iloc[i]))	
		x.append(temp)
	c = ['Account1','Account2','Account3','Account4','Account5','Account6','date','time','price','qty','side',
		'Account1_C++','Account2_C++','Account3_C++','Account4_C++','Account5_C++','Account6_C++','Account1_matlab','Account2_matlab','Account3_matlab','Account4_matlab','Account5_matlab','Account6_matlab'] 
	df = pd.DataFrame(data=x,columns=c)
	df.to_csv('compare.csv',index=False)

if __name__ == '__main__':
	df1 = pd.read_csv('C++_position.csv')
	df2 = pd.read_csv('matlab_position.csv')
	df3 = pd.read_csv('test5.csv')
	print(len(df1))
	print(len(df2))
	print(len(df3))
	compare(df1,df2,df3)	