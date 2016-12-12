/*
 * Evaluation.h
 *
 *  Created on: Jan 21, 2016
 *      Author: qi
 */

#ifndef MODEL_EVALUATION_H_
#define MODEL_EVALUATION_H_

#include "MNL.h"

namespace rec {

class Evaluation {
public:
	MNL& _mnl;
	string _transPredFile;
	string _resultFile;
public:
	Evaluation(MNL& mnl,const string& transPredFile, const string& resultFile);
	void evaluate();
	virtual ~Evaluation();
};

} /* namespace rec */

#endif /* MODEL_EVALUATION_H_ */
