#include "clusteringprocess.h"

ClusteringProcess::ClusteringProcess(vector<vector<string> >* vclusters1,vector<string>* rowcolname,MatrixXf* pi){
    vclusters=vclusters1;
    allnodes=rowcolname;
    piit=pi;
}

ClusteringProcess::~ClusteringProcess(){

}

void ClusteringProcess::GlobalProcess(){
    vector<string> alastnodesclu=GetAlllastNodesClusters();

    //cout<<"lastnodes"<<endl;
    //for(vector<string>::iterator it=alastnodesclu.begin();it!=alastnodesclu.end();it++){
    //    cout<<*it<<" ";
    //}
    //cout<<endl;

    //cout<<"seedclusters:"<<endl;
    //for(int i=0;i<vclusters->size();i++){
    //    for(vector<string>::iterator it=vclusters->at(i).begin();it!=vclusters->at(i).end();it++){
    //        cout<<*it<<" ";
    //    }
    //    cout<<endl;
    //}
    //cout<<endl;

    map<string,int> nameandloc;
    int countfornameandloc=0;
    for(vector<string>::iterator itname=allnodes->begin();itname!=allnodes->end();itname++){
        nameandloc.insert(map<string,int>::value_type(*itname,countfornameandloc));
        countfornameandloc+=1;
    }
    

    for(vector<string>::iterator it=alastnodesclu.begin();it!=alastnodesclu.end();it++){
        int loci=nameandloc[*it];
        vector<float> disttmp;
        map<int,float> locanddis;
        for(int i=0;i<vclusters->size();i++){
            float tempdis=0.0;
            for(vector<string>::iterator itnodein=vclusters->at(i).begin();itnodein!=vclusters->at(i).end();itnodein++){
                int locj=nameandloc[*itnodein];
                MatrixXf pitemp=piit->block<1,1>(loci,locj);
                tempdis+=pitemp(0,0);
            }
            tempdis=tempdis*1.0/vclusters->at(i).size();
            disttmp.push_back(tempdis);
            locanddis[i]=tempdis;
        }

        vector<float>::iterator biggest=max_element(disttmp.begin(),disttmp.end());
        int locbiggest=distance(disttmp.begin(),biggest);
	//cout<<*it<<" "<<locbiggest<<endl;
        vclusters->at(locbiggest).push_back(*it);
	disttmp.clear();
        for(int i=0;i<vclusters->size();i++){
           if(i!=locbiggest){
	       float discomp=0;
	       discomp=locanddis[locbiggest]-locanddis[i];
	       float ratioofdis=discomp*1.0/locanddis[locbiggest];
	       if(ratioofdis<=0.0 && ratioofdis>=0){
                   vclusters->at(i).push_back(*it);
	       }
           }
	}
    }
    
    //SortAndMerge();

    //cout<<"mergedclusters:"<<endl;
    //for(int j=0;j<vclusters->size();j++){
    //    for(vector<string>::iterator it=vclusters->at(j).begin();it!=vclusters->at(j).end();it++){
    //        cout<<*it<<" ";
    //    }
    //    cout<<endl;
    //}
}

void ClusteringProcess::SortAndMerge(){
    sort(vclusters->begin(),vclusters->end(),greater_equal_secon_two);
    vector<string> vtemp_c;
    vector<string> vtemp_merge;
    for(int i=0;i<vclusters->size();i++){
        for(int j=i+1;j<vclusters->size();j++)
        {
            sort(vclusters->at(i).begin(),vclusters->at(i).end());
            sort(vclusters->at(j).begin(),vclusters->at(j).end());
            set_intersection(vclusters->at(i).begin(),vclusters->at(i).end(),vclusters->at(j).begin(),vclusters->at(j).end(),back_inserter(vtemp_c));
            float valuesize_c=(vtemp_c.size()*1.0)/vclusters->at(j).size();
	        float valuesize_d=(vtemp_c.size()*1.0)/vclusters->at(i).size();
            if(valuesize_c>=0.9 && valuesize_d>=0.3){
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

vector<string>  ClusteringProcess::GetAlllastNodesClusters(){
    vector<string>  alllastnodescluster;
    vector<string>  vallnodes;
    for(int i=0;i<vclusters->size();i++){
        for(int j=0;j<vclusters->at(i).size();j++){
            vector<string>::iterator findname;
            findname=find(vallnodes.begin(),vallnodes.end(),vclusters->at(i)[j]);
            if(findname==vallnodes.end()){
                vallnodes.push_back(vclusters->at(i)[j]);
            }
        }
    }
    sort(vallnodes.begin(),vallnodes.end());
    set_difference(allnodes->begin(),allnodes->end(),vallnodes.begin(),vallnodes.end(),back_inserter(alllastnodescluster));
    vallnodes.clear();
    return alllastnodescluster;
}
