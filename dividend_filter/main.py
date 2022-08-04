#!/usr/bin/env python3
from cmath import isnan, nan
from numpy import NaN
from openpyxl import Workbook
import pandas as pd
import operator

def filter(param, value, op, do):
	global df
	global cursor
	global ops
	df.sort_values(by=param, ascending=False if op == operator.lt else True, inplace=True)
	df.reset_index(drop=True, inplace=True)
	for index, row in df.iterrows():
		if op(row[param], value):
			cursor = index
			break
	print("operation {}: deleting {} rows".format(ops, len(df) - len(df[:cursor])))
	ops += 1
	if do:
		df = df[:cursor]

def filter_condition(param, value, op, cond, cond_value, cond_op, do):
	global df
	global ops
	counter = 0
	df.sort_values(by=param, ascending=False, inplace=True)
	df.reset_index(drop=True, inplace=True)
	for index, row in df.iterrows():
		if op(row[param], value):
			if cond_op(row[cond], cond_value) == False:
				if do:
					df.drop(index, inplace= True)
				counter += 1
	print("operation {}: deleting {} rows".format(ops, counter))
	ops += 1

def find_end():
	for index, row in df.iterrows():
		if pd.isnull(row['Yield']):
			return index

def prep_table():
	global df
	del df['Seq']
	del df['DR']
	del df['SP']
	del df['Sch']
	del df['A/D*']
	del df['DEG']
	del df['Payout']
	del df['Factor']
	del df['Rule']
	del df['Graham']
	del df["Annualized"]
	del df["Price"]
	del df["Month"]
	del df["Own."]
	del df['$']
	del df['%']
	df = df[:find_end()]
	for index, row in df.iterrows():
		if isnan(row['Equity']):
			df.iloc[index] = 0.0

if __name__ == '__main__':
	src = 'test.xlsx'
	dest = Workbook('dest.xlsx')
	cursor = 0
	ops = 0
	df = pd.read_excel(src, sheet_name='All CCC', header=5)
	writer = pd.ExcelWriter('dest.xlsx', engine = 'openpyxl')
	prep_table()

	filter('1-yr',   7.5000000,   operator.lt, True)
	filter('Yield',  2.000000,	  operator.lt, True)
	filter('10-yr',  7.5000000,   operator.lt, True)
	filter('Equity', 1.00,		  operator.gt, True)
	filter('ROE',	 10.000000,   operator.lt, True)
	filter('($Mil)', 1000.000000, operator.lt, True)
	filter('3-yr',   7.5000000,   operator.lt, True)
	filter('5-yr',   7.5000000,   operator.lt, True)
	filter_condition('Payout.1', 75.000000, operator.gt, 'Industry', 'Equity Real Estate Investment Trusts (REITs)', operator.eq, True)
	df.sort_values(by='Yield', ascending=False, inplace=True)

	df.to_excel(writer, sheet_name = 'final')
	writer.close()