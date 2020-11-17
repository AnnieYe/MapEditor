// CPointParamerterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CPointParamerterDlg.h"
#include "afxdialogex.h"


// CPointParamerterDlg 对话框

IMPLEMENT_DYNAMIC(CPointParamerterDlg, CDialogEx)

CPointParamerterDlg::CPointParamerterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POINT_PARAMETER, pParent)
{
	m_Pattern = 0;
}

CPointParamerterDlg::~CPointParamerterDlg()
{
}

void CPointParamerterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_POINT_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CPointParamerterDlg, CDialogEx)
//	ON_CBN_SELCHANGE(IDC_POINT_PATTERN, &CPointParamerterDlg::OnSelchangePointPattern)
ON_CBN_SELCHANGE(IDC_POINT_PATTERN, &CPointParamerterDlg::OnCbnSelchangePointPattern)
END_MESSAGE_MAP()


// CPointParamerterDlg 消息处理程序


BOOL CPointParamerterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CPointParamerterDlg::OnCbnSelchangePointPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Pattern = m_ComboBox.GetCurSel();
}
