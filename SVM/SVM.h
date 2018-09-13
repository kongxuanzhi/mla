#pragma once

#ifndef  __SVM_H_
#define  __SVM_H_

#include "Util.h"
#include "Model.h"

//svm label分为1和-1
class SVM : public Model<int> {
private:
	vector<int32_t> _supportVecIndexs;       //支持向量的样本索引
	vector<int32_t> _boundVecIndex;          //边界上的样本索引
	vector<int32_t> _notBoundVecIndex;       //不在边界上的索引
	vector<float> _alphas;                   //拉格朗日乘子
	vector<vector<float> > _kernels;         //样本的核函数矩阵（样本两两相乘得到的结果） 
	vector<float> _error;					 //错误率
	float _b;                                // b
	float _c;                                // C
	int32_t _maxIterCnt;                     //最大迭代次数

public:

	SVM() {}
	SVM(float C, int32_t maxIterCnt) : _c(C), _maxIterCnt(maxIterCnt) {}
	~SVM() {}

	// init parameters: 
	// alpha->0.0 
	// error->0.0 
	// bias->0.0
	void init();

	// 0 < alpha_i < c <=> _y[i] * _error[i] == 0
	// 0 == alpha_i    <=> _y[i] * _error[i] > 0
	// c == alpha_i    <=> _y[i] * _error[i] < 0
	//样本i满足kkt条件：
	//1. ai 在0和c之间，yi*error[i]要等于0 意味着样本点在边界上
	//2. ai 等于0时，yi*error[i] 要大于0 意味着样本点在间隔外，对结果不产生影响
	//3. ai 等于C时，yi*error[i] 要小于0 意味着样本点在间隔内，容许一定的错误分类样本
	bool fitKkt(int32_t i);

	//内循环： 首先外层循环找到一个支持向量不满足kkt条件的情况下，需要更新alpha，找一个j样本，更新alphaj和alphai
	//更新完ai和aj之后，更新b和error
	bool innerLoop(int32_t i);

	// 贪心查找能使|error_i - error_j|最大化的j样本作为第二个优化变量
	int32_t selectj(int32_t i);

	void updateSupportVectorIndex();

	void  updateBoundVectorIndex();

	float kernelCalc(const vector<float> &xi, const vector<float> &xj);

	float predict(const vector<float> &feature);

	void train();
};

#endif

