// CLineParamerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CLineParamerDlg.h"
#include "afxdialogex.h"


// CLineParamerDlg 对话框

IMPLEMENT_DYNAMIC(CLineParamerDlg, CDialogEx)

CLineParamerDlg::CLineParamerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINE_PARAMETER, pParent)
{
	m_Patttern = 0;
}

CLineParamerDlg::~CLineParamerDlg()
{
}

void CLineParamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINE_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_LINE_COLOR, m_ColorBox);
}


BEGIN_MESSAGE_MAP(CLineParamerDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_LINE_PATTERN, &CLineParamerDlg::OnSelchangeLinePattern)
END_MESSAGE_MAP()


// CLineParamerDlg 消息处理程序


BOOL CLineParamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Patttern);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLineParamerDlg::OnSelchangeLinePattern()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Patttern = m_ComboBox.GetCurSel();
}
