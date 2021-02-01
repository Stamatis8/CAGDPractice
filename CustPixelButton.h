#pragma once
#include "wx/wx.h"
#include "CustBitmapData.h"
class CustPixelButton : public wxBitmapButton
{
private:
    wxBitmap thisbitmapidle;
    wxBitmap thisbitmapselected;
    wxBitmap thisbitmapclicked;
    wxPoint WhereOnParent;
public:
    CustPixelButton(wxWindow* parent,
        wxWindowID id,
        const wxBitmap& bitmapidle,
        const wxBitmap& bitmapselected,
        const wxBitmap& bitmapclicked,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxBORDER_NONE,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    CustPixelButton(wxWindow* parent,
        wxWindowID id,
        CustBitmapData& DataInput,
        long style = wxBORDER_NONE,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr);
    void OnIdle(wxMouseEvent& event);
    void OnSelected(wxMouseEvent& event);
    void OnClicked(wxMouseEvent& event);
    void OnUnClicked(wxMouseEvent& event);
    CustBitmapData GetBitmapData();
    wxDECLARE_EVENT_TABLE();
};



