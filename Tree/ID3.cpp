
#include "ID3.h"

void ID3::setData(vector<vector<float> > &samples, vector<int32_t> &labels)
{
    cout <<  "set" << endl;
    this->_samples = samples;
    this->_labels = labels;
    this->_sampleCnt = this->_samples.size();
    this->_featureCnt = this->_samples[0].size();
    this->_labelCnt = 0;
    for (int32_t j = 0; j < this->_featureCnt; j++) {
        this->_types.push_back(EQUAL);
    }

    this->_root = Node();
    this->_root->index = 1;
    this->_root->depth = 1;
    for (int i = 0; i < this->_sampleCnt; i++) {
        this->_root->sampleIndexs.push_back(i);
    }

    for (int j = 0; j < this->_featureCnt; j++) {
        this->_root->featureIndexs.push_back(j);
    }

    this->_root->H = this->getNodeEntrop(this->_root);
}

void ID3::setType(int index, int type) 
{
    this->_types[index] = type;
}

float ID3::getBestSplitFeatureIndex(Node node, int &featureIndex) 
{
    float maxEntrop = FLOAT_MIN;
    float splitVal = -1;
    for (vector<int32_t>::iterator it = node->featureIndexs.begin(); it != node->featureIndexs.end(); it++) {
        float entrop = 0.0;
        if (this->_types[*it] == EQUAL) {
            this->getDiscretFeatureEntrop(node->sampleIndexs, *it, entrop);
            if (entrop > maxEntrop) {
                maxEntrop = entrop;
                featureIndex = *it;
            }
        } else {
            this->getContinuousFeatureEntrop(node->sampleIndexs, *it, entrop);
            if (entrop > maxEntrop) {
                maxEntrop = entrop;
                featureIndex = *it;
            }
        }
    }

    return maxEntrop;
}

map<float, map<int32_t, int32_t> > ID3::getDiscretFeatureEntrop(vector<int32_t> sampleIndexs, int featurej, float &entrop) 
{
    map<float, map<int32_t, int32_t> > valLabelCnt;
    map<float, int32_t> valCnts;
    for (vector<int32_t>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        float value = this->_samples[*it][featurej];
        int32_t label = this->_labels[*it];
        //每个值对应的label的个数
        valLabelCnt[value][label]++;
        valCnts[value]++;
    }

    entrop = 0.0;
    map<float, map<int32_t, int32_t> >::iterator iter;
    for (iter = valLabelCnt.begin(); iter != valLabelCnt.end(); iter++) {
        float value = iter->first;
        int32_t nodeLabelCnt = valCnts[value];
        entrop += this->getLabelMapEntrop(iter->second, nodeLabelCnt); //某一个节点的熵值*该节点的样本占比
    }

    return valLabelCnt;
}

float ID3::getContinuousFeatureEntrop(vector<int32_t> sampleIndexs, int featurej, float &entrop)
{
    vector<float> featureValues;
    for (vector<int32_t>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        featureValues.push_back(this->_samples[*it][featurej]);
    }
    sort(featureValues.begin(), featureValues.end());
    
    float splitVal = 0.0;
    float maxEntrop = FLOAT_MIN;
    for (int k = 0; k < featureValues.size() - 1; k++) {
        entrop = 0.0;
        float midVal = (featureValues[k] + featureValues[k + 1]) / 2.0;
        map<int32_t, int32_t> leftLabelCnts, rightLabelCnts;
        
        //循环未排序的节点， 根据midVal进行分类
        for (vector<int32_t>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
            float featureJValue = this->_samples[*it][featurej];
            if (midVal <= featureJValue) {
                leftLabelCnts[this->_labels[*it]] ++;
            } else {
                rightLabelCnts[this->_labels[*it]] ++;
            }
        }

        entrop = this->getLabelMapEntrop(leftLabelCnts, k + 1) + this->getLabelMapEntrop(rightLabelCnts, sampleIndexs.size() - k + 1);
        if (entrop > maxEntrop) {
            maxEntrop = entrop;
            splitVal = midVal;
        }
    }

    entrop = maxEntrop;

    return splitVal; 
}

//某一个节点的熵值
float ID3::getLabelMapEntrop(map<int32_t, int32_t> labelCnts, int32_t nodeLabelCnt) //[3, 4, 4, 5], 16
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

float ID3::getNodeEntrop(Node node) 
{
    map<int32_t, int32_t> labelCnts;

    for (vector<int32_t>::iterator it = node->sampleIndexs.begin(); it != node->sampleIndexs.end(); it++) {
        labelCnts[this->_labels[*it]] ++;
    }

    return this->getLabelMapEntrop(labelCnts,  node->sampleIndexs.size());
}

