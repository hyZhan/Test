/*
 * route.cpp
 *
 *  Created on: 2016-3-17
 *      Author: zhy
 */

#include "route.h"
#include"string.h"
#include"memory.h"
#include "lib_record.h"
#include <algorithm>
#include <stdio.h>
#define INF 32768
#define MaxVertex 600
#define MaxDemand 50
//你要完成的功能总入口
void Mutate(int mutate_num,int demand_num,int best_gene[][ MaxDemand+2])
{  int i,temp=0;
 for(i=0;i<demand_num/2;i++)
 {
	 temp=best_gene[demand_num+i][mutate_num];best_gene[demand_num+i][mutate_num]=best_gene[demand_num+i][demand_num-1-mutate_num];best_gene[demand_num+i][demand_num-1-mutate_num]=temp;
 }
}
void print_path(int s,int e ,int path[][MaxVertex])
{	int s_temp=0,e_temp=0;
	e_temp=path[s][e];
	if(e_temp==e)printf("|");
	while(e_temp!=e)
	{
		printf("|%d|",e_temp);
		s_temp=e_temp;e_temp=path[s_temp][e];
	}
}
void print_2array(int row, int col, int array[][600])
{
	int i, j;
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++)
		{
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
//	i=array[0][0];array[0][0]=array[0][1];array[0][1]=i;
}
void print_1array(int col, int array[])
{
	int i;
	for (i = 0; i<col; i++)
	{
			printf("%d  ", array[i]);
	}
	printf("\n");
}
void swap(int low, int high, int array[])
{
	int temp=0;
	temp=array[low];array[low]=array[high];array[high]=temp;
}
void revert(int low , int high , int array[] )
{
	int i=low ,  j = high ,temp;
	while(i != j)
	{
		temp=array[low];array[low]=array[high];array[high]=temp;
		i++ ; j--;
	}
}
int Partition (int a[],int perm[] ,int low, int high)
{
	int i = low;
	int j = high;
	int temp=0;
		while (i < j)
		{
			while ((a[i] <=a[j]) && (i < j))
			{
				j--;
			}
			if(i<j)
			{
				temp=a[i];a[ i ] = a[ j ] ; a[ i ] = temp;
				temp=perm[ i ] ;perm[ i ] = perm[ j ] ; perm[ j ] = temp;
				i++ ;
			}
			while ((a[i] <= a[j]) && (i < j))
			{
				i++;
			}
			if(i<j)
			{
				temp=a[i];a[ i ] = a[ j ] ; a[ i ] = temp;
				temp=perm[ i ] ;perm[ i ] = perm[ j ] ; perm[ j ] = temp;
				j-- ;
			}
		}
		return i ;
}
void QuickSort(int r[],int perm[], int first, int end)
{
   if (first<end)
  {                                   //递归结束
       int pivot=Partition(r, perm , first, end);  //一次划分
       QuickSort(r, perm ,first, pivot-1);//递归地对左侧子序列进行快速排序
       QuickSort(r, perm ,pivot+1, end);  //递归地对右侧子序列进行快速排序
 }
}

