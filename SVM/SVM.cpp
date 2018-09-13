
#include "SVM.h"

float SVM::predict(const vector<float> &feature) {
	float val = 0.0;
	for (vector<int>::iterator it = this->_supportVecIndexs.begin();
		it != this->_supportVecIndexs.end(); it++) {
		val += this->_alphas[*it] * this->_labels[*it] * kernelCalc(this->_samples[*it], feature);
	}
	return val + this->_b;
}

float SVM::kernelCalc(const vector<float> &xi, const vector<float> &xj) {
	//ret = xi * xj �����ڻ�
	float ret = 0.0;
	for (int k = 0; k < this->_featureCnt; k++) {
		ret += xi[k] * xj[k];
	}

	return ret;
}

void SVM::init() {
	this->_b = 0.0;

	for (int i = 0; i < this->_sampleCnt; i++) {
		this->_alphas.push_back(0); //��ʼ��Ϊ0
		this->_notBoundVecIndex.push_back(i); //��֧���������������� ���е㿴���Ƿ�֧������
		this->_error.push_back(-this->_labels[i]); //��ʼ�����Ϊ��-y
	}

	//��ʼ���˾���sample_size * sample_size, ����ȫΪ0
	for (int i = 0; i < this->_sampleCnt; i++) {
		this->_kernels.push_back(vector<float>(this->_sampleCnt, 0.0));
	}

	//�˾����ǶԳƵ��������󣬼���һ���k(xi, xj)�����ھ�����
	for (int i = 0; i < this->_sampleCnt; i++) {
		for (int j = i; j < this->_sampleCnt; j++) {
			// �˺�����������ӳ�䣬����˺�����һ���������ڻ���
			float kernel_value = kernelCalc(this->_samples[i], this->_samples[j]);
			this->_kernels[i][j] = kernel_value;
			this->_kernels[j][i] = kernel_value;
		}
	}
}

bool SVM::fitKkt(int i) {
	//0 < a < c, �ж�yi(wxi+b-ryi) = 0��֧������
	if (this->_alphas[i] > 0 && this->_alphas[i] < this->_c) {
		return (this->_labels[i] * this->_error[i] - 0.0) < EPS;
	} else if (fabs(this->_alphas[i] - 0) < EPS) { //����ⲿ����������
		return this->_labels[i] * this->_error[i] > 0.0;
	} else if (fabs(this->_alphas[i] - this->_c) < EPS) { //����ڲ�
		return this->_labels[i] * this->_error[i] < 0.0;
	}

	return true;
}

int SVM::selectj(int i) {
	int smaplej;
	float maxErrorMargin = 0.0;
	bool found = false;

	//ȡ�������������
	for (int j = 0; j < this->_sampleCnt; j++) {
		if (i != j && fabs(this->_error[i] - this->_error[j]) > maxErrorMargin) {
			maxErrorMargin = fabs(this->_error[i] - this->_error[j]);
			smaplej = j;
			found = true;
		}
	}
	//������ˣ������ȡһ�������ǵȼ۵�
	if (!found) {
		do {
			srand((unsigned)time(NULL));
			smaplej = rand() % this->_sampleCnt;
		} while (smaplej == i); //��֤��ͬ��i��j
	}

	return smaplej;
}

