/*
 * CFParam.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#include "CFParam.h"

namespace rec {


CFParam::CFParam(int numProds, int dim):_latGrad(dim,numProds,fill::zeros),_biasGrad(numProds,fill::zeros),_gBiasGrad(0){

}

CFParam& CFParam::operator=(double s) {
	_latGrad = s;
	_biasGrad = s;
	_gBiasGrad = s;
	return *this;
}

bool CFParam::operator == (const CFParam& rhs) const{
	if (all(vectorise(_latGrad - rhs._latGrad) == 0) && all(_biasGrad - rhs._biasGrad == 0) && _gBiasGrad == rhs._gBiasGrad) {
		return true;
	}
	return false;
}


CFParam CFParam::operator* (double s) const{
	/// do the scalar product for each component
	CFParam result(*this);
	result._latGrad *= s;
	result._biasGrad *= s;
	result._gBiasGrad *= s;
	return result;
}

CFParam CFParam::operator+ (const CFParam& rhs) const{
	/// do the scalar product for each component
	CFParam result(*this);
	result._latGrad += rhs._latGrad;
	result._biasGrad += rhs._biasGrad;
	result._gBiasGrad += rhs._gBiasGrad;
	return result;
}

CFParam CFParam::operator- (const CFParam& rhs) const{
	/// do the scalar product for each component
	CFParam result(*this);
	result._latGrad -= rhs._latGrad;
	result._biasGrad -= rhs._biasGrad;
	result._gBiasGrad -= rhs._gBiasGrad;
	return result;
	return result;
}

CFParam& CFParam::operator*= (double s){
	/// do the scalar product for each component
	_latGrad *= s;
	_biasGrad *= s;
	_gBiasGrad *= s;
	return *this;
}

CFParam& CFParam::operator+= (const CFParam& rhs){
	/// do the scalar product for each component
	_latGrad += rhs._latGrad;
	_biasGrad += rhs._biasGrad;
	_gBiasGrad += rhs._gBiasGrad;
	return *this;
}

CFParam& CFParam::operator-= (const CFParam& rhs){
	/// do the scalar product for each component
	_latGrad -= rhs._latGrad;
	_biasGrad -= rhs._biasGrad;
	_gBiasGrad -= rhs._gBiasGrad;
	return *this;
}

ostream& operator<< (ostream& oss, const CFParam& tg) {
	oss << "latent grad:" << endl;
	oss << tg._latGrad << endl;
	oss << "bias grad:" << endl;
	oss << tg._biasGrad << endl;
	oss << "global bias:" << endl;
	oss << tg._gBiasGrad << endl;
	return oss;
}


} /* namespace model */
