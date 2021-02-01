#include "CustBitmapData.h"


//CONSTRUCTORS
CustBitmapData::CustBitmapData(std::vector<wxBitmap> inputbtm, wxSize inputsize, int inputIsNull)
{
	bitmap = inputbtm;
	size = inputsize;
	IsNull = inputIsNull;
}
CustBitmapData::CustBitmapData(std::vector<wxBitmap> inputbtm, wxColour colourinput) {
	bitmap = inputbtm;
	size = inputbtm[0].GetSize();
	BackroundColour = colourinput;
}
CustBitmapData::CustBitmapData(std::vector<wxBitmap> inputbtm)
{
	bitmap = inputbtm;
	size = inputbtm[0].GetSize();
}
wxColour CustBitmapData::GetBackroundColour()
{
	return this->BackroundColour;
}
CustBitmapData::CustBitmapData() {
	IsNull = 0;
}
std::vector<wxBitmap> CustBitmapData::GetBitmap()
{
	if(IsNull==1)return bitmap;
}

wxSize CustBitmapData::GetSize()
{
	if(IsNull == 1)return size;
}

void CustBitmapData::SetBitmap(std::vector<wxBitmap> setbtm)
{
	bitmap.clear();
	bitmap = setbtm;
	size.x = bitmap[0].GetWidth();
	size.y = bitmap[0].GetHeight();
	IsNull = 1;
}

void CustBitmapData::SetSize(wxSize setsize)
{
	size = setsize;
}

int CustBitmapData::GetIsNull()
{
	return IsNull;
}

void CustBitmapData::Clear()
{
	bitmap.clear();
	size.x = 0;
	size.y = 0;
	IsNull = 0;
}

wxPoint CustBitmapData::GetPoint()
{
	return point;
}

void CustBitmapData::SetPoint(wxPoint inputpoint)
{
	point = inputpoint;
}

bool CustBitmapData::SetIsNull(int Input)
{
	this->IsNull = Input;
	return true;
}
