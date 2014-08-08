
// drawerView.h : interface of the CdrawerView class
//

#pragma once

class D3DEngine;
class DeltaCtrlDiag;
class CdrawerDoc;
class InputHandler;

class CdrawerView : public CView
{
protected: // create from serialization only
	CdrawerView();
	DECLARE_DYNCREATE(CdrawerView)

// Attributes
public:
	CdrawerDoc* GetDocument() const;

// Operations
public:
	void OnInitialUpdate();
	BOOL Render();
	void SetDeltaValue(int val);
	
	D3DEngine *GetD3DEngine() const{return m3DEngine;}
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CdrawerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	D3DEngine *m3DEngine;
	DeltaCtrlDiag *mDiag;
	InputHandler *mHandler;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClose();
	afx_msg void OnDeltaValue();
	afx_msg void OnEditGauss();
	afx_msg void OnUpdateEditGauss(CCmdUI *pCmdUI);
	afx_msg void OnEditMsaa();
	afx_msg void OnUpdateEditMsaa(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in drawerView.cpp
inline CdrawerDoc* CdrawerView::GetDocument() const
   { return reinterpret_cast<CdrawerDoc*>(m_pDocument); }
#endif

