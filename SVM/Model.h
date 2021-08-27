#pragma once
#ifndef __MODEL_H_
#define __MODEL_H_

#include "Util.h"

template <class T>
class Model {
public:
	Model() {}
	virtual ~Model() {}

	int32_t _sampleCnt;
	int32_t _featureCnt;

	vector<std::vector<float> > _samples;
	vector<T> _labels;

	void setData(vector<vector<float> > &samples, vector<T> &labels);

	virtual void train() {}

	virtual float predict(const vector<float> &sample) {}
};


template <class T>
void Model<T>::setData(vector<vector<float> > &samples,
	vector<T> &label) 
{
	this->_samples = samples;
	this->_labels = labels;
	this->_sampleCnt = (int32_t) samples.size();
	
	if (!this->_sampleCnt) {
		exit(0);
	}

	this->_featureCnt = (int32_t)samples[0].size();
}

#endif
