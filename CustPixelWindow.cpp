#include "CustPixelWindow.h"
#include <wx/utils.h>


BEGIN_EVENT_TABLE(CustPixelWindow, wxFrame)
EVT_BUTTON(wxID_EXIT, CustPixelWindow::OnExit)
EVT_BUTTON(wxID_HIGHEST + 888, CustPixelWindow::OnMinimize)
//SelectionButtons
EVT_BUTTON(wxID_HIGHEST + 1, CustPixelWindow::OnSelectionStartButton)
EVT_BUTTON(wxID_HIGHEST + 2, CustPixelWindow::OnSelectionEndButton)
EVT_BUTTON(wxID_HIGHEST + 3, CustPixelWindow::OnSelectionNextButton)
EVT_BUTTON(wxID_HIGHEST + 4, CustPixelWindow::OnSelectionPreviousButton)
EVT_BUTTON(wxID_HIGHEST + 5, CustPixelWindow::OnSelectingPointsToggleButton)
//
//TranslationButtons
EVT_BUTTON(wxID_HIGHEST + 6, CustPixelWindow::OnUpButton)
EVT_BUTTON(wxID_HIGHEST + 7, CustPixelWindow::OnUpUpButton)
EVT_BUTTON(wxID_HIGHEST + 8, CustPixelWindow::OnDownButton)
EVT_BUTTON(wxID_HIGHEST + 9, CustPixelWindow::OnDownDownButton)
EVT_BUTTON(wxID_HIGHEST + 10, CustPixelWindow::OnRightButton)
EVT_BUTTON(wxID_HIGHEST + 11, CustPixelWindow::OnRightRightButton)
EVT_BUTTON(wxID_HIGHEST + 12, CustPixelWindow::OnLeftButton)
EVT_BUTTON(wxID_HIGHEST + 13, CustPixelWindow::OnLeftLeftButton)
//
EVT_MOTION(CustPixelWindow::OnMoveWindow)
EVT_LEFT_DOWN(CustPixelWindow::OnLeftDown)
EVT_LEFT_UP(CustPixelWindow::OnLeftUp)
EVT_LEAVE_WINDOW(CustPixelWindow::OnMouseLeave)
END_EVENT_TABLE();




//CONSTRUCTOR
CustPixelWindow::CustPixelWindow
(wxWindow* parent,
    wxWindowID id,
    wxBitmap backroundbitmap,
    CustBitmapData exitbuttonbtminput,
    const wxPoint& pos,
    const wxSize& size,
    CustBitmapData minbuttonbtminput,
    long style,
    const wxString& name)
	:wxFrame(parent,id,"",pos,size,style,name)
{
    
    BackroundBitmap = backroundbitmap;
    
    //Initializing ExitButton
    ExitButton = new CustPixelButton(this, wxID_EXIT, exitbuttonbtminput.GetBitmap()[0], exitbuttonbtminput.GetBitmap()[1], exitbuttonbtminput.GetBitmap()[2], exitbuttonbtminput.GetPoint(), exitbuttonbtminput.GetSize());
    ExitButton->SetBackgroundColour(wxColour(backroundcolour));
    
    //Initializing MinButton
    if (minbuttonbtminput.GetIsNull() == 1) {
        MinButton = new CustPixelButton(this, wxID_HIGHEST + 888, minbuttonbtminput.GetBitmap()[0], minbuttonbtminput.GetBitmap()[1], minbuttonbtminput.GetBitmap()[2], minbuttonbtminput.GetPoint(), minbuttonbtminput.GetSize());
        MinButton->SetBackgroundColour(wxColour(backroundcolour));
    }

    //Connecting Paint Event
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(CustPixelWindow::OnPaint));
}

