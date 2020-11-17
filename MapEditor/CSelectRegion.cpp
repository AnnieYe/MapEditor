// CSelectRegion.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CSelectRegion.h"
#include "afxdialogex.h"


// CSelectRegion 对话框

IMPLEMENT_DYNAMIC(CSelectRegion, CDialogEx)

CSelectRegion::CSelectRegion(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGION_SELECT, pParent)
{

	m_select = 0;
	m_pattern = 0;
}

CSelectRegion::~CSelectRegion()
{
}

void CSelectRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOX, m_comobox);
	DDX_Control(pDX, IDC_COLOR_BOX, m_colorbox);
}


BEGIN_MESSAGE_MAP(CSelectRegion, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_KIND, &CSelectRegion::OnBnClickedSelectKind)
	ON_BN_CLICKED(IDC_SELECT_COLOR, &CSelectRegion::OnBnClickedSelectColor)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CSelectRegion::OnBnClickedSelectAll)
	ON_CBN_SELCHANGE(IDC_COMBO_BOX, &CSelectRegion::OnCbnSelchangeComboBox)
END_MESSAGE_MAP()


// CSelectRegion 消息处理程序


void CSelectRegion::OnBnClickedSelectKind()
{
	// TODO: 在此添加控件通知处理程序代码
	m_select = 1;
}


void CSelectRegion::OnBnClickedSelectColor()
{
	// TODO: 在此添加控件通知处理程序代码

	m_select = 2;
}


void CSelectRegion::OnBnClickedSelectAll()
{
	// TODO: 在此添加控件通知处理程序代码

	m_select = 3;
}


void CSelectRegion::OnCbnSelchangeComboBox()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pattern = m_comobox.GetCurSel();
}


BOOL CSelectRegion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comobox.SetCurSel(m_pattern);
	m_colorbox.SetColor(RGB(0, 0, 0));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
