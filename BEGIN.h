#pragma once
#include "wx/wx.h"
#include "CustPixelWindow.h"
class BEGIN : public wxApp
{
private:
	CustPixelWindow* window = nullptr;
public:
	bool OnInit();
};

