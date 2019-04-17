#ifndef _EXPANSIONANDMERGE_H
#define _EXPANSIONANDMERGE_H
#include <assert.h>
#include <math.h>
#include "Eigen/Dense"
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

using namespace std;
using namespace Eigen;

class EandM{
private:
    vector<vector<string> >* vclusters;
    vector<string>* allnodes;
    MatrixXf* mat_w_origin;
    MatrixXf* piit;
public:
    EandM(vector<vector<string> >*,vector<string>* ,MatrixXf*, MatrixXf*);
    ~EandM();
    static bool greater_equal_secon_two(const vector<string> &v1,const vector<string> &v2){
        return v1.size()>v2.size();
    }
    typedef pair<string,int> PAIR;
    static bool cmp_by_value(const PAIR& lhs, const PAIR& rhs){
        return lhs.second>rhs.second;
    }
    void Expansion();
    float calculatedensity(vector<string>);
    vector<string> findlinkingnodes(vector<string>, vector<string> );
    void SortAndMerge();
};
#endif
