#include "pch.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>


using namespace std;


void min_str()
{
	vector<string> str_vec;

	for (int i = 0; i < 3; ++i) {
		string tmp_str;
		cin >> tmp_str;
		str_vec.push_back(tmp_str);
	}

	sort(begin(str_vec), end(str_vec));

	cout << str_vec[0];
}


void equation()
{
	double a, b, c;

	cin >> a >> b >> c;

	if (a != 0) {
		double dis = pow(b, 2) - 4 * a * c;

		if (dis >= 0) {
			double x1 = (-1 * b - sqrt(dis)) / (2 * a);
			double x2 = (-1 * b + sqrt(dis)) / (2 * a);

			if (x1 == x2) {
				cout << x1;
			}
			else {
				cout << x1 << " " << x2;
			}
		}
	}
	else {
		if (b != 0) {
			cout << -1 * c / b;
		}
	}
}


void percents()
{
	double N, A, B, X, Y;

	cin >> N >> A >> B >> X >> Y;

	if (N > A && N <= B) {
		N *= (1 - X / 100);
	}

	if (N > B) {
		N *= (1 - Y / 100);
	}

	cout << N;
}


void even()
{
	int a, b;

	cin >> a >> b;

	string s = "";

	time_t st, en;

	time(&st);
	
	if (a % 2 != 0) {
		a += 1;
	}

	int i = a;

	while (i <= b) {
		s.append((to_string(i)));
		s.push_back(' ');
		i += 2;
	}

	if (!s.empty()) {
		s.pop_back();
	}

	cout << s << endl;

	time(&en);

	cout << difftime(en, st);
}


void second_pos()
{
	string str;

	cin >> str;

	int num_f = 0;
	int ind;
	int count = 0;

	for (auto c : str) {
		if (c == 'f') {
			ind = count;
			num_f += 1;
		}

		if (num_f == 2) {
			break;
		}

		count++;
	}

	if (num_f == 1) {
		ind = -1;
	}

	if (num_f == 0) {
		ind = -2;
	}

	cout << ind;
}


void max_del()
{
	int a, b;

	cin >> a >> b;

	while (a > 0 && b > 0) {
		if (a > b) {
			a %= b;
		}

		if (b > a) {
			b %= a;
		}
	}

	cout << a + b;
}


void dec_to_bin()
{
	int num;

	cin >> num;

	string str;

	while (num != 0) {
		str.append(to_string(num % 2));

		num /= 2;
	}

	reverse(str.begin(), str.end());

	cout << str;
}


int main()
{
	dec_to_bin();
	
	return 0;
}