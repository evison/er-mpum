/*
 * TransStatus.h
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#ifndef MODEL_TRANSSTATUS_H_
#define MODEL_TRANSSTATUS_H_
#include "includes.h"
#include "ModelParam.h"
#include "Transaction.h"

namespace rec {

//! record transaction status, e.g. pairwise power, utility and so on
class TransStatus{
public:
	const ModelParam& _mp;
	const Transaction& _trans;
	vec _pairPower;
	vec _pairAlpha; //! weight
	/// pairwise utility for all assignments
	mat _assignPairUtility;
	/// utility for each assignment
	vec _assignUtility;
	//! pairwise factor productivity
	vec _pairFctProdCF;
	vec _pairFctProd;
public:
	TransStatus(const ModelParam& mp, const Transaction& trans);
	void updatePower();
	void updateUtility();
	void update();
	void setPairPower(int idx1, int idx2, double r);
	double getPairPower(int idx1, int idx2);
	void setAssignPairUtility(int assignIdx, int idx1, int idx2, double utility);
	double getAssignPairUtility(int assignIdx, int idx1, int idx2);
	double updateAssignUtility(int assignIdx);
	double getPairAlpha(int idx1, int idx2);
	void setPairAlpha(int idx1, int idx2, double val);
	void setPairAlpha(double val);
	double getAssignUtility(int assignIdx);
	double getUtility();
	//! pairwise factor productivity
	void updateFctProd();

	inline double getPairFactorProduct(int j, int k) {
		int offset = Transaction::pairOffset(j,k,_trans.getItemCnt());
		return getPairFactorProduct(offset);
	}

	inline double getPairFactorProduct(int offset) {
		return _pairFctProd[offset];
	}

	inline double getPairFactorProductCF(int j, int k) {
		int offset = Transaction::pairOffset(j,k,_trans.getItemCnt());
		return getPairFactorProductCF(offset);
	}

	inline double getPairFactorProductCF(int offset) {
		return _pairFctProdCF[offset];
	}

	double nnl();
};

} /* namespace model */

#endif /* MODEL_TRANSSTATUS_H_ */