bool CustPixelWindow::InitializeButtons()
{
    std::vector<wxBitmap> BtmVec;
    CustBitmapData BtmDt;
    int Id = 0;
    //[1]SelectionButtons
        //SelectionStartButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
        wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionStartButton/Idle.png"), wxBITMAP_TYPE_PNG))
        ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionStartButton/Selected.png"), wxBITMAP_TYPE_PNG))
        ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionStartButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(40, 85));
    SelectionStartButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SelectionStartButton->SetBackgroundColour(BtmDt.GetBackroundColour());
        //SelectionEndButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(40, 119));
    SelectionStartButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SelectionStartButton->SetBackgroundColour(BtmDt.GetBackroundColour());
        //SelectionNextButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionNextButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionNextButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionNextButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(144, 119));
    SelectionStartButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SelectionStartButton->SetBackgroundColour(BtmDt.GetBackroundColour());
        //SelectionPreviousButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionPreviousButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionPreviousButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionPreviousButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(110, 119));
    SelectionStartButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SelectionStartButton->SetBackgroundColour(BtmDt.GetBackroundColour());
    //End[1]

    //[2]Point Selection Buttons
        //SelectingPointsToggleButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectingPointsToggleButton/IdleInactive.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectingPointsToggleButton/SelectedInactive.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectingPointsToggleButton/IdleActive.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectingPointsToggleButton/SelectedActive.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(110, 85));
    SelectingPointsToggleButton = new CustPixelToggleButton(this, wxID_HIGHEST + Id, BtmDt,"#d4d4d4");
    //End[2]

    //[3]Translation Buttons
        //Up
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Up/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Up/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Up/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(127, 349));
    UpButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    UpButton->SetBackgroundColour("#d4d4d4");
        //UpUp
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/UpUp/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/UpUp/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/UpUp/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(127, 315));
    UpUpButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    UpUpButton->SetBackgroundColour("#d4d4d4");
        //Down
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Down/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Down/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Down/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(127, 417));
    DownButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    DownButton->SetBackgroundColour("#d4d4d4");
        //DownDown
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/DownDown/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/DownDown/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/DownDown/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(127, 451));
    DownDownButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    DownDownButton->SetBackgroundColour("#d4d4d4");
        //Right
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Right/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Right/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Right/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(161, 383));
    RightButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    RightButton->SetBackgroundColour("#d4d4d4");
        //RightRight
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/RightRight/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/RightRight/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/RightRight/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(195, 383));
    RightRightButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    RightRightButton->SetBackgroundColour("#d4d4d4");
        //Left
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Left/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Left/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/Left/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(93, 383));
    LeftButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    LeftButton->SetBackgroundColour("#d4d4d4");
        //LeftLeft
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/TranslationButtons/LeftLeft/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/LeftLeft/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/TranslationButtons/LeftLeft/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(59, 383));
    LeftLeftButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    LeftLeftButton->SetBackgroundColour("#d4d4d4");
    //End[3]

    //[4]Object Creation Buttons
            //EndObjectCreation
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/SelectionButtons/SelectionEndButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(80, 252));
    EndObjectCreationButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    EndObjectCreationButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnEndObjectCreation, this, wxID_HIGHEST + Id);
        //DeleteObjectButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeleteObjectButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeleteObjectButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeleteObjectButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(182, 86));
    DeleteObjectButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    DeleteObjectButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnDeleteObjectButton, this, wxID_HIGHEST + Id);
        //DeletePointButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeletePointButton/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeletePointButton/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/DeletePointButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(182, 120));
    DeletePointButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    DeletePointButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnDeletePointButton, this, wxID_HIGHEST + Id);

        //StartControlPolygonCreationButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
    wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/ControlPolygonButtons/Start/Idle.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/ControlPolygonButtons/Start/Selected.png"), wxBITMAP_TYPE_PNG))
    ,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/ControlPolygonButtons/Start/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(41, 184));
    StartControlPolygonCreationButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    StartControlPolygonCreationButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnStartControlPolygonCreation,this,wxID_HIGHEST+Id);
        
        //StartAitekensInterpolantCreationButton
    Id++;
    BtmVec = std::vector<wxBitmap>{
wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/AitekensInterpolantButton/Idle.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/AitekensInterpolantButton/Selected.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/ObjectCreationButtons/AitekensInterpolantButton/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(119, 184));
    StartAitekensInterpolantCreationButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    StartAitekensInterpolantCreationButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnAitekensInterpolantButton, this, wxID_HIGHEST + Id);
    //
    //[5]Bezier Specific
        //Elevate Degree
    Id++;
    BtmVec = std::vector<wxBitmap>{
wxBitmap(wxImage(wxT("Buttons/BezierButtons/ElevateDegree/Idle.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/ElevateDegree/Selected.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/ElevateDegree/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(191, 186));
    ElevateBezierDegreeButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    ElevateBezierDegreeButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnElevateDegreeButton, this, wxID_HIGHEST + Id);
        //Subdivide 1/4
    Id++;
    BtmVec = std::vector<wxBitmap>{
wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Idle.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Selected.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(203, 236));
    SubdivideBezierQuarterButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SubdivideBezierQuarterButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnSubdivideButtonQuarter, this, wxID_HIGHEST + Id);
        //Subdivide 1/2
    Id++;
    BtmVec = std::vector<wxBitmap>{
wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Idle.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Selected.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(229, 245));
    SubdivideBezierHalfButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SubdivideBezierHalfButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnSubdivideButtonHalf, this, wxID_HIGHEST + Id);
    //Subdivide 3/4
    Id++;
    BtmVec = std::vector<wxBitmap>{
wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Idle.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Selected.png"), wxBITMAP_TYPE_PNG))
,wxBitmap(wxImage(wxT("Buttons/BezierButtons/Subdivide/Clicked.png"), wxBITMAP_TYPE_PNG))
    };
    BtmDt = CustBitmapData(BtmVec, BtmVec[0].GetSize());
    BtmDt.SetPoint(wxPoint(216, 268));
    SubdivideBezierThreeQuartersButton = new CustPixelButton(this, wxID_HIGHEST + Id, BtmDt);
    SubdivideBezierThreeQuartersButton->SetBackgroundColour("#d4d4d4");
    this->Bind(wxEVT_BUTTON, &CustPixelWindow::OnSubdivideButtonThreeQuarters, this, wxID_HIGHEST + Id);

    //
    return true;
}

