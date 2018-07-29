#include "../Util.h"
#include "ID3.h"

int main(int argc, const char** argv)
{
    cout << "Hello world!" << endl;

    vector<vector<float> > samples;
    vector<int32_t> labels;

    readRawDataFromCsv("./test.csv", samples, labels);
#if _DEBUG
    printData(samples, labels);
#endif
    int k = 0.8 * samples.size();
    vector<vector<float> > trainSamples(samples.begin(), samples.begin() + k); 
    vector<vector<float> > testSamples(samples.begin() + k, samples.end()); 
    vector<int32_t> trainLabels(labels.begin(), labels.begin() + k);
    vector<int32_t> testlabels(labels.begin() + k, labels.end());

    ID3* id3 = new ID3();
    id3->setData(trainSamples, trainLabels);

    id3->training();

    return 0;
}