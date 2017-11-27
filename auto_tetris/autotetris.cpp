// autotetris.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "autotetris.h"
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AUTOTETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOTETRIS));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOTETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUTOTETRIS); 
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_BORDER | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
VOID CALLBACK TimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// 定时器
	checkTetris(hWnd,VK_DOWN);
}
// 函数： 启动游戏
VOID InitGame(HWND hWnd)
{
	// 初始化参数
	btCurGrid.init(PAINT_YNODE,GAME_XNODE);
	iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
	iCurDif = SIMPLE;
	iCurScore = 0;
	// 生成首个下落图形
	curTetris.random();
	// 生成下一个下落图形
	nextTetris.random();
	// 启动定时器
	KillTimer(hWnd, TIMER_ID);

	SetTimer(hWnd, TIMER_ID, iInterval, TimeProc);

}

// 函数： 初始化窗体的位置和大小
VOID InitWndPostionSize(HWND hWnd)
{
	RECT rect;
	int nWinX, nWinY, nClientX, nClientY;
	//获取窗口大小  
	GetWindowRect(hWnd, &rect);
	nWinX = rect.right - rect.left;
	nWinY = rect.bottom - rect.top;
	//获取客户区大小  
	GetClientRect(hWnd, &rect);
	nClientX = rect.right - rect.left;
	nClientY = rect.bottom - rect.top;

	// 获取当前屏幕的宽高，将窗口放置中心区域
	UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
	// 调整窗口位置与大小
	MoveWindow(hWnd, (iScreenX - PAINT_WIDTH)/2, (iScreenY - PAINT_HEIGHT)/3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}

// 函数： 绘制背景
VOID DrawBackGround(HDC hdc)
{
	HPEN hPenNull = (HPEN)GetStockObject(NULL_PEN);         // HPEN NULL
	HBRUSH hBrushGray = (HBRUSH)GetStockObject(GRAY_BRUSH); // hBRUSH GRAY
	SelectObject(hdc,hPenNull);
	SelectObject(hdc, hBrushGray); // 背景颜色	
	// 游戏区域大小 (HDC,左上角X,左上角Y，右下角X，右下角Y)
	Rectangle(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY, GAME_POSITION.iRightX, GAME_POSITION.iRightY);
	// 信息区大小
	Rectangle(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY, INFO_POSITION.iRightX, INFO_POSITION.iRightY);
	DeleteObject(hBrushGray);
	DeleteObject(hPenNull);

}
// 函数： 绘制信息栏
VOID DrawInfo(HDC hdc)
{
	// 下一个下落方块
	RECT rect;
	HFONT hFont = CreateFont(23, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"微软雅黑");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255,255,0));
	
	NEXT_FONT_POSITION.setRect(rect);
	DrawText(hdc, TEXT("下一个"), _tcslen(TEXT("下一个")), &rect , 0);

	LEVEL_FONT_POSITION.setRect(rect);
	TCHAR tcDif[10];
	LPCTSTR lpctDif = TEXT("难度: %d");
	wsprintf(tcDif, lpctDif , iCurDif);
	DrawText(hdc, tcDif, _tcslen(tcDif), &rect , 0);

	SCORE_FONT_POSITION.setRect(rect);
	TCHAR tcScore[10];
	LPCTSTR lpctScore = TEXT("得分: %d");
	wsprintf(tcScore, lpctScore, iCurScore);
	DrawText(hdc, tcScore, _tcslen(tcScore), &rect , 0);

	DeleteObject(hFont);
}
// 根据坐标获取当前节点位置
RectPosition getRectPosition(BYTE x, BYTE y)
{
	RectPosition rectPs;
	rectPs.iLeftX = BLOCK_SIZE + y*NODE_SIZE;
	rectPs.iLeftY = BLOCK_SIZE + x*NODE_SIZE;
	rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
	rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
	return rectPs;
}
// 函数：绘制方块
VOID DrawTetris(HDC hdc)
{
	RectPosition ps;
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc, hPen);
	// 绘制Game区域
	for (int i =0; i< PAINT_YNODE; i++)
	{
		for (int j =0; j< GAME_XNODE; j++)
		{
			if (btCurGrid[i][j] > 0)
			{
				// 绘制当前节点
				SelectObject(hdc, getHbrush(btCurGrid[i][j]));
				ps = getRectPosition(i, j);
				Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
			}
		}
	}
	// 绘制正在下落图形
	SelectObject(hdc, getHbrush(curTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(curTetris.positions[i].x , curTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
	}
	// 绘制下一次下落图形
	int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
	int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
	SelectObject(hdc, getHbrush(nextTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(nextTetris.positions[i].x , nextTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX+ move_x, ps.iRightY +move_y);
	}
}
// 调整游戏难度
VOID setLevel(HWND hWnd)
{
	BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore >= GENERAL_SCORE;
	BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore >= DIFFICULT_SCORE;
	BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore >= DIFFICULTEST_SCORE;
	
	// 设置难度
	if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
	{
		iCurDif ++;
		iInterval -= 250;
		KillTimer(hWnd , TIMER_ID);
		SetTimer(hWnd , TIMER_ID, iInterval , TimeProc);
	}
}
// 下落完成
VOID finishDown(HWND hWnd)
{
	// 更新btCurGrid当前界面数组
	btCurGrid.addCurTerisToGrid(curTetris);
	// 更新下落方块，重新生成nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// 消除
	iCurScore += btCurGrid.clearLine();
	// 判断生成位置是否被占用
	if (btCurGrid.isExist(curTetris))
	{
		// TODO 游戏结束
		KillTimer(hWnd, TIMER_ID);
		MessageBox(hWnd, TEXT("游戏结束"),TEXT("消息"),MB_OK);
		return;
	}
	// 调整游戏难度(ai模式不需要)
	if (!AI_SWITCH)
	{
		setLevel(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE);
		// 如果为AI模式，继续计算
		AIHandle(hWnd);
	}
}

// 检查图形下落是否越界/完成 / 消除
VOID checkTetris(HWND hWnd, INT type)
{
	int maxX , minX , maxY , minY;
	Tetris temp = curTetris;
	switch (type)
	{
		case VK_LEFT: // 左
			temp.left();
			break;
		case VK_RIGHT: // 右
			temp.right();
			break;
		case VK_UP: // 上
			temp.rotate();
			break;
		case VK_DOWN: // 下(产生快速下降效果)
			temp.down(); 
			break;
	}
	temp.getMaxMin(minX , maxX , minY , maxY);

	// 判断是否越界
	if (maxX > 19)
	{
		// 触碰底部，下落结束
		finishDown(hWnd);
		return;
	}
	if (minX < 0 || maxY > 9 || minY < 0)
	{
		// 触碰左右边界
		return;
	}
	// 判断位置是否被占用
	if (btCurGrid.isExist(temp))
	{
		if (type == VK_DOWN)
		{
			// 下落到已存在节点上方，下落结束
			finishDown(hWnd);
		}
		return;
	}
	// 操作允许
	curTetris = temp;
	InvalidateRect(hWnd, NULL, TRUE);
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			// 初始化窗体的位置和大小
			InitWndPostionSize(hWnd);
			// 启动游戏
			InitGame(hWnd);
		}
		break;
	case WM_KEYDOWN:
		{
			if (!AI_SWITCH) // 在自动游戏模式下 键盘不可用
			{
				checkTetris(hWnd,wParam);
			}
		}
		break;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
			case IDM_COMMON:
				AI_SWITCH = false;
				InitGame(hWnd);
				break;
			case IDM_AI:
				AI_SWITCH = true;
				InitGame(hWnd);
				AIHandle(hWnd);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			// 绘制背景
			DrawBackGround(hdc);
			// 绘制信息栏
			DrawInfo(hdc);
			// 绘制图形
			DrawTetris(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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

// 函数：启动AI模式
VOID AIHandle(HWND hWnd)
{
	// Pierre Dellacherie 获取最佳落脚点
	getBestPosition();
	// 自动移动到最佳位置并结束下落
	aiMoveTetris(hWnd);
}
// 获取最佳评分及其位置
VOID getBestPosition()
{
	bestScore = 0x80000000;
	int maxX , minX , maxY , minY;
	algoTetris = curTetris;	
	Tetris temp;
	//获取网格水平面
	getHorizonIndex(); 
	// 图形有四种状态
	for (int i = 0 ; i < 4 ; i ++)
	{
		algoTetris.getMaxMin(minX , maxX , minY , maxY);
		int num = 10 - (maxY - minY); // 每次变换产生num种情况
		for (int j = 0 ; j < num ; j++)
		{
			// 还原网格
			algoGrid = btCurGrid; 
			temp = algoTetris;
			// 将base_tetris放到algoGrid的指定位置中, 返回需要向左移动次数
			int moveLeft = putTetrisToGrid(j);
			// 评分
			int score = Dellacherie();
			if (score > bestScore)
			{
				bestScore = score;
				bestTetris = algoTetris;
				needRotate = i;
				needMoveLeft = moveLeft;
			}
			// 还原 algoTetri
			algoTetris = temp;
		}
		algoTetris.rotate();
	}
}
VOID CALLBACK AITimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	if (needRotate)
	{
		// 先变形
		needRotate--;
		curTetris.rotate();
	} else if (needMoveLeft > 0)
	{
		// 左移动
		curTetris.left();
		needMoveLeft--;
	}
	else if(needMoveLeft < 0)
	{
		// 右移动
		curTetris.right();
		needMoveLeft++;
	}
	// 重新绘制界面
	InvalidateRect(hWnd, NULL, TRUE);
	// 结束定时器
	if (needRotate == 0 && needMoveLeft == 0)
	{
		KillTimer(hWnd, AI_TIMER_ID);
	}
}
// 快速测试用定时器
VOID CALLBACK TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// 更新btCurGrid当前界面数组
	btCurGrid.addCurTerisToGrid(bestTetris);
	// 更新下落方块，重新生成nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// 消除
	iCurScore += btCurGrid.clearLine();
	// 判断生成位置是否被占用
	if (btCurGrid.isExist(curTetris))
	{
		// TODO 游戏结束
		KillTimer(hWnd, 3);
		MessageBox(hWnd, TEXT("游戏结束"),TEXT("消息"),MB_OK);
		return;
	}
	// 如果为AI模式，继续计算
	AIHandle(hWnd);
}
// 将方块移动到最佳位置
VOID aiMoveTetris(HWND hWnd)
{
	SetTimer(hWnd, AI_TIMER_ID, iInterval / 5 ,AITimeProc);
	// SetTimer(hWnd, 3, 0 ,TESTTimeProc); // 快速测试使用
}
// 获取网格水平线
VOID getHorizonIndex()
{
	for (int i = 0 ; i < GAME_XNODE; i ++)
	{
		for(int j = 0 ; j < PAINT_YNODE -1;j++)
		{
			if (btCurGrid[j+1][i])
			{
				horizonIndex[i] = j;
				break;
			}
			if (j == (PAINT_YNODE -2))
			{
				horizonIndex[i] = j + 1;
			}
		}
	}

}
INT putTetrisToGrid(int col)
{
	int left_bottom_index = algoTetris.getLeftBottom(); // 获取最左下角的元素下标
	
	int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // 需要向下移动的距离
	int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // 需要向左移动的距离
	// 调整向下移动距离
	for (int j = 0 ; j < 4 ; j ++)
	{
		if (j != left_bottom_index)
		{
			int colDistance = algoTetris.positions[j].y - algoTetris.positions[left_bottom_index].y;
			int rowDistance = horizonIndex[col + colDistance] - algoTetris.positions[j].x;
			if (rowDistance < maxDistanceRow)
			{
				maxDistanceRow = rowDistance;
			}
		}
	}
	// 将algoTetris移动到指定位位置
	for(int j = 0 ; j < 4 ; j ++)
	{
		algoTetris.positions[j].x += maxDistanceRow;
		algoTetris.positions[j].y -= maxDistanceCol;
		algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
	}
	return maxDistanceCol;
}
// AI评分算法
INT Dellacherie()
{
	INT lh_score, re_score, rt_score, ct_score, nh_score, ws_score;
	lh_score = getLH();
	re_score = getRE();
	rt_score = getRT();
	ct_score = getCT();
	nh_score = getNH();
	ws_score = getWS();
	return lh_score * LH_WEIGHT + re_score * RE_WEIGHT + rt_score * RT_WEIGHT + ct_score * CT_WEIGHT + nh_score * NH_WEIGHT + ws_score * WS_WEIGHT;
}
// 高度评分
INT getLH() 
{
	int maxX, minX, maxY, minY;
	algoTetris.getMaxMin(minX, maxX, minY, maxY);
	return PAINT_YNODE - maxX;
} 
// 消除行*贡献方块数评分
INT getRE() 
{	
	int  status , clearline = 0, cells = 0;
	
	for (int i = PAINT_YNODE -1; i >= 0 ; i--)
	{
		status = algoGrid.lineStatus(i);
		if (status == 0)
		{
			continue;
		}
		if (status == 1)
		{
			// 满
			clearline++; // 行+1
			// 此行贡献方块数
			for (int j = 0 ; j < 4 ; j++)
			{
				if (algoTetris.positions[j].x == i)
				{
					cells++;
				}
			}
		}
		else 
		{
			// 空
			break;
		}
	}
	return clearline * cells;
} 
// 行变化率评分
INT getRT() 
{
	// TODO 可以优化，遇到空行，以上行直接每行+2
	int lineChane = 0, lastStatus;
	for (int i = PAINT_YNODE - 1; i >= 0; i--)
	{
		lastStatus = 1; // 左墙壁状态为有效，当作存在图形
		for (int j = 0;j < GAME_XNODE; j++)
		{
			// 与上一个格子不同
			if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
			{
				lineChane++;
				lastStatus = algoGrid[i][j];
			}
			// 当前行最后一个节点为空
			if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
			{
				lineChane++;
			}
		}
	}
	return lineChane;
}  
// 列变化率评分
INT getCT() 
{
	int colChane = 0, lastStatus;
	for (int i = 0; i < GAME_XNODE; i++)
	{
		lastStatus = 1; // 左墙壁状态为有效，当作存在图形
		for (int j = 0;j < PAINT_YNODE; j++)
		{
			// 与上一个格子不同
			if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
			{
				colChane++;
				lastStatus = algoGrid[j][i];
			}
			// 当前行最后一个节点为空
			if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
			{
				colChane++;
			}
		}
	}
	return colChane;
}  
// 空洞数评分
INT getNH() 
{
	int holes = 0;
	bool holeFlag = false; // 是否可以开始计算空洞
	for (int i = 0; i < GAME_XNODE; i++)
	{
		holeFlag = false;
		for (int j = 0;j < PAINT_YNODE; j++)
		{
			if (!holeFlag)
			{
				if (algoGrid[j][i])
				{
					holeFlag = true;
				}
			}
			else 
			{
				if (algoGrid[j][i] == 0)
				{
					holes++;
				}
			}
		}
	}
	return holes;
}  
// 井深评分
INT getWS() 
{
	// TODO: 井深求解不精确
	int wells = 0;
	int center, left, right;
	for (int i = 0; i < PAINT_YNODE; i++)
	{
		for (int j = 0;j < GAME_XNODE; j++)
		{
			
			center = algoGrid[i][j];
			if (j > 0)
			{
				left = algoGrid[i][j - 1];
			}
			else
			{
				left = 1;
			}
			if (j < GAME_XNODE - 1)
			{
				right = algoGrid[i][j+1];
			}
			else
			{
				right = 1;
			}
			// 是否为井
			if (left && right && !center)
			{
				for (int k = i; k < PAINT_YNODE;k ++)
				{
					if (algoGrid[k][j] == 0)
					{
						wells++;
					}
					else 
					{
						break;
					}
				}
			}
		}
	}
	return wells;
} 
