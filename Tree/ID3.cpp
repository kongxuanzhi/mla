#include "ID3.h"

void ID3::setData(vector<vector<double> > &samples, vector<uint> &labels)
{
    this->_samples = samples;
    this->_labels = labels;
    this->_sampleCnt = this->_samples.size();
    this->_featureCnt = this->_samples[0].size();
    this->_labelCnt = 0;
    for (uint j = 0; j < this->_featureCnt; j++) {
        this->_types.push_back(EQUAL);
    }

	this->_root = new Node();
	this->_root->index = 1;
	this->_root->depth = 1;
	for (uint i = 0; i < this->_sampleCnt; i++) {
		this->_root->sampleIndexs.push_back(i);
	}

	for (uint j = 0; j < this->_featureCnt; j++) {
		this->_root->featureIndexs.push_back(j);
	}

	this->_root->H = this->getNodeEntrop(this->_root);
}

void ID3::setType(int index, int type) 
{
    this->_types[index] = type;
}

double ID3::getBestSplitFeatureIndex(PNode node, int &featureIndex) 
{
    double maxEntrop = double_MIN;
    double splitVal = -1;
    for (vector<uint>::iterator it = node->featureIndexs.begin(); it != node->featureIndexs.end(); it++) {
        double entrop = 0.0;
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

		cout << *it << ": " << node->H - entrop << endl;
    }

    return maxEntrop;
}

map<double, map<uint, uint> > ID3::getDiscretFeatureEntrop(vector<uint> sampleIndexs, int featurej, double &entrop) 
{
    map<double, map<uint, uint> > valLabelCnt;
    map<double, uint> valCnts;
    for (vector<uint>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        double value = this->_samples[*it][featurej];
        uint label = this->_labels[*it];
        //每个值对应的label的个数
        valLabelCnt[value][label]++;
        valCnts[value]++;
    }

    entrop = 0.0;
    map<double, map<uint, uint> >::iterator iter;
    for (iter = valLabelCnt.begin(); iter != valLabelCnt.end(); iter++) {
        double value = iter->first;
        uint nodeLabelCnt = valCnts[value];
		entrop += this->getLabelMapEntrop(iter->second, nodeLabelCnt);; //某一个节点的熵值*该节点的样本占比
    }

    return valLabelCnt;
}

double ID3::getContinuousFeatureEntrop(vector<uint> sampleIndexs, int featurej, double &entrop)
{
    vector<double> featureValues;
    for (vector<uint>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
        featureValues.push_back(this->_samples[*it][featurej]);
    }
    sort(featureValues.begin(), featureValues.end());
    
    double splitVal = 0.0;
    double maxEntrop = double_MIN;
    for (uint k = 0; k < featureValues.size() - 1; k++) {
        entrop = 0.0;
        double midVal = (featureValues[k] + featureValues[k + 1]) / 2.0;
        map<uint, uint> leftLabelCnts, rightLabelCnts;
        
        //循环未排序的节点， 根据midVal进行分类
        for (vector<uint>::iterator it = sampleIndexs.begin(); it != sampleIndexs.end(); it++) {
            double featureJValue = this->_samples[*it][featurej];
            if (midVal >= featureJValue) {
                leftLabelCnts[this->_labels[*it]] ++;
            } else {
                rightLabelCnts[this->_labels[*it]] ++;
            }
        }

        entrop = this->getLabelMapEntrop(leftLabelCnts, k + 1) + this->getLabelMapEntrop(rightLabelCnts, sampleIndexs.size() - k - 1);
        if (entrop > maxEntrop) {
            maxEntrop = entrop;
            splitVal = midVal;
        }
    }

    entrop = maxEntrop;

    return splitVal; 
}

//某一个节点的熵值
double ID3::getLabelMapEntrop(map<uint, uint> labelCnts, uint nodeLabelCnt) //[3, 4, 4, 5], 16
{
    if (nodeLabelCnt == 0) return 0.0;

    double entrop = 0.0;
    map<uint, uint>::iterator labelIter;
    for (labelIter = labelCnts.begin(); labelIter != labelCnts.end(); labelIter++) {
        uint labelCnt = labelIter->second;
        double p = 1.0 * labelCnt / nodeLabelCnt;
        entrop += this->getGiniVal(p);
    }

    return 1.0 * nodeLabelCnt / this->_sampleCnt * entrop;
}

