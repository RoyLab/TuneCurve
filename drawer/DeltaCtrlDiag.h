#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// DeltaCtrlDiag dialog
class CdrawerView;

class DeltaCtrlDiag : public CDialogEx
{
	DECLARE_DYNAMIC(DeltaCtrlDiag)

public:
	DeltaCtrlDiag(CWnd* pParent = NULL);   // standard constructor
	virtual ~DeltaCtrlDiag();

// Dialog Data
	enum { IDD = IDD_DELTA_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl mSlider;
	CStatic mDeltaVal;

	CdrawerView *mParent;

	CSliderCtrl mThetSlider;
	CStatic mThetVal;

	CSliderCtrl mMultiSlider;
	CStatic mMultiVal;

	CSliderCtrl mClipSlider;
	CStatic mClipValue;

	afx_msg void OnNcDestroy();

	CSliderCtrl mNoiseSlider;
	CStatic mNoiseVal;

	CSliderCtrl mTransSlider;
	CStatic mTransVal;
};
