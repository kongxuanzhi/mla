
#include "ID3.h"

void ID3::setData(vector<vector<float> > &samples, vector<int32_t> &labels) {
    this->_samples = samples;
    this->_labels = labels;
    this->_sampleCnt = this->_samples.size();
    this->_featureCnt = this->_samples[0].size();
    this->_labelCnt = 0;
    for (int32_t j = 0; j < this->_featureCnt; j++) {
        this->_types.push_back(EQUAL);
    }
}

void ID3::setType(int index, int type) {
    this->_types[index] = type;
}

float ID3::setSplitFeatureAndValue(Node node, int &featureIndex, float &bestSplitVal) {
    float maxEntrop = FLOAT_MIN;
    float entrop = -1;
    float splitVal = -1;
    for (vector<int32_t>::iterator it = node->featureIndexs.begin(); it != node->featureIndexs.end(); it++) {
        if (this->_types[*it] == EQUAL) {
            entrop = this->getDiscretFeatureEntrop(node->sampleIndexs, *it);
            if (entrop > maxEntrop) {
                maxEntrop = entrop;
                featureIndex = *it;
            }
        } else {
            entrop = this->getContinuousFeatureEntrop(node->sampleIndexs, *it, splitVal);
            if (entrop > maxEntrop) {
                maxEntrop = entrop;
                featureIndex = *it;
                bestSplitVal = splitVal;
            }
        }
    }

    return entrop;
}

float ID3::getDiscretFeatureEntrop(vector<int32_t> sampleIndexs, int featurej) {
    map<float, map<int32_t, int32_t> > valLabelCnt;
    map<float, int32_t> valCnts;
    for (vector<int32_t>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        float value = this->_samples[*it][featurej];
        int32_t label = this->_labels[*it];
        //每个值对应的label的个数
        valLabelCnt[value][label] != NULL ? valLabelCnt[value][label]++ : valLabelCnt[value][label] = 1;
        valCnts[value] != NULL ? valCnts[value]++ : valCnts[value] = 1;
    }

    float entrop = 0.0;
    map<float, map<int32_t, int32_t> >::iterator iter;
    for (iter = valLabelCnt.begin(); iter != valLabelCnt.end(); iter++) {
        float value = iter->first;
        int32_t nodeLabelCnt = valCnts[value];
        entrop += this->getOneNodeEntrop(iter->second, nodeLabelCnt); //某一个节点的熵值*该节点的样本占比
    }

    return entrop;
}

float ID3::getContinuousFeatureEntrop(vector<int32_t> sampleIndexs, int featurej, float &splitVal)
{
    vector<float> featureValues;
    for (vector<int32_t>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        featureValues.push_back(this->_samples[*it][featurej]);
    }
    sort(featureValues.begin(), featureValues.end());

    float maxEntrop = FLOAT_MIN;
    for (int k = 0; k < featureValues.size() - 1; k++) {
        float entrop = 0.0;
        float midVal = (featureValues[k] + featureValues[k + 1]) / 2.0;
        map<int32_t, int32_t> leftLabelCnts, rightLabelCnts;
        //这里有严重的问题， 1. 要对节点的label进行和featurej特征一起排序，计算label的熵值
        for (int i = 0; i < featureValues.size(); i++) {
            if (i <= k) {
                leftLabelCnts[this->_labels[i]] != NULL ? leftLabelCnts[this->_labels[i]] ++ : leftLabelCnts[this->_labels[i]] = 0;
            } else {
                rightLabelCnts[this->_labels[i]] != NULL ? rightLabelCnts[this->_labels[i]] ++ : rightLabelCnts[this->_labels[i]] = 0;
            }
        }

        entrop = this->getOneNodeEntrop(leftLabelCnts, k + 1) + this->getOneNodeEntrop(rightLabelCnts, featureValues.size() - k + 1);
        if (entrop > maxEntrop) {
            maxEntrop = entrop;
            splitVal = midVal;
        }
    }

    return maxEntrop; 
}

//某一个节点的熵值
float ID3::getOneNodeEntrop(map<int32_t, int32_t> labelCnts, int32_t nodeLabelCnt)
{
    if (nodeLabelCnt == 0) return 0.0;

    float entrop = 1.0;
    map<int32_t, int32_t>::iterator labelIter;
    for (labelIter = labelCnts.begin(); labelIter != labelCnts.end(); labelIter++) {
        int32_t labelCnt = labelIter->second;
        float p = 0.1 * labelCnt / nodeLabelCnt;
        entrop += this->getGiniVal(p);
    }

    return 0.1 * nodeLabelCnt / this->_sampleCnt * entrop;
}

//关键的一个步骤
float ID3::getGiniVal(float p) 
{
    return -1.0 *  p * p;
}

void ID3::splitData(int featureIndex, float splitVal) {

}

void ID3::buildTree()
{
    int index = 1;
    queue<Node> tree;
    Node node = Node();
    node -> index = index ++;
    for (int  i = 0; i < this->_sampleCnt; i++) {
        this->_root -> sampleIndexs.push_back(i);
    }

    int featureIndex = -1;
    float bestSplitVal = -1;

    node->H = this->setSplitFeatureAndValue(featureIndex, bestSplitVal);

    node->condition = this->_types[featureIndex] != EQUAL

    tree.push(this-> _root);
    while(!tree.empty()) {
        Node node = tree.front();
        tree.pop();

        for ()
        Node child = Node();
        child.sampleIndexs = 
        child.labelCnts = 
        child.index = index ++;
        child.sampleIndexs = 
        node->childs.push_back();
    }
}

void ID3::createNode(int index, vector<int> sampleIndex)
{

}

void ID3::training();

void ID3::predict(vector<float> sample);
