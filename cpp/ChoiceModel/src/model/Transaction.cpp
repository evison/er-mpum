/*
 * Transaction.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: qi
 */

#include "Transaction.h"

namespace rec {

Transaction::Transaction(int numItems, int numAssigns){
	init(numItems,numAssigns);
}

void Transaction::init(int numItems, int numAssigns) {
	// TODO Auto-generated constructor stub
	if (numItems > 0) {
		_itemIds.resize(numItems);
		if (numAssigns > 0) {
			_quantAssign.resize(numAssigns,numItems);
		}
	}
}

void Transaction::sampleAssign(int sampleCnt) {
	/// randomly select sampleCnt assignments

}

Transaction::~Transaction() {
	// TODO Auto-generated destructor stub
}

int Transaction::pairOffset(int idx1, int idx2, int N) {
	int a = N - 1;
	int b = N - idx1;
	int offset = (a + b) * idx1 / 2 + idx2 - idx1 - 1;
	return offset;
}

double Transaction::myPow(double q, double r) {
	double p = 0;
	if (q != 0) {
		if (r < 0) {
			p = 1/pow(q,-r);
		} else {
			p= pow(q,r);
		}
	}
	return p;
}

double Transaction::pairwiseLogCESUtility(double q1, double q2, double r,double alpha) {
	double p1 = myPow(q1,r), p2 = myPow(q2,r);
	// utility is log(CES(q1,q2)) = log((q1^r + q2^r)^1/r) = 1/r*log(q1^r + q2^r）
//	return 1/r * log(alpha * p1 + (1 - alpha) * p2);
	return 1/r * log(p1 +  p2);
}

istream& operator>> (istream& iss, Transaction& trans){
	int numAssigns, numItems;
	iss >> numItems >> numAssigns; //! number of items and number assignments
	trans.init(numItems,numAssigns); //! initialization
	for (int i = 0; i < numItems; i++) {
		int tmp;
		iss >> tmp;
		trans.setItemId(i,tmp);
	}

	/// now read the quantity assignment lines
	/// each line has integers separated by space and each integer is the quantity of the item at the same index
	for (int i = 0; i < numAssigns; i++) {
		for (int j = 0; j < numItems; j++) {
			double tmp;
			iss >> tmp;
			trans.setAssignItemQ(i,j,tmp);
		}
	}
	/// end of reading
	return iss;
}

ostream& operator<< (ostream& oss, const Transaction& trans) {
	oss << trans._quantAssign.n_rows << " " << trans._itemIds.size() << endl;
	for (int i = 0; i < trans._itemIds.size(); i++) {
		oss << (i == 0? "": " ") << trans._itemIds[i] ;
	}
	oss << endl;
	for (int i = 0; i < trans._quantAssign.n_rows; i++) {
		for (int j = 0; j < trans._quantAssign.n_cols; j++) {
			oss << (j == 0 ? "" : " ") << trans._quantAssign(i,j);
		}
		oss << endl;
	}
	return oss;
}


} /* namespace model */