wxColour CustPixelWindow::GetBackroundColour()
{
    return this->backroundcolour;
}

//DRAW
void CustPixelWindow::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(BackroundBitmap,wxPoint(0,0));
    if (MovePanel.GetIsNull() == 1) {
        dc.DrawBitmap(MovePanel.GetBitmap()[0], MovePanel.GetPoint());
    }
}

//EXIT
void CustPixelWindow::OnExit(wxCommandEvent& event) {
    this->Close();
}

//METHODS
void CustPixelWindow::OnMinimize(wxCommandEvent& event)
{
    this->Iconize();
}

void CustPixelWindow::SetMovePanel(CustBitmapData movepanelbtminput)
{
    MovePanel = movepanelbtminput;
}

bool CustPixelWindow::IsInWindow(wxPoint pnt, CustBitmapData Data)
{
    wxSize A = Data.GetSize();
    wxPoint P = Data.GetPoint();
    if ((pnt.x > P.x)
        && (pnt.y > P.y)
        && (pnt.x < P.x + A.x)
        && (pnt.y < P.y + A.y)) {
        return true;
    }
    else return false;
}

//---------------------------------------------------
//CURSOR HANDLING START
//---------------------------------------------------

void CustPixelWindow::OnMoveWindow(wxMouseEvent& event)
{
    //(pos.x > MovePanel.GetPoint().x) && (pos.y > MovePanel.GetPoint().y) && (pos.x < (MovePanel.GetPoint().x + MovePanel.GetSize().GetWidth())) && (pos.y < (MovePanel.GetPoint().y + MovePanel.GetSize().GetHeight()))
    wxPoint pos = event.GetPosition();
    if (IsInWindow(pos,this->MovePanel)) {

        if (event.Dragging()) {
            int dx = pos.x - LastPos.x;
            int dy = pos.y - LastPos.y;
            wxPoint origin = this->GetPosition();
            this->SetPosition(wxPoint(origin.x + dx, origin.y + dy));
        }
    }
}

