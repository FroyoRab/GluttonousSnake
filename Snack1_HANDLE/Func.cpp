#include "Func.h"
#include "Food.h"
#include "someclass.h"
//////////////////////////
//全局变量
bool bUp = false, bDown = false, bLeft = false, bRight = false;
bool b1Up = false, b1Down = false, b1Left = false, b1Right = false;

//timeupdown
int nSleepTime = 75;
int nSleepset = 75;

//每吃到一个增加的个数
int uplong = 10;

//保存分数
int nSavepoint = 0;

//当局游戏最高记录
int nPointMax = 0;

//🐍动态数组
vector<int> SnackX;
vector<int> SnackY;

//食物🍎
//基础食物
_Food stdFood;
//加速食物
_FastFood FastFood;
//双倍食物
_DoubleFood DoubleFood;
//草丛生成
_grass grass_land;

//🐍长度
int nSnackLong = 1;

//地图
char c_map[51][102];

//蛇双倍彩色控制
bool SnackColor = false;
//蛇加速红黄控制
bool SnackColor1 =false;

//是否读档过
bool read_save = false;

//音乐
char temp[255];

//加分值
int addpoint = 100;

//建立用于存档的结构体SAVELOAD
typedef struct _SnackSave
{
	char nName[10];
	int point;
}SAVELOAD;

SAVELOAD _stSave;
//定义并分配内存stSave结构体指针.
SAVELOAD *stSave = &_stSave;

//////////////////////////

//+判断死亡
int snakedead(vector<int> &vSnackX,vector<int> &vSnackY)
{
	//dead = 0;
	
	if (vSnackX[0] == 0 ||			//判断蛇头是否碰到边
		vSnackY[0] == 0 ||			
		vSnackX[0] == WINDOW_X + 1 
		|| vSnackY[0] == WINDOW_Y + 1
		|| c_map[SnackY[0]][SnackX[0]*2] == ICO)//判断地图障碍物
	{
		//死亡宣告！
		dead = 1;
		
		//蛇清空
		vSnackX.clear();
		vSnackY.clear();
		return dead;
	}

	for (int i = 1; i < nSnackLong; i++)
	{
		if (SnackX[0] == SnackX[i]//判断是否撞到自己
			&& SnackY[0] == SnackY[i])
		{
			dead = 1;//死亡宣告！
			SnackX.clear();
			SnackY.clear();
			break;
		}
	}
	return dead;
}

//初始化
void _Init()
{
	nSavepoint = 0;
	addpoint = 100;
	//打印边框
	prPoint();
	//死亡判断初始化
	dead = 0;
	SnackColor = false;
	SnackColor1 = false;
	/***************音乐****************/
	mciSendString("play MUSIC repeat", NULL, 0, 0);

	if (read_save == false) {//判断是否有选择过读档是的话不进行初始化
		SnackX.push_back(0);
		SnackY.push_back(0);
		//蛇头开始位置存入数组
		do {
		SnackX[0]=RandNum(WINDOW_X);
		SnackY[0]=RandNum(WINDOW_Y);
		} while (c_map[SnackY[0]][SnackX[0] * 2] == ICO);
		//打印蛇头
		printSomeIn(SnackX[0], SnackY[0], HEAD, 0xf5);
		nSleepTime = nSleepset;
		nSnackLong = 1;
	}
	else {//没有读档过初始化并且生成蛇头
		printSomeIn(SnackX[0], SnackY[0], HEAD, 0xf5);
		for (int i = 1; i < nSnackLong; i++)
		{
			printSomeIn(SnackX[i], SnackY[i], SNACK);
		}
	}
	get_map();

	FastFood.init();//食物初始化
	DoubleFood.init();
	stdFood.GiveFood();//食物刷新时间开始
	FastFood.StartGive();
	DoubleFood.StartGive();
	grass_land.set_givetm();//地形刷新时间开始
}
 
