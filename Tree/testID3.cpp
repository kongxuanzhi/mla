#pragma once

#include "ID3.h"

void dfsNode(PNode node)
{
    cout << "start" << endl;
    if (node->childs.size() == 0) return;
    cout << "node-index :" << node->index << endl;
    cout << "depth :" << node->depth << endl;
    cout << "node-features:" << endl;
    for (uint  i = 0; i < node->featureIndexs.size(); i++) {
        cout << node->featureIndexs[i] << " ";
    }
    cout << endl;
    cout << "node-samplesIndexs:" << endl;
    for (uint i = 0; i < node->sampleIndexs.size(); i++) {
        cout << node->sampleIndexs[i] << " ";
    }
    cout << endl;
    cout << "node-H:" << node->H << endl;

    for (uint i = 0; i < node->childs.size(); i++) {
        dfsNode(node->childs[i]);
    }
}

int main(int argc, const char** argv)
{
    cout << "Hello world!" << endl;

    vector<vector<double> > samples;
    vector<uint> labels;

    readRawDataFromCsv("./test.csv", samples, labels);
#if _DEBUG
    printData(samples, labels);
#endif
	//int k = (int)(0.8 * samples.size());
	int k = (int)(samples.size());
    vector<vector<double> > trainSamples(samples.begin(), samples.begin() + k); 
    //vector<vector<double> > testSamples(samples.begin() + k, samples.end()); 
    vector<uint> trainLabels(labels.begin(), labels.begin() + k);
    //vector<uint> testlabels(labels.begin() + k, labels.end());

    ID3 *id3 = new ID3();
    id3->setData(trainSamples, trainLabels);
	id3->setType(6, GREAT);
	id3->setType(7, GREAT);
	PNode root = id3->buildTree();
    
    dfsNode(root);
    
    // id3->training();

    return 0;
}