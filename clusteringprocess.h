#ifndef _CLUSTERINGPROCESS_H
#define _CLUSTERINGPROCESS_H
#include <assert.h>
#include <math.h>
#include "Eigen/Dense"
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

using namespace std;
using namespace Eigen;

class ClusteringProcess{
private:
    vector<vector<string> >* vclusters;
    vector<string>* allnodes;
    MatrixXf* piit;
public:
    ClusteringProcess(vector<vector<string> >*,vector<string>*, MatrixXf*);
    ~ClusteringProcess();
    void GlobalProcess();
    static bool greater_equal_secon_two(const vector<string> &v1,const vector<string> &v2){
        return v1.size()>v2.size();
    }
    void SortAndMerge();
    vector<string> GetAlllastNodesClusters();
};
#endif
