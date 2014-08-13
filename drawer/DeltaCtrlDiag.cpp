// DeltaCtrlDiag.cpp : implementation file
//

#include "stdafx.h"
#include "drawer.h"
#include "DeltaCtrlDiag.h"
#include "afxdialogex.h"
#include "drawerView.h"
#include "D3DEngine.h"


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
	DDX_Control(pDX, IDC_TRANSPARENCY_SLIDER, mTransSlider);
	DDX_Control(pDX, IDC_TRANSPARNCY_VALUE, mTransVal);
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
		int val = mSlider.GetPos();
		CString tmp;
		tmp.Format(_T("%d"), val);
		mDeltaVal.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetSigmaValue(val);
		break;
	}
	case IDC_THET_SLIDER:
	{
		int val = mThetSlider.GetPos();
		CString tmp;
		tmp.Format(_T("%d"), val);
		mThetVal.SetWindowTextW(tmp);
		mParent->GetD3DEngine()->SetThetValue(val);
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
		int val = mClipSlider.GetPos();
		mParent->GetD3DEngine()->SetClipEdgeValue(val);
		float val2 = val/20.0f+1.0f;
		CString tmp;
		tmp.Format(_T("%0.2f"), val2);
		mClipValue.SetWindowTextW(tmp);
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
	case IDC_TRANSPARENCY_SLIDER:
	{
		float val =(1+ mTransSlider.GetPos())/101.0f;
		float alpha = (mParent->GetD3DEngine()->
			GetPixelBuffer().color[3] > 0)?val:-val;
		mParent->GetD3DEngine()->GetPixelBuffer().color[3] = alpha;
		CString tmp;
		tmp.Format(_T("%0.2f"), val);
		mTransVal.SetWindowTextW(tmp);
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
