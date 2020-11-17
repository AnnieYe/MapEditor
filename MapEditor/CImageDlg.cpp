// CImageDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CImageDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>
#include "MapEditor_Dll.h"

extern bool      GBmpFCreated;                                  //临时文件是否创建
extern CString   GBmpFName;                                     //永久文件名（含路径）
extern CString   GBmpTmpNdxFName;                               //临时索引文件名（含路径）
extern CString   GBmpTmpDatFName;                               //临时数据文件名（含路径）
extern bool      GBmpChanged;                                   //是否更改
extern int       GBmpNum;                                       //物理数
extern int       GBmpLNum;					     				//逻辑数
extern CFile     *GBmpTmpDatF;                                  //读取临时数据文件的指针对象
extern CFile     *GBmpTmpNdxF;

// CImageDlg 对话框

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGE, pParent)
{
}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_LIST, m_image_list);
	DDX_Control(pDX, IDC_PICTURE, m_picture_control);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_BUTTON, &CImageDlg::OnBnClickedAddButton)
	ON_LBN_SELCHANGE(IDC_IMAGE_LIST, &CImageDlg::OnLbnSelchangeImageList)
	ON_BN_CLICKED(IDC_DELETE_IMAGE, &CImageDlg::OnBnClickedDeleteImage)
END_MESSAGE_MAP()


// CImageDlg 消息处理程序


void CImageDlg::OnBnClickedAddButton()  //添加图片按钮
{
	CFileDialog dlg(true);     //文件选择窗口
	dlg.m_ofn.lpstrFilter = TEXT("bmp\0*.bmp");  //设置选择文件类型
	if (dlg.DoModal() == IDOK)
	{
		CString BmpAdress = dlg.m_ofn.lpstrFile;  //获取bmp路径
		CFile bmpFile(BmpAdress, CFile::modeRead);  //打开文件
		BMP_NDX_STRU TempBmpNdx;  //位图文件索引
		TempBmpNdx.num = GBmpNum;  //位图编号
		TempBmpNdx.isDel = 0;
		TempBmpNdx.length = bmpFile.GetLength();  //记录文件长度
		TempBmpNdx.datOff = GBmpTmpDatF->GetLength();  //记录偏移
		WriteBmpNdxToFile(GBmpTmpNdxF, GBmpNum, TempBmpNdx);  //写入位图索引
		WriteBmpDatToFile(GBmpTmpDatF, &bmpFile);//写入文图数据
		for (int i = 0; i < GBmpLNum; i++)
		{
			m_image_list.DeleteString(0);  //删除列表框
		}
		GBmpNum++;
		GBmpLNum++;
		BMP_NDX_STRU tempNdx;  //添加列表框
		for (int i = 0; i < GBmpNum; i++)
		{
			ReadTempFileToBmpNdx(GBmpTmpNdxF, i, tempNdx);
			if (!tempNdx.isDel)
			{
				CString str;
				str.Format(TEXT("%d"), tempNdx.num);
				m_image_list.AddString(str);
			}
		}
		m_image_list.UpdateData(1);  //显示列表框
	}
	GBmpChanged = true;
}


BOOL CImageDlg::OnInitDialog()  //载入管理位图窗口
{
	CDialogEx::OnInitDialog();
	BMP_NDX_STRU tempNdx;//初始化列表框
	for (int i = 0; i < GBmpNum; i++)
	{
		ReadTempFileToBmpNdx(GBmpTmpNdxF, i, tempNdx);
		if (!tempNdx.isDel)
		{
			CString str;
			str.Format(TEXT("%d"), tempNdx.num);
			m_image_list.AddString(str);
		}
	}
	return TRUE;
}


void CImageDlg::OnLbnSelchangeImageList()  //选择列表框-》更新预览框显示
{
	CString str;
	m_image_list.GetText(m_image_list.GetCurSel(), str); //获取选择
	BMP_NDX_STRU tempBmpNdx;
	ReadTempFileToBmpNdx(GBmpTmpNdxF,_ttoi(str) , tempBmpNdx);  //读取索引
	CFile tempBmpFile;
	tempBmpFile.Open(TEXT("ShowTempBmpDat.bmp"), CFile::modeReadWrite|CFile::modeCreate); //图片临时文件
	ReadTempFileToBmpDat(GBmpTmpDatF, tempBmpNdx.datOff, tempBmpNdx.length, tempBmpFile);  //转存文件
	BITMAPINFO *pBitmapInfo;   //位图文件信息
	BITMAPFILEHEADER bmpHeader;  //位图文件头
	BITMAPINFOHEADER bmpInfo;   //位图信息头
	tempBmpFile.SeekToBegin();
	tempBmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER));
	tempBmpFile.Read(&bmpInfo,sizeof(BITMAPINFOHEADER));
	BYTE *pData = new BYTE[bmpHeader.bfSize - bmpHeader.bfOffBits];  //图片信息数据
	tempBmpFile.Read(pData, bmpHeader.bfSize - bmpHeader.bfOffBits);
	RECT rect;
	m_picture_control.GetClientRect(&rect);
	CDC *dc = m_picture_control.GetDC();
	dc->SetStretchBltMode(COLORONCOLOR);   //以设置指定DC中的位图拉伸模式,COLORONCOLOR删除所有消除的像素行
	pBitmapInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)];
	memcpy(pBitmapInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));  //复制信息
	//绘制预览图像
	StretchDIBits(dc->GetSafeHdc(), 0, 0, rect.right, rect.bottom, 0, 0, bmpInfo.biWidth, bmpInfo.biHeight, pData, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	tempBmpFile.Close();  //关闭文件
	CFile::Remove(TEXT("ShowTempBmpDat.bmp"));  //删除临时文件
}


void CImageDlg::OnBnClickedDeleteImage()  //删除图片按钮
{
	CString str;
	m_image_list.GetText(m_image_list.GetCurSel(), str); //获取选择的文本
	BMP_NDX_STRU tempBmpNdx;
	ReadTempFileToBmpNdx(GBmpTmpNdxF, _ttoi(str), tempBmpNdx);//读入索引
	tempBmpNdx.isDel = 1;  //标记删除
	WriteBmpNdxToFile(GBmpTmpNdxF, _ttoi(str), tempBmpNdx); //跟新索引
	m_image_list.DeleteString(m_image_list.GetCurSel());  //删除字符串
	GBmpChanged = true;
}
