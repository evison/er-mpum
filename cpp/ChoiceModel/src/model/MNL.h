/*
 * MNL.h
 *
 *  Created on: Jan 13, 2016
 *      Author: qi
 */

#ifndef MODEL_MNL_H_
#define MODEL_MNL_H_
#include <armadillo>
#include <string>
#include <vector>
#include "set"
#include "Transaction.h"
#include "ModelParam.h"
#include "CFParam.h"
#include "TransStatus.h"

namespace rec {
using namespace arma;
using namespace std;

class MNL {
public:
	int _dim; //! latent vector dimension
	double _eta;
	double _learnRate;
	/// transaction data file
	string _transFile;
	int _numTrans;
	int _numProducts;
	vector<Transaction> _transactions;
	ModelParam _modelParam;
protected:
	void _factorProductivity(CFParam& tg, int j, int k, TransStatus& tstatus);
	void _powerSigmoidTransform(CFParam& tg, int assignIdx, int j , int k, TransStatus& tstatus);
	/**
	 * pairwise product utility gradient w.r.t the two products' latent vectors, biases and global bias
	 * @param trans	transaction
	 * @param assignIdx	assignment idx
	 * @param j	index of first product
	 * @param k index of second product
	 * @param tg	transaction gradient
	 * @return resulted gradient
	 */
	void _pairLogCESUtilityGrad(TransStatus& tstatus, int assignIdx, int j, int k,CFParam& powerCFGrad, CFParam& fctProdCFGrad);

	/**
	 * calculate the gradient of a candidate set of a transaction
	 *
	 * @param trans transaction
	 * @param assignIdx candidiate set index
	 * @return resulted gradient by aggregating the gradient resulted by all product pairs @see _pairUtilityGrad
	 */
	void _assignUtilityGrad(TransStatus& tstatus, int assignIdx, CFParam& powerCFGrad, CFParam& fctProdCFGrad);
	/**
	 * gradient by regularization terms
	 */
	CFParam _powerCFRegGrad(const Transaction& trans);
	CFParam _fctProdCFRegGrad(const Transaction& trans);

	/**
	 * update solution by subtracting the variable gradient
	 * @param pids involved product ids
	 * @param pMatGrad product latent vectors
	 * @param pBiasGrad product bias
	 * @param gBiasGrad globl bias
	 */
	void _applyGrad(set<int>& pids, mat& pMatGrad, vec& pBiasGrad, double &gBiasGrad, mat& fctMatGrad, vec& fctBiasGrad, double &gFctBiasGrad);
	/**
	 *aggregate the gradient of multiple transactions (minibatch SGD)
	 *
	 *@param trans Transaction instance
	 *@param tg gradient resulted by the given transaction
	 *@param pMatGrad aggregated product latent vector gradient
	 *@param pBiasGrad aggregated product bias gradient
	 *@param gBiasGrad aggregated global bias gradient
	 */
	void _aggregateGrad(Transaction& trans, CFParam& tg, mat& pMatGrad, vec& pBiasGrad, double &gBiasGrad);
	/// regualrization term cost
	double _regCost(Transaction& trans);
	void _transPredict(const Transaction& trans, const set<int>& allIds, vector<int>& recItems, vector<pair<int,double>>& productR);
public:
	MNL(int dim = 10, double eta=0.01);
	int getMaxProductId();
	/**
	 * calculate the power of the utility of two products
	 * @param	idx1	first product id
	 * @param	idx2	second product id
	 * @return	resulted power
	 */
	double cfPower(int idx1, int idx2) const;

	inline vec getPowerCFLat(int idx) const{
		return _modelParam._powerLat.col(idx);
	}

	inline double getPowerCFBias(int idx) const {
		return _modelParam._powerBias[idx];
	}

	inline double getPowerCFGBias() const {
		return _modelParam._gPowerBias;
	}

	/**
	 * update the gradient of the variables involved in transaction of transId
	 * @param transId transaction idx
	 */
	void transCFGrad(TransStatus& tstatus,  CFParam& powerCFGrad, CFParam& fctProdCFGrad);

	void loadFromFile(string file);
	void sgd(int maxEpochs = 50);
	inline int getNumProducts() {
		return _numProducts;
	}
	inline int getNumTransactions() {
		return _numTrans;
	}

	inline void saveModel(const string& prefix) {
		_modelParam.save(prefix);
	}
	inline void loadModel(const string& prefix) {
		_modelParam.load(prefix);
	}

	void predict(const string& folder);
	vector<int> findSubstitutionPair();
	virtual ~MNL();
};


istream& operator>> (istream& iss, MNL& mnl);

}

#endif /* MODEL_MNL_H_ */