//方向      xy修改哪个   修改多少
void move(char x_or_y, int num)
{
	while (true)
	{
		printSomeIn(SnackX[nSnackLong - 1], SnackY[nSnackLong - 1], "  ");
		//蛇尾消除

		if (grass_land.sanck_end_in() ||//如果蛇尾在随机生成的草地内
			c_map[SnackY[nSnackLong - 1]]//如果蛇尾在地图草地内
			[SnackX[nSnackLong - 1] * 2] == grass_in_map)
			
			printSomeIn(SnackX[nSnackLong - 1], //就在用草地消除蛇尾
				SnackY[nSnackLong - 1], GRASS_ico, 0xf2);

		for (int i = nSnackLong - 1; i > 0; i--)
		{
			if (i > 0) {
				SnackY[i] = SnackY[i - 1];
				SnackX[i] = SnackX[i - 1];
			}
		}//将🐍尾设为蛇头

		switch (x_or_y)//根据传入值更改蛇头
		{
		case'x':SnackX[0] = SnackX[0] + num; break;
		case'y':SnackY[0] = SnackY[0] + num; break;
		}
		
		//判断蛇死亡
		if (snakedead(SnackX, SnackY)) break;
		
		//蛇的彩色循环色设置，一定要无符号，否则跟0xf?判断的时候未知bug
		static unsigned int tempcolor = 0xf0;
		static unsigned int tempcolor1 = 0x8f;
		printSomeIn(SnackX[0], SnackY[0], HEAD, 0xf5);//头部
		if (nSnackLong > 1)//蛇的如果就一个，直接过
		{
			if (grass_land.snack_in()//蛇在随机草地和地图草地内
				|| c_map[SnackY[1]][SnackX[1] * 2] == grass_in_map)
			{
				printSomeIn(SnackX[1], SnackY[1], GRASS_ico, 0xf2);//脖子打印草地
			}
			else if (SnackColor == true || SnackColor1 == true)
			{
				if (SnackColor == true) {//彩虹循环打印，双倍食物效果
					//打印蛇头和🐍脖子hh
					printSomeIn(SnackX[0], SnackY[0], HEAD, tempcolor);
					printSomeIn(SnackX[1], SnackY[1], SNACK, tempcolor);
					
					if (tempcolor == 0xfd)
						tempcolor = 0xf0;
					else tempcolor++;
						
				}
				else if (SnackColor1 == true) {//双色切换打印，加速食物效果
					printSomeIn(SnackX[0], SnackY[0], HEAD, tempcolor);
					printSomeIn(SnackX[1], SnackY[1], SNACK, tempcolor1++);
					if (tempcolor1 == 0xef)
						tempcolor1 = 0x8f;
					else tempcolor1+=0x10;
				}
			}
			else
			{
				printSomeIn(SnackX[0], SnackY[0], HEAD, 0xf5);
				printSomeIn(SnackX[1], SnackY[1], SNACK);
			}
		}
		stdFood.isEatFood();
		FastFood.isEatFood();
		DoubleFood.isEatFood();
		//判断是否吃到食物

		Sleep(nSleepTime < 0 ? 0 : nSleepTime);
		//Putwhat();
		if (_kbhit())break;
	}
}

//获取随机数
int RandNum(int nMax)
{
	return(rand() % nMax + 1);
}

//在任意位置打印‘字符’
void printSomeIn(int x, int y, string a,WORD color)
{
	printIn(x, y, color);
	std::cout << a;
}

//将光标转移到xy
void printIn(int x, int y,WORD color)
{
	//定义坐标类型和句柄类型
	COORD point;
	WORD _color = color;
	//坐标位置确定
	point.X = x*2;
	point.Y = y;

	//根据坐标位置设置光标位置
	SetConsoleCursorPosition(OutHandle, point);
	SetConsoleTextAttribute(OutHandle, _color);
}

//打印边框
void prBox()
{
	//第一行
	for (int i = 0; i <= WINDOW_X+1; i++)
	{
		printSomeIn(i, 0, WALL);
	}

	//第一列
	for (int i = 0; i <= WINDOW_Y+1; i++)
	{
		printSomeIn(0, i, WALL);
	}

	//右边列
	for (int i = 0; i <= WINDOW_X+1; i++)
	{
		printSomeIn(i, 50, WALL);
	}

	//最下面行
	for (int i = 0; i <= WINDOW_Y+1; i++)
	{
		printSomeIn(50, i, WALL);
	}
}

