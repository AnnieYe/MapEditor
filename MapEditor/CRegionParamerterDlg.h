#pragma once


// CRegionParamerterDlg 对话框

class CRegionParamerterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegionParamerterDlg)

public:
	CRegionParamerterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegionParamerterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGION_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	CMFCColorButton m_ColorBox;
	int m_Pattern;
	int m_Bmp;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombobox();
	CComboBox m_ComBox_Bmp;
	afx_msg void OnCbnSelchangeComboxBmp();
};
