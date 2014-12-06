#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"

using namespace std;

class CShape
{
public:
	POINT ptStart;
	POINT ptEnd;
	COLORREF myRGB;

	virtual bool judge(POINT ptJudge) = 0;
	virtual void draw(HDC hdc, bool erase)
	{
		SelectObject(hdc, GetStockObject(NULL_BRUSH));

		if (erase == false)
		{
			SelectObject(hdc, CreatePen(PS_SOLID, 3, myRGB));
		}
		else
		{
			SelectObject(hdc, GetStockObject(WHITE_PEN));
		}
	}

	
	void move(HDC hdc, int deltaX, int deltaY)
	{
		draw(hdc, true);
		ptStart.x +=deltaX;
		ptStart.y +=deltaY;
		ptEnd.x += deltaX;
		ptEnd.y += deltaY;
		draw(hdc, false);
	}

};

class CRect: public CShape
{
public:
	CRect(POINT start, POINT end, COLORREF rgb)
	{
		ptStart = start;
		ptEnd = end;
		myRGB = rgb;
	}
	bool judge(POINT ptJudge)
	{
		if (ptJudge.x > ptStart.x && ptJudge.x < ptEnd.x && ptJudge.y > ptStart.y && ptJudge.y < ptEnd.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw(HDC hdc, bool erase)
	{
		CShape::draw(hdc, erase);
		Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	~CRect();


};

class CEllipse :public CShape
{
public:
	CEllipse(POINT start, POINT end, COLORREF rgb)
	{
		ptStart = start;
		ptEnd = end;
		myRGB = rgb;
	}
	bool judge(POINT ptJudge)
	{
		POINT ptMid = { (ptStart.x + ptEnd.x) / 2, (ptStart.y + ptEnd.y) / 2 };
		int radiumA = (ptEnd.x - ptStart.x) / 2;
		int radiumB = (ptEnd.y - ptStart.y) / 2;
		if (radiumB*radiumB*(ptJudge.x - ptMid.x)*(ptJudge.x - ptMid.x) + radiumA*radiumA*
			(ptJudge.y - ptMid.y)*(ptJudge.y - ptMid.y) < radiumA*radiumA*radiumB*radiumB)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw(HDC hdc, bool erase)
	{
		CShape::draw(hdc, erase);
		Ellipse(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}


};

class CSquare : public CShape
{
public:
	CSquare(POINT start, POINT end, COLORREF rgb)
	{
		ptStart = start;
		ptEnd = end;
		myRGB = rgb;
	}
	bool judge(POINT ptJudge)
	{
		if (ptJudge.x > ptStart.x && ptJudge.x < ptEnd.x && ptJudge.y > ptStart.y && ptJudge.y < ptStart.y + ptEnd.x - ptStart.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw(HDC hdc, bool erase)
	{
		CShape::draw(hdc, erase);
		Ellipse(hdc, ptStart.x, ptStart.y, ptEnd.x, ptStart.y + ptEnd.x - ptStart.x);
	}


};

class CCircle :public CShape
{
public:
	CCircle(POINT start, POINT end, COLORREF rgb)
	{
		ptStart = start;
		ptEnd = end;
		myRGB = rgb;
	}
	bool judge(POINT ptJudge)
	{
		POINT ptMid = { (ptStart.x + ptEnd.x) / 2, (ptStart.y + ptStart.y + ptEnd.x - ptStart.x) / 2 };
		if ((ptJudge.x - ptMid.x)*(ptJudge.x - ptMid.x) + (ptJudge.y - ptMid.y)*(ptJudge.y - ptMid.y) 
			< (ptEnd.x - ptStart.x)* (ptEnd.x - ptStart.x))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw(HDC hdc, bool erase)
	{
		CShape::draw(hdc, erase);
		Ellipse(hdc, ptStart.x, ptStart.y, ptEnd.x, ptStart.y + ptEnd.x - ptStart.x);
	}


};