void CustPixelWindow::OnLeftUp(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (HasCapture()) {
            this->ReleaseMouse();
     }
    
}

void CustPixelWindow::OnLeftDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    //Creating Plot BitmapData
    CustBitmapData PlotBitmapData;
    PlotBitmapData.SetSize(dynamic_cast<Plot*>(this->GetChildren()[PlotPosition])->GetSize());
    PlotBitmapData.SetPoint(dynamic_cast<Plot*>(this->GetChildren()[PlotPosition])->GetPosition());
    PlotBitmapData.SetIsNull(1);
    //

    if (IsInWindow(pos, this->MovePanel)) {
        this->CaptureMouse();
        if (event.LeftIsDown()) {
            LastPos = pos;
        }
    }
    else if (CreatingControlPolygon && IsInWindow(pos, PlotBitmapData)) {
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        //[*]pos is now based on parent window. We now conver it to plot window
        wxPoint AdjustedPos = wxPoint(
            pos.x - plot->GetPosition().x
            ,plot->GetSize().y - (pos.y - plot->GetPosition().y)
        );
        pos = AdjustedPos;//Pos is now cartesian on plot window
        //End[*]
        std::vector<double> ActualCoordinates = plot->ConvertCoordinatesToActual(
            std::vector<double> {double(pos.x), double(pos.y)});
        if (ObjectCreationIsFirstPoint) {
            std::vector<std::vector<double>> Point = { ActualCoordinates };
            plot->AddObject(PlotObject(Point, "BezierCurve", 50));
            CurrentObjectCreationIdentifier = plot->GetMaxIdentifier();
            ObjectCreationIsFirstPoint = false;
        }
        else {
            double PointSize = plot->GetObject(CurrentObjectCreationIdentifier - 1).GetPoints().size();
            double Precision;
            Precision = 100;
            plot->SetObjectPrecision(CurrentObjectCreationIdentifier,Precision);
            plot->AddControlPoint(CurrentObjectCreationIdentifier, ActualCoordinates);
        }
    }
    else if (CreatingAitekensInterpolant && IsInWindow(pos, PlotBitmapData)) {
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        //[*]pos is now based on parent window. We now conver it to plot window
        wxPoint AdjustedPos = wxPoint(
            pos.x - plot->GetPosition().x
            , plot->GetSize().y - (pos.y - plot->GetPosition().y)
        );
        pos = AdjustedPos;//Pos is now cartesian on plot window
        //End[*]
        std::vector<double> ActualCoordinates = plot->ConvertCoordinatesToActual(
            std::vector<double> {double(pos.x), double(pos.y)});
        if (ObjectCreationIsFirstPoint) {
            std::vector<std::vector<double>> Point = { ActualCoordinates };
            plot->AddObject(PlotObject(Point, "AitekensInterpolant", 50));
            CurrentObjectCreationIdentifier = plot->GetMaxIdentifier();
            ObjectCreationIsFirstPoint = false;
        }
        else {
            double PointSize = plot->GetObject(CurrentObjectCreationIdentifier - 1).GetPoints().size();
            double Precision;
            Precision = 100;
            plot->SetObjectPrecision(CurrentObjectCreationIdentifier, Precision);
            plot->AddInterpolationPoint(CurrentObjectCreationIdentifier, ActualCoordinates);
        }
    }
}

void CustPixelWindow::OnMouseLeave(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    if (HasCapture()) {
            this->ReleaseMouse();
    }
}

//---------------------------------------------------
//CURSOR HANDLING END
//---------------------------------------------------