//打印右边数据
void prPoint()
{
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 5);
	printf("%10s %d\n", "当前分数：", nSavepoint);
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 7);
	printf("历史最高记录： ");
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 9);
	printf("姓名：%s  分数：%d", stSave->nName, stSave->point);
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 11);
	printf("当前最高记录： %d", nPointMax);
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 13);
	printf("%10s", "请使用英文输入法游玩");
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 15);
	printf("%10s", "无法移动请确认大写锁定关闭");
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 17);
	printf("%10s", "当前游戏设定：");
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 19);
	printf("速度：%d", (100 - nSleepTime + 2));
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 21);
	printf("每次增加：%d 格长度", uplong);
	printIn(WINDOW_X + 5, (WINDOW_Y / 4) + 23);
	printf("按除WASD以外按键暂停，WASD恢复");
	printIn(0, 0);
}

//存档
void SaveLoad()
{

	if (nSavepoint < stSave->point)
	{
		return;
	}
	nPointMax = nSavepoint;
	for (int i = 0; i != 1;)
	{
		//询问用户是否需要存档
		system("cls");
		prBox();
		printIn(WINDOW_X/2-6, WINDOW_Y/2-2);
		printf("是否需要存档？1.是 2.否");
		char Input2 = _getch();
		switch (Input2)
		{
		case'1':
		{
			system("cls");
			prBox();
			prPoint();

			//获取数据
			printIn(WINDOW_X / 2 - 6, WINDOW_Y / 2 - 1);
			printf("请输入姓名：");
			scanf_s("%s", stSave->nName, 10);
			stSave->point = nPointMax;

			/*************存档*****************/

			//打开文件
			FILE *pfile;
			//判断文件打开
			if (fopen_s(&pfile, "SnakeSave.i", "wb"))
				printf("save point err!,can not open file\"SnackSave.i\"");

			fwrite(stSave, sizeof(SAVELOAD), 1, pfile);

			fclose(pfile);
			system("cls");
			printIn(44, 23);
			printf("存档成功！");
			i = 1;//退出外循环
			system("pause");
			system("cls");
		}
		case'2':
		{
			i = 1;//退出外循环
			break;
		}
		default:break;
		}
	}

}

//读档
void LoadSave()
{
	FILE *pfile;
	if (fopen_s(&pfile, "SnakeSave.i", "r"))
	{
		printIn(40, 23);
		printf("open file error!Now creat new file.");
		if (fopen_s(&pfile, "SnackSave.i", "w"))
		{
			printIn(40, 24);
			printf("creat file error!");
			exit(1);
		}
	}
	//主要是蛇的位置，分数和地图分开存的
	fread_s(stSave, sizeof(SAVELOAD),
		sizeof(SAVELOAD), 1, pfile);

	fclose(pfile);
	system("cls");
}

//游戏设置
void setting()
{
	while (1)
	{
		system("cls");
		printf("蛇移动速度设置(默认30,设置范围：10-100)：");
		int ntemp = 0;
		scanf_s("%d", &ntemp);
		if (ntemp <= 100 && ntemp >= 10)
		{
			nSleepset = (102 - ntemp);
			while (true)
			{
				printf("蛇每次吃到食物增加几格(默认10,设置范围：无限制)：");
				scanf_s("%d", &ntemp);
				if (ntemp > 0)
				{
					uplong = ntemp;
					system("cls");
					printf("设置成功！");
					system("pause");
					system("cls");
					break;
				}
			}
			break;
		}
		else printf("请输入正确数值!");
	}
}

