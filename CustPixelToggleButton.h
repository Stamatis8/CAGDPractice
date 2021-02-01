#pragma once
#include "wx/wx.h"
#include "CustBitmapData.h"

class CustPixelToggleButton : public wxBitmapButton
{
private:
	CustBitmapData Bitmaps;
	/*
	* [0]->IdleInactive
	* [1]->Selected Inactive
	* [2]->IdleActive
	* [3]->SelectedActive
	*/

	bool IsActiveAttribute = false;

	wxColour MaskColour = "#d4d4d4";
public:
	wxWindowID GetId();
	//Constructors
	CustPixelToggleButton(
		wxWindow* parent,
		wxWindowID id,
		CustBitmapData BtmDtInput);
	CustPixelToggleButton(
		wxWindow* parent,
		wxWindowID id,
		CustBitmapData BtmDtInput,
		wxColour MaskColourInput);
	//

	bool IsActive();
	bool Toggle();

	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);
	void OnLeftClick(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

