#pragma once
#include "wx/wx.h"
#include "vector"

class CustBitmapData
{
private:
	std::vector<wxBitmap> bitmap;
	wxSize size;
	int IsNull = 0;
	wxPoint point;
	wxColour BackroundColour;
public:
	CustBitmapData();
	CustBitmapData(std::vector<wxBitmap> inputbtm, wxSize inputsize, int inputIsNull = 1);
	CustBitmapData(std::vector<wxBitmap> inputbtm,wxColour colourinput);
	CustBitmapData(std::vector<wxBitmap> inputbtm);
	wxColour GetBackroundColour();
	std::vector<wxBitmap> GetBitmap();
	wxSize GetSize();
	void SetBitmap(std::vector<wxBitmap> setbtm);
	void SetSize(wxSize setsize);
	int GetIsNull();
	void Clear();
	wxPoint GetPoint();
	void SetPoint(wxPoint inputpoint);
	bool SetIsNull(int Input);

};



