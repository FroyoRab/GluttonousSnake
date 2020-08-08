#include "Func.h"
#include "someclass.h"
#define WINDOW_X 49
#define WINDOW_Y 49
////////////////////////////////


//是否死亡
int dead = 0;
int dead1 = 0;

//控制台句柄
HANDLE OutHandle;

extern _grass grass_land;
///////////////////////////////

int main()
{
	system("color f0");
	/***************音乐****************/
	mciSendString(("open sound\\back.mp3 alias MUSIC"), temp, 255, 0);

	//获取窗口标准输出句柄,用于窗口坐标打印设置
	OutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	//设置窗口大小
	char *ptemp = new char[40];
	sprintf_s(ptemp, 40, "mode con:cols=%d lines=%d",
							WINDOW_X + 100, WINDOW_Y + 2);

	system("title Snackkkkkkkkkkkkk");
	//设置光标隐藏
	CONSOLE_CURSOR_INFO CURSORINFO;

	//在控制台句柄获取光标
	GetConsoleCursorInfo(OutHandle, &CURSORINFO);

	//设置光标信息
	CURSORINFO.bVisible = false;

	//设置光标
	SetConsoleCursorInfo(OutHandle, &CURSORINFO);
	system("mode con:cols=128 lines=50");
	printIn(0, 0, 0xf3);
	
	Cover();//封面

	//获取随机数
	srand((unsigned int)time(0));
	PlaySoundA("sound\\startmanu.wav", NULL, SND_ASYNC | SND_NODEFAULT);
	system(ptemp);
	delete(ptemp);
	ptemp = NULL;
	while (true)
	{
		//读取存档
		LoadSave();
		prBox();
		printMemu();
		char cInput1 = _getch();//获取菜单输入
		switch (cInput1)
		{
		case '1'://开始游戏
		{
			PlaySoundA("sound\\click.wav", 
				NULL, SND_ASYNC | SND_NODEFAULT);
			system("cls");
			_Init();	//包括边框打印，死亡判断初始
						//蛇头随机并打印，长度初始，
						//食物随机并打印，横纵速度对应
			while (1)	//进入游戏
			{
				//prPoint();		//打印右边分数
				if (dead){//判断死亡宣告
					mciSendString("stop MUSIC", NULL, 0, 0);
					PlaySoundA("sound\\dead.wav",
						NULL, SND_ASYNC | SND_NODEFAULT);
					SaveLoad();
					break; 
				}
				char cInput = _getch();
				Keyin(cInput);	//按键判断及响应
								//每移动一格或者按键都会出来一次
								//重新根据bUp/down/等判断方向
			}
			//在死亡判定时判断if (need_save)SaveLoad();//一旦游戏结束，判断是否需要存档
			break;
		}
		case '2': {
			setting();//游戏设置
			PlaySoundA("sound\\click.wav",
				NULL, SND_ASYNC | SND_NODEFAULT);
			break; 
		}
		case'3': {
			system("cls");
			get_map();//获取地图并且直接打印
			PlaySoundA("sound\\click.wav",
				NULL, SND_ASYNC | SND_NODEFAULT);
			system("pause");
			break;
		}
		case'4': {
			PlaySoundA("sound\\click.wav",
			NULL, SND_ASYNC | SND_NODEFAULT);
			init_map();//重置地图
			system("pause");
			break; 
		}
		case'5':system("map.txt"); break;//编辑地图，直接cmd命令打开map.txt
		case'6':read_all(); break;
		case'7':exit(0);//退出
		default:break;
		}
	}
}

//打印菜单
void printMemu()
{
	printIn((WINDOW_X / 2) - 2, 19);
	printf("%10s", "①.开始游戏\n");
	printIn((WINDOW_X / 2) - 2, 21);
	printf("%10s", "②.游戏设置\n");
	printIn((WINDOW_X / 2) - 2, 23);
	printf("%10s", "③.查看当前地图\n");
	printIn((WINDOW_X / 2) - 2, 25);
	printf("%10s", "④.重置地图\n");
	printIn((WINDOW_X / 2) - 2, 27);
	printf("%10s", "⑤.编辑地图\n");
	printIn((WINDOW_X / 2) - 2, 29);
	printf("%10s", "⑥.读取存档\n");
	printIn((WINDOW_X / 2) - 3, 31);
	printf("%10s", "⑦.退出\n");
	printIn(0, 0);

	//打印右边分数
	prPoint();
}

