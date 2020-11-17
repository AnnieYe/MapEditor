// CSelectPoint.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CSelectPoint.h"
#include "afxdialogex.h"


// CSelectPoint 对话框

IMPLEMENT_DYNAMIC(CSelectPoint, CDialogEx)

CSelectPoint::CSelectPoint(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POINT_SELECT, pParent)
{
	m_select = 0;
	m_pattern = 0;
}

CSelectPoint::~CSelectPoint()
{
}

void CSelectPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOX, m_comobbox);
	DDX_Control(pDX, IDC_COLOR_BOX, m_colorbox);
}


BEGIN_MESSAGE_MAP(CSelectPoint, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_SELECT_KING, &CSelectPoint::OnBnClickedRadioSelectKing)
	ON_BN_CLICKED(IDC_RADIO_SELECT_COLOR, &CSelectPoint::OnBnClickedRadioSelectColor)
	ON_BN_CLICKED(IDC_RADIO_ALL, &CSelectPoint::OnBnClickedRadioAll)
	ON_CBN_SELCHANGE(IDC_COMBO_BOX, &CSelectPoint::OnCbnSelchangeComboBox)
END_MESSAGE_MAP()


// CSelectPoint 消息处理程序


void CSelectPoint::OnBnClickedRadioSelectKing()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 1;
}


void CSelectPoint::OnBnClickedRadioSelectColor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 2;
}


void CSelectPoint::OnBnClickedRadioAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 3;
}


void CSelectPoint::OnCbnSelchangeComboBox()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pattern = m_comobbox.GetCurSel();
}


BOOL CSelectPoint::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comobbox.SetCurSel(m_pattern);
	m_colorbox.SetColor(RGB(0, 0, 0));
	
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
