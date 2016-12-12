#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "candidates.h"
using namespace std;

map<string, float>* read_price(string price_file) {
	ifstream ifs(price_file.c_str());
	string line;
	string id;
	float price;
	map<string, float>* ret = new map<string, float>();
	while (getline(ifs, line)) {
		istringstream iss(line);
		iss >> id;
		iss >> price;
		ret->insert(pair<string, float>(id, price));
	}
	return ret;
}

void generate_trans(string raw_trans_file, string new_trans_file) {
	cout << "In read_transaction" << endl;
	ifstream ifs(raw_trans_file.c_str());
	ofstream ofs(new_trans_file.c_str());
	Transaction* trans = new Transaction();
	int i = 0;
	while (ifs >> *trans) {
		cout << "i =" << i << endl;
		i++;
		generate(trans, 5000);
		ofs << *trans;
	}
	cout << "Before return" << endl;
	return;
}

int main() {
	// string raw_trans_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/raw_transactions.txt";
	string raw_trans_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/shopcom/raw_transactions.txt";
	//string price_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/itemprice.txt";
	//string output_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/candidates.txt";
	// string new_trans_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/new_transactions.txt";
	string new_trans_file = "/Users/evison/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/shopcom/new_transactions.txt";
	//ifstream ifs(inputFile.c_str(),ifstream::in);
	// ifstream ifs(raw_trans_file.c_str(), ifstream::in);
	// ofstream ofs(test_out_file.c_str(), ofstream::out);
	//map<string, float> *price = read_price(price_file);
	// vector<Transaction*> trans;
	generate_trans(raw_trans_file, new_trans_file);
	cout << "After read" << endl;
	/*
	for (map<string, float>::iterator it = price->begin(); it != price->end(); ++it) {
		cout << it->first << " " << it-> second << endl;
	}
	cout << price->size() << endl;
	*/
	return 0;
}