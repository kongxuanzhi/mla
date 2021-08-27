#ifndef __FEATURE_H_
#define __FEATURE_H_

#include "Util.h"

namespace mla {
	namespace feature {
		//归一化
		void feature_normalize(int32_t normalize_type,
			std::vector<std::vector<float> > &feature);

		void feature_select();

		//kfold检验
		void k_fold_validation(int k);

		//属性离散化
		void feature_discretization(const std::string& dis_primitive_str,
			std::vector<std::vector<float> > &feature);
	}
}
#endif