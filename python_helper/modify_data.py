# -*- coding: utf-8 -*-
# @Author: shayaan
# @Date:   2020-06-14 22:35:39
# @Last Modified by:   shayaan
# @Last Modified time: 2020-06-15 14:51:11


import pandas as pd

def findSide(x):
	if x.Side == 'SELL':
		return -1 * x.Qty
	else:
		return x.Qty

def convertToAccountPosition(df):
	x = []
	account = [0,0,0,0,0,0]
	for index,row in df.iterrows():
		account[row['Give-up Ref']-1] += row.Qty
		x.append(account[:])

	return x


if __name__ == '__main__':
	df = pd.read_csv('SAUMA_FOUR_20091119_default.csv')
	df.Qty = df.apply(findSide,axis=1)
	x = convertToAccountPosition(df)
	df_ = pd.DataFrame(data=x,columns=['Account1','Account2','Account3','Account4','Account5','Account6'])
	df_.to_csv('matlab_position.csv',index=False)