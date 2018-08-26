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

using namespace std;

vector<string> split(string line, char delim) {
    string temp = "";
    vector<string> result;

    for (int32_t i = 0; i < line.length(); i++) {
        if (line[i] != delim) {
            temp += line[i];
        } else {
            result.push_back(temp);
            temp = "";
        }
    }

    if ("" != temp) {
        result.push_back(temp);
    }

    return result;
}

void readRawDataFromCsv(string fileName, vector<vector<float> > &samples, vector<int32_t> &labels) {
    const char delim = ',';
    string line;

    ifstream dataFile(fileName.c_str()); 
    if (!dataFile.is_open()) {
        cout << "File:" + fileName + " open failed" << endl;
        exit(-1);
    }

    while(getline(dataFile, line)) {
        vector<string> features = split(line, delim);
        vector<float> sample;
        for (int32_t j = 0; j < features.size(); j++) {
            if (j < features.size() - 1) {
                sample.push_back(atof(features[j].c_str()));
            } else {
                labels.push_back(atoi(features[j].c_str()));
            }
        }
        samples.push_back(sample);
    }
}

void printVector2D(vector<vector<float> > twodArray) {

    for (int32_t i = 0; i < twodArray.size(); i++) {
        for (int32_t j = 0; j < twodArray[0].size(); j++) {
            cout << twodArray[i][j] << '\t';
        }
        cout << endl;
    }
}

void printVector1D(vector<int32_t> oneDArray) {
    for (int32_t  i = 0 ; i< oneDArray.size(); i++) {
        cout << oneDArray[i] << endl;
    }
}

void printData(vector<vector<float> > samples, vector<int32_t> labels) {
    printVector2D(samples);
    printVector1D(labels);
}

float sigmoid(float x) {
    return 1.0 / (1 + exp(-x));
}

void gradientDescent(
    vector<float> &w, 
    float learnRate, 
    const vector<vector<float> > &samples, 
    const vector<int32_t> &labels,
    int32_t maxIterCnt,
    float eps
 ) 
{
    for (int32_t i = 0; i < maxIterCnt; i++) {
        vector<float> predictVals(samples.size(), 0.0);
        float originVal = 0.0, preVal = 0.0;
        for (int32_t i = 0; i < samples.size(); i++) {
            float sum = 0.0;
            for (int32_t j = 0; j < samples[0].size(); j++ ) {
                sum += w[j] * samples[i][j];
            }
            float p = sigmoid(sum);
            preVal += labels[i] * log(p) + (1 - labels[i]) * log(1 - p);
            predictVals[i] = p;
        }

        if (fabs(originVal - preVal) < eps) {
            break;
        }

        originVal = preVal;

        for (int32_t j = 0; j < samples[0].size(); j++) {
            float gradient = 0.0;
            for (int32_t i = 0; i < samples.size(); i++) {
                gradient += (predictVals[i] - labels[i]) * samples[i][j];
            }

            w[j] += learnRate * gradient/samples.size();
        }
    }
}

void stochasticGradientDescent(
    vector<float> &w, 
    float learnRate, 
    const vector<vector<float> > &samples, 
    const vector<int32_t> &labels,
    int32_t maxIterCnt,
    float eps
) {
    for (int32_t i = 0; i < maxIterCnt; i++) {
        vector<float> predictVals(samples.size(), 0.0);
        for (int32_t i = 0; i < samples.size(); i++) {
            float sum = 0.0;
            for (int32_t j = 0; j < samples[0].size(); j++ ) {
                sum += w[j] * samples[i][j];
            }
            predictVals[i] = sigmoid(sum);
        }
    }
}
#endif