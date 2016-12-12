/*
 * Expression.h
 *
 *  Created on: Jan 14, 2016
 *      Author: qi
 */

#ifndef MODEL_EXPRESSION_H_
#define MODEL_EXPRESSION_H_

#include "armadillo"
#include "math.h"

namespace rec {

using namespace arma;
using namespace std;

class CONST_OP {};
class VAR_OP {};
class NULL_OP {};
class PLUS_OP{};
class MINUS_OP {};
class MUL_OP {};
class DIV_OP {};
class POW_OP {};
class LOG_OP {};
class EXP_OP {};

//class Param {
//protected:
//	vec& _vals;
//	// begin index
//	int _begInd;
//	// length
//	int _len;
//public:
//	Param(vec& vals, int begInd, int len):_vals(vals),_begInd(begInd),_len(len){}
//
//	Param& operator+ (const Param& rhs) {
//
//	}
//};
/**
 * expression for simple gradients
 */
template <class T1, class T2, class T3>
class Expression {
public:
	typedef T1 FIRST_OP_TYPE;
	typedef T2 SECOND_OP_TYPE;
	typedef T3 EXP_REL_TYPE;
protected:
	FIRST_OP_TYPE& _op1;
	SECOND_OP_TYPE& _op2;
protected:
	double _fval(PLUS_OP dummy) {
		return _op1.fval() + _op2.fval();
	}

	void _grad(PLUS_OP dummy) {

	}

	double _fval(MINUS_OP dummy) {
		return _op1.fval() - _op2.fval();
	}

	double _fval(MUL_OP dummy) {
		return _op1.fval() * _op2.fval();
	}

	double _fval(DIV_OP dummy) {
		return _op1.fval() / _op2.fval();
	}

	double _fval(POW_OP dummy) {
		return pow(_op1.fval(),_op2.fval());
	}

	double _fval(LOG_OP dummy) {
		return log(_op1.fval());
	}

	double _fval(EXP_OP dummy) {
		return exp(_op1.fval());
	}

public:
	Expression(FIRST_OP_TYPE& op1, SECOND_OP_TYPE& op2)
	:_op1(op1),_op2(op2) {	}
	/*
	 * function value
	 */
	double fval() {
		return _fval(EXP_REL_TYPE());
	}

	/*
	 * get gradient
	 */
	void grad() {

	}
	virtual ~Expression();
};

} /* namespace model */

#endif /* MODEL_EXPRESSION_H_ */
