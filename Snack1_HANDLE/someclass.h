#pragma once

class _Food
{
	int FoodX;
	int FoodY;
public:
	void GiveFood();
	virtual void isEatFood();
	void ChangeFood(int numX, int numY);
	virtual void prFood(int nfoodx, int nfoody);
protected:
	void clearFood();
	bool JudgeEat();
};

class _FastFood :protected _Food
{
	int Vanishtm ;
	int Givetm ;
	int Fasttm ;
	int nSpeedtemp ;
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

class _DoubleFood :public _FastFood
{
	int nUplong;
public:
	void prinfo(int choise, int temptm);
	void prFood(int nfoodx, int nfoody);
	void isEatFood();
	void goreturn();
};

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