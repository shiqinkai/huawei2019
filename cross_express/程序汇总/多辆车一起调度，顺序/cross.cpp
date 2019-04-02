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
	int finish_road;   //ÿ��·����ɵ��ȵĵ�·�������ڵ��ȵڶ���
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
	int num_car1;      //�������еĳ�����
	int num_car2;
	float congestion1; //����ӵ���̶�
	float congestion2;
	int *first_sch = nullptr;  //��¼��ǰ��·�ȴ�״̬�ĵ�һ��
	int *last_sch = nullptr; //��¼ÿ���������һ���ȴ�״̬�ĳ���λ��
	int *last_finish = nullptr; //��¼ÿ������first_schǰ����ɵ����һ������λ��
	int *is_finished = nullptr; //��¼ÿ����·�ڵ��ȹ������Ƿ��Ѿ�������ɡ�
	int isFrom; //�ñ������ڵ��ȵĵڶ����������жϸ�·���Ǹõ�·��FromID����ToID�����ھ���ʹ��Lane�����е���һ�������ݣ�0��ǰ��Σ�1�ú���
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
	int state;   //��¼����ǰ��״̬��1δ������2��������3δ���ȣ�4�ȴ���5�ѵ��ȣ�6�ѽ���  
	int operate;  //��¼�Գ��Ĳ�����1ֱ�У�2��ת��3��ת
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
void makeCross(map<int,cross,less<int>>&Cross, string A)  //��cross.txt���뵽cross��Vector��
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
void makeRoad(map<int,road, less<int>>&Road, string A)  //��road.txt���뵽road��Vector�У������ݵ�˫�������������顣
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
		Road.insert(pair<int,road>(Id,road(Id, longth, max_v, num_road, fromId, toId, isD)));
		NUM_OF_ROAD++;
	}
}
void makeCar(map<int,car,less<int>>&Car, string A)   //��car.txt���뵽Car��Vector�С�
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
		if (plan_time > MAX_PLAN_TIME)             //�˹���Ϊ��¼���ļƻ�����ʱ���
			MAX_PLAN_TIME = plan_time;
		NUM_OF_CAR++;
	}
}
void makeLane(map<int,int*>&Lane , const int num, map<int,road> &Road) //������������
{
	map<int, road>::iterator pos;
	for (pos = Road.begin();pos != Road.end();pos++)
	{
		int Id = pos->second.getId();
		int isD = pos->second.getisDouble();
		int num_road = pos->second.getnum_road();
		int longth = pos->second.getlongth();
		int num_lane = isD == 1 ? num_road * 2 : num_road;//������
		int long_int = num_lane*longth;//���г�������
		Lane[Id] = new int[long_int];  //������������
		for (int i = 0;i < long_int;i++)
			Lane[Id - Road[0].getId()][i] = -1;  //�Գ�����ÿһ��λ�ý��г�ʼ������ʼ��Ϊ-1
	}
}
void makeNodeToPath(int *nodeToPath, map<int,road,less<int>>& Road, int num_cross, int num_road, int first, map<int, int>&CrossToInt)   //�ú������ڼ�¼����·��֮��ĵ�·�ţ����ں��ڽ�floyd�㷨�õ��Ľڵ�·��ת��Ϊ��··��
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
void makeMinLen2Nodes(int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, map<int,road,less<int>>&Road, int num_road, int num_cross, map<int, int>&CrossToInt)  //����ʼʱ��������֮�����̾������ʻʱ��
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
		minLen2Nodes1[from*num_cross + to] = len + (con>0.6 ? (int)con*len : 0);   //��߼�����������֮����̲�������Ŀ����һ�����ڳ��ٸ��ڵ�·��Сֵ��ʹ�ã�һ�������ڳ���С�ڵ�·������Сֵ��ʹ��
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
	map<int,cross,less<int>> Cross;  //�����洢·��
	map<int,road,less<int>>Road;   //�����洢��·
	map<int,car,less<int>>Car;   //�����洢����	queue<queue<int>>ThePathOfDrive;  //��¼ÿ�������г�·�ߣ���һλ��¼��������ʵ����ʱ��
					  //vector<cross>::iterator pos;
	string CrossTXT = crossPath;
	string RoadTXT = roadPath;
	string CarTXT = carPath;
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
			makeRoad(Road, line);
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
			makeCar(Car, line);
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
		int num_lane = isD == 1 ? num_road * 2 : num_road;//������
		int long_int = num_lane*longth;//���г�������
		RoadToInt.insert(pair<int, int>(Id, value++));
		Lane[Id] = new int[long_int];  //������������
		for (int i = 0;i < long_int;i++)
			Lane[Id][i] = -1;  //�Գ�����ÿһ��λ�ý��г�ʼ������ʼ��Ϊ-1
	}	
	int cross_value = 0;
	for (pos_cross = Cross.begin();pos_cross != Cross.end();pos_cross++)
	{
		int Id = pos_cross->second.getId();
		CrossToInt.insert(pair<int, int>(Id, cross_value++));
	}
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
	int *nodeToPath = new int[size];    //�洢�������������ڵ�֮��ĵ�·
	for (int i = 0;i < size;i++)
		nodeToPath[i] = -1;
	makeNodeToPath(nodeToPath, Road, NUM_OF_CROSS, NUM_OF_ROAD, Cross[0].getId(), CrossToInt);
	makeMinLen2Nodes(minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, CrossToInt);  //����ʼʱ��������֮�����̾������ʻʱ��
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
	sort(car_time.begin(), car_time.end(), compare);   //���������г�ʱ���ɴ�С�Գ��ƺŽ������򣬱��ں���ĵ���
	int at_onetime = 300;   //�˴��Ǽ�¼һ�������ȳ�����
	int all_time = 0; //���Ƕ��г�ʱ��Ƭ���ۼ�
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