void CustPixelWindow::OnSelectionStartButton(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    this->InvalidateCurrentPointSelection();
    if(plot->GetIdentifiers().size()!=0)this->CurrentlySelectedIdentifier = plot->GetIdentifiers()[0];
    this->CurrentlySelectedIdentifierPosition = 0;
    this->IsSelectionProcess = true;
    plot->SetSelectedIdentifiers(CurrentlySelectedIdentifier);
    if (SelectingPointsToggleButton->IsActive()) {
        this->OnSelectingPointsToggleButton(event);
        this->OnSelectingPointsToggleButton(event);
    }

}

void CustPixelWindow::OnSelectionEndButton(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    this->IsSelectionProcess = false;

    //Terminating PointSelection
    this->InvalidateCurrentPointSelection();
    //
    //Terminating ObjectSelection
    this->CurrentlySelectedIdentifierPosition = -1;
    this->CurrentlySelectedIdentifier = -1;
    plot->SetSelectedIdentifiers(CurrentlySelectedIdentifier);
    //
}

void CustPixelWindow::OnSelectionNextButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {
        if (!SelectingPointsToggleButton->IsActive()) {
            this->InvalidateCurrentPointSelection();
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            if (CurrentlySelectedIdentifierPosition >= plot->GetIdentifiers().size()-1) {
                CurrentlySelectedIdentifierPosition = 0;
                this->CurrentlySelectedIdentifier = plot->GetIdentifiers()[0];
            }
            else {
                this->CurrentlySelectedIdentifierPosition++;
                this->CurrentlySelectedIdentifier = plot->GetIdentifiers()[CurrentlySelectedIdentifierPosition];
            } 

            plot->SetSelectedIdentifiers(CurrentlySelectedIdentifier);
        }
        else {
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            //Setting Correct Point Position
            if (CurrentlySelectedPointPosition >= plot->GetObject(CurrentlySelectedIdentifier).GetPoints().size()-1) {
                CurrentlySelectedPointPosition = 0;
            }
            else CurrentlySelectedPointPosition++;
            //

            //Selecting Point in plot
            plot->SetSelectedPoints(std::vector<std::vector<double>> {
                plot->GetObject(CurrentlySelectedIdentifier).GetPoints()[CurrentlySelectedPointPosition]
            });
            //
        }
    }
}

void CustPixelWindow::OnSelectionPreviousButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {
        if (!SelectingPointsToggleButton->IsActive()) {
            this->InvalidateCurrentPointSelection();
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            if (this->CurrentlySelectedIdentifierPosition <= 0) {
                this->CurrentlySelectedIdentifierPosition = plot->GetIdentifiers().size() - 1;
                this->CurrentlySelectedIdentifier = plot->GetIdentifiers()[CurrentlySelectedIdentifierPosition];
            }
            else {
                this->CurrentlySelectedIdentifierPosition--;
                this->CurrentlySelectedIdentifier = plot->GetIdentifiers()[CurrentlySelectedIdentifierPosition];
                this->CurrentlySelectedIdentifier--;
            } 

            plot->SetSelectedIdentifiers(CurrentlySelectedIdentifier);
        }
        else {
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            //Setting Correct Point Position
            if (CurrentlySelectedPointPosition <= 0) {
                CurrentlySelectedPointPosition = plot->GetObject(CurrentlySelectedIdentifier).GetPoints().size() - 1;
            }
            else CurrentlySelectedPointPosition--;
            //
            //Selecting Point in plot
            plot->SetSelectedPoints(std::vector<std::vector<double>> {
                plot->GetObject(CurrentlySelectedIdentifier).GetPoints()[CurrentlySelectedPointPosition]
            });
            //
        }
    }
}

void CustPixelWindow::OnSelectingPointsToggleButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {
        if (SelectingPointsToggleButton->IsActive()) {
            CurrentlySelectedPointPosition = 0;
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            plot->SetSelectedPoints(std::vector<std::vector<double>> {
                plot->GetObject(CurrentlySelectedIdentifier).GetPoints()[CurrentlySelectedPointPosition]
            });
        }
        else {
            this->InvalidateCurrentPointSelection();
        }
    }
}

