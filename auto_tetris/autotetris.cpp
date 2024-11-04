// autotetris.cpp: ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "autotetris.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AUTOTETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOTETRIS));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
    checkTetris(hWnd, VK_DOWN);

    // 如果达到某个条件，停止定时器
    if (/* 某个条件 */)
    {
        KillTimer(hWnd, idTimer);
    }
}
// ������ ������Ϸ
VOID InitGame(HWND hWnd)
{
	// ��ʼ������
	btCurGrid.init(PAINT_YNODE,GAME_XNODE);
	iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
	iCurDif = SIMPLE;
	iCurScore = 0;
	// �����׸�����ͼ��
	curTetris.random();
	// ������һ������ͼ��
	nextTetris.random();
	// ������ʱ��
	KillTimer(hWnd, TIMER_ID);

	SetTimer(hWnd, TIMER_ID, iInterval, TimeProc);

}

// ������ ��ʼ�������λ�úʹ�С
VOID InitWndPostionSize(HWND hWnd)
{
	RECT rect;
	int nWinX, nWinY, nClientX, nClientY;
	//��ȡ���ڴ�С  
	GetWindowRect(hWnd, &rect);
	nWinX = rect.right - rect.left;
	nWinY = rect.bottom - rect.top;
	//��ȡ�ͻ�����С  
	GetClientRect(hWnd, &rect);
	nClientX = rect.right - rect.left;
	nClientY = rect.bottom - rect.top;

	// ��ȡ��ǰ��Ļ�Ŀ��ߣ������ڷ�����������
	UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
	// ��������λ�����С
	MoveWindow(hWnd, (iScreenX - PAINT_WIDTH)/2, (iScreenY - PAINT_HEIGHT)/3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}

// ������ ���Ʊ���
VOID DrawBackGround(HDC hdc)
{
	HPEN hPenNull = (HPEN)GetStockObject(NULL_PEN);         // HPEN NULL
	HBRUSH hBrushGray = (HBRUSH)GetStockObject(GRAY_BRUSH); // hBRUSH GRAY
	SelectObject(hdc,hPenNull);
	SelectObject(hdc, hBrushGray); // ������ɫ	
	// ��Ϸ�����С (HDC,���Ͻ�X,���Ͻ�Y�����½�X�����½�Y)
	Rectangle(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY, GAME_POSITION.iRightX, GAME_POSITION.iRightY);
	// ��Ϣ����С
	Rectangle(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY, INFO_POSITION.iRightX, INFO_POSITION.iRightY);
	DeleteObject(hBrushGray);
	DeleteObject(hPenNull);

}
// ������ ������Ϣ��
VOID DrawInfo(HDC hdc)
{
	// ��һ�����䷽��
	RECT rect;
	HFONT hFont = CreateFont(23, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"΢���ź�");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255,255,0));
	
	NEXT_FONT_POSITION.setRect(rect);
	DrawText(hdc, TEXT("��һ��"), _tcslen(TEXT("��һ��")), &rect , 0);

	LEVEL_FONT_POSITION.setRect(rect);
	TCHAR tcDif[10];
	LPCTSTR lpctDif = TEXT("�Ѷ�: %d");
	wsprintf(tcDif, lpctDif , iCurDif);
	DrawText(hdc, tcDif, _tcslen(tcDif), &rect , 0);

	SCORE_FONT_POSITION.setRect(rect);
	TCHAR tcScore[10];
	LPCTSTR lpctScore = TEXT("�÷�: %d");
	wsprintf(tcScore, lpctScore, iCurScore);
	DrawText(hdc, tcScore, _tcslen(tcScore), &rect , 0);

	DeleteObject(hFont);
}
// ���������ȡ��ǰ�ڵ�λ��
RectPosition getRectPosition(BYTE x, BYTE y)
{
	RectPosition rectPs;
	rectPs.iLeftX = BLOCK_SIZE + y*NODE_SIZE;
	rectPs.iLeftY = BLOCK_SIZE + x*NODE_SIZE;
	rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
	rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
	return rectPs;
}
// ���������Ʒ���
VOID DrawTetris(HDC hdc)
{
	RectPosition ps;
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc, hPen);
	// ����Game����
	for (int i =0; i< PAINT_YNODE; i++)
	{
		for (int j =0; j< GAME_XNODE; j++)
		{
			if (btCurGrid[i][j] > 0)
			{
				// ���Ƶ�ǰ�ڵ�
				SelectObject(hdc, getHbrush(btCurGrid[i][j]));
				ps = getRectPosition(i, j);
				Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
			}
		}
	}
	// ������������ͼ��
	SelectObject(hdc, getHbrush(curTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(curTetris.positions[i].x , curTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
	}
	// ������һ������ͼ��
	int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
	int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
	SelectObject(hdc, getHbrush(nextTetris.color));
	for (int i =0;i < 4; i++)
	{
		ps = getRectPosition(nextTetris.positions[i].x , nextTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX+ move_x, ps.iRightY +move_y);
	}
}
// ������Ϸ�Ѷ�
VOID setLevel(HWND hWnd)
{
	BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore >= GENERAL_SCORE;
	BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore >= DIFFICULT_SCORE;
	BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore >= DIFFICULTEST_SCORE;
	
	// �����Ѷ�
	if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
	{
		iCurDif ++;
		iInterval -= 250;
		KillTimer(hWnd , TIMER_ID);
		SetTimer(hWnd , TIMER_ID, iInterval , TimeProc);
	}
}
// �������
VOID finishDown(HWND hWnd)
{
	// ����btCurGrid��ǰ��������
	btCurGrid.addCurTerisToGrid(curTetris);
	// �������䷽�飬��������nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// ����
	iCurScore += btCurGrid.clearLine();
	// �ж�����λ���Ƿ�ռ��
	if (btCurGrid.isExist(curTetris))
	{
		// TODO ��Ϸ����
		KillTimer(hWnd, TIMER_ID);
		MessageBox(hWnd, TEXT("��Ϸ����"),TEXT("��Ϣ"),MB_OK);
		return;
	}
	// ������Ϸ�Ѷ�(aiģʽ����Ҫ)
	if (!AI_SWITCH)
	{
		setLevel(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE);
		// ���ΪAIģʽ����������
		AIHandle(hWnd);
	}
}

// ���ͼ�������Ƿ�Խ��/��� / ����
VOID checkTetris(HWND hWnd, INT type)
{
	int maxX , minX , maxY , minY;
	Tetris temp = curTetris;
	switch (type)
	{
		case VK_LEFT: // ��
			temp.left();
			break;
		case VK_RIGHT: // ��
			temp.right();
			break;
		case VK_UP: // ��
			temp.rotate();
			break;
		case VK_DOWN: // ��(���������½�Ч��)
			temp.down(); 
			break;
	}
	temp.getMaxMin(minX , maxX , minY , maxY);

	// �ж��Ƿ�Խ��
	if (maxX > 19)
	{
		// �����ײ����������
		finishDown(hWnd);
		return;
	}
	if (minX < 0 || maxY > 9 || minY < 0)
	{
		// �������ұ߽�
		return;
	}
	// �ж�λ���Ƿ�ռ��
	if (btCurGrid.isExist(temp))
	{
		if (type == VK_DOWN)
		{
			// ���䵽�Ѵ��ڽڵ��Ϸ����������
			finishDown(hWnd);
		}
		return;
	}
	// ��������
	curTetris = temp;
	InvalidateRect(hWnd, NULL, TRUE);
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
                checkTetris(hWnd, wParam);
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
        // 清理定时器
        KillTimer(hWnd, TIMER_ID);
        KillTimer(hWnd, AI_TIMER_ID);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ��������
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

// ����������AIģʽ
VOID AIHandle(HWND hWnd)
{
	// Pierre Dellacherie ��ȡ�����ŵ�
	getBestPosition();
	// �Զ��ƶ������λ�ò���������
	aiMoveTetris(hWnd);
}
// ��ȡ������ּ���λ��
VOID getBestPosition()
{
	bestScore = 0x80000000;
	int maxX , minX , maxY , minY;
	algoTetris = curTetris;	
	Tetris temp;
	//��ȡ����ˮƽ��
	getHorizonIndex(); 
	// ͼ��������״̬
	for (int i = 0 ; i < 4 ; i ++)
	{
		algoTetris.getMaxMin(minX , maxX , minY , maxY);
		int num = 10 - (maxY - minY); // ÿ�α任����num�����
		for (int j = 0 ; j < num ; j++)
		{
			// ��ԭ����
			algoGrid = btCurGrid; 
			temp = algoTetris;
			// ��base_tetris�ŵ�algoGrid��ָ��λ����, ������Ҫ�����ƶ�����
			int moveLeft = putTetrisToGrid(j);
			// ����
			int score = Dellacherie();
			if (score > bestScore)
			{
				bestScore = score;
				bestTetris = algoTetris;
				needRotate = i;
				needMoveLeft = moveLeft;
			}
			// ��ԭ algoTetri
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
    } 
    else if (needMoveLeft > 0)
    {
        // 左移动
        curTetris.left();
        needMoveLeft--;
    } 
    else if (needMoveLeft < 0)
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
// ���ٲ����ö�ʱ��
VOID CALLBACK TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// ����btCurGrid��ǰ��������
	btCurGrid.addCurTerisToGrid(bestTetris);
	// �������䷽�飬��������nextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// ����
	iCurScore += btCurGrid.clearLine();
	// �ж�����λ���Ƿ�ռ��
	if (btCurGrid.isExist(curTetris))
	{
		// TODO ��Ϸ����
		KillTimer(hWnd, 3);
		MessageBox(hWnd, TEXT("��Ϸ����"),TEXT("��Ϣ"),MB_OK);
		return;
	}
	// ���ΪAIģʽ����������
	AIHandle(hWnd);
}
// �������ƶ������λ��
VOID aiMoveTetris(HWND hWnd)
{
	SetTimer(hWnd, AI_TIMER_ID, iInterval / 5 ,AITimeProc);
	// SetTimer(hWnd, 3, 0 ,TESTTimeProc); // ���ٲ���ʹ��
}
// ��ȡ����ˮƽ��
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
	int left_bottom_index = algoTetris.getLeftBottom(); // ��ȡ�����½ǵ�Ԫ���±�
	
	int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // ��Ҫ�����ƶ��ľ���
	int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // ��Ҫ�����ƶ��ľ���
	// ���������ƶ�����
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
	// ��algoTetris�ƶ���ָ��λλ��
	for(int j = 0 ; j < 4 ; j ++)
	{
		algoTetris.positions[j].x += maxDistanceRow;
		algoTetris.positions[j].y -= maxDistanceCol;
		algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
	}
	return maxDistanceCol;
}
// AI�����㷨
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
// �߶�����
INT getLH() 
{
	int maxX, minX, maxY, minY;
	algoTetris.getMaxMin(minX, maxX, minY, maxY);
	return PAINT_YNODE - maxX;
} 
// ������*���׷���������
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
			// ��
			clearline++; // ��+1
			// ���й��׷�����
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
			// ��
			break;
		}
	}
	return clearline * cells;
} 
// �б仯������
INT getRT() 
{
	// TODO �����Ż����������У�������ֱ��ÿ��+2
	int lineChane = 0, lastStatus;
	for (int i = PAINT_YNODE - 1; i >= 0; i--)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		for (int j = 0;j < GAME_XNODE; j++)
		{
			// ����һ�����Ӳ�ͬ
			if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
			{
				lineChane++;
				lastStatus = algoGrid[i][j];
			}
			// ��ǰ�����һ���ڵ�Ϊ��
			if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
			{
				lineChane++;
			}
		}
	}
	return lineChane;
}  
// �б仯������
INT getCT() 
{
	int colChane = 0, lastStatus;
	for (int i = 0; i < GAME_XNODE; i++)
	{
		lastStatus = 1; // ��ǽ��״̬Ϊ��Ч����������ͼ��
		for (int j = 0;j < PAINT_YNODE; j++)
		{
			// ����һ�����Ӳ�ͬ
			if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
			{
				colChane++;
				lastStatus = algoGrid[j][i];
			}
			// ��ǰ�����һ���ڵ�Ϊ��
			if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
			{
				colChane++;
			}
		}
	}
	return colChane;
}  
// �ն�������
INT getNH() 
{
	int holes = 0;
	bool holeFlag = false; // �Ƿ���Կ�ʼ����ն�
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
// ��������
INT getWS() 
{
	// TODO: ������ⲻ��ȷ
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
			// �Ƿ�Ϊ��
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
