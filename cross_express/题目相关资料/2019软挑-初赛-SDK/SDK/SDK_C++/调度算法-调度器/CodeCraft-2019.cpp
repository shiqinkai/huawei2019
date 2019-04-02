#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<map>
#include<list>
using namespace std;
int NUM_OF_CROSS = 0;
int NUM_OF_ROAD = 0;
int NUM_OF_CAR = 0;
int MAX_PLAN_TIME = 0;
int MIN_V_ROAD = 9999;
int NUM_OF_FINISH = 0;    //记录到达目的地的车辆数
int NUM_OF_DEPART = 0;
int NUM_OF_REMAIN = 0;
int NUM_ON_ROAD = 0;
class cross {
private:
	int Id;
	int road1;
	int road2;
	int road3;
	int road4;
	int finish_road;   //每个路口完成调度的道路数，用于调度第二步
public:
	cross(int id = 0, int r1 = -1, int r2 = -1, int r3 = -1, int r4 = -1, int fi = 0);
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
	int num_car1;      //车道上有的车辆数
	int num_car2;
	float congestion1; //车道拥挤程度
	float congestion2;
	int *first_sch = nullptr;  //记录当前道路等待状态的第一辆
	int *last_sch = nullptr; //记录每个车道最后一辆等待状态的车的位置，Lane中的位置
	int *last_finish = nullptr; //记录每条车道first_sch前已完成的最后一辆车的位置，Lane中的位置
	int *is_finished = nullptr; //记录每条道路在调度过程中是否已经调度完成。
	int isFrom; //该变量用于调度的第二步，用于判断该路口是该道路的FromID还是ToID，用于决定使用Lane矩阵中的哪一部分数据，0用前半段，1用后半段
	int *isFull = nullptr;   //用于调度第二步，对于从车库发车的车来说其规划路径需要此参数
public:
	road(int id = 0, int lon = 0, int v = 0, int num = 0, int from = 0, int to = 0, int isD = 0, int a_v = 0, int n_c1 = 0, int n_c2 = 0, float con1 = 0, float con2 = 0);
	int getId() { return Id; }
	int getlongth() { return longth; }
	int getmax_v() { return max_v; }
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
	int getfirst_sch(int a) { return *(first_sch + a); }
	void setfirst_sch(int a, int val) { *(first_sch + a) = val; }
	int getlast_sch(int a) { return *(last_sch + a); }
	void setlast_sch(int a, int val) { *(last_sch + a) = val; }
	int getlast_finish(int a) { return *(last_finish + a); }
	void setlast_finish(int a, int val) { *(last_finish + a) = val; }
	int getis_finished(int a) { return *(is_finished + a); }
	void setis_finished(int a, int val) { *(is_finished + a) = val; }
	int getisFrom() { return isFrom; }
	void setisFrom(int a) { isFrom = a; }
	int getisFull(int a) { return *(isFull + a); }
	void setisFull(int a, int val) { *(isFull + a) = val; }
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
	int state;   //记录车当前的状态：1未发车；2待发车；3未调度；4等待；5已调度；6已结束  
	int operate;  //记录对车的操作：1直行；2左转；3右转
public:
	car(int id = 0, int from = 0, int to = 0, int v = 0, int pt = -1, int at = -1, int sta = 1, int ope = 0);
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
car::car(int id, int from, int to, int v, int pt, int at, int sta, int ope)
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
road::road(int id, int lon, int v, int num, int from, int to, int isD, int a_v, int c1, int c2, float con1, float con2)
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
		last_sch[i] = num*lon*(i / num) - 1;
	last_finish = new int[num*(isD + 1)];
	for (int i = 0;i < num*(isD + 1);i++)
		last_finish[i] = num*lon*(i / num) - 1;
	is_finished = new int[isD + 1];
	for (int i = 0;i < isD + 1;i++)
		is_finished[i] = 0;
	isFrom = 0;
	isFull = new int[isD + 1];
	for (int i = 0;i < isD + 1;i++)
		isFull[i] = 0;

}
cross::cross(int id, int r1, int r2, int r3, int r4, int f)
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
void makeCross(vector<cross>&Cross, string A)  //将cross.txt导入到cross的Vector中
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
void makeRoad(vector<road>&Road, string A)  //将road.txt导入到road的Vector中，并根据单双道创建车道数组。
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
		if (max_v < MIN_V_ROAD)     //记录所有道路最高限速的最小值
			MIN_V_ROAD = max_v;
		Road.push_back(road(Id, longth, max_v, num_road, fromId, toId, isD));
		NUM_OF_ROAD++;
	}
}
void makeCar(vector<car>&Car, string A, map<int, list<int>>&map, vector<vector<int>>&ThePathOfDrive)   //将car.txt导入到Car的Vector中。
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
		if (map.count(plan_time) == 0)                    //此过程为记录每个发车片段的对应的车牌号
		{
			list<int>Li;
			map.insert(pair<int, list<int>>(plan_time, Li));
			map[plan_time].push_back(Id);
		}
		else
			map[plan_time].push_back(Id);
		if (plan_time > MAX_PLAN_TIME)             //此过程为记录最大的计划发车时间点
			MAX_PLAN_TIME = plan_time;
		vector<int>vec;
		ThePathOfDrive.push_back(vec);   //为每一辆车增加一个queue，用来存储车辆的驾驶路线
		NUM_OF_CAR++;
	}
}
void makeAnswer(vector<car>&Car, string A, vector<vector<int>>&ThePathOfDrive)
{
	if (A[0] == '#')
		return;
	else
	{
		int state = 0;
		int i = 1;
		int num = 0;
		int carId = 0;
		int act_time = 0;
		int first_car = Car[0].getId();
		while (A[i] != ')')
		{
			if (A[i] >= '0'&&A[i] <= '9')
			{
				num = num * 10 + A[i] - '0';
				i++;
			}
			else if (A[i] == ',')
			{
				if (state == 0)  carId = num;
				if (state == 1) act_time = num;
				if (state > 1)
				{
					ThePathOfDrive[carId - first_car].push_back(num);
				}
				state++;
				num = 0;
				i++;
			}
			else
				i++;
		}
		ThePathOfDrive[carId - first_car].push_back(num);
		Car[carId - first_car].setact_time(act_time);
	}
}
void makeLane(int **Lane, const int num, vector<road> &Road) //构建车道数组
{
	for (int i = 0;i < num;i++)
	{
		int Id = Road[i].getId();
		int isD = Road[i].getisDouble();
		int num_road = Road[i].getnum_road();
		int longth = Road[i].getlongth();
		int num_lane = isD == 1 ? num_road * 2 : num_road;//车道数
		int long_int = num_lane*longth;//所有车道长度
		Lane[Id - Road[0].getId()] = new int[long_int];  //创建车道数组
		for (int i = 0;i < long_int;i++)
			Lane[Id - Road[0].getId()][i] = -1;  //对车道上每一个位置进行初始化，初始化为-1
	}
}
void makeNodeToPath(int *nodeToPath, vector<road>& Road, int num_cross, int num_road, int first)   //该函数用于记录两个路口之间的道路号，用于后期将floyd算法得到的节点路径转化为道路路径
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
void makeMinLen2Nodes(int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, vector<road>&Road, int num_road, int num_cross, int first)  //求解初始时任意两点之间的最短距离和行驶时间
{
	for (int i = 0;i < num_road;i++)
	{
		int from = Road[i].getfromId() - first;
		int to = Road[i].gettoId() - first;
		int isD = Road[i].getisDouble();
		int len = Road[i].getlongth();
		int v = Road[i].getact_v();
		float con = Road[i].getcongestion1();
		int t = len%v == 0 ? len / v : len / v + 1;
		int isfull1 = Road[i].getisFull(0);
		minLen2Nodes1[from*num_cross + to] = len + (con>0.2 ? (int)con*len : 0) + isfull1 * 2000;   //这边计算两个两点之间最短参数，其目的是一个用于车速高于道路最小值的使用，一个是用于车速小于道路限速最小值的使用
		minLen2Nodes2[from*num_cross + to] = t + (con>0.2 ? (int)con*t : 0) + isfull1 * 2000;
		pathOf2Nodes1[from*num_cross + to] = to;
		pathOf2Nodes2[from*num_cross + to] = to;
		if (isD == 1)
		{
			float con1 = Road[i].getcongestion2();
			int isfull2 = Road[i].getisFull(1);
			minLen2Nodes1[to*num_cross + from] = len + (con1>0.2 ? (int)con*len : 0) + isfull2 * 2000;
			minLen2Nodes2[to*num_cross + from] = t + (con1>0.2 ? (int)con*t : 0) + isfull2 * 2000;
			pathOf2Nodes1[to*num_cross + from] = from;
			pathOf2Nodes2[to*num_cross + from] = from;
		}
	}
	for (int k = 0;k < num_cross;k++)   //floyd 算法求解任意两个点之间的最短距离
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



*/
void driveAllCarJustOnRoadToEndState1(int No_road, int **Lane, vector<car>&Car, vector<road>&Road)  //处于调度算法的第一步 No_road是相对值，减去第一条路的Id
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
			if ((i - v*num < 0 && last_one[i%num]<0) || (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 4)) //说明该车出路口或者前面有等待的车挡着
			{
				Car[Lane[No_road][i] - first].setstate(4);   //该车状态设置为等待
				last_one[i%num] = i;
			}
			else if ((last_one[i%num] >= 0 && last_one[i%num] >= (i - v*num)) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //该车当前车道前面的车已调度完，但在该车最大行使距离内
			{
				Car[Lane[No_road][i] - first].setstate(5);   //该车状态设置为完成调度
				if (i != last_one[i%num] + num)              //当该车不在前车后面一个
				{
					Lane[No_road][last_one[i%num] + num] = Lane[No_road][i];
					Lane[No_road][i] = -1;
					last_one[i%num] = last_one[i%num] + num;
				}
				else
					last_one[i%num] = i;
			}
			else   //该车可以行驶距离v
			{
				Car[Lane[No_road][i] - first].setstate(5);    //当该车可以行驶时，行驶，并设置为已调度完成
				Lane[No_road][i - v*num] = Lane[No_road][i];
				Lane[No_road][i] = -1;
				last_one[i%num] = i - v*num;
			}
		}

	}
	Road[No_road].setfirst_sch(0, 0);  //初始化first_sch为0
	for (int i = 0;i < num;i++)
	{
		Road[No_road].setlast_sch(i, last_one[i]);  //初始化last_sch的值为当前车道的最后一辆车所在的位置
		Road[No_road].setlast_finish(i, -1);   //初始化last_finish为-num+i
	}
	if (isD == 1)           //当是双车道时，需要对另一方向车道进行处理
	{
		for (int i = 0;i < num;i++)
			last_one[i] = len - 1;
		for (int i = len;i < 2 * len;i++)
		{
			if (Lane[No_road][i] != -1)
			{
				int v = min(Car[Lane[No_road][i] - first].getmax_v(), Road[No_road].getmax_v());
				if ((i - v*num < len&& last_one[i%num]<len) || (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 4)) //说明该车出路口或者前面有等待的车挡着
				{
					Car[Lane[No_road][i] - first].setstate(4);   //该车状态设置为等待
					last_one[i%num] = i;
				}
				else if ((last_one[i%num] >= len && last_one[i%num] >= (i - v*num)) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //该车当前车道前面的车已调度完，但在该车最大行使距离内
				{
					Car[Lane[No_road][i] - first].setstate(5);   //该车状态设置为完成调度
					if (i != last_one[i%num] + num)              //当该车不在前车后面一个
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
					Car[Lane[No_road][i] - first].setstate(5);    //当该车可以行驶时，行驶，并设置为已调度完成
					Lane[No_road][i - v*num] = Lane[No_road][i];
					Lane[No_road][i] = -1;
					last_one[i%num] = i - v*num;
				}
			}
		}
		int lon = Road[No_road].getlongth();
		int num = Road[No_road].getnum_road();
		int len = lon*num;
		Road[No_road].setfirst_sch(1, len);    //初始化first_sch为len

		for (int i = num;i < 2 * num;i++)
		{
			Road[No_road].setlast_sch(i, last_one[i%num]);   //初始化每条车道的last_sch为每条车道最后一辆车的位置
			Road[No_road].setlast_finish(i, len - 1);   //初始化last_finish为len-num+i
		}
	}
	delete[]last_one;

}
int getCarId(int roadId, vector<road>&Road, vector<car>&Car, int **Lane, int isFrom, int first_car)   //该函数用来得到当前道路优先调用的等待车辆的ID，如果调用结束，则返回-1，配置相关属性
{
	int first_sch = Road[roadId].getfirst_sch(isFrom);
	int lon = Road[roadId].getlongth();
	int num = Road[roadId].getnum_road();
	int len = lon*num;
	while (first_sch != len + len*isFrom)
	{
		if (Lane[roadId][first_sch] == -1)
			first_sch++;
		else if (Car[Lane[roadId][first_sch] - first_car].getstate() == 5)
			first_sch++;
		else
		{
			Road[roadId].setfirst_sch(isFrom, first_sch);    //对first_sch进行更新
			return Lane[roadId][first_sch];
		}
	}
	Road[roadId].setis_finished(isFrom, 1);     //表示该道路已经调度完成，返回-1
	return -1;
}
int getCarId1(int roadId, vector<road>&Road, vector<car>&Car, int **Lane, int isFrom, int first_car)   //该函数用来得到当前道路优先调用的等待车辆的ID，如果调用结束，则返回-1，配置相关属性
{
	int first_sch = Road[roadId].getfirst_sch(isFrom);
	int lon = Road[roadId].getlongth();
	int num = Road[roadId].getnum_road();
	int len = lon*num;
	while (first_sch != len + len*isFrom)
	{
		if (Lane[roadId][first_sch] == -1)
			first_sch++;
		else if (Car[Lane[roadId][first_sch] - first_car].getstate() == 5)
			first_sch++;
		else
		{
			return Lane[roadId][first_sch];
		}
	}
	return -1;
}
int driveCar1(int carId, int No_Cross, int first_cross, int cur_road, int next_road, vector<road>&Road, vector<car>&Car, int **Lane)   //该函数用于处理要过路口的车辆，更新相关参数
{
	int first_car = Car[0].getId();
	int cur_num = Road[cur_road].getnum_road();
	int cur_lon = Road[cur_road].getlongth();
	int cur_len = cur_num*cur_lon;
	int next_num = Road[next_road].getnum_road();
	int next_lon = Road[next_road].getlongth();
	int next_len = next_num*next_lon;
	int isFrom = Road[cur_road].getisFrom();    //判断当前路口相对于cur_road是from还是to
	int isFromNext = No_Cross + first_cross == Road[next_road].getfromId() ? 0 : 1;  //这边判断isFrom得反一下，相对于驶入车辆其判断与是处车辆反一下
	int cur_len_first = isFrom*cur_len;  //用于在计算last_sch和last_finish时候用
	int cur_sch = Road[cur_road].getfirst_sch(isFrom);
	int cur_v = min(Road[cur_road].getmax_v(), Car[carId - first_car].getmax_v());      //得到当前道路车速
	int next_v = min(Road[next_road].getmax_v(), Car[carId - first_car].getmax_v());      //得到下一条道路的车速
	int cur_d = (cur_sch - cur_len_first) / cur_num;    //该车距离路口还有的距离
	int next_exp_d = next_v - cur_d > 0 ? next_v - cur_d : 0;
	//***********判断将该车汇入到下一个道路的哪个车道
	if (next_exp_d != 0)
	{

		for (int i = 0;i < next_num;i++)
		{
			int loc = Road[next_road].getlast_sch(next_num*isFromNext + i) / next_num - next_lon*isFromNext;    //相对值
			if (Road[next_road].getlast_sch(next_num*isFromNext + i) == -1)
				loc = -1;
			if (loc == next_lon - 1 && Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] != -1 && Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 4 && i == next_num - 1)     //如果下一车道的最后一个位置有车，但是等待，当前道路的车继续等待
				return 0;
			if (loc == next_lon - 1)    //表示该条车道已经满了
				continue;
			if (loc != next_lon - 1)   //表示该条道路没满
			{
				int remain = next_lon - 1 - loc;   //该未满道路剩余的可以放车的空间
				if (remain >= next_exp_d)  //表示下一条道路剩余的空间大于所要行驶的空间
				{
					int next_loc = next_num*next_lon*isFromNext + next_num*(next_lon - next_exp_d) + i;      //绝对位置，与Lane一致
					Lane[next_road][next_loc] = carId;    //将车驶过去
					Car[carId - first_car].setstate(5);    //将驶过去的车标志位已调度
					Road[next_road].setlast_sch(next_num*isFromNext + i, next_loc);      //更新last_sch
					if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //当前调度的车为该车道的最后一辆车，需要更新该车道的last_sch信息
					{
						Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first - 1);
					}
					Lane[cur_road][cur_sch] = -1;   //将该车位清空
					if (isFrom == 1)                   //调整道路上的车辆数
					{
						int num = Road[cur_road].getnum_car2();
						num--;
						Road[cur_road].setnum_car2(num);
					}
					else
					{
						int num = Road[cur_road].getnum_car1();
						num--;
						Road[cur_road].setnum_car1(num);
					}
					if (isFromNext == 1)
					{
						int num = Road[next_road].getnum_car2();
						num++;
						Road[next_road].setnum_car2(num);
					}
					else
					{
						int num = Road[next_road].getnum_car1();
						num++;
						Road[next_road].setnum_car1(num);
					}
					//cout << "drive car" << carId << endl;
					return 1;
				}
				else if (Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 5)   //剩余距离小于预期，但该车道的最后一辆车已调度完全，将需要调度的车放在该车后面
				{
					int next_loc1 = Road[next_road].getlast_sch(next_num*isFromNext + i);        //下一道路车道上原本最后一辆车的绝对位置
					Lane[next_road][next_loc1 + next_num] = carId;    //将车驶过去
					Car[carId - first_car].setstate(5);    //将驶过去的车标志位已调度
					Road[next_road].setlast_sch(next_num*isFromNext + i, next_loc1 + next_num);      //更新last_sch
					if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //当前调度的车为该车道的最后一辆车，需要更新该车道的last_sch信息
					{
						Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first - 1);
					}
					Lane[cur_road][cur_sch] = -1;   //将该车位清空
					if (isFrom == 1)                  //调整道路上的车辆数
					{
						int num = Road[cur_road].getnum_car2();
						num--;
						Road[cur_road].setnum_car2(num);
					}
					else
					{
						int num = Road[cur_road].getnum_car1();
						num--;
						Road[cur_road].setnum_car1(num);
					}
					if (isFromNext == 1)
					{
						int num = Road[next_road].getnum_car2();
						num++;
						Road[next_road].setnum_car2(num);
					}
					else
					{
						int num = Road[next_road].getnum_car1();
						num++;
						Road[next_road].setnum_car1(num);
					}
					return 1;
				}
				else if (Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 4)   //表示前面一辆车是等待状态，将要驶入的车继续等待，进入下一条道路的调度
				{
					return 0;
				}
			}

		}
		//当要驶入的道路满了，将车停在路口
	}
	//车辆驶到路口 包括两种情况：下一条道路的已满；next_exp_d==0
	Lane[cur_road][isFrom*cur_len + cur_sch%cur_num] = carId;  //将车的开过去
	if (isFrom*cur_len + cur_sch%cur_num != cur_sch)   //排除原本就在顶格的情况
		Lane[cur_road][cur_sch] = -1;     //将原来的位置清空
	Road[cur_road].setlast_finish(isFrom*cur_num + cur_sch%cur_num, isFrom*cur_len + cur_sch%cur_num);    //将车行驶的位置标志位该车道的
	if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //当前调度的车为该车道的最后一辆车，需要更新该车道的last_sch信息
	{
		Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first + cur_sch%cur_num);
	}
	Car[carId - first_car].setstate(5);  //将该车的状态设置为已调度
	return 1;
}
void driveCar2(int carId, int roadId, vector<road>&Road, vector<car>&Car, int **Lane)   //调度那些不需要驶过路口的车
{
	int first_car = Car[0].getId();
	int num_road = Road[roadId].getnum_road();
	int lon = Road[roadId].getlongth();
	int len = num_road*lon;
	int v = min(Car[carId - first_car].getmax_v(), Road[roadId].getmax_v());
	int isFrom = Road[roadId].getisFrom();
	int first_sch = Road[roadId].getfirst_sch(isFrom);
	int last_finish = Road[roadId].getlast_finish(isFrom*num_road + first_sch%num_road);
	int last_sch = Road[roadId].getlast_sch(isFrom*num_road + first_sch%num_road);
	if (first_sch - v*num_road > last_finish)   //当该车可以行驶最大距离
	{
		Lane[roadId][first_sch - v*num_road] = carId;
		Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, first_sch - v*num_road);
		if (first_sch == last_sch)         //如果当前调度的车为当前车道最后一辆，需要将last_sch前移
		{
			Road[roadId].setlast_sch(isFrom*num_road + first_sch%num_road, first_sch - v*num_road);
		}
	}
	else            //当该车只能行驶到last_finish的车后面
	{
		Lane[roadId][last_finish + num_road] = carId;
		if (last_finish + num_road != first_sch)  //排除原本就在last_finish后一格的情况
			Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, last_finish + num_road);
		if (first_sch == last_sch)         //如果当前调度的车为当前车道最后一辆，需要将last_sch前移
		{
			Road[roadId].setlast_sch(isFrom*num_road + first_sch%num_road, last_finish + num_road);
		}
	}

}
bool driveCar3(int carID, vector<car>&Car, int No_road, vector<road>&Road, int **Lane, int first_cross)  //对从车库出发的车进行调度，carID是绝对值，road是相对值
{
	int first_car = Car[0].getId();
	int fromId = Road[No_road].getfromId();
	int crossId = Car[carID - first_car].getfromId();
	int isFrom = crossId == fromId ? 0 : 1;   //这边判断反一下，因为该crossId是入口Id不是出口Id;
	int num_road = Road[No_road].getnum_road();
	int lon = Road[No_road].getlongth();
	int len = num_road*lon;
	int v = min(Car[carID - first_car].getmax_v(), Road[No_road].getmax_v());   //获取该车能够在该道路上行驶的速度
	for (int i = num_road*isFrom;i < num_road*(1 + isFrom);i++)    //按照调度算法决定驶入哪一条道路
	{
		int lastone = Road[No_road].getlast_sch(i);
		if (lastone == -1)
			lastone = -1;
		else
			lastone = lastone / num_road - lon*isFrom;
		if (lastone != lon - 1)    //如果该条车道有空位置
		{
			int remain = lon - 1 - lastone;
			if (remain >= v)   //剩余的空间大于等于v
			{
				int loc = lon*num_road*isFrom + num_road*(lon - v) + i - num_road*isFrom;
				Lane[No_road][loc] = carID;
				Road[No_road].setlast_sch(i, loc);
				if (isFrom == 1)
				{
					int num = Road[No_road].getnum_car2();
					num++;
					Road[No_road].setnum_car2(num);
					float con = (float)num / (float)len;
					Road[No_road].setcongestion2(con);
				}
				else
				{
					int num = Road[No_road].getnum_car1();
					num++;
					Road[No_road].setnum_car1(num);
					float con = (float)num / (float)len;
					Road[No_road].setcongestion1(con);
				}
				Car[carID - first_car].setstate(5);
				NUM_OF_DEPART++;
				NUM_ON_ROAD++;
				return true;
			}
			else    //剩余空间小于v
			{
				int loc = Road[No_road].getlast_sch(i) + num_road;
				Lane[No_road][loc] = carID;
				Road[No_road].setlast_sch(i, loc);
				if (isFrom == 1)
				{
					int num = Road[No_road].getnum_car2();
					num++;
					Road[No_road].setnum_car2(num);
					float con = (float)num / (float)len;
					Road[No_road].setcongestion2(con);
				}
				else
				{
					int num = Road[No_road].getnum_car1();
					num++;
					Road[No_road].setnum_car1(num);
					float con = (float)num / (float)len;
					Road[No_road].setcongestion1(con);
				}
				Car[carID - first_car].setstate(5);
				NUM_OF_DEPART++;
				NUM_ON_ROAD++;
				return true;
			}
		}
	}
	return false;
}
int getNextNo(int No)      //[4,1,2,3]方便过路口时道路之间的切换
{                         //[4,3,1,2]道路号按升序排列，方便对每条道路的遍历调度,在顺序调度时需要转换
	if (No == 3)return 1;
	if (No == 2)return 0;
	if (No == 0)return 3;
	if (No == 1)return 2;
}
int  driveAllCarForEachCross(int No_Cross, vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive)   //此处是对于每个路口的车辆进行调度，No_Cross是相对值。
{
	int first_cross = Cross[0].getId();//得到第一个路口的Id，方便绝对Id和相对Id的切换
	int first_car = Car[0].getId();  //得到第一个车的Id，方便绝对Id和相对Id的切换
	int first_road = Road[0].getId();//得到第一个道路的Id，方便绝对Id和相对Id的切换
									 //
	int road1 = Cross[No_Cross].getroad1();
	int road2 = Cross[No_Cross].getroad2();
	int road3 = Cross[No_Cross].getroad3();
	int road4 = Cross[No_Cross].getroad4();
	if (road1 != -1)
		road1 = road1 - first_road;
	if (road2 != -1)
		road2 = road2 - first_road;
	if (road3 != -1)
		road3 = road3 - first_road;
	if (road4 != -1)
		road4 = road4 - first_road;
	int arr[4];    //[4,1,2,3]方便过路口时道路之间的切换
				   //[4,3,1,2]道路号按升序排列，方便对每条道路的遍历调度,在顺序调度时需要转换
	arr[0] = road4;
	arr[1] = road1;
	arr[2] = road2;
	arr[3] = road3;
	int arr1[4] = { road4,road1,road2,road3 };
	for (int i = 0;i < 4;i++)     //不对那些只有从该路口进的道路进行处理
	{
		if (arr[i] != -1 && (Road[arr[i]].getisDouble() == 0) && (No_Cross + first_cross == Road[arr[i]].getfromId()))
			arr[i] = -1;
	}
	if (Cross[No_Cross].getfinish_road() == 0)     //此处主要是对每个时间片第一轮的finish_road的值进行改进，排除-1的边的影响
		for (int i = 0;i < 4;i++)
			if (arr[i] == -1)
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
			}
	for (int i = 0;i < 4;i++)    //判断该路口ID相对每条道路是FromID还是ToID
	{
		if (arr[i] != -1)
		{
			int is = ((No_Cross + first_cross) == (Road[arr[i]].getfromId())) ? 1 : 0;
			Road[arr[i]].setisFrom(is);
		}

	}
	int cur_road = (arr[0] != -1 && Road[arr[0]].getis_finished(Road[arr[0]].getisFrom()) == 0) ? arr[0] : ((arr[3] != -1 && Road[arr[3]].getis_finished(Road[arr[3]].getisFrom()) == 0) ? arr[3] : ((arr[1] != -1 && Road[arr[1]].getis_finished(Road[arr[1]].getisFrom()) == 0) ? arr[1] : arr[2]));   //得到最小的道路号作为执行道路号
	int No = cur_road == road4 ? 0 : (cur_road == road1 ? 1 : (cur_road == road2 ? 2 : 3));   //得到当前道路对应的编号（数组中的编号）

	int num_of_driven = 0;    //该变量用于记录每一次循环移动的车辆数
	int num_all_driven = 0;   //用于记录每个路口在该次循环中的移动的车辆数，便于检测死锁
	int num_of_change = 0;  //用于记录道路转换的情况，防止出现节点内调度死锁
	while (Cross[No_Cross].getfinish_road()<4)    //当当前路口还有道路没有调度完，否则跳出当前当前路口，进入下一个
	{
		if (num_of_change == 32)
		{
			No = (No + 1) % 4;
			cur_road = arr[No];
			if (cur_road != -1 && Road[cur_road].getis_finished(Road[cur_road].getisFrom()) == 1)
				return num_all_driven;
			num_of_change++;
			num_of_driven++;
			continue;
		}
		if (num_of_change == 40)
			return num_all_driven;  //当该道路内部出现循环时，跳出该节点，执行下一个节点
		if (num_of_driven == 4)
			break;                       //用来判断当四条道路没有可以移动的车辆时，退出当前路口的调度
		if (cur_road == -1 || Road[cur_road].getis_finished(Road[cur_road].getisFrom()) == 1)
		{
			No = getNextNo(No);    //按顺序调度下一条道路
			cur_road = arr[No];
			num_of_driven++;
			continue;
		}
		int num_road1 = Road[cur_road].getnum_road();
		int lon1 = Road[cur_road].getlongth();
		int len1 = num_road1*lon1;
		int isFrom1 = Road[cur_road].getisFrom();
		int carID = getCarId(cur_road, Road, Car, Lane, Road[cur_road].getisFrom(), first_car);      //获取当前待调度的车的carID
		int first_sch1 = Road[cur_road].getfirst_sch(isFrom1);
		if (Road[cur_road].getlast_finish(isFrom1*num_road1 + first_sch1%num_road1) == (len1*isFrom1 - 1))      //表示该道路上该车前面没有车挡着
		{
			if ((Road[cur_road].getis_finished(isFrom1) == 1) || (carID == -1))    //表示当前道路已经调度完成
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
				//cout << "路口" << No_Cross << "已完成道路" << cur_road << endl;
				if (num == 4)          //表示当前路口已经调度完
					break;
				No = getNextNo(No);    //按顺序调度下一条道路
				cur_road = arr[No];
				num_of_driven++;
				num_of_change++;
				continue;
			}
			if (carID != -1)
			{
				int v11 = min(Car[carID - first_car].getmax_v(), Road[cur_road].getmax_v());
				int remain11 = (Road[cur_road].getfirst_sch(isFrom1) - len1*isFrom1) / num_road1;
				if (remain11 >= v11)
				{
					driveCar2(carID, cur_road, Road, Car, Lane);
					num_all_driven++;
					continue;
				}
				else if (remain11<v11&&Car[carID - first_car].gettoId() - first_cross == No_Cross)    //当当前车辆会驶出路口但已经行驶到目的地
				{
					NUM_OF_FINISH++;
					NUM_ON_ROAD--;
					//	cout << "该车已经完成调度" << endl;
					Lane[cur_road][first_sch1] = -1;    //将当前位置清空
					if (Road[cur_road].getlast_sch(first_sch1%num_road1 + num_road1*isFrom1) == first_sch1)  //当前调度的车为该车道的最后一辆车，需要更新该车道的last_sch信息
					{
						Road[cur_road].setlast_sch(first_sch1%num_road1 + num_road1*isFrom1, len1*isFrom1 - 1);
					}
					num_all_driven++;
					continue;
				}
			}
			vector<int>::iterator pos;          //根据行驶路线，并结合车辆当前位置，在路口更新车辆行驶的operate
			for (pos = ThePathOfDrive[carID - first_car].begin();pos != (ThePathOfDrive[carID - first_car].end());pos++)
			{
				if ((*pos) == cur_road + first_road)    //搜索到当前道路
				{

					pos++;
					if (pos != (ThePathOfDrive[carID - first_car].end()))
					{
						//cout << "  pos++" << endl;
						int next_r = (*pos) - first_road;    //下一条要行使的道路的相对值
						if (next_r == arr1[(No + 2) % 4])   //表示直行
							Car[carID - first_car].setoperate(0);
						else if (next_r == arr1[(No + 1) % 4])   //表示左转
							Car[carID - first_car].setoperate(1);
						else if (next_r == arr1[(No + 3) % 4])   //表示左右转
							Car[carID - first_car].setoperate(2);
						break;
					}
					else
						pos--;

				}
			}
			//cout << "doing it " << endl;
			if ((Car[carID - first_car].getoperate()) == 0)         //表示当前的车为直行
			{
				int next_road = arr1[(No + 2) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //表示该车为继续等待，转入下一个道路
				{
					No = getNextNo(No);    //按顺序调度下一条道路
					cur_road = arr[No];
					num_of_driven++;
					num_of_change++;
					continue;
				}
				num_of_driven = 0;
				num_all_driven++;
				continue;
			}
			else if ((Car[carID - first_car].getoperate()) == 1)     //表示当前的车为左转
			{
				int next_road1 = arr[(No + 3) % 4];
				if (next_road1 != -1)    //考虑arr[(No + 3) % 4]这条道路是否为-1，如果不是-1进行判断
				{
					int isFrom_next = Road[next_road1].getisFrom();
					int carID1 = getCarId1(next_road1, Road, Car, Lane, isFrom_next, first_car);
					if (carID1 != -1 && Car[carID1 - first_car].getoperate() == 0)     //当左转遇到直行时，需要转到直行车道
					{
						No = (No + 3) % 4;
						cur_road = next_road1;
						num_of_driven = 0;
						num_of_change++;
						continue;
					}
				}
				//表示当前道路的车优先行驶
				int next_road = arr1[(No + 1) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //表示该车为继续等待，转入下一个道路
				{
					No = getNextNo(No);    //按顺序调度下一条道路
					cur_road = arr[No];
					num_of_driven++;      //记录为等待的车道数
					num_of_change++;
					continue;
				}

				num_of_driven = 0;
				num_all_driven++;
				continue;
			}
			else if ((Car[carID - first_car].getoperate()) == 2)     //表示当前的车为右转
			{
				int next_road1 = arr[(No + 1) % 4];      //考虑将要驶入道路的为直行的那条道路
				int next_road2 = arr[(No + 2) % 4];      //考虑将要驶入道路的为左转的那条道路
				if (next_road1 != -1)
				{
					int isFrom_next1 = Road[next_road1].getisFrom();
					int carID1 = getCarId1(next_road1, Road, Car, Lane, isFrom_next1, first_car);
					if (carID1 != -1 && Car[carID1 - first_car].getoperate() == 0)     //当右转遇到直行时，需要转到直行车道
					{
						No = (No + 1) % 4;
						cur_road = next_road1;
						num_of_driven = 0;
						num_of_change++;
						continue;
					}
				}
				if (next_road2 != -1)
				{
					int isFrom_next2 = Road[next_road2].getisFrom();
					int carID2 = getCarId1(next_road2, Road, Car, Lane, isFrom_next2, first_car);
					if (carID2 != -1 && Car[carID2 - first_car].getoperate() == 1)     //当右转遇到左转时，需要转到左转车道
					{
						No = (No + 2) % 4;
						cur_road = next_road2;
						num_of_driven = 0;
						num_of_change++;
						continue;
					}
				}
				//表示当前道路的车优先行
				int next_road = arr1[(No + 3) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //表示该车为继续等待，转入下一个道路
				{
					No = getNextNo(No);    //按顺序调度下一条道路
					cur_road = arr[No];
					num_of_driven++;
					num_of_change++;
					continue;
				}

				num_of_driven = 0;
				num_all_driven++;
				continue;
			}
		}
		else              //表示该道路待调度的车前面有已调度完成的车挡着
		{
			if ((Road[cur_road].getis_finished(isFrom1) == 1) || (carID == -1))    //表示当前道路已经调度完成
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
				if (num == 4)          //表示当前路口已经调度完
					break;
				No = getNextNo(No);    //按顺序调度下一条道路
				cur_road = arr[No];
				num_of_driven++;
				num_of_change++;
				continue;
			}
			driveCar2(carID, cur_road, Road, Car, Lane);    //调度车辆
			num_all_driven++;
		}
	}
	return num_all_driven;      //用于记录当前路口在该次循环中移动的车辆数，便于检测死锁
}
bool driveAllCarToTheEnd(vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive)  //该函数对调度第二步的每个路口的遍历,如果进入死锁则返回false否则返回true
{
	int No_cross_finish = 0;
	int num_driven_car = 0; //用于检测是否发生死锁
	int isd = Road[79].getisDouble();
	while (No_cross_finish < NUM_OF_CROSS)       //用于调度每个路口，直至路口调度完成
	{
		for (int No_cross = 0;No_cross < NUM_OF_CROSS;No_cross++)    //一次对每个路口进行遍历
		{
			if (Cross[No_cross].getfinish_road() == 4)          //表示该路口已经调度完成
			{
				No_cross_finish++;
				continue;
			}
			else                        //表示该路口没有调度完
			{
				int num = driveAllCarForEachCross(No_cross, Cross, Road, Lane, Car, ThePathOfDrive);
				if (Cross[No_cross].getfinish_road() == 4)
					No_cross_finish++;
				num_driven_car += num;
			}
			//	cout << "do the cross:" << No_cross << endl;

		}
		//cout << "完成道路数为" << No_cross_finish << "行驶车辆数" << num_driven_car << endl;
		if (No_cross_finish == NUM_OF_CROSS)     //表示所有路口已经完成调度
			return true;
		if (num_driven_car == 0)            //表示有路口未调度完，但没有车移动，表示已进入死锁
			return false;
		No_cross_finish = 0;
		num_driven_car = 0;
	}
}
bool dispatchCarPerTime(int cur_time, vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, list<int>& Ready_start, vector<vector<int>>&ThePathOfDrive, int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, int *nodeToPath)
{
	for (int No_road = 0;No_road < NUM_OF_ROAD;No_road++)     //调度第一步，对每一条道路进行进行调度
	{
		driveAllCarJustOnRoadToEndState1(No_road, Lane, Car, Road);
	}
	//cout << "完成第一步" << endl;
	bool isFinish = driveAllCarToTheEnd(Cross, Road, Lane, Car, ThePathOfDrive);     //调度第二步，看是否出现死锁
	if (isFinish == false)
		return false;
	//cout << "完成第二步" << endl;
	list<int>::iterator pos;
	int first_road = Road[0].getId();
	int first_car = Car[0].getId();
	int first_cross = Cross[0].getId();
	int num_depart = 0;
	for (pos = Ready_start.begin();pos != Ready_start.end();)
	{

		if (NUM_ON_ROAD > 1100)   //路上车大于700不发车
			break;
		//if (num_depart > 200)    //单个时间片发车量不超过400
		//	break;
		if (cur_time < 100 && cur_time >= 10 && cur_time % 4 != 0)
			break;
		if (cur_time < 10 && cur_time % 2 != 0)
			break;
		if (cur_time >= 100 && cur_time < 350 && cur_time % 6 != 0)
			break;
		if (cur_time >= 350 && cur_time < 500 && cur_time % 4 != 0)
			break;
		if (cur_time >= 500 && cur_time % 4 != 0)
			break;
		if (cur_time > 10 && cur_time < 450 && num_depart > 60)   //时间超过10，单个时间片发车量不超过100
			break;
		if (cur_time <= 10 && num_depart > 150)   //时间超过10，单个时间片发车量不超过100
			break;
		if (cur_time >450 && num_depart > 150)   //时间超过10，单个时间片发车量不超过100
			break;
		int No_road = ThePathOfDrive[*pos - first_car].front() - first_road;
		bool isDepart = driveCar3(*pos, Car, No_road, Road, Lane, first_cross);
		if (isDepart == true)
		{
			Car[*pos - first_car].setact_time(cur_time);
			num_depart++;
			pos = Ready_start.erase(pos);
		}
		else
			pos++;
	}
	for (int i = 0;i < NUM_OF_CAR;i++)
		Car[i].setstate(3);             //将每一辆车都标识为待调度
	for (int i = 0;i < NUM_OF_CROSS;i++)
		Cross[i].setfinish_road(0);     //将每一个路口的finish_road重置为0
	for (int i = 0;i < NUM_OF_ROAD;i++)   //将每条道路的is_finished设置为0
	{
		int isD = Road[i].getisDouble();
		for (int j = 0;j < (isD + 1);j++)
			Road[i].setis_finished(j, 0);
	}
	return true;
}
int dispatchCar(vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive, map<int, list<int>>&EachTimeOfPlan, int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, int *nodeToPath)  //一个时间片一个时间片进行相应的调度
{
	int cur_time = -1;
	list<int> Ready_start;
	while (NUM_OF_FINISH != NUM_OF_CAR)     //当还有车没有到达目的地，不断进行调度
	{
		cur_time++;
		cout << "当前时间片为：" << cur_time << endl;
		for (int i = 0;i < NUM_OF_ROAD;i++)
		{
			int num = Road[i].getnum_road();
			int lon = Road[i].getlongth();
			int isD = Road[i].getisDouble();
			int len = num*lon*(1 + isD);
		}
		if (EachTimeOfPlan.count(cur_time) == 1)   //将cur_time==plan_time的车加到发车队列中
		{
			Ready_start.merge(EachTimeOfPlan[cur_time]);
		}
		bool isOK = dispatchCarPerTime(cur_time, Cross, Road, Lane, Car, Ready_start, ThePathOfDrive, minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, nodeToPath);  //进行每个时间片的调度
		if (isOK == false)     //当出现死锁，返回，否则进行下一个时间片的调度
			return -1;
	}
	return cur_time;    //返回总的调度时间
}
int main()
{
	vector<cross> Cross;  //用来存储路口
	vector<road>Road;   //用来存储道路
	vector<car>Car;   //用来存储汽车
	map<int, list<int>>EachTimeOfPlan;  //每个时间片准备发车的车组成的链表
	vector<vector<int>>ThePathOfDrive;  //记录每辆车的行车路线
	string CrossTXT = "cross.txt";
	string RoadTXT = "road.txt";
	string CarTXT = "car.txt";
	ifstream infile;
	string line;
	infile.open(CrossTXT);
	if (infile) // 有该文件  
	{
		while (getline(infile, line)) // line中不包括每行的换行符  
		{
			makeCross(Cross, line);
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	infile.open(RoadTXT);
	if (infile) // 有该文件  
	{
		while (getline(infile, line)) // line中不包括每行的换行符  
		{
			makeRoad(Road, line);
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	infile.open(CarTXT);
	if (infile) // 有该文件  
	{
		while (getline(infile, line)) // line中不包括每行的换行符  
		{
			makeCar(Car, line, EachTimeOfPlan, ThePathOfDrive);
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	int **Lane = new int*[NUM_OF_ROAD];  //构建车道数组
	makeLane(Lane, NUM_OF_ROAD, Road);
	int size = NUM_OF_CROSS*NUM_OF_CROSS;
	int *minLen2Nodes1 = new int[size]; //存储连点之间的最短距离
	for (int i = 0;i < size; i++)
		if (i % (NUM_OF_CROSS + 1) == 0)
			minLen2Nodes1[i] = 0;
		else
			minLen2Nodes1[i] = 999999;
	int *minLen2Nodes2 = new int[size];  //存储两点之间的最短行驶时间
	for (int i = 0;i < size; i++)
		if (i % (NUM_OF_CROSS + 1) == 0)
			minLen2Nodes2[i] = 0;
		else
			minLen2Nodes2[i] = 999999;
	int *pathOf2Nodes1 = new int[size];
	for (int i = 0;i < size;i++)        //存取路径最短的路径
		pathOf2Nodes1[i] = -1;
	int *pathOf2Nodes2 = new int[size];  //存储时间最短的路径
	for (int i = 0;i < size;i++)
		pathOf2Nodes2[i] = -1;
	int *nodeToPath = new int[size];    //存储任意两个相连节点之间的道路
	for (int i = 0;i < size;i++)
		nodeToPath[i] = -1;
	makeNodeToPath(nodeToPath, Road, NUM_OF_CROSS, NUM_OF_ROAD, Cross[0].getId());
	makeMinLen2Nodes(minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, Cross[0].getId());   //构建当前道路环境下的任意两点之间的最短路径
	int first_cross = Cross[0].getId();
	int first_car = Car[0].getId();
	for (int carId = 0;carId < NUM_OF_CAR;carId++)       //为每一辆车规划静态路线  此处的carId为相对值
	{
		int from = Car[carId].getfromId() - first_cross;
		int to = Car[carId].gettoId() - first_cross;
		int v = Car[carId].getmax_v();
		{
			if (v > MIN_V_ROAD)                 //记录行车路线
			{
				int i = from;
				int j = to;
				while (pathOf2Nodes2[i*NUM_OF_CROSS + j] != to)
				{
					int k = pathOf2Nodes2[i*NUM_OF_CROSS + j];
					ThePathOfDrive[carId].push_back(nodeToPath[i*NUM_OF_CROSS + k]);
					i = k;
				}
				ThePathOfDrive[carId].push_back(nodeToPath[i*NUM_OF_CROSS + j]);
			}
			else
			{
				int i = from;
				int j = to;
				while (pathOf2Nodes1[i*NUM_OF_CROSS + j] != to)
				{
					int k = pathOf2Nodes1[i*NUM_OF_CROSS + j];
					ThePathOfDrive[carId].push_back(nodeToPath[i*NUM_OF_CROSS + k]);
					i = k;
				}
				ThePathOfDrive[carId].push_back(nodeToPath[i*NUM_OF_CROSS + j]);
			}
		}
	}
	int value = dispatchCar(Cross, Road, Lane, Car, ThePathOfDrive, EachTimeOfPlan, minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, nodeToPath);
	if (value < 0)
		cout << value << endl;
	else
		cout << "the time is:" << value << endl;
	if (value > 0)
	{
		ofstream outfile;
		outfile.open("answer.txt");
		outfile << "#(carId,StartTime,RoadId...)" << endl;
		for (int i = 0;i < NUM_OF_CAR;i++)
		{
			outfile << "(" << Car[i].getId() << "," << Car[i].getact_time();
			vector<int>::iterator pos;
			for (pos = ThePathOfDrive[i].begin();pos != ThePathOfDrive[i].end();pos++)
				outfile << "," << *pos;
			outfile << ")" << endl;
		}
		outfile.close();
	}

	/*

	else
	{
	ofstream outfile("answer.txt");        //将结果输出到answer.txt文件中

	outfile << "#(carId,StartTime,RoadId...)" << endl;
	for (int i = 0;i < NUM_OF_CAR;i++)
	{
	outfile<<"(" << Car[i].getId() << "," << Car[i].getact_time() << ",";
	vector<int>::iterator pos;
	for (pos = ThePathOfDrive[i].begin();pos != ThePathOfDrive[i].end();pos++)
	{
	outfile << (*pos) << ",";
	}
	outfile << ")" << endl;
	}


	}
	*/

	//makeMinLen2Nodes(minLen2Nodes1,minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, Cross[0].getId());  //求解初始时任意两点之间的最短距离和行驶时间
	//************************just for test
	/*
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

	*/
	//**************************
	//cout << NUM_OF_CROSS << " " << NUM_OF_ROAD << "  " << NUM_OF_CAR << "  " << MAX_PLAN_TIME << endl;
	delete[]Lane;
	delete[]minLen2Nodes1;
	delete[]minLen2Nodes2;
	delete[]pathOf2Nodes1;
	delete[]pathOf2Nodes2;
	delete[]nodeToPath;
	system("pause");
	return 0;
}