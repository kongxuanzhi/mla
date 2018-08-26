#include "../Util.h"
#include "ID3.cpp"

void dfsNode(Node node)
{
    cout << "start" << endl;
    if (node->childs.size() == 0) return;
    cout << "node-index :" << node->index << endl;
    cout << "depth :" << node->depth << endl;
    cout << "node-features:" << endl;
    for (int  i = 0; i < node->featureIndexs.size(); i++) {
        cout << node->featureIndexs[i] << " ";
    }
    cout << endl;
    cout << "node-samplesIndexs:" << endl;
    for (int i = 0; i < node->sampleIndexs.size(); i++) {
        cout << node->sampleIndexs[i] << " ";
    }
    cout << endl;
    cout << "node-H:" << node->H << endl;

    for (int i = 0; i < node->childs.size(); i++) {
        dfsNode(node->childs[i]);
    }
}

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

    ID3 *id3 = new ID3();
    id3->setData(trainSamples, trainLabels);
    Node root = id3->buildTree();
    
    dfsNode(root);

    
    // id3->training();

    return 0;
}