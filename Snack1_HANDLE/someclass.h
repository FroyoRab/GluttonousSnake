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
	//����������������ݴ�
	void Now_give();
	//�ж����Ƿ��ڲݴ��ڣ�������ɵĲݴԣ�
	bool snack_in();
	//���òݴԶ���������
	void set_givetm();
	//�жϳ���ʱ���Ƿ���
	void do_give();
	//���òݴԶ��������ʧ
	void set_vanishtm();
	//�ж���ʧʱ���Ƿ���
	void do_vanish();
	//��ӡ�ݴ�
	void print_land();
	//�����ݴ�
	void clear_land();
	//��ӡ�ұ���Ϣ����������
	void prinfo();
	//�ж���β�ǲ����ڲݴ���(��֤��β�Ĵ�ӡ�հײ�������ݴ�)
	bool sanck_end_in();
};