//打印封面
void Cover() {
	//封面
	std::cout<<"\n\n\n\n\n\n\n\n                                                                                                                               '\n                                                                                      .';;'                                    '\n                                                                                ;%;'|&@@@@&;. :&@$:                            '\n                                                                             '$|:%@@@@@@@@@@&; `%@@@@$:                        '\n                                                                           .|&!|@@@&|';&@@@@@&: !@@@@@@@%`                     '\n                                                                          .:::%@@%|&@||@@@@@@@;   `%@@@@@@%`                   '\n                                                                        .|@%!$&&%`'%|!&@@||@@&:.|@|` '$@@@@@!                  '\n                                                                       ;&&%$@@@@@@@@@@$:.;@@@! :&@@@%` ;@@@@@|.                '\n                                                                     `$@@&&&&&@@&;.    !&@@&: :&@@@@@&; ;&@@@@|.               '\n                                                                    .:%&&&&@@@|.   :$@@@&!.    '$@@@@@&: !@@@@@;               '\n                                                                     ::  .'!!'   :&&|`           !@@@@@$''$@@@@|.              '\n                                                                       ;%` `%;                   .|@@@@@;`%@@@@%`              '\n                                                                      !%`            ..           :&@@@@|'|@@@@%`              '\n                           `;%$@@@@@@&$|;'.                      '%@@&;      .!&@@@@@@$|;;;;'     '&@@@@|'|@@@@|.              '\n                       :&@@@@@@@@@@@@@@@@@@@@@@&|'                 :;     :&@@@@@|`               !@@@@@;'$@@@&:               '\n                     !@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&!.               :$@@@@&;                  :&@@@@%`;@@@@!                '\n                 :''$@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$:        '$@@@@@|.                   ;&@@@@$''$@@@|.                '\n               `|:`%@@@@@@@%'            .:%&@@@@@@@@@&@@@@@@&;  `%@@@@@@!                    '$@@@@@%``%@@@!                  '\n              `||.:$@@@@@&:                     :%@@@@@@@@@$' `%@@@@@@&;                   :$@@@@@@$' '$@@$'                   '\n              :$|.`%@@@@@!                           :%$!. `%@@@@@@@&:   :!;;:'''':;!|$@@@@@@@@@$:   !@@&:                     '\n              '$&: :&@@@@%`                             :$@@@@@@@@%` !&@&|`  .:|$@@@@@@@@@&%;.     !@@%`                       '\n               !@$: `|@@@@&:                        .!&@@@@@@@@&;  !@@@@@@@@@@&%!;:'''':;!%$@&;.'$&!.                          '\n                ;@@%` .!&@@@@&!`                .;&@@@@@@@@@@|.    .;$@@@@@@@@@@@@@@@@@@@&|`.:;'                               '\n                 '$@@&!.    `'::::'`       .:%@@@@@@@@@@@&!.              `:!|%%%%|!:`                                         '\n                   `%@@@@@&%|;:::;;!|%$&&@@@@@@@@@@@@@$:                                                                       '\n                      `|@@@@@@@@@@&&&@@@@@@@@@@@@@%:                                                                           '\n                           `!%&&&$&@@@@@@@@&%;`                                                                                '\n                                                                                                                               '\n";
	while(1) {
		//闪烁封面下面的开始游戏
		printSomeIn(WINDOW_X / 2 + 2, WINDOW_Y / 2 + 15, 
			"开始游戏  请按任意键继续", 0x8f);
		if (_kbhit())break;
		Sleep(500);
		printSomeIn(WINDOW_X / 2 + 2, WINDOW_Y / 2 + 15, 
			"                        ", 0xf8);
		if (_kbhit())break;
		Sleep(200);
	}
}