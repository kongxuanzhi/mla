
#ifndef __LR_H__
#define __LR_H__

#include "../Util.h"

class LR {
    public:
        int _maxIterCnt;
        float _learnRate;
        vector<float> _w;

        LR() {}

        float getlogLoss();
};

#endif