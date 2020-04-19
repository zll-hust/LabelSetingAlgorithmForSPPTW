//SPPTW GLSA 
//By ZLL_HUST

#include <iostream>
#include <vector>
using namespace std;

class label  //标记类，存放该路径当前所在的结点，总时间，总花费 
{
	public:
		int node;
		int time;
 	    int cost;
	    label(){node=-1,time=-1,cost=-1;};
};

int const N=4;
int const INF=9999;

vector<vector<label> > Q(N);//将集合Q，P定义为二维向量，存放每一个结点的labels 
vector<vector<label> > P(N);

double cost[N][N];//这里采用简单的二维表来存储数据，不多作展开，可以关注公众号以后的推文 
double time[N][N];
double time_win_a[N]={0,3,4,4};
double time_win_b[N]={0,10,6,10};

//初始化数据 
void init();
//查找字典序最小的label 
label minlex(vector<label> &);
//构造集合T：未处理的labels 
bool buildT(vector<label> &);
//dominance判断，剪枝无效label 
bool EFF(label);
//对label的总处理 
void treatlabel(label);
//GLSA总流程 
void GLSA();

//初始化数据 ，采用推文中所选例子，加入了负权边 
void init()
{
	label X0;
	X0.cost=0;
	X0.time=0;
	X0.node=0;
	vector<label> X;
	X.push_back(X0);
	Q[0]=X; 
	
	for (int i=0;i<N;i++)
	    for(int j=0;j<N;j++)
	        cost[i][j]=time[i][j]=-INF;
	
	cost[0][1]=2;
	time[0][1]=3;
	
	cost[1][2]=5;
	time[1][2]=2;
	
	cost[0][3]=-7;
	time[0][3]=5;
	
	cost[3][1]=1;
	time[3][1]=1; 
}

//查找字典序最小的label ，优先判断time，其次判断cost 
label minlex(vector<label> &T)
{
	label min;
	min.time=INF;
	for (int i=0;i<T.size();i++)
		if (T[i].time<min.time || (T[i].time==min.time && T[i].cost<min.cost))
			min=T[i];
	return min;
}

//构造集合T：未处理的labels。由于我们往集合Q和P中加入labels是有序的，所以只需要从P.size后面开始加入。 
bool buildT(vector<label> &T)
{
	for (int i=0;i<N;i++) 
		for (int j=P[i].size();j<Q[i].size();j++)
		   T.push_back(Q[i][j]); 
	if (T.size()==0) 
	   return false;//若所有标记都已处理，返回false值，退出程序 
	else 
	   return true;
}

//dominance判断，剪枝无效label 
bool EFF(label next)
{
	bool is_dominated=false;
	for (int i=0;i<Q[next.node].size();i++)
		if (next.time>Q[next.node][i].time && next.cost>Q[next.node][i].cost)
		    is_dominated=true;
	return !is_dominated;
}

//对label的总处理 
void treatlabel(label FT)
{
	for(int succ=1;succ<N;succ++)
	{
	if (cost[FT.node][succ]!=-INF)//寻找后继结点 
	{
		if ((FT.time+time[FT.node][succ])<=time_win_b[succ])//时间窗约束 
		{
			label next;//更新标记 
			next.time=(FT.time+time[FT.node][succ])>time_win_a[succ]?(FT.time+time[FT.node][succ]):time_win_a[succ];
			next.cost=FT.cost+cost[FT.node][succ];
			next.node=succ;
			//cout<<FT.node <<"'s one successor is"<<succ<<next.cost <<next.time <<endl;
			if (EFF(next))
			   Q[next.node].push_back(next);
		}
	}
    }
	P[FT.node].push_back(FT);//FT已处理，加入集合P 
}

//GLSA总流程 
void GLSA()
{
	bool flag=true;
	vector<label> T;
	flag=buildT(T);
	int i=0;
	while(flag)
	{
		label FT=minlex(T);
		treatlabel(FT);
		T.clear();
		flag=buildT(T);
		cout<<"F(T)'s cost is  "<<FT.cost<<"\t"<<"F(T)'s time is  "<<FT.time <<"\t"<<"F(T)'s node is  "<<FT.node <<endl;
	}
	for (int i=1;i<N;i++)//由于加入集合时按照先时间后花费的标准，因此每个结点的最后一个label为最迟、最短路径 
	   cout<<"起点1到达点"<<i+1<<"的最短路径花费为"<<Q[i][Q[i].size()-1].cost<<endl; 
}

int main()
{
	init();
	GLSA();
	return 0;
}
