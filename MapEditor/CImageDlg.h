#pragma once


// CImageDlg 对话框

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImageDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddButton();
	CListBox m_image_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeImageList();
	int m_Bmp;
	CStatic m_picture_control;
	afx_msg void OnBnClickedDeleteImage();
};
