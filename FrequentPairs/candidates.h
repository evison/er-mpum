#ifndef _CANDIDATES_H_
#define _CANDIDATES_H_

#include <string>
using namespace std;

class Transaction {
public:
	int len;	//number of products
	string* pid;	//product id
	float* p;	//price
	int* q;	//quantity
	float W;	//Budget
	int num;	//number of candidates
	int** qc; //candidate matrix

	Transaction() {
		this->len = 0;
		this->pid = NULL;
		this->p = NULL;
		this->q = NULL;
		this->W = 0;
		this->num = 0;
		this->qc = NULL;
	}

	Transaction(const Transaction& t) {
		this->len = t.len;
		this->pid = t.pid;
		this->p = t.p;
		this->q = t.q;
		this->W = t.W;
		this->num = t.num;
		this->qc = t.qc;
	}

	Transaction(int len) {
		this->len = len;
		this->pid = new string[len];
		this->p = new float[len];
		this->q = new int[len];
		this->W = 0.0;
		this->num = 0;
		this->qc = NULL;
	}

	Transaction(int len, string* pid, float* p, int* q) {
		this->len = len;
		this->pid = pid;
		this->p = p;
		this->q = q;
		this->W = 0.0;
		for (int i = 0; i < len; i++) {
			this->W += this->p[i] * this->q[i];
		}
		this->num = 0;
		this->qc = NULL;
	}

	~Transaction(){
		delete this->pid;
		delete this->p;
		delete this->q;
		for (int i = 0; i < num; i++) {
			delete this->qc[i];
		}
		delete qc;
	}
};

/*
class PseudoTransaction {
public:
	Transaction* trans;
	int num;
	int len;
	int** q;

	PseudoTransaction(Transaction* trans, int num, int len, int** q) {
		this->trans = trans;
		this->num = num;
		this->len = len;
		this->q = q;
	}
};
*/

istream& operator>> (istream& iss, Transaction& trans);
ostream& operator<< (ostream& oss, const Transaction& trans);

void generate(Transaction* t, int num);

#endif