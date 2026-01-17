#include<iostream>
#include<fstream>
#include<typeinfo>
#include<stdlib.h>
#include <vector>
#include <math.h>

using namespace std;
vector<double> spa1(int iter,vector<double> rx_demod,vector< vector <int> > sparse_vnd,vector< vector<int> > sparse_cnd,int edge_num,vector<int> cnd,vector<int> vnd){
	
	vector<double>  s_LLR(edge_num);
    vector<double>  p_LLR(edge_num);
    vector<double>  emp(edge_num);
    vector<double>  s_LLR_forRBP(edge_num);
    double sign=0.0;
    double D=0.0;
	
	//*initialize LLR
    s_LLR=emp;
    //*iter start from here
	for(int z=1;z<=iter;z++){
        //*sum_rule
        for(int i=1;i<edge_num;i++)
        {
            s_LLR[i]+=rx_demod[vnd[i]];
            
            for(int j=1;j<sparse_vnd[vnd[i]].size();j++)
            {
                
                if(i!=sparse_vnd[vnd[i]][j])
                {
                    s_LLR[i]+=p_LLR[sparse_vnd[vnd[i]][j]];
                    //cout<<i<<" "<<j<<endl;
                }
            }
            
            if(s_LLR[i]<0.00001 && s_LLR[i]>-0.00001){
                //s_LLR[i]=0.00001;
            }
            //cout<<s_LLR[i]<<endl;
        }
        
        //*initial p_LLR
        for(int i=1;i<edge_num;i++)
        {
            p_LLR[i]=0;
            //p_LLR[i]=1.0;
            
        }
        //*product rule
        for(int i=1;i<edge_num;i++)
        {
            //cout<<v[i]<<" "<<c[i]<<endl;
            sign=1;
            for(int j=1;j<sparse_cnd[cnd[i]].size();j++)
            {
                if(i!=sparse_cnd[cnd[i]][j])
                {
                    D=exp(fabs(s_LLR[sparse_cnd[cnd[i]][j]]));
                    if(D>1e10){
                    	D=1e10;
                    	//cndout<<log(D+1)-log(D-1)<<endl;
					}
                    D=log(D+1)-log(D-1);
                    if(isinf(D) || isnan(D)){
                        D=0.0;
                     //   cout<<"D=0.0"<<endl;
                        //cout<<(fabs(s_LLR[sparse_cnd[c[i]][j]])+1)/(fabs(s_LLR[sparse_cnd[c[i]][j]])-1)<<endl;
                    }
                    //cout<<D<<endl;
                    p_LLR[i]+=D;
                    sign*=s_LLR[sparse_cnd[cnd[i]][j]]/fabs(s_LLR[sparse_cnd[cnd[i]][j]]);
                    
					//*debug nan & inf
					if(i==946){
                        //cout<<s_LLR[sparse_cnd[c[i]][j]]/fabs(s_LLR[sparse_cnd[c[i]][j]])<<endl;
                    }
                    //cout<<sign<<endl;
                    //p_LLR[i]*=tanh(s_LLR[sparse_cnd[c[i]][j]]/2.0);
                    
					//*debug nan & inf
					if(i==922){
                        //cout<<isfinite(s_LLR[sparse_cnd[c[i]][j]])<<" "<<z<<endl;
                        //cout<<tanh(s_LLR[sparse_cnd[c[i]][j]]/2.0)<<endl;
                        //cout<<exp(fabs(s_LLR[sparse_cnd[c[i]][j]]))<<" "<<D<<endl;
                    }
                    
                }
                
            }
            if(i==946){
                //cout<<sign<<endl;
            }
            //p_LLR[i]=2.0*atanh(p_LLR[i]);
            //cout<<p_LLR[i]<<endl;
            p_LLR[i]=log(exp(p_LLR[i])+1)-log(exp(p_LLR[i])-1);
            if(isinf(p_LLR[i])){
            //	cout<<i<<endl;
            }
            //cout<<p_LLR[i]<<endl;
            p_LLR[i]*=sign;
            //cout<<p_LLR[i]<<endl;
            //cout<<sign<<endl;
            
        }
        //*initialize LLR and save the LLR of this time
        s_LLR_forRBP=s_LLR;
        s_LLR=emp;
        //*iter end to here
    }
    //*add rx_demod
    vector<double> result_BP(309);
    for(int i=1;i<309;i++){
        result_BP[i]+=rx_demod[i];
    }
    //*add LLR to BP
    for(int j=1;j<edge_num;j++){
        result_BP[vnd[j]]+=p_LLR[j];
    }
    
    
    
    
    
    
	return p_LLR;
	//return result_BP;
}
