#ifndef __REG_TREE_H__
#define __REG_TREE_H__

#include "BaseTree.h"

namespace mla {
namespace tree {

class RegressionTree : public mla::basic::Tree<float> {

public:
    RegressionTree() {} 
    
    RegressionTree(int32_t maxNodeCnt, 
            int32_t maxDepth, 
            bool isMultiThreadOn,
            bool ensemble) : mla::basic::Tree<float>(maxNodeCnt, maxDepth, isMultiThreadOn, 0, ensemble) {
    }
    
    void optSplitPos(int &nOptFeatureIndex,
                float &nOptFeatureVal,
                std::vector<int32_t> &vCurrentIndex,
                std::vector<int32_t> &vFeatureIndex); 
    
    void optSplitPosMultiThread(int &nOptFeatureIndex,
            float &nOptFeatureVal,
            std::vector<int32_t> &vCurrentIndex,
            std::vector<int32_t> &vFeatureIndex);

    void splitData(struct mla::basic::Node<float>* &node,
        const int &nOptFeatureIndex,
        const float &fOptFeatureVal,
        const std::vector<int32_t> &vTempCurrentIndex,
        std::vector<int32_t> &vLeftIndex,
        std::vector<int32_t> &vRightIndex);

    friend void* selectFeatureFunc(void* param);

    float predict(const std::vector<float> &testFeatureX);

};
}
}
#endif
