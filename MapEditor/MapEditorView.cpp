
// MapEditorView.cpp: CMapEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapEditor.h"
#endif

#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include "CCreateFileDlg.h"
#include "CPointParamerterDlg.h"
#include "CLineParamerDlg.h"
#include "CRegionParamerterDlg.h"
#include "CSelectPoint.h"
#include "CSelectLine.h"
#include "CSelectRegion.h"
#include "CImageDlg.h"
#include <vector>
#include <string>
#include "MapEditor_Dll.h"
#pragma comment(lib,"MapEditor_Dll.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
///------------------------------------------------------------点版本信息------------------------------------------------------------///
VERSION GPntVer =
{
	GPntVer.flag[0] = 'P',
	GPntVer.flag[1] = 'N',
	GPntVer.flag[2] = 'T',
	GPntVer.version = 10
};

///------------------------------------------------------------线版本信息------------------------------------------------------------///
VERSION GLinVer =
{
	GLinVer.flag[0] = 'L',
	GLinVer.flag[1] = 'i',
	GLinVer.flag[2] = 'n',
	GLinVer.version = 10
};

///------------------------------------------------------------区版本信息------------------------------------------------------------///
VERSION GRegVer =
{
	GLinVer.flag[0] = 'R',
	GLinVer.flag[1] = 'e',
	GLinVer.flag[2] = 'g',
	GLinVer.version = 10
};
///------------------------------------------------------------位图版本信息------------------------------------------------------------///
VERSION GBmpVer =
{
	GLinVer.flag[0] = 'B',
	GLinVer.flag[1] = 'm',
	GLinVer.flag[2] = 'p',
	GLinVer.version = 10
};

///------------------------------------------------------默认点结构与临时点结构------------------------------------------------------///
PNT_STRU GPnt = { GPnt.isDel = 0,GPnt.color = RGB(0,0,0),GPnt.pattern = 0 };  //默认

///------------------------------------------------默认线缩影结构、临时线索引结构相关------------------------------------------------///
LIN_NDX_STRU GLin = { GLin.isDel = 0,GLin.color = RGB(0,0,0),GLin.pattern = 0,GLin.dotNum = 0,GLin.datOff = 0 };  //默认
LIN_NDX_STRU GTLin; //正在绘制线记录
POINT GLPnt = { GLPnt.x = -1,GLPnt.y = -1 };  //记录线段起点
CPoint GMPnt(-1, -1);   //记录鼠标上一次位置

///-------------------------------------------------默认区索引结构、临时索引结构及相关------------------------------------------------///
REG_NDX_STRU GReg = { GReg.bmp = -1, GReg.isDel = 0,GReg.color = RGB(0,0,0),GReg.pattern = 0 ,GReg.dotNum = 0,GReg.datOff = 0 };
REG_NDX_STRU GTReg;
///-------------------------------------------------默认区索引结构、临时索引结构及相关------------------------------------------------///
BMP_NDX_STRU GBmp = { GReg.isDel = 0 ,GReg.datOff = 0,GBmp.length = 0,GBmp.num = 0 };


///------------------------------------------------------------点全局变量------------------------------------------------------------///
bool      GPntFCreated = false;                                                    //临时文件是否创建
CString   GPntFName;                                                               //永久文件名（含路径）
CString   GPntTmpFName = CString("tempPntF.dat");                                  //临时文件名（含路径）
bool      GPntChanged = false;                                                     //是否更改
int       GPntNum = 0;                                                             //物理数
int       GPntLNum = 0;														       //逻辑数
CFile     *GPntTmpF = new CFile();                                                 //读取临时文件指针对象

int GPntNdx = -1;    //FindPnt查找到的位置

PNT_STRU GTPnt;      //移动点时的临时变量

CPoint GPntLBDPnt(-1, -1);   //左键点下位置
CPoint GPntMMPnt(-1, -1);    //鼠标移动位置
int GPntStytle = 0;          //删除方式
///------------------------------------------------------------线全局变量------------------------------------------------------------///
bool      GLinFCreated = false;                                                    //临时文件是否创建
CString   GLinFName;                                                               //永久文件名（含路径）
CString   GLinTmpNdxFName = CString("tempLinF.ndx");                               //临时索引文件名（含路径）
CString   GLinTmpDatFName = CString("tempLinF.dat");                               //临时数据文件名（含路径）
bool      GLinChanged = false;                                                     //是否更改
int       GLinNum = 0;                                                             //物理数
int       GLinLNum = 0;														       //逻辑数
CFile     *GLinTmpDatF = new CFile();                                              //读取临时数据文件的指针对象
CFile     *GLinTmpNdxF = new CFile();                                              //读取临时索引文件的指针对象

int GLinNdx = -1;    //FindLin 与 FindPointOnLin查找到的位置
int GLinDotNdx = -1;    //FindPntOnLin 查找到的位置
int GLinDotNdx1 = -1;    //FindPntOnLin 查找到的位置
int GLinDotNdx2 = -1;    //FindPntOnLin 查找到的位置
LIN_NDX_STRU tempLinNdx;   //找到的线的临时索引文件

CPoint GLinLBDPnt(-1, -1);   //鼠标左键按下位置
CPoint GLinMMPnt(-1, -1);    //记录鼠标上一个位置 --》删除绘制
long GLinMMOffsetX = 0;      //x偏移
long GLinMMOffsetY = 0;       //y偏移
LIN_NDX_STRU GLinMMTmpNdx;   //移动线时的临时变量

//CPoint GLinLBDPnt(-1, -1);   //左键点下位置
//CPoint GLinMMPnt(-1, -1);    //鼠标移动位置
int GLinStytle = 0;          //删除方式
///------------------------------------------------------------区全局变量------------------------------------------------------------///
bool      GRegFCreated = false;                                                    //临时文件是否创建
CString   GRegFName;                                                               //永久文件名（含路径）
CString   GRegTmpNdxFName = CString("tempRegF.ndx");                               //临时索引文件名（含路径）
CString   GRegTmpDatFName = CString("tempRegF.dat");                               //临时数据文件名（含路径）
bool      GRegChanged = false;                                                     //是否更改
int       GRegNum = 0;                                                             //物理数
int       GRegLNum = 0;														       //逻辑数
CFile     *GRegTmpDatF = new CFile();                                              //读取临时数据文件的指针对象
CFile     *GRegTmpNdxF = new CFile();                                              //读取临时索引文件的指针对象

int GRegNdx = -1; //FindReg查找到的位置

CPoint GRegCreateMMPnt(-1, -1);       //鼠标移动前一点状态
CPoint GRegCreateStartPnt(-1, -1);    //造区的起点
REG_NDX_STRU GRegMMTmpNdx;            //记录鼠标选中的索引
CPoint GRegLBDPnt(-1, -1);            // 记录左键按下的位置，用于算偏移量
CPoint GRegMMPnt(-1, -1);             //记录鼠标移动时上一次状态，用于擦除
long GRegMMOffsetX = 0;               //X偏移
long GRegMMOffsetY = 0;               //Y偏移

//CPoint GRegLBDPnt(-1, -1);   //左键点下位置
//CPoint GRegMMPnt(-1, -1);    //鼠标移动位置
int GRegStytle = 0;          //删除方式
///------------------------------------------------------------放大与缩小------------------------------------------------------------///
CPoint GZoomLBDPnt(-1, -1);   //左键点下位置
CPoint GZoomMMPnt(-1, -1);    //鼠标移动位置
double GZoomOffset_x = 0;     //放大偏移x
double GZoomOffset_y = 0;     //放大偏移y
double GZoom = 1.0;           //放大倍数
int GZoomStytle = 0;          //放大方式

///------------------------------------------------------------连接线相关------------------------------------------------------------///
LIN_NDX_STRU GStartLin = GLin;  //第一条线
int GnStart = -1;
LIN_NDX_STRU GEndLin = GLin;    //第二条线
int GnEnd = -1;
int GnLine = 0;

///-----------------------------------------------------------移动窗口相关-----------------------------------------------------------///
CPoint GWinMoveLBDPnt(-1, -1);   //左键按下后的位置
CPoint GWinMoveMMPnt(-1, -1);    //鼠标移动前状态
///------------------------------------------------------------与复位相关------------------------------------------------------------///
double GMaxX = 0;
double GMaxY = 0;
double GMinX = 0;
double GMinY = 0;

///-----------------------------------------------------------显示状态相关-----------------------------------------------------------///
enum State { SHOWSTATE_UNDEL, SHOWSTATE_DEL };
State GCurShowState = SHOWSTATE_UNDEL;    //显示状态
bool GShowPnt = true;   //当前显示的结构是否为点
bool GShowLin = true;   //当前显示的结构是否为线
bool GShowReg = true;   //当前显示的结构是否为区
///--------------------------------------------------------位图文件相关-------------------------------------------------------------///
bool      GBmpFCreated = false;                                                    //临时文件是否创建
CString   GBmpFName;                                                               //永久文件名（含路径）
CString   GBmpTmpNdxFName = CString("tempBmpF.ndx");                               //临时索引文件名（含路径）
CString   GBmpTmpDatFName = CString("tempBmpF.dat");                               //临时数据文件名（含路径）
bool      GBmpChanged = false;                                                     //是否更改
int       GBmpNum = 0;                                                             //物理数
int       GBmpLNum = 0;					     									   //逻辑数
CFile     *GBmpTmpDatF = new CFile();                                              //读取临时数据文件的指针对象
CFile     *GBmpTmpNdxF = new CFile();
///------------------------------------------------------------与操作相关------------------------------------------------------------///
enum Action
{
	Noaction,
	OPERSTATE_INPUT_PNT,
	OPERSTATE_DELETE_PNT,
	OPERSTATE_MOVE_PNT,
	OPERSTATE_INPUT_LIN,
	OPERSTATE_DELETE_LIN,
	OPERSTATE_MOVE_LIN,
	OPERSTATE_INPUT_REG,
	OPERSTATE_DELETE_REG,
	OPERSTATE_MOVE_REG,
	OPERSTATE_ZOOM_IN,
	OPERSTATE_ZOOM_OUT,
	OPERSTATE_WINDOW_MOVE,
	OPERSTATE_LIN_DELETE_PNT,
	OPERSTATE_LIN_ADD_PNT,
	OPERSTATE_LINK_LIN,
	OPERSTATE_MODIFY_POINT_PARAMETER,
	OPERSTATE_MODIFY_lin_PARAMETER,
	OPERSTATE_MODIFY_REGION_PARAMETER,
	OPERSTATE_UNDELETE_PNT,
	OPERSTATE_UNDELETE_LIN,
	OPERSTATE_UNDELETE_REG
};
Action GCurOperState;

