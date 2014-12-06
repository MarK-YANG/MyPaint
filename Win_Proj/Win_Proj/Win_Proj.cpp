// Win_Proj.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win_Proj.h"
#include "CShape.h"
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100
#define TIME_CLOCK 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//My declaration of functions
void drawLED(HDC hdc, int x, int y, double length, double width, int num);
void rePaint(HDC hdc, HWND hWnd);
int mySelect(POINT pt);
void printCMD(HDC hdc, LPCTSTR text);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN_PROJ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN_PROJ));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN_PROJ));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN_PROJ);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//myArgs
vector<CShape*> vShape;
POINT ptStart;
POINT ptPre;
COLORREF myRGB = RGB(0, 0, 0);
CShape*  currentShape = NULL;
CShape*  preShape = new CRect({ 0, 0 }, { 0, 0 }, RGB(255, 255, 255));
int iSelectObject = 0;
bool isDragging = false;
bool isDrawing = false;
int drawAction = 0;
bool isShift = false;
LPCTSTR textCMD = TEXT("[MyPaint$]   Hello :) !");

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	

	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(hWnd, TIME_CLOCK, 1000, NULL);
	}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		
		rePaint(hdc, hWnd);
	
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		ptStart = pt;
		
		switch (mySelect(pt))
		{
		case 1:
		{
			textCMD = TEXT("[MyPaint$] Draw a Rectangle !");
			printCMD(hdc, textCMD);
			drawAction = 1;
		}
			break;
		case 2:
		{
			textCMD = TEXT("[MyPaint$] Draw a Ellipse !");
			printCMD(hdc, textCMD);
			drawAction = 2;
		}
			break;
		case 8:
		{
			textCMD = TEXT("[MyPaint$] Draw a Line !");
			printCMD(hdc, textCMD);
			drawAction = 3;
		}
			break;
		case 3:
		{
			textCMD = TEXT("[MyPaint$] Use Black !");
			printCMD(hdc, textCMD);
			myRGB = RGB(0, 0, 0);
		}
			break;
		case 4:
		{
			textCMD = TEXT("[MyPaint$] Use Red !");
			printCMD(hdc, textCMD);
			myRGB = RGB(255, 0, 0);
		}
			break;
		case 5:
		{
			textCMD = TEXT("[MyPaint$] Use Green !");
			printCMD(hdc, textCMD);
			myRGB = RGB(0, 255, 0);
		}
			break;
		case 6:
		{
			textCMD = TEXT("[MyPaint$] Use Blue !");
			printCMD(hdc, textCMD);
			myRGB = RGB(0, 0, 255);
		}
			break;
		case 7:
		{
			textCMD = TEXT("[MyPaint$] Moving !");
			printCMD(hdc, textCMD);
			drawAction = 5;
		}
			break;
		case -1:
		{
			if (drawAction == 5)
			{
				if (DragDetect(hWnd, pt))
				{
					for (int i = 0; i < vShape.size(); i++)
					{
						if (vShape[i]->judge(pt))
						{
							isDragging = true;
							textCMD = TEXT("[MyPaint$] Is Moving !");
							printCMD(hdc, textCMD);
							iSelectObject = i;
							ptPre = pt;
							break;
						}
					}
				}
				else
				{
					textCMD = TEXT("[MyPaint$]  Not Moving !");
					printCMD(hdc, textCMD);
				}
			}
			if (drawAction != 5 && drawAction != 0)
			{
				if (DragDetect(hWnd, pt))
				{
					isDrawing = true;
					ptStart = pt;
					textCMD = TEXT("[MyPaint$] Drawing !");
					printCMD(hdc, textCMD);
				}
				else
				{
					textCMD = TEXT("[MyPaint$] Not Drawing !");
					printCMD(hdc, textCMD);
				}
			}

		}
			break;
		default:
			textCMD = TEXT("[MyPaint$] Error :X !");
			printCMD(hdc, textCMD);
			break;
		}

		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_LBUTTONUP:
	{
		hdc = GetDC(hWnd);
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };

		if (isDrawing)
		{
			vShape.push_back(currentShape);
		}

		isDrawing = false;
		isDragging = false;
		isShift = false;
		
		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hWnd);
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };

		if (isDrawing)
		{
			if (drawAction == 1)
			{
				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					preShape->draw(hdc, true);

					rePaint(hdc, hWnd);

					currentShape = new CSquare(ptStart, pt, myRGB);
					currentShape->draw(hdc, false);
					textCMD = TEXT("[MyPaint$] Draw a Suqare !");
					printCMD(hdc, textCMD);
					preShape = currentShape;

				}
				else
				{
					preShape->draw(hdc, true);

					rePaint(hdc, hWnd);

					currentShape = new CRect(ptStart, pt, myRGB);
					currentShape->draw(hdc, false);
					textCMD = TEXT("[MyPaint$] Draw a Rectangle !");
					printCMD(hdc, textCMD);
					preShape = currentShape;
				}
			}
			if (drawAction == 2)
			{
				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					preShape->draw(hdc, true);

					rePaint(hdc, hWnd);

					currentShape = new CCircle(ptStart, pt, myRGB);
					currentShape->draw(hdc, false);
					textCMD = TEXT("[MyPaint$] Draw a Circle !");
					printCMD(hdc, textCMD);
					preShape = currentShape;
				}
				else
				{
					preShape->draw(hdc, true);

					rePaint(hdc, hWnd);

					currentShape = new CEllipse(ptStart, pt, myRGB);
					currentShape->draw(hdc, false);
					textCMD = TEXT("[MyPaint$] Draw a Ellipse !");
					printCMD(hdc, textCMD);
					preShape = currentShape;
				}
			}

			if (drawAction == 3)
			{
				preShape->draw(hdc, true);
				rePaint(hdc, hWnd);
				currentShape = new CLine(ptStart, pt, myRGB);
				currentShape->draw(hdc, false);
				textCMD = TEXT("[MyPaint$] Draw a Line !");
				printCMD(hdc, textCMD);
				preShape = currentShape;
			}

		}

		if (isDragging)
		{
			int deltaX = pt.x - ptPre.x;
			int deltaY = pt.y - ptPre.y;
			rePaint(hdc, hWnd);
			vShape[iSelectObject]->move(hdc, deltaX, deltaY);
			ptPre = pt;
		}


		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_RBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		textCMD = TEXT("[MyPaint$] Clear =¡£= !");
		printCMD(hdc, textCMD);
		for (int  i = 0; i <vShape.size(); i++)
		{
			vShape[i]->draw(hdc, true);
		}

		vShape.clear();
		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_RBUTTONUP:
	{
		hdc = GetDC(hWnd);
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };

		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_TIMER:
	{
		hdc = GetDC(hWnd);
		
		//SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(143, 165, 33)));
		/*calculate points*/
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		int clientX = clientRect.right;
		int space = 15;
		double length = 10;
		double width = 7.5;
		POINT pt[6] = {};
		RECT rect1, rect2, rect3, rect4;
		POINT firstPt = { clientX - 2 * space, 10 };
		pt[5] = firstPt;
		pt[4] = { pt[5].x - space, firstPt.y };
		pt[3] = { pt[4].x - space - space / 3, firstPt.y };
		pt[2] = { pt[3].x - space, firstPt.y };
		pt[1] = { pt[2].x - space - space / 3, firstPt.y };
		pt[0] = { pt[1].x - space, firstPt.y };

		rect1.left = pt[4].x - (space + space / 3 - width) / 2 - 1;
		rect1.top = firstPt.y + length / 2 - 1;
		rect1.right = pt[4].x - (space + space / 3 - width) / 2 + 1;
		rect1.bottom = firstPt.y + length / 2 + 1;
		rect2 = rect3 = rect4 = rect1;
		OffsetRect(&rect2, 0, length);
		OffsetRect(&rect3, -space * 2 - space / 3, 0);
		OffsetRect(&rect4, -space * 2 - space / 3, length);


		HPEN hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		HPEN hPenGreen = CreatePen(PS_SOLID, 1, RGB(143, 165, 33));
		HBRUSH hBrushGreen = CreateSolidBrush(RGB(143, 165, 33));
		HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));

		/*paint the previous LEDs white*/
		SelectObject(hdc, hBrushWhite);
		SelectObject(hdc, hPenWhite);
		drawLED(hdc, pt[0].x, pt[0].y, length, width, 8);
		drawLED(hdc, pt[1].x, pt[1].y, length, width, 8);
		drawLED(hdc, pt[2].x, pt[2].y, length, width, 8);
		drawLED(hdc, pt[3].x, pt[3].y, length, width, 8);
		drawLED(hdc, pt[4].x, pt[4].y, length, width, 8);
		drawLED(hdc, pt[5].x, pt[5].y, length, width, 8);



		SYSTEMTIME time;
		GetLocalTime(&time);

		/*paint the current LEDs red*/
		SelectObject(hdc, hBrushGreen);
		SelectObject(hdc, hPenGreen);
		drawLED(hdc, pt[0].x, pt[0].y, length, width, time.wHour/10);
		drawLED(hdc, pt[1].x, pt[1].y, length, width, time.wHour%10);
		drawLED(hdc, pt[2].x, pt[2].y, length, width, time.wMinute/10);
		drawLED(hdc, pt[3].x, pt[3].y, length, width, time.wMinute%10);
		drawLED(hdc, pt[4].x, pt[4].y, length, width, time.wSecond/10);
		drawLED(hdc, pt[5].x, pt[5].y, length, width, time.wSecond%10);

		Ellipse(hdc, rect1.left, rect1.top, rect1.right, rect1.bottom);
		Ellipse(hdc, rect2.left, rect2.top, rect2.right, rect2.bottom);
		Ellipse(hdc, rect3.left, rect3.top, rect3.right, rect3.bottom);
		Ellipse(hdc, rect4.left, rect4.top, rect4.right, rect4.bottom);

		Sleep(500);

		SelectObject(hdc, hBrushWhite);
		SelectObject(hdc, hPenWhite);
		Ellipse(hdc, rect1.left, rect1.top, rect1.right, rect1.bottom);
		Ellipse(hdc, rect2.left, rect2.top, rect2.right, rect2.bottom);
		Ellipse(hdc, rect3.left, rect3.top, rect3.right, rect3.bottom);
		Ellipse(hdc, rect4.left, rect4.top, rect4.right, rect4.bottom);

		ReleaseDC(hWnd, hdc);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//drawLED function
