// DeltaCtrlDiag.cpp : implementation file
//

#include "stdafx.h"
#include "drawer.h"
#include "DeltaCtrlDiag.h"
#include "afxdialogex.h"
#include "drawerView.h"
#include "D3DEngine.h"


void DeltaCtrlDiag::SetLineWidth(float width)
{
	mSlider.SetPos(int(width*2));
	CString tmp;
	tmp.Format(_T("%0.2f"), width);
	mDeltaVal.SetWindowTextW(tmp);
}
void DeltaCtrlDiag::SetXAxis(float height)
{
	mClipSlider.SetPos(int(height/2));
	CString tmp;
	tmp.Format(_T("%0.2f"), height);
	mClipValue.SetWindowTextW(tmp);
}
void DeltaCtrlDiag::SetYAxis(float height)
{
	mThetSlider.SetPos(int(height/2));
	CString tmp;
	tmp.Format(_T("%0.2f"), height);
	mThetVal.SetWindowTextW(tmp);
}

// DeltaCtrlDiag dialog

IMPLEMENT_DYNAMIC(DeltaCtrlDiag, CDialogEx)

DeltaCtrlDiag::DeltaCtrlDiag(CWnd* pParent /*=NULL*/)
	: CDialogEx(DeltaCtrlDiag::IDD, pParent)
{
	mParent = (CdrawerView*)pParent;
}

DeltaCtrlDiag::~DeltaCtrlDiag()
{
}

void DeltaCtrlDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Slider(pDX, IDC_DELTA_SLIDER, mSliderVal);
	//  DDV_MinMaxInt(pDX, mSliderVal, 0, 100);
	//  DDX_Text(pDX, IDC_DELTA_VALUE, mDeltaVal);
	DDX_Control(pDX, IDC_DELTA_SLIDER, mSlider);
	DDX_Control(pDX, IDC_DELTA_VALUE, mDeltaVal);
	DDX_Control(pDX, IDC_THET_SLIDER, mThetSlider);
	DDX_Control(pDX, IDC_THET_VALUE, mThetVal);
	DDX_Control(pDX, IDC_MS_SLIDER, mMultiSlider);
	//  DDX_Control(pDX, IDC_MS_VALUE, mMultiVlu);
	DDX_Control(pDX, IDC_MS_VALUE, mMultiVal);
	DDX_Control(pDX, IDC_CLIP_SLIDER, mClipSlider);
	DDX_Control(pDX, IDC_CLIP_VALUE, mClipValue);
	DDX_Control(pDX, IDC_NOISE_SLIDER, mNoiseSlider);
	DDX_Control(pDX, IDC_NOISE_VALUE, mNoiseVal);
}


BEGIN_MESSAGE_MAP(DeltaCtrlDiag, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// DeltaCtrlDiag message handlers


void DeltaCtrlDiag::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	switch (pScrollBar->GetDlgCtrlID())
	{
	case IDC_DELTA_SLIDER:
	{
		//int val = mSlider.GetPos();
		//CString tmp;
		//tmp.Format(_T("%d"), val);
		//mDeltaVal.SetWindowTextW(tmp);
		//mParent->GetD3DEngine()->SetSigmaValue(val);
		float val = exp(mSlider.GetPos()/25.0)*20.0/exp(4.0);
		CString tmp;
		tmp.Format(_T("%0.2f"), val);
		mDeltaVal.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetLineWidth(val);
		break;
	}
	case IDC_THET_SLIDER:
	{
		//int val = mThetSlider.GetPos();
		//CString tmp;
		//tmp.Format(_T("%d"), val);
		//mThetVal.SetWindowTextW(tmp);
		//mParent->GetD3DEngine()->SetThetValue(val);
		float val = mThetSlider.GetPos()*1.0f;
		CString tmp;
		tmp.Format(_T("%0.2f"), val);
		mThetVal.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetYAxis(val);
		break;
	}
	case IDC_MS_SLIDER:
	{
		int val = mMultiSlider.GetPos()/21+1;
		CString tmp;
		tmp.Format(_T("%d"), val);
		mMultiVal.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetMSAAlvl(val);
		break;
	}
	case IDC_CLIP_SLIDER:
	{
		//int val = mClipSlider.GetPos();
		//mParent->GetD3DEngine()->SetClipEdgeValue(val);
		//float val2 = val/20.0f+1.0f;
		//CString tmp;
		//tmp.Format(_T("%0.2f"), val2);
		//mClipValue.SetWindowTextW(tmp);
		//break;
		float val = mClipSlider.GetPos()*6.0f;
		CString tmp;
		tmp.Format(_T("%0.2f"), val);
		mClipValue.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetXAxis(val);
		break;
	}
	case IDC_NOISE_SLIDER:
	{
		float val = mNoiseSlider.GetPos()/100.0f;
		mParent->GetD3DEngine()->SetNoiseValue(val);
		CString tmp;
		tmp.Format(_T("%0.2f"), val);
		mNoiseVal.SetWindowTextW(tmp);
		break;
	}
	default:
		break;
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void DeltaCtrlDiag::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: Add your message handler code here
	//delete this;
}
