#pragma once

#include "resource.h"
#include "class.h"


/* *************宏定义************* */
#define BLOCK_SIZE 10 // 留白
#define NODE_SIZE 30 // 每个节点大小为10
#define GAME_XNODE 10 // 游戏区X轴10个节点
#define INFO_XNODE 6 // 信息展示区宽度5个节点
#define PAINT_YNODE 20 //Y轴20个节点
#define PAINT_WIDTH ((GAME_XNODE+INFO_XNODE) * NODE_SIZE +  3 * BLOCK_SIZE) // 窗口宽度
#define PAINT_HEIGHT (PAINT_YNODE * NODE_SIZE + 2 * BLOCK_SIZE)             // 窗口高度

#define TIMER_ID 1
#define AI_TIMER_ID 2

/* *************全局常量************* */
// 游戏难度
CONST enum GAME_DIF
{
	SIMPLE = 1,
	GENERAL,
	DIFFICULT,
	DIFFICULTEST
};
CONST UINT GENERAL_SCORE = 5; // 中等难度对应分数限制
CONST UINT DIFFICULT_SCORE = 10; // 困难对应分数限制
CONST UINT DIFFICULTEST_SCORE = 15; // 特别困难对应分数限制
// 游戏区坐标
CONST RectPosition GAME_POSITION(BLOCK_SIZE, BLOCK_SIZE,BLOCK_SIZE + NODE_SIZE * GAME_XNODE, BLOCK_SIZE + NODE_SIZE * PAINT_YNODE);
// 信息区坐标
CONST RectPosition INFO_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE, BLOCK_SIZE,PAINT_WIDTH - BLOCK_SIZE, PAINT_HEIGHT - BLOCK_SIZE);
// 信息区 - 下一个文字坐标
CONST RectPosition NEXT_FONT_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE + BLOCK_SIZE, BLOCK_SIZE * 2,PAINT_WIDTH - BLOCK_SIZE*2, BLOCK_SIZE * 2 + 2 * NODE_SIZE);
// 信息区 - 下一个图形坐标
CONST RectPosition NEXT_TETRIS_POSITION(BLOCK_SIZE*2 + NODE_SIZE * GAME_XNODE + NODE_SIZE , BLOCK_SIZE * 2 + 2 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE , BLOCK_SIZE * 2 + NODE_SIZE * 6);
// 信息区 - 难度文字坐标
CONST RectPosition LEVEL_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 10 * NODE_SIZE, PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 11 * NODE_SIZE);
// 信息区 - 得分文字坐标
CONST RectPosition SCORE_FONT_POSITION(NODE_SIZE * GAME_XNODE + NODE_SIZE * 2, BLOCK_SIZE * 2 + 11 * NODE_SIZE,PAINT_WIDTH - BLOCK_SIZE - NODE_SIZE, BLOCK_SIZE * 2 + 12 * NODE_SIZE);

CONST UINT DEFAULT_INTERVAL = 1000; // 默认下落时间间隔

/* *************全局变量************* */
UINT iCurDif = SIMPLE; // 当前游戏难度
UINT iCurScore = 0;	   // 当前得分
/* 游戏区域栅栏 ,值表示对应颜色枚举类型*/
// BYTE btCurGrid[PAINT_YNODE][GAME_XNODE];
Grid btCurGrid(PAINT_YNODE, GAME_XNODE);
/*  
* 当前下落图形 (x,y) 关于(a,b)顺时针旋转90度后为 (y-b+a,a+b-x) 
* 在20 * 10的网格中 定义图形类，包含四个点坐标与颜色信息
*/
Tetris curTetris, nextTetris;
UINT iInterval = DEFAULT_INTERVAL; // 下落时间间隔

/* *************函数定义************* */
VOID InitWndPostionSize(HWND);  // 初始化窗体的位置和大小
VOID InitGame(HWND);			  // 启动游戏
VOID DrawBackGround(HDC);		  // 绘制背景
VOID DrawInfo(HDC);				  // 绘制信息栏
VOID DrawTetris(HDC);			  // 绘制下落图形/信息
VOID checkTetris(HWND,INT);		      // 检查图形移动是否越界 -> 下落结束
VOID finishDown(HWND);			  // 下落结束: 更换当前图形，生成新图形 ， 消除
VOID setLevel(HWND);				  // 调整游戏难度
// 根据坐标位置获取节点像素位置
RectPosition getRectPosition(BYTE , BYTE);


/* ***************AI模式**************** */
BOOL AI_SWITCH = false;
UINT AI_DEFAULT_INTERVAL = 10;
// 评分项权重
CONST INT LH_WEIGHT = -45;
CONST INT RE_WEIGHT = 34;
CONST INT RT_WEIGHT = -32;
CONST INT CT_WEIGHT = -93;
CONST INT NH_WEIGHT = -79;
CONST INT WS_WEIGHT = -34;

Tetris bestTetris, algoTetris; // 最佳位置
Grid algoGrid(PAINT_YNODE, GAME_XNODE);// 用于计算的栅栏
INT horizonIndex[GAME_XNODE]; // 记录网格水平面
INT bestScore = 0x80000000;   // 最佳评分,初始值为最小INT值
INT needRotate = 0, needMoveLeft = 0; // 每次到达最佳位置需要转动、向左移动次数

VOID getHorizonIndex();
VOID AIHandle(HWND); // AI模式定时器处理函数
VOID getBestPosition(); // 获取最佳评分及其位置
INT putTetrisToGrid(INT); // 将base_tetris放到algoGrid的指定位置中, 返回需要向左移动次数
INT Dellacherie();    // AI算法：计算最佳评分位置
INT getLH(); // 高度评分
INT getRE();  // 消除行*贡献方块数评分
INT getRT();  // 行变化率评分
INT getCT();  // 列变化率评分
INT getNH();  // 空洞数评分
INT getWS();  // 井深评分

VOID aiMoveTetris(HWND); // 将方块移动到最佳位置