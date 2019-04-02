#include <iostream>
#include<string>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<list>
#include<map>
#include<functional>
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
	int *last_sch = nullptr; //记录每个车道最后一辆等待状态的车的位置
	int *last_finish = nullptr; //记录每条车道first_sch前已完成的最后一辆车的位置
	int *is_finished = nullptr; //记录每条道路在调度过程中是否已经调度完成。
	int isFrom; //该变量用于调度的第二步，用于判断该路口是该道路的FromID还是ToID，用于决定使用Lane矩阵中的哪一部分数据，0用前半段，1用后半段
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
	int driven_time;
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
	int getdriven_time() { return driven_time; }
	void setdriven_time(int t) { driven_time = t; }
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
	driven_time = 0;
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
		last_sch[i] = (i / num)*lon*num + i%num;
	last_finish = new int[num*(isD + 1)];
	for (int i = 0;i < num*(isD + 1);i++)
		last_sch[i] = -1;
	is_finished = new int[isD + 1];
	for (int i = 0;i < isD + 1;i++)
		is_finished[i] = 0;
	isFrom = 0;

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
void makeCross(map<int,cross,less<int>>&Cross, string A)  //将cross.txt导入到cross的Vector中
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
		Cross.insert(pair<int,cross>(Id,cross(Id, road1, road2, road3, road4)));
		NUM_OF_CROSS++;
	}
}
void makeRoad(map<int,road, less<int>>&Road, string A)  //将road.txt导入到road的Vector中，并根据单双道创建车道数组。
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
		Road.insert(pair<int,road>(Id,road(Id, longth, max_v, num_road, fromId, toId, isD)));
		NUM_OF_ROAD++;
	}
}
void makeCar(map<int,car,less<int>>&Car, string A)   //将car.txt导入到Car的Vector中。
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
		Car.insert(pair<int,car>(Id,car(Id, fromId, toId, max_v, plan_time)));
		if (plan_time > MAX_PLAN_TIME)             //此过程为记录最大的计划发车时间点
			MAX_PLAN_TIME = plan_time;
		NUM_OF_CAR++;
	}
}
void makeLane(map<int,int*>&Lane , const int num, map<int,road> &Road) //构建车道数组
{
	map<int, road>::iterator pos;
	for (pos = Road.begin();pos != Road.end();pos++)
	{
		int Id = pos->second.getId();
		int isD = pos->second.getisDouble();
		int num_road = pos->second.getnum_road();
		int longth = pos->second.getlongth();
		int num_lane = isD == 1 ? num_road * 2 : num_road;//车道数
		int long_int = num_lane*longth;//所有车道长度
		Lane[Id] = new int[long_int];  //创建车道数组
		for (int i = 0;i < long_int;i++)
			Lane[Id - Road[0].getId()][i] = -1;  //对车道上每一个位置进行初始化，初始化为-1
	}
}
void makeNodeToPath(int *nodeToPath, map<int,road,less<int>>& Road, int num_cross, int num_road, int first, map<int, int>&CrossToInt)   //该函数用于记录两个路口之间的道路号，用于后期将floyd算法得到的节点路径转化为道路路径
{
	map<int, road, less<int>>::iterator pos;
	for (pos = Road.begin();pos != Road.end();pos++)
	{
		int id = pos->second.getId();
		int isD = pos->second.getisDouble();
		int from = CrossToInt[pos->second.getfromId()];
		int to = CrossToInt[pos->second.gettoId()];
		nodeToPath[from*num_cross + to] = id;
		if (isD == 1)
			nodeToPath[to*num_cross + from] = id;
	}
}
void makeMinLen2Nodes(int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, map<int,road,less<int>>&Road, int num_road, int num_cross, map<int, int>&CrossToInt)  //求解初始时任意两点之间的最短距离和行驶时间
{
	map<int, road>::iterator pos;
	for (pos = Road.begin();pos != Road.end();pos++)
	{
		int from = CrossToInt[pos->second.getfromId()];
		int to = CrossToInt[pos->second.gettoId()];
		int isD = pos->second.getisDouble();
		int len = pos->second.getlongth();
		int v = pos->second.getact_v();
		float con = pos->second.getcongestion1();
		int t = len%v == 0 ? len / v : len / v + 1;
		minLen2Nodes1[from*num_cross + to] = len + (con>0.6 ? (int)con*len : 0);   //这边计算两个两点之间最短参数，其目的是一个用于车速高于道路最小值的使用，一个是用于车速小于道路限速最小值的使用
		minLen2Nodes2[from*num_cross + to] = t + (con>0.6 ? (int)con*t : 0);
		pathOf2Nodes1[from*num_cross + to] = to;
		pathOf2Nodes2[from*num_cross + to] = to;
		if (isD == 1)
		{
			float con1 = pos->second.getcongestion2();
			minLen2Nodes1[to*num_cross + from] = len + (con1>0.6 ? (int)con*len : 0);
			minLen2Nodes2[to*num_cross + from] = t + (con1>0.6 ? (int)con*t : 0);
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
bool compare(car car1, car car2)
{
	return (car1.getdriven_time()) > (car2.getdriven_time());
}
int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	
	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file
	map<int,cross,less<int>> Cross;  //用来存储路口
	map<int,road,less<int>>Road;   //用来存储道路
	map<int,car,less<int>>Car;   //用来存储汽车	queue<queue<int>>ThePathOfDrive;  //记录每辆车的行车路线，第一位记录的是其真实发车时间
					  //vector<cross>::iterator pos;
	string CrossTXT = crossPath;
	string RoadTXT = roadPath;
	string CarTXT = carPath;
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
			makeCar(Car, line);
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
	infile.close();
	//for (int i = 0;i < NUM_OF_CAR;i++)
	//{
	//	Car[i].printCar();
	//}
	map<int, int*>Lane;
	map<int, road,less<int>>::iterator pos;
	map<int, road, less<int>>::iterator pos_road;
	map<int, cross, less<int>>::iterator pos_cross;
	map<int, int>RoadToInt;
	map<int, int>CrossToInt;
	int value = 0;
	for (pos = Road.begin();pos != Road.end();pos++)
	{
		int Id = pos->second.getId();
		int isD = pos->second.getisDouble();
		int num_road = pos->second.getnum_road();
		int longth = pos->second.getlongth();
		int num_lane = isD == 1 ? num_road * 2 : num_road;//车道数
		int long_int = num_lane*longth;//所有车道长度
		RoadToInt.insert(pair<int, int>(Id, value++));
		Lane[Id] = new int[long_int];  //创建车道数组
		for (int i = 0;i < long_int;i++)
			Lane[Id][i] = -1;  //对车道上每一个位置进行初始化，初始化为-1
	}	
	int cross_value = 0;
	for (pos_cross = Cross.begin();pos_cross != Cross.end();pos_cross++)
	{
		int Id = pos_cross->second.getId();
		CrossToInt.insert(pair<int, int>(Id, cross_value++));
	}
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
	makeNodeToPath(nodeToPath, Road, NUM_OF_CROSS, NUM_OF_ROAD, Cross[0].getId(), CrossToInt);
	makeMinLen2Nodes(minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, CrossToInt);  //求解初始时任意两点之间的最短距离和行驶时间
																																	  //************************just for test
																																	  //infile.open("answer.txt");
																																	  //if (infile)
																																	  //{
																																	  //	infile.close();
																																	  //	DeleteFile("answer.txt");
																																	  //}
	vector<car> car_time;
	map<int, car, less<int>>::iterator pos_car;
	for (pos_car = Car.begin();pos_car != Car.end();pos++)
	{
		car_time.push_back(pos_car->second);
		int fromId = pos_car->second.getfromId();
		int toId = pos_car->second.gettoId();
		int from = CrossToInt[fromId];
		int to = CrossToInt[toId];
		int v = pos_car->second.getmax_v();
		int time;
		if (v > MIN_V_ROAD)
			time = minLen2Nodes2[from*NUM_OF_CROSS + to];
		else
			time = (minLen2Nodes1[from*NUM_OF_CROSS + to] / v);
		pos_car->second.setdriven_time(time);
	}
	sort(car_time.begin(), car_time.end(), compare);   //依据理想行车时间由大到小对车牌号进行排序，便于后面的调度
	int at_onetime = 300;   //此处是记录一次最大调度车辆数
	int all_time = 0; //这是对行车时间片的累加
	int pre = 0;
	int num = 0;
	for (pos_car = Car.begin();pos_car != Car.end();pos_car++)
	{
		int carId = pos_car->second.getId();
		if (num%at_onetime == 0)
			pre = Car[carId].getdriven_time();
		if ((Car[carId].getplan_time())>all_time)
			Car[carId].setact_time((Car[carId].getplan_time()));
		else
			Car[carId].setact_time(all_time);
		if (num%at_onetime == at_onetime - 1)
			all_time += pre;
		num++;
		//cout << all_time << endl;
	}
	ofstream outfile(answerPath);

	outfile << "#(carId,StartTime,RoadId...)" << endl;
	for (pos_car = Car.begin();pos_car != Car.end();pos_car++)
	{
		int Id = pos_car->second.getId();
		//int plan_time = Car[i].getplan_time();
		int act_time = pos_car->second.getact_time();
		int fromId = pos_car->second.getfromId();
		int toId = pos_car->second.gettoId();
		int from = CrossToInt[fromId];
		int to = CrossToInt[toId];
		int v = pos_car->second.getmax_v();
		//plan_time += pre;
		outfile << "(" << Id << "," << act_time << ",";
		//if (v > MIN_V_ROAD)
		//	outfile << minLen2Nodes2[from*NUM_OF_CROSS + to] << ")" << endl;
		//else
		//  outfile << (minLen2Nodes1[from*NUM_OF_CROSS + to]/v) << ")" << endl;
		//if (v > MIN_V_ROAD)
		//	pre = minLen2Nodes2[from*NUM_OF_CROSS + to];
		//else
		//	pre = (minLen2Nodes1[from*NUM_OF_CROSS + to] / v);

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
				i = k;
			}
			outfile << nodeToPath[i*NUM_OF_CROSS + j] << ")" << endl;
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
				i = k;
			}
			outfile << nodeToPath[i*NUM_OF_CROSS + j] << ")" << endl;
			//outfile << toId<<")" << endl;

		}

	}

	//**************************
	//cout << NUM_OF_CROSS << " " << NUM_OF_ROAD << "  " << NUM_OF_CAR << "  " << MAX_PLAN_TIME << endl;
	//delete[]Lane;
	delete[]minLen2Nodes1;
	delete[]minLen2Nodes2;
	delete[]pathOf2Nodes1;
	delete[]pathOf2Nodes2;
	delete[]nodeToPath;
	return 0;
}