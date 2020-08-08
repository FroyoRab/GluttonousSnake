#include "Food.h"
#include "Func.h"
#include <ctime>
class _Food
{
	int FoodX = 0;
	int FoodY = 0;
public:
	void GiveFood();
	virtual void isEatFood();
	void ChangeFood(int numX, int numY);
	virtual void prFood(int nfoodx, int nfoody);
protected:
	void clearFood();
	bool JudgeEat();
};


void _Food::GiveFood()
{
	bool need_while = false;
	bool need_while1 = false;
	do {
		ChangeFood(RandNum(WINDOW_X), RandNum(WINDOW_Y));
		for (int i = 0; i < nSnackLong; i++)
		{
			if (FoodX == SnackX[i] && FoodY == SnackY[i]) {
				need_while = true;//如果食物生成在蛇的身上，返回一个true
				break;
			}
			else need_while = false;
		}
		if (c_map[FoodY][FoodX * 2] == ICO)
			need_while1 = true;//如果食物生成在障碍物闪，返回一个true
		else need_while1 = false;
	} while (need_while||need_while1);//有一个true就重来，不递归了，怪卡的
	prFood(FoodX, FoodY);
}

bool _Food::JudgeEat()
{
	if (SnackX[0] == FoodX && SnackY[0] == FoodY)
		return true;
	else return false;
}

void _Food::isEatFood()
{
	if (JudgeEat())
	{
		GiveFood();//给新食物
		Points(addpoint);//加100分
		prPoint();//更新右边面板
		PlaySoundA("sound\\eat.wav", NULL, SND_ASYNC | SND_NODEFAULT);
		for (int i = 0; i < uplong; i++)//增加蛇
		{
			nSnackLong += 1;
			SnackX.push_back(SnackX[nSnackLong - 2]);
			SnackY.push_back(SnackY[nSnackLong - 2]);
		}
	}

}

void _Food::ChangeFood(int numX, int numY)
{
	FoodX = numX;
	FoodY = numY;
}

void _Food::prFood(int nfoodx, int nfoody)
{
	printSomeIn(nfoodx, nfoody, FOOD,0xf5);
}

void _Food::clearFood()
{
	printSomeIn(FoodX, FoodY, "  ");
	PlaySoundA("sound\\fastfoodvanish.wav", NULL, SND_ASYNC | SND_NODEFAULT);
	FoodX = FoodY = 0;
}


/*******************特殊食物**********************/

//加速食物
class _FastFood :protected _Food
{
	int Vanishtm = 0;
	int Givetm = 0;
	int Fasttm = 0;
	int nSpeedtemp = 0;
protected:
	void SetVanish();
	void Vanish();
	void SetGive();
	void Give();
	void setFast();
	void NoFast();
	void testpr();
public:
	void StartGive();
	void isEatFood();
	void prFood(int nfoodx, int nfoody);
	void init();
	virtual void prinfo(int choise, int temptm);
	virtual void goreturn();
};

void _FastFood::isEatFood()
{
	Give();
	Vanish();//不断检测是否需要消失和给予以及加速停止
	NoFast();
	testpr();//测试打印
	if (JudgeEat())
	{
		Vanishtm = 0;//重置vanish
		Givetm = 0;//重置give
		SnackColor1 = true;
		SetGive();//设置新的给予时间
		Points(addpoint + 20);
		PlaySoundA("sound\\fastfood.wav", 
			NULL, SND_ASYNC | SND_NODEFAULT);
		nSpeedtemp = nSleepTime;
		nSleepTime -= 30;
		prPoint();//更新右边面板
		setFast();
	}
}

void _FastFood::StartGive() {//开始给特殊食物
	Givetm = time(0) + RandNum(5);
}

void _FastFood::prFood(int nfoodx, int nfoody)
{
	printSomeIn(nfoodx, nfoody, SpeedupFood_ico,0xf4);
	PlaySoundA("sound\\givefastfood.wav", 
		NULL, SND_ASYNC | SND_NODEFAULT);
}

void _FastFood::init()
{
	Fasttm = 0;
	Vanishtm = 0;
	Givetm = 0;
}

