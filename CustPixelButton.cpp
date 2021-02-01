#include "CustPixelButton.h"
wxBEGIN_EVENT_TABLE(CustPixelButton, wxBitmapButton)
EVT_ENTER_WINDOW(OnSelected)
EVT_LEAVE_WINDOW(OnIdle)
EVT_LEFT_DOWN(OnClicked)
EVT_LEFT_UP(OnUnClicked)
wxEND_EVENT_TABLE();


//CONSTRUCTOR
CustPixelButton::CustPixelButton(wxWindow* parent,
    wxWindowID id,
    const wxBitmap& bitmapidle,
    const wxBitmap& bitmapselected,
    const wxBitmap& bitmapclicked,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString& name)
    :wxBitmapButton(parent, id, bitmapidle, pos, size, style, validator, name)
{
    WhereOnParent = pos;
    thisbitmapidle = bitmapidle;
    thisbitmapselected = bitmapselected;
    thisbitmapclicked = bitmapclicked;
}

CustPixelButton::CustPixelButton(wxWindow* parent,
    wxWindowID id,
    CustBitmapData& DataInput,
    long style,
    const wxValidator& validator,
    const wxString& name)
    :wxBitmapButton(parent, id, DataInput.GetBitmap()[0], DataInput.GetPoint(), DataInput.GetSize(), style, validator, name)
{
    WhereOnParent = DataInput.GetPoint();
    thisbitmapidle = DataInput.GetBitmap()[0];
    thisbitmapselected = DataInput.GetBitmap()[1];
    thisbitmapclicked = DataInput.GetBitmap()[2];
}

void CustPixelButton::OnIdle(wxMouseEvent& event)
{
    this->SetBitmap(thisbitmapidle);
}

void CustPixelButton::OnSelected(wxMouseEvent& event)
{
    this->SetBitmap(thisbitmapselected);
}

void CustPixelButton::OnClicked(wxMouseEvent& event)
{
    this->SetBitmap(thisbitmapclicked);
}

void CustPixelButton::OnUnClicked(wxMouseEvent& event)
{
    this->SetBitmap(thisbitmapselected);
    MSWClickButtonIfPossible(this);
}

CustBitmapData CustPixelButton::GetBitmapData()
{
    std::vector<wxBitmap> BitmapVector = { this->thisbitmapidle,this->thisbitmapselected,this->thisbitmapclicked };
    CustBitmapData ReturnData(BitmapVector, BitmapVector[0].GetSize());
    ReturnData.SetPoint(this->WhereOnParent);
    return ReturnData;
}