void Off_Spring(int demand_num, int father[][MaxDemand+2])
{
	int temp,m=1,i,j,generation_num=(demand_num-2)*2;;
		for(i=2;i<demand_num-1;i++)      //产生父代1的子代
		{
			for(j=0;j<demand_num;j++)
			{
				father[i][j]=father[0][j];
			}
			temp=father[i][m];father[i][m]=father[i][m+1];father[i][m+1]=temp;
			m++;
		}
		m=1;
		for(;i<generation_num-1;i++)      //产生父代1的子代
				{
					for(j=0;j<demand_num;j++)
					{
						father[i][j]=father[1][j];
					}
					temp=father[i][m];father[i][m]=father[i][m+1];father[i][m+1]=temp;
					m++;
				}
}
void search_route(char *topo[5000], int edge_num, char *demand)
{
	 //暫存变量i,j,k,m,temp,顶点数vertex_num，总需求列表长度demand_num，种群大小best_gene_num,迭代数iteration,优化值opt,优化阈值opt_limit
	int i,j,k, m ,temp,vertex_num=0,demand_num=0,best_gene_num,iteration,opt,opt_limit,opt_min1,opt_min2 , position1 , position2 , position3;
	//拓扑矩阵topo_array，权值矩阵cost，路径矩阵path，边ID矩阵edge_array，总需求列表demand_col，父代及其子代构成的种群best_gene
	int topo_array[5000][4]={0},cost[MaxVertex][MaxVertex],path[MaxVertex][MaxVertex]={0},edge_array[MaxVertex][MaxVertex]={0},demand_col[MaxDemand+2]={0},best_gene[MaxDemand*6][MaxDemand+2],best_gene_cost[MaxDemand*6]={0};
	int temparray [ MaxDemand + 2 ] , randperm [ MaxDemand * 6 ] ;
	bool flag;  //标志位
	for(i=0;i<MaxVertex;i++)			//cost矩阵初始化
{
		std::fill_n(cost[i], MaxVertex, INF);
		cost[i][i]=0;
}
	for(i=0;i<edge_num;i++)			//读取topo.csv表至topo_array矩阵
	{
		j=0;k=0;
		flag=true;
		while((*(topo+i))[j+1]!='\r'&&(*(topo+i))[j+1]!='\n'&&(*(topo+i))[j+1]!='\0')
		{
			if(flag)
			{
				temp=int((*(topo+i))[j] - '0');
				j++;
				flag=false;
			}
			else
			{
				temp=temp*10+int((*(topo+i))[j] - '0');
				j++;
			}
			if((*(topo+i))[j]==',')
			{
				topo_array[i][k]=temp;
				flag=true;
				j++;k++;
			}
			if((*(topo+i))[j+1]=='\r'||(*(topo+i))[j+1]=='\n'||(*(topo+i))[j+1]=='\0')
			{
				topo_array[i][k]=temp;
				break;
			}
		}
		edge_array[topo_array[i][1]][topo_array[i][2]]=i;             //路径与边ID的对应表
		if(topo_array[i][1]>vertex_num)vertex_num=topo_array[i][1];     //求顶点数
		if(topo_array[i][2]>vertex_num)vertex_num=topo_array[i][2];		//求顶点数
		cost[topo_array[i][1]][topo_array[i][2]]=topo_array[i][3];				//初次权值矩阵初始化
		path[topo_array[i][1]][topo_array[i][2]]=topo_array[i][2];				//初次路径矩阵初始化
	}
	vertex_num+=1;			//求顶点数
	for(k=0;k<vertex_num;k++)      	//Floyd算法
	{
		for(i=0;i<vertex_num;i++)
		{
			for(j=0;j<vertex_num;j++)
			{
				if(cost[i][j]>(cost[i][k]+cost[k][j]))
				{
					cost[i][j]=cost[i][k]+cost[k][j];
					path[i][j]=k;
				}
			}
		}
	}
j=0;k=0;
flag=true;
while(demand[j]!='\r'&&demand[j]!='\n'&&demand[j]!='\0')   //读取demand.csv表至demand_col
{
	if(flag)
	{
			temp=int(demand[j] - '0');
			j++;
			flag=false;
	}
	else
	{
			temp=temp*10+int(demand[j] - '0');
			j++;
	}
	if(demand[j]==','||demand[j]=='|')
	{
			demand_col[k]=temp;
			flag=true;
			j++;k++;demand_num++;
	}
	if(demand[j]=='\r'||demand[j]=='\n'||demand[j]=='\0')
	{
			demand_col[k]=temp;demand_num++;
			break;
	}
}
if(demand_num>4)      //需求总长度大于4，即有3个或以上的经由点的初始种群产生
	{
	best_gene_num=(demand_num-2)*2;
	for(i=1;i<demand_num-1;i++)		//产生父代2
			{
				best_gene[0][i]=demand_col[demand_num-i];
			}
	best_gene[0][0]=demand_col[0];best_gene[0][demand_num-1]=demand_col[1];
	for(i=1;i<demand_num-1;i++)		//产生父代2
		{
			best_gene[1][i]=demand_col[demand_num-i];
		}
	best_gene[1][0]=demand_col[0];best_gene[1][demand_num-1]=demand_col[1];
	}
else
{
	best_gene_num=demand_num-2;
	for(i=1;i<demand_num-1;i++)		//产生父代1
		{
			best_gene[0][i]=demand_col[i+1];
		}
		best_gene[0][0]=demand_col[0];best_gene[0][demand_num-1]=demand_col[1];
 if(best_gene_num==2)      //若只有一个经由点则只有一个父代，不需要子代
 {
	 for(i=1;i<demand_num-1;i++)		//产生父代2
	 			{
	 				best_gene[1][i]=demand_col[demand_num-i];
	 			}
	 best_gene[1][0]=demand_col[0];best_gene[1][demand_num-1]=demand_col[1];
 }
}

for( i = 0 ; i < best_gene_num ; i++ )
{
	for( k =0 ; k <  demand_num-2; k++)  temparray [k] = k+2 ;
	for (j = 1; j < demand_num-1; j++){
			temp = rand() % ( demand_num - j - 1 );
			best_gene[ i ][ j ]= demand_col[ temparray [ temp ] ];
			temparray[ temp ] = temparray[ demand_num - 2 - j ] ;
		}
	best_gene[ i ] [ 0 ] = demand_col[ 0 ] ; best_gene[ i ] [ demand_num - 1 ] = demand_col [ 1 ] ;
	 for( j =0 ; j < demand_num-3 ; j ++ )
	 	{
		 for( k = j+2 ; k < demand_num - 1 ; k ++ )
		 	 	{
			 	 	 if(cost [ best_gene [ i ][ j ]] [  best_gene [ i ][ k ] ]+ cost [ best_gene [ i ][ j + 1 ]] [  best_gene [ i ][ k + 1 ] ] <  cost [ best_gene [ i ][ j ]] [  best_gene [ i ][ j + 1 ] ]+ cost [ best_gene [ i ][ k ]] [  best_gene [ i ][ k + 1 ] ] )
			 	 	 {
			 	 		 revert( k , j + 1 , best_gene [ i ]);
			 	 	 }
		 	 	}
	 	}
}
/*
flag=true;
while(flag)
{
	i=1;
	for(j=0;j<best_gene_num;j++)
		{
			for(k=0;k<demand_num-1;k++)
			{
				best_gene_cost[j]+=cost[best_gene[j][k]][best_gene[j][k+1]];
			}
		}
	for(j=0;j<best_gene_num;j++)
	{
		if(best_gene_cost[j]<INF)
		{
			flag=false;break;
		}
	}
	if(flag==true)
	{
		Mutate(i,demand_num,best_gene);
		Off_Spring(demand_num,best_gene);
	}
}
*/
iteration=50*(demand_num-2)/10;
opt_limit=(demand_num-2)*((demand_num-2)/10);
//opt_limit=5;
//opt=best_gene_cost[0];
for(i=0;i<iteration;i++)
{
	std::fill_n(best_gene_cost, MaxDemand*2, 0);
	for (m = 0; m < demand_num; m++)
	{
		for( k =0 ; k <  demand_num-2; k++)  temparray [k] = k+2 ;
			for (j = 0; j < 3; j++)
			{
						temp = rand() % ( demand_num - 2 );
						randperm [ j ]=  temparray [ temp ] ;
						temparray[ temp ] = temparray[ demand_num - 1 - j ] ;
					}
		 if(randperm [ 0 ]< randperm [ 1 ]){
			 if(randperm [ 0 ]< randperm [ 2 ]){
				 position1 = randperm [ 0 ];
				 if(randperm [ 1 ]< randperm [ 2 ]){
					 position2 = randperm [ 1 ];  position3 = randperm [ 2 ];
				 }
				 else{
					 position2 = randperm [ 2 ];  position3 = randperm [ 1 ];
				 }
			 }
			 else{
				 position1 = randperm [ 2 ]; position2 = randperm [ 0 ];  position3 = randperm [ 1 ];
			 }
		 }
		 else{
			 if(randperm [ 1 ]< randperm [ 2 ]){
			 				 position1 = randperm [ 1 ];
			 				 if(randperm [ 0 ]< randperm [ 2 ]){
			 									 position2 = randperm [ 0 ];  position3 = randperm [ 2 ];
			 								 }
			 								 else{
			 									 position2 = randperm [ 2 ];  position3 = randperm [ 0 ];
			 								 }
			 			 }
			 			 else{
			 				 position1 = randperm [ 2 ]; position2 = randperm [ 1 ];  position3 = randperm [ 0 ];
			 			 }
		 }
				for( j = 0 ; j < position1-1 ; j++)
				{
					best_gene[ m +demand_num][ j ] = best_gene[ m ][ j ];
				}
				for( j = position2 +1  ; j < position3 ; j++)
				{
				best_gene[ m +demand_num][ j ] = best_gene[ m ][ j ];
				}
				for( j = position1  ; j < position2 ; j++)
				{
				best_gene[ m +demand_num][ j ] = best_gene[ m ][ j ];
				}
				for( j = position3 + 1  ; j < demand_num ; j++)
				{
				best_gene[ m +demand_num][ j ] = best_gene[ m ][ j ];
				}
	}
	for(j=0;j<best_gene_num*2;j++)
	{
		for(k=0;k<demand_num-1;k++)
		{
			best_gene_cost[j]+=cost[best_gene[j][k]][best_gene[j][k+1]];
		}
	}
	for( k =0 ; k <  best_gene_num * 2; k++)  randperm [k] = k ;
	QuickSort(best_gene_cost , randperm , 0 , best_gene_num*2 );
	opt=best_gene_cost [ best_gene_num*2 -1] - best_gene_cost [ 0 ];
	QuickSort( randperm ,  best_gene_cost,  0 , best_gene_num );
	for(j=0;j<best_gene_num;j++)
		{
			for(k=0;k<demand_num-1;k++)
			{
				best_gene[ j + best_gene_num*2 ][ k ] =best_gene[ randperm[ j ]  ][ k ] ;
			}
		}
	for(j=0;j<best_gene_num;j++)
			{
				for(k=0;k<demand_num-1;k++)
				{
					best_gene[ j  ][ k ] =best_gene[ j + best_gene_num*2  ][ k ] ;
				}
			}
	//if(opt<=opt_limit)break;
}
for(i=0;i<demand_num-1;i++)
{
	printf("%d",best_gene[0][i]);
	print_path(best_gene[0][i],best_gene[0][i+1],path);
	if(i==demand_num-2)
		{
		printf("%d\n",best_gene[0][i+1]);
		}
}
	//print_1array(demand_num,path[19]);
	//printf("%d\n",best_gene_cost[0]);
	//print_1array(demand_num,best_gene[0]);
	//print_1array(demand_num,demand_col);
	//print_2array(vertex_num,vertex_num,cost);
	printf("\n");
	//print_2array(vertex_num,vertex_num,path);
	unsigned short result[] = {2, 6, 3};//示例中的一个解
    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}






