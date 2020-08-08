#include "Func.h"
#include "Food.h"
#include <Windows.h>
//随机生成地形的最大大小
#define LARGE 50

class _grass
{
	int give_time = 0;
	int vanish_time = 0;
public:
	COORD *Positation;
	_grass();
	virtual ~_grass();
	//在数组内生成随机草丛
	void Now_give();
	//判断蛇是否在草丛内（随机生成的草丛）
	bool snack_in();
	//设置草丛多少秒后出现
	void set_givetm();
	//判断出现时间是否到了
	void do_give();
	//设置草丛多少秒后消失
	void set_vanishtm();
	//判断消失时间是否到了
	void do_vanish();
	//打印草丛
	void print_land();
	//擦掉草丛
	void clear_land();
	//打印右边信息多少秒后出现
	void prinfo();
	//判断蛇尾是不是在草丛内(保证蛇尾的打印空白不会擦除草丛)
	bool sanck_end_in();
};

_grass::_grass()
{
	Positation = new COORD[LARGE+1];
}

_grass::~_grass() {
	if (Positation != NULL)
		delete[] Positation;
	Positation = NULL;
}

//生成随机草丛
void _grass::Now_give()
{
	Positation[0].X = RandNum(43) + 1;
	Positation[0].Y = RandNum(43) + 1;//在1-44之间随机左上角坐标值

	int count = 1;
	for (int cols = 0; cols < 10; cols++)
	{
		for (int lines = 0; lines < 5; lines++,count++)
		{
			Positation[count].X = Positation[0].X + cols;
			Positation[count].Y = Positation[0].Y + lines;
		}
	}//根据左上角的位置推其他50格的位置

	for (int i = 0; i < 20; i++)//随机几个格子变成空白，有草丛的感觉
	{
		int rand_temp = RandNum(LARGE);
		Positation[rand_temp].X = 0;
		Positation[rand_temp].Y = 0;
	}
	print_land();//打印整个草丛
}

//打印草丛
void _grass::print_land()
{
	for (int i = 0; i < LARGE; i++)
	{
		printSomeIn(Positation[i].X, Positation[i].Y,
			GRASS_ico, 0xf2);//绿色
	}
}

//擦掉草丛
void _grass::clear_land()
{//每个格子用两个空白填空
	for (int i = 0; i < LARGE; i++)
	{
		printSomeIn(Positation[i].X, Positation[i].Y, "   ");
	}
	//打印一下地图，避免删除的时候把障碍物也删除了
	get_map();
}

//打印右边信息多少秒后出现
void _grass::prinfo()
{
	printIn(54, 13);
	std::cout << "grass_give:"
		<< (give_time <= 0 ? 0 : (give_time - int(time(0))));//差值才是剩余时间
	printIn(54, 14);
	std::cout << "grass_vanish:"
		<< (vanish_time <= 0 ? 0 : (vanish_time - int(time(0))));
}

//判断蛇尾是不是在草丛内(保证蛇尾的打印空白不会擦除草丛)
bool _grass::sanck_end_in()
{
	if (give_time - int(time(0)) > 0)return false;//如果没有草地直接返回判断
	for (int i = 0; i < LARGE; i++)//用蛇尾坐标遍历草丛数组
	{
		if (SnackX[nSnackLong-1] == Positation[i].X
			&&SnackY[nSnackLong-1] == Positation[i].Y)
			return true;
	}
	return false;
}

//判断蛇是否在草丛内（随机生成的草丛）
bool _grass::snack_in()
{
	do_give();//顺便判断给时间和消失时间
	do_vanish();
	prinfo();
	if (give_time-int(time(0)) > 0)return false;
	for (int i = 0; i < 100; i++)
	{
		if (nSnackLong > 1)					//长度大于一的时候判断脖子是不是进草丛了
		{									//否则导致补的草丛会出现在外面
			if (SnackX[1] == Positation[i].X
				&&SnackY[1] == Positation[i].Y)
				return true;
		}
		else if (SnackX[0] == Positation[i].X
			&&SnackY[0] == Positation[i].Y)
			return true;
	}
	return false;
}

//设置草丛出现时间
void _grass::set_givetm()
{
	give_time = time(0);
	give_time += RandNum(60);
}

//判断出现时间是不是到了
void _grass::do_give()
{
	if (give_time <= time(0) && give_time != 0) {
		Now_give();//数组内生成草丛位置
		give_time = 0;//重置出现时间
		set_vanishtm();//设置消除时间
	}
}

//设置消失时间
void _grass::set_vanishtm()
{
	vanish_time = time(0);
	vanish_time += RandNum(120);
}

//判断消失时间是否到了
void _grass::do_vanish()
{
	if (vanish_time <= time(0) && vanish_time != 0) {
		clear_land();
		vanish_time = 0;
		set_givetm();
	}
}