// CMapEditorView

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_WINDOW_ZOOM_IN, &CMapEditorView::OnWindowZoomIn)
	ON_COMMAND(ID_FILE_NEW, &CMapEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_POINT, &CMapEditorView::OnFileOpenPoint)
	ON_COMMAND(ID_FILE_OPEN_LINE, &CMapEditorView::OnFileOpenLine)
	ON_COMMAND(ID_FILE_OPEN_REGION, &CMapEditorView::OnFileOpenRegion)
	ON_COMMAND(ID_FILE_SAVE_POINT, &CMapEditorView::OnFileSavePoint)
	ON_COMMAND(ID_FILE_SAVE_LINE, &CMapEditorView::OnFileSaveLine)
	ON_COMMAND(ID_FILE_SAVE_REGION, &CMapEditorView::OnFileSaveRegion)
	ON_COMMAND(ID_FILE_SAVE_AS_POINT, &CMapEditorView::OnFileSaveAsPoint)
	ON_COMMAND(ID_FILE_SAVE_AS_LINE, &CMapEditorView::OnFileSaveAsLine)
	ON_COMMAND(ID_FILE_SAVE_AS_REGION, &CMapEditorView::OnFileSaveAsRegion)
	ON_COMMAND(ID_APP_EXIT, &CMapEditorView::OnAppExit)
	ON_COMMAND(ID_WINDOW_ZOOM_OUT, &CMapEditorView::OnWindowZoomOut)
	ON_COMMAND(ID_WINDOW_MOVE, &CMapEditorView::OnWindowMove)
	ON_COMMAND(ID_WINDOW_RESET, &CMapEditorView::OnWindowReset)
	ON_COMMAND(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnWindowShowPoint)
	ON_COMMAND(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnWindowShowLine)
	ON_COMMAND(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnWindowShowRegion)
	ON_COMMAND(ID_POINT_CREATE, &CMapEditorView::OnPointCreate)
	ON_COMMAND(ID_POINT_MOVE, &CMapEditorView::OnPointMove)
	ON_COMMAND(ID_POINT_DELETE, &CMapEditorView::OnPointDelete)
	ON_COMMAND(ID_POINT_SHOW_DELETED, &CMapEditorView::OnPointShowDeleted)
	ON_COMMAND(ID_POINT_UNDELETE, &CMapEditorView::OnPointUndelete)
	ON_COMMAND(ID_POINT_MODIFY_PARAMETER, &CMapEditorView::OnPointModifyParameter)
	ON_COMMAND(ID_PONIT_SET_DEEPARAMETER, &CMapEditorView::OnPonitSetDeeparameter)
	ON_COMMAND(ID_LINE_CREATE, &CMapEditorView::OnLineCreate)
	ON_COMMAND(ID_LINE_MOVE, &CMapEditorView::OnLineMove)
	ON_COMMAND(ID_LINE_DELETE, &CMapEditorView::OnLineDelete)
	ON_COMMAND(ID_LINE_SHOW_DELETED, &CMapEditorView::OnLineShowDeleted)
	ON_COMMAND(ID_LINE_UNDELETE, &CMapEditorView::OnLineUndelete)
	ON_COMMAND(ID_LINE_DELETE_DOT, &CMapEditorView::OnLineDeleteDot)
	ON_COMMAND(ID_LINE_ADD_DOT, &CMapEditorView::OnLineAddDot)
	ON_COMMAND(ID_LINE_LINK, &CMapEditorView::OnLineLink)
	ON_COMMAND(ID_LINE_MODIFY_PARAMETER, &CMapEditorView::OnLineModifyParameter)
	ON_COMMAND(ID_LINE_SET_DEFPARAMETER, &CMapEditorView::OnLineSetDefparameter)
	ON_COMMAND(ID_REGION_CREATE, &CMapEditorView::OnRegionCreate)
	ON_COMMAND(ID_REGION_MOVE, &CMapEditorView::OnRegionMove)
	ON_COMMAND(ID_REGION_DELETE, &CMapEditorView::OnRegionDelete)
	ON_COMMAND(ID_REGION_SHOW_DELETED, &CMapEditorView::OnRegionShowDeleted)
	ON_COMMAND(ID_REGION_UNDELETE, &CMapEditorView::OnRegionUndelete)
	ON_COMMAND(ID_REGION_MODIFY_PARAMETER, &CMapEditorView::OnRegionModifyParameter)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
//	ON_WM_DESTROY()
ON_WM_DESTROY()
ON_WM_MOUSEMOVE()
ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnUpdateWindowShowPoint)
ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnUpdateWindowShowLine)
ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnUpdateWindowShowRegion)
ON_UPDATE_COMMAND_UI(ID_POINT_SHOW_DELETED, &CMapEditorView::OnUpdatePointShowDeleted)
ON_UPDATE_COMMAND_UI(ID_LINE_SHOW_DELETED, &CMapEditorView::OnUpdateLineShowDeleted)
ON_UPDATE_COMMAND_UI(ID_REGION_SHOW_DELETED, &CMapEditorView::OnUpdateRegionShowDeleted)
ON_COMMAND(ID_REGION_SET_DEFPARAMETER, &CMapEditorView::OnRegionSetDefparameter)
ON_WM_CREATE()
ON_COMMAND(ID_IMAGE_CONTRAL, &CMapEditorView::OnImageContral)
ON_COMMAND(ID_FILE_OPEN_BMP, &CMapEditorView::OnFileOpenBmp)
ON_COMMAND(ID_FILE_SAVE_BMP, &CMapEditorView::OnFileSaveBmp)
ON_COMMAND(ID_FILE_SAVE_AS_BMP, &CMapEditorView::OnFileSaveAsBmp)
END_MESSAGE_MAP()

// CMapEditorView 构造/析构

CMapEditorView::CMapEditorView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapEditorView 绘图

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect mrect;
	GetClientRect(&mrect);
	CClientDC dc(this);
	dc.FillSolidRect(0, 0, mrect.Width(), mrect.Height(), dc.GetBkColor());//填充背景颜色
	dc.SetROP2(R2_NOTXORPEN);
	//ShowAllPnt(&dc, GPntTmpF, GPntNum);
	//ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum);
	if(GShowPnt)
		ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if(GShowLin)
		ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	if(GShowReg)
		ShowAllReg(&dc, GRegTmpNdxF, GRegTmpDatF, GRegNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState, GBmpNum,GBmpTmpNdxF, GBmpTmpDatF);
	ReleaseDC(&dc);
	// TODO: 在此处为本机数据添加绘制代码
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum > 1)
			{
				WriteLinNdxToFile(GLinTmpNdxF, GLinNum, GTLin);//写入线索引
				GLinNum++;
				GLinLNum++;
				DrawSeg(&dc, GTLin, GLPnt, point);
				GLin.datOff += (GTLin.dotNum * sizeof(D_DOT));  //增加偏移，从上一次开始
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			else if (GTLin.dotNum == 1)
			{
				DrawSeg(&dc, GTLin, GLPnt, point);
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			break;
		case OPERSTATE_LIN_ADD_PNT:
			if (GLinDotNdx1 != -1)
			{
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				int count = 0;
				long oldDattOffset = tempLinNdx.datOff;
				tempLinNdx.datOff = GLinTmpDatF->GetLength();
				tempLinNdx.dotNum++;
				if (GLinDotNdx1 > GLinDotNdx2)  //保证第一个数据索引更小
				{
					int temp = GLinDotNdx1;
					GLinDotNdx1 = GLinDotNdx2;
					GLinDotNdx2 = temp;
				}
				D_DOT tdot;
				for (int i = 0; i < tempLinNdx.dotNum; i++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, oldDattOffset, i, tdot);
					WriteLinDatToFile(GLinTmpDatF, tempLinNdx.datOff, count++, tdot);
					if (i == GLinDotNdx1)
					{
						WriteLinDatToFile(GLinTmpDatF, tempLinNdx.datOff, count++, dot);
					}
				}
				UpdateLin(GLinTmpNdxF, GLinNdx, tempLinNdx);
				GLinNdx = -1;
				GLinDotNdx1 = -1;
				GLinDotNdx2 = -1;
				GLinMMPnt = CPoint(-1, -1);
			}
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		//造区右键抬起事件(核心部分)
		case OPERSTATE_INPUT_REG:
			if (GTReg.dotNum > 2)   //点数大于二->造区
			{
				WriteRegNdxToFile(GRegTmpNdxF, GRegNum, GTReg);  //写入区索引文件
				//区物理与逻辑数增加
				++GRegNum;
				++GRegLNum;
				//删除多余的一个三角形
				POINT *pt = new POINT[3];   //存储起点终点与鼠标点
				D_DOT dot;
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);  //读取起点
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);  //数据坐标到试图坐标
				DotToPnt(pt[0], dot);  //POINT转D_DOT
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, GTReg.dotNum - 1, dot); //终点
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				pt[2] = point;  //鼠标点
				DrawReg(&dc, GTReg, pt, 3,GBmpNum,GBmpTmpNdxF, GBmpTmpDatF);  //删除多余三角形
				delete[] pt;
				GReg.datOff += (GTReg.dotNum * sizeof(D_DOT));  //全局区数据节点偏移更新
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));  //恢复临时区索引
			}
			else if (GTReg.dotNum == 2)  //两个点相当于取消绘制
			{
				//同上，删除三角形
				POINT *pt = new POINT[3];
				D_DOT dot;
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[0], dot);
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, GTReg.dotNum - 1, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				pt[2] = point;
				DrawReg(&dc, GTReg, pt, 3, GBmpNum, GBmpTmpNdxF,GBmpTmpDatF);
				delete[] pt;
				memset(&GTReg, 0, sizeof(REG_NDX_STRU)); //恢复临时区索引
			}
			else if (GTReg.dotNum == 1)  //一个点同样相当于取消绘制
			{
				//删除绘制的起点到鼠标点的线
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern,tln.color = GTReg.color };
				DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));  //恢复临时区索引
			}
			//恢复起点与鼠标上一位置点数据
			GRegCreateMMPnt = CPoint(-1, -1);
			GRegCreateStartPnt = CPoint(-1, -1);
			break;
		}
	}
}

void CMapEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapEditorView 诊断

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView 消息处理程序


