#ifndef  __SVM_H_
#define  __SVM_H_

#include "Util.h"
#include "Model.h"

namespace mla {
	namespace svm {
		class SVM : public mla::model::Model<float> {
		private:
			std::vector<int32_t> _support_vec_index;    // 支持向量点
			std::vector<int32_t> _bound_vec_index;      // 最大间隔边界上的点
			std::vector<int32_t> _non_bound_vec_index;  // 边界外的点
			std::vector<float> _alpha;                  // 拉格朗日乘子
			std::vector<std::vector<float> > _kernel;   // 核矩阵
			std::vector<float> _error;                  // 每个样本的分类错误
			float _b;                                   // 偏差bias 
			float _c;                                   // 中和结构风险和经验风险的值
			int32_t _max_iter_cnt;                      // 最大迭代次数
		public:
			SVM() {}
			SVM(float C, int32_t max_iter_cnt) :_c(C), _max_iter_cnt(max_iter_cnt) {}
			~SVM() {}
			
			// 初始化参数 拉格朗日乘子alpha->0.0 分类错误值error->0.0 偏差bias->0.0
			void init_alpha();

			// 测试第i个样本是否满足kkt条件:    
			// 0 < alpha_i < c <=> _y[i] * _error[i] == 0  -> 支持向量点
			// 0 == alpha_i    <=> _y[i] * _error[i] > 0   -> 在最大间隔之外
			// c == alpha_i    <=> _y[i] * _error[i] < 0   -> 分类错误
			bool fit_kkt(int32_t i);

			// inner loop in standard svm source code
			// so as to find the alpah_j, and update alpha_i, alpha_j, bias, error
			// firstly ,we search the support vectors, if someone in them does'not match
			// the kkt condition, then we choose it as the alpha_i that stays to be optimized
			bool inner_loop(int32_t i);

			// slect alpha_j given alpha_i
			// normally, we search the alpha which can maximum |error_i - error_j| 贪心分类误差最大的两个样本点
			int32_t select_j(int32_t i);

			// 更新支持向量
			void update_support_vector_index();

			// update vector index as a result of updating alpha_i & alpha_j
			void  update_bound_vector_index();

			// calculate the kernel output gona to be used in prediction stage
			float kernel_cal(const std::vector<float> &x,
				const std::vector<float> &feature);

			// given the vector values of feature in test sample, output the outcome
			// >= 1: positive <= -1:negative otherwise: unknown
			float predict(const std::vector<float> &feature);

			// train the model
			void train(int32_t opt_type);

			// load the model
			void load_model(const char* model_file_name);

			// dump the model
			void dump_model(const char* model_file_name);
		};

	}
}
#endif
