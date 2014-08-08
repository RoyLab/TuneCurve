#include "stdafx.h"
#include "InputHandler.h"
#include "drawerView.h"
#include "D3DEngine.h"
#include "Camera.h"
#include "VirtualDeviceDx11.h"
#include <cmath>

InputHandler::InputHandler(void):
	mParent(nullptr), mIsLeftButtonDown(false)
{
}

InputHandler::InputHandler(CdrawerView *parent)
{
	new(this)(InputHandler);
	mParent = parent;
}


InputHandler::~InputHandler(void)
{
}

void InputHandler::OnLButtonUp(UINT nFlags, CPoint point)
{
	mIsLeftButtonDown = false;
}

void InputHandler::OnLButtonDown(UINT nFlags, CPoint point)
{
	mIsLeftButtonDown = true;
	mMarkedPos = point;
}

BOOL InputHandler::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Camera *cam = mParent->GetD3DEngine()->GetCamera();
	VirtualDeviceDx11 *dev = mParent->GetD3DEngine()->GetDevice();
	cam->ScaleView((float)pow(1.001f, zDelta));
	dev->SetScale(cam->GetScale());
	return true;
}

void InputHandler::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!mIsLeftButtonDown) return;

	CPoint tmp = (point - mMarkedPos);
	mParent->GetD3DEngine()->GetCamera()->Translate((float)tmp.x, (float)tmp.y);
	mMarkedPos = point;
}
