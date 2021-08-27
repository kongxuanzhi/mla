#include "RegTree.h"

namespace mla {
namespace tree {

void RegressionTree::optSplitPos(int &nOptFeatureIndex,
            float &fOptFeatureVal,
            std::vector<int32_t> &vCurrentIndex,
            std::vector<int32_t> &vFeatureIndex) {    
    
    float minDevia = INT_MAX;
    
    // sample the feature
    std::vector<int32_t> vTempFeatureIndex;
    
    if (getEnsemble()) {
        srand( (unsigned)time(NULL));
        for (int32_t i = 0; i < vFeatureIndex.size(); ++i) {
            int32_t t = rand() % vFeatureIndex.size();
            std::swap(vFeatureIndex[i], vFeatureIndex[t]);
        }
        vTempFeatureIndex.assign(vFeatureIndex.begin(), vFeatureIndex.begin() + getRandFeatureCnt());
    } else {
        vTempFeatureIndex.assign(vFeatureIndex.begin(), vFeatureIndex.end());    
    }
        
    std::cout << getRandFeatureCnt() << " " << std::endl;
    std::cout << vTempFeatureIndex.size() << std::endl;
    for (int32_t i = 0; i < (int32_t)vTempFeatureIndex.size(); i ++) {
        std::cout << vTempFeatureIndex[i] << " ";    
        std::map<int32_t, float> tmpFeatureValue;
        for (int32_t j = 0; j < vCurrentIndex.size(); j ++) {
            float tmpVal = getTrainingX()[vCurrentIndex[j]][vTempFeatureIndex[i]];
            tmpFeatureValue[vCurrentIndex[j]] = tmpVal;
        }
        
        std::vector<int32_t> vTempCurrentIndex(vCurrentIndex.begin(), vCurrentIndex.end());
        sortIndexVec(vTempCurrentIndex, tmpFeatureValue);
        float totValue = 0.0;
        float totSqaValue = 0.0;
        for (int32_t j = 0; j < (int32_t)vTempCurrentIndex.size(); j ++) {
            float tmpVal = getTrainingY()[vTempCurrentIndex[j]];
            totValue += tmpVal;
            totSqaValue += tmpVal * tmpVal;
        }
        float curTotVal = 0.0;
        float curTotSqaVal = 0.0;
        float minDeviaTemp = INT_MAX, featureVal = 0.0;
        for (int32_t j = 0; j < (int32_t)vTempCurrentIndex.size(); ++ j) {
            float tmpVal = getTrainingY()[vTempCurrentIndex[j]];
            curTotVal += tmpVal;
            curTotSqaVal += tmpVal * tmpVal;
            
            float curDevia = totSqaValue - curTotVal * curTotVal / (j + 1);
            
            if (j + 1 != (int32_t)vTempCurrentIndex.size()) {
                curDevia -= (totValue - curTotVal) *
                           (totValue - curTotVal) / 
                        (vTempCurrentIndex.size() - j - 1);
            }

            if (curDevia < minDevia) {
                minDevia = curDevia;
                nOptFeatureIndex = vTempFeatureIndex[i];
                fOptFeatureVal = tmpFeatureValue[vTempCurrentIndex[j]];
            }
#ifdef DEBUG
            if (curDevia < minDeviaTemp) {
                minDeviaTemp = curDevia;
                featureVal = tmpFeatureValue[vTempCurrentIndex[j]];
            }
#endif
        }
    }
}

void RegressionTree::splitData(mla::basic::Node<float>* &top,
        const int &nOptFeatureIndex,
        const float &fOptFeatureVal,
        const std::vector<int32_t> &vTmpCurrentIndex,
           std::vector<int32_t> &vLeftIndex,
        std::vector<int32_t> &vRightIndex) {

    float label = 0.0;
    for (int32_t i = 0; i < vTmpCurrentIndex.size(); i ++) {
        label += getTrainingY()[vTmpCurrentIndex[i]];
    }
    label /= (int32_t)vTmpCurrentIndex.size();
    
    top->m_nCurrentOptSplitIndex = nOptFeatureIndex;
    top->m_fCurrentOptSplitValue = fOptFeatureVal;
    top->label = label;

    for (int32_t j = 0; j < vTmpCurrentIndex.size(); j ++) {
        if (getTrainingX()[vTmpCurrentIndex[j]][nOptFeatureIndex] <= fOptFeatureVal) {
            vLeftIndex.push_back(vTmpCurrentIndex[j]);
        } else {
            vRightIndex.push_back(vTmpCurrentIndex[j]);
        }
    }
}

float RegressionTree::predict( const std::vector<float> &testFeatureX) {
    mla::basic::Node<float>* oTreeNode = getTreeRootNode();
    while (true) {
        if (NULL == oTreeNode->m_oLeft && NULL == oTreeNode->m_oRight) {
            return oTreeNode->label;
        }
        
        if (testFeatureX[oTreeNode->m_nCurrentOptSplitIndex] <= oTreeNode->m_fCurrentOptSplitValue) {
            if (NULL == oTreeNode->m_oLeft) {
                return oTreeNode->label;
            } else {
                oTreeNode = oTreeNode->m_oLeft;
            }
        } else {
            if (NULL == oTreeNode->m_oRight) {
                return oTreeNode->label;
            } else {
                oTreeNode = oTreeNode->m_oRight;
            }
        }
    }
}
}
}
