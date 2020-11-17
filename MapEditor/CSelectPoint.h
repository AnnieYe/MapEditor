#pragma once


// CSelectPoint 对话框

class CSelectPoint : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectPoint)

public:
	CSelectPoint(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectPoint();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POINT_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_select;
	CComboBox m_comobbox;
	CMFCColorButton m_colorbox;
	afx_msg void OnBnClickedRadioSelectKing();
	afx_msg void OnBnClickedRadioSelectColor();
	afx_msg void OnBnClickedRadioAll();
	afx_msg void OnCbnSelchangeComboBox();
	int m_pattern;
	virtual BOOL OnInitDialog();
};
