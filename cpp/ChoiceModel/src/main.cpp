//============================================================================
// Name        : ChoiceModel.cpp
// Author      : Qi Zhao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <armadillo>
#include <gtest/gtest.h>
#include "model/MNL.h"
#include "model/Evaluation.h"

using namespace std;

using namespace arma;
using namespace rec;

void utilityEvaluate() {
	string dataset = "shopcom";
	string dataRoot = "/home/qi/git/sigir16/experiment/data/" + dataset;
	string transFile = dataRoot + "/Train.txt";
	string predFile = dataRoot + "/Pred10.txt";
	string utilResFile = dataRoot + "/Pred10Util.txt";
	string modelFilePrefix = dataRoot + "/mp";
	ifstream ifs(transFile.c_str(), ifstream::in);
	if (ifs.good()) {
		MNL mnl(Config::CONFIG_INST._dim, Config::CONFIG_INST._eta);
		ifs >> mnl;
		mnl.loadModel(modelFilePrefix);
		/// now
		Evaluation eval(mnl,predFile,utilResFile);
		eval.evaluate();
	}
}

void train() {
	string dataset = "shopcom";
	string dataRoot = "/home/qi/git/sigir16/experiment/data/" + dataset;
	string transFile = dataRoot + "/Train.txt";
	string modelFilePrefix = dataRoot + "/mp";

	ifstream ifs(transFile.c_str(), ifstream::in);
	if (ifs.good()) {
		MNL mnl(Config::CONFIG_INST._dim, Config::CONFIG_INST._eta);
		cout << "load transactions from file:" << transFile << endl;
		ifs >> mnl;

		cout << "# of products:" << mnl.getNumProducts()
				<< ", # of transactions:" << mnl.getNumTransactions() << endl;

//		mnl.sgd(10);
//		mnl.saveModel(modelFilePrefix);
//		mnl.predict(dataRoot);
	}
}

int main() {
	train();
//	utilityEvaluate();
	return 0;
}
