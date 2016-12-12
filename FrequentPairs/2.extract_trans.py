import csv

if __name__=='__main__':
	fi_price = open('../amazonBabyRegistry/itemprice.txt', 'r')
	fi_list = open('../amazonBabyRegistry/listItems.txt', 'r')
	fo = open('../amazonBabyRegistry/raw_transactions.txt', 'w')

	#read price
	price = dict();
	for line in fi_price:
		line = line.strip().split()
		price[line[0]] = line[1]

	#read lists
	for line in fi_list:
		line = line.strip().split()[2]
		pids = line.split(',')
		count = dict()
		omitline = False
		for pid in pids:
			if not pid in price:
				omitline = True
				break
			if not pid in count:
				count[pid] = 0
			count[pid] = count[pid] + 1
		if omitline:
			continue
		fo.write(str(len(count)) + ' ' + '0' + '\n')
		idlist = list()
		qlist = list()
		for pid in count:
			idlist.append(pid)
			qlist.append(count[pid])
		#write ID
		idx = 0
		for pid in idlist:
			if idx == 0:
				fo.write(pid)
			else:
				fo.write(" " + pid)
			idx = idx + 1
		fo.write('\n')
		#write price
		idx = 0
		for pid in idlist:
			if idx == 0:
				fo.write(price[pid])
			else:
				fo.write(" " + price[pid])
			idx = idx + 1
		fo.write('\n')
		#write quantity
		idx = 0
		for q in qlist:
			if idx == 0:
				fo.write(str(q))
			else:
				fo.write(" " + str(q))
			idx = idx + 1
		fo.write('\n')
