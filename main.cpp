/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年06月15日 星期三 10时49分27秒
 ************************************************************************/

#include<iostream>
#include "Eigen/Dense"
#include "readnet.h"
#include "rwrgetseedcluster.h"
#include "clusteringprocess.h"
#include "expansionandmerge.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace Eigen;

bool greater_equal_secon_two(const vector<string> &v1,const vector<string> &v2){
    return v1.size()>v2.size();
}

int main(int argc,char *argv[]){
    vector<string> rowcolname;
    map<string,float> mat_map;
    ReadNet readnet(argv[1]);
    readnet.readOriginGraph(&rowcolname,&mat_map);
    vector<string>::iterator it;
    /*for(it=rowcolname.begin();it!=rowcolname.end();it++){
   	cout<<*it<<"*";
    } 
    cout<<endl;*/
    RwrGetSeedCluster rwrgetseed(rowcolname.size());
    MatrixXf mat_w=rwrgetseed.getMatrix(&rowcolname,&mat_map);
    MatrixXf pi=rwrgetseed.rwrgetpi(&mat_w);
    //cout<<pi<<endl;
    for(int i=0;i<rowcolname.size();i++){
	pi(i,i)=0;
    }
    float gseedthreshold=atof(argv[2]);
    vector<vector<string> > vclusters=rwrgetseed.getseed(gseedthreshold,pi,rowcolname);
    sort(vclusters.begin(),vclusters.end(),greater_equal_secon_two);
    rwrgetseed.seeduniq(&vclusters);
    

    ClusteringProcess clupro(&vclusters,&rowcolname,&pi);
    clupro.GlobalProcess();
    EandM em(&vclusters,&rowcolname,&mat_w,&pi);
    em.Expansion();
    for(int i=0;i<vclusters.size();i++){
        for(int j=0;j<vclusters[i].size();j++){
           cout<<vclusters[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}
