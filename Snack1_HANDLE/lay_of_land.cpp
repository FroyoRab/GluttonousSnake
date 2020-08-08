#include "Func.h"
#include "Food.h"
#include <Windows.h>
//������ɵ��ε�����С
#define LARGE 50

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

_grass::_grass()
{
	Positation = new COORD[LARGE+1];
}

_grass::~_grass() {
	if (Positation != NULL)
		delete[] Positation;
	Positation = NULL;
}

//��������ݴ�
void _grass::Now_give()
{
	Positation[0].X = RandNum(43) + 1;
	Positation[0].Y = RandNum(43) + 1;//��1-44֮��������Ͻ�����ֵ

	int count = 1;
	for (int cols = 0; cols < 10; cols++)
	{
		for (int lines = 0; lines < 5; lines++,count++)
		{
			Positation[count].X = Positation[0].X + cols;
			Positation[count].Y = Positation[0].Y + lines;
		}
	}//�������Ͻǵ�λ��������50���λ��

	for (int i = 0; i < 20; i++)//����������ӱ�ɿհף��вݴԵĸо�
	{
		int rand_temp = RandNum(LARGE);
		Positation[rand_temp].X = 0;
		Positation[rand_temp].Y = 0;
	}
	print_land();//��ӡ�����ݴ�
}

//��ӡ�ݴ�
void _grass::print_land()
{
	for (int i = 0; i < LARGE; i++)
	{
		printSomeIn(Positation[i].X, Positation[i].Y,
			GRASS_ico, 0xf2);//��ɫ
	}
}

//�����ݴ�
void _grass::clear_land()
{//ÿ�������������հ����
	for (int i = 0; i < LARGE; i++)
	{
		printSomeIn(Positation[i].X, Positation[i].Y, "   ");
	}
	//��ӡһ�µ�ͼ������ɾ����ʱ����ϰ���Ҳɾ����
	get_map();
}

//��ӡ�ұ���Ϣ����������
void _grass::prinfo()
{
	printIn(54, 13);
	std::cout << "grass_give:"
		<< (give_time <= 0 ? 0 : (give_time - int(time(0))));//��ֵ����ʣ��ʱ��
	printIn(54, 14);
	std::cout << "grass_vanish:"
		<< (vanish_time <= 0 ? 0 : (vanish_time - int(time(0))));
}

//�ж���β�ǲ����ڲݴ���(��֤��β�Ĵ�ӡ�հײ�������ݴ�)
bool _grass::sanck_end_in()
{
	if (give_time - int(time(0)) > 0)return false;//���û�вݵ�ֱ�ӷ����ж�
	for (int i = 0; i < LARGE; i++)//����β��������ݴ�����
	{
		if (SnackX[nSnackLong-1] == Positation[i].X
			&&SnackY[nSnackLong-1] == Positation[i].Y)
			return true;
	}
	return false;
}

//�ж����Ƿ��ڲݴ��ڣ�������ɵĲݴԣ�
bool _grass::snack_in()
{
	do_give();//˳���жϸ�ʱ�����ʧʱ��
	do_vanish();
	prinfo();
	if (give_time-int(time(0)) > 0)return false;
	for (int i = 0; i < 100; i++)
	{
		if (nSnackLong > 1)					//���ȴ���һ��ʱ���жϲ����ǲ��ǽ��ݴ���
		{									//�����²��ĲݴԻ����������
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

//���òݴԳ���ʱ��
void _grass::set_givetm()
{
	give_time = time(0);
	give_time += RandNum(60);
}

//�жϳ���ʱ���ǲ��ǵ���
void _grass::do_give()
{
	if (give_time <= time(0) && give_time != 0) {
		Now_give();//���������ɲݴ�λ��
		give_time = 0;//���ó���ʱ��
		set_vanishtm();//��������ʱ��
	}
}

//������ʧʱ��
void _grass::set_vanishtm()
{
	vanish_time = time(0);
	vanish_time += RandNum(120);
}

//�ж���ʧʱ���Ƿ���
void _grass::do_vanish()
{
	if (vanish_time <= time(0) && vanish_time != 0) {
		clear_land();
		vanish_time = 0;
		set_givetm();
	}
}




