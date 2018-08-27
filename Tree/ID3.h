#ifndef __ID3_H__
#define __ID3_H__

#include "Util.h"

#define EQUAL 0
#define GREAT 1
#define LESS 2

typedef struct Condition {
	uint type; //分割类型 > =
    int feature; //第几个特征
} Condition;

typedef struct Node {
    vector<uint> sampleIndexs;
    vector<uint> featureIndexs;
    uint index; //节点序号
    uint depth; //深度
    double H; //熵值
    Condition condition; //节点分类条件
    vector<struct Node*> childs; //字节点
} *PNode;

//连续值离散化  //二分法取中间值

class ID3 {
    private:
        vector<vector<double> > _samples;
        vector<uint> _labels;
        vector<uint> _types;
        uint _featureCnt;
        uint _sampleCnt;
        uint _labelCnt;
		PNode _root;
    public:
        ID3() {}

        //树的剪枝
        //1. 预剪枝： 在构建决策树的过程中，提前停止， 停止条件有节点样本个数，树的深度等
        //2. 后剪枝： 在构造完决策树后，然后开始剪枝
        // ID3(uint maxNodeN, uint maxDepth) {}
        
        void setData(vector<vector<double> > &samples, vector<uint> &labels);

        void setType(int index, int type);

        //信息增益
        double getBestSplitFeatureIndex(PNode node, int &featureIndex);
        double getLabelMapEntrop(map<uint, uint> labelCnts, uint valueCnt);
        double getNodeEntrop(PNode node);
        //关键的一个步骤
		double getGiniVal(double p);

        map<double, map<uint, uint> >  getDiscretFeatureEntrop(vector<uint> sampleIndexs, int featurej, double &entrop);
        double getContinuousFeatureEntrop(vector<uint> sampleIndexs, int featurej, double &entrop);

        PNode buildTree();

        int buildChildNodes(PNode node, int index);

        //1. 分析
        void training();

        uint predict(vector<double> sample);
};
#endif

