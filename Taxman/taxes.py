#!/usr/bin/python3

def my_is_decimal(string):
	dot_count = 0
	for i in string:
		if i == '.':
			dot_count += 1
		if not ('0' <= i <= '9' or i == '.') or dot_count > 1:
			return False
	return True

def check_line(items, thresholds, line_count):
	for i in items:
		flag = 0
		if not my_is_decimal(i):
			if not i == "inf":
				print("one of the fields in your input is not a number")
				flag = 1
				break
	if flag == 1:
		return 0
	if len(items) != 3:
		print("line {}: wrong number of items, have {} instead of 3".format(line_count, len(items)))
		return 0
	min = float(items[0])
	if min < thresholds[len(thresholds) - 1][1]:
		print("line {}: lower bracket is lower than previous upper bracket".format(line_count))
		return 0
	max = float(items[1])
	if max < min:
		print("line {}: upper bracket is lower than lower bracket".format(line_count))
		return 0
	perc = float(items[2])
	if perc < 0:
		print("line {}: negative percentage".format(line_count))
		return 0
	return (min, max, perc)

def get_thresholds():
	global exonerated
	global thresholds
	global cotisations 
	read_flag = 0
	cotisations_flag = 0
	try:
		fd = open("brackets.txt")
		line_count = 1
		while 1:
			line = fd.readline()
			if line == "":
				break
			if "[cotisations]" in line:
				cotisations_flag = 1
				line_count += 1
				continue
			if '\n' in line:
				line = line[:-1]
			items = line.split()
			if items[0] == "exonerated" and len(items) > 1 and my_is_decimal(items[1]):
				exonerated = float(items[1])
				line_count += 1
				continue
			if not cotisations_flag:
				if tmp := check_line(items, thresholds, line_count):
					thresholds.append(tmp)
			elif cotisations_flag:
				if tmp := check_line(items, cotisations, line_count):
					cotisations.append(tmp)
			else:
				read_flag = 1
				raise Exception('file syntax error')
			line_count += 1
	
	except:
		if read_flag == 1:
			print("problem in 'thresholds.taxes' ")
		else:
			print("file 'thresholds.taxes' not found")
		inpt = input("Do you want to input thresholds manually ? (y / n): ")
		while (not inpt or (inpt.upper() != "Y" and inpt.upper() != "N")):
			inpt = input("type in a 'y' for yes or 'n' for no and press enter")
		if inpt.upper() == 'N':
			print("exiting...")
			exit()
		thresholds = [(0, 0, 0)]
		cotisations = [(0, 0, 0)]
		line_count = 1
		while 1:
			inpt = input("input threshold in the following pattern:\n'min max percentage'\npress <ENTER> on an empty line to finish input\n")
			if not inpt:
				break
			items = inpt.split()
			tmp = check_line(items, thresholds, line_count)
			if tmp:
				thresholds.append(tmp)
				line_count += 1
		inpt = input("is there an exonerated amount ?: ")
		if inpt and my_is_decimal(inpt):
			exonerated = float(inpt)
		while 1:
			inpt = input("input cotisations in the following pattern:\n'min max percentage'\npress <ENTER> on an empty line to finish input\n")
			if not inpt:
				break
			items = inpt.split()
			tmp = check_line(items, cotisations, line_count)
			if tmp:
				cotisations.append(tmp)
				line_count += 1
		fd = open("brackets.txt", "w+")
		fd.write("exonerated {}\n".format(exonerated))
		for i in thresholds:
			fd.write("{} {} {}\n".format(i[0], i[1], i[2]))
		fd.write("[cotisations]")
		for i in cotisations:
			fd.write("{} {} {}\n".format(i[0], i[1], i[2]))

if __name__ == '__main__':
	exonerated = 0
	thresholds = [(0, 0, 0)]
	cotisations = [(0, 0, 0)]
	get_thresholds()
	while 1:
		gross = input('enter your gross revenue (empty to quit program): ')
		if not gross:
			print("exiting...")
			exit()
		currency = "€"
		while not my_is_decimal(gross) or float(gross) < 0:
			gross = input('enter your gross revenue (must be a positive number): ')
		gross = float(gross)
		tmp = input('enter amount of business expenses: ')
		while not tmp or not my_is_decimal(tmp) or float(tmp) < 0 or float(tmp) > gross:
			tmp = input('enter amount of business expenses (must be a positive number smaller than your revenue): ')
		tmp = float(tmp)
		gross = gross - tmp
		gross_step = gross - exonerated
		taxes = 0
		cotisations_to_pay = 0
		if gross < exonerated:
			net = gross - cotisations_to_pay
		else:
			for i in thresholds:
				if gross_step > i[1]:
					taxes += (i[1] - i[0]) * (i[2] / 100)
				else:
					taxes += (gross_step - i[0]) * (i[2] / 100)
					break
		net = gross - taxes
		for i in cotisations:
			if net > i[1]:
				cotisations_to_pay += (i[1] - i[0]) * (i[2] / 100)
			else:
				cotisations_to_pay += (net - i[0]) * (i[2] / 100)
				break
		net -= cotisations_to_pay
		print("taxes to pay: {0:.2f}{1}".format(taxes, currency))
		print("cotisations to pay: {0:.2f}{1}".format(cotisations_to_pay, currency))
		print("net income = {0:.2f}{1}".format(net, currency))