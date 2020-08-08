#include "Func.h"
#include "Food.h"
extern bool bUp, bDown, bLeft, bRight;
extern bool b1Up, b1Down, b1Left, b1Right;

void Keyin(char cInput)
{
		//Snack1X[0] = Snack1Y[0] = 0;
		switch (cInput)
		{
		case'w': {move(/*bUp, */ 'y', -1); break; }
		case's': {move(/*bDown,*/ 'y', 1); break; }
		case'a': {move(/*bLeft,*/'x', -1); break; }
		case'd': {move(/*bRight,*/'x', 1); break; }

		case'+': {nSleepTime -= 5; break; }
		case'-': {if (nSleepTime >= 100)break;
				 else nSleepTime += 5; break; }
		case'[': {uplong -= 10; break; }
		case']': {uplong += 10; break; }
		case 'p': {save_all(); break; }
		}
}