void  drawLED(HDC hdc, int x, int y, double length, double width, int num)
{
	POINT pt[6] = { x, y, x + width, y, x, y + length, x + width, y + length, x, y + 2 * length, x + width, y + 2 * length };
	POINT array[10] = {};
	int iArrayCount = 0;

	switch (num)
	{
	case 1:
		array[0] = pt[1];
		array[1] = pt[3];
		array[2] = pt[5];
		iArrayCount = 3;
		break;
	case 2:
		array[0] = pt[0];
		array[1] = pt[1];
		array[2] = pt[3];
		array[3] = pt[2];
		array[4] = pt[4];
		array[5] = pt[5];
		iArrayCount = 6;
		break;
	case 3:
		array[0] = pt[0];
		array[1] = pt[1];
		array[2] = pt[3];
		array[3] = pt[2];
		array[4] = pt[3];
		array[5] = pt[5];
		array[6] = pt[4];
		iArrayCount = 7;
		break;
	case 4:
		array[0] = pt[0];
		array[1] = pt[2];
		array[2] = pt[3];
		array[3] = pt[1];
		array[4] = pt[3];
		array[5] = pt[5];
		iArrayCount = 6;
		break;
	case 5:
		array[0] = pt[1];
		array[1] = pt[0];
		array[2] = pt[2];
		array[3] = pt[3];
		array[4] = pt[5];
		array[5] = pt[4];
		iArrayCount = 6;
		break;
	case 6:
		array[0] = pt[1];
		array[1] = pt[0];
		array[2] = pt[2];
		array[3] = pt[4];
		array[4] = pt[5];
		array[5] = pt[3];
		array[6] = pt[2];
		iArrayCount = 7;
		break;
	case 7:
		array[0] = pt[0];
		array[1] = pt[1];
		array[2] = pt[3];
		array[3] = pt[5];
		iArrayCount = 4;
		break;
	case 8:
		array[0] = pt[1];
		array[1] = pt[0];
		array[2] = pt[2];
		array[3] = pt[4];
		array[4] = pt[5];
		array[5] = pt[3];
		array[6] = pt[1];
		array[7] = pt[0];
		array[8] = pt[2];
		array[9] = pt[3];
		iArrayCount = 10;
		break;
	case 9:
		array[0] = pt[3];
		array[1] = pt[2];
		array[2] = pt[0];
		array[3] = pt[1];
		array[4] = pt[3];
		array[5] = pt[5];
		array[6] = pt[4];
		iArrayCount = 7;
		break;
	case 0:
		array[0] = pt[1];
		array[1] = pt[0];
		array[2] = pt[2];
		array[3] = pt[4];
		array[4] = pt[5];
		array[5] = pt[3];
		array[6] = pt[1];
		iArrayCount = 7;
		break;

	default:
		break;
	}
	Polyline(hdc, array, iArrayCount);

}