//获取地图并直接打印
void get_map() {
	using std::cout;
	using std::endl;
	FILE *pfile;
	if (fopen_s(&pfile, "map.txt", "rb")) {
		cout << "打开文件失败，重置地图" << endl;
		Sleep(200);
		init_map();
		fopen_s(&pfile, "map.txt", "rb");
	}

	fread_s(&c_map, sizeof(c_map), sizeof(c_map), 1, pfile);

	for (int i = 1; i < 101; i += 2)//墙如果在双数列的右移
	{
		for (int a = 0; a < 51; a++)
		{
			if (c_map[a][i] == ICO
				&&(c_map[a][i - 1] == map_ICO
				||c_map[a][i + 1] == map_ICO))
			{
				c_map[a][i] = map_ICO;
				c_map[a][i + 1] = ICO;
			}
		}
	}
	for (int i = 1; i < 101; i += 2)//草如果在双数列的右移
	{
		for (int a = 0; a < 51; a++)
		{
			if (c_map[a][i] == grass_in_map)
			{
				c_map[a][i] = map_ICO;
				c_map[a][i + 1] = grass_in_map;
			}
		}
	}

	for (int i = 0; i < 101; i++)//打印墙体和草
	{
		for (int a = 0; a < 51; a++)
		{
			if (c_map[a][i] == map_ICO){}
			else if(c_map[a][i]==ICO){
				printSomeIn(i / 2, a, WALL, 0xf8);
			}
			else if (c_map[a][i] == grass_in_map) {
				printSomeIn(i / 2, a, GRASS_ico, 0xf2);
			}
		}
	}
	fclose(pfile);
	save_map();
}


//重置地图
void init_map() {
	system("cls");
	system("copy /Y map_back.txt map.txt");//将备份文件覆盖现有地图
	save_map();
	printIn(WINDOW_X / 2 - 10, WINDOW_Y / 2);
	std::cout << "重置地图并保存成功！";
}

//保存地图
void save_map()
{
	using std::cout;
	using std::endl;
	FILE *pfile;
	if (fopen_s(&pfile, "map.txt", "wb")) {
		cout << "打开文件失败" << endl;
		exit(0);
	} 
	else cout << "打开文件成功" ;

	fwrite(c_map, sizeof(c_map), 1, pfile);
	fclose(pfile);
}

//暂停并存档
void save_all()
{
	system("cls");
	save_map();
	using std::cout;
	using std::endl;
	FILE *pfile;
	if (fopen_s(&pfile, "saveall.i", "wb")) {
		cout << "打开文件失败" << endl;
		exit(0);
	}
	fprintf_s(pfile, "nSnackLong:%d", nSnackLong);//先存长度
	fwrite(&SnackX, nSnackLong * 4, 1, pfile);
	fwrite(&SnackY, nSnackLong * 4, 1, pfile);//再存整个蛇的数组
	printIn(WINDOW_X / 2 - 2, WINDOW_Y / 2);
	cout << "存档成功！" << endl;
	printIn(WINDOW_X / 2 - 2, WINDOW_Y / 2+1);
	cout << "任意键退出";
	dead = 1;
	system("pause");
	fclose(pfile);
}

//读档
void read_all()
{
	using std::cout;
	using std::endl;
	FILE *pfile;
	if (fopen_s(&pfile, "saveall.i", "wb")) {
		cout << "打开文件失败" << endl;
		exit(0);
	}
	else cout << "打开文件成功" << endl;
	fscanf_s(pfile, "nSnackLong:%d", &nSnackLong);
	for (int i = 0; i < nSnackLong; i++)
	{
		SnackX.push_back(0);//根据蛇的长度扩大蛇的数组，再将蛇的数据存入数组
		SnackY.push_back(0);
	}
	fread_s(&SnackX, nSnackLong * 4, nSnackLong * 4, 1, pfile);
	fread_s(&SnackY, nSnackLong * 4, nSnackLong * 4, 1, pfile);
	printIn(WINDOW_X / 2 - 2, WINDOW_Y / 2);
	read_save = true;
	system("cls");
	printIn(WINDOW_X / 2 - 2, WINDOW_Y / 2);
	cout << "读档成功，可以直接开始游戏";
	system("pause");
	fclose(pfile);
}

//加分提示与加分
void Points(int point) {
	static int tempcolor = 0xf1;//颜色循环
	printIn(WINDOW_X + 13, (WINDOW_Y / 4) + 5);
	std::cout << "                   ";
	printIn(WINDOW_X + 13, (WINDOW_Y / 4) + 5, tempcolor);
	if (tempcolor < 0xfe)
		tempcolor++;
	else
		tempcolor = 0xf1;
	std::cout << "+ " << point << "  ";
	nSavepoint += point;//加分
}
