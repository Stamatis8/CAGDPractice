#include "CustPixelToggleButton.h"
wxBEGIN_EVENT_TABLE(CustPixelToggleButton, wxBitmapButton)
EVT_LEFT_DOWN(OnLeftClick)
EVT_ENTER_WINDOW(OnEnterWindow)
EVT_LEAVE_WINDOW(OnLeaveWindow)
wxEND_EVENT_TABLE();

wxWindowID CustPixelToggleButton::GetId()
{
	return this->m_windowId;
}

CustPixelToggleButton::CustPixelToggleButton(
	wxWindow* parent,
	wxWindowID id
	, CustBitmapData BtmDtInput)
	:wxBitmapButton(parent,
		id,
		BtmDtInput.GetBitmap()[0],
		BtmDtInput.GetPoint(),
		BtmDtInput.GetSize(),
		wxBORDER_NONE,
		wxDefaultValidator,
		wxButtonNameStr)
{
	Bitmaps = BtmDtInput;
	this->SetBackgroundColour(MaskColour);
}
CustPixelToggleButton::CustPixelToggleButton(
	wxWindow* parent,
	wxWindowID id,
	CustBitmapData BtmDtInput,
	wxColour MaskColourInput)
	:wxBitmapButton(parent,
		id,
		BtmDtInput.GetBitmap()[0],
		BtmDtInput.GetPoint(),
		BtmDtInput.GetSize(),
		wxBORDER_NONE,
		wxDefaultValidator,
		wxButtonNameStr)
{
	Bitmaps = BtmDtInput;
	MaskColour = MaskColourInput;
	this->SetBackgroundColour(MaskColour);
}

bool CustPixelToggleButton::IsActive()
{
	return IsActiveAttribute;
}

bool CustPixelToggleButton::Toggle()
{
	if (IsActiveAttribute) {
		this->IsActiveAttribute = false;
		this->SetBitmap(Bitmaps.GetBitmap()[1]);
	}
	else {
		this->IsActiveAttribute = true;
		this->SetBitmap(Bitmaps.GetBitmap()[3]);
	}
	return true;
}

void CustPixelToggleButton::OnEnterWindow(wxMouseEvent& event)
{
	if (IsActiveAttribute)this->SetBitmap(Bitmaps.GetBitmap()[3]);
	else this->SetBitmap(Bitmaps.GetBitmap()[1]);
}

void CustPixelToggleButton::OnLeaveWindow(wxMouseEvent& event)
{
	if (IsActiveAttribute)this->SetBitmap(Bitmaps.GetBitmap()[2]);
	else this->SetBitmap(Bitmaps.GetBitmap()[0]);
}

void CustPixelToggleButton::OnLeftClick(wxMouseEvent& event)
{
	if (IsActiveAttribute) {
		this->IsActiveAttribute = false;
		this->SetBitmap(Bitmaps.GetBitmap()[1]);
	}
	else {
		this->IsActiveAttribute = true;
		this->SetBitmap(Bitmaps.GetBitmap()[3]);
	}
	MSWClickButtonIfPossible(this);
}

