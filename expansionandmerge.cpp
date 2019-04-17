#include "expansionandmerge.h"

EandM::EandM(vector<vector<string> >* vclusters1,vector<string>* rowcolname,MatrixXf* mat_w,MatrixXf* pi){
   vclusters=vclusters1;
   allnodes=rowcolname;
   mat_w_origin=mat_w;
   piit=pi;
}

EandM::~EandM(){

}

void EandM::Expansion(){
    sort(vclusters->begin(),vclusters->end(),greater_equal_secon_two);
    for(int i=0;i<vclusters->size();i++){
    	sort(vclusters->at(i).begin(),vclusters->at(i).end());
    }

    vector<float> densityorigin;
    for(int i=0;i<vclusters->size();i++){
        float a=calculatedensity(vclusters->at(i));
	densityorigin.push_back(a);
    }
    
    vector<vector<string> > valllast;
    for(int i=0;i<vclusters->size();i++){
        vector<string> vone;
	set_difference(allnodes->begin(),allnodes->end(),vclusters->at(i).begin(),vclusters->at(i).end(),back_inserter(vone));
        valllast.push_back(vone);
        vone.clear();
    }
    
    #pragma omp parallel for
    for(int i=0;i<vclusters->size();i++){
        cout<<i<<endl;
        for(int count=0;count<=allnodes->size();count++){
	    vector<string> linknodes=findlinkingnodes(vclusters->at(i),valllast[i]);
            if(linknodes.size()==0){
            	break;
            }else{
               for(vector<string>::iterator it=linknodes.begin();it!=linknodes.end();it++){
                  string add=*it;
                  valllast[i].erase(remove(valllast[i].begin(),valllast[i].end(),add),valllast[i].end());
                  linknodes.erase(it);
                  vclusters->at(i).push_back(add);
                  float densityadd=calculatedensity(vclusters->at(i));
                  if(densityadd-densityorigin[i]>0){
                     densityorigin[i]=densityadd;
                  }else{
                     vclusters->at(i).pop_back();
                  }
                  it=it-1;
               } 
            }
            linknodes.clear();
        }
    }
    SortAndMerge();
}

vector<string> EandM::findlinkingnodes(vector<string> vcluster,vector<string> valllastone){
    vector<string> linknodes;
    map<string,int> mapnandn;
    for(vector<string>::iterator itli=valllastone.begin();itli!=valllastone.end();itli++){
        vector<string>::iterator findl=find(linknodes.begin(),linknodes.end(),*itli);
        if(findl==linknodes.end()){
            vector<string>::iterator iLocation=find(allnodes->begin(),allnodes->end(),*itli);
            if(iLocation!=allnodes->end()){
                int numoflink=0;
                int Location=iLocation-allnodes->begin();
                for(vector<string>::iterator itlj=vcluster.begin();itlj!=vcluster.end();itlj++){
                    vector<string>::iterator jLocation=find(allnodes->begin(),allnodes->end(),*itlj);
                    if(jLocation!=allnodes->end()){
                        int jL=jLocation-allnodes->begin();
                        if(mat_w_origin->row(Location).col(jL)(0,0)>0){
                             numoflink+=1;
                        }
                    }
                 }
                 if(numoflink>=1){
                    linknodes.push_back(*itli);
                    mapnandn[*itli]=numoflink;
                 }
              }
         }
    }
    linknodes.clear();
    vector<PAIR> pairnandn(mapnandn.begin(),mapnandn.end());
    sort(pairnandn.begin(),pairnandn.end(),cmp_by_value);
    vector<string> linknodest;
    for(int i=0;i!=pairnandn.size();++i){
        linknodest.push_back(pairnandn[i].first);
    }
    map<string,int >(mapnandn).swap(mapnandn);
    vector<PAIR >(pairnandn).swap(pairnandn);
    return linknodest;
}

float EandM::calculatedensity(vector<string> vcluster){
    float density;
    float interweights=0.0;
    for(vector<string>::iterator iti=vcluster.begin();iti!=vcluster.end()-1;iti++){
    	vector<string>::iterator iLoc=find(allnodes->begin(),allnodes->end(),*iti);
        if(iLoc!=allnodes->end()){
            int iLocH=iLoc-allnodes->begin();
            for(vector<string>::iterator itj=iti+1;itj!=vcluster.end();itj++){
         	vector<string>::iterator jLoc=find(allnodes->begin(),allnodes->end(),*itj);
                if(jLoc!=allnodes->end()){
                    int jLocH=jLoc-allnodes->begin();
                    if(mat_w_origin->row(iLocH).col(jLocH)(0,0)>0.0){
                         interweights+=piit->row(iLocH).col(jLocH)(0,0);
                    }
                }
            }
        }
    }
    density=interweights;

    float outweights=0.0;
    for(vector<string>::iterator it=vcluster.begin();it!=vcluster.end();it++){
    	vector<string>::iterator iLocation=find(allnodes->begin(),allnodes->end(),*it);
        if(iLocation!=allnodes->end()){
            int Location=iLocation-allnodes->begin();
            for(int iL=0;iL<allnodes->size();iL++){
                if(mat_w_origin->row(Location).col(iL)(0,0)>0.0){
                     outweights+=piit->row(Location).col(iL)(0,0);
                }
            }
        }
    }
    float allweights=outweights-density;
    float temp=(density*1.0)/allweights;
    return temp;
}

void EandM::SortAndMerge(){
    sort(vclusters->begin(),vclusters->end(),greater_equal_secon_two);
    vector<string> vtemp_c;
    vector<string> vtemp_merge;
    for(int i=0;i<vclusters->size();i++){
        for(int j=i+1;j<vclusters->size();j++){
            sort(vclusters->at(i).begin(),vclusters->at(i).end());
            sort(vclusters->at(j).begin(),vclusters->at(j).end());
            set_intersection(vclusters->at(i).begin(),vclusters->at(i).end(),vclusters->at(j).begin(),vclusters->at(j).end(),back_inserter(vtemp_c));
            float rates=(vtemp_c.size()*1.0)/vclusters->at(j).size();
            float rateb=(vtemp_c.size()*1.0)/vclusters->at(i).size();
            if(rates>=0.5){
                set_union(vclusters->at(i).begin(),vclusters->at(i).end(),vclusters->at(j).begin(),vclusters->at(j).end(),back_inserter(vtemp_merge));
                vclusters->at(i)=vtemp_merge;
                vclusters->erase(vclusters->begin()+j);
                j=j-1;
            }
            vtemp_c.clear();
            vtemp_merge.clear();
        }
    }
}
