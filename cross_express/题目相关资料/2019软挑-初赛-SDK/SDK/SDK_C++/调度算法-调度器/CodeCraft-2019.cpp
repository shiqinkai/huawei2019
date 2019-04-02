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
int NUM_OF_FINISH = 0;    //��¼����Ŀ�ĵصĳ�����
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
	int *last_sch = nullptr; //��¼ÿ���������һ���ȴ�״̬�ĳ���λ�ã�Lane�е�λ��
	int *last_finish = nullptr; //��¼ÿ������first_schǰ����ɵ����һ������λ�ã�Lane�е�λ��
	int *is_finished = nullptr; //��¼ÿ����·�ڵ��ȹ������Ƿ��Ѿ�������ɡ�
	int isFrom; //�ñ������ڵ��ȵĵڶ����������жϸ�·���Ǹõ�·��FromID����ToID�����ھ���ʹ��Lane�����е���һ�������ݣ�0��ǰ��Σ�1�ú���
	int *isFull = nullptr;   //���ڵ��ȵڶ��������ڴӳ��ⷢ���ĳ���˵��滮·����Ҫ�˲���
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
	int state;   //��¼����ǰ��״̬��1δ������2��������3δ���ȣ�4�ȴ���5�ѵ��ȣ�6�ѽ���  
	int operate;  //��¼�Գ��Ĳ�����1ֱ�У�2��ת��3��ת
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
		Road.push_back(road(Id, longth, max_v, num_road, fromId, toId, isD));
		NUM_OF_ROAD++;
	}
}
void makeCar(vector<car>&Car, string A, map<int, list<int>>&map, vector<vector<int>>&ThePathOfDrive)   //��car.txt���뵽Car��Vector�С�
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
		vector<int>vec;
		ThePathOfDrive.push_back(vec);   //Ϊÿһ��������һ��queue�������洢�����ļ�ʻ·��
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
void makeLane(int **Lane, const int num, vector<road> &Road) //������������
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
void makeNodeToPath(int *nodeToPath, vector<road>& Road, int num_cross, int num_road, int first)   //�ú������ڼ�¼����·��֮��ĵ�·�ţ����ں��ڽ�floyd�㷨�õ��Ľڵ�·��ת��Ϊ��··��
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
void makeMinLen2Nodes(int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, vector<road>&Road, int num_road, int num_cross, int first)  //����ʼʱ��������֮�����̾������ʻʱ��
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
		minLen2Nodes1[from*num_cross + to] = len + (con>0.2 ? (int)con*len : 0) + isfull1 * 2000;   //��߼�����������֮����̲�������Ŀ����һ�����ڳ��ٸ��ڵ�·��Сֵ��ʹ�ã�һ�������ڳ���С�ڵ�·������Сֵ��ʹ��
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
			if ((i - v*num < 0 && last_one[i%num]<0) || (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 4)) //˵���ó���·�ڻ���ǰ���еȴ��ĳ�����
			{
				Car[Lane[No_road][i] - first].setstate(4);   //�ó�״̬����Ϊ�ȴ�
				last_one[i%num] = i;
			}
			else if ((last_one[i%num] >= 0 && last_one[i%num] >= (i - v*num)) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //�ó���ǰ����ǰ��ĳ��ѵ����꣬���ڸó������ʹ������
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
			else   //�ó�������ʻ����v
			{
				Car[Lane[No_road][i] - first].setstate(5);    //���ó�������ʻʱ����ʻ��������Ϊ�ѵ������
				Lane[No_road][i - v*num] = Lane[No_road][i];
				Lane[No_road][i] = -1;
				last_one[i%num] = i - v*num;
			}
		}

	}
	Road[No_road].setfirst_sch(0, 0);  //��ʼ��first_schΪ0
	for (int i = 0;i < num;i++)
	{
		Road[No_road].setlast_sch(i, last_one[i]);  //��ʼ��last_sch��ֵΪ��ǰ���������һ�������ڵ�λ��
		Road[No_road].setlast_finish(i, -1);   //��ʼ��last_finishΪ-num+i
	}
	if (isD == 1)           //����˫����ʱ����Ҫ����һ���򳵵����д���
	{
		for (int i = 0;i < num;i++)
			last_one[i] = len - 1;
		for (int i = len;i < 2 * len;i++)
		{
			if (Lane[No_road][i] != -1)
			{
				int v = min(Car[Lane[No_road][i] - first].getmax_v(), Road[No_road].getmax_v());
				if ((i - v*num < len&& last_one[i%num]<len) || (last_one[i%num] >= (i - v*num) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 4)) //˵���ó���·�ڻ���ǰ���еȴ��ĳ�����
				{
					Car[Lane[No_road][i] - first].setstate(4);   //�ó�״̬����Ϊ�ȴ�
					last_one[i%num] = i;
				}
				else if ((last_one[i%num] >= len && last_one[i%num] >= (i - v*num)) && Car[Lane[No_road][last_one[i%num]] - first].getstate() == 5)  //�ó���ǰ����ǰ��ĳ��ѵ����꣬���ڸó������ʹ������
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
		}
		int lon = Road[No_road].getlongth();
		int num = Road[No_road].getnum_road();
		int len = lon*num;
		Road[No_road].setfirst_sch(1, len);    //��ʼ��first_schΪlen

		for (int i = num;i < 2 * num;i++)
		{
			Road[No_road].setlast_sch(i, last_one[i%num]);   //��ʼ��ÿ��������last_schΪÿ���������һ������λ��
			Road[No_road].setlast_finish(i, len - 1);   //��ʼ��last_finishΪlen-num+i
		}
	}
	delete[]last_one;

}
int getCarId(int roadId, vector<road>&Road, vector<car>&Car, int **Lane, int isFrom, int first_car)   //�ú��������õ���ǰ��·���ȵ��õĵȴ�������ID��������ý������򷵻�-1�������������
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
			Road[roadId].setfirst_sch(isFrom, first_sch);    //��first_sch���и���
			return Lane[roadId][first_sch];
		}
	}
	Road[roadId].setis_finished(isFrom, 1);     //��ʾ�õ�·�Ѿ�������ɣ�����-1
	return -1;
}
int getCarId1(int roadId, vector<road>&Road, vector<car>&Car, int **Lane, int isFrom, int first_car)   //�ú��������õ���ǰ��·���ȵ��õĵȴ�������ID��������ý������򷵻�-1�������������
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
int driveCar1(int carId, int No_Cross, int first_cross, int cur_road, int next_road, vector<road>&Road, vector<car>&Car, int **Lane)   //�ú������ڴ���Ҫ��·�ڵĳ�����������ز���
{
	int first_car = Car[0].getId();
	int cur_num = Road[cur_road].getnum_road();
	int cur_lon = Road[cur_road].getlongth();
	int cur_len = cur_num*cur_lon;
	int next_num = Road[next_road].getnum_road();
	int next_lon = Road[next_road].getlongth();
	int next_len = next_num*next_lon;
	int isFrom = Road[cur_road].getisFrom();    //�жϵ�ǰ·�������cur_road��from����to
	int isFromNext = No_Cross + first_cross == Road[next_road].getfromId() ? 0 : 1;  //����ж�isFrom�÷�һ�£������ʻ�복�����ж����Ǵ�������һ��
	int cur_len_first = isFrom*cur_len;  //�����ڼ���last_sch��last_finishʱ����
	int cur_sch = Road[cur_road].getfirst_sch(isFrom);
	int cur_v = min(Road[cur_road].getmax_v(), Car[carId - first_car].getmax_v());      //�õ���ǰ��·����
	int next_v = min(Road[next_road].getmax_v(), Car[carId - first_car].getmax_v());      //�õ���һ����·�ĳ���
	int cur_d = (cur_sch - cur_len_first) / cur_num;    //�ó�����·�ڻ��еľ���
	int next_exp_d = next_v - cur_d > 0 ? next_v - cur_d : 0;
	//***********�жϽ��ó����뵽��һ����·���ĸ�����
	if (next_exp_d != 0)
	{

		for (int i = 0;i < next_num;i++)
		{
			int loc = Road[next_road].getlast_sch(next_num*isFromNext + i) / next_num - next_lon*isFromNext;    //���ֵ
			if (Road[next_road].getlast_sch(next_num*isFromNext + i) == -1)
				loc = -1;
			if (loc == next_lon - 1 && Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] != -1 && Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 4 && i == next_num - 1)     //�����һ���������һ��λ���г������ǵȴ�����ǰ��·�ĳ������ȴ�
				return 0;
			if (loc == next_lon - 1)    //��ʾ���������Ѿ�����
				continue;
			if (loc != next_lon - 1)   //��ʾ������·û��
			{
				int remain = next_lon - 1 - loc;   //��δ����·ʣ��Ŀ��Էų��Ŀռ�
				if (remain >= next_exp_d)  //��ʾ��һ����·ʣ��Ŀռ������Ҫ��ʻ�Ŀռ�
				{
					int next_loc = next_num*next_lon*isFromNext + next_num*(next_lon - next_exp_d) + i;      //����λ�ã���Laneһ��
					Lane[next_road][next_loc] = carId;    //����ʻ��ȥ
					Car[carId - first_car].setstate(5);    //��ʻ��ȥ�ĳ���־λ�ѵ���
					Road[next_road].setlast_sch(next_num*isFromNext + i, next_loc);      //����last_sch
					if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //��ǰ���ȵĳ�Ϊ�ó��������һ��������Ҫ���¸ó�����last_sch��Ϣ
					{
						Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first - 1);
					}
					Lane[cur_road][cur_sch] = -1;   //���ó�λ���
					if (isFrom == 1)                   //������·�ϵĳ�����
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
				else if (Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 5)   //ʣ�����С��Ԥ�ڣ����ó��������һ�����ѵ�����ȫ������Ҫ���ȵĳ����ڸó�����
				{
					int next_loc1 = Road[next_road].getlast_sch(next_num*isFromNext + i);        //��һ��·������ԭ�����һ�����ľ���λ��
					Lane[next_road][next_loc1 + next_num] = carId;    //����ʻ��ȥ
					Car[carId - first_car].setstate(5);    //��ʻ��ȥ�ĳ���־λ�ѵ���
					Road[next_road].setlast_sch(next_num*isFromNext + i, next_loc1 + next_num);      //����last_sch
					if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //��ǰ���ȵĳ�Ϊ�ó��������һ��������Ҫ���¸ó�����last_sch��Ϣ
					{
						Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first - 1);
					}
					Lane[cur_road][cur_sch] = -1;   //���ó�λ���
					if (isFrom == 1)                  //������·�ϵĳ�����
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
				else if (Car[Lane[next_road][Road[next_road].getlast_sch(next_num*isFromNext + i)] - first_car].getstate() == 4)   //��ʾǰ��һ�����ǵȴ�״̬����Ҫʻ��ĳ������ȴ���������һ����·�ĵ���
				{
					return 0;
				}
			}

		}
		//��Ҫʻ��ĵ�·���ˣ�����ͣ��·��
	}
	//����ʻ��·�� ���������������һ����·��������next_exp_d==0
	Lane[cur_road][isFrom*cur_len + cur_sch%cur_num] = carId;  //�����Ŀ���ȥ
	if (isFrom*cur_len + cur_sch%cur_num != cur_sch)   //�ų�ԭ�����ڶ�������
		Lane[cur_road][cur_sch] = -1;     //��ԭ����λ�����
	Road[cur_road].setlast_finish(isFrom*cur_num + cur_sch%cur_num, isFrom*cur_len + cur_sch%cur_num);    //������ʻ��λ�ñ�־λ�ó�����
	if (Road[cur_road].getlast_sch(cur_sch%cur_num + cur_num*isFrom) == cur_sch)  //��ǰ���ȵĳ�Ϊ�ó��������һ��������Ҫ���¸ó�����last_sch��Ϣ
	{
		Road[cur_road].setlast_sch(cur_sch%cur_num + cur_num*isFrom, cur_len_first + cur_sch%cur_num);
	}
	Car[carId - first_car].setstate(5);  //���ó���״̬����Ϊ�ѵ���
	return 1;
}
void driveCar2(int carId, int roadId, vector<road>&Road, vector<car>&Car, int **Lane)   //������Щ����Ҫʻ��·�ڵĳ�
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
	if (first_sch - v*num_road > last_finish)   //���ó�������ʻ������
	{
		Lane[roadId][first_sch - v*num_road] = carId;
		Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, first_sch - v*num_road);
		if (first_sch == last_sch)         //�����ǰ���ȵĳ�Ϊ��ǰ�������һ������Ҫ��last_schǰ��
		{
			Road[roadId].setlast_sch(isFrom*num_road + first_sch%num_road, first_sch - v*num_road);
		}
	}
	else            //���ó�ֻ����ʻ��last_finish�ĳ�����
	{
		Lane[roadId][last_finish + num_road] = carId;
		if (last_finish + num_road != first_sch)  //�ų�ԭ������last_finish��һ������
			Lane[roadId][first_sch] = -1;
		Car[carId - first_car].setstate(5);
		Road[roadId].setlast_finish(isFrom*num_road + first_sch%num_road, last_finish + num_road);
		if (first_sch == last_sch)         //�����ǰ���ȵĳ�Ϊ��ǰ�������һ������Ҫ��last_schǰ��
		{
			Road[roadId].setlast_sch(isFrom*num_road + first_sch%num_road, last_finish + num_road);
		}
	}

}
bool driveCar3(int carID, vector<car>&Car, int No_road, vector<road>&Road, int **Lane, int first_cross)  //�Դӳ�������ĳ����е��ȣ�carID�Ǿ���ֵ��road�����ֵ
{
	int first_car = Car[0].getId();
	int fromId = Road[No_road].getfromId();
	int crossId = Car[carID - first_car].getfromId();
	int isFrom = crossId == fromId ? 0 : 1;   //����жϷ�һ�£���Ϊ��crossId�����Id���ǳ���Id;
	int num_road = Road[No_road].getnum_road();
	int lon = Road[No_road].getlongth();
	int len = num_road*lon;
	int v = min(Car[carID - first_car].getmax_v(), Road[No_road].getmax_v());   //��ȡ�ó��ܹ��ڸõ�·����ʻ���ٶ�
	for (int i = num_road*isFrom;i < num_road*(1 + isFrom);i++)    //���յ����㷨����ʻ����һ����·
	{
		int lastone = Road[No_road].getlast_sch(i);
		if (lastone == -1)
			lastone = -1;
		else
			lastone = lastone / num_road - lon*isFrom;
		if (lastone != lon - 1)    //������������п�λ��
		{
			int remain = lon - 1 - lastone;
			if (remain >= v)   //ʣ��Ŀռ���ڵ���v
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
			else    //ʣ��ռ�С��v
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
int getNextNo(int No)      //[4,1,2,3]�����·��ʱ��·֮����л�
{                         //[4,3,1,2]��·�Ű��������У������ÿ����·�ı�������,��˳�����ʱ��Ҫת��
	if (No == 3)return 1;
	if (No == 2)return 0;
	if (No == 0)return 3;
	if (No == 1)return 2;
}
int  driveAllCarForEachCross(int No_Cross, vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive)   //�˴��Ƕ���ÿ��·�ڵĳ������е��ȣ�No_Cross�����ֵ��
{
	int first_cross = Cross[0].getId();//�õ���һ��·�ڵ�Id���������Id�����Id���л�
	int first_car = Car[0].getId();  //�õ���һ������Id���������Id�����Id���л�
	int first_road = Road[0].getId();//�õ���һ����·��Id���������Id�����Id���л�
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
	int arr[4];    //[4,1,2,3]�����·��ʱ��·֮����л�
				   //[4,3,1,2]��·�Ű��������У������ÿ����·�ı�������,��˳�����ʱ��Ҫת��
	arr[0] = road4;
	arr[1] = road1;
	arr[2] = road2;
	arr[3] = road3;
	int arr1[4] = { road4,road1,road2,road3 };
	for (int i = 0;i < 4;i++)     //������Щֻ�дӸ�·�ڽ��ĵ�·���д���
	{
		if (arr[i] != -1 && (Road[arr[i]].getisDouble() == 0) && (No_Cross + first_cross == Road[arr[i]].getfromId()))
			arr[i] = -1;
	}
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
		if (arr[i] != -1)
		{
			int is = ((No_Cross + first_cross) == (Road[arr[i]].getfromId())) ? 1 : 0;
			Road[arr[i]].setisFrom(is);
		}

	}
	int cur_road = (arr[0] != -1 && Road[arr[0]].getis_finished(Road[arr[0]].getisFrom()) == 0) ? arr[0] : ((arr[3] != -1 && Road[arr[3]].getis_finished(Road[arr[3]].getisFrom()) == 0) ? arr[3] : ((arr[1] != -1 && Road[arr[1]].getis_finished(Road[arr[1]].getisFrom()) == 0) ? arr[1] : arr[2]));   //�õ���С�ĵ�·����Ϊִ�е�·��
	int No = cur_road == road4 ? 0 : (cur_road == road1 ? 1 : (cur_road == road2 ? 2 : 3));   //�õ���ǰ��·��Ӧ�ı�ţ������еı�ţ�

	int num_of_driven = 0;    //�ñ������ڼ�¼ÿһ��ѭ���ƶ��ĳ�����
	int num_all_driven = 0;   //���ڼ�¼ÿ��·���ڸô�ѭ���е��ƶ��ĳ����������ڼ������
	int num_of_change = 0;  //���ڼ�¼��·ת�����������ֹ���ֽڵ��ڵ�������
	while (Cross[No_Cross].getfinish_road()<4)    //����ǰ·�ڻ��е�·û�е����꣬����������ǰ��ǰ·�ڣ�������һ��
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
			return num_all_driven;  //���õ�·�ڲ�����ѭ��ʱ�������ýڵ㣬ִ����һ���ڵ�
		if (num_of_driven == 4)
			break;                       //�����жϵ�������·û�п����ƶ��ĳ���ʱ���˳���ǰ·�ڵĵ���
		if (cur_road == -1 || Road[cur_road].getis_finished(Road[cur_road].getisFrom()) == 1)
		{
			No = getNextNo(No);    //��˳�������һ����·
			cur_road = arr[No];
			num_of_driven++;
			continue;
		}
		int num_road1 = Road[cur_road].getnum_road();
		int lon1 = Road[cur_road].getlongth();
		int len1 = num_road1*lon1;
		int isFrom1 = Road[cur_road].getisFrom();
		int carID = getCarId(cur_road, Road, Car, Lane, Road[cur_road].getisFrom(), first_car);      //��ȡ��ǰ�����ȵĳ���carID
		int first_sch1 = Road[cur_road].getfirst_sch(isFrom1);
		if (Road[cur_road].getlast_finish(isFrom1*num_road1 + first_sch1%num_road1) == (len1*isFrom1 - 1))      //��ʾ�õ�·�ϸó�ǰ��û�г�����
		{
			if ((Road[cur_road].getis_finished(isFrom1) == 1) || (carID == -1))    //��ʾ��ǰ��·�Ѿ��������
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
				//cout << "·��" << No_Cross << "����ɵ�·" << cur_road << endl;
				if (num == 4)          //��ʾ��ǰ·���Ѿ�������
					break;
				No = getNextNo(No);    //��˳�������һ����·
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
				else if (remain11<v11&&Car[carID - first_car].gettoId() - first_cross == No_Cross)    //����ǰ������ʻ��·�ڵ��Ѿ���ʻ��Ŀ�ĵ�
				{
					NUM_OF_FINISH++;
					NUM_ON_ROAD--;
					//	cout << "�ó��Ѿ���ɵ���" << endl;
					Lane[cur_road][first_sch1] = -1;    //����ǰλ�����
					if (Road[cur_road].getlast_sch(first_sch1%num_road1 + num_road1*isFrom1) == first_sch1)  //��ǰ���ȵĳ�Ϊ�ó��������һ��������Ҫ���¸ó�����last_sch��Ϣ
					{
						Road[cur_road].setlast_sch(first_sch1%num_road1 + num_road1*isFrom1, len1*isFrom1 - 1);
					}
					num_all_driven++;
					continue;
				}
			}
			vector<int>::iterator pos;          //������ʻ·�ߣ�����ϳ�����ǰλ�ã���·�ڸ��³�����ʻ��operate
			for (pos = ThePathOfDrive[carID - first_car].begin();pos != (ThePathOfDrive[carID - first_car].end());pos++)
			{
				if ((*pos) == cur_road + first_road)    //��������ǰ��·
				{

					pos++;
					if (pos != (ThePathOfDrive[carID - first_car].end()))
					{
						//cout << "  pos++" << endl;
						int next_r = (*pos) - first_road;    //��һ��Ҫ��ʹ�ĵ�·�����ֵ
						if (next_r == arr1[(No + 2) % 4])   //��ʾֱ��
							Car[carID - first_car].setoperate(0);
						else if (next_r == arr1[(No + 1) % 4])   //��ʾ��ת
							Car[carID - first_car].setoperate(1);
						else if (next_r == arr1[(No + 3) % 4])   //��ʾ����ת
							Car[carID - first_car].setoperate(2);
						break;
					}
					else
						pos--;

				}
			}
			//cout << "doing it " << endl;
			if ((Car[carID - first_car].getoperate()) == 0)         //��ʾ��ǰ�ĳ�Ϊֱ��
			{
				int next_road = arr1[(No + 2) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //��ʾ�ó�Ϊ�����ȴ���ת����һ����·
				{
					No = getNextNo(No);    //��˳�������һ����·
					cur_road = arr[No];
					num_of_driven++;
					num_of_change++;
					continue;
				}
				num_of_driven = 0;
				num_all_driven++;
				continue;
			}
			else if ((Car[carID - first_car].getoperate()) == 1)     //��ʾ��ǰ�ĳ�Ϊ��ת
			{
				int next_road1 = arr[(No + 3) % 4];
				if (next_road1 != -1)    //����arr[(No + 3) % 4]������·�Ƿ�Ϊ-1���������-1�����ж�
				{
					int isFrom_next = Road[next_road1].getisFrom();
					int carID1 = getCarId1(next_road1, Road, Car, Lane, isFrom_next, first_car);
					if (carID1 != -1 && Car[carID1 - first_car].getoperate() == 0)     //����ת����ֱ��ʱ����Ҫת��ֱ�г���
					{
						No = (No + 3) % 4;
						cur_road = next_road1;
						num_of_driven = 0;
						num_of_change++;
						continue;
					}
				}
				//��ʾ��ǰ��·�ĳ�������ʻ
				int next_road = arr1[(No + 1) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //��ʾ�ó�Ϊ�����ȴ���ת����һ����·
				{
					No = getNextNo(No);    //��˳�������һ����·
					cur_road = arr[No];
					num_of_driven++;      //��¼Ϊ�ȴ��ĳ�����
					num_of_change++;
					continue;
				}

				num_of_driven = 0;
				num_all_driven++;
				continue;
			}
			else if ((Car[carID - first_car].getoperate()) == 2)     //��ʾ��ǰ�ĳ�Ϊ��ת
			{
				int next_road1 = arr[(No + 1) % 4];      //���ǽ�Ҫʻ���·��Ϊֱ�е�������·
				int next_road2 = arr[(No + 2) % 4];      //���ǽ�Ҫʻ���·��Ϊ��ת��������·
				if (next_road1 != -1)
				{
					int isFrom_next1 = Road[next_road1].getisFrom();
					int carID1 = getCarId1(next_road1, Road, Car, Lane, isFrom_next1, first_car);
					if (carID1 != -1 && Car[carID1 - first_car].getoperate() == 0)     //����ת����ֱ��ʱ����Ҫת��ֱ�г���
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
					if (carID2 != -1 && Car[carID2 - first_car].getoperate() == 1)     //����ת������תʱ����Ҫת����ת����
					{
						No = (No + 2) % 4;
						cur_road = next_road2;
						num_of_driven = 0;
						num_of_change++;
						continue;
					}
				}
				//��ʾ��ǰ��·�ĳ�������
				int next_road = arr1[(No + 3) % 4];
				int isdrive = driveCar1(carID, No_Cross, first_cross, cur_road, next_road, Road, Car, Lane);
				if (isdrive == 0)  //��ʾ�ó�Ϊ�����ȴ���ת����һ����·
				{
					No = getNextNo(No);    //��˳�������һ����·
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
		else              //��ʾ�õ�·�����ȵĳ�ǰ�����ѵ�����ɵĳ�����
		{
			if ((Road[cur_road].getis_finished(isFrom1) == 1) || (carID == -1))    //��ʾ��ǰ��·�Ѿ��������
			{
				int num = Cross[No_Cross].getfinish_road();
				num++;
				Cross[No_Cross].setfinish_road(num);
				if (num == 4)          //��ʾ��ǰ·���Ѿ�������
					break;
				No = getNextNo(No);    //��˳�������һ����·
				cur_road = arr[No];
				num_of_driven++;
				num_of_change++;
				continue;
			}
			driveCar2(carID, cur_road, Road, Car, Lane);    //���ȳ���
			num_all_driven++;
		}
	}
	return num_all_driven;      //���ڼ�¼��ǰ·���ڸô�ѭ�����ƶ��ĳ����������ڼ������
}
bool driveAllCarToTheEnd(vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive)  //�ú����Ե��ȵڶ�����ÿ��·�ڵı���,������������򷵻�false���򷵻�true
{
	int No_cross_finish = 0;
	int num_driven_car = 0; //���ڼ���Ƿ�������
	int isd = Road[79].getisDouble();
	while (No_cross_finish < NUM_OF_CROSS)       //���ڵ���ÿ��·�ڣ�ֱ��·�ڵ������
	{
		for (int No_cross = 0;No_cross < NUM_OF_CROSS;No_cross++)    //һ�ζ�ÿ��·�ڽ��б���
		{
			if (Cross[No_cross].getfinish_road() == 4)          //��ʾ��·���Ѿ��������
			{
				No_cross_finish++;
				continue;
			}
			else                        //��ʾ��·��û�е�����
			{
				int num = driveAllCarForEachCross(No_cross, Cross, Road, Lane, Car, ThePathOfDrive);
				if (Cross[No_cross].getfinish_road() == 4)
					No_cross_finish++;
				num_driven_car += num;
			}
			//	cout << "do the cross:" << No_cross << endl;

		}
		//cout << "��ɵ�·��Ϊ" << No_cross_finish << "��ʻ������" << num_driven_car << endl;
		if (No_cross_finish == NUM_OF_CROSS)     //��ʾ����·���Ѿ���ɵ���
			return true;
		if (num_driven_car == 0)            //��ʾ��·��δ�����꣬��û�г��ƶ�����ʾ�ѽ�������
			return false;
		No_cross_finish = 0;
		num_driven_car = 0;
	}
}
bool dispatchCarPerTime(int cur_time, vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, list<int>& Ready_start, vector<vector<int>>&ThePathOfDrive, int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, int *nodeToPath)
{
	for (int No_road = 0;No_road < NUM_OF_ROAD;No_road++)     //���ȵ�һ������ÿһ����·���н��е���
	{
		driveAllCarJustOnRoadToEndState1(No_road, Lane, Car, Road);
	}
	//cout << "��ɵ�һ��" << endl;
	bool isFinish = driveAllCarToTheEnd(Cross, Road, Lane, Car, ThePathOfDrive);     //���ȵڶ��������Ƿ��������
	if (isFinish == false)
		return false;
	//cout << "��ɵڶ���" << endl;
	list<int>::iterator pos;
	int first_road = Road[0].getId();
	int first_car = Car[0].getId();
	int first_cross = Cross[0].getId();
	int num_depart = 0;
	for (pos = Ready_start.begin();pos != Ready_start.end();)
	{

		if (NUM_ON_ROAD > 1100)   //·�ϳ�����700������
			break;
		//if (num_depart > 200)    //����ʱ��Ƭ������������400
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
		if (cur_time > 10 && cur_time < 450 && num_depart > 60)   //ʱ�䳬��10������ʱ��Ƭ������������100
			break;
		if (cur_time <= 10 && num_depart > 150)   //ʱ�䳬��10������ʱ��Ƭ������������100
			break;
		if (cur_time >450 && num_depart > 150)   //ʱ�䳬��10������ʱ��Ƭ������������100
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
		Car[i].setstate(3);             //��ÿһ��������ʶΪ������
	for (int i = 0;i < NUM_OF_CROSS;i++)
		Cross[i].setfinish_road(0);     //��ÿһ��·�ڵ�finish_road����Ϊ0
	for (int i = 0;i < NUM_OF_ROAD;i++)   //��ÿ����·��is_finished����Ϊ0
	{
		int isD = Road[i].getisDouble();
		for (int j = 0;j < (isD + 1);j++)
			Road[i].setis_finished(j, 0);
	}
	return true;
}
int dispatchCar(vector<cross>&Cross, vector<road>&Road, int **Lane, vector<car>&Car, vector<vector<int>>&ThePathOfDrive, map<int, list<int>>&EachTimeOfPlan, int *minLen2Nodes1, int *minLen2Nodes2, int *pathOf2Nodes1, int *pathOf2Nodes2, int *nodeToPath)  //һ��ʱ��Ƭһ��ʱ��Ƭ������Ӧ�ĵ���
{
	int cur_time = -1;
	list<int> Ready_start;
	while (NUM_OF_FINISH != NUM_OF_CAR)     //�����г�û�е���Ŀ�ĵأ����Ͻ��е���
	{
		cur_time++;
		cout << "��ǰʱ��ƬΪ��" << cur_time << endl;
		for (int i = 0;i < NUM_OF_ROAD;i++)
		{
			int num = Road[i].getnum_road();
			int lon = Road[i].getlongth();
			int isD = Road[i].getisDouble();
			int len = num*lon*(1 + isD);
		}
		if (EachTimeOfPlan.count(cur_time) == 1)   //��cur_time==plan_time�ĳ��ӵ�����������
		{
			Ready_start.merge(EachTimeOfPlan[cur_time]);
		}
		bool isOK = dispatchCarPerTime(cur_time, Cross, Road, Lane, Car, Ready_start, ThePathOfDrive, minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, nodeToPath);  //����ÿ��ʱ��Ƭ�ĵ���
		if (isOK == false)     //���������������أ����������һ��ʱ��Ƭ�ĵ���
			return -1;
	}
	return cur_time;    //�����ܵĵ���ʱ��
}
int main()
{
	vector<cross> Cross;  //�����洢·��
	vector<road>Road;   //�����洢��·
	vector<car>Car;   //�����洢����
	map<int, list<int>>EachTimeOfPlan;  //ÿ��ʱ��Ƭ׼�������ĳ���ɵ�����
	vector<vector<int>>ThePathOfDrive;  //��¼ÿ�������г�·��
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
			makeCar(Car, line, EachTimeOfPlan, ThePathOfDrive);
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	infile.close();
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
	int *nodeToPath = new int[size];    //�洢�������������ڵ�֮��ĵ�·
	for (int i = 0;i < size;i++)
		nodeToPath[i] = -1;
	makeNodeToPath(nodeToPath, Road, NUM_OF_CROSS, NUM_OF_ROAD, Cross[0].getId());
	makeMinLen2Nodes(minLen2Nodes1, minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, Cross[0].getId());   //������ǰ��·�����µ���������֮������·��
	int first_cross = Cross[0].getId();
	int first_car = Car[0].getId();
	for (int carId = 0;carId < NUM_OF_CAR;carId++)       //Ϊÿһ�����滮��̬·��  �˴���carIdΪ���ֵ
	{
		int from = Car[carId].getfromId() - first_cross;
		int to = Car[carId].gettoId() - first_cross;
		int v = Car[carId].getmax_v();
		{
			if (v > MIN_V_ROAD)                 //��¼�г�·��
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
	ofstream outfile("answer.txt");        //����������answer.txt�ļ���

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

	//makeMinLen2Nodes(minLen2Nodes1,minLen2Nodes2, pathOf2Nodes1, pathOf2Nodes2, Road, NUM_OF_ROAD, NUM_OF_CROSS, Cross[0].getId());  //����ʼʱ��������֮�����̾������ʻʱ��
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