void _FastFood::NoFast() {
	if (Fasttm == time(0)) {
		goreturn();
		prPoint();
	}
}
void _FastFood::goreturn() {
	PlaySoundA("sound\\nofast.wav", NULL, SND_ASYNC | SND_NODEFAULT);
	nSleepTime = nSpeedtemp;
	SnackColor1 = false;
}

void _FastFood::setFast() {
	Fasttm = time(0);
	Fasttm += RandNum(5)+5;
}

void _FastFood::Give() {//需要不断检测的给予
	if (Givetm <= time(0) && Givetm != 0) {
		GiveFood();

		Givetm = 0;//重置give时间，保证不生成两个
		SetVanish();
	}
}

void _FastFood::SetGive() {//消失后随机设置给予时间
	Givetm = time(0);
	Givetm += RandNum(30);
}

void _FastFood::Vanish() {//需要不断检测的消失
	if (Vanishtm <= time(0) && Vanishtm != 0) {
		clearFood();
		Vanishtm = 0;
		SetGive();
	}
}

void _FastFood::testpr()
{//右边时间提示，从测试的数值直接改的
	using std::cout;
	if ((Givetm - int(time(0))) >= 0) {
		prinfo(1, Givetm);
	}
	if ((Vanishtm - int(time(0))) >= 0) {
		prinfo(2, Vanishtm);
	}
	if ((Fasttm - int(time(0))) >= 0) {
		prinfo(3, Fasttm);
	}
	else {
		prinfo(0, 0);
	}
}

void _FastFood::prinfo(int choise,int temptm)
{
	if (choise == 1)
	{
		printIn(54, 5);
		std::cout << SpeedupFood_ico
			<< ":"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒后给予 ";
	}
	if (choise == 2)
	{
		printIn(54, 5);
		std::cout << SpeedupFood_ico
			<< ":"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒后消失 ";
	}
	if (choise == 3)
	{
		printIn(54, 7);
		std::cout << SpeedupFood_ico
			<< "效果:"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒结束 ";
	}
	else
	{
		printIn(54, 7); std::cout << "                     ";
	}

}

void _FastFood::SetVanish() {//随机设置消失时间
	Vanishtm = time(0);
	Vanishtm += 5;
}



/************************增长翻倍食物***********************/

class _DoubleFood :public _FastFood
{
	int nUplong;
public:
	void prFood(int nfoodx, int nfoody);
	void prinfo(int choise, int temptm);
	void isEatFood();
	void goreturn();
};

void _DoubleFood::goreturn()
{
	PlaySoundA("sound\\noDouble.wav", NULL, SND_ASYNC | SND_NODEFAULT);
	uplong = nUplong;
	addpoint /= 2;
	SnackColor = false;
}

void _DoubleFood::prinfo(int choise, int temptm)
{
	using std::cout;
	if (choise == 1){
		printIn(54, 9);
		std::cout << DoubleFood_ico
			<< ":"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒后给予 ";
	}
	if (choise == 2){
		printIn(54, 9);
		std::cout << DoubleFood_ico
			<< ":"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒后消失 ";
	}
	if (choise == 3){
		printIn(54, 11);
		std::cout << DoubleFood_ico
			<< "效果:"
			<< (temptm <= 0 ? 0 : (temptm - int(time(0))))
			<< " 秒结束 ";
	}
	else{
		printIn(54, 11); std::cout << "                     ";
	}
}

void _DoubleFood::prFood(int nfoodx,int nfoody)
{
	printSomeIn(nfoodx, nfoody, DoubleFood_ico,0xf5);
	PlaySoundA("sound\\giveDouble.wav", 
		NULL, SND_ASYNC | SND_NODEFAULT);
}

void _DoubleFood::isEatFood()
{
	Give();
	Vanish();//不断检测是否需要消失和给予以及加速停止
	NoFast();
	testpr();//测试打印
	if (JudgeEat())
	{
		init();
		SetGive();//设置新的给予时间
		SnackColor = true;
		Points(addpoint+100);
		PlaySoundA("sound\\DoubleEat.wav", NULL, SND_ASYNC | SND_NODEFAULT);
		nUplong = uplong;
		uplong *= 2;
		addpoint *= 2;
		prPoint();//更新右边面板
		setFast();
	}
}