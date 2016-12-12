import math

if __name__=='__main__':
	fi = open('trans.txt', 'r')
	fo_train = open('train.txt', 'w')
	fo_test = open('test.txt', 'w')

	#fo_train.write('listid,itemid,quantity\n')
	#fo_test.write('listid,itemid,quantity\n')

	train_ratio = 0.8
	test_ratio = 1 - train_ratio

	user_id = 0
	while True:
		line = fi.readline()
		if not line:
			break
		[num_item, num_trans] = [int(x) for x in line.strip().split()]
		num_train_item = int(math.ceil(train_ratio * num_item))
		num_test_item = num_item - num_train_item
		item_list = [int(x) for x in fi.readline().strip().split()]
		quantity_list = [int(x) for x in fi.readline().strip().split()]
		for i in range(num_trans - 1):
			line = fi.readline()
		for i in range(num_train_item):
			fo_train.write(str(user_id) + ',' + str(item_list[i]) + ',1\n')
		for i in range(num_train_item, num_item):
			fo_test.write(str(user_id) + ',' + str(item_list[i]) + ',1\n')
		user_id = user_id + 1

	fi.close()
	fo_train.close()
	fo_test.close()
