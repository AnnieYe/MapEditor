#pragma once


// CPointParamerterDlg 对话框

class CPointParamerterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointParamerterDlg)

public:
	CPointParamerterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPointParamerterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POINT_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	int m_Pattern;
	virtual BOOL OnInitDialog();
	CMFCColorButton m_ColorButton;
	afx_msg void OnCbnSelchangePointPattern();
};
