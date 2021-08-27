#pragma once

#ifndef __Util_H__
#define __Util_H__

#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <algorithm>

#define _DEBUG 1
#define double_MIN -10003.0
typedef unsigned int uint;

using namespace std;

vector<string> split(string line, char delim);

void readRawDataFromCsv(string fileName, vector<vector<double> > &samples, vector<uint> &labels);

void printVector2D(vector<vector<double> > twodArray);

void printVector1D(vector<uint> oneDArray);

void printData(vector<vector<double> > samples, vector<uint> labels);

double sigmoid(double x);

void gradientDescent(
	vector<double> &w,
	double learnRate,
	const vector<vector<double> > &samples,
	const vector<uint> &labels,
	uint maxIterCnt,
	double eps
);

void stochasticGradientDescent(
	vector<double> &w,
	double learnRate,
	const vector<vector<double> > &samples,
	const vector<uint> &labels,
	uint maxIterCnt,
	double eps
);
#endif