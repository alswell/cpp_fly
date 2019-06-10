#include "LiteBKG.h"

CLiteInterface::~CLiteInterface()
{

}

CLiteBKG::CLiteBKG(CLiteInterface& wnd, CLiteDCInterface& dcImpl, int W, int H)
	: CLiteCtrlBase(RECT(0, 0, W, H), dcImpl)
	, m_wnd(wnd)
	, m_rcPaintRgn(0, 0, W, H)
	, m_nWidth(W)
	, m_nHeight(H)
{
	m_pHoverCtrl = nullptr;
	m_pDownCtrl = nullptr;

	m_pTip = ADD_TIP(0, 0, 120, 60);
	m_pTip->ShowCtrl(FALSE);

	m_pBKG = ADD_BLOCK(0, 0, W, H, RGBH(1EA084), RGBH(00FF00));
	m_pTitle = ADD_TXT(0, 0, 50, 20, "CVLab", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_BTN(W - 20, 0, 20, 20, "X", StdBtn, OnClose);
}


CLiteBKG::~CLiteBKG()
{
}

void CLiteBKG::OnClose()
{
	m_wnd.OnClose();
}

void CLiteBKG::InvalidateCtrl2(RECT& rc)
{
	m_rcPaintRgn.UnionRect(RECT(m_rcPaintRgn), rc);
	m_wnd.Refresh(rc);
}

void CLiteBKG::OnPaint()
{
	if (m_rcPaintRgn.IsRectEmpty())
		return;
	DrawChildren(POINT(0, 0), RECT(0, 0, m_nWidth, m_nHeight));
	m_rcPaintRgn.SetRectEmpty();
}

void CLiteBKG::OnMouseMove(POINT pt)
{
	m_pTip->MoveTo(pt.x + 20, pt.y + 20);
	if (m_pDownCtrl)
		m_pDownCtrl->ActivateMove(pt);
	CMouseCapturer* pHandler = WantCapture(pt);
	if (m_pHoverCtrl && m_pHoverCtrl != pHandler)
		m_pHoverCtrl->ReleaseMouse(m_pDownCtrl);
	m_pHoverCtrl = pHandler;
	if (!m_pDownCtrl && m_pHoverCtrl)
		m_pHoverCtrl->CaptureMouse();

	bool bShowTip = false;
	if (m_pHoverCtrl)
	{
		char* str = nullptr;
		if (!(m_pDownCtrl && m_pDownCtrl != m_pHoverCtrl))
			str = m_pHoverCtrl->GetTipString();
		if (str)
		{
			bShowTip = true;
			m_pTip->SetText(str);
		}
	}
	m_pTip->ShowCtrl(bShowTip);
}

void CLiteBKG::OnLBtnDown(POINT pt)
{
	if (m_pHoverCtrl)
	{
		m_pDownCtrl = m_pHoverCtrl;
		m_pDownCtrl->Activate(pt);
	}
}

void CLiteBKG::OnLBtnUp()
{
	if (m_pDownCtrl)
	{
		if (m_pDownCtrl == m_pHoverCtrl)
		{
			m_pDownCtrl->Inactivate(true);
		}
		else
		{
			m_pDownCtrl->Inactivate(false);
			if (m_pHoverCtrl)
				m_pHoverCtrl->CaptureMouse();
		}
		m_pDownCtrl = nullptr;
	}
}

void CLiteBKG::OnMouseWhell(int zDelta)
{
	if (m_pHoverCtrl && !m_pDownCtrl)
	{
		m_pHoverCtrl->MouseWhell(zDelta);
	}
}

void CLiteBKG::OnRBtnDown(POINT pt)
{
	m_pHoverCtrl->RBtnDown(pt);
}