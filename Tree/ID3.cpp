
#include "ID3.h"

void ID3::setData(vector<vector<float> > &samples, vector<int32_t> &labels) {
    this->_samples = samples;
    this->_labels = labels;
    this->_sampleCnt = this->_samples.size();
    this->_featureCnt = this->_samples[0].size();
    this->_labelCnt = 0;
    for (int32_t j = 0; j <　this->_featureCnt; j++) {
        this->_types.push_back(EQUAL);
    }
}

void ID3::setType(int index, int type) {
    this->_types[index] = type;
}

void ID3::setSplitFeatureAndValue() {
    float maxEntrop = FLOAR_MIN;
    int fatureIndex = 0;
    for (int32_t j = 0; j < this->_featureCnt; j++) {
        map<float, map<int32_t, int32_t> > valLabelCnt;
        map<float, int32_t> valCnts;
        for (int32_t i = 0; i < this->_sampleCnt; i++) {
            float value = this->_samples[i][j];
            int32_t label = this->_labels[i];
            //每个值对应的label的个数
            if (this->_types[j] == EQUAL) {
                valLabelCnt[value][label] != NULL ? valLabelCnt[value][label]++ : valLabelCnt[value][label] = 1;
                valCnts[value] != NULL ? valCnts[value]++ : valCnts[value] = 1;
            }
        }

        float entrop = 0.0;
        map<int, map<int32_t, int32_t> >::iterator iter;
        for (iter = valLabelCnt::begin(); iter != valLabelCnt::end(); iter++) {
            float value = iter->first;
            map<int32_t, int32_t> labelCnts = iter->second;
            map<int32_t, int32_t>::iterator labelIter;

            int32_t valueCnt = valCnts[value];
            for (labelIter = labelCnts::begin(); labelIter != labelCnts::end(); labelIter++) {
                int32_t labelCnt = *labelIter;
                float p = 0.1 * labelCnt / valueCnt;
                entrop += 0.1 * valueCnt / _sampleCnt * (- p * log(p));
            }
        }

        if (entrop > maxEntrop) {
            maxEntrop = entrop;
            fatureIndex = j;
        }
    }

    return featureIndex;
}

//关键的一个步骤
void ID3::getGiniVal() 
{

}

void ID3::buildTree();

void ID3::training();

void ID3::predict(vector<float> sample);
