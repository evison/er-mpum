/*
 * CFParam.h
 *
 *  Created on: Jan 19, 2016
 *      Author: qi
 */

#ifndef MODEL_CFPARAM_H_
#define MODEL_CFPARAM_H_

#include "includes.h"

namespace rec {

class CFParam {
public:
	/// latent vectors
	mat _latGrad;
	/// product bias
	vec _biasGrad;
	double _gBiasGrad;
public:
	CFParam(int numObjs, int dim);
	CFParam operator* (double s) const;
	CFParam& operator*= (double s);
	CFParam operator+ (const CFParam& rhs) const;
	CFParam operator- (const CFParam& rhs) const;
	CFParam& operator+= (const CFParam& rhs);
	CFParam& operator-= (const CFParam& rhs);
	CFParam& operator=(double s);
	bool operator == (const CFParam& rhs) const;
};

typedef CFParam CFGradient;

ostream& operator<< (ostream& oss, const CFParam& tg);

}
#endif /* MODEL_CFPARAM_H_ */
