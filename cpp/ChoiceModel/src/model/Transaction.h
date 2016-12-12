/*
 * Transaction.h
 *
 *  Created on: Jan 13, 2016
 *      Author: qi
 */

#ifndef MODEL_TRANSACTION_H_
#define MODEL_TRANSACTION_H_
#include "armadillo"
#include "vector"
#include <string>
#include <sstream>
#include <iostream>
#include "CFParam.h"
#include "Config.h"

namespace rec {
using namespace std;
using namespace arma;

class MNL;

class Transaction {

public:
	vector<int> _itemIds;
	/// quantity assignment to the items
	mat _quantAssign;
public:
	Transaction(int numItems = 0, int numAssigns = 0);
	void init(int numItems = 0, int numAssigns = 0);
	void sampleAssign(int sampleCnt = 10);
	/**
	 * calculate offset for given index pair
	 * @param idx1	index1
	 * @param idx2 	index2
	 * @param  N 	size of the array
	 * @return offset
	 */
	static int pairOffset(int idx1, int idx2, int N);
	static double myPow(double q, double r);

	inline int getItemCnt() const{
		return _itemIds.size();
	}

	inline int getPairCnt() const{
		int N = _itemIds.size();
		return N * (N - 1) / 2;
	}

	inline int getAssignCnt() const{
		return _quantAssign.n_rows;
	}

	inline double getAssignItemQ (int assignIdx, int itemIdx) const {
		return _quantAssign(assignIdx,itemIdx);
	}

	inline double setAssignItemQ(int assignIdx, int itemIdx, double q) {
		_quantAssign(assignIdx,itemIdx) = q;
		return q;
	}

	inline int getItemId(int idx) const {
		return _itemIds[idx];
	}

	inline int setItemId(int idx, int pid) {
		_itemIds[idx] = pid;
		return pid;
	}

	inline void reset() {
		_itemIds.clear();
		_quantAssign.clear();
	}
	/**
	 * pairwise product utility by U(x,y) = 1/r * log((alpha * x^r + (1 - alpha)*y^r))
	 */
	static double pairwiseLogCESUtility(double q1, double q2, double r, double alpha = 0.5);
	virtual ~Transaction();
};

istream& operator>> (istream& iss, Transaction& trans);
ostream& operator<< (ostream& oss, const Transaction& trans);



} /* namespace model */

#endif /* MODEL_TRANSACTION_H_ */
