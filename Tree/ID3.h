
#ifndef __ID3_H__
#define __ID3_H__

#include "../Util.h"

#define EQUAL 0
#define GREAT 1
#define LESS 2

typedef struct Condition {
    int32_t type; //分割类型 > =
    int32_t feature; //第几个特征
    float value; //分割点
} Condition;

typedef struct Node {
    vector<int32_t> sampleIndexs;
    vector<int32_t> featureIndexs;
    vector<int32_t> labelCnts;  // [5, 6, 1]
    int32_t index; //节点序号
    int32_t depth; //深度
    int32_t H; //熵值
    Condition condition; //节点分类条件
    vector<struct Node*> childs; //字节点
} *Node;

//连续值离散化  //二分法取中间值

class ID3 {
    private:
        vector<vector<float> > _samples;
        vector<int32_t> _labels;
        vector<int32_t> _types;
        int32_t _featureCnt;
        int32_t _sampleCnt;
        int32_t _labelCnt;
        Node _root;
    public:
        ID3() {}

        //树的剪枝
        //1. 预剪枝： 在构建决策树的过程中，提前停止， 停止条件有节点样本个数，树的深度等
        //2. 后剪枝： 在构造完决策树后，然后开始剪枝
        ID3(int32_t maxNodeN, int32_t maxDepth) {}
        
        void setData(vector<vector<float> > &samples, vector<int32_t> &labels);

        void setType(int index, int type);

        //信息增益
        float setSplitFeatureAndValue(Node node, int &featureIndex, float &bestSplitVal);
        float getOneNodeEntrop(map<int32_t, int32_t> labelCnts, int32_t valueCnt);
        //关键的一个步骤
        float getGiniVal(float p) {
            return -1.0 *  p * log(p);
        }

        float getDiscretFeatureEntrop(vector<int32_t> sampleIndexs, int featurej);
        float getContinuousFeatureEntrop(vector<int32_t> sampleIndexs, int featurej, float &splitval);

        void buildTree();

        void splitData();

        //1. 分析
        void training();

        void predict(vector<float> sample);
};
#endif

