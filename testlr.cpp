#include "LR.h"
#include "Feature.h"

int main(int argv, char* argc[]) {

	mla::lr::LR *lr = new mla::lr::LR();
	//  ./Bin/lr DATA/ionosphere 0.05 500 2 0 2 0.2
	if (argv < 3) {
		std::cout << "Usage: ./LR [training data] [learning_rate] [max iter cnt] [nor type] [opt type] " << std::endl;
		std::cout << "	nor type: 0-min_max  1-square normalization" << std::endl;
		std::cout << "	optimize type: 0-gradient decent 1-stochastic gradient descent" << std::endl;
		std::cout << "	regularization type: 0-l1 1-l2 " << std::endl;
	}

	//读取数据
	std::string line;
	std::ifstream fis(argc[1]);
    std::vector<int32_t> vctLabel;
	std::vector<std::vector<float> > vctFeature;
	while (getline(fis, line)) {
        std::vector<float> tempFeature;
		std::vector<std::string> vctSplitRes;
		mla::util::split(line, '\t', vctSplitRes);
		tempFeature.push_back(1.0);
		for (size_t i = 0; i < vctSplitRes.size(); i ++) {
            if (i + 1 < vctSplitRes.size()) {
                tempFeature.push_back(atof(vctSplitRes[i].c_str()));
            } else {
                vctLabel.push_back(atoi(vctSplitRes[i].c_str()));
            }
        }
        vctFeature.push_back(tempFeature);
    }
   	
	//将样本的80%用于训练模型，20%用于测试模型的准确度
	int sampleCnt = vctFeature.size(); 
	int splitPos = (int)(0.8 * sampleCnt);
    
	std::vector<std::vector<float> > vctTrainFeature(vctFeature.begin(), vctFeature.begin() + splitPos);
	std::vector<std::vector<float> > vctTestFeature(vctFeature.begin() + splitPos, vctFeature.end());
    
	std::vector<int32_t> vctTrainLabel(vctLabel.begin(), vctLabel.begin() + splitPos);
    std::vector<int32_t> vctTestLabel(vctLabel.begin() + splitPos, vctLabel.end());
	float learning_rate = atof(argc[2]); //固定的学习率，一般在0-1之间
	int32_t max_iter_cnt = atoi(argc[3]); //最大迭代次数 
	int32_t nor_type = atoi(argc[4]); //样本归一化项，0：min_max; 1: square 2: no normalization
	int32_t opt_type = atoi(argc[5]); //优化方法： 0：gradient decent; 1: stochastic gradient decent
	int32_t reg_type = -1; //正则化项： 0：L1正则化; 1: L2正则化
	float lambda = 0.0;

	reg_type = atoi(argc[6]);
	lambda = atof(argc[7]);

	//初始化参数
	lr->set_parameter(max_iter_cnt, learning_rate, reg_type, lambda, 0);
	lr->set_data(vctTrainFeature, vctTrainLabel);
	
	//对数据归一化处理
	mla::feature::feature_normalize(nor_type, vctTrainFeature);	
	mla::feature::feature_normalize(nor_type, vctTestFeature);
	
	//训练模型
	lr->train(opt_type);
	
	//测试模型在训练集上的准确度
	int32_t tot = 0, right = 0;
	for (int32_t i = 0; i < vctTrainFeature.size(); i ++) {
		std::cout << vctTrainLabel[i] << " " << lr->predict(vctTrainFeature[i]) << std::endl;
		if (lr->predict(vctTrainFeature[i]) < 0.5 && vctTrainLabel[i] == 0 ||
				lr->predict(vctTrainFeature[i]) > 0.5 && vctTrainLabel[i] == 1) {
			right += 1;
		}
		tot += 1;
	}
	
	std::cout << right * 1.0 / tot << std::endl;
	tot = 0, right = 0;
	//测试模型在测试集上的准确度
	for (int32_t i = 0; i < vctTestFeature.size(); i ++) {
		std::cout << vctTestLabel[i] << " " << lr->predict(vctTestFeature[i]) << std::endl;
		if (lr->predict(vctTestFeature[i]) < 0.5 && vctTestLabel[i] == 0 ||
				lr->predict(vctTestFeature[i]) > 0.5 && vctTestLabel[i] == 1) {
			right += 1;
		}
		tot += 1;
	}
	std::cout << right * 1.0 / tot << std::endl;
	//保存模型,暂未实现
	if (argv == 7) {
		lr->dump_model(argc[6]);
	}
	
	return 0;
}