if __name__ == '__main__':
	fi = open('/Users/evison/Documents/dataset/shopcom/orderdata.txt', 'r')
	fo = open('../shopcom/itemprice.txt', 'w')

	price = dict()
	for line in fi:
		line = line.strip().split('\t')
		#print line
		try:
			pid = line[7]
			p = line[9]
			if len(pid) > 0 and len(p) > 0:
				price[pid] = p
		except:
			pass

	for pid in price:
		fo.write(pid + '\t' + price[pid] + '\n')

	fi.close()
	fo.close()
