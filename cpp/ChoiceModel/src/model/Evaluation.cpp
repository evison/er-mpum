/*
 * Evaluation.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: qi
 */

#include "Evaluation.h"

namespace rec {

Evaluation::Evaluation(MNL& mnl,const string& transPredFile, const string& resultFile):_mnl(mnl),_transPredFile(transPredFile)
,_resultFile(resultFile){

}

void Evaluation::evaluate() {
	ifstream ifs(_transPredFile.c_str(),ifstream::in);
	ofstream ofs(_resultFile.c_str(),ofstream::out);

	// ten items each line
	string line;
	int transCnt = _mnl._transactions.size();
	for (int i = 0; i < transCnt; i++) {
		Transaction& trans = _mnl._transactions[i];
		double aggUtil = 0;
		for (int j = 0; j < 10; j++) {
			int pj;
			ifs >> pj;
			/// calculate the utility added by this item
			for (int k = 0; k < trans._itemIds.size(); k++) {
				double qk = trans._quantAssign(0,k);
				int pk = trans._itemIds[k];
				/// r by CF
				double bjk = _mnl.cfPower(pj,pk);
				double rjk = 1 - Config::CONFIG_INST._sigBound /(1 + std::exp(-bjk));
				double qkr = Transaction::myPow(qk,rjk);
				double util =  Transaction::myPow(qkr + 1,1/rjk);
				aggUtil += util;
			}
			/// output the util
			ofs << (j == 0? "" : ",") << aggUtil;
		}
		//! end of transation
		ofs << endl;
	}
	ifs.close();
	ofs.close();
}


Evaluation::~Evaluation() {
	// TODO Auto-generated destructor stub
}

} /* namespace rec */
