// CSelectLine.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CSelectLine.h"
#include "afxdialogex.h"


// CSelectLine 对话框

IMPLEMENT_DYNAMIC(CSelectLine, CDialogEx)

CSelectLine::CSelectLine(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINE_SELECT, pParent)
{
	m_pattern = 0;
	m_select = 0;
}

CSelectLine::~CSelectLine()
{
}

void CSelectLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOX, m_comobox);
	DDX_Control(pDX, IDC_COLOR_BOX, m_colorbox);
}


BEGIN_MESSAGE_MAP(CSelectLine, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_KIND, &CSelectLine::OnBnClickedSelectKind)
	ON_BN_CLICKED(IDC_SELECT_COLOR, &CSelectLine::OnBnClickedSelectColor)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CSelectLine::OnBnClickedSelectAll)
	ON_CBN_SELCHANGE(IDC_COMBO_BOX, &CSelectLine::OnCbnSelchangeComboBox)
END_MESSAGE_MAP()


// CSelectLine 消息处理程序


void CSelectLine::OnBnClickedSelectKind()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 1;
}


void CSelectLine::OnBnClickedSelectColor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 2;
}


void CSelectLine::OnBnClickedSelectAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 3;
}


void CSelectLine::OnCbnSelchangeComboBox()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pattern = m_comobox.GetCurSel();
}


BOOL CSelectLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comobox.SetCurSel(m_pattern);
	m_colorbox.SetColor(RGB(0, 0, 0));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
