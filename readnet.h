/*************************************************************************
	> File Name: readnet.h
	> Author: 
	> Mail: 
	> Created Time: 2016年06月15日 星期三 11时07分04秒
 ************************************************************************/

#ifndef _READNET_H
#define _READNET_H
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;
class ReadNet{
    private:
        char* filein;
    public:
        ReadNet(char*);
        ~ReadNet();
    void readOriginGraph(vector<string> *rowcolname,map<string,float> *mat_map);
};
#endif
