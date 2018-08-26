#ifndef __BASETREE_H_
#define __BASETREE_H_

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
    vector<int32_t> labelCnts;  // [5, 6, 1]
    int32_t index; //节点序号
    int32_t depth; //深度
    int32_t H; //熵值
    Condition condition; //节点分类条件
    vector<struct Node*> childs; //字节点
} Node;

class BaseTree {
    public:
        Node* root;
                
        void setData(vector<vector<float> > &samples, vector<int32_t> &labels);

};

#endif