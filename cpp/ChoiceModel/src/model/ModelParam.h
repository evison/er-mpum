/*
 * ModelParam.h
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#ifndef MODEL_MODELPARAM_H_
#define MODEL_MODELPARAM_H_
#include "includes.h"

namespace rec {

class ModelParam {
public:
	/// Transaction data
	//! CF parameters for pairwise power (variable b in the paper)
	mat _powerLat; //! product specific latent vectors
	vec _powerBias; //! product specific bias
	/// global bias
	double _gPowerBias; //! global bias

	//! CF parameters for factor productivity model
	mat _fctLat; //! product specific latent vectors
	vec _fctBias; //! product specific bias
	double _gFctBias; //! global bias
public:
	ModelParam(int numProducts = 0, int dim = 0);
	void init(int numProducts, int dim);
	inline vec getPowerCFLat(int idx) const{
		return _powerLat.col(idx);
	}

	inline double getPowerCFBias(int idx) const {
		return _powerBias[idx];
	}

	inline double getPowerCFGBias() const {
		return _gPowerBias;
	}
	/**
	 * calculate the power of the utility of two products
	 * @param	idx1	first product id
	 * @param	idx2	second product id
	 * @return	resulted power
	 */
	double cfPower(int idx1, int idx2) const;
	/// save the model parameters to file

	void save(const string& file);
	void load(const string& file);
	virtual ~ModelParam();
};

double CF(const vec& v1, const vec& v2, double b1, double b2, double b);

} /* namespace model */

#endif /* MODEL_MODELPARAM_H_ */
