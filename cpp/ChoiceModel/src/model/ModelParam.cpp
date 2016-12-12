/*
 * ModelParam.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#include "ModelParam.h"

namespace rec {

ModelParam::ModelParam(int numProducts, int dim) {
	init(numProducts,dim);
}

void ModelParam::init(int numProducts, int dim) {
	/// initialize parameters
	if (numProducts > 0) {
		cout << "latent dimension:" << dim << endl;
		_powerLat = mat(dim, numProducts,arma::fill::randu);
		_powerLat *= 1e-3;
		_powerBias = vec(numProducts,arma::fill::randu);
		_powerBias *= 1e-3;
		_gPowerBias = 0;

		//! CF on the factor productivity parameter of CES utility formulation
		_fctLat = mat(dim, numProducts,arma::fill::randu);
		_fctBias = vec(numProducts,arma::fill::randu);
		_gFctBias = 0;
	}
}

/**
 * power of a given product pair, it corresponds to the argument of exponential function of Eq. 12
 * the actual power for CES is: 1 - exp(cfPower(idx1,idx2)
 */
double ModelParam::cfPower(int idx1, int idx2) const {
	const vec& v1 = getPowerCFLat(idx1);
	const vec& v2 = getPowerCFLat(idx2);
	double b1 = getPowerCFBias(idx1);
	double b2 = getPowerCFBias(idx2);
	double b0 = getPowerCFGBias();
	return CF(v1,v2,b1,b2,b0);
}

void ModelParam::save(const string& filePrefix) {
	/// save each model paremters
	string latFile = filePrefix + "_LAT_MAT.csv";
	string biasFile = filePrefix + "_BIAS_VEC.csv";
	string gBiasFile = filePrefix + "_GBIAS_S.csv";
	cout << "start to save the model parmeters to file\n";
	_powerLat.save(latFile,arma::csv_ascii);
	_powerBias.save(biasFile,csv_ascii);
	vec gBiasVec(1);
	gBiasVec[0] = _gPowerBias;
	gBiasVec.save(gBiasFile,csv_ascii);
	cout << "end of saving" << endl;
}

void ModelParam::load(const string& filePrefix) {
	string latFile = filePrefix + "_LAT_MAT.csv";
	string biasFile = filePrefix + "_BIAS_VEC.csv";
	string gBiasFile = filePrefix + "_GBIAS_S.csv";
	cout << "start to load the model parmeters to file\n";
	_powerLat.load(latFile,arma::csv_ascii);
	_powerBias.load(biasFile,csv_ascii);
	vec gBiasVec;
	gBiasVec.load(gBiasFile,csv_ascii);
	_gPowerBias = gBiasVec[0];
	cout << "end of loading" << endl;
}

ModelParam::~ModelParam() {
	// TODO Auto-generated destructor stub
}


double CF(const vec& v1, const vec& v2, double b1, double b2, double b) {
	return accu(v1 % v2) + b1 + b2 + b;
}

} /* namespace model */
