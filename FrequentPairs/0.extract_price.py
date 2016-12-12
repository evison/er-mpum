import csv

if __name__=='__main__':
	fi = open('../amazonBabyRegistry/itemProfile.csv', 'r')
	fo = open('../amazonBabyRegistry/itemprice.txt', 'w')

	line = fi.readline()
	for line in fi:
		line = line.strip().split('\t')
		if len(line) >= 3:
			pid = line[0]
			price = line[3]
			if len(pid) == 10 and price.isdigit():
				fo.write(pid + '\t' + price + '\n')

	fi.close()
	fo.close()
	'''
	fo = open('../amazonBabyRegistry/itemprice.txt', 'w')
	with open('../amazonBabyRegistry/itemProfile.csv') as csvfile:
		reader = csv.DictReader(csvfile)
		for row in reader:
			fo.write(row['id'] + '\t' + row['price'] + '\n')
	fo.close()
	'''