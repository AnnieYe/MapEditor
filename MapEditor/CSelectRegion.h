#pragma once


// CSelectRegion 对话框

class CSelectRegion : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectRegion)

public:
	CSelectRegion(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectRegion();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGION_SELECT };
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
	CComboBox m_comobox;
	CMFCColorButton m_colorbox;
	afx_msg void OnCbnSelchangeComboBox();
	virtual BOOL OnInitDialog();
};
