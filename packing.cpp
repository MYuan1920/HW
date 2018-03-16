#include "packing.h"
#include <functional>
#include<algorithm>
using namespace std;

bool cmp1(vector<int> a, vector<int> b){
    if(a[0] > b[0]) return true;
    else if (a[0] < b[0]) return false;
    else return a[1] >= b[1];
}
bool cmp2(vector<int> a, vector<int> b){
    if(a[1] > b[1]) return true;
    else if (a[1] < b[1]) return false;
    else return a[0] >= b[0];
}
vector<vector<int>> packing_FFD  (int CPU, int Memory, vector<int> flavor, vector<vector<int>> flavor_param,char *optimization){
	Memory=Memory*1024;
	int sum=0;      //虚拟机总数
	for(int i=0;i<flavor.size();i++){
		sum+=flavor[i];
	}
	int last_CPU=0; int last_Memory=0;
	int last_index=0;
	vector< vector<int> >res; //服务器   res【i】 第i个服务器  sever【i】（0-15） 每种虚拟机数量
	vector<vector<int>> sever;    //sever【i】 第i个服务器   sever【i】 （0:lastcpu  1：lastmemory）
	if(optimization=="CPU") //   CPU 只考虑优化单个维度资源利用率,但保证memory不超
	{
		sort(flavor_param.begin(),flavor_param.end(),cmp1);   //按照cpu大小排序
		while(sum--){
			for(unsigned int i=last_index;i<flavor_param.size();i++){
				if(flavor[flavor_param[i][2]]==0) continue;
				else{
					int mincpu=INT_MAX;
					//int minmem=INT_MAX;
					int mark=-1;
					for(int j=0;j<sever.size();j++){   //寻找最佳服务器  把新的虚拟机放到所能容纳到最满的服务器
						if(flavor_param[i][0]<=sever[j][0]&&flavor_param[i][1]<=sever[j][1]){
							if((sever[j][0]-flavor_param[i][0])<mincpu){
							mark=j;
							}							
					}}
				    if(mark!=-1){
					    sever[mark][0]=sever[mark][0]-flavor_param[i][0];
					    sever[mark][1]=sever[mark][1]-flavor_param[i][1];
					    flavor[flavor_param[i][2]]--;
					    last_index=i;
					    res[mark][flavor_param[i][2]]++;
					    break;
						}
					else{ //新加入服务器
						vector<int> cm(2);
					   cm[0]=CPU-flavor_param[i][0];
					   cm[1]=Memory-flavor_param[i][1];
					   sever.push_back(cm);
					   flavor[flavor_param[i][2]]--;
					   last_index=i;
					   vector<int> tmp(flavor_param.size(),0);
		               tmp[flavor_param[i][2]] ++;
		               res.push_back(tmp);
					   break;
					}
				}
			}		
		}
				
	}	
if(optimization=="MEM") //   MEM  只考虑优化单个维度资源利用率,但保证CPU不超
	{
		sort(flavor_param.begin(),flavor_param.end(),cmp2);   //按照cpu大小排序
		while(sum--){
			for(unsigned int i=last_index;i<flavor_param.size();i++){
				if(flavor[flavor_param[i][2]]==0) continue;
				else{
					//int mincpu=INT_MAX;
					int minmem=INT_MAX;
					int mark=-1;
					for(int j=0;j<sever.size();j++){   //寻找最佳服务器  把新的虚拟机放到所能容纳到最满的服务器
						if(flavor_param[i][0]<=sever[j][0]&&flavor_param[i][1]<=sever[j][1]){
							if((sever[j][1]-flavor_param[i][1])<minmem){
							mark=j;
							}							
					}}
				    if(mark!=-1){
					    sever[mark][0]=sever[mark][0]-flavor_param[i][0];
					    sever[mark][1]=sever[mark][1]-flavor_param[i][1];
					    flavor[flavor_param[i][2]]--;
					    last_index=i;
					    res[mark][flavor_param[i][2]]++;
					    break;
						}
					else{ //新加入服务器
						vector<int> cm(2);
					   cm[0]=CPU-flavor_param[i][0];
					   cm[1]=Memory-flavor_param[i][1];
					   sever.push_back(cm);
					   flavor[flavor_param[i][2]]--;
					  last_index=i;
					   vector<int> tmp(flavor_param.size(),0);
		               tmp[flavor_param[i][2]] ++;
		               res.push_back(tmp);
					   break;
					}
				}
			}		
		}
				
	}	
	return res;
}