void CMapEditorView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated&&GLinFCreated&&GRegFCreated&&GBmpFCreated)
	{
		MessageBox(TEXT("文件已经创建。"), TEXT("提示"), MB_OK);
		return;
	}
	CCreateFileDlg dlg;
	if (!(dlg.DoModal() == IDOK))
		return;
	CString str;//保存输出信息
	if (!GPntFCreated)
	{
		GPntTmpFName = dlg.m_add + CString("\\") + GPntTmpFName;  //文件名连接
		if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GPntFCreated = true;
			str += "tempPntF.dat\n";
		}
		else
		{
			GPntTmpFName = CString("tempPntF.dat");
			TRACE(TEXT("File could not be opened\n"));  //调试输出
		}
	}
	if (!GLinFCreated)
	{
		GLinTmpNdxFName = dlg.m_add + CString("\\") + GLinTmpNdxFName;  //文件名连接
		GLinTmpDatFName = dlg.m_add + CString("\\") + GLinTmpDatFName;  //文件名连接
		if (GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)&& GLinTmpDatF->Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GLinFCreated = true;
			str += "tempLinF.dat tempLinF.ndx\n";
		}
		else
		{
			GLinTmpDatFName = CString("tempLinF.dat"); 
			GLinTmpNdxFName = CString("tempLinF.ndx");
			TRACE(TEXT("File could not be opened n"));  //调试输出
		}
	}
	if (!GRegFCreated)
	{
		GRegTmpNdxFName = dlg.m_add + CString("\\") + GRegTmpNdxFName;  //文件名连接
		GRegTmpDatFName = dlg.m_add + CString("\\") + GRegTmpDatFName;  //文件名连接
		if (GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary) && GRegTmpDatF->Open(GRegTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GRegFCreated = true;
			str += "tempRegF.dat tempRegF.ndx\n";
		}
		else
		{
			GRegTmpDatFName = CString("tempRegF.dat");
			GRegTmpNdxFName = CString("tempRegF.ndx");
			TRACE(TEXT("File could not be opened n"));  //调试输出
		}
	}
	if (!GBmpFCreated)
	{
		GBmpTmpNdxFName = dlg.m_add + CString("\\") + GBmpTmpNdxFName;  //文件名连接
		GBmpTmpDatFName = dlg.m_add + CString("\\") + GBmpTmpDatFName;  //文件名连接
		if (GBmpTmpNdxF->Open(GBmpTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary) && GBmpTmpDatF->Open(GBmpTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
		{
			GBmpFCreated = true;
			str += "tempBmpF.dat tempBmpF.ndx\n";
		}
		else
		{
			GRegTmpDatFName = CString("tempBmpF.dat");
			GRegTmpNdxFName = CString("tempBmpF.ndx");
			TRACE(TEXT("File could not be opened n"));  //调试输出
		}
	}
	str += "创建成功。";
	MessageBox(str, TEXT("提示"), MB_OK);  //输出创建信息
}


void CMapEditorView::OnFileOpenPoint()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = TEXT("pnt\0*.pnt");
	if (GPntChanged)  /// 文件更改 提示是否保存后在打开
	{
		if (IDYES == MessageBox(TEXT("文件未保存。\n是否需要保存点文件？"), TEXT("提示"), MB_YESNO | MB_ICONQUESTION))
		{
			OnFileSavePoint();
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GPntFName = dlg.m_ofn.lpstrFile;   //获取选择得永久文件路径
		CFile *pntF = new CFile();
		if (!pntF->Open(GPntFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(TEXT("打开文件失败！"));
			return;
		}
		int nPos = GPntFName.ReverseFind(TEXT('\\'));
		CString floderAdd = GPntFName.Left(nPos);
		if (GPntTmpFName != "tempPntF.dat") //如果临时文件已经创建 则 定义到文件头
		{
			GPntTmpF->SeekToBegin();
		}
		else
		{
			GPntTmpFName = floderAdd + CString("\\") + GPntTmpFName;
			if (!GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GPntTmpFName = CString("tempPntF.dat");
				TRACE(TEXT("无法打开文件！"));
			}
			else
			{
				GPntFCreated = true;
			}
		}
		ReadPntPermanentFileToTemp(pntF, GPntTmpF, GPntNum, GPntLNum);  //从永久文件读取到临时文件
		pntF->Close();
		delete pntF;
		CString windowText = dlg.GetFileName() + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL); //无效窗口 -》 重绘（OnDraw）
	}
	GCurOperState = Noaction;
}


void CMapEditorView::OnFileOpenLine()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = TEXT("lin\0*.lin");
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("文件还未保存。是否要保存？"), MB_YESNO | MB_ICONQUESTION))
		{
			OnFileSaveLine();
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GLinFCreated = false;
		int IsCreate = 0;
		GLinFName = dlg.m_ofn.lpstrFile;  //永久文件名称
		CFile *LinF = new CFile();
		if (!LinF->Open(GLinFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(TEXT("无法打开文件\n"));
			return;
		}
		int nPos = GLinFName.ReverseFind(TEXT('\\'));
		CString floderAdd = GLinFName.Left(nPos);
		if (GLinTmpDatFName != "tempLinF.dat")
		{
			GLinTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpDatFName = floderAdd + CString("\\") + GLinTmpDatFName;
			if (!GLinTmpDatF->Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpDatFName = CString("tempLinF.dat");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (GLinTmpNdxFName != "tempLinF.ndx")
		{
			GLinTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpNdxFName = floderAdd + CString("\\") + GLinTmpNdxFName;
			if (!GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpNdxFName = CString("tempLinF.ndx");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (2 == IsCreate)
			GLinFCreated = true;
		ReadLinPermanentFileToTemp(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, GLinNum, GLinLNum, GLin.datOff);
		LinF->Close();
		delete LinF;
		CString windowText = dlg.GetFileName() + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = TEXT("reg\0*.reg");
	if (GRegChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("文件还未保存。是否要保存？"), MB_YESNO | MB_ICONQUESTION))
		{
			OnFileSaveRegion();
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GRegFCreated = false;
		int IsCreate = 0;
		GRegFName = dlg.m_ofn.lpstrFile;  //永久文件名称
		CFile *RegF = new CFile();
		if (!RegF->Open(GRegFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(TEXT("无法打开文件\n"));
			return;
		}
		int nPos = GRegFName.ReverseFind(TEXT('\\'));
		CString floderAdd = GRegFName.Left(nPos);
		if (GRegTmpDatFName != "tempRegF.dat")
		{
			GRegTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GRegTmpDatFName = floderAdd + CString("\\") + GRegTmpDatFName;
			if (!GRegTmpDatF->Open(GRegTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegTmpDatFName = CString("tempRegF.dat");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (GRegTmpNdxFName != "tempRegF.ndx")
		{
			GRegTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GRegTmpNdxFName = floderAdd + CString("\\") + GRegTmpNdxFName;
			if (!GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegTmpNdxFName = CString("tempRegF.ndx");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (2 == IsCreate)
			GRegFCreated = true;
		ReadRegPermanentFileToTemp(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum, GReg.datOff);
		RegF->Close();
		delete RegF;
		CString windowText = dlg.GetFileName() + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileSavePoint()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	if (GPntFCreated == false)
	{
		MessageBox(TEXT("点临时文件还未创建。"), TEXT("提示"),MB_OK);
		return;
	}
	CFile *pntF = new CFile();
	if (GPntFName.IsEmpty() == false)  ///删除原有得永久文件
	{
		CFile::Remove(GPntFName);
	}
	else
	{
		LPCTSTR lpszFilters;                    ///获取地址
		lpszFilters = TEXT("点(*.pnt)|*.pnt||");
		CFileDialog dlg(false, TEXT("pnt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);   //false 另存问对话框 true打开对话框
		int nPos = GPntTmpFName.ReverseFind(TEXT('\\')); //最后一个反斜杠
		CString folderAdd = GPntTmpFName.Left(nPos);
		dlg.m_ofn.lpstrInitialDir = folderAdd;
		if (dlg.DoModal() == IDOK)
			GPntFName = dlg.GetPathName();
		else return;
	}
	PNT_STRU tempPnt;
	if (pntF->Open(GPntFName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))   ///写入版本数量信息
	{
		pntF->Write(&GPntVer, sizeof(VERSION));
		pntF->Write(&GPntNum, sizeof(int));
		pntF->Write(&GPntLNum, sizeof(int));
	}
	else
	{
		TRACE(TEXT("无法打开文件"));
	}
	for (int i = 0; i < GPntNum; i++)
	{
		ReadTempFileToPnt(GPntTmpF, i, tempPnt);
		pntF->Write(&tempPnt, sizeof(PNT_STRU));
	}
	pntF->Close();
	delete pntF;
	GPntChanged = false;
	int nPos = GPntFName.ReverseFind(TEXT('\\'));
	CString windowText = GPntFName.Right(GPntFName.GetLength() - nPos - 1) + " - MapEditor";
	GetParent()->SetWindowTextW(windowText);
}


void CMapEditorView::OnFileSaveLine()  //保存线(文件->保存->保存线)响应函数
{
	if (GLinFCreated)  //文件还未创建
	{
		CFile *LinF = new CFile();  //永久存储文件
		if (GLinFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = TEXT("线(*.lin)|*.lin||");  //设置打开文件类型
			CFileDialog dlg(false, TEXT("lin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);  //初始化窗口
			int nPos = GLinTmpDatFName.ReverseFind(TEXT('\\'));//查找最后一个“\”
			CString folderAdd = GLinTmpDatFName.Left(nPos); //去除文件名称
			dlg.m_ofn.lpstrInitialDir = folderAdd;  //设置默认路径
			if (dlg.DoModal() == IDOK) //子窗口方式打开
			{
				GLinFName = dlg.GetPathName();  //设置文件路径
			}
			else  //选择取消 直接退出
				return;
		}
		else
		{
			LinF->Remove(GLinFName);
		}
		if (!LinF->Open(GLinFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) //判断是否打开成功
		{
			TRACE(TEXT("无法打开文件"));
			return;
		}
		WriteTempToLinePermanentFile(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer, GLinNum, GLinLNum);  //临时文件写道永久文件
		LinF->Close();
		delete LinF;
		GLinChanged = false;  //保存文件后变更状态改变
		int nPos = GLinFName.ReverseFind(TEXT('\\'));
		CString windowText = GLinFName.Right(GLinFName.GetLength() - nPos - 1) + " - MapEditor";  //设置标题
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(TEXT("线临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnFileSaveRegion()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		CFile *RegF = new CFile();
		if (GRegFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = TEXT("区(*.reg)|*.reg||");
			CFileDialog dlg(false, TEXT("reg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);
			int nPos = GRegTmpDatFName.ReverseFind(TEXT('\\'));
			CString folderAdd = GRegTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GRegFName = dlg.GetPathName();
			}
			else
			{
				return;
			}
		}
		else
		{
			RegF->Remove(GRegFName);
		}
		if (!RegF->Open(GRegFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(TEXT("无法打开文件"));
			return;
		}
		WriteTempToRegPermanentFile(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum);
		RegF->Close();
		delete RegF;
		GRegChanged = false;  //保存文件后变更状态改变
		int nPos = GRegFName.ReverseFind(TEXT('\\'));
		CString windowText = GRegFName.Right(GRegFName.GetLength() - nPos - 1) + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(TEXT("区临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnFileSaveAsPoint()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GPntFName;  //存储文件名
	GPntFName = CString("");
	OnFileSavePoint();
	if (GPntFName == "")
	{
		GPntFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GLinFName;  //存储文件名
	GLinFName = CString("");
	OnFileSaveLine();
	if (GLinFName == "")
	{
		GLinFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsRegion()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GRegFName;  //存储文件名
	GRegFName = CString("");
	OnFileSaveRegion();
	if (GRegFName == "")
	{
		GRegFName = tempFName;
	}
}


void CMapEditorView::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("点文件还未保存,是否要保存文件？"),MB_YESNO|MB_ICONQUESTION))  //自动分配安全的s父hWnd
		{
			OnFileSavePoint();
		}
	}
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("线文件还未保存,是否要保存文件？"), MB_YESNO | MB_ICONQUESTION))  //自动分配安全的s父hWnd
		{
			OnFileSaveLine();
		}
	}
	if (GRegChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("面文件还未保存,是否要保存文件？"), MB_YESNO | MB_ICONQUESTION))  //自动分配安全的s父hWnd
		{
			OnFileSaveRegion();
		}
	}
	if (GBmpChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("位图文件还未保存,是否要保存文件？"), MB_YESNO | MB_ICONQUESTION))  //自动分配安全的s父hWnd
		{
			OnFileSaveBmp();
		}
	}
	GetParent()->DestroyWindow();
}


void CMapEditorView::OnWindowZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_IN;
	}
	else
	{
		MessageBox(TEXT("临时文件还未打开"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnWindowZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_OUT;
	}
	else
	{
		MessageBox(TEXT("临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnWindowMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_WINDOW_MOVE;
	}
	else
	{
		MessageBox(TEXT("临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnWindowReset()
{
	// TODO: 在此添加命令处理程序代码
	GCurShowState = SHOWSTATE_UNDEL;
	this->Invalidate();
	GShowPnt = true;
	GShowLin = true;
	GShowReg = true;
	//重置偏移放大倍数
	GZoomOffset_x = 0;
	GZoomOffset_y = 0;
	GZoom = 1.0;
	//遍历图像
	D_DOT tempPt;
	PNT_STRU tempPnt;
	LIN_NDX_STRU tempLin;
	REG_NDX_STRU tempReg;
	bool isInit = false;
	if (GPntLNum == 0 && GLinLNum == 0 && GRegLNum == 0)
		return;
	//初始化
	if (isInit == false && GPntLNum > 0)
	{
		for (int i = 0; i < GPntNum; i++)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);
			if (tempPnt.isDel)
				continue;
			else
			{
				GMaxX = tempPnt.x;
				GMinX = tempPnt.x;
				GMaxY = tempPnt.y;
				GMinY = tempPnt.y;
				isInit = true;
				break;
			}
		}
	}
	if (isInit == false && GLinLNum > 0)
	{
		for (int i = 0; i < GLinNum && !isInit; i++)   //BUG -> 重复初始化
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum && !isInit; j++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	if (isInit == false && GRegLNum > 0)
	{
		for (int i = 0; i < GRegNum && !isInit; i++)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF,i,tempReg);
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum && !isInit; j++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	if (isInit == false)
	{
		this->Invalidate();
		return;
	}
	if (GPntFCreated)
	{
		for (int i = 0; i < GPntNum; i++)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);
			if (tempPnt.isDel)
				continue;
			else
			{
				if (tempPnt.x > GMaxX)
					GMaxX = tempPnt.x;
				if (tempPnt.y > GMaxY)
					GMaxY = tempPnt.y;
				if (tempPnt.x < GMinX)
					GMinX = tempPnt.x;
				if (tempPnt.y < GMinY)
					GMinY = tempPnt.y;
			}
		}
	}
	if (GLinFCreated)
	{
		for (int i = 0; i < GLinNum; i++)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);
			if (tempLin.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempLin.dotNum; j++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	if (GRegFCreated)
	{
		for (int i = 0; i < GRegLNum; i++)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);
			if (tempReg.isDel)
				continue;
			else
			{
				for (int j = 0; j < tempReg.dotNum; j++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	GMaxX += 20;
	GMinX -= 20;
	GMaxY += 20;
	GMinY -= 20;
	RECT rect, client;
	double zoom;
	GetClientRect(&client);
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	modulusZoom(client, rect, zoom);
	GMaxX += 20 / zoom;
	GMinX -= 20 / zoom;
	GMaxY += 20 / zoom;
	GMinY -= 20 / zoom;
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	modulusZoom(client, rect, zoom);
	double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right*(zoom - 1) / (2.0*zoom));
	double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom*(zoom - 1) / (2.0*zoom));
	GZoomOffset_x += (x0 / zoom);
	GZoomOffset_y += (y0 / zoom);
	GZoom *= zoom;
	GCurOperState = Noaction;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowPoint()
{
	// TODO: 在此添加命令处理程序代码
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	if (GShowPnt == true)
		GShowPnt = false;
	else
		GShowPnt = true;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowLine()
{
	// TODO: 在此添加命令处理程序代码
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	if (GShowLin == true)
		GShowLin = false;
	else
		GShowLin = true;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowRegion()
{
	// TODO: 在此添加命令处理程序代码
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL;
	if (GShowReg == true)
		GShowReg = false;
	else
		GShowReg = true;
	this->Invalidate();
}


void CMapEditorView::OnPointCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_PNT;  //设置状态
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
		this->Invalidate();
	}
	else
	{
		MessageBox(TEXT("文件还未创建"), TEXT("提示"),MB_OK);
	}
}


void CMapEditorView::OnPointMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_PNT;
	}
	else
	{
		MessageBox(TEXT("文件还未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnPointDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_PNT;  //设置操作状态
	}
	else
	{
		MessageBox(TEXT("临时点文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnPointShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	else if (GCurShowState != SHOWSTATE_DEL && GShowPnt != true)
	{
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->Invalidate();
}


void CMapEditorView::OnPointUndelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_PNT;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnPointModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_POINT_PARAMETER;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnPonitSetDeeparameter()
{
	// TODO: 在此添加命令处理程序代码
	CPointParamerterDlg dlg;
	dlg.m_Pattern = GPnt.pattern;
	dlg.m_ColorButton.SetColor(GPnt.color);
	if (IDOK == dlg.DoModal())
	{
		GPnt.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GPnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));   //复制颜色！！！？？？---》》COLORREF   unsigined long
	}
}


void CMapEditorView::OnLineCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_LIN;
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
		this->Invalidate();
	}
	else
	{
		MessageBox(TEXT("线临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_LIN;
	}
	else
	{
		MessageBox(TEXT("线临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_LIN;
	}
	else
	{
		MessageBox(TEXT("临时线文件还未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	else if (GCurShowState != SHOWSTATE_DEL && GShowLin != true)
	{
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->Invalidate();
}


void CMapEditorView::OnLineUndelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_LIN;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineDeleteDot()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LIN_DELETE_PNT;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineAddDot()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LIN_ADD_PNT;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineLink()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LINK_LIN;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_lin_PARAMETER;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnLineSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
	CLineParamerDlg dlg;
	dlg.m_Patttern = GLin.pattern;
	dlg.m_ColorBox.SetColor(GLin.color);
	if (IDOK == dlg.DoModal())
	{
		GLin.pattern = dlg.m_Patttern;
		COLORREF tempColor = dlg.m_ColorBox.GetColor();
		memcpy_s(&GLin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnRegionCreate()         //点击造区(区编辑->造区)响应事件
{
	if (GRegFCreated)                         //判断文件是否创建
	{
		GCurOperState = OPERSTATE_INPUT_REG;  //设置操作状态
		//设置显示状态
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
		this->Invalidate();                   //重绘窗口
	}
	else
	{
		MessageBox(TEXT("区临时文件未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnRegionMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_REG;
	}
	else
	{
		MessageBox(TEXT("区临时文件未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnRegionDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_REG;
	}
	else
	{
		MessageBox(TEXT("区临时文件未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnRegionShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	else if (GCurShowState != SHOWSTATE_DEL && GShowReg != true)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->Invalidate();
}


void CMapEditorView::OnRegionUndelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_REG;
		GCurShowState = SHOWSTATE_DEL;
		this->Invalidate();
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnRegionModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_REGION_PARAMETER;
	}
	else
	{
		MessageBox(TEXT("临时文件未创建"), TEXT("提示"), MB_OK);
	}
}

void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
			DotToPnt(point, dot);
			GTPnt = FindPnt(point, GPntNum, GPntTmpF, GPntNdx);
			dot.x = GTPnt.x;
			dot.y = GTPnt.y;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			GTPnt.x = dot.x;
			GTPnt.y = dot.y;
			break;
		case OPERSTATE_DELETE_PNT:  //删除点左键按下
			GPntLBDPnt = point;  //存储左键按下点
			GPntMMPnt = point;   //初始化上次鼠标位置
			break;
		case OPERSTATE_MODIFY_POINT_PARAMETER:
			GPntLBDPnt = point;
			GPntMMPnt = point;
			break;
		}
	}
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_LIN:
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
			DotToPnt(point, dot);
			GLinMMTmpNdx = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx,10);  //查找最近的线
			GLinMMOffsetX = 0;
			GLinMMOffsetY = 0;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
			DotToPnt(point, dot);
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		case OPERSTATE_DELETE_LIN:
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		case OPERSTATE_MODIFY_lin_PARAMETER:
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		//造区左键按下事件
		case OPERSTATE_MOVE_REG:
			GRegLBDPnt = point;
			GRegMMPnt = point;
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
			DotToPnt(point, dot);
			GRegMMTmpNdx = FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);  //查找最近的区
			GRegMMOffsetX = 0;
			GRegMMOffsetY = 0;
			break;
		case OPERSTATE_DELETE_REG:
			GRegLBDPnt = point;
			GRegMMPnt = point;
			break;
		case OPERSTATE_MODIFY_REGION_PARAMETER:
			GRegLBDPnt = point;
			GRegMMPnt = point;
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:
			GZoomLBDPnt = point;
			GZoomMMPnt = point;
			break;
		case OPERSTATE_WINDOW_MOVE:
			GWinMoveLBDPnt = point;
			GWinMoveMMPnt = point;
			break;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN); //绘图模式
	D_DOT dot;  //用于坐标转换
	RECT client,rect;
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_PNT:
			PNT_STRU pnt;
			memcpy_s(&pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));  //默认参数初始化
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
			pnt.x = dot.x;
			pnt.y = dot.y;
			WritePntToFile(GPntTmpF, GPntNum, pnt);
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
			pnt.x = dot.x;
			pnt.y = dot.y;
			DrawPnt(&dc, pnt);
			GPntNum++;   //物理数量
			GPntLNum++;  //逻辑数量
			GPntChanged = true;
			break;
		case OPERSTATE_DELETE_PNT:  //删除点左键抬起事件
			GetClientRect(&client);  //获取View窗口的矩形
			//根据拖行距离判断删除模式
			if (abs(GPntLBDPnt.x - GPntMMPnt.x) <= 10 && abs(GPntLBDPnt.y - GPntMMPnt.y) <= 10)
				GPntStytle = 0;  //点选
			else
				GPntStytle = 1;  //框选
			if (GPntStytle == 0)
			{
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
				DotToPnt(point, dot);
				FindPnt(point, GPntNum, GPntTmpF, GPntNdx);   //GPntNdx用于存放查找返回值
				if (GPntNdx != -1)
				{
					PNT_STRU pnt;
					ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);  //读取点数据
					pnt.isDel = 1;  //标记删除
					UpdatePnt(GPntTmpF, GPntNdx, pnt);    //将原有位置的点数据替换为更改后的点数据
					dot.x = pnt.x;
					dot.y = pnt.y;
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
					pnt.x = dot.x;
					pnt.y = dot.y;
					DrawPnt(&dc, pnt);
					GPntNdx = -1;
					GPntChanged = true;
					GPntLNum--;
				}
			}
			else
			{
				PNT_STRU tempPnt;
				rect.right = max(point.x, GPntLBDPnt.x);   //确保左上右下
				rect.left = min(point.x, GPntLBDPnt.x);
				rect.bottom = max(point.y, GPntLBDPnt.y);
				rect.top = min(point.y, GPntLBDPnt.y);
				D_DOT rect_d_dot[4];
				rect_d_dot[0].y = rect.top;//存储四个顶点
				rect_d_dot[0].x = rect.left;
				rect_d_dot[1].y = rect.top;
				rect_d_dot[1].x = rect.right;
				rect_d_dot[2].y = rect.bottom;
				rect_d_dot[2].x = rect.right;
				rect_d_dot[3].y = rect.bottom;
				rect_d_dot[3].x = rect.left;
				for (int i = 0; i < 4; i++)  //转换视图坐标到数据坐标
				{
					PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				for (int i = 0; i < GPntNum; i++)  //遍历所有点
				{
					ReadTempFileToPnt(GPntTmpF, i, tempPnt);  //读取点数据
					if (tempPnt.isDel == 0)
					{
						if (PtInPolygon(CPoint(tempPnt.x, tempPnt.y), rect_d_dot, 4))  //判断点是否在框选内部
						{
							tempPnt.isDel = 1;
							UpdatePnt(GPntTmpF, i, tempPnt);    //将原有位置的点数据替换为更改后的点数据
							dot.x = tempPnt.x;
							dot.y = tempPnt.y;
							PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
							tempPnt.x = dot.x;
							tempPnt.y = dot.y;
							DrawPnt(&dc, tempPnt);  //异或绘制删除
							GPntChanged = true;
							GPntLNum--;
						}
					}
				}
			}
			GPntLBDPnt = CPoint(-1, -1);  //恢复左键按下位置
			GPntMMPnt = CPoint(-1, -1);  //恢复上次移动位置
			this->Invalidate();  //重绘
			break;
		case OPERSTATE_MODIFY_POINT_PARAMETER:
			GetClientRect(&client);
			if (abs(GPntLBDPnt.x - GPntMMPnt.x) <= 10 && abs(GPntLBDPnt.y - GPntMMPnt.y) <= 10)
			{
				GPntStytle = 0;
			}
			else
			{
				GPntStytle = 1;
			}
			if (GPntStytle == 0)
			{
				int m_select;
				int m_pattern;
				COLORREF m_color;
				CSelectPoint dlgs;   //筛选

				if (dlgs.DoModal() == IDOK)
				{
					if (dlgs.m_select == 0)
						break;
					else if (dlgs.m_select == 1)
					{
						m_select = dlgs.m_select;
						m_pattern = dlgs.m_pattern;
					}
					else if (dlgs.m_select == 2)
					{
						m_select = dlgs.m_select;
						m_color = dlgs.m_colorbox.GetColor();
					}
					else if (dlgs.m_select == 3)
					{
						m_select = dlgs.m_select;
					}
				}
				else
				{
					GPntLBDPnt = CPoint(-1, -1);
					GPntMMPnt = CPoint(-1, -1);
					this->Invalidate();
					break;
				}
				CPointParamerterDlg dlg;  //参数
				PNT_STRU select_pnt = GPnt;
				dlg.m_Pattern = select_pnt.pattern;
				dlg.m_ColorButton.SetColor(select_pnt.color);
				if (IDOK == dlg.DoModal())
				{
					select_pnt.pattern = dlg.m_Pattern;
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&select_pnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));   //复制颜色！！！？？？---》》COLORREF   unsigined long
				}
				for (int i = 0; i < GPntNum; i++)
				{
					PNT_STRU pnt;
					ReadTempFileToPnt(GPntTmpF, i, pnt);
					if (pnt.isDel == 1)
						continue;
					dot.x = pnt.x;
					dot.y = pnt.y;
					PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
					pnt.x = dot.x;
					pnt.y = dot.y;
					if ((m_select == 1 && pnt.pattern == m_pattern) || (m_select == 2 && pnt.color == m_color) || m_select == 3)
					{
						DrawPnt(&dc, pnt);  //删除原有点
						pnt.pattern = select_pnt.pattern;
						pnt.color = select_pnt.color;
						DrawPnt(&dc, pnt);  //重新绘制
						dot.x = pnt.x;
						dot.y = pnt.y;
						PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
						pnt.x = dot.x;
						pnt.y = dot.y;
						UpdatePnt(GPntTmpF, i, pnt);    //将原有位置的点数据替换为更改后的点数据
						GPntNdx = -1;
						GPntChanged = true;
					}
				}
			}
			else
			{
				PNT_STRU tempPnt;
				rect.right = max(point.x, GPntLBDPnt.x);   //确保左上右下
				rect.left = min(point.x, GPntLBDPnt.x);
				rect.bottom = max(point.y, GPntLBDPnt.y);
				rect.top = min(point.y, GPntLBDPnt.y);
				D_DOT rect_d_dot[4];
				rect_d_dot[0].y = rect.top;
				rect_d_dot[0].x = rect.left;
				rect_d_dot[1].y = rect.top;
				rect_d_dot[1].x = rect.right;
				rect_d_dot[2].y = rect.bottom;
				rect_d_dot[2].x = rect.right;
				rect_d_dot[3].y = rect.bottom;
				rect_d_dot[3].x = rect.left;
				for (int i = 0; i < 4; i++)
				{
					PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				CPointParamerterDlg dlg;  //参数
				PNT_STRU select_pnt = GPnt;
				dlg.m_Pattern = select_pnt.pattern;
				dlg.m_ColorButton.SetColor(select_pnt.color);
				if (IDOK == dlg.DoModal())
				{
					select_pnt.pattern = dlg.m_Pattern;
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&select_pnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));   //复制颜色！！！？？？---》》COLORREF   unsigined long
				}
				for (int i = 0; i < GPntNum; i++)
				{
					ReadTempFileToPnt(GPntTmpF, i, tempPnt);
					if (tempPnt.isDel == 0)
					{
						if (PtInPolygon(CPoint(tempPnt.x, tempPnt.y), rect_d_dot, 4))
						{
							PNT_STRU pnt = tempPnt;
							dot.x = pnt.x;
							dot.y = pnt.y;
							PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
							pnt.x = dot.x;
							pnt.y = dot.y;
							DrawPnt(&dc, pnt);  //删除原有点
							pnt.pattern = select_pnt.pattern;
							pnt.color = select_pnt.color;
							DrawPnt(&dc, pnt);  //重新绘制
							dot.x = pnt.x;
							dot.y = pnt.y;
							PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
							pnt.x = dot.x;
							pnt.y = dot.y;
							UpdatePnt(GPntTmpF, i, pnt);    //将原有位置的点数据替换为更改后的点数据
							GPntNdx = -1;
							GPntChanged = true;
						}
					}
				}
			}
			GPntLBDPnt = CPoint(-1, -1);
			GPntMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		case OPERSTATE_MOVE_PNT:
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);   //坐标系转换
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);
				pnt.x = point.x;
				pnt.y = point.y;
				UpdatePnt(GPntTmpF, GPntNdx, pnt);
				GPntNdx = -1;
				GPntChanged = true;
			}
			break;
		case OPERSTATE_UNDELETE_PNT:
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			FindDeletePnt(point, GPntNum,GPntTmpF, GPntNdx);
			if (GPntNdx != -1)
			{
				GPntLNum++;
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);
				pnt.isDel = 0;
				UpdatePnt(GPntTmpF, GPntNdx, pnt);
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);
				GPntChanged = true;
				GPntNdx = -1;
			}
			break;
		}
	}
	if (GLinFCreated)
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum == 0)
			{
				memcpy_s(&GTLin, sizeof(LIN_NDX_STRU), &GLin, sizeof(LIN_NDX_STRU));  //默认参数初始化
			}
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
			WriteLinDatToFile(GLinTmpDatF, GLin.datOff, GTLin.dotNum, dot);  //写入折线节点数据
			GTLin.dotNum++;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
			GLPnt.x = (long)dot.x;  //设置线起点坐标
			GLPnt.y = (long)dot.y;
			GLinChanged = true;
			break;
		case OPERSTATE_DELETE_LIN:
			GetClientRect(&client);
			if (abs(GLinLBDPnt.x - GLinMMPnt.x) <= 10 && abs(GLinLBDPnt.y - GLinMMPnt.y) <= 10)
			{
				GLinStytle = 0;
			}
			else
			{
				GLinStytle = 1;
			}
			if (GLinStytle == 0)
			{
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
				DotToPnt(point, dot);
				FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx, 10);
				if (GLinNdx != -1)
				{
					GLinLNum--;   //逻辑数减少
					GLinChanged = true;
					LIN_NDX_STRU TmpLinNdx;
					D_DOT dot1, dot2;
					POINT pnt1, pnt2;
					ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
					TmpLinNdx.isDel = 1;
					UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);  //更新线数据
					for (int i = 0; i < TmpLinNdx.dotNum - 1; i++)  //删除线
					{
						ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
						ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
						PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
						PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
						DotToPnt(pnt1, dot1);
						DotToPnt(pnt2, dot2);
						DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
					}
					GLinNdx = -1;
				}
			}
			else
			{
				LIN_NDX_STRU tempLinNdx;
				rect.right = max(point.x, GLinLBDPnt.x);   //确保左上右下
				rect.left = min(point.x, GLinLBDPnt.x);
				rect.bottom = max(point.y, GLinLBDPnt.y);
				rect.top = min(point.y, GLinLBDPnt.y);
				D_DOT rect_d_dot[4];
				rect_d_dot[0].y = rect.top;
				rect_d_dot[0].x = rect.left;
				rect_d_dot[1].y = rect.top;
				rect_d_dot[1].x = rect.right;
				rect_d_dot[2].y = rect.bottom;
				rect_d_dot[2].x = rect.right;
				rect_d_dot[3].y = rect.bottom;
				rect_d_dot[3].x = rect.left;
				for (int i = 0; i < 4; i++)
				{
					PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				bool LinInRect = true;
				D_DOT tdot;
				for (int i = 0; i < GLinNum; i++)
				{
					ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLinNdx);
					if (tempLinNdx.isDel == 0)
					{
						LinInRect = true;
						for (int j = 0; j < tempLinNdx.dotNum; j++)
						{
							ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, j, tdot);
							if (!PtInPolygon(CPoint(tdot.x, tdot.y), rect_d_dot, 4))
							{
								LinInRect = false;
								break;
							}
						}
						if (LinInRect)
						{
							GLinLNum--;   //逻辑数减少
							GLinChanged = true;
							D_DOT dot1, dot2;
							POINT pnt1, pnt2;
							tempLinNdx.isDel = 1;
							UpdateLin(GLinTmpNdxF, i, tempLinNdx);  //更新线数据
							for (int i = 0; i < tempLinNdx.dotNum - 1; i++)  //删除线
							{
								ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i, dot1);
								ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i + 1, dot2);
								PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								DotToPnt(pnt1, dot1);
								DotToPnt(pnt2, dot2);
								DrawSeg(&dc, tempLinNdx, pnt1, pnt2);
							}
						}
					}
				}
			}
			GLinLBDPnt = CPoint(-1, -1);
			GLinMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
			case OPERSTATE_MODIFY_lin_PARAMETER:
				GetClientRect(&client);
				if (abs(GLinLBDPnt.x - GLinMMPnt.x) <= 10 && abs(GLinLBDPnt.y - GLinMMPnt.y) <= 10)
				{
					GLinStytle = 0;
				}
				else
				{
					GLinStytle = 1;
				}
				if (GLinStytle == 0)
				{
					int m_select;
					int m_pattern;
					COLORREF m_color;
					CSelectLine dlgs;   //筛选
					if (dlgs.DoModal() == IDOK)
					{
						if (dlgs.m_select == 0)
							break;
						else if (dlgs.m_select == 1)
						{
							m_select = dlgs.m_select;
							m_pattern = dlgs.m_pattern;
						}
						else if (dlgs.m_select == 2)
						{
							m_select = dlgs.m_select;
							m_color = dlgs.m_colorbox.GetColor();
						}
						else if (dlgs.m_select == 3)
						{
							m_select = dlgs.m_select;
						}
					}
					else
					{
						GLinLBDPnt = CPoint(-1, -1);
						GLinMMPnt = CPoint(-1, -1);
						this->Invalidate();
						break;
					}
					LIN_NDX_STRU select_lin = GLin;
					CLineParamerDlg dlg;  //参数
					dlg.m_Patttern = select_lin.pattern;
					dlg.m_ColorBox.SetColor(select_lin.color);
					if (IDOK == dlg.DoModal())
					{
						select_lin.pattern = dlg.m_Patttern;
						COLORREF tempColor = dlg.m_ColorBox.GetColor();
						memcpy_s(&select_lin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					}
					for (int i = 0; i < GLinNum; i++)
					{
						LIN_NDX_STRU TmpLinNdx;
						D_DOT dot1, dot2;
						POINT pnt1, pnt2;
						ReadTempFileToLinNdx(GLinTmpNdxF, i, TmpLinNdx);
						if (TmpLinNdx.isDel == 1)
						{
							continue;
						}
						if ((m_select == 1 && TmpLinNdx.pattern == m_pattern) || (m_select == 2 && TmpLinNdx.color == m_color) || m_select == 3)
						{
							for (int i = 0; i < TmpLinNdx.dotNum - 1; i++)  //删除线
							{
								ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
								ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
								PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								DotToPnt(pnt1, dot1);
								DotToPnt(pnt2, dot2);
								DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
							}
							TmpLinNdx.pattern = select_lin.pattern;
							TmpLinNdx.color = select_lin.color;
							UpdateLin(GLinTmpNdxF, i, TmpLinNdx);  //更新线数据
							for (int i = 0; i < TmpLinNdx.dotNum - 1; i++) //重绘
							{
								ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
								ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
								PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
								DotToPnt(pnt1, dot1);
								DotToPnt(pnt2, dot2);
								DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
							}
						}
					}
				}
				else
				{
					LIN_NDX_STRU tempLinNdx;
					rect.right = max(point.x, GLinLBDPnt.x);   //确保左上右下
					rect.left = min(point.x, GLinLBDPnt.x);
					rect.bottom = max(point.y, GLinLBDPnt.y);
					rect.top = min(point.y, GLinLBDPnt.y);
					D_DOT rect_d_dot[4];
					rect_d_dot[0].y = rect.top;
					rect_d_dot[0].x = rect.left;
					rect_d_dot[1].y = rect.top;
					rect_d_dot[1].x = rect.right;
					rect_d_dot[2].y = rect.bottom;
					rect_d_dot[2].x = rect.right;
					rect_d_dot[3].y = rect.bottom;
					rect_d_dot[3].x = rect.left;
					for (int i = 0; i < 4; i++)
					{
						PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					}
					LIN_NDX_STRU select_lin = GLin;
					CLineParamerDlg dlg;  //参数
					dlg.m_Patttern = select_lin.pattern;
					dlg.m_ColorBox.SetColor(select_lin.color);
					if (IDOK == dlg.DoModal())
					{
						select_lin.pattern = dlg.m_Patttern;
						COLORREF tempColor = dlg.m_ColorBox.GetColor();
						memcpy_s(&select_lin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					}
					bool LinInRect = true;
					D_DOT tdot;
					for (int i = 0; i < GLinNum; i++)
					{
						ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLinNdx);
						if (tempLinNdx.isDel == 0)
						{
							LinInRect = true;
							for (int j = 0; j < tempLinNdx.dotNum; j++)
							{
								ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, j, tdot);
								if (!PtInPolygon(CPoint(tdot.x, tdot.y), rect_d_dot, 4))
								{
									LinInRect = false;
									break;
								}
							}
							if (LinInRect)
							{
								GLinChanged = true;
								D_DOT dot1, dot2;
								POINT pnt1, pnt2;
								for (int i = 0; i < tempLinNdx.dotNum - 1; i++)  //删除线
								{
									ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i, dot1);
									ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i + 1, dot2);
									PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
									PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
									DotToPnt(pnt1, dot1);
									DotToPnt(pnt2, dot2);
									DrawSeg(&dc, tempLinNdx, pnt1, pnt2);
								}
								tempLinNdx.pattern = select_lin.pattern;
								tempLinNdx.color = select_lin.color;
								for (int i = 0; i < tempLinNdx.dotNum - 1; i++) //重绘
								{
									ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i, dot1);
									ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, i + 1, dot2);
									PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
									PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
									DotToPnt(pnt1, dot1);
									DotToPnt(pnt2, dot2);
									DrawSeg(&dc, tempLinNdx, pnt1, pnt2);
								}
								UpdateLin(GLinTmpNdxF, i, tempLinNdx);  //更新线数据
							}
						}
					}
				}
				GLinLBDPnt = CPoint(-1, -1);
				GLinMMPnt = CPoint(-1, -1);
				this->Invalidate();
				break;
		case OPERSTATE_MOVE_LIN:
			if (GLinNdx != -1)
			{
				if (GLinLBDPnt.x != -1 && GLinLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					PntToDot(dot2, GLinLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLinNdx, offset_x, offset_y);
					GLinNdx = -1;
					GLinMMOffsetX = 0;
					GLinMMOffsetY = 0;
					GLinChanged = true;
				}
			}
			break;
		case OPERSTATE_LINK_LIN:
			if (GnLine < 2)
			{
				LIN_NDX_STRU line;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标转换
				DotToPnt(point, dot);
				line = FindLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx,10);
				if (GLinNdx != -1)
				{
					GnLine++;
					if (GnLine == 1)//choose 1
					{
						GStartLin = line;
						GnStart = GLinNdx;
					}
					else if (GnLine == 2)//choose 2
					{
						if (GnStart != GLinNdx)
						{
							GEndLin = line;
							GnEnd = GLinNdx;
						}
						else
							GnLine--;
					}
				}
				if (GnLine != 0)
				{
					D_DOT pt;
					if (GnLine == 1)
					{
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, 0, pt);   //起点画框
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 5, (long)pt.y - 5, (long)pt.x + 5, (long)pt.y + 5);
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, GStartLin.dotNum - 1, pt);   //终点画框
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);
						dc.Ellipse((long)pt.x - 5, (long)pt.y - 5, (long)pt.x + 5, (long)pt.y + 5);

					}
					else
					{
						AlterLindot(GLinTmpDatF, GStartLin, GEndLin, GnStart, GnEnd, GLin.datOff);   //改变线的点数据
						AlterStartLin(GLinTmpNdxF, GLin.datOff, GnStart, GStartLin.dotNum + GEndLin.dotNum);  //修改第一条线的索引
						AlterEndLin(GLinTmpNdxF, GnEnd);  //修改第二条线的索引
						GLin.datOff += (GStartLin.dotNum + GEndLin.dotNum) * sizeof(D_DOT);  //连接线索引
						GnLine = 0; //恢复选择线数量
						GLinLNum--; //逻辑数减少
						GLinChanged = true;
						GnStart = -1;
						GnEnd = -1;
						GLinNdx = -1;
						this->Invalidate();
					}
				}
			}
			break;
		case OPERSTATE_UNDELETE_LIN:  //恢复删除线左键弹起响应(核心部分)
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
			DotToPnt(point, dot);
			FindDeleteLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);  //查找删除的线
			if (GLinNdx != -1)
			{
				GLinLNum++;   //逻辑数增加
				GLinChanged = true;
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);
				TmpLinNdx.isDel = 0;  //设置未删除
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx);  //更新线数据
				for (int i = 0; i < TmpLinNdx.dotNum - 1; i++)  //绘制线
				{
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);  //读取节点数据
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);
				}
				GLinNdx = -1;  //重置线查找变量
			}
			break;
		case OPERSTATE_LIN_DELETE_PNT:
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			tempLinNdx = FindPntOnLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx, GLinDotNdx);
			if (GLinDotNdx != -1) 
			{
				if (tempLinNdx.dotNum <= 2) //多余两个点才可以删除
				{
					MessageBox(TEXT("点数小于二无法删除"), TEXT("提示"), MB_OK);
					break;
				}
				D_DOT tdot;
				int count = 0;
				long oldDattOffset = tempLinNdx.datOff;
				tempLinNdx.datOff = GLinTmpDatF->GetLength();
				tempLinNdx.dotNum--;
				for (int i = 0; i < tempLinNdx.dotNum; i++)
				{
					if (i != GLinDotNdx)
					{
						ReadTempFileToLinDat(GLinTmpDatF, oldDattOffset, i, tdot);
						WriteLinDatToFile(GLinTmpDatF, tempLinNdx.datOff, count++, tdot);
					}
				}
				UpdateLin(GLinTmpNdxF, GLinNdx, tempLinNdx);
				//删除线并绘制新线
				D_DOT last, the, next;
				if (GLinDotNdx != 0)
					ReadTempFileToLinDat(GLinTmpDatF, oldDattOffset, GLinDotNdx - 1, last);
				ReadTempFileToLinDat(GLinTmpDatF, oldDattOffset, GLinDotNdx, the);
				if(GLinDotNdx != tempLinNdx.dotNum)
					ReadTempFileToLinDat(GLinTmpDatF, oldDattOffset, GLinDotNdx + 1, next);
				POINT pt1, pt2, pt3;
				if (GLinDotNdx != 0)
					DotToPnt(pt1, last);
				DotToPnt(pt2, the);
				if (GLinDotNdx != tempLinNdx.dotNum)
					DotToPnt(pt3, next);
				if (GLinDotNdx != 0)
					DrawSeg(&dc, tempLinNdx, pt1, pt2);
				if (GLinDotNdx != tempLinNdx.dotNum)
					DrawSeg(&dc, tempLinNdx, pt2, pt3);
				if(GLinDotNdx != 0 && GLinDotNdx != tempLinNdx.dotNum)  //已经减少 所以不用-1
					DrawSeg(&dc, tempLinNdx, pt1, pt3);
				GLinNdx = -1;
				GLinDotNdx = -1;
				GLinChanged = true;
			}
			break;
		case OPERSTATE_LIN_ADD_PNT:
			if (GLinDotNdx1 != -1)  //防止多次点击左键
			{
				break;
			}
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			tempLinNdx = FindPntOnLin(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx, GLinDotNdx1, GLinDotNdx2);
			D_DOT dot1, dot2;
			POINT pnt1, pnt2;
			ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, GLinDotNdx1, dot1);
			ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, GLinDotNdx2, dot2);
			PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
			PntToDot(dot, point);
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(point, dot);
			PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
			PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
			DotToPnt(pnt1, dot1);
			DotToPnt(pnt2, dot2);
			DrawSeg(&dc, tempLinNdx, pnt1, pnt2);  //删除原有线
			DrawSeg(&dc, tempLinNdx, point, pnt1);
			DrawSeg(&dc, tempLinNdx, point, pnt2);
			GLinMMPnt = CPoint(point.x, point.y);  //记录位置用于删除
			break;
		}
		
	}
	if (GRegFCreated)
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		//造区左键抬起事件(核心部分)
		case OPERSTATE_INPUT_REG:
			//初始化绘制区的索引、区起点、鼠标上上一次移动点数据
			if (GTReg.dotNum == 0)
				memcpy_s(&GTReg, sizeof(REG_NDX_STRU), &GReg, sizeof(REG_NDX_STRU));
			if (GRegCreateStartPnt.x == -1 && GRegCreateStartPnt.y == -1)
				GRegCreateStartPnt = point;
			if (GRegCreateMMPnt.x == -1 && GRegCreateMMPnt.y == -1)
				GRegCreateMMPnt = point;
			PntToDot(dot, point);   //CPoint转D_DOT
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);  //窗口坐标转数据坐标
			WriteRegDatToFile(GRegTmpDatF, GReg.datOff, GTReg.dotNum, dot);  //写入创建的点
			++GTReg.dotNum;  //点数自增
			if (GTReg.dotNum == 2)
				this->Invalidate();  //重绘窗口
			GRegChanged = true;   //标记区数据更改
			break;
		case OPERSTATE_DELETE_REG:
			GetClientRect(&client);
			if (abs(GRegLBDPnt.x - GRegMMPnt.x) <= 10 && abs(GRegLBDPnt.y - GRegMMPnt.y) <= 10)
			{
				GRegStytle = 0;
			}
			else
			{
				GRegStytle = 1;
			}
			if (GRegStytle == 0)
			{
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
				DotToPnt(point, dot);
				FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);
				if (GRegNdx != -1)
				{
					GRegLNum--;
					GRegChanged = true;
					REG_NDX_STRU TmpRegNdx;
					ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);
					TmpRegNdx.isDel = 1;
					UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);
					D_DOT *dot = new D_DOT[TmpRegNdx.dotNum];
					GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
					GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
					for (int i = 0; i < TmpRegNdx.dotNum; i++)
					{
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
					}
					POINT *pnt = new POINT[TmpRegNdx.dotNum];
					DotToPnt(pnt, dot, TmpRegNdx.dotNum);
					DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
					delete[] pnt;
					delete[] dot;
					GRegNdx = -1;
				}
			}
			else
			{
				REG_NDX_STRU tempRegNdx;
				rect.right = max(point.x, GRegLBDPnt.x);   //确保左上右下
				rect.left = min(point.x, GRegLBDPnt.x);
				rect.bottom = max(point.y, GRegLBDPnt.y);
				rect.top = min(point.y, GRegLBDPnt.y);
				D_DOT rect_d_dot[4];
				rect_d_dot[0].y = rect.top;
				rect_d_dot[0].x = rect.left;
				rect_d_dot[1].y = rect.top;
				rect_d_dot[1].x = rect.right;
				rect_d_dot[2].y = rect.bottom;
				rect_d_dot[2].x = rect.right;
				rect_d_dot[3].y = rect.bottom;
				rect_d_dot[3].x = rect.left;
				for (int i = 0; i < 4; i++)
				{
					PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				bool RegInRect = true;
				D_DOT tdot;
				for (int i = 0; i < GRegNum; i++)
				{
					ReadTempFileToRegNdx(GRegTmpNdxF, i, tempRegNdx);
					if (tempRegNdx.isDel == 0)
					{
						RegInRect = true;
						for (int j = 0; j < tempRegNdx.dotNum; j++)
						{
							ReadTempFileToRegDat(GRegTmpDatF, tempRegNdx.datOff, j, tdot);
							if (!PtInPolygon(CPoint(tdot.x, tdot.y), rect_d_dot, 4))
							{
								RegInRect = false;
								break;
							}
						}
						if (RegInRect)
						{
							GRegLNum--;
							GRegChanged = true;
							tempRegNdx.isDel = 1;
							UpdateReg(GRegTmpNdxF, i, tempRegNdx);
							D_DOT *dot = new D_DOT[tempRegNdx.dotNum];
							GRegTmpDatF->Seek(tempRegNdx.datOff, CFile::begin);
							GRegTmpDatF->Read(dot, tempRegNdx.dotNum * sizeof(D_DOT));
							for (int j = 0; j < tempRegNdx.dotNum; j++)
							{
								PntDPtoVP(dot[j], GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
							}
							POINT *pnt = new POINT[tempRegNdx.dotNum];
							DotToPnt(pnt, dot, tempRegNdx.dotNum);
							DrawReg(&dc, tempRegNdx, pnt, tempRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
							delete[] pnt;
							delete[] dot;
						}
					}
				}
			}
			GRegLBDPnt = CPoint(-1, -1);
			GRegMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
			case OPERSTATE_MODIFY_REGION_PARAMETER:
				GetClientRect(&client);
				if (abs(GRegLBDPnt.x - GRegMMPnt.x) <= 10 && abs(GRegLBDPnt.y - GRegMMPnt.y) <= 10)
				{
					GRegStytle = 0;
				}
				else
				{
					GRegStytle = 1;
				}
				if (GRegStytle == 0)
				{
					int m_select;
					int m_pattern;
					COLORREF m_color;
					CSelectRegion dlgs;   //筛选
					if (dlgs.DoModal() == IDOK)
					{
						if (dlgs.m_select == 0)
							break;
						else if (dlgs.m_select == 1)
						{
							m_select = dlgs.m_select;
							m_pattern = dlgs.m_pattern;
						}
						else if (dlgs.m_select == 2)
						{
							m_select = dlgs.m_select;
							m_color = dlgs.m_colorbox.GetColor();
						}
						else if (dlgs.m_select == 3)
						{
							m_select = dlgs.m_select;
						}
					}
					else
					{
						GRegLBDPnt = CPoint(-1, -1);
						GRegMMPnt = CPoint(-1, -1);
						this->Invalidate();
						break;
					}
					REG_NDX_STRU select_reg = GReg;
					CRegionParamerterDlg dlg;  //参数
					dlg.m_Pattern = select_reg.pattern;
					dlg.m_ColorBox.SetColor(select_reg.color);
					if (IDOK == dlg.DoModal())
					{
						select_reg.pattern = dlg.m_Pattern;
						COLORREF tempColor = dlg.m_ColorBox.GetColor();
						memcpy_s(&select_reg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					}
					for (int i = 0; i < GRegNum; i++)
					{
						GRegChanged = true;
						REG_NDX_STRU TmpRegNdx;
						ReadTempFileToRegNdx(GRegTmpNdxF, i, TmpRegNdx);
						if (TmpRegNdx.isDel == 1)
							continue;
						if ((m_select == 1 && TmpRegNdx.pattern == m_pattern) || (m_select == 2 && TmpRegNdx.color == m_color) || m_select == 3)
						{
							D_DOT *dot = new D_DOT[TmpRegNdx.dotNum];
							GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
							GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
							for (int i = 0; i < TmpRegNdx.dotNum; i++)
							{
								PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
							}
							POINT *pnt = new POINT[TmpRegNdx.dotNum];
							DotToPnt(pnt, dot, TmpRegNdx.dotNum);
							DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);  //删除区
							TmpRegNdx.pattern = select_reg.pattern;
							TmpRegNdx.color = select_reg.color;
							DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);  //重绘
							UpdateReg(GRegTmpNdxF, i, TmpRegNdx);
							delete[] pnt;
							delete[] dot;
						}
					}
				}
				else
				{
					REG_NDX_STRU tempRegNdx;
					rect.right = max(point.x, GRegLBDPnt.x);   //确保左上右下
					rect.left = min(point.x, GRegLBDPnt.x);
					rect.bottom = max(point.y, GRegLBDPnt.y);
					rect.top = min(point.y, GRegLBDPnt.y);
					D_DOT rect_d_dot[4];
					rect_d_dot[0].y = rect.top;
					rect_d_dot[0].x = rect.left;
					rect_d_dot[1].y = rect.top;
					rect_d_dot[1].x = rect.right;
					rect_d_dot[2].y = rect.bottom;
					rect_d_dot[2].x = rect.right;
					rect_d_dot[3].y = rect.bottom;
					rect_d_dot[3].x = rect.left;
					for (int i = 0; i < 4; i++)
					{
						PntVPtoDP(rect_d_dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
					}
					REG_NDX_STRU select_reg = GReg;
					CRegionParamerterDlg dlg;  //参数
					dlg.m_Pattern = select_reg.pattern;
					dlg.m_ColorBox.SetColor(select_reg.color);
					if (IDOK == dlg.DoModal())
					{
						select_reg.pattern = dlg.m_Pattern;
						COLORREF tempColor = dlg.m_ColorBox.GetColor();
						memcpy_s(&select_reg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					}
					bool RegInRect = true;
					D_DOT tdot;
					for (int i = 0; i < GRegNum; i++)
					{
						ReadTempFileToRegNdx(GRegTmpNdxF, i, tempRegNdx);
						if (tempRegNdx.isDel == 0)
						{
							RegInRect = true;
							for (int j = 0; j < tempRegNdx.dotNum; j++)
							{
								ReadTempFileToRegDat(GRegTmpDatF, tempRegNdx.datOff, j, tdot);
								if (!PtInPolygon(CPoint(tdot.x, tdot.y), rect_d_dot, 4))
								{
									RegInRect = false;
									break;
								}
							}
							if (RegInRect)
							{
								GRegChanged = true;
								D_DOT *dot = new D_DOT[tempRegNdx.dotNum];
								GRegTmpDatF->Seek(tempRegNdx.datOff, CFile::begin);
								GRegTmpDatF->Read(dot, tempRegNdx.dotNum * sizeof(D_DOT));
								for (int j = 0; j < tempRegNdx.dotNum; j++)
								{
									PntDPtoVP(dot[j], GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
								}
								POINT *pnt = new POINT[tempRegNdx.dotNum];
								DotToPnt(pnt, dot, tempRegNdx.dotNum);
								DrawReg(&dc, tempRegNdx, pnt, tempRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
								tempRegNdx.pattern = select_reg.pattern;
								tempRegNdx.color = select_reg.color;
								DrawReg(&dc, tempRegNdx, pnt, tempRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
								UpdateReg(GRegTmpNdxF, i, tempRegNdx);
								delete[] pnt;
								delete[] dot;
							}
						}
					}
				}
				GRegLBDPnt = CPoint(-1, -1);
				GRegMMPnt = CPoint(-1, -1);
				this->Invalidate();
				break;
		case OPERSTATE_MOVE_REG:
			if (GRegNdx != -1)
			{
				if (GRegLBDPnt.x != -1 && GRegLBDPnt.y != -1)
				{
					D_DOT dot1, dot2;
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntToDot(dot2, GRegLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateReg(GRegTmpNdxF, GRegTmpDatF, GRegNdx, offset_x, offset_y);
					GRegNdx = -1;
					GRegMMOffsetX = 0;
					GRegMMOffsetY = 0;
					GRegChanged = true;
				}
			}
			break;
		case OPERSTATE_UNDELETE_REG:
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
			DotToPnt(point, dot);
			FindDeleteReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);
			if (GRegNdx != -1)
			{
				GRegLNum++;
				GRegChanged = true;
				REG_NDX_STRU TmpRegNdx;
				ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);
				TmpRegNdx.isDel = 0;
				UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);
				D_DOT *dot = new D_DOT[TmpRegNdx.dotNum];
				GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
				GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
				for (int i = 0; i < TmpRegNdx.dotNum; i++)
				{
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);  //坐标系转换
				}
				POINT *pnt = new POINT[TmpRegNdx.dotNum];
				DotToPnt(pnt, dot, TmpRegNdx.dotNum);
				DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
				delete[] pnt;
				delete[] dot;
				GRegNdx = -1;
			}
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		RECT client, rect;
		double zoom = 1.0;
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:
			GetClientRect(&client);
			if (abs(GZoomLBDPnt.x - GZoomMMPnt.x) <= 10 && abs(GZoomLBDPnt.y - GZoomMMPnt.y) <= 10)
			{
				GZoomStytle = 0;
			}
			else
			{
				GZoomStytle = 1;
			}
			if (GZoomStytle == 0)
			{
				double x0 = point.x - (client.right / 2.0) + (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) + (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);
				GZoomOffset_y += (y0 / GZoom);
				GZoom *= 4 / 3.0;
			}
			else
			{
				rect.right = max(point.x, GZoomLBDPnt.x);   //确保左上右下
				rect.left = min(point.x, GZoomLBDPnt.x);
				rect.bottom = max(point.y, GZoomLBDPnt.y);
				rect.top = min(point.y, GZoomLBDPnt.y);
				modulusZoom(client, rect, zoom);  //计算放大倍数
				double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right*(zoom - 1) / (2.0*zoom));
				double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom*(zoom - 1) / (2.0*zoom));
				GZoomOffset_x += (x0 / GZoom);
				GZoomOffset_y += (y0 / GZoom);
				GZoom *= zoom;
				GZoomStytle = 0;
			}
			GZoomLBDPnt = CPoint(-1, -1);
			GZoomMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		case OPERSTATE_ZOOM_OUT:
			if (true)
			{
				GetClientRect(&client);
				double x0 = point.x - (client.right / 2.0) - (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) - (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom);
				GZoomOffset_y += (y0 / GZoom);
				GZoom *= 3 / 4.0;
				this->Invalidate();
			}
			break;
		case OPERSTATE_WINDOW_MOVE:
			GWinMoveLBDPnt = CPoint(-1, -1);
			GWinMoveMMPnt = CPoint(-1, -1);
			break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMapEditorView::OnDestroy()
{
	CView::OnDestroy();
	if (GPntFCreated)  //删除点临时文件
	{
		if (GPntTmpF->m_hFile != CFile::hFileNull)
		{
			GPntTmpF->Close();
			GPntTmpF->Remove(GPntTmpFName);
		}
	}
	delete GPntTmpF;
	if (GLinFCreated)
	{
		if (GLinTmpDatF->m_hFile != CFile::hFileNull)
		{
			GLinTmpDatF->Close();
			GLinTmpDatF->Remove(GLinTmpDatFName);
		}
		if (GLinTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GLinTmpNdxF->Close();
			GLinTmpNdxF->Remove(GLinTmpNdxFName);
		}
	}
	delete GLinTmpDatF;
	delete GLinTmpNdxF;
	if (GRegFCreated)
	{
		if (GRegTmpDatF->m_hFile != CFile::hFileNull)
		{
			GRegTmpDatF->Close();
			GRegTmpDatF->Remove(GRegTmpDatFName);
		}
		if (GRegTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GRegTmpNdxF->Close();
			GRegTmpNdxF->Remove(GRegTmpNdxFName);
		}
	}
	delete GRegTmpDatF;
	delete GRegTmpNdxF;
	if (GBmpFCreated)
	{
		if (GBmpTmpDatF->m_hFile != CFile::hFileNull)
		{
			GBmpTmpDatF->Close();
			GBmpTmpDatF->Remove(GBmpTmpDatFName);
		}
		if (GBmpTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GBmpTmpNdxF->Close();
			GBmpTmpNdxF->Remove(GBmpTmpNdxFName);
		}
	}
	delete GBmpTmpDatF;
	delete GBmpTmpNdxF;
	// TODO: 在此处添加消息处理程序代码
}


void CMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);
	if (GPntFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT:
			if (GPntNdx != -1)
			{
				DrawPnt(&dc, GTPnt);   //异或绘制 删除原有图像
				GTPnt.x = point.x;
				GTPnt.y = point.y;
				DrawPnt(&dc, GTPnt);
			}
			break;
		case OPERSTATE_DELETE_PNT:  //删除点鼠标移动(核心部分)
			if (GPntMMPnt.x != -1 && GPntMMPnt.y != -1)
			{
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen *oldPen = dc.SelectObject(&pen);  //选择点线画笔 并 保存默认画笔
				dc.Rectangle(GPntLBDPnt.x, GPntLBDPnt.y, GPntMMPnt.x, GPntMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GPntLBDPnt.x, GPntLBDPnt.y, point.x, point.y);  //绘制新矩形
				GPntMMPnt = point;  //记录鼠标位置
				dc.SelectObject(oldPen);  //恢复默认画笔
			}
			break;
		case OPERSTATE_MODIFY_POINT_PARAMETER:
			if (GPntMMPnt.x != -1 && GPntMMPnt.y != -1)
			{
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen *oldPen = dc.SelectObject(&pen);
				dc.Rectangle(GPntLBDPnt.x, GPntLBDPnt.y, GPntMMPnt.x, GPntMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GPntLBDPnt.x, GPntLBDPnt.y, point.x, point.y);
				GPntMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		}
	}
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN:
			if (GTLin.dotNum > 0)
			{
				if (GMPnt.x != -1 && GMPnt.y != -1)
				{
					DrawSeg(&dc, GTLin, GLPnt, GMPnt);  //删除动态绘制线
				}
				GMPnt.x = point.x;  //记录位置 用于异或绘制删除上次动态线
				GMPnt.y = point.y;
				POINT mpoint = { mpoint.x = point.x,mpoint.y = point.y };
				DrawSeg(&dc, GTLin, GLPnt, mpoint); //绘制本次线
			}
			break;
		case OPERSTATE_MOVE_LIN:
			if (GLinNdx != -1)
			{
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)//删除上次绘制图像
				{
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				GLinMMOffsetX += (point.x - GLinMMPnt.x);  //更新偏移
				GLinMMOffsetY += (point.y - GLinMMPnt.y);
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)//绘制图像
				{
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);     //坐标转换
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				GLinMMPnt = point;
			}
			break;
		case OPERSTATE_LIN_ADD_PNT:
			if (GLinDotNdx1 != -1)
			{
				D_DOT dot, dot1, dot2;
				POINT pnt1, pnt2;
				PntToDot(dot, point);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(point, dot);
				ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, GLinDotNdx1, dot1);
				ReadTempFileToLinDat(GLinTmpDatF, tempLinNdx.datOff, GLinDotNdx2, dot2);
				PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
				PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pnt1, dot1);
				DotToPnt(pnt2, dot2);
				DrawSeg(&dc, tempLinNdx, GLinMMPnt, pnt1);  //删除
				DrawSeg(&dc, tempLinNdx, GLinMMPnt, pnt2);  //删除
				DrawSeg(&dc, tempLinNdx, point, pnt1);
				DrawSeg(&dc, tempLinNdx, point, pnt2);
				GLinMMPnt = CPoint(point.x, point.y);
			}
			break;
		case OPERSTATE_DELETE_LIN:
			if (GLinMMPnt.x != -1 && GLinMMPnt.y != -1)
			{
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen *oldPen = dc.SelectObject(&pen);
				dc.Rectangle(GLinLBDPnt.x, GLinLBDPnt.y, GLinMMPnt.x, GLinMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GLinLBDPnt.x, GLinLBDPnt.y, point.x, point.y);
				GLinMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		case OPERSTATE_MODIFY_lin_PARAMETER:
			if (GLinMMPnt.x != -1 && GLinMMPnt.y != -1)
			{
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen *oldPen = dc.SelectObject(&pen);
				dc.Rectangle(GLinLBDPnt.x, GLinLBDPnt.y, GLinMMPnt.x, GLinMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GLinLBDPnt.x, GLinLBDPnt.y, point.x, point.y);
				GLinMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		//造区鼠标移动事件(核心部分)
		case OPERSTATE_INPUT_REG:
			if (GRegCreateMMPnt.x != -1 && GRegCreateMMPnt.y != -1)  //判断是否开始i创建
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);  //异或绘制(重复绘制及删除)
				LIN_NDX_STRU tln =
				{
					tln.pattern = GTReg.pattern,
					tln.color = GTReg.color 
				};//设置区域参数
				if (GTReg.dotNum == 1)  //只有一个点的时候相当于一条线
				{
					DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);  //删除上次绘制
					DrawSeg(&dc, tln, GRegCreateStartPnt, point);  //绘制新的线
				}
				else
				{
					D_DOT *dot = new D_DOT[GTReg.dotNum];   //存储节点的数据
					for (int i = 0; i < GTReg.dotNum; i++)
					{
						ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, i, dot[i]);   //从文件中读取节点数据
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);   //数据坐标到窗口坐标
					}
					POINT *pnt = new POINT[GTReg.dotNum + 1];  //多申请一个POINT存储鼠标点
					DotToPnt(pnt, dot, GTReg.dotNum);   //批量D_DOT转POINT
					pnt[GTReg.dotNum] = GRegCreateMMPnt;  //存储上一次鼠标点
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);  //删除上次绘制区域
					pnt[GTReg.dotNum] = point;  //存储当前鼠标点
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);  //绘制新图形
					delete[] dot;
					delete[] pnt;
				}
				GRegCreateMMPnt = point;  //记录本次鼠标点
			}
			break;
		case OPERSTATE_MOVE_REG:
			if (GRegNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN);
				D_DOT *dot = new D_DOT[GRegMMTmpNdx.dotNum];
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)//擦出上次绘制
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				POINT *pnt = new POINT[GRegMMTmpNdx.dotNum];
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
				GRegMMOffsetX = GRegMMOffsetX + point.x - GRegMMPnt.x;//计算偏移量
				GRegMMOffsetY = GRegMMOffsetY + point.y - GRegMMPnt.y;
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)//绘制新上次绘制
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum, GBmpNum, GBmpTmpNdxF, GBmpTmpDatF);
				delete[] dot;
				delete[] pnt;
				GRegMMPnt = point;
			}
			break;
		case OPERSTATE_DELETE_REG:
			if (GRegMMPnt.x != -1 && GRegMMPnt.y != -1)
			{
				CPen pen(PS_DOT, 1, RGB(0, 0, 0));
				CPen *oldPen = dc.SelectObject(&pen);
				dc.Rectangle(GRegLBDPnt.x, GRegLBDPnt.y, GRegMMPnt.x, GRegMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GRegLBDPnt.x, GRegLBDPnt.y, point.x, point.y);
				GRegMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		case OPERSTATE_MODIFY_REGION_PARAMETER:
				if (GRegMMPnt.x != -1 && GRegMMPnt.y != -1)
				{
					CPen pen(PS_DOT, 1, RGB(0, 0, 0));
					CPen *oldPen = dc.SelectObject(&pen);
					dc.Rectangle(GRegLBDPnt.x, GRegLBDPnt.y, GRegMMPnt.x, GRegMMPnt.y);  //删除上次绘制边框
					dc.Rectangle(GRegLBDPnt.x, GRegLBDPnt.y, point.x, point.y);
					GRegMMPnt = point;
					dc.SelectObject(oldPen);
				}
				break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		CClientDC dc(this);
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen *oldPen = dc.SelectObject(&pen);
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:
			if (GZoomMMPnt.x != -1 && GZoomMMPnt.y != -1)
			{
				dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, GZoomMMPnt.x, GZoomMMPnt.y);  //删除上次绘制边框
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, point.x, point.y);
				GZoomMMPnt = point;
				dc.SelectObject(oldPen);
			}
			break;
		case OPERSTATE_WINDOW_MOVE:
			if (GWinMoveMMPnt.x != -1 && GWinMoveMMPnt.y != -1)
			{
				CPoint offset(0, 0);
				offset.x = point.x - GWinMoveLBDPnt.x;
				offset.y = point.y - GWinMoveLBDPnt.y;
				GZoomOffset_x -= offset.x / GZoom;
				GZoomOffset_y -= offset.y / GZoom;
				GWinMoveLBDPnt = point;
				this->Invalidate();
			}
			break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CMapEditorView::OnUpdateWindowShowPoint(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_UNDEL && GShowPnt == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdateWindowShowLine(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_UNDEL && GShowLin == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdateWindowShowRegion(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_UNDEL && GShowReg == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdatePointShowDeleted(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_DEL && GShowPnt == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdateLineShowDeleted(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_DEL && GShowLin == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnUpdateRegionShowDeleted(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (GCurShowState == SHOWSTATE_DEL && GShowReg == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CMapEditorView::OnRegionSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
	CRegionParamerterDlg dlg;
	dlg.m_Pattern = GReg.pattern;
	dlg.m_ColorBox.SetColor(GReg.color);
	if (IDOK == dlg.DoModal())
	{
		GReg.pattern = dlg.m_Pattern;
		GReg.bmp = dlg.m_Bmp;
		COLORREF tempColor = dlg.m_ColorBox.GetColor();
		memcpy_s(&GReg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}

int CMapEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  在此添加您专用的创建代码
	/*if (!BmpNdxF.Open(TEXT("bmp\\bmpNdxF.ndx"), CFile::modeRead))
	{
		CreateDirectory(TEXT("bmp"),0);
		BmpNdxF.Open(TEXT("bmp\\bmpNdxF.ndx"),CFile::modeCreate|CFile::modeRead);
		BmpMaxNum = 0;
		BmpNum = 0;
	}
	else
	{
		BMP_NDX_STRU tempNdx;
		BmpNdxF.Seek(0, CFile::begin);
		BmpNdxF.Read(&BmpNum, sizeof(long));
		BmpNdxF.Read(&BmpMaxNum,sizeof(long));
		for (int i = 0; i < BmpNum; i++)
		{
			BmpNdxF.Read(&tempNdx, sizeof(BMP_NDX_STRU));
			bmpNdxV.push_back(tempNdx);
		}
		for (int i = 0; i < BmpNum; i++)
		{
			BmpNdxF.Seek(bmpNdxV[i].datOff,CFile::begin);
			WCHAR *tempstr = new WCHAR[bmpNdxV[i].length];
			BmpNdxF.Read(tempstr, bmpNdxV[i].length * sizeof(WCHAR));
			bmpAddrV.push_back(CString(tempstr));
		}
	}*/
	return 0;
}


void CMapEditorView::OnImageContral()  //选择管理位图窗口
{
	CImageDlg dlg;
	if (GBmpFCreated)
	{
		dlg.DoModal();  //打开位图管理界面
	}
	else
	{
		MessageBox(TEXT("位图文件未打开"), TEXT("提示"), MB_OK);
	}
}

void CMapEditorView::OnFileOpenBmp()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = TEXT("bitmap\0*.bitmap");
	if (GBmpChanged == true)
	{
		if (IDYES == AfxMessageBox(TEXT("文件还未保存。是否要保存？"), MB_YESNO | MB_ICONQUESTION))
		{
			OnFileSaveBmp();
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GBmpFCreated = false;
		int IsCreate = 0;
		GBmpFName = dlg.m_ofn.lpstrFile;  //永久文件名称
		CFile *BmpF = new CFile();
		if (!BmpF->Open(GBmpFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(TEXT("无法打开文件\n"));
			return;
		}
		int nPos = GBmpFName.ReverseFind(TEXT('\\'));
		CString floderAdd = GBmpFName.Left(nPos);
		if (GBmpTmpDatFName != "tempBmpF.dat")
		{
			GBmpTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GBmpTmpDatFName = floderAdd + CString("\\") + GBmpTmpDatFName;
			if (!GBmpTmpDatF->Open(GBmpTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GBmpTmpDatFName = CString("tempBmpF.dat");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (GBmpTmpNdxFName != "tempBmpF.ndx")
		{
			GBmpTmpNdxF->SeekToBegin();
			++IsCreate;
		}
		else
		{
			GBmpTmpNdxFName = floderAdd + CString("\\") + GBmpTmpNdxFName;
			if (!GBmpTmpNdxF->Open(GBmpTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GBmpTmpNdxFName = CString("tempBmpF.ndx");
				TRACE(TEXT("无法打开文件\n"));
			}
			else
				++IsCreate;
		}
		if (2 == IsCreate)
			GBmpFCreated = true;
		ReadBmpPermanentFileToTemp(BmpF, GBmpTmpDatF, GBmpTmpNdxF, GBmpVer, GBmpNum, GBmpLNum,GBmp.datOff);
		BmpF->Close();
		delete BmpF;
		CString windowText = dlg.GetFileName() + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileSaveBmp()
{
	// TODO: 在此添加命令处理程序代码
	if (GBmpFCreated)
	{
		CFile *BmpF = new CFile();
		if (GBmpFName.IsEmpty())
		{
			LPCTSTR lpszFilters;
			lpszFilters = TEXT("区(*.bitmap)|*.bitmap||");
			CFileDialog dlg(false, TEXT("bitmap"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters);
			int nPos = GBmpTmpDatFName.ReverseFind(TEXT('\\'));
			CString folderAdd = GBmpTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GBmpFName = dlg.GetPathName();
			}
			else
			{
				return;
			}
		}
		else
		{
			BmpF->Remove(GBmpFName);
		}
		if (!BmpF->Open(GBmpFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(TEXT("无法打开文件"));
			return;
		}
		WriteTempToBmpPermanentFile(BmpF, GBmpTmpDatF, GBmpTmpNdxF, GBmpVer, GBmpNum, GBmpLNum);
		BmpF->Close();
		delete BmpF;
		GBmpChanged = false;  //保存文件后变更状态改变
		int nPos = GBmpFName.ReverseFind(TEXT('\\'));
		CString windowText = GBmpFName.Right(GBmpFName.GetLength() - nPos - 1) + " - MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(TEXT("区临时文件还未创建。"), TEXT("提示"), MB_OK);
	}
}


void CMapEditorView::OnFileSaveAsBmp()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GBmpFName;  //存储文件名
	GBmpFName = CString("");
	OnFileSaveBmp();
	if (GBmpFName == "")
	{
		GBmpFName = tempFName;
	}
}
