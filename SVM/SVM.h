#pragma once

#ifndef  __SVM_H_
#define  __SVM_H_

#include "Util.h"
#include "Model.h"

//svm label��Ϊ1��-1
class SVM : public Model<int> {
private:
	vector<int32_t> _supportVecIndexs;       //֧����������������
	vector<int32_t> _boundVecIndex;          //�߽��ϵ���������
	vector<int32_t> _notBoundVecIndex;       //���ڱ߽��ϵ�����
	vector<float> _alphas;                   //�������ճ���
	vector<vector<float> > _kernels;         //�����ĺ˺�����������������˵õ��Ľ���� 
	vector<float> _error;					 //������
	float _b;                                // b
	float _c;                                // C
	int32_t _maxIterCnt;                     //����������

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
	//����i����kkt������
	//1. ai ��0��c֮�䣬yi*error[i]Ҫ����0 ��ζ���������ڱ߽���
	//2. ai ����0ʱ��yi*error[i] Ҫ����0 ��ζ���������ڼ���⣬�Խ��������Ӱ��
	//3. ai ����Cʱ��yi*error[i] ҪС��0 ��ζ���������ڼ���ڣ�����һ���Ĵ����������
	bool fitKkt(int32_t i);

	//��ѭ���� �������ѭ���ҵ�һ��֧������������kkt����������£���Ҫ����alpha����һ��j����������alphaj��alphai
	//������ai��aj֮�󣬸���b��error
	bool innerLoop(int32_t i);

	// ̰�Ĳ�����ʹ|error_i - error_j|��󻯵�j������Ϊ�ڶ����Ż�����
	int32_t selectj(int32_t i);

	void updateSupportVectorIndex();

	void  updateBoundVectorIndex();

	float kernelCalc(const vector<float> &xi, const vector<float> &xj);

	float predict(const vector<float> &feature);

	void train();
};

#endif

