/*************************************************************************
	> File Name: rwrgetseedcluster.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年06月15日 星期三 21时57分52秒
 ************************************************************************/

#include<iostream>
#include "rwrgetseedcluster.h"

using namespace std;
using namespace Eigen;

RwrGetSeedCluster::RwrGetSeedCluster(int rclen):rowcollen(rclen){

}

RwrGetSeedCluster::~RwrGetSeedCluster(){

}

MatrixXf RwrGetSeedCluster::getMatrix(vector<string> *rowcolname,map<string,float> *mat_map){
    MatrixXf mat_w(rowcollen,rowcollen);
    mat_w=MatrixXf::Zero(rowcollen,rowcollen);
    for(int i=0;i<rowcollen;i++){
        for(int j=0;j<rowcollen;j++){
            string rcname_1='-'+rowcolname->at(i)+'-'+rowcolname->at(j);
            string rcname_2='-'+rowcolname->at(j)+'-'+rowcolname->at(i);
            map<string,float>::iterator itmatfind_1=mat_map->find(rcname_1);
            map<string,float>::iterator itmatfind_2=mat_map->find(rcname_2);
            if(itmatfind_1!=mat_map->end()){
                mat_w(i,j)=itmatfind_1->second;
            }
            if(itmatfind_2!=mat_map->end()){
                mat_w(i,j)=itmatfind_2->second;
            }
        }
    } 
    MatrixXf mat_d(rowcollen,rowcollen);
    mat_d=MatrixXf::Zero(rowcollen,rowcollen);
    for(int i=0;i<rowcollen;i++){
	//float rowsum=mat_w.row(i).sum();
        mat_d(i,i)=mat_w.row(i).sum();
    }
    MatrixXf mat_dd=mat_d.array().pow(0.5);
    MatrixXf mat_ddd=mat_dd.inverse();
    mat_w=mat_ddd*mat_w*mat_ddd;
    /*for(int i=0;i<rowcollen;i++){
        float rowsum=mat_w.row(i).sum();
        for(int j=0;j<rowcollen;j++){
            mat_w(i,j)=mat_w(i,j)/rowsum;
        }
    }*/
    return mat_w;
}

MatrixXf RwrGetSeedCluster::rwrgetpi(MatrixXf* mat_w){
    MatrixXf mat_I=MatrixXf::Identity(rowcollen,rowcollen);
    MatrixXf mat_Start=MatrixXf::Identity(rowcollen,rowcollen);
    float c=0.19;
    MatrixXf pi=(mat_I-mat_w->transpose()*(1-c)).inverse()*mat_Start*c;
    //cout<<pi<<endl;
    return pi;
}

vector<vector<string> > RwrGetSeedCluster::getseed(float gseedthreshold,MatrixXf pi,vector<string> rowcolname){
    vector<vector<string> > vclusters;
    vector<string> vcluster;
    for(int i=0;i<rowcollen;i++){
        float gt=pi.row(i).maxCoeff()*gseedthreshold;
        for(int j=0;j<rowcollen;j++){
            if(pi(j,i)>gt){
                vcluster.push_back(rowcolname[j]);
            }
        }
        vcluster.push_back(rowcolname[i]);
        vclusters.push_back(vcluster);
        vcluster.clear();
    }
  
    /*
    for(int i=0;i<rowcollen;i++){
    	if(vclusters[i].size()==1){
            float maxlocvalue=0.0;
            float maxtwolocvalue=-1.0;
            int maxloc=-1;
            int maxtwoloc=-2;
            for(int j=0;j<rowcollen;j++){
            	if(pi(j,i)>maxlocvalue){
                    maxtwolocvalue=maxlocvalue;
                    maxtwoloc=maxloc;
                    maxlocvalue=pi(i,j);
                    maxloc=j;
                }
                if(pi(j,i)>maxtwolocvalue && pi(j,i)<maxlocvalue){
                  maxtwolocvalue=pi(i,j);
                  maxtwoloc=j;
                }
            }
            vclusters[i].push_back(rowcolname[maxtwoloc]);
        }
    }
    */
    return vclusters;
}

void RwrGetSeedCluster::seeduniq(vector<vector<string> > *vclusters){
    vector<int> stloc;
    vector<string> vtemp;
    for(int i=0;i<vclusters->size();i++){
    	sort(vclusters->at(i).begin(),vclusters->at(i).end());
    }
    stloc.push_back(0);
    for(int i=1;i<vclusters->size();i++){
        int count=0;
        for(int loc=0;loc<stloc.size();loc++){
            set_intersection(vclusters->at(stloc[loc]).begin(),vclusters->at(stloc[loc]).end(),vclusters->at(i).begin(),vclusters->at(i).end(),back_inserter(vtemp));
            float ratiosize=(vtemp.size())*1.0/vclusters->at(stloc[loc]).size();
            if(ratiosize<=0.0){
                count+=1;
            }
            vtemp.clear();
       }
       if(count==stloc.size()){
            stloc.push_back(i);
       }
    }
    vector<int> eraseloc;
    for(int i=0;i<vclusters->size();i++){
        vector<int>::iterator sf=find(stloc.begin(),stloc.end(),i);
        if(sf==stloc.end()){
            eraseloc.push_back(i);
        }
    }

    /*for(int i=0;i<vclusters->size();i++){
	if(vclusters->at(i).size()==1){
	     eraseloc.push_back(i);
	}
    }*/

    sort(eraseloc.begin(),eraseloc.end(),greater<int>());
    for(int i=0;i<eraseloc.size();i++){
        vclusters->erase(vclusters->begin()+eraseloc[i]);
    }
    
    for(int i=0;i<vclusters->size();i++){
	if(vclusters->at(i).size()==1){
	    vclusters->erase(vclusters->begin()+i);
	    i--;
	}
    }

}

void RwrGetSeedCluster::seeduniqo(vector<vector<string> > *vclusters){
    vector<string> vtemp;
    for(int i=0;i<vclusters->size();i++){
   	sort(vclusters->at(i).begin(),vclusters->at(i).end());
    }
    for(int i=0;i<vclusters->size();i++){
        for(int j=i+1;j<vclusters->size();j++){
            set_intersection(vclusters->at(i).begin(),vclusters->at(i).end(),vclusters->at(j).begin(),vclusters->at(j).end(),back_inserter(vtemp));
            float sizevalue=(vtemp.size())/vclusters->at(j).size();
            if(sizevalue>=0.8){
                vclusters->erase(vclusters->begin()+j);
                j=j-1;
            }
            vtemp.clear();
        }
    }

    for(int i=0;i<vclusters->size();i++){
        if(vclusters->at(i).size()==1){
            vclusters->erase(vclusters->begin()+i);
            i--;
        }
    }

}
