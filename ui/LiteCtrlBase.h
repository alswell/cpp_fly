#pragma once
#include "func.h"

struct CSize
{
	int cx, cy;
};
typedef unsigned long COLORREF;
typedef int BOOL;
typedef unsigned UINT;
#define CLR_NONE                0xFFFFFFFFL
#define CLR_DEFAULT             0xFF000000L
#define FALSE 0
#define TRUE  1
//#define RGBH(v)					((0x##v & 0xff) << 16 | (0x##v & 0xff00) | (0x##v >> 16))	// RGBH(rgb) =  RGB(r,g,b)
#define RGBH(v)					0x##v

#include <vector>
#include <map>
using namespace std;

#ifndef CONF_THIS_CLS
#define CONF_THIS_CLS(cls) typedef cls THIS_CLS
#endif

#define RectW(l, t, w, h)		(RECT(l, t, (l + w), (t + h)))

#define DRAW_INIT(rcDraw, rcRelLoc, ptOffset, rcRgn, rcParentRgn) \
	RECT rcDraw = rcRelLoc;\
	RECT rcRgn;\
	rcDraw.OffsetRect(ptOffset);\
	rcRgn.IntersectRect(rcDraw, rcParentRgn);\
	if (rcRgn.IsRectEmpty())\
		return;

enum ETextFormat
{
	TXT_FMT_TOP = 0x00000000,
	TXT_FMT_LEFT = 0x00000000,
	TXT_FMT_CENTER = 0x00000001,
	TXT_FMT_RIGHT = 0x00000002,
	TXT_FMT_VCENTER = 0x00000004,
	TXT_FMT_BOTTOM = 0x00000008,
	TXT_FMT_WORDBREAK = 0x00000010,
	TXT_FMT_SINGLELINE = 0x00000020,
	TXT_FMT_EXPANDTABS = 0x00000040,
	TXT_FMT_TABSTOP = 0x00000080,
	TXT_FMT_NOCLIP = 0x00000100,
	TXT_FMT_EXTERNALLEADING = 0x00000200,
	TXT_FMT_CALCRECT = 0x00000400,
	TXT_FMT_NOPREFIX = 0x00000800,
	TXT_FMT_INTERNAL = 0x00001000,

	TXT_FMT_EDITCONTROL = 0x00002000,
	TXT_FMT_PATH_ELLIPSIS = 0x00004000,
	TXT_FMT_END_ELLIPSIS = 0x00008000,
	TXT_FMT_MODIFYSTRING = 0x00010000,
	TXT_FMT_RTLREADING = 0x00020000,
	TXT_FMT_WORD_ELLIPSIS = 0x00040000,
	TXT_FMT_NOFULLWIDTHCHARBREAK = 0x00080000,
	TXT_FMT_HIDEPREFIX = 0x00100000,
	TXT_FMT_PREFIXONLY = 0x00200000,
};
#define STD_TXT_FMT (TXT_FMT_CENTER | TXT_FMT_VCENTER | TXT_FMT_SINGLELINE | TXT_FMT_END_ELLIPSIS)

class CLiteDCInterface
{
public:
	virtual ~CLiteDCInterface();
	virtual void Point(int x, int y, COLORREF clr) = 0;
	virtual void Line(int x1, int y1, int x2, int y2, COLORREF clr) = 0;
	virtual void Rectangle(RECT rcRgn, RECT rcDraw, COLORREF clrBorder, COLORREF clrBKG) = 0;
	virtual void TextStd(RECT rcRgn, RECT rcDraw, char * str, COLORREF clr) = 0;
	virtual void Text(RECT rcRgn, RECT rcDraw, char * str, COLORREF clr, unsigned nFormat) = 0;
	virtual void BitBlt(const CLiteDCInterface& dc_src, int src_x, int src_y, unsigned w, unsigned h, int des_x, int des_y) = 0;
	virtual char* BeginData(int x, int y, unsigned w, unsigned h) = 0;
	virtual void EndData() = 0;
};

class CMouseCapturer;
class CLiteCtrlBase
{
public:
	CLiteCtrlBase(RECT rcRelLoc, CLiteDCInterface& dcImpl);
	CLiteCtrlBase(RECT rcRelLoc, CLiteCtrlBase* pParentCtrl);
	virtual ~CLiteCtrlBase();

protected:
	CLiteDCInterface& m_dcImpl;
	virtual void Draw(POINT ptOffset, RECT rcParentRgn);
	void DrawChildren(POINT ptOffset = POINT(0, 0), RECT rcRgn = RECT());
	virtual CMouseCapturer* WantCapture(POINT pt);
	//virtual CLiteCtrlBase* LBtnDown(CPoint pt);
	//virtual CLiteCtrlBase* LBtnUp(CPoint pt);

protected:
	BOOL m_bIsMouseIn;
	POINT m_ptMousePos;
	RECT m_rcRelLoc;
	CSize m_szBmp;
	POINT m_ptScroll;
	map<int, vector<CLiteCtrlBase*>> m_vCtrls;
private:
	RECT* m_pOldRect;
protected:
	UINT m_nTimerID;
public:
	CLiteCtrlBase* m_pParentCtrl;
	void InvalidateCtrl();
protected:
	virtual void InvalidateCtrl2(RECT& rc);
public:
	BOOL m_bAlpha;
	BOOL m_bIsVisible;
	void ShowCtrl(BOOL bShow);
	int m_nZOrder;

	POINT ParentToChild(POINT pt);
	CLiteCtrlBase* AddCtrl(CLiteCtrlBase* pCtrl, int nZOrder = 0);
	RECT GetDrawRect();
	void MoveToX(int x);
	void MoveToY(int y);
	void MoveTo(int x, int y);
	void Move(int x, int y);
};
