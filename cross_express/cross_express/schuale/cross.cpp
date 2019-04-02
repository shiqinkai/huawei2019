#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<hash_map>
#include<list>
using namespace std;
int NUM_OF_CROSS = 0;
int NUM_OF_ROAD = 0;
int NUM_OF_CAR = 0;
int MAX_PLAN_TIME = 0;
int MIN_V_ROAD = 9999;
class cross {
private:
	int Id;
	int road1;
	int road2;
	int road3;
	int road4;
	int finish_road;   //ÿ��·����ɵ��ȵĵ�·�������ڵ��ȵڶ���
public:
	cross(int id=0 , int r1 = -1, int r2 = -1, int r3 = -1, int r4 = -1,int fi=0);
	void printCross();
	int getId() { return Id; }
	int getroad1() { return road1; }
	int getroad2() { return road2; }
	int getroad3() { return road3; }
	int getroad4() { return road4; }
	int getfinish_road() { return finish_road; }
	void setfinish_road(int f) { finish_road = f; }
};
class road {
private:
	int Id;
	int longth;
	int max_v;
	int num_road;
	int fromId;
	int toId;
	int isDouble;
	int act_v;
	int num_car1;      //�������еĳ�����
	int num_car2;
	float congestion1; //����ӵ���̶�
	float congestion2;
	int *first_sch=nullptr;  //��¼��ǰ��·�ȴ�״̬�ĵ�һ��
	int *last_sch=nullptr; //��¼ÿ���������һ���ȴ�״̬�ĳ���λ��
	int *last_finish = nullptr; //��¼ÿ������first_schǰ����ɵ����һ������λ��
	int *is_finished = nullptr; //��¼ÿ����·�ڵ��ȹ������Ƿ��Ѿ�������ɡ�
	int isFrom; //�ñ������ڵ��ȵĵڶ����������жϸ�·���Ǹõ�·��FromID����ToID�����ھ���ʹ��Lane�����е���һ�������ݣ�0��ǰ��Σ�1�ú���
public:
	road(int id = 0, int lon = 0, int v = 0, int num = 0, int from = 0, int to = 0, int isD = 0, int a_v = 0, int n_c1 = 0, int n_c2 = 0, float con1 = 0, float con2 = 0 );
	int getId() {return Id;}
	int getlongth() { return longth; }
	int getmax_v() {return max_v;}
	int getnum_road() { return num_road; }
	int getfromId() { return fromId; }
	int gettoId() { return toId; }
	int getisDouble() { return isDouble; }
	int getact_v() { return act_v; }
	int getnum_car1() { return num_car1; }
	int getnum_car2() { return num_car2; }
	float getcongestion1() { return congestion1; }
	float getcongestion2() { return congestion2; }
	void setcongestion1(float con) { congestion1 = con; }
	void setcongestion2(float con) { congestion2 = con; }
	void setnum_car1(int c) { num_car1 = c; }
	void setnum_car2(int c) { num_car2 = c; }
	void setact_v(int v) { act_v = v; }
	int getfirst_sch(int a) { return *(first_sch+a); }
	void setfirst_sch(int a, int val) { *(first_sch + a) = val; }
	int getlast_sch(int a) { return *(last_sch + a); }
	void setlast_sch(int a,int val){ *(last_sch + a) = val; }
	int getlast_finish(int a) { return *(last_finish + a); }
	void setlast_finish(int a, int val) { *(last_finish + a) = val; }
	int getis_finished(int a) { return *(is_finished + a); }
	void setis_finished(int a, int val) { *(is_finished + a) = val; }
	int getisFrom() { return isFrom; }
	void setisFrom(int a) { isFrom = a; }
	void printRoad();
};
class car {
private:
	int Id;
	int fromId;
	int toId;
	int max_v;
	int plan_time;
	int act_time;
	int state;   //��¼����ǰ��״̬��1δ������2��������3δ���ȣ�4�ȴ���5�ѵ��ȣ�6�ѽ���  
	int operate;  //��¼�Գ��Ĳ�����1ֱ�У�2��ת��3��ת
public:
	car(int id = 0, int from = 0, int to = 0, int v = 0, int pt = -1, int at = -1,int sta=1,int ope=0);
	int getId() { return Id; }
	int getfromId() { return fromId; }
	int gettoId() { return toId; }
	int getmax_v() { return max_v; }
	int getplan_time() { return plan_time; }
	int getact_time() { return act_time; }
	int getstate() { return state; }
	int getoperate() { return operate; }
	void setact_time(int t) { act_time = t; }
	void setstate(int s) { state = s; }
	void setoperate(int o) { operate = o; }
	void printCar();
};
car::car(int id, int from, int to, int v, int pt,int at,int sta,int ope)
{
	Id = id;
	fromId = from;
	toId = to;
	max_v = v;
	plan_time = pt;
	act_time = at;
	state = sta;
	operate = ope;
}
road::road(int id, int lon, int v, int num, int from, int to, int isD,int a_v,int c1,int c2,float con1,float con2)
{
	Id = id;
	longth = lon;
	max_v = v;
	num_road = num;
	fromId = from;
	toId = to;
	isDouble = isD;
	act_v = v;
	num_car1 = c1;
	num_car2 = c2;
	congestion1 = con1;
	congestion2 = con2;
	first_sch = new int[isD == 1 ? 2 : 1];
	for (int i = 0;i < isD + 1;i++)
		first_sch[i] = i*lon*num;
	last_sch = new int[num*(isD + 1)];
	for (int i = 0;i < num*(isD + 1);i++)
		last_sch[i] = (i / num)*lon*num + i%num;
	last_finish=new int[num*(isD + 1)];
	for (int i = 0;i < num*(isD + 1);i++)
		last_sch[i] = -1;
	is_finished = new int[isD + 1];
	for (int i = 0;i < isD + 1;i++)
		is_finished[i] = 0;
	isFrom = 0;

}
cross::cross(int id,int r1,int r2,int r3,int r4,int f)
{
	Id = id;
	road1 = r1;
	road2 = r2;
	road3 = r3;
	road4 = r4;
	finish_road = f;
}
void cross::printCross()
{
	cout << "ID=" << Id << ",road1=" << road1 << ",road2=" << road2 << ",road3=" << road3 << ",road4=" << road4 << endl;
}
void road::printRoad()
{
	cout << "ID=" << Id << ",longth=" << longth << ",max_v=" << max_v << ",num_road=" << num_road << ",isD=" << isDouble << endl;
}
void car::printCar()
{
	cout << "ID=" << Id << ",fromId=" << fromId << ",toId=" << toId << ",max_v=" << max_v << ",planTime=" << plan_time << endl;
}
void makeCross(vector<cross>&Cross, string A)  //��cross.txt���뵽cross��Vector��
{
	if (A[0] == '#') 
		return;
	else
	{
		int pos = 1;
		int state = 0;
		int i = 1;
		int num = 0;
		int Id;
		int road1;
		int road2;
		int road3;
		int road4;
		while (A[i] != ')')
		{
			if (A[i] >= '0'&&A[i] <= '9')
			{
				num = num * 10 + A[i] - '0';
				i++;
			}
			else if (A[i] == '-')
			{
				pos = 0;
				i++;
			}
			else if (A[i] == ',')
			{
				if (pos == 0) num = -num;
				if (state == 0)  Id = num;
				if (state == 1) road1 = num;
				if (state == 2) road2 = num;
				if (state == 3) road3 = num;
				state++;
				num = 0;
				pos = 1;
				i++;
			}
			else
				i++;
		}
		if (pos == 0) num = -num;
		road4 = num;
		Cross.push_back(cross(Id, road1, road2, road3, road4));
		NUM_OF_CROSS++;
	}
}
void makeRoad(vector<road>&Road, string A)  //��road.txt���뵽road��Vector�У������ݵ�˫�������������顣
{
	if (A[0] == '#')
		return;
	else
	{
		int state = 0;
		int i = 1;
		int num = 0;
		int Id;
		int longth;
		int max_v;
		int num_road;
		int fromId;
		int toId;
		int isD;
		while (A[i] != ')')
		{
			if (A[i] >= '0'&&A[i] <= '9')
			{
				num = num * 10 + A[i] - '0';
				i++;
			}
			else if (A[i] == ',')
			{
				if (state == 0)  Id = num;
				if (state == 1) longth = num;
				if (state == 2) max_v = num;
				if (state == 3) num_road = num;
				if (state == 4) fromId = num;
				if (state == 5) toId = num;
				state++;
				num = 0;
				i++;
			}
			else
				i++;
		}
		isD = num;
		if (max_v < MIN_V_ROAD)     //��¼���е�·������ٵ���Сֵ
			MIN_V_ROAD = max_v;
		Road.push_back(road(Id,longth,max_v,num_road,fromId,toId,isD ));
		NUM_OF_ROAD++;
	}
}
void makeCar(vector<car>&Car, string A,hash_map<int,list<int>>&map)   //��car.txt���뵽Car��Vector�С�
{
	if (A[0] == '#')
		return;
	else
	{
		int state = 0;
		int i = 1;
		int num = 0;
		int Id;
		int fromId;
		int toId;
		int max_v;
		int plan_time;
		while (A[i] != ')')
		{
			if (A[i] >= '0'&&A[i] <= '9')
			{
				num = num * 10 + A[i] - '0';
				i++;
			}
			else if (A[i] == ',')
			{
				if (state == 0)  Id = num;
				if (state == 1) fromId = num;
				if (state == 2) toId = num;
				if (state == 3) max_v = num;
				state++;
				num = 0;
				i++;
			}
			else
				i++;
		}
		plan_time = num;
		Car.push_back(car(Id, fromId, toId, max_v, plan_time));
		if (map.count(plan_time) == 0)                    //�˹���Ϊ��¼ÿ������Ƭ�εĶ�Ӧ�ĳ��ƺ�
		{
			list<int>Li;
			map.insert(pair<int, list<int>>(plan_time, Li));
			map[plan_time].push_back(Id);
		}
		else
			map[plan_time].push_back(Id);
		if (plan_time > MAX_PLAN_TIME)             //�˹���Ϊ��¼���ļƻ�����ʱ���
			MAX_PLAN_TIME = plan_time;
		NUM_OF_CAR++;
	}
}
void makeLane(int **Lane,const int num,vector<road> &Road) //������������
{
	for (int i = 0;i < num;i++)
	{
		int Id = Road[i].getId();
		int isD = Road[i].getisDouble();
		int num_road = Road[i].getnum_road();
		int longth = Road[i].getlongth();
		int num_lane = isD == 1 ? num_road * 2 : num_road;//������
		int long_int = num_lane*longth;//���г�������
		Lane[Id - Road[0].getId()] = new int[long_int];  //������������
		for (int i = 0;i < long_int;i++)
			Lane[Id - Road[0].getId()][i] = -1;  //�Գ�����ÿһ��λ�ý��г�ʼ������ʼ��Ϊ-1
	}
}
void makeNodeToPath(int *nodeToPath,vector<road>& Road, int num_cross, int num_road, int first)   //�ú������ڼ�¼����·��֮��ĵ�·�ţ����ں��ڽ�floyd�㷨�õ��Ľڵ�·��ת��Ϊ��··��
{
	for (int i = 0;i < num_road;i++)
	{
		int id = Road[i].getId();
		int isD = Road[i].getisDouble();
		int from = Road[i].getfromId() - first;
		int to = Road[i].gettoId() - first;
		nodeToPath[from*num_cross + to] = id;
		if (isD == 1)
			nodeToPath[to*num_cross + from] = id;
	}
}
void makeMinLen2Nodes(int *minLen2Nodes1,int *minLen2Nodes2,int *pathOf2Nodes1, int *pathOf2Nodes2, vector<road>&Road,int num_road,int num_cross, int first)  //����ʼʱ��������֮�����̾������ʻʱ��
{
	for (int i = 0;i < num_road;i++)
	{
		int from = Road[i].getfromId()-first; 
		int to = Road[i].gettoId()-first;
		int isD = Road[i].getisDouble();
		int len = Road[i].getlongth();
		int v = Road[i].getact_v();
		float con = Road[i].getcongestion1();
		int t = len%v == 0 ? len / v : len / v + 1;
		minLen2Nodes1[from*num_cross + to] = len + (con>0.6 ? (int)con*len : 0);   //��߼�����������֮����̲�������Ŀ����һ�����ڳ��ٸ��ڵ�·��Сֵ��ʹ�ã�һ�������ڳ���С�ڵ�·������Сֵ��ʹ��
		minLen2Nodes2[from*num_cross + to] = t+(con>0.6?(int)con*t:0);
		pathOf2Nodes1[from*num_cross + to] = to ;
		pathOf2Nodes2[from*num_cross + to] = to ;
		if (isD == 1)
		{
			float con1 = Road[i].getcongestion2();
			minLen2Nodes1[to*num_cross + from] = len + (con1>0.6 ? (int)con*len : 0);
			minLen2Nodes2[to*num_cross + from] = t + (con1>0.6 ? (int)con*t : 0);
			pathOf2Nodes1[to*num_cross + from] = from ;
			pathOf2Nodes2[to*num_cross + from] = from ;
		}
	}
	for (int k = 0;k < num_cross;k++)   //floyd �㷨�������������֮�����̾���
	{
		for (int i = 0;i < num_cross;i++)
			for (int j = 0;j < num_cross;j++)
			{
				if (minLen2Nodes2[i*num_cross + j]>(minLen2Nodes2[i*num_cross + k] + minLen2Nodes2[k*num_cross + j]))
				{
					minLen2Nodes2[i*num_cross + j] = minLen2Nodes2[i*num_cross + k] + minLen2Nodes2[k*num_cross + j];
					pathOf2Nodes2[i*num_cross + j] = pathOf2Nodes2[i*num_cross + k]; 
				}
				if (minLen2Nodes1[i*num_cross + j]>(minLen2Nodes1[i*num_cross + k] + minLen2Nodes1[k*num_cross + j]))
				{
					minLen2Nodes1[i*num_cross + j] = minLen2Nodes1[i*num_cross + k] + minLen2Nodes1[k*num_cross + j];
					pathOf2Nodes1[i*num_cross + j] = pathOf2Nodes1[i*num_cross + k];
				}
			}
	}
}
/*




void driveAllCarJustOnRoadToEndState1(int No_road, int **Lane, vector<car>&Car, vector<road>&Road)  //���ڵ����㷨�ĵ�һ�� No_road�����ֵ����ȥ��һ��·��Id
{                                                                                                                         
	int longth = Road[No_road].getlongth();
	int num = Road[No_road].getnum_road();
	int isD = Road[No_road].getisDouble();
	int first = Car[0].getId();
	int len = longth*num;
	int *last_one = new int[num];
		for (int i = 0;i < num;i++)
			last_one[i] = -1;
		for (int i = 0;i < len;i++)
		{
			if (Lane[No_road][i] != -1)
			{
				int v = min(Car[Lane[No_road][i] - first].getmax_v(), Road[No_road].getmax_v());
				if (i - v*num < 0 || (last_one[i%num] >= (i - v*num)&& Car[Lane[No_road][last_one[i%num]] - first].getstate()==4)) //˵���ó���·�ڻ���ǰ���еȴ��ĳ�����
				{
					Car[Lane[No_road][i] - first].setstate(4);   //�ó�״̬����Ϊ�ȴ�
					last_one[i%num] = i;
				}
				else if (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //�ó���ǰ����ǰ��ĳ��ѵ����꣬���ڸó������ʹ������
				{
					Car[Lane[No_road][i] - first].setstate(5);   //�ó�״̬����Ϊ��ɵ���
					if (i != last_one[i%num] + num)              //���ó�����ǰ������һ��
					{
						Lane[No_road][last_one[i%num] + num] = Lane[No_road][i];  
						Lane[No_road][i] = -1;
						last_one[i%num] = last_one[i%num] + num;
					}
					else
						last_one[i%num] = i;
				}
				else if(Car[Lane[No_road][last_one[i%num]] - first].getstate() == 3)   //δ����״̬�ĳ�
				{
					Car[Lane[No_road][i] - first].setstate(5);    //���ó�������ʻʱ����ʻ��������Ϊ�ѵ������
					Lane[No_road][i - v*num] = Lane[No_road][i];
					Lane[No_road][i] = -1;
					last_one[i%num] = i - v*num;
				}
			}
			Road[No_road].setfirst_sch(0, 0);  //��ʼ��first_schΪ0
			for (int i = 0;i < num;i++)
			{
				Road[No_road].setlast_sch(i, last_one[i]);  //��ʼ��last_sch��ֵΪ��ǰ���������һ�������ڵ�λ��
				Road[No_road].setlast_finish(i, -1);   //��ʼ��last_finishΪ-1
			}
		}
		if (isD == 1)           //����˫����ʱ����Ҫ����һ���򳵵����д���
		{
			for (int i = 0;i < num;i++)
				last_one[i] = len-1;
			for (int i = len;i < 2 * len;i++)     
			{
				if(Lane[No_road][i] != -1)
				{
					int v = min(Car[Lane[No_road][i] - first].getmax_v(), Road[No_road].getmax_v());
					if (i - v*num < 0 || (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 4)) //˵���ó���·�ڻ���ǰ���еȴ��ĳ�����
					{
						Car[Lane[No_road][i] - first].setstate(4);   //�ó�״̬����Ϊ�ȴ�
						last_one[i%num] = i;
					}
					else if (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //�ó���ǰ����ǰ��ĳ��ѵ����꣬���ڸó������ʹ������
					{
						Car[Lane[No_road][i] - first].setstate(5);   //�ó�״̬����Ϊ��ɵ���
						if (i != last_one[i%num] + num)              //���ó�����ǰ������һ��
						{
							Lane[No_road][last_one[i%num] + num] = Lane[No_road][i];
							Lane[No_road][i] = -1;
							last_one[i%num] = last_one[i%num] + num;
						}
						else
							last_one[i%num] = i;
					}
					else
					{
						Car[Lane[No_road][i] - first].setstate(5);    //���ó�������ʻʱ����ʻ��������Ϊ�ѵ������
						Lane[No_road][i - v*num] = Lane[No_road][i];
						Lane[No_road][i] = -1;
						last_one[i%num] = i - v*num;
					}
				}
				int lon = Road[No_road].getlongth();
				int num = Road[No_road].getnum_road();
				int len = lon*num;
				Road[No_road].setfirst_sch(1, len);    //��ʼ��first_schΪlen
				
				for (int i = num;i < 2*num;i++)
				{
					Road[No_road].setlast_sch(i, last_one[i%num]);   //��ʼ��ÿ��������last_schΪÿ���������һ������λ��
					Road[No_road].setlast_finish(i, len-1);   //��ʼ��last_finishΪlen-1
				}
			}
		}
		delete[]last_one;
}
int getCarId(int roadId, vector<road>&Road,vector<car>&Car, int **Lane,int isFrom,int first_car)   //�ú��������õ���ǰ��·���ȵ��õĵȴ�������ID��������ý������򷵻�-1�������������
{
	int first_sch = Road[roadId].getfirst_sch(isFrom);
	int lon = Road[roadId].getlongth();
	int num = Road[roadId].getnum_road();
	int len = lon*num;
	while (first_sch !=len + len*isFrom)
	{
		if (Lane[roadId][first_sch] == -1)
			first_sch++;
		else if (Car[Lane[roadId][first_sch] - first_car].getstate() == 5)
			first_sch++;
		else
		{
			Road[roadId].setfirst_sch(isFrom, first_sch);    //��first_sch���и���
			return Lane[roadId][first_sch];
		}
	}
	Road[roadId].setis_finished(isFrom, 1);     //��ʾ�õ�·�Ѿ�������ɣ�����-1
	return -1;
}
int driveCar1(int carId,int No_Cross,int first_cross,int cur_road, int next_road, vector<road>&Road, vector<car>&Car, int **Lane)   //�ú������ڴ���Ҫ��·�ڵĳ�����������ز���
{
	int first_car = Car[0].getId();
	int cur_num = Road[cur_road].getnum_road();
	int cur_lon = Road[cur_road].getlongth();
	int next_num = Road[next_road].getnum_road();
	int next_lon = Road[next_road].getlongth();
	int isFrom = Road[cur_road].getisFrom();    //�жϵ�ǰ·�������cur_road��from����to
	int isFromNext = No_Cross + first_cross == Road[next_road].getfromId() ? 1 : 0;
	int cur_sch = Road[cur_road].getfirst_sch(isFrom);
	int cur_v = min(Road[cur_road].getmax_v(), Car[carId - first_car].getmax_v());      //�õ���ǰ��·����
	int next_v = min(Road[next_road].getmax_v(), Car[carId - first_car].getmax_v());      //�õ���һ����·�ĳ���
	int cur_d = cur_sch / cur_num;    //�ó�����·�ڻ��еľ���
	int next_exp_d = next_v - cur_d > 0 ? next_v - cur_d : 0;
	//int num_of_next;  //��¼�ó���Ҫʻ�����һ����·�ĳ�����
	//***********�жϽ��ó����뵽��һ����·���ĸ�����
	if (next_exp_d != 0)   
	{
		for (int i = 0;i < next_num;i++)
		{
			int loc = Road[next_road].getlast_sch(next_num*isFromNext + i) / next_num - next_lon*isFromNext;
			if (loc == next_lon - 1)    //��ʾ���������Ѿ�����
				continue;
			if (loc != next_lon - 1)   //��ʾ������·û��
			{
				int remain = next_lon - 1 - loc;   //��δ����·ʣ��Ŀ��Էų��Ŀռ�
				if (remain >= next_exp_d)  //��ʾ��һ����·ʣ��Ŀռ������Ҫ��ʻ�Ŀռ�
				{
					int next_loc = next_num*next_lon*isFromNext + next_num*(next_lon - next_exp_d) + i;
					Lane[next_road][next_loc] = carId;    //����ʻ��ȥ
					Car[carId - first_car].setstate(5);    //��ʻ��ȥ�ĳ���־λ�ѵ���
					Road[next_road].setlast_sch(next_num*isFromNext + i, next_loc);      //����last_sch
					Lane[cur_road][cur_sch] = -1;   //���ó�λ���
					return 1;
				}
				else if (Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)]].getstate() == 5)   //ʣ�����С��Ԥ�ڣ����ó��������һ�����ѵ�����ȫ������Ҫ���ȵĳ����ڸó�����
				{
					Lane[next_road][loc + next_num] = carId;    //����ʻ��ȥ
					Car[carId - first_car].setstate(5);    //��ʻ��ȥ�ĳ���־λ�ѵ���
					Road[next_road].setlast_sch(next_num*isFromNext + i, loc + next_num);      //����last_sch
					Lane[cur_road][cur_sch] = -1;   //���ó�λ���
					return 1;
				}
				else    //��ʾǰ��һ�����ǵȴ�״̬����Ҫʻ��ĳ������ȴ���������һ����·�ĵ���
				{
					return 0;
				}
			}
		}
	                           //��Ҫʻ��ĵ�·���ˣ�����ͣ��·��
	}
  //����ʻ��·�� ���������������һ����·��������next_exp_d==0
	{
		int cur_len = cur_num*cur_lon;
		Lane[cur_road][isFrom*cur_len + cur_sch%cur_num] = carId;  //�����Ŀ���ȥ
		Lane[cur_road][cur_sch] = -1;     //��ԭ����λ�����
		Road[cur_road].setlast_finish(isFrom*cur_num + cur_sch%cur_num, isFrom*cur_len + cur_sch%cur_num);    //������ʻ��λ�ñ�־λ�ó�����last_finish
		Car[carId - first_car].setstate(5);  //���ó���״̬����Ϊ�ѵ���
		return 1;
	}
}
void driveCar2(int carId,int roadId,vector<road>&Road,vector<car>&Car,int **Lane)   //������Щ����Ҫʻ��·�ڵĳ�
{
	int first_car = Car[0].getId();
	int num_road = Road[roadId].getnum_road();
	int lon = Road[roadId].getlongth();
	int len = num_road*lon;
	int v = min(Car[carId - first_car].getmax_v(), Road[roadId].getmax_v());
	int isFrom = Road[roadId].getisFrom();
	int first_sch = Road[roadId].getfirst_sch(isFrom);
	int last_finish = Road[roadId].getlast_finish(isFrom*num_road + first_sch%num_road);
	if (first_sch - v*num_road > last_finish)   //���ó�������ʻ������
	{
		Lane[roadId][first_sch - v*num_road] = carId;
		Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, first_sch - v*num_road);
	}
	else            //���ó�ֻ����ʻ��last_finish�ĳ�����
	{
		Lane[roadId][last_finish+num_road] = carId;
		Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, last_finish + num_road);
	}
}
int getNextNo(int No)      //[4,1,2,3]�����·��ʱ��·֮����л�
{                         //[4,3,1,2]��·�Ű��������У������ÿ����·�ı�������,��˳�����ʱ��Ҫת��
	if (No == 3)return 2;
	if (No == 2)return 0;
	if (No == 0)return 1;
	if (No == 1)return 3;
}
void driveAllCarForEachCross(int No_Cross, vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car)   //�˴��Ƕ���ÿ��·�ڵĳ������е��ȣ�No_Cross�����ֵ��
{
	int first_cross = Cross[0].getId();//�õ���һ��·�ڵ�Id���������Id�����Id���л�
	int first_car = Car[0].getId();  //�õ���һ������Id���������Id�����Id���л�
	int first_road = Road[0].getId();//�õ���һ����·��Id���������Id�����Id���л�
	int road1 = Cross[No_Cross].getroad1() - first_road;
	int road2 = Cross[No_Cross].getroad2() - first_road;
	int road3 = Cross[No_Cross].getroad3() - first_road;
	int road4 = Cross[No_Cross].getroad4() - first_road;
	int arr[4] ;    //[4,1,2,3]�����·��ʱ��·֮����л�
	                //[4,3,1,2]��·�Ű��������У������ÿ����·�ı�������,��˳�����ʱ��Ҫת��
	arr[0] = road4;
	arr[1] = road1;
	arr[2] = road2;
	arr[3] = road3;
	if (Cross[No_Cross].getfinish_road() == 0)     //�˴���Ҫ�Ƕ�ÿ��ʱ��Ƭ��һ�ֵ�finish_road��ֵ���иĽ����ų�-1�ıߵ�Ӱ��
		for (int i = 0;i < 4;i++)
			if (arr[i] == -1)
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
			}
	for (int i = 0;i < 4;i++)    //�жϸ�·��ID���ÿ����·��FromID����ToID
	{
		Road[arr[i]].setisFrom( No_Cross + first_cross == Road[arr[i]].getfromId() ? 1 : 0);
	}
	int cur_road = (road4 != -1 &&Road[road4].getis_finished(Road[road4].getisFrom())==0)? road4 : ((road3 != -1 && Road[road3].getis_finished(Road[road3].getisFrom()) == 0) ? road3 : ((road1 != -1 && Road[road1].getis_finished(Road[road1].getisFrom()) == 0) ? road1 : road2));   //�õ���С�ĵ�·����Ϊִ�е�·��
	int No = cur_road == road4 ? 0 : (cur_road == road1 ? 1 : (cur_road == road2 ? 2 : 3));   //�õ���ǰ��·��Ӧ�ı�ţ������еı�ţ�
	while (Cross[No_Cross].getfinish_road()<4)    //����ǰ·�ڻ��е�·û�е�����
	{
		int num_of_driven = 0;    //�ñ������ڼ�¼ÿһ��ѭ���ƶ��ĳ�����
		int carID = getCarId(cur_road, Road, Car,Lane,Road[cur_road].getisFrom(),first_car);      //��ȡ��ǰ�����ȵĳ���carID
		if (carID == -1)    //��ʾ��ǰ��·�Ѿ��������
		{
			int num = Cross[No_Cross].getfinish_road();
			num++;
			Cross[No_Cross].setfinish_road(num);
			if (num == 4)          //��ʾ��ǰ·���Ѿ�������
				break;
			No = getNextNo(No);    //��˳�������һ����·
			cur_road = arr[No];
			continue;
		}
		if (Car[carID-first_car].getoperate() == 0)         //��ʾ��ǰ�ĳ�Ϊֱ��
		{
			int next_road = arr[(No + 2) % 4];
			int isdrive=driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
			if (isdrive == 0)  //��ʾ�ó�Ϊ�����ȴ���ת����һ����·
			{
				No = getNextNo(No);    //��˳�������һ����·
				cur_road = arr[No];
			}
			continue;
		}
		else if (Car[carID].getoperate() == 1)     //��ʾ��ǰ�ĳ�Ϊ��ת
		{
			int next_road = arr[(No + 3) % 4];
			int isFrom_next = Road[next_road].getisFrom();
			int carID1 = getCarId(next_road, Road, Car, Lane, isFrom_next, first_car);
			if (Car[carID1 - first_car].getoperate() == 0)     //����ת����ֱ��ʱ����Ҫת��ֱ�г���
			{
				No = (No + 3) % 4;
				cur_road = next_road;
				continue;
			}
		}

	}



}
*/
int main()
{
	vector<cross> Cross;  //�����洢·��
	vector<road>Road;   //�����洢��·
	vector<car>Car;   //�����洢����
	hash_map<int, list<int>>EachTimeOfPlan;  //ÿ��ʱ��Ƭ׼�������ĳ���ɵ�����
	queue<queue<int>>ThePathOfDrive;  //��¼ÿ�������г�·�ߣ���һλ��¼��������ʵ����ʱ��
	//vector<cross>::iterator pos;
	string CrossTXT = "cross.txt";
	string RoadTXT = "road.txt";
	string CarTXT = "car.txt";
	ifstream infile;
	string line;
	infile.open(CrossTXT);
	if (infile) // �и��ļ�  
	{
		while (getline(infile, line)) // line�в�����ÿ�еĻ��з�  
		{
			makeCross(Cross, line);
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	infile.open(RoadTXT);
	if (infile) // �и��ļ�  
	{
		while (getline(infile, line)) // line�в�����ÿ�еĻ��з�  
		{
			makeRoad(Road,line);
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	infile.open(CarTXT);
	if (infile) // �и��ļ�  
	{
		while (getline(infile, line)) // line�в�����ÿ�еĻ��з�  
		{
			makeCar(Car,line,EachTimeOfPlan);
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	//for (int i = 0;i < NUM_OF_CAR;i++)
	//{
	//	Car[i].printCar();
	//}
	int **Lane = new int*[NUM_OF_ROAD];  //������������
	makeLane(Lane, NUM_OF_ROAD, Road);
	int size = NUM_OF_CROSS*NUM_OF_CROSS;
	int *minLen2Nodes1 = new int[size]; //�洢����֮�����̾���
	for (int i = 0;i < size; i++)
		if (i % (NUM_OF_CROSS + 1) == 0)
			minLen2Nodes1[i] = 0;
		else
			minLen2Nodes1[i] = 999999;
	int *minLen2Nodes2 = new int[size];  //�洢����֮��������ʻʱ��
	for (int i = 0;i < size; i++)
		if (i % (NUM_OF_CROSS + 1) == 0)
			minLen2Nodes2[i] = 0;
		else
			minLen2Nodes2[i] = 999999;
	int *pathOf2Nodes1 = new int[size];
	for (int i = 0;i < size;i++)        //��ȡ·����̵�·��
		pathOf2Nodes1[i] = -1;
	int *pathOf2Nodes2 = new int[size];  //�洢ʱ����̵�·��
	for (int i = 0;i < size;i++)
		pathOf2Nodes2[i] = -1;
	int *nodeToPath=new int[size];    //�洢�������������ڵ�֮��ĵ�·
	for (int i = 0;i < size;i++)
		nodeToPath[i] = -1;
	makeNodeToPath(nodeToPath, Road, NUM_OF_CROSS, NUM_OF_ROAD, Cross[0].getId());
	makeMinLen2Nodes(minLen2Nodes1,minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, Cross[0].getId());  //����ʼʱ��������֮�����̾������ʻʱ��
	//************************just for test
	//infile.open("answer.txt");
	//if (infile)
	//{
	//	infile.close();
	//	DeleteFile("answer.txt");
	//}
	int plan_time = Car[0].getplan_time();
	int pre = 0;
	ofstream outfile("answer.txt");
	
	outfile << "#(carId,StartTime,RoadId...)" << endl;
	for (int i = 0;i < NUM_OF_CAR;i++)
	{
		int Id = Car[i].getId();
		//int plan_time = Car[i].getplan_time();
		int fromId = Car[i].getfromId();
		int toId = Car[i].gettoId();
		int from = fromId - Cross[0].getId();
		int to = toId - Cross[0].getId();
		int v = Car[i].getmax_v();
		plan_time += pre;
		outfile << "(" << Id << "," << plan_time << ",";
		//if (v > MIN_V_ROAD)
		//	outfile << minLen2Nodes2[from*NUM_OF_CROSS + to] << ")" << endl;
		//else
		//  outfile << (minLen2Nodes1[from*NUM_OF_CROSS + to]/v) << ")" << endl;
		if (v > MIN_V_ROAD)
			pre = minLen2Nodes2[from*NUM_OF_CROSS + to];
		else
			pre = (minLen2Nodes1[from*NUM_OF_CROSS + to] / v);
		
		if (v > MIN_V_ROAD)
		{
			int i = from;
			int j = to;
			//outfile << fromId << ",";
			while (pathOf2Nodes2[i*NUM_OF_CROSS + j] != to)
			{
				int k = pathOf2Nodes2[i*NUM_OF_CROSS + j];
				outfile << nodeToPath[i*NUM_OF_CROSS + k] << ",";
				//outfile << k +Cross[0].getId() << ",";
				i = k ;
			}
			outfile << nodeToPath[i*NUM_OF_CROSS + j] <<")" <<endl;
			//outfile << toId <<")"<< endl;
		}
		else
		{
			int i = from;
			int j = to;
			//outfile << fromId << ",";
			while (pathOf2Nodes1[i*NUM_OF_CROSS + j] != to)
			{
				int k = pathOf2Nodes2[i*NUM_OF_CROSS + j];
				outfile << nodeToPath[i*NUM_OF_CROSS + k] << ",";
				//outfile << k +Cross[0].getId() << ",";
				i = k ;
			}
			outfile << nodeToPath[i*NUM_OF_CROSS + j] <<")"<< endl;
			//outfile << toId<<")" << endl;

		}
		
	}
	
	
	//**************************
	//cout << NUM_OF_CROSS << " " << NUM_OF_ROAD << "  " << NUM_OF_CAR << "  " << MAX_PLAN_TIME << endl;
	delete[]Lane;
	delete[]minLen2Nodes1;
	delete[]minLen2Nodes2;
	delete[]pathOf2Nodes1;
	delete[]pathOf2Nodes2;
	delete[]nodeToPath;
	system("pause");
	return 1;
}