/*
 * TransStatus.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#include "TransStatus.h"

namespace rec {


TransStatus::TransStatus(const ModelParam& mp, const Transaction& trans):_mp(mp),_trans(trans){
	int numPairs = trans.getPairCnt();
	int numAssigns = trans.getAssignCnt();
	_pairPower.resize(numPairs);
	_pairAlpha.resize(numPairs);
	_pairAlpha.fill(0.5); //! assuming both products have the same weight
	_assignPairUtility.resize(numAssigns,numPairs);
	_assignUtility.resize(numAssigns);
	_pairFctProdCF.resize(numPairs);
	_pairFctProd.resize(numPairs);
}

void TransStatus::updateUtility() {
	for (int i = 0; i < _trans.getAssignCnt(); i++) {
		updateAssignUtility(i);
	}
}

void TransStatus::updateFctProd() {
	int N = _trans.getItemCnt();
	double b0 = _mp._gFctBias;
	for (int j = 0; j < N; j++) {
		const vec& v1 = _mp._fctLat.col(j);
		double b1 = _mp._fctBias[j];
		for (int k = j + 1; k < N; k++) {
			const vec& v2 = _mp._fctLat.col(k);
			double b2 = _mp._fctBias[k];
			double cfVal = rec::CF(v1, v2, b1, b2, b0);
			int offset = Transaction::pairOffset(j, k, N);
			/// before sigmoid function is applied
			_pairFctProdCF[offset] = cfVal;
			/// do the sigmoid transformation
			_pairFctProd[offset] = 1.0 / (1 + std::exp(-cfVal));
		}
	}
}


void TransStatus::updatePower() {
	int N = _trans.getItemCnt();
	/// first update the pairwise power
	for (int i = 0; i < N; i++) {
		int pid1 = _trans._itemIds[i];
		for (int j = i + 1; j < N; j++) {
			int pid2 = _trans._itemIds[j];
			double r = _mp.cfPower(pid1,pid2);
			/// note the product index here are not pid1, pid2
			setPairPower(i,j,r);
		}
	}
}

void TransStatus::update() {
	updatePower();
	/// update factor productivity term
//	updateFctProd();
	/// update utility
	updateUtility();
}

void TransStatus::setAssignPairUtility(int assignIdx, int idx1, int idx2, double utility){
	// number of items
	if (idx1 > idx2) swap(idx1,idx2);
	int N = _trans._itemIds.size();
	int offset = Transaction::pairOffset(idx1, idx2, N);
	_assignPairUtility(assignIdx,offset) = utility;
}

double TransStatus::getAssignPairUtility(int assignIdx, int idx1, int idx2) {
	if (idx1 > idx2) swap(idx1,idx2);
	int offset = Transaction::pairOffset(idx1,idx2,_trans._itemIds.size());
	return _assignPairUtility(assignIdx,offset);
}

double TransStatus::getPairAlpha(int idx1, int idx2) {
	int offset = Transaction::pairOffset(idx1,idx2,_trans.getItemCnt());
	return _pairAlpha[offset];
}

void TransStatus::setPairAlpha(int idx1, int idx2, double val) {
	int offset = Transaction::pairOffset(idx1,idx2,_trans.getItemCnt());
	_pairAlpha[offset] = val;
}

void TransStatus::setPairAlpha(double val) {
	for (int i = 0; i < _trans.getPairCnt(); i++)
		_pairAlpha[i] = val;
}

double TransStatus::nnl() {
	double sumExpUtil = 0;
	double selectedUtil = 0;
	for (int i = 0; i < _trans.getAssignCnt(); i++) {
		double expUtil = std::exp(getAssignUtility(i));
		if (i == 0) selectedUtil = expUtil;
		sumExpUtil += expUtil;
	}
	return log(sumExpUtil / selectedUtil);
}

double TransStatus::getAssignUtility(int assignIdx) {
	return _assignUtility[assignIdx];
}

double TransStatus::getUtility(){
	double totalUtil = 0;
	for (int i = 0; i < _trans.getAssignCnt(); i++) {
		totalUtil += getAssignUtility(i);
	}
	return totalUtil;
}

double TransStatus::updateAssignUtility(int assignIdx) {
	int N = _trans._itemIds.size();
	/// sum of all pairwise utilities
	double U = 0;
	for (int j = 0; j < N; j++) {
		int qj = _trans._quantAssign(assignIdx,j);
		for (int k = j + 1; k < N; k++) {
			int qk = _trans._quantAssign(assignIdx,k);
			/// get the power
			double b = getPairPower(j,k);
			double alpha = getPairAlpha(j,k);
			double r = 1 - Config::CONFIG_INST._sigBound/(1 + std::exp(-b));
//			double fctProd = getPairFactorProduct(j,k);
//			fctProd = 1;
			/// without the factor productivity
//			double u = Transaction::pairwiseLogCESUtility(qj,qk,r, alpha) + std::log(fctProd);
			double u = Transaction::pairwiseLogCESUtility(qj,qk,r, alpha);

			setAssignPairUtility(assignIdx,j,k, u);
			U += u;
		}
	}
	_assignUtility[assignIdx] = U / 2;
	return _assignUtility[assignIdx];
}

void TransStatus::setPairPower(int idx1, int idx2, double r) {
	int N = _trans._itemIds.size();
	int offset = Transaction::pairOffset(idx1,idx2, N);
	_pairPower[offset] = r;
}

double TransStatus::getPairPower(int idx1, int idx2) {
	int N = _trans._itemIds.size();
	int offset = Transaction::pairOffset(idx1,idx2, N);
	return _pairPower[offset];
}


} /* namespace model */
