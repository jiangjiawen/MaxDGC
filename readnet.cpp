/*************************************************************************
	> File Name: readnet.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年06月15日 星期三 10时50分23秒
 ************************************************************************/

#include "readnet.h"
using namespace std;

ReadNet::ReadNet(char* fileinget):filein(fileinget){

}

ReadNet::~ReadNet(){

}

void ReadNet::readOriginGraph(vector<string> *rowcolname,map<string,float> *mat_map){
    ifstream in(filein);
    string s;
    const char *delim=" ";
    while(getline(in,s)){
        char* p=strtok(const_cast<char*>(s.c_str()),delim);
        int i=0;
        string _mergeName;
        float weight;
        while(p){
            if(i==0 || i==1){
                string s;
                s=p;
                _mergeName=_mergeName+'-'+s;
                vector<string>::iterator findit=find(rowcolname->begin(),rowcolname->end(),s);
                if(findit==rowcolname->end()){
                    rowcolname->push_back(s);
                }
            }
            if(i==2){
                weight=atof(p);
            }
            //weight=atof(p);
            p=strtok(NULL,delim);
            i=i+1;
        }
        mat_map->insert(map<string,float>::value_type(_mergeName,weight));
    }
    sort(rowcolname->begin(),rowcolname->end());
}
