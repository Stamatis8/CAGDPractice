#include "BEGIN.h"
#include "wx/wx.h"
#include "CustPixelWindow.h"
#include "vector"
#include "Plot.h"
#include "cmath"
#include "BezierCurve.h"
wxIMPLEMENT_APP(BEGIN);

bool BEGIN::OnInit() {
	wxInitAllImageHandlers();
	//CREATING EXIT BUTTON
	wxImage IdleExtBtnPng(wxT("Buttons/ExitButton/Idle.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Idle(IdleExtBtnPng);
	wxImage ClickedExtBtnPng(wxT("Buttons/ExitButton/Clicked.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Clicked(ClickedExtBtnPng);
	wxImage SelectedExtBtnPng(wxT("Buttons/ExitButton/Selected.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Selected(SelectedExtBtnPng);
	std::vector<wxBitmap> ExtBtnBitmapList {Idle, Selected, Clicked};
	CustBitmapData ExtBtnData(ExtBtnBitmapList,wxSize(16,16));
	ExtBtnData.SetPoint(wxPoint(1452, 4));
	//

	//CREATING MIN BUTTON
	wxImage Idleimg1(wxT("Buttons/MinimizeButton/Idle.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Idle1(Idleimg1);
	wxImage Selectedimg1(wxT("Buttons/MinimizeButton/Selected.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Selected1(Selectedimg1);
	wxImage Clickedimg1(wxT("Buttons/MinimizeButton/Clicked.png"), wxBITMAP_TYPE_PNG);
	wxBitmap Clicked1(Clickedimg1);
	std::vector<wxBitmap> bitmaplist1 = { Idle1,Selected1,Clicked1 };
	CustBitmapData minbuttonbtm(bitmaplist1, wxSize(16, 16));
	minbuttonbtm.SetPoint(wxPoint(1452, 25));
	//

	//CREATING MOVE PANEL
	wxImage MovePanelPng(wxT("Windows/MovePanel.png"), wxBITMAP_TYPE_PNG);
	wxBitmap MovePanelBtm(MovePanelPng);
	std::vector<wxBitmap> MovePanelBtmList{ MovePanelBtm };
	CustBitmapData MovePanel(MovePanelBtmList, wxSize(846, 37));
	MovePanel.SetPoint(wxPoint(526, 4));
	//

	//CREATING WINDOW
	wxImage WindowBackroundPng(wxT("Windows/Window.png"), wxBITMAP_TYPE_PNG);
	wxBitmap WindowBackroundBtm(WindowBackroundPng);
	window = new CustPixelWindow(nullptr, wxID_ANY, WindowBackroundBtm, ExtBtnData, wxPoint(50, 50), wxSize(1472, 700),minbuttonbtm);
	window->SetMovePanel(MovePanel);
	window->Show();
	//

	//CREATING PLOT
	wxImage PlotPng(wxT("Windows/Canvas.png"), wxBITMAP_TYPE_PNG);
	wxBitmap PlotBtm(PlotPng);
	Plot* Graph = new Plot(window, PlotBtm,wxID_LOWEST,wxPoint(286,63));
	//

	//SettingSpecialCursor
	wxCursor SpCr = wxCursor(wxImage(wxT("Cursors/SpecialCursor.png"), wxBITMAP_TYPE_PNG));
	Graph->SetSpecialCursor(SpCr);
	//
	window->InitializeButtons();

	std::vector<PlotObject> Objects;

	std::vector<std::vector<double>> ControlPoints{
		{50,50},
		{100,100},
		{150,50},
		};
	std::vector<double> Range{
		0,
		1
	};
	
	PlotObject Bezier(ControlPoints, "BezierCurve", 50);
	//Objects.push_back(Bezier);

	std::vector<std::vector<double>> InterpolationPoints{
		{50,50},
		{100,100},
		{150,50},
		{200,300},

	};

	PlotObject AitekensInterpolant(InterpolationPoints, "AitekensInterpolant", 50);
	//Objects.push_back(AitekensInterpolant);

	std::vector<std::vector<double>> Points{
	{0,0},
	{0,617},
	{1166,617},
	{1166,0},
	{0,0}
	};
	PlotObject InitialRectangle(Points);
	Objects.push_back(InitialRectangle);
	for (auto i : Objects) {
		Graph->AddObject(i);
	}

	return true;
}