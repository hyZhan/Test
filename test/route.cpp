
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
{  int temp=0;
	temp=best_gene[0][mutate_num];best_gene[0][mutate_num]=best_gene[0][demand_num-1-mutate_num];best_gene[0][demand_num-1-mutate_num]=temp;
	temp=best_gene[1][mutate_num];best_gene[1][mutate_num]=best_gene[1][demand_num-1-mutate_num];best_gene[1][demand_num-1-mutate_num]=temp;
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
	int i,j,k,temp,vertex_num=0,demand_num=0,best_gene_num,iteration,opt,opt_limit,opt_min1,opt_min2; //暫存变量i,j,k,m,temp,顶点数vertex_num，总需求列表长度demand_num，种群大小best_gene_num,迭代数iteration,优化值opt,优化阈值opt_limit
	//拓扑矩阵topo_array，权值矩阵cost，路径矩阵path，边ID矩阵edge_array，总需求列表demand_col，父代及其子代构成的种群best_gene
	int topo_array[5000][4]={0},cost[MaxVertex][MaxVertex],path[MaxVertex][MaxVertex]={0},edge_array[MaxVertex][MaxVertex]={0},demand_col[MaxDemand+2]={0},best_gene[MaxDemand*2][MaxDemand+2],best_gene_cost[MaxDemand*2]={0};
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
		while((*(topo+i))[j]!='\0')
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
			if((*(topo+i))[j+1]==10||(*(topo+i))[j+1]=='\0')
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
while(demand[j]!='\0')   //读取demand.csv表至demand_col
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
	if(demand[j]==10||demand[j]=='\0')
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
Off_Spring(demand_num,best_gene);
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
iteration=50*(demand_num-2)/10;
opt_limit=(demand_num-2)*((demand_num-2)/10);
//opt_limit=50;
opt=INF;
for(i=0;i<iteration;i++)
{
	std::fill_n(best_gene_cost, MaxDemand*2, 0);
//	best_gene_cost[MaxDemand*2]={0};
	for(j=0;j<best_gene_num;j++)
	{
		for(k=0;k<demand_num-1;k++)
		{
			best_gene_cost[j]+=cost[best_gene[j][k]][best_gene[j][k+1]];
		}
	}
	if(best_gene_cost[0]<best_gene_cost[1])
	{
		opt_min1=0;opt_min2=1;
	}
	else{opt_min1=1;opt_min2=0;}
	for(j=2;j<best_gene_num-1;j++)
	{
		if(best_gene_cost[j]<best_gene_cost[opt_min2])
		{
			if(best_gene_cost[j]<best_gene_cost[opt_min1])
					{
				opt_min2=opt_min1;opt_min1=j;
					}
			else{
				opt_min2=j;
			}
		}
	}
	best_gene_cost[0]=best_gene_cost[opt_min1];best_gene_cost[1]=best_gene_cost[opt_min2];
	opt=opt-best_gene_cost[0];
	if(opt<=opt_limit)break;
	for(i=1;i<demand_num-1;i++)		//产生下一个父代1
		{
			best_gene[0][i]=best_gene[opt_min1][i];
		}
	for(i=1;i<demand_num-1;i++)		//产生下一个父代2
		{
		best_gene[0][i]=best_gene[opt_min2][i];
		}
	Off_Spring(demand_num,best_gene);
}
	printf("%d\n",best_gene_cost[0]);
	print_1array(demand_num,best_gene[0]);
	//print_2array(vertex_num,vertex_num,cost);
	//print_2array(vertex_num,vertex_num,path);
	/*
    for(i=0;i<vertex_num;i++)
    	{
    		for(j=0;j<vertex_num;j++)
    				{
    					printf ("%d ",cost[i][j]);
    				}
    		printf("\n");
    	}
    for(i=0;i<vertex_num;i++)
    	{
    		for(j=0;j<vertex_num;j++)
    				{
    					printf ("%d ",path[i][j]);
    				}
    		printf("\n");
    	}
    	*/
	unsigned short result[] = {2, 6, 3};//示例中的一个解
    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}


