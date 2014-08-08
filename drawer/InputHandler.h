#pragma once
class CdrawerView;

class InputHandler
{
public:
	InputHandler(void);
	InputHandler(CdrawerView *parent);
	~InputHandler(void);

	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint point);
protected:
	CdrawerView *mParent;
	CPoint mMarkedPos;

	bool mIsLeftButtonDown;
};