//return -1.0 *  p * log(p);
//关键的一个步骤
double ID3::getGiniVal(double p) 
{
    return -1.0 *  p * log(p)/log(2);//-1.0 *  p * p;
}

double ID3::getNodeEntrop(PNode node) 
{
    map<uint, uint> labelCnts;

    for (vector<uint>::iterator it = node->sampleIndexs.begin(); it != node->sampleIndexs.end(); it++) {
        labelCnts[this->_labels[*it]] ++;
    }

    return this->getLabelMapEntrop(labelCnts,  node->sampleIndexs.size());
}

PNode ID3::buildTree()
{
    int index = 1;
    int featureIndex = -1;
    double bestSplitVal = -1.0;
    queue<PNode> tree;
    tree.push(this->_root);
    cout << this->_root << "fcdf" << endl;
    while(!tree.empty()) {
        PNode node = tree.front();
        tree.pop();
        if (node->featureIndexs.size() == 0 || node->sampleIndexs.size() == 0) continue;
        if (node->depth == 2) continue;
        this->getBestSplitFeatureIndex(node, featureIndex);
        node->condition = { this->_types[featureIndex], featureIndex };

        index = this->buildChildNodes(node, index);
        for (uint i = 0; i < node->childs.size(); i++) {
            tree.push(node->childs[i]);
        }
    }

    return this->_root;
}

int ID3::buildChildNodes(PNode parentNode, int index) 
{
    uint featureIndex = parentNode->condition.feature;
    double entrop = 0.0;
    if (parentNode->condition.type == EQUAL) {
        map<double, map<uint, uint> > valLabelCnt = this->getDiscretFeatureEntrop(parentNode->sampleIndexs, featureIndex, entrop);
        for (map<double, map<uint, uint> >::iterator iter = valLabelCnt.begin(); iter != valLabelCnt.end(); iter++) {
            PNode node = PNode();
            double value = iter->first;
            for (vector<uint>::iterator it = parentNode->sampleIndexs.begin(); it != parentNode->sampleIndexs.end(); it++) {
                if (this->_samples[*it][featureIndex] == value) {
                    node->sampleIndexs.push_back(*it);
                }
            }
            parentNode->childs.push_back(node);
        }
    } else {
        PNode leftNode = PNode();
        PNode rightNode = PNode();
        double bestSplitVal = this->getContinuousFeatureEntrop(parentNode->sampleIndexs, featureIndex, entrop);
        for (vector<uint>::iterator it = parentNode->sampleIndexs.begin(); it != parentNode->sampleIndexs.end(); it++) {
            if (this->_samples[*it][featureIndex] <= bestSplitVal) {
                leftNode->sampleIndexs.push_back(*it);
            } else {
                rightNode->sampleIndexs.push_back(*it);
            }
        }
        parentNode->childs.push_back(leftNode);
        parentNode->childs.push_back(rightNode);
    }

    vector<uint> remineFeatures;
    for (vector<uint>::iterator it = parentNode->featureIndexs.begin(); it != parentNode->featureIndexs.end(); it++) {
        if (*it != featureIndex) {
            remineFeatures.push_back(*it);
        }
    }

    for (uint i = 0; i < parentNode->childs.size(); i++) {
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
uint ID3::predict(vector<double> sample) {
    int featurej = this->_root->condition.feature;
    double value = sample[featurej];
    for (uint i = 0; i < this->_root->childs.size(); i++) {
        PNode child = this->_root->childs[i];
        if (this->_root->condition.type == EQUAL) {
           if (this->_samples[child->sampleIndexs[0]][featurej] == value) {
                map<uint, uint> labelCnts;
                for (vector<uint>::iterator it = child->sampleIndexs.begin(); it != child->sampleIndexs.end(); it++) {
                    labelCnts[this->_labels[*it]]++;
                }

                uint maxLabelCnt = INT_MIN;
                uint label = -1;
                for (map<uint, uint>::iterator labelIter = labelCnts.begin(); labelIter != labelCnts.end(); labelIter++) {
                    uint labelCnt = labelIter->second;
                    if (labelCnt > maxLabelCnt) {
                        maxLabelCnt = labelCnt;
                        label = labelIter->first;
                    }
                }

                return label;
            }
        } else {
            if (this->_samples[child->sampleIndexs[0]][featurej] <= value) {
				return 1;
            }
        }
    }

	return 1;
}
