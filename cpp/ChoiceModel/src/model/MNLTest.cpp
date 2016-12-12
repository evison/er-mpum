#include <gtest/gtest.h>
#include "MNL.h"
#include "Transaction.h"
#include "CFParam.h"
#include <fstream>
#include <string>

#define BUFFER_SIZE	1000000
using namespace std;
using namespace rec;


void initTransaction(Transaction& trans) {
	string inputFile = "/home/qi/git/sigir16/experiment/cpp/ChoiceModel/TestTransaction.txt";
	ifstream ifs(inputFile.c_str(),ifstream::in);
	ifs >> trans;
}

TEST(Transaction, Read) {
	string inputFile = "/home/qi/git/sigir16/experiment/cpp/ChoiceModel/TestTransaction.txt";
	string outputFile = "/home/qi/git/sigir16/experiment/cpp/ChoiceModel/TestTransaction2.txt";
	ifstream ifs(inputFile.c_str(),ifstream::in);
	ofstream ofs(outputFile.c_str(),ifstream::out);

	Transaction trans;
	ifs >> trans;
	/// output transaction
	ofs << trans;
	ifs.close();
	ofs.close();
	/// compare the two file content
	vector<char> buffer1(BUFFER_SIZE,0);
	vector<char> buffer2(BUFFER_SIZE,0);
	ifstream ifs1(inputFile.c_str(),ifstream::in|ifstream::binary);
	ifstream ifs2(outputFile.c_str(),ifstream::in|ifstream::binary);
	ifs1.read(&buffer1[0],BUFFER_SIZE);
	ifs2.read(&buffer2[0],BUFFER_SIZE);
	EXPECT_EQ(buffer1,buffer2);

}

TEST(MNL, LoadTransaction) {
	string inputFile = "/home/qi/git/sigir16/experiment/cpp/ChoiceModel/Transactions.txt";
	string outputFile = "/home/qi/git/sigir16/experiment/cpp/ChoiceModel/TransactionsVerify.txt";
	ifstream ifs(inputFile.c_str(),ifstream::in);
	ofstream ofs(outputFile.c_str(),ifstream::out);
	MNL mnl;
	ifs >> mnl;

	for (int i = 0; i < mnl._numTrans; i++) {
		ofs << mnl._transactions[i];
	}
	ifs.close();
	ofs.close();

	vector<char> buffer1(BUFFER_SIZE,0);
	vector<char> buffer2(BUFFER_SIZE,0);
	ifstream ifs1(inputFile.c_str(),ifstream::in|ifstream::binary);
	ifstream ifs2(outputFile.c_str(),ifstream::in|ifstream::binary);
	ifs1.read(&buffer1[0],BUFFER_SIZE);
	ifs2.read(&buffer2[0],BUFFER_SIZE);
	EXPECT_EQ(buffer1,buffer2);
}

TEST(Transaction, PairOffset) {
	int N = 10;
	vector<int> expected, actual;
	int offset = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			expected.push_back(offset++);
			actual.push_back(Transaction::pairOffset(i,j,N));
		}
	}
	EXPECT_EQ(expected,actual);
}

//
//TEST(Transaction, PairPower) {
//	Transaction trans;
//	initTransaction(trans);
//	///
//	double r = 0;
//	int N = trans._itemIds.size();
//	int pairCnt = trans.getPairCnt();
//	vec expected(pairCnt);
//	vec retrieved(pairCnt);
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			r += 0.1;
//			int offset = Transaction::pairOffset(i,j,N);
//			expected[offset] = r;
//			trans.setPairPower(i,j,r);
//			retrieved[offset] = trans.getPairPower(i,j);
//		}
//	}
//	EXPECT_EQ(accu(expected - trans._pairPower), 0);
//	EXPECT_EQ(accu(expected - retrieved),0);
//}
//
TEST(Transaction, PairUtility) {
	vector<int> q1{1,3,5};
	vector<int> q2 = {2,4,6};
	vector<double> expected = {log(0.5*1+0.5*2), 0.5 * log(0.5 * 3 * 3 + 0.5 *4 * 4), -1 * log(0.5 * 1/5.0 + 0.5 *1/6.0)};
	vector<double> actual = {Transaction::pairwiseLogCESUtility(q1[0],q2[0],1),
			Transaction::pairwiseLogCESUtility(q1[1],q2[1],2),
			Transaction::pairwiseLogCESUtility(q1[2],q2[2],-1)};
	EXPECT_EQ(actual,expected);
}
//
//TEST(Transaction, AssignUtility) {
//	Transaction trans;
//	initTransaction(trans);
//	/// set the power
//	int pairCnt = trans.getPairCnt();
//	int N = trans.getItemCnt();
//	int M = trans.getAssignCnt();
//	vec pairPower(pairCnt,fill::randu);
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			int offset = Transaction::pairOffset(i,j,N);
//			trans.setPairPower(i,j,pairPower[offset]);
//		}
//	}
//	EXPECT_EQ(accu(trans._pairPower-pairPower),0);
//	/// get the utility
//	mat assignPairUtil(M,pairCnt);
//	int offset = 0;
//	for (int i = 0; i < M; i++) {
//		offset = 0;
//		for (int j = 0; j < N; j++) {
//			int qj = trans.getAssignItemQ(i,j);
//			for (int k = j + 1; k < N; k++) {
//				int qk = trans.getAssignItemQ(i,k);
//				double r = trans.getPairPower(j,k);
//				double util = Transaction::pairwiseLogCESUtility(qj,qk,1 - exp(r));
//				assignPairUtil(i,offset++) = util;
//			}
//		}
//		trans.updateAssignUtility(i);
//	}
//
//	EXPECT_EQ(all(vectorise(assignPairUtil - trans._assignPairUtility) == 0),true);
//
//}

TEST(CFParam, Init) {
	CFParam tg(10,5);
	mat m(5,10,fill::zeros);
	vec v(10,fill::zeros);
	double b = 0;
	EXPECT_EQ(all(vectorise(tg._latGrad - m) == 0),true);
	EXPECT_EQ(all(tg._biasGrad - v == 0), true);
	EXPECT_EQ(tg._gBiasGrad,b);
}

TEST(CFParam, Operation) {
	CFParam tg(10,5), tg1(10,5), tg2(10,5);
	tg = 1;
	tg1 = 2;
	tg2 = 4;
	EXPECT_EQ(tg * 2 == tg1, true);
	tg *= 2;
	EXPECT_EQ(tg + tg1 == tg2, true);
	EXPECT_EQ(tg2 - tg1 == tg, true);
}

TEST(ARMA,Log) {
	vec v{0,0,0};
	double r = -0.2;
	vec b = arma::pow(v,r);
	cout << b << endl;
	EXPECT_EQ(accu(b),0);
}

