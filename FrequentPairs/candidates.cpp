#include <iostream>
#include <cmath>
#include "candidates.h"
using namespace std;

/*
len	num
id1	id2	id3
p1	p2	p3
q1	q2	q3
qc1	qc2	qc3
qc1	qc2	qc3
*/
istream& operator>> (istream& iss, Transaction& trans) {
	iss >> trans.len >> trans.num;
	trans.pid = new string[trans.len];
	trans.p = new float[trans.len];
	trans.q = new int[trans.len];
	trans.qc = new int*[trans.num];
	for (int i = 0; i < trans.len; i++) iss >> trans.pid[i];
	for (int i = 0; i < trans.len; i++) iss >> trans.p[i];
	for (int i = 0; i < trans.len; i++) iss >> trans.q[i];
	for (int i = 0; i < trans.num; i++) {
		trans.qc[i] = new int[trans.len];
		for (int j = 0; j < trans.len; j++) {
			iss >> trans.qc[i][j];
		}
	}
	return iss;
}

ostream& operator<< (ostream& oss, const Transaction& trans) {
	oss << trans.len << " " << trans.num << endl;
	for (int i = 0; i < trans.len; i++) oss << (i == 0 ? "" : " ") << trans.pid[i];
	oss << endl;
	for (int i = 0; i < trans.len; i++) oss << (i == 0 ? "" : " ") << trans.p[i];
	oss << endl;
	for (int i = 0; i < trans.len; i++) oss << (i == 0 ? "" : " ") << trans.q[i];
	oss << endl;
	for (int i = 0; i < trans.num; i++) {
		for (int j = 0; j < trans.len; j++) {
			oss << (j == 0 ? "" : " ") << trans.qc[i][j];
		}
		oss << endl;
	}
	return oss;
}

int max(int* q, int len) {
	int res = -1;
	for (int i = 0; i < len; i++) {
		if (q[i] > res) res = q[i];
	}
	return res;
}

void generate(Transaction* t, int num) {
	int** result = new int*[num];
	int max_q = max(t->q, t->len);
	int delta_q = 0;
	int current_num = 0;
	while (delta_q <= max_q) {
		if (current_num >= num) break;
		delta_q += 1;
		for (int i = 0; i < t->len; i++) {
			if (current_num >= num) break;
			if (t->q[i] - delta_q >= 0) {
				float delta_W = delta_q * t->p[i];
				for (int j = 0; j < t->len; j++) {
					if (j == i) continue;
					if (delta_W >= t->p[j]) {
						int delta_q_j = floor(delta_W / t->p[j]);
						result[current_num] = new int[t->len];
						for (int k = 0; k < t->len; k++) {
							result[current_num][k] = t->q[k];
						}
						result[current_num][i] -= delta_q;
						result[current_num][j] += delta_q_j;
						current_num += 1;
						if (current_num >= num) {
							break;
						}
					}
				}
			}
		}
	}
	// PseudoTransaction* pt = new PseudoTransaction(t, current_num, t->len, result);
	t->num = current_num;
	t->qc = result;
	return;
}

/*
int main() {
	string pid[5] = {"1","2","3","4","5"};
	float p[5] = {3,2,4,1,2};
	int q[5] = {3,4,2,1,5};
	Transaction* t = new Transaction(5, pid, p, q);
	generate(t, 200);
	cout << t->num << endl;
	for (int i = 0; i < t->num; i++) {
		for (int j = 0; j < t->len; j++) {
			cout << t->qc[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
*/