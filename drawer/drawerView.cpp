
// drawerView.cpp : implementation of the CdrawerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "drawer.h"
#endif

#include "drawerDoc.h"
#include "drawerView.h"
#include "D3DEngine.h"
#include "DeltaCtrlDiag.h"
#include "InputHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdrawerView

IMPLEMENT_DYNCREATE(CdrawerView, CView)

BEGIN_MESSAGE_MAP(CdrawerView, CView)
//	ON_WM_CREATE()
//	ON_WM_CLOSE()
//	ON_WM_ENTERIDLE()
//ON_WM_ENTERIDLE()
ON_COMMAND(ID_DELTA_VALUE, &CdrawerView::OnDeltaValue)
ON_COMMAND(ID_EDIT_GAUSS, &CdrawerView::OnEditGauss)
ON_UPDATE_COMMAND_UI(ID_EDIT_GAUSS, &CdrawerView::OnUpdateEditGauss)
ON_COMMAND(ID_EDIT_MSAA, &CdrawerView::OnEditMsaa)
ON_UPDATE_COMMAND_UI(ID_EDIT_MSAA, &CdrawerView::OnUpdateEditMsaa)
ON_WM_DESTROY()
//ON_COMMAND(ID_EDIT_MSAALEVEL, &CdrawerView::OnEditMsaalevel)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
ON_WM_MOUSEMOVE()
ON_UPDATE_COMMAND_UI(ID_MODE_STOCHASTICGAUSS, &CdrawerView::OnUpdateModeStochasticgauss)
ON_COMMAND(ID_MODE_STOCHASTICGAUSS, &CdrawerView::OnModeStochasticgauss)
END_MESSAGE_MAP()

// CdrawerView construction/destruction

CdrawerView::CdrawerView()
{
	// TODO: add construction code here
	mDiag = nullptr;
	mHandler = new InputHandler(this);
}

CdrawerView::~CdrawerView()
{
	if (mDiag) delete mDiag;
	if (mHandler) delete mHandler;
}

BOOL CdrawerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

BOOL CdrawerView::Render()
{
	return m3DEngine->Frame();
}

// CdrawerView drawing

void CdrawerView::OnDraw(CDC* pDC)
{
	CdrawerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CdrawerView::SetDeltaValue(int val)
{
	m3DEngine->SetSigmaValue(val);
}


// CdrawerView diagnostics

#ifdef _DEBUG
void CdrawerView::AssertValid() const
{
	CView::AssertValid();
}

void CdrawerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdrawerDoc* CdrawerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawerDoc)));
	return (CdrawerDoc*)m_pDocument;
}
#endif //_DEBUG


// CdrawerView message handlers

void CdrawerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m3DEngine = new D3DEngine;

	CRect rc;
	GetClientRect(&rc);

	bool res(false);
	res = m3DEngine->Initialize(rc.Width(), rc.Height(), GetSafeHwnd());

	if (!res) PostQuitMessage(0);
	m3DEngine->SetAAMode(AA_GAUSS);
	m3DEngine->SetSigmaValue(50);
	m3DEngine->SetMSAAlvl(2);
	m3DEngine->SetClipEdgeValue(30);
	
	////////////////////////////////////////
	m3DEngine->SetLineWidth(2.0f);
	m3DEngine->SetYAxis(200.0f);
	////////////////////////////////////////
}

void CdrawerView::OnDeltaValue()
{
	// TODO: Add your command handler code here
	if (!mDiag)
	{
		mDiag = new DeltaCtrlDiag(this);
		mDiag->Create(IDD_DELTA_CTRL, nullptr);
		mDiag->mSlider.SetPos(50);
		mDiag->mDeltaVal.SetWindowTextW(L"50");
		mDiag->mMultiSlider.SetPos(40);
		mDiag->mMultiVal.SetWindowTextW(L"2");
		mDiag->mClipSlider.SetPos(30);
		mDiag->mClipValue.SetWindowTextW(L"2.50");
		mDiag->mNoiseSlider.SetPos(20);
		mDiag->mNoiseVal.SetWindowTextW(L"0.2");

		////////////////////////////////////////
		mDiag->SetLineWidth(2.0f);
		mDiag->SetYAxis(200.0f);
		////////////////////////////////////////
		switch (m3DEngine->GetAAMode())
		{
		case AA_GAUSS:
			OnEditGauss();
			break;
		case AA_MS:
			OnEditMsaa();
			break;
		case AA_STOCHASTIC_GAUSS:
			OnModeStochasticgauss();
			break;
		default:
			break;
		}

	}
	mDiag->ShowWindow(SW_SHOW);
}


void CdrawerView::OnEditGauss()
{
	m3DEngine->SetAAMode(AA_GAUSS);
	if (mDiag)
	{
		//mDiag->mSlider.EnableWindow(true);
		//mDiag->mThetSlider.EnableWindow(true);
		//mDiag->mClipSlider.EnableWindow(true);
		//mDiag->mMultiSlider.EnableWindow(false);
		//mDiag->mNoiseSlider.EnableWindow(false);
	}

}


void CdrawerView::OnUpdateEditGauss(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(AA_GAUSS == m3DEngine->GetAAMode());
}


void CdrawerView::OnEditMsaa()
{
	m3DEngine->SetAAMode(AA_MS);
	if (mDiag)
	{
		//mDiag->mSlider.EnableWindow(false);
		//mDiag->mThetSlider.EnableWindow(false);
		//mDiag->mClipSlider.EnableWindow(false);
		//mDiag->mMultiSlider.EnableWindow(true);
		//mDiag->mNoiseSlider.EnableWindow(false);
	}
}


void CdrawerView::OnUpdateEditMsaa(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(AA_MS == m3DEngine->GetAAMode());
}

void CdrawerView::OnUpdateModeStochasticgauss(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(AA_STOCHASTIC_GAUSS == m3DEngine->GetAAMode());
}


void CdrawerView::OnModeStochasticgauss()
{
	m3DEngine->SetAAMode(AA_STOCHASTIC_GAUSS);
	if (mDiag)
	{
		//mDiag->mSlider.EnableWindow(true);
		//mDiag->mThetSlider.EnableWindow(true);
		//mDiag->mClipSlider.EnableWindow(true);
		//mDiag->mMultiSlider.EnableWindow(false);
		//mDiag->mNoiseSlider.EnableWindow(true);
	}
}

void CdrawerView::OnDestroy()
{
	if (m3DEngine)
	{
		m3DEngine->Shutdown();
		delete m3DEngine;
		m3DEngine = nullptr;
	}
	if (mDiag)
	{
		delete mDiag;
		mDiag = nullptr;
	}

	CView::OnDestroy();
}

void CdrawerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mHandler->OnLButtonDown(nFlags, point);
	CView::OnLButtonDown(nFlags, point);
}


void CdrawerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	mHandler->OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
}

BOOL CdrawerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//mHandler->OnMouseWheel(nFlags, zDelta, pt);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CdrawerView::OnMouseMove(UINT nFlags, CPoint point)
{
	mHandler->OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);
}