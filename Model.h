#ifndef __MODEL_H_
#define __MODEL_H_

#include "Util.h"

namespace mla {
	namespace model {
		template <class T> //这里只对标记类型做抽象
		class Model {
		public:
			Model() {}
			virtual ~Model() {}

			std::string _name; //模型名称
			int32_t _sample_size; //样本个数
			int32_t _feature_dim; //属性个数

			std::vector<std::vector<float> > _feature; //存放样本
			std::vector<T> _label; //类标签

			void set_data(std::vector<std::vector<float> > &feature,
				std::vector<T> &label);
			virtual void train(int32_t opt_type) {}
			virtual float predict(const std::vector<float> &feautre) = 0;
			// load model from file model_file_name
			virtual void load_model(const char* model_file_name) {}
			// dump model into file model_file_name
			virtual void dump_model(const char* model_file_name) {}
		};

		template <class T>
		void Model<T>::set_data(std::vector<std::vector<float> > &feature,
			std::vector<T> &label) {
			_feature = feature;
			_label = label;
			_sample_size = (int32_t)feature.size();
			if (0 == _sample_size) {
				std::cerr << "Error When Setting the Training Sample!" << std::endl;
				exit(0);
			}
			_feature_dim = (int32_t)feature[0].size();
		}
	}
}
#endif