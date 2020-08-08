#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <string>

/////////////////////////////////////////
#define FOOD "★"
#define SpeedupFood_ico "▲"
#define DoubleFood_ico "◆"
#define GRASS_ico "▓▓"
#define SNACK /*"●"*/"◎"
#define HEAD "●"
#define WALL "■"
#define WINDOW_X 49
#define WINDOW_Y 49
#define ICO '#'
#define map_ICO '_'
#define grass_in_map 'v'


using std::vector;
using std::string;
///////////////////////////////////

extern int dead;

extern HANDLE OutHandle;

extern bool SnackColor;//蛇彩色控制
extern bool SnackColor1;//蛇红黄控制

/***************音乐****************/
extern char temp[255];

extern char c_map[51][102];

extern int addpoint;

/////////////////////////////////
//函数声明8

//打印菜单
void printMemu();

//随机数获取
int RandNum(int);

//初始化
void _Init();

//窗口任意位置输出
void printIn(int x, int y, WORD color =0xf0);

//打印边框
void prBox();

//打印右边数据
void prPoint();

//存档
void SaveLoad();

//读取存档
void LoadSave();

//方向      xy修改哪个   修改多少
void move(/*bool,*/ char, int);

//按键判断
void Keyin(char cInput);

//游戏设置
void setting();

/*
 *在任意位置打印‘字符’
 */
void printSomeIn(int x, int y, string a, WORD color= 0xf0);

//获取地图
void get_map();

//初始化地图
void init_map();

//保存地图
void save_map();

//读取蛇位置存档
void read_all();

//暂停并存档
void save_all();

//封面
void Cover();

//加分
void Points(int);