void rePaint(HDC hdc, HWND	hWnd)
{
	/*seperate line*/
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);
	MoveToEx(hdc, 0, 40, NULL);
	SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(223, 23, 178)));
	LineTo(hdc, rectClient.right, 40);

	/*command rectangle*/
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(32, 23, 224)));
	Rectangle(hdc, 25, 10, 55, 30);

	/*command ellipse*/
	SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 223, 255)));
	Ellipse(hdc, 75, 10, 105, 30);

	/*command line*/
	SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(231, 179, 37)));
	MoveToEx(hdc, 125, 10, NULL);
	LineTo(hdc, 155, 30);

	/*seperate line*/
	SelectObject(hdc, CreatePen(PS_DOT, 1, RGB(223, 23, 178)));
	MoveToEx(hdc, 200, 0, NULL);
	LineTo(hdc, 200, 40);
	MoveToEx(hdc, 300, 0, NULL);
	LineTo(hdc, 300, 40);
	MoveToEx(hdc, rectClient.right - 150, 0, NULL);
	LineTo(hdc, rectClient.right - 150, 40);
	MoveToEx(hdc, 530, 0, NULL);
	LineTo(hdc, 530, 40);

	/*move command*/
	WCHAR temp[100];
	int len = wsprintf(temp, TEXT("Move"));
	TextOut(hdc, 235, 13, temp, len);


	/*color balck*/
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	Ellipse(hdc, 325, 10, 345, 30);

	/*color red*/
	SelectObject(hdc, CreateSolidBrush(RGB(255, 0 ,0)));
	SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
	Ellipse(hdc, 370, 10, 390, 30);

	/*color green*/
	SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
	SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));
	Ellipse(hdc, 415, 10, 435, 30);

	/*color blue*/
	SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
	SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 255)));
	Ellipse(hdc, 460, 10, 480, 30);

	/*repaint shapes*/
	for (int i = 0; i < vShape.size(); i++)
	{
		vShape[i]->draw(hdc, false);
	}

	/*repaint command*/
	printCMD(hdc, textCMD);
}