bool CustPixelWindow::RefreshPointsSelection()
{
    if (IsSelectionProcess) {
        if (SelectingPointsToggleButton->IsActive()) {
            Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
            int PointSizeNow = plot->GetObject(CurrentlySelectedIdentifier).GetPoints().size();
            int PreviousPointPosition = CurrentlySelectedPointPosition;
            //TogglingPointSelection
            MSWClickButtonIfPossible(SelectingPointsToggleButton);
            MSWClickButtonIfPossible(SelectingPointsToggleButton);
            //

            //Reselecting Point
            if (PreviousPointPosition<PointSizeNow && PreviousPointPosition>-1) {
                for (int i{ 0 };i < PreviousPointPosition;i++) {
                    CurrentlySelectedPointPosition++;
                }
            }

            plot->SetSelectedPoints(std::vector<std::vector<double>> {
                plot->GetObject(CurrentlySelectedIdentifier).GetPoints()[CurrentlySelectedPointPosition]
            });
            //
        }
        else {
            InvalidateCurrentPointSelection();
        }

        return true;
    }
    else return false;
}

bool CustPixelWindow::InvalidateCurrentPointSelection()
{
    this->CurrentlySelectedPointPosition = -1;
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    plot->ClearSelectedPoints();
    return true;
}

bool CustPixelWindow::Translate(int Identifier, int x, int y, int PointPosition)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    if (PointPosition == -1) {
        plot->Translate(Identifier, x, y);
    }
    else {
        PlotObject obj = plot->GetObject(Identifier);
        if (obj.GetType() == "BezierCurve") {//Temporary
            if (PointPosition >= plot->GetObject(Identifier - 1).GetPoints().size())PointPosition = plot->GetObject(Identifier - 1).GetPoints().size() - 1;
            Translate(Identifier - 1, x, y, PointPosition);
        }
        else if (obj.GetType() == "BezierControlPolygon") {
            std::vector<double> OldControlPoint = plot->GetObject(Identifier).GetPoints()[PointPosition];
            plot->ChangeControlPoint(Identifier+1, PointPosition
                , std::vector<double> {OldControlPoint[0] + x, OldControlPoint[1] + y});
        }
        else {//default case
            std::vector<double> OldPnt = plot->GetObject(Identifier).GetPoints()[PointPosition];
            plot->ChangePoint(Identifier, PointPosition, std::vector<double> {OldPnt[0] + x, OldPnt[1] + y});
        }
        RefreshPointsSelection();
    }

    plot->Update();
    return true;
}

void CustPixelWindow::OnUpButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * TranslationPercentage };
            double y{ Height * TranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, 0, y, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, 0, y);
            }
        }
    }
}

void CustPixelWindow::OnUpUpButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * LargeTranslationPercentage };
            double y{ Height * LargeTranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, 0, y, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, 0, y);
            }
        }
    }
}

void CustPixelWindow::OnDownButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * TranslationPercentage };
            double y{ Height * TranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, 0, -y, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, 0, -y);
            }
        }
    }
}

void CustPixelWindow::OnDownDownButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * LargeTranslationPercentage };
            double y{ Height * LargeTranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, 0, -y, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, 0, -y);
            }
        }
    }
}

void CustPixelWindow::OnRightButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * TranslationPercentage };
            double y{ Height * TranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, x, 0, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, x, 0);
            }
        }
    }
}

void CustPixelWindow::OnRightRightButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * LargeTranslationPercentage };
            double y{ Height * LargeTranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, x, 0, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, x, 0);
            }
        }
    }
}

void CustPixelWindow::OnLeftButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * TranslationPercentage };
            double y{ Height * TranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, -x, 0, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, -x, 0);
            }
        }
    }
}

