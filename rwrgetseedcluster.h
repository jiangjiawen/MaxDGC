/*************************************************************************
	> File Name: rwrgetseedcluster.h
	> Author: 
	> Mail: 
	> Created Time: 2016年06月15日 星期三 21时53分05秒
 ************************************************************************/

#ifndef _RWRGETSEEDCLUSTER_H
#define _RWRGETSEEDCLUSTER_H
#include <iostream>
#include "Eigen/Dense"
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using namespace Eigen;

class RwrGetSeedCluster{
    private:
        int rowcollen;
    public:
    RwrGetSeedCluster(int);
    ~RwrGetSeedCluster();
    MatrixXf getMatrix(vector<string> *rowcolname,map<string,float> *mat_map);
    MatrixXf rwrgetpi(MatrixXf*);
    vector<vector<string> > getseed(float,MatrixXf,vector<string>);
    void seeduniq(vector<vector<string> >*);
    void seeduniqo(vector<vector<string> >*);
};
#endif