//SMO�㷨�� ���幫ʽ�Ƶ��� https://blog.csdn.net/luoshixian099/article/details/51227754
bool SVM::innerLoop(int i) {
	int j;
	float L, H, b1, b2;
	float eta, alphaJNew, alphaJNewUnclip, alphaINew;

	j = selectj(i);
	
	//�����ĸ
	//����i���ڻ���  xi*xi + xj*xj - 2 * xi * xj = (xi - xj)^2
	eta = this->_kernels[i][i] + this->_kernels[j][j] - 2 * this->_kernels[i][j];

	//��ĸ����Ϊ0
	if (fabs(eta - 0.0) < EPS) {
		return false;
	}
	//�ֱ��aj��ƫ��
	alphaJNewUnclip = this->_alphas[j] + (this->_labels[j] * (this->_error[i] - this->_error[j]) / eta);
	// �и�߽�
	// 1. 0 <= ai, aj <= c, 
	// 2. ai_old * yi + aj_old * yj = ai_new * yi + aj_new * yj
	if (this->_labels[i] * this->_labels[j] != 1) { //б��Ϊ��	
		L = max((float)0, this->_alphas[j] - this->_alphas[i]);
		H = min(this->_c, this->_c + this->_alphas[j] - this->_alphas[i]);
	} else { //б��Ϊ��
		L = max((float)0, this->_alphas[j] + this->_alphas[i] - this->_c);
		H = min(this->_c, this->_alphas[j] + this->_alphas[i]);
	}

	// ֱ���и�
	if (alphaJNewUnclip > H) alphaJNew = H;
	else if (alphaJNewUnclip >= L && alphaJNewUnclip <= H) alphaJNew = alphaJNewUnclip;
	else alphaJNew = L;

	//����alphaJNew�͵�ʽai_old * yi + aj_old * yj = ai_new * yi + aj_new * yj ���Լ����alphaINew
	alphaINew = this->_alphas[i] + _labels[i] * _labels[j] * (this->_alphas[j] - alphaJNew);

	//  update the bias
	b1 = _b - _error[i] - _labels[i] * (alphaINew - _alphas[i]) * _kernels[i][i] - _labels[j] * _kernels[i][j] * (alphaJNew - _alphas[j]);
	b2 = _b - _error[j] - _labels[i] * (alphaINew - _alphas[i]) * _kernels[i][j] - _labels[j] * _kernels[j][j] * (alphaJNew - _alphas[j]);
	if (alphaINew < this->_c && alphaINew > 0) {
		this->_b = b1;
	} else if (alphaJNew < this->_c && alphaJNew > 0) {
		this->_b = b2;
	} else {
		this->_b = (b1 + b2) / 2;
	}
	// update the error
	float baseErrorI = 0.0, baseErrorJ = 0.0;

	for (int k = 0; k < _sampleCnt; k++) {
		baseErrorI += _labels[k] * _alphas[k] * _kernels[i][k];
		baseErrorJ += _labels[k] * _alphas[k] * _kernels[j][k];
	}

	_error[i] = baseErrorI + _b - _labels[i];
	_error[j] = baseErrorJ + _b - _labels[j];

	_alphas[i] = alphaINew;
	_alphas[j] = alphaJNew;

	return true;
}

void SVM::train() {
	int curIter = 0;
	while (curIter++ < this->_maxIterCnt) {
		int tag = 0;
		cout << "Iter " << curIter << endl;
		for (vector<int>::iterator it = _boundVecIndex.begin(); it != _boundVecIndex.end(); it++) {
			if (!fitKkt(*it)) {
				if (innerLoop(*it)) tag += 1;
			}
		}
		if (0 == tag) {
			// find in non-support vector
			for (vector<int>::iterator it = _notBoundVecIndex.begin();
				it != _notBoundVecIndex.end(); it++) {
				if (!fitKkt(*it)) {
					if (innerLoop(*it)) tag += 1;
				}
			}
		}
		// update non-support vector & support vector
		updateSupportVectorIndex();
		updateBoundVectorIndex();
		cerr << "support vector cnt:" << _supportVecIndexs.size() << endl;
		if (0 == tag) break;
	}
}

void SVM::updateSupportVectorIndex() {
	_supportVecIndexs.clear();
	for (vector<int>::iterator it = _boundVecIndex.begin(); it != _boundVecIndex.end(); it++) {
		if (fitKkt(*it)) {
			_supportVecIndexs.push_back(*it);
		}
	}
}

void SVM::updateBoundVectorIndex() {
	_boundVecIndex.clear();
	_notBoundVecIndex.clear();
	for (int i = 0; i < _sampleCnt; i++) {
		if (fabs(_alphas[i] - 0.0) <= EPS || fabs(_alphas[i] - _c) <= EPS) {
			_notBoundVecIndex.push_back(i);
		} else {
			_boundVecIndex.push_back(i);
		}
	}
}
