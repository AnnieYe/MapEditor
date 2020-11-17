#pragma once


// CSelectLine 对话框

class CSelectLine : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectLine)

public:
	CSelectLine(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectLine();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_select;
	int m_pattern;
	afx_msg void OnBnClickedSelectKind();
	afx_msg void OnBnClickedSelectColor();
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnCbnSelchangeComboBox();
	CComboBox m_comobox;
	CMFCColorButton m_colorbox;
	virtual BOOL OnInitDialog();
};
