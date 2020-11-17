// CCreateFileDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapEditor.h"
#include "CCreateFileDlg.h"
#include "afxdialogex.h"


// CCreateFileDlg 对话框

IMPLEMENT_DYNAMIC(CCreateFileDlg, CDialogEx)

CCreateFileDlg::CCreateFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_FILE, pParent)
	, m_add(_T(""))
{
	//初始化路径
	HMODULE module = GetModuleHandle(0);
	char* pFileName = new char[MAX_PATH];
	GetModuleFileName(module,LPWSTR(pFileName),MAX_PATH);
	m_add.Format(TEXT("%s"),pFileName);
	int nPos = m_add.ReverseFind(TEXT('\\'));
	if (nPos < 0)
	{
		m_add = "";
	}
	else
	{
		m_add = m_add.Left(nPos); //去除右侧文件名
	}
	delete[] pFileName;
}

CCreateFileDlg::~CCreateFileDlg()
{
}

void CCreateFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CREATE_FILE_ADDRESS, m_add);
}


BEGIN_MESSAGE_MAP(CCreateFileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CREATE_FILE_CHANGE_ADDRESS_BTN, &CCreateFileDlg::OnBnClickedCreateFileChangeAddressBtn)
//	ON_BN_CLICKED(IDOK, &CCreateFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateFileDlg 消息处理程序


void CCreateFileDlg::OnBnClickedCreateFileChangeAddressBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.lpszTitle = TEXT("请选择零临时件存放路径：");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;  //非系统路径无法确定
	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);  //显示选择路径窗口
	if (lpDlist != NULL)
	{
		TCHAR chPath[MAX_PATH];
		SHGetPathFromIDList(lpDlist, chPath);  //获取地址
		m_add = chPath;
		UpdateData(FALSE); //false 变量-》控件     true 控件-》变量
	}
}