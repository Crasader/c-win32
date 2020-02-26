// PaintDesktopDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <vector>


#pragma comment(lib, "gdiplus.lib")
int main()
{
	HWND hDesktopHwnd = GetDesktopWindow();
	HDC hdc = GetDC(hDesktopHwnd);

	if (hdc != NULL)
	{
		unsigned Screen_X = GetDeviceCaps(hdc, HORZRES);
		unsigned Screen_Y = GetDeviceCaps(hdc, VERTRES);
		
		POINT pos;
		pos.x= Screen_X;
		pos.y = Screen_Y;
		HPEN Pen = CreatePen(PS_SOLID, 5.0f, RGB(40, 86, 200));
		SelectObject(hdc, Pen);

		while (true)
		{
			for (int i=0;i<(Screen_Y /100)+1;i++)
			{
				MoveToEx(hdc, 0, i * 100, NULL);
				LineTo(hdc, Screen_X, i * 100);
			}
			for (int i=0;i<(Screen_X/100)+1;i++)
			{
				MoveToEx(hdc, i * 100, 0, NULL);
				LineTo(hdc, i * 100, Screen_Y);
			}
			
		}
	}
    return 0;
}