void CustPixelWindow::OnLeftLeftButton(wxCommandEvent& event)
{
    if (IsSelectionProcess) {//Checking we are selecting
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        std::vector<double> Box = plot->GetObjMinMaxBox();
        if (Box.size() >= 4) {//Checking Box
            double Width = Box[1] - Box[0];
            double Height = Box[3] - Box[2];
            double x{ Width * LargeTranslationPercentage };
            double y{ Height * LargeTranslationPercentage };
            if (SelectingPointsToggleButton->IsActive()) {//Checking Point Selection
                Translate(CurrentlySelectedIdentifier, -x, 0, CurrentlySelectedPointPosition);
            }
            else {//Object Translation
                Translate(CurrentlySelectedIdentifier, -x, 0);
            }
        }
    }
}

void CustPixelWindow::OnStartControlPolygonCreation(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    CreatingControlPolygon = true;
    ObjectCreationIsFirstPoint = true;
    plot->DoSpecialCursor = true;
}

void CustPixelWindow::OnAitekensInterpolantButton(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    CreatingAitekensInterpolant = true;
    ObjectCreationIsFirstPoint = true;
    plot->DoSpecialCursor = true;
}

void CustPixelWindow::OnElevateDegreeButton(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    plot->ElevateDegree(CurrentlySelectedIdentifier);
}

void CustPixelWindow::OnSubdivideButtonQuarter(wxCommandEvent& event)
{
    this->Subdivide(CurrentlySelectedIdentifier, 0.25);
}

void CustPixelWindow::OnSubdivideButtonHalf(wxCommandEvent& event)
{
    this->Subdivide(CurrentlySelectedIdentifier, 0.5);
}

void CustPixelWindow::OnSubdivideButtonThreeQuarters(wxCommandEvent& event)
{
    this->Subdivide(CurrentlySelectedIdentifier, 0.75);
}

bool CustPixelWindow::FalseAllCreations()
{
    CreatingControlPolygon = false;
    CreatingAitekensInterpolant = false;
    return true;
}

void CustPixelWindow::OnEndObjectCreation(wxCommandEvent& event)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    FalseAllCreations();
    CurrentObjectCreationIdentifier = -1;
    ObjectCreationIsFirstPoint = true;
    plot->DoSpecialCursor = false;
}

void CustPixelWindow::OnDeleteObjectButton(wxCommandEvent& event)
{
    if (IsSelectionProcess&&!SelectingPointsToggleButton->IsActive()) {

        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        PlotObject obj = plot->GetObject(CurrentlySelectedIdentifier);
        if (obj.GetType() == "BezierControlPolygon") {
            plot->RemoveObject(CurrentlySelectedIdentifier+1);
            CurrentlySelectedIdentifierPosition++;
        }
        else plot->RemoveObject(CurrentlySelectedIdentifier);

        if (plot->GetIdentifiers().size() == 0) {//Checking if we are removing the last object
            this->OnSelectionEndButton(event);
        }
        else this->OnSelectionNextButton(event);
        this->OnEndObjectCreation(event);

        
    }
}

void CustPixelWindow::OnDeletePointButton(wxCommandEvent& event)
{
    if (IsSelectionProcess && SelectingPointsToggleButton->IsActive()) {
        Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
        if (plot->GetObject(CurrentlySelectedIdentifier).GetPoints().size() == 1) {
            /*
            * Checking if we are removing the last point of an object
            */
            this->OnSelectingPointsToggleButton(event);
            SelectingPointsToggleButton->Toggle();
            this->OnDeleteObjectButton(event);
            this->OnSelectionNextButton(event);
        }
        else {
            plot->RemovePoint(CurrentlySelectedIdentifier, CurrentlySelectedPointPosition);
            this->OnSelectionNextButton(event);
            this->OnSelectionPreviousButton(event);
        }
        this->OnEndObjectCreation(event);
    }
}

bool CustPixelWindow::Subdivide(int Identifier, double c)
{
    Plot* plot = dynamic_cast<Plot*>(this->GetChildren()[PlotPosition]);
    plot->Subdivide(Identifier, c);
    return true;
}

