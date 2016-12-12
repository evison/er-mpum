/*
 * MNL.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: qi
 */

#include "MNL.h"
#include "exception"
#include "math.h"
#include "iterator"
#include "utility"
#include "algorithm"

namespace rec{

MNL::MNL(int dim, double eta):_dim(dim),_eta(eta),_learnRate(0.001),_numTrans(0),_numProducts(0) {
	// TODO Auto-generated constructor stub

}


MNL::~MNL() {
	// TODO Auto-generated destructor stub
}

void MNL::loadFromFile(string file){
	ifstream ifs(file.c_str(),ifstream::in);
	if(ifs.good()) {
		ifs >> *this;
		ifs.close();
	} else {
		/// failed to open
		throw ifstream::failure(file);
	}

}



/**
 * power of a given product pair, it corresponds to the argument of exponential function of Eq. 12
 * the actual power for CES is: 1 - exp(cfPower(idx1,idx2)
 */
double MNL::cfPower(int idx1, int idx2) const {
	const vec& v1 = getPowerCFLat(idx1);
	const vec& v2 = getPowerCFLat(idx2);
	double b1 = getPowerCFBias(idx1);
	double b2 = getPowerCFBias(idx2);
	double val = CF(v1,v2,b1,b2,getPowerCFGBias());
	return val;
}

void MNL::_factorProductivity(CFParam& tg, int j, int k, TransStatus& tstatus) {
	/// log(F) = log(1/(1 + exp(-x)))
	double pfCF = tstatus.getPairFactorProductCF(j,k);
	double expCF = std::exp(-pfCF);
	double dudf = expCF/(1 + expCF);
	int pidj = tstatus._trans.getItemId(j);
	int pidk = tstatus._trans.getItemId(k);
	const vec& xj = _modelParam._fctLat.col(pidj);
	const vec& xk = _modelParam._fctLat.col(pidk);

	tg._latGrad.col(j) += (xk * dudf); //！gradient for latent vector of product j
	tg._latGrad.col(k) += (xj * dudf); //!gradient for latent vector of product k
	tg._biasGrad[j] += dudf; //! gradient for product j bias
	tg._biasGrad[k] += dudf; //! gradient for product k bias
	tg._gBiasGrad += dudf; //! gradient of global bias
}

void MNL::_powerSigmoidTransform(CFParam& tg, int assignIdx, int j , int k, TransStatus& tstatus) {
	/// (1/(1+exp(-x)))' = exp(x)/(1+exp(x))^2 where x is b
	double qj = tstatus._trans.getAssignItemQ(assignIdx,j); //! quantity of product j
	double qk = tstatus._trans.getAssignItemQ(assignIdx,k); //! quantity of product k

	double b = tstatus.getPairPower(j,k); //! get the CF value by Eq. 12
//	double alpha = tstatus.getPairAlpha(j,k);
	double r = 1 - Config::CONFIG_INST._sigBound/(1+std::exp(-b));//! Sigmoid transform, NOT exponential

//	double qjr = alpha * Transaction::myPow(qj,r);
//	double qkr = (1 - alpha) * Transaction::myPow(qk,r);
	double qjr = Transaction::myPow(qj,r);
	double qkr = Transaction::myPow(qk,r);

	double dudr = -1/(r*r) * std::log(qjr + qkr);

	double qjdr = 0, qkdr = 0;
	if (qj > 0) {
		qjdr = log(qj) * qjr;
	}
	if (qk > 0) {
		qkdr = log(qk) * qkr;
	}
	if(qjr + qkr > 0) {
		dudr += (1/r * 1/(qjr + qkr) * (qjdr + qkdr));
	}

	double expb = std::exp(b);
	double drdb = -Config::CONFIG_INST._sigBound * expb/((1+expb) * (1+expb));
	dudr *= drdb;
	/// update the gradient of CF parameters
	/// gradient with latent vector of product j
	int pidj = tstatus._trans.getItemId(j);
	int pidk = tstatus._trans.getItemId(k);
	const vec& xj = _modelParam._powerLat.col(pidj);
	const vec& xk = _modelParam._powerLat.col(pidk);
	tg._latGrad.col(j) += (xk * dudr); //！gradient for latent vector of product j
	tg._latGrad.col(k) += (xj * dudr); //!gradient for latent vector of product k
	tg._biasGrad[j] += dudr; //! gradient for product j bias
	tg._biasGrad[k] += dudr; //! gradient for product k bias
	tg._gBiasGrad += dudr; //! gradient of global bias
}


/**
 * calculate the gradient of product pair j-k of a given candidate set
 *
 * @param trans transaction
 * @param assignIdx candidate set index
 * @param j product j
 * @param k product k
 * @param tg gradient holder, will be updated
 * @return
 */
void MNL::_pairLogCESUtilityGrad(TransStatus& tstatus, int assignIdx, int j, int k, CFParam& powerCFGrad, CFParam& fctProdCFGrad) {
	/// update factor productivity CF gradient
	double qj = tstatus._trans.getAssignItemQ(assignIdx,j); //! quantity of product j
	double qk = tstatus._trans.getAssignItemQ(assignIdx,k); //! quantity of product k
	if (qj + qk > 0 ){
//		_factorProductivity(fctProdCFGrad, j,k, tstatus);
		_powerSigmoidTransform(powerCFGrad,assignIdx, j,k, tstatus);
	}
}

void MNL::_assignUtilityGrad(TransStatus& tstatus, int assignIdx, CFParam& powerCFGrad, CFParam& fctProdCFGrad){
	int N = tstatus._trans.getItemCnt(); //! number of products
	for (int j = 0; j < N; j++) { //! product j
		for (int k = j + 1; k < N; k++) { //! product k
			_pairLogCESUtilityGrad(tstatus,assignIdx,j,k,powerCFGrad, fctProdCFGrad); //! add the gradient by product pair jk
		}
	}
}

CFParam MNL::_powerCFRegGrad(const Transaction& trans) {
	int N = trans.getItemCnt(); //! number of items
	CFParam tg(N,_dim); //! resulted gradient
	for (int i = 0; i < N; i++) { //! product index i
		int prodId = trans._itemIds[i]; //! get the product id
		tg._latGrad.col(i) = _modelParam._powerLat.col(prodId); //! gradient of the latent vector of prodId
		tg._biasGrad[i] = _modelParam._powerBias[prodId]; //！ gradient of the bias of prodId
	}
	tg._gBiasGrad = _modelParam._gPowerBias;
	/// regularization coefficient
	tg *= _eta;
	return tg;
}

CFParam MNL::_fctProdCFRegGrad(const Transaction& trans) {
	int N = trans.getItemCnt(); //! number of items
	CFParam tg(N,_dim); //! resulted gradient
	for (int i = 0; i < N; i++) { //! product index i
		int prodId = trans._itemIds[i]; //! get the product id
		tg._latGrad.col(i) = _modelParam._fctLat.col(prodId); //! gradient of the latent vector of prodId
		tg._biasGrad[i] = _modelParam._fctBias[prodId]; //！ gradient of the bias of prodId
	}
	tg._gBiasGrad = _modelParam._gFctBias;
	/// regularization coefficient
	tg *= _eta;
	return tg;
}


void MNL::_aggregateGrad(Transaction& trans, CFParam& tg, mat& latGrad, vec& biasGrad, double& gBiasGrad) {
	for (int i = 0; i < trans.getItemCnt(); i++) {
		int prodId = trans._itemIds[i]; //! get product id
		latGrad.col(prodId) += (tg._latGrad.col(i)); //! add the gradient by the transaction
		biasGrad[prodId] += (tg._biasGrad[i]);
	}
	gBiasGrad += (tg._gBiasGrad);
}

void MNL::_applyGrad(set<int>& pids, mat& pMatGrad, vec& pBiasGrad, double &gBiasGrad, mat& fctMatGrad, vec& fctBiasGrad, double& gFctBiasGrad) {
	bool isnan = std::isnan(gBiasGrad);
	for (auto pid: pids) { //! only consider products involved in the mini-batch SGD
		if(!isnan) {
			_modelParam._powerLat.col(pid) -= (pMatGrad.col(pid) * _learnRate);
			_modelParam._powerBias[pid] -= (pBiasGrad[pid] * _learnRate);
		}
		pMatGrad.col(pid) = vec(_dim,fill::zeros); //! reset the gradient for next batch
		pBiasGrad[pid] = 0;
	}
	if (!isnan)
		_modelParam._gPowerBias -= (gBiasGrad * _learnRate);
	gBiasGrad = 0;
	pids.clear(); //! clear all product ids in the batch
}

void MNL::transCFGrad(TransStatus& tstatus, CFParam& powerCFGrad, CFParam& fctProdCFGrad) {
	int M = tstatus._trans.getAssignCnt(); //! number of candidate sets in the transaction, including the observed transaction
	vec expUtil = arma::exp(tstatus._assignUtility);
	double sumExpUtil = arma::accu(expUtil);
	int N = tstatus._trans.getItemCnt();
	_assignUtilityGrad(tstatus, 0,powerCFGrad,fctProdCFGrad);
	powerCFGrad *= expUtil[0];
//	fctProdCFGrad *= expUtil[0];

	/// gradient w.r.t all transactions except the first one
	CFParam powerCFGrad2 = powerCFGrad; //! avoid duplicate process of candidate set 1
//	CFParam fctProdCFGrad2 = fctProdCFGrad; //! avoid duplicate process of candidate set 1

	for (int i = 1; i < M; i++) {
		CFParam tmpTg1(N,_dim), tmpTg2(N,_dim);
		_assignUtilityGrad(tstatus,i,tmpTg1, tmpTg2); //! utility of i-th transaction
		tmpTg1 *= expUtil[i];
		powerCFGrad2 += tmpTg1;

		//tmpTg2 *= expUtil[i];
		//fctProdCFGrad2 += tmpTg2;
	}

	double recipYSquare = 1/(sumExpUtil * sumExpUtil);
	powerCFGrad *= 1/sumExpUtil;
	powerCFGrad -= (powerCFGrad2 * expUtil[0] * recipYSquare);
	powerCFGrad *= -sumExpUtil/expUtil[0]; //! derivative of negative log-likelihood
	powerCFGrad += _powerCFRegGrad(tstatus._trans); //! gradient of regularization term

//	fctProdCFGrad *= 1/sumExpUtil;
//	fctProdCFGrad -= (fctProdCFGrad2 * expUtil[0] * recipYSquare);
//	fctProdCFGrad *= -sumExpUtil/expUtil[0]; //! derivative of negative log-likelihood
//	fctProdCFGrad += _fctProdCFRegGrad(tstatus._trans); //! gradient of regularization term
}

double MNL::_regCost(Transaction& trans) {
	double cost = 0;
	for (auto pid : trans._itemIds) {
		vec pLat = _modelParam._powerLat.col(pid);
//		vec fctLat = _modelParam._fctLat.col(pid);
		double pBias = _modelParam._powerBias[pid];
//		double fctBias = _modelParam._fctBias[pid];
//		cost += (accu(pLat % pLat) + accu(fctLat % fctLat) + pBias * pBias + fctBias * fctBias);
		cost += (accu(pLat % pLat)  + pBias * pBias);

	}
//	cost += (_modelParam._gPowerBias * _modelParam._gPowerBias + _modelParam._gFctBias * _modelParam._gFctBias);
	cost += (_modelParam._gPowerBias * _modelParam._gPowerBias);
	return cost * _eta;
}

void MNL::_transPredict(const Transaction& trans, const set<int>& allIds,vector<int>& recItems, vector<pair<int,double>>& productR) {
	/// rec by finding top items that gives the most marginal utility
	set<int> transItems;
	transItems.insert(trans._itemIds.begin(),trans._itemIds.end());
	vector<pair<int,double>> itemScore;
	for (auto pk : allIds) {
		double aggUtil = 0;
		/// aggregate the pairwise utility between id and each of transItems
		if (transItems.find(pk) == transItems.end()) {
			for (int j = 0; j < trans._itemIds.size(); j++) {
				double qj = trans._quantAssign(0,j);
				int pj = trans._itemIds[j];
				/// r by CF
				double bjk = cfPower(pj,pk);
				double rjk = 1 - Config::CONFIG_INST._sigBound /(1 + std::exp(-bjk));
				double qjr = Transaction::myPow(qj,rjk);
				double util =  Transaction::myPow(qjr + 1,1/rjk);
				aggUtil += util;
			}
		}
		itemScore.push_back(make_pair(pk,aggUtil));
	}
	/// sort the rec items in descreasing utility order
	std::sort(itemScore.begin(),itemScore.end(),[&itemScore](const pair<int,double>& v1, const pair<int,double>&v2) {
		return v1.second > v2.second;
	});
	/// get the top 50
	int maxResult = 50;
	recItems.resize(maxResult);
	for (int i = 0; i < maxResult; i++) {
		recItems[i] = itemScore[i].first;
	}

	/// get R for each product pair of the transaction
	if (productR.empty()) {
		int maxProdId = *(allIds.rbegin());
		productR.resize(maxProdId + 1,make_pair(0,0));
	}
	/// try all pairs
	int N = trans._itemIds.size();
	for (int j = 0; j < N; j++) {
		int pj = trans._itemIds[j];
		for (int k = j + 1; k < N; k++) {
			int pk = trans._itemIds[k];
			double bjk = cfPower(pj,pk);
			double rjk = 1 - Config::CONFIG_INST._sigBound /(1 + std::exp(-bjk));
			productR[pj].first++;
			productR[pj].second += rjk;
			productR[pk].first++;
			productR[pk].second += rjk;
		}
	}
}

void MNL::predict(const string& resultFolder) {
	/// make top 10 prediction for each transaction
	string predFile = resultFolder + "/Pred.txt";
	ofstream ofs(predFile.c_str(),ofstream::out);
	if (!ofs.good()) {
		cerr << "failed to open prediction result file for writing" << endl;
		exit(1);
	}
	cout << "recommendation for each transaction" << endl;

	set<int> allItemIds;
	for(auto iter = _transactions.begin(); iter != _transactions.end(); ++iter) {
		for(auto id : iter->_itemIds) {
			allItemIds.insert(id);
		}
	}
	cout << "# of candidate items:" << allItemIds.size() << endl;
	/// make top-50 recommendations for each transaction
	int transIdx = 0;
	/// pairwise product R
	vector<pair<int,double>> productR(_numProducts,make_pair(0,0));

	for (auto& trans : _transactions) {
		transIdx++;
		cout << "trans idx:" << transIdx << " out of " << _transactions.size() << endl;
		vector<int> recItems;
		_transPredict(trans,allItemIds,recItems,productR);
		for (int i = 0; i <recItems.size(); i++) {
			int pid = recItems[i];
			ofs << (i == 0 ? "" : ",") << pid;
		}
		ofs << endl;
	}
	ofs.close();
	/// take the average of productR
	string productRFile = resultFolder + "/productR.txt";
	ofs.open(productRFile.c_str(),ofstream::out);
	for (int i = 0; i < productR.size(); i++) {
		pair<int,double>& tmp = productR[i];
		if (tmp.first > 0) {
			tmp.second /= tmp.first;
			ofs << i << "," << tmp.second << endl;
		}
	}
	ofs.close();
}

vector<int> MNL::findSubstitutionPair() {
	/// considering all product pairs in the dataset and print it
	vector<int> result(2);
	double maxR = -100;
	set<int> allItemIdSet;
	vector<int> allIdVec;
	for(auto iter = _transactions.begin(); iter != _transactions.end(); ++iter) {
		for(auto id : iter->_itemIds) {
			allItemIdSet.insert(id);
		}
	}
	for (auto & id : allItemIdSet) {
		allIdVec.push_back(id);
	}
	int N = allIdVec.size();
	for (int j = 0; j < N; j++) {
		int pj = allIdVec[j];
		for (int k = j + 1; k < N; k++) {
			int pk = allIdVec[k];
			double bjk = cfPower(pj,pk);
			double rjk = 1 - Config::CONFIG_INST._sigBound /(1 + std::exp(-bjk));
			if (rjk > maxR) {
				maxR = rjk;
				result[0] = pj;
				result[1] = pk;
				cout << "larger power found:" << maxR << ", they are:" << pj << "-" << pk << endl;
			}
		}
	}
	return result;
}

int MNL::getMaxProductId() {
	int maxId = 0;
	for (auto& trans : _transactions) {
		for (auto & pid : trans._itemIds) {
			if (pid > maxId) {
				maxId = pid;
			}
		}
	}
	return maxId;
}

void MNL::sgd(int maxEpochs) {
	cout << "std optimization..." << endl;
	int T = _transactions.size(); 	//! number of transactions
	int batchSize = 1;	//! minibatch size - perform gradient descent after aggregating the gradient of 10 transactions

	uvec transIndices(T); //! for random visit of transactions
	for (int i = 0; i < T; i++) {
		transIndices[i] = i;
	}

	mat pLatGrad(_dim, _numProducts,fill::zeros); //! product latent vector gradients
	vec pBiasGrad (_numProducts,fill::zeros); //! product bias gradient
	double gBiasGrad = 0; //! global bias gradient

	mat fctLatGrad(_dim, _numProducts,fill::zeros); //! product latent vector gradients
	vec fctBiasGrad (_numProducts,fill::zeros); //! product bias gradient
	double gFctBiasGrad = 0; //! global bias gradient

	int step = 0;
	/// unique product ids in a mini batch
	set<int> batchPids;
	for (int epoch = 0; epoch < maxEpochs; epoch++) {
		uvec shuffledTransIndices = shuffle(transIndices); //! shuffle the transactions
		double fval = 0; //! objective function value
		for (int k = 0; k < shuffledTransIndices.size(); k++) {
			int tIdx = shuffledTransIndices[k]; //! transaction index
			Transaction& trans = _transactions[tIdx];
			for (auto pid : trans._itemIds) {
				batchPids.insert(pid); //! products in current transaction
			}
			step++;
			TransStatus tstatus(_modelParam,trans);
			tstatus.update();
			int N = trans.getItemCnt();
			CFParam powerCFGrad(N,_dim), fctCFGrad(N,_dim);
			transCFGrad(tstatus, powerCFGrad, fctCFGrad); //! get the gradient
			double transNNL = tstatus.nnl();
			double transRegCost = _regCost(trans);
			bool invalidTrans = false;
			if (std::isnan(transNNL) || std::isnan(transRegCost)) {
				cerr << "transNNL:" << transNNL << ",transRegCost:" << transRegCost << endl;
				cerr << "epoch:" << epoch << ",step:" << step << endl;
				invalidTrans = true;
			} else {
				fval += (transNNL + transRegCost);
			}
			_learnRate = Config::CONFIG_INST._initLearnRate/(1+ epoch * 0.1); //! diminishing the learning rate as optimization proceeds
			/// skip nan gradient
			if (!invalidTrans) {
				_aggregateGrad(trans,powerCFGrad,pLatGrad,pBiasGrad,gBiasGrad); //! aggregate the gradient of current transaction
			}

			if ((step % batchSize == 0 && step > 0) || step % T == 0) {
				_applyGrad(batchPids,pLatGrad,pBiasGrad,gBiasGrad,fctLatGrad,fctBiasGrad,gFctBiasGrad); //! update the parameters
				if (std::isnan(_modelParam._gPowerBias)) {
					cout << "global bias:" << this->_modelParam._gPowerBias << endl;
					cout << "nan trans idx:" << step << endl;
					getchar();
				}
			}
		}
		cout << "epoch:" << epoch << ",nnl value:" << fval << ", learning rate:" << _learnRate  << endl;
	}
}



istream& operator>> (istream& iss, MNL& mnl) {
	/// read in the number of transactions, number of products
	/// read in the transactions
	Transaction trans;
	int readCnt = 0;
	int entryCnt = 0;
	set<int> uniqItems;
	while(iss >> trans) {
		readCnt++;
		mnl._transactions.push_back(trans);
//		cout << trans << endl;
//		if(readCnt == 10000) break;
		entryCnt += trans._itemIds.size();
		uniqItems.insert(trans._itemIds.begin(),trans._itemIds.end());
		trans.reset();
	}
	mnl._numProducts = mnl.getMaxProductId() + 1;
	mnl._numTrans = mnl._transactions.size();
	double density = double(entryCnt)/(uniqItems.size() * mnl.getNumTransactions());
	cout << "density:" << density << endl;
	mnl._modelParam.init(mnl._numProducts,mnl._dim);
	return iss;
}
}
