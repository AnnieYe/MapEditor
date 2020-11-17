// CRegionParamerterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CRegionParamerterDlg.h"
#include "afxdialogex.h"
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


// CRegionParamerterDlg 对话框

IMPLEMENT_DYNAMIC(CRegionParamerterDlg, CDialogEx)

CRegionParamerterDlg::CRegionParamerterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGION_PARAMETER, pParent)
{
	m_Pattern = 0;
}

CRegionParamerterDlg::~CRegionParamerterDlg()
{
}

void CRegionParamerterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ComboBox, m_ComboBox);
	DDX_Control(pDX, IDC_ColorBox, m_ColorBox);
	DDX_Control(pDX, IDC_COMBOX_BMP, m_ComBox_Bmp);
}


BEGIN_MESSAGE_MAP(CRegionParamerterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ComboBox, &CRegionParamerterDlg::OnSelchangeCombobox)
	ON_CBN_SELCHANGE(IDC_COMBOX_BMP, &CRegionParamerterDlg::OnCbnSelchangeComboxBmp)
END_MESSAGE_MAP()


// CRegionParamerterDlg 消息处理程序


BOOL CRegionParamerterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern);
	BMP_NDX_STRU tempNdx;
	for (int i = 0; i < GBmpNum; i++)
	{
		ReadTempFileToBmpNdx(GBmpTmpNdxF, i, tempNdx);
		if (!tempNdx.isDel)
		{
			CString str;
			str.Format(TEXT("%d"), tempNdx.num);
			m_ComBox_Bmp.AddString(str);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegionParamerterDlg::OnSelchangeCombobox()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Pattern = m_ComboBox.GetCurSel();
}


void CRegionParamerterDlg::OnCbnSelchangeComboxBmp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_ComBox_Bmp.GetLBText(m_ComBox_Bmp.GetCurSel(), str);
	m_Bmp = _ttoi(str);
}