int mySelect(POINT pt)
{
	if (pt.x > 25 && pt.x < 55 && pt.y > 10 && pt.y < 30)
	{
		return 1;
	}
	else if (pt.x > 75 && pt.x < 105 && pt.y > 10 && pt.y < 30)
	{
		return 2;
	}
	else if (pt.x > 325 && pt.x < 345 && pt.y > 10 && pt.y < 30)
	{
		return 3;
	}
	else if (pt.x > 370 && pt.x < 390 && pt.y > 10 && pt.y < 30)
	{
		return 4;
	}
	else if (pt.x > 415 && pt.x < 435 && pt.y > 10 && pt.y < 30)
	{
		return 5;
	}
	else if (pt.x > 460 && pt.x < 480 && pt.y > 10 && pt.y < 30)
	{
		return 6;
	}
	else if (pt.x > 230 && pt.x < 280 && pt.y > 10 && pt.y < 30)
	{
		return 7;
	}
	else if (pt.x > 125 && pt.x < 155 && pt.y > 10 && pt.y < 30)
	{
		return 8;
	}
	else
	{
		return -1;
	}
}

void printCMD(HDC hdc, LPCTSTR text)
{
	WCHAR temp[100];
	int len = wsprintf(temp, TEXT("                                                             "));
	TextOut(hdc, 550, 13, temp, len);
	len = wsprintf(temp, text);
	TextOut(hdc, 550, 13, temp, len);
}