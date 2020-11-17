// MapEditor_Dll.h: MapEditor_Dll DLL 的主标头文件
//
#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include "resource.h"		// 主符号
// CMapEditorDllApp
// 有关此类实现的信息，请参阅 MapEditor_Dll.cpp
//
class CMapEditorDllApp : public CWinApp
{
public:
	CMapEditorDllApp();

// 重写
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
typedef struct {
	char flag[3];//PNT LIN REG
	int version; //version/10.0
}VERSION;   //版本数据结构
typedef struct
{
	double x;
	double y;
}D_DOT;  //节点点
///---------------------------------------------------------------
typedef struct
{
	double    x;
	double    y;
	COLORREF  color;
	int       pattern;
	char      isDel;
}PNT_STRU;   //点数据结构
///---------------------------------------------------------------
typedef struct
{
	char isDel;     //是否删除
	COLORREF color; //线颜色
	int pattern;    //型号
	long dotNum;    //节点个数
	long datOff;    //节点偏移
} LIN_NDX_STRU; //线数据索引文件
///---------------------------------------------------------------
typedef struct
{
	char isDel;
	COLORREF color;
	int pattern;
	long dotNum;
	long datOff;
	int bmp;
}REG_NDX_STRU;
///--------------------------------------------------------------
typedef struct
{
	char isDel;
	int num;
	long datOff;
	long length;
}BMP_NDX_STRU;
_declspec(dllexport) double Distance(double x1, double y1, double x2, double y2);  //计算两点距离
_declspec(dllexport) double DisPntToSeg(D_DOT pt1, D_DOT pt2, D_DOT pt);
_declspec(dllexport) PNT_STRU FindPnt(CPoint mousePoint, int pntNum, CFile *pntTmpF, int &nPnt); //查找最近点
_declspec(dllexport) LIN_NDX_STRU FindLin(CFile *LinTmpNdxF, CFile *LinTmpDatF, CPoint mousePoint, int LinNum, int &nLinNdx, int dis);  //查找离鼠标最近的线
_declspec(dllexport) BOOL PtInPolygon(CPoint p, D_DOT *ptPolygon, int nCount);  //判断点是否再区内
_declspec(dllexport) REG_NDX_STRU FindReg(CFile *RegTmpNdxF, CFile *RegTmpDatF, CPoint mousePoint, int RegNum, int &nRegNdx);  //查找离鼠标最近的区
_declspec(dllexport) PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile *PntTmpF, int &nPnt);   //查找最近的删除点
_declspec(dllexport) LIN_NDX_STRU FindDeleteLin(CFile *LinTmpNdxF, CFile *LinTmpDatF, CPoint mousePoint, int LinNum, int &nLinNdx);  //查找离鼠标最近的删除线
_declspec(dllexport) REG_NDX_STRU FindDeleteReg(CFile *RegTmpNdxF, CFile *RegTmpDatF, CPoint mousePoint, int RegNum, int &nRegNdx);  //查找离鼠标最近的删除区
_declspec(dllexport) LIN_NDX_STRU FindPntOnLin(CFile *LinTmpNdxF, CFile *LinTmpDatF, CPoint mousePoint, int LinNum, int &nLinNdx, int &nDotNdx);  //查找最近的的线上最近的点
_declspec(dllexport) LIN_NDX_STRU FindPntOnLin(CFile *LinTmpNdxF, CFile *LinTmpDatF, CPoint mousePoint, int LinNum, int &nLinNdx, int &nDotNdx1, int &nDotNdx2);  //查找最近的的线上最近的两个点
_declspec(dllexport) void PntToDot(D_DOT *dot, POINT *pnt, int num);  ///point dot 转换
_declspec(dllexport) void PntToDot(D_DOT &dot, POINT pnt);
_declspec(dllexport) void DotToPnt(POINT *pnt, D_DOT* dot, int num);
_declspec(dllexport) void DotToPnt(POINT &pnt, D_DOT dot);
_declspec(dllexport) void PntVPtoDP(D_DOT &pt, double zoom, double offset_x, double offset_y);    //窗口坐标系到数据坐标系
_declspec(dllexport) void PntDPtoVP(D_DOT &pt, double zoom, double offset_x, double offset_y);    //数据坐标系到窗口坐标系
_declspec(dllexport) D_DOT GetCenter(RECT rect);    //计算矩形中心
_declspec(dllexport) void modulusZoom(RECT client, RECT rect, double &zoom);  //计算拉框放大时的倍数
_declspec(dllexport) void AlterLindot(CFile *LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDataOff);   //连接线改变线的点数据
///---------------------------点绘制相关--------------------------//
_declspec(dllexport) void DrawPnt(CClientDC *dc, PNT_STRU point);
_declspec(dllexport) void ShowAllPnt(CClientDC *dc, CFile *pntTmpF, int pntNum);
_declspec(dllexport) void ShowAllPnt(CClientDC *dc, CFile *PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);
///---------------------------线绘制相关--------------------------//
_declspec(dllexport) void DrawSeg(CClientDC *dc, LIN_NDX_STRU line, POINT point1, POINT point2);
_declspec(dllexport) void ShowAllLin(CClientDC *dc, CFile *LinTmpNdxF, CFile *LinTmpDatF, int LinNum);
_declspec(dllexport) void ShowAllLin(CClientDC *dc, CFile *LinTmpNdxF, CFile *LinTmpDatF, int LinNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);
///---------------------------区绘制相关--------------------------//
_declspec(dllexport) void DrawReg(CClientDC *dc, REG_NDX_STRU Region, POINT *pt, long nPnt, int GBmpNum, CFile *GBmpTmpNdxF, CFile *GBmpTmpDatF);
_declspec(dllexport) void ShowAllReg(CClientDC *dc, CFile *RegTmpNdxF, CFile *RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel, int GBmpNum, CFile *GBmpTmpNdxF, CFile *GBmpTmpDatF);
_declspec(dllexport) void WritePntToFile(CFile *PntTmpF, int i, PNT_STRU point);//点数据写入临时文件
_declspec(dllexport) void ReadTempFileToPnt(CFile *PntTmpF, int i, PNT_STRU &point);  //读取临时点文件（用于构建永久文件）
_declspec(dllexport) void ReadPntPermanentFileToTemp(CFile *pntF, CFile *pntTmpF, int &nPnt, int &nLPnt); //从永久文件读取到临时文件
_declspec(dllexport) void UpdatePnt(CFile *pntTmpF, int i, PNT_STRU pnt); //修改临时文件中的点数据！          =============》》            解决了使用链表更改点之后的迭代查找！！！！
_declspec(dllexport) void WriteLinNdxToFile(CFile *linTmpNdxF, int i, LIN_NDX_STRU line);   //线索引写入临时文件
_declspec(dllexport) void WriteLinDatToFile(CFile *linTmpDatF, long datOff, int i, D_DOT point);  //线节点数据写入临时文件
_declspec(dllexport) void ReadTempFileToLinDat(CFile *LinTmpDatF, long datOff, int i, D_DOT &pnt);  //从临时线数据文件中读取线点数据
_declspec(dllexport) void ReadTempFileToLinNdx(CFile *LinTmpNdxF, int i, LIN_NDX_STRU &LinNdx);     //从临时线索引文件中读取线索引数据
_declspec(dllexport) void WriteTempToLinePermanentFile(CFile *LinF, CFile *LinTmpDatF, CFile *LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);//将线索引和数据写入永久文件中
_declspec(dllexport) void ReadLinPermanentFileToTemp(CFile *LinF, CFile *LinTmpDatF, CFile *LinTmpNdxF, VERSION &LinVer, int &nLin, int &nLLin, long &TmpFlinDatOffset);//从永久文件中读取线数据
_declspec(dllexport) void UpdateLin(CFile *LinTmpNdxF, int nLin, LIN_NDX_STRU line);  //修改临时文件中线数据   --》  删除线时跟新Ndx
_declspec(dllexport) void UpdateLin(CFile *LinTmpNdxF, CFile *LinTmpDatF, int LinNdx, double offset_x, double offset_y);  //修改临时文件中线数据   --》  移动线时跟新Dat
_declspec(dllexport) void AlterStartLin(CFile *LinTmpNdxF, long subdatOff, int nLine, int subNum);  //修改第一条线索引
_declspec(dllexport) void AlterEndLin(CFile *LinTmpNdxF, int nLine);  //修改第二条线索引
_declspec(dllexport) void WriteRegNdxToFile(CFile *RegTmpNdxF, int i, REG_NDX_STRU Region);   //区索引写入临时文件
_declspec(dllexport) void WriteRegDatToFile(CFile *RegTmpDatF, long datOff, int i, D_DOT point);  //区数据写入临时文件
_declspec(dllexport) void ReadTempFileToRegNdx(CFile *RegTmpNdxF, int i, REG_NDX_STRU &RegNdx);   //从临时文件读取区索引
_declspec(dllexport) void ReadTempFileToRegDat(CFile *RegTmpDatF, long datOff, int i, D_DOT &Pnt);  //从临时文件读取区数据
_declspec(dllexport) void ReadRegPermanentFileToTemp(CFile *RegF, CFile *RegTmpDatF, CFile *RegTmpNdxF, VERSION &RegVer, int &nReg, int &nLReg, long &TmpFlinDatOffset);
_declspec(dllexport) void WriteTempToRegPermanentFile(CFile *RegF, CFile *RegTmpDatF, CFile *RegTmpNdxF, VERSION RegVer, int nReg, int nLReg);
_declspec(dllexport) void UpdateReg(CFile *RegTmpNdxF, int nReg, REG_NDX_STRU Region); //更新区临时索引文件   -->>删除时更新Ndx
_declspec(dllexport) void UpdateReg(CFile *RegTmpNdxF, CFile *RegTmpDatF, int RegNdx, double offset_x, double offset_y);   //更新区数据文件  --》   移动时更新Dat
_declspec(dllexport) void WriteBmpNdxToFile(CFile *BmpTmpNdxF, int i, BMP_NDX_STRU Bmp);   //位图索引写入临时文件
_declspec(dllexport) void WriteBmpDatToFile(CFile *BmpTmpDatF, CFile *bitmap);  //位图数据写入临时文件
_declspec(dllexport) void ReadTempFileToBmpNdx(CFile *BmpTmpNdxF, int i, BMP_NDX_STRU &BmpNdx);   //从临时文件读取位图索引
_declspec(dllexport) void ReadTempFileToBmpDat(CFile *BmpTmpDatF, long datOff, int length, CFile &bitmap);  //从临时文件读取位图数据
_declspec(dllexport) void ReadBmpPermanentFileToTemp(CFile *BmpF, CFile *BmpTmpDatF, CFile *BmpTmpNdxF, VERSION &BmpVer, int &nBmp, int &nLBmp, long &TmpFlinDatOffset);
_declspec(dllexport) void WriteTempToBmpPermanentFile(CFile *BmpF, CFile *BmpTmpDatF, CFile *BmpTmpNdxF, VERSION BmpVer, int nBmp, int nLBmp);