
if __name__=='__main__':
	fi = open('../amazonBabyRegistry/listItems.txt', 'r')
	fo = open('../amazonBabyRegistry/transactions.txt', 'w')

	for line in fi:
		trans = line.strip().split('\t')[2]
		if len(trans) > 0:
			fo.write(trans + '\n')

	fi.close()
	fo.close()
	