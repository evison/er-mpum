import csv

if __name__=='__main__':
	fi_price = open('../shopcom/itemprice.txt', 'r')
	fi_list = open('/Users/evison/Documents/dataset/shopcom/orderdata.txt', 'r')
	fo = open('../shopcom/raw_transactions.txt', 'w')

	#read price
	price = dict();
	for line in fi_price:
		line = line.strip().split()
		price[line[0]] = float(line[1])

	#read transactions
	current_orderid = "4098488"
	transaction = dict() #pid : q
	for line in fi_list:
		line = line.strip().split('\t')
		try:
			orderid = line[0]
			pid = line[7]
			q = int(line[10])
		except:
			continue
		if orderid == current_orderid:
			if pid in price:
				if pid not in transaction:
					transaction[pid] = 0
				transaction[pid] = transaction[pid] + q
		else:
			fo.write(str(len(transaction)) + '\t' + '0' + '\n')
			pid_list = transaction.keys()
			for i in range(len(pid_list)):
				if i == 0:
					fo.write(pid_list[i])
				else:
					fo.write(' ' + pid_list[i])
			fo.write('\n')
			for i in range(len(pid_list)):
				if i == 0:
					fo.write(str(round(price[pid_list[i]],2)))
				else:
					fo.write(' ' + str(round(price[pid_list[i]],2)))
			fo.write('\n')
			for i in range(len(pid_list)):
				if i == 0:
					fo.write(str(transaction[pid_list[i]]))
				else:
					fo.write(' ' + str(transaction[pid_list[i]]))
			fo.write('\n')
			#reset orderid and transactions
			current_orderid = orderid
			transaction = dict()
			if pid in price:
				transaction[pid] = q
	#write the last transaction
	fo.write(str(len(transaction)) + '\t' + '0' + '\n')
	pid_list = transaction.keys()
	for i in range(len(pid_list)):
		if i == 0:
			fo.write(pid_list[i])
		else:
			fo.write(' ' + pid_list[i])
	fo.write('\n')
	for i in range(len(pid_list)):
		if i == 0:
			fo.write(str(round(price[pid_list[i]],2)))
		else:
			fo.write(' ' + str(round(price[pid_list[i]],2)))
	fo.write('\n')
	for i in range(len(pid_list)):
		if i == 0:
			fo.write(str(transaction[pid_list[i]]))
		else:
			fo.write(' ' + str(transaction[pid_list[i]]))
	fo.write('\n')