Node ID3::buildTree()
{
    int index = 1;
    int featureIndex = -1;
    float bestSplitVal = -1.0;
    queue<Node> tree;
    tree.push(this->_root);
    cout << this->root << "fcdf" << endl;
    while(!tree.empty()) {
        Node node = tree.front();
        tree.pop();
        if (node->featureIndexs.size() == 0 || node->sampleIndexs.size() == 0) continue;
        if (node->depth == 2) continue;
        this->getBestSplitFeatureIndex(node, featureIndex);
        node->condition = { this->_types[featureIndex], featureIndex };

        index = this->buildChildNodes(node, index);
        for (int i = 0; i < node->childs.size(); i++) {
            tree.push(node->childs[i]);
        }
    }

    return this->_root;
}

int ID3::buildChildNodes(Node parentNode, int index) 
{
    int32_t featureIndex = parentNode->condition.feature;
    float entrop = 0.0;
    if (parentNode->condition.type == EQUAL) {
        map<float, map<int32_t, int32_t> > valLabelCnt = this->getDiscretFeatureEntrop(parentNode->sampleIndexs, featureIndex, entrop);
        for (map<float, map<int32_t, int32_t> >::iterator iter = valLabelCnt.begin(); iter != valLabelCnt.end(); iter++) {
            Node node = Node();
            float value = iter->first;
            for (vector<int32_t>::iterator it = parentNode->sampleIndexs.begin(); it != parentNode->sampleIndexs.end(); it++) {
                if (this->_samples[*it][featureIndex] == value) {
                    node->sampleIndexs.push_back(*it);
                }
            }
            parentNode->childs.push_back(node);
        }
    } else {
        Node leftNode = Node();
        Node rightNode = Node();
        float bestSplitVal = this->getContinuousFeatureEntrop(parentNode->sampleIndexs, featureIndex, entrop);
        for (vector<int32_t>::iterator it = parentNode->sampleIndexs.begin(); it != parentNode->sampleIndexs.end(); it++) {
            if (this->_samples[*it][featureIndex] <= bestSplitVal) {
                leftNode->sampleIndexs.push_back(*it);
            } else {
                rightNode->sampleIndexs.push_back(*it);
            }
        }
        parentNode->childs.push_back(leftNode);
        parentNode->childs.push_back(rightNode);
    }

    vector<int32_t> remineFeatures;
    for (vector<int32_t>::iterator it = parentNode->featureIndexs.begin(); it != parentNode->featureIndexs.end(); it++) {
        if (*it != featureIndex) {
            remineFeatures.push_back(*it);
        }
    }

    for (int i = 0; i < parentNode->childs.size(); i++) {
        parentNode->childs[i]->index = ++index;
        parentNode->childs[i]->depth = parentNode->depth + 1;
        parentNode->childs[i]->featureIndexs = remineFeatures;
        parentNode->childs[i]->H = this->getNodeEntrop(parentNode->childs[i]);
    }

    return index;
}

void ID3::training() {
    
}

//1. 将样本落在某个叶子节点上，
//2. 计算该节点中label占比较多的，即为分类样本。 如果做回归的话，这里求的是叶子节点label的均值。
int32_t ID3::predict(vector<float> sample) {
    int featurej = this->_root->condition.feature;
    float value = sample[featurej];
    for (int i = 0; i < this->_root->childs.size(); i++) {
        Node child = this->_root->childs[i];
        if (this->_root->condition.type == EQUAL) {
           if (this->_samples[child->sampleIndexs[0]][featurej] == value) {
                map<int32_t, int32_t> labelCnts;
                for (vector<int32_t>::iterator it = child->sampleIndexs.begin(); it != child->sampleIndexs.end(); it++) {
                    labelCnts[this->_labels[*it]]++;
                }

                int32_t maxLabelCnt = INT_MIN;
                int32_t label = -1;
                for (map<int32_t, int32_t>::iterator labelIter = labelCnts.begin(); labelIter != labelCnts.end(); labelIter++) {
                    int32_t labelCnt = labelIter->second;
                    if (labelCnt > maxLabelCnt) {
                        maxLabelCnt = labelCnt;
                        label = labelIter->first;
                    }
                }

                return label;
            }
        } else {
            if (this->_samples[child->sampleIndexs[0]][featurej] <= value) {

            }
        }
    }
}
