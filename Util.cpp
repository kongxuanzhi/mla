
#include "Util.h"

vector<string> split(string line, char delim) {
	string temp = "";
	vector<string> result;

	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] != delim) {
			temp += line[i];
		}
		else {
			result.push_back(temp);
			temp = "";
		}
	}

	if ("" != temp) {
		result.push_back(temp);
	}

	return result;
}

void readRawDataFromCsv(string fileName, vector<vector<double> > &samples, vector<uint> &labels) {
	const char delim = '\t';
	string line;

	ifstream dataFile(fileName.c_str());
	if (!dataFile.is_open()) {
		cout << "File:" + fileName + " open failed" << endl;
		exit(-1);
	}

	while (getline(dataFile, line)) {
		vector<string> features = split(line, delim);
		vector<double> sample;
		for (uint j = 0; j < features.size(); j++) {
			if (j < features.size() - 1) {
				sample.push_back(atof(features[j].c_str()));
			}
			else {
				labels.push_back(atoi(features[j].c_str()));
			}
		}
		samples.push_back(sample);
	}
}

void printVector2D(vector<vector<double> > twodArray) {

	for (uint i = 0; i < twodArray.size(); i++) {
		for (uint j = 0; j < twodArray[0].size(); j++) {
			cout << twodArray[i][j] << '\t';
		}
		cout << endl;
	}
}

void printVector1D(vector<uint> oneDArray) {
	for (uint i = 0; i< oneDArray.size(); i++) {
		cout << oneDArray[i] << endl;
	}
}

void printData(vector<vector<double> > samples, vector<uint> labels) {
	printVector2D(samples);
	printVector1D(labels);
}

double sigmoid(double x) {
	return double(1.0 / (1 + exp(-x)));
}

void gradientDescent(
	vector<double> &w,
	double learnRate,
	const vector<vector<double> > &samples,
	const vector<uint> &labels,
	uint maxIterCnt,
	double eps
)
{
	for (uint i = 0; i < maxIterCnt; i++) {
		vector<double> predictVals(samples.size(), 0.0);
		double originVal = 0.0, preVal = 0.0;
		for (uint i = 0; i < samples.size(); i++) {
			double sum = 0.0;
			for (uint j = 0; j < samples[0].size(); j++) {
				sum += w[j] * samples[i][j];
			}
			double p = sigmoid(sum);
			preVal += labels[i] * log(p) + (1 - labels[i]) * log(1 - p);
			predictVals[i] = p;
		}

		if (fabs(originVal - preVal) < eps) {
			break;
		}

		originVal = preVal;

		for (uint j = 0; j < samples[0].size(); j++) {
			double gradient = 0.0;
			for (uint i = 0; i < samples.size(); i++) {
				gradient += (predictVals[i] - labels[i]) * samples[i][j];
			}

			w[j] += learnRate * gradient / samples.size();
		}
	}
}

void stochasticGradientDescent(
	vector<double> &w,
	double learnRate,
	const vector<vector<double> > &samples,
	const vector<uint> &labels,
	uint maxIterCnt,
	double eps
) {
	for (uint i = 0; i < maxIterCnt; i++) {
		vector<double> predictVals(samples.size(), 0.0);
		for (uint i = 0; i < samples.size(); i++) {
			double sum = 0.0;
			for (uint j = 0; j < samples[0].size(); j++) {
				sum += w[j] * samples[i][j];
			}
			predictVals[i] = sigmoid(sum);
		}
	}
}