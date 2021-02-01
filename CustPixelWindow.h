#pragma once
#include "wx/wx.h"
#include "CustBitmapData.h"
#include "CustPixelButton.h"
#include "CustPixelToggleButton.h"
#include "Plot.h"
class CustPixelWindow : public wxFrame
{
private:
    wxBitmap BackroundBitmap;
    CustPixelButton* ExitButton = nullptr;
    CustPixelButton* MinButton = nullptr;
    CustBitmapData MovePanel;
    wxString backroundcolour = "#9D9D9D";
    wxPanel* movewindowpanel = nullptr;
    //PlotChildrenPosition
    int PlotPosition = 2;
    //For moving panel
    wxPoint LastPos;

    //Selection System
        //Buttons
    CustPixelButton* SelectionStartButton = nullptr;
    CustPixelButton* SelectionEndButton = nullptr;
    CustPixelButton* SelectionNextButton = nullptr;
    CustPixelButton* SelectionPreviousButton = nullptr;
    CustPixelButton* InitiatePointSelectionButton = nullptr;
    CustPixelButton* TerminatePointSelectionButton = nullptr;
    CustPixelToggleButton* SelectingPointsToggleButton = nullptr;
        //Variables
    int CurrentlySelectedIdentifier = -1;
    int CurrentlySelectedIdentifierPosition = -1;
    int CurrentlySelectedPointPosition = -1;
    bool IsSelectionProcess = false;

    //Translation System
    CustPixelButton* UpButton = nullptr;
    CustPixelButton* UpUpButton = nullptr;
    CustPixelButton* DownButton = nullptr;
    CustPixelButton* DownDownButton = nullptr;
    CustPixelButton* RightButton = nullptr;
    CustPixelButton* RightRightButton = nullptr;
    CustPixelButton* LeftButton = nullptr;
    CustPixelButton* LeftLeftButton = nullptr;
    double TranslationPercentage = { 0.02 };
    double LargeTranslationPercentage = { 0.2 };

    //Object/Point Editing System
    int CurrentSelectedPointPosition = -1;

    //Object Creation System
    bool ObjectCreationIsFirstPoint = true;
    int CurrentObjectCreationIdentifier = -1;
    CustPixelButton* EndObjectCreationButton = nullptr;
    /*
    * Update "False All Creations" when adding new type here
    */
        //Deleting
    CustPixelButton* DeleteObjectButton = nullptr;
    CustPixelButton* DeletePointButton = nullptr;
        //ControlPolygon
    bool CreatingControlPolygon = false;
    CustPixelButton* StartControlPolygonCreationButton = nullptr;
        //Aitekens Interpolant
    bool CreatingAitekensInterpolant = false;
    CustPixelButton* StartAitekensInterpolantCreationButton = nullptr;

    //BezierSpecific
    CustPixelButton* ElevateBezierDegreeButton = nullptr;
    CustPixelButton* SubdivideBezierHalfButton = nullptr;
    CustPixelButton* SubdivideBezierQuarterButton = nullptr;
    CustPixelButton* SubdivideBezierThreeQuartersButton = nullptr;

public:
    CustPixelWindow(wxWindow* parent,
        wxWindowID id,
        wxBitmap backroundbitmap,
        CustBitmapData exitbuttonbtminput,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        CustBitmapData minbuttonbtminput = CustBitmapData(),
        long style = wxMINIMIZE_BOX | wxCLIP_CHILDREN,
        const wxString& name = wxFrameNameStr);

    //Asset Initializers
    bool InitializeButtons();
    //

    //GetSet
    wxColour GetBackroundColour();
    //
    void OnPaint(wxPaintEvent& event);

    void OnExit(wxCommandEvent& event);

    void OnMinimize(wxCommandEvent& event);

    void SetMovePanel(CustBitmapData movepanelbtminput);

    //Logic
    bool IsInWindow(wxPoint pnt, CustBitmapData Data);

    //CURSOR HANDLING------------------------------
    void OnMoveWindow(wxMouseEvent& event);

    void OnLeftUp(wxMouseEvent& event);

    void OnLeftDown(wxMouseEvent& event);

    void OnMouseLeave(wxMouseEvent& event);
    //CURSOR HANDLING--------------------------------

    //Selection System
    void OnSelectionStartButton(wxCommandEvent& event);
    void OnSelectionEndButton(wxCommandEvent& event);
    void OnSelectionNextButton(wxCommandEvent& event);
    void OnSelectionPreviousButton(wxCommandEvent& event);
    
        //Point Selection System
    void OnSelectingPointsToggleButton(wxCommandEvent& event);
    bool RefreshPointsSelection();
    bool InvalidateCurrentPointSelection();

    //Translation System
    bool Translate(int Identifier, int x, int y, int PointPosition = -1);
    void OnUpButton(wxCommandEvent & event);
    void OnUpUpButton(wxCommandEvent & event);
    void OnDownButton(wxCommandEvent & event);
    void OnDownDownButton(wxCommandEvent & event);
    void OnRightButton(wxCommandEvent & event);
    void OnRightRightButton(wxCommandEvent & event);
    void OnLeftButton(wxCommandEvent& event);
    void OnLeftLeftButton(wxCommandEvent& event);

    //ObjectCreationSystem
    bool FalseAllCreations();
    void OnEndObjectCreation(wxCommandEvent& event);
        //Delete
    void OnDeleteObjectButton(wxCommandEvent& event);
    void OnDeletePointButton(wxCommandEvent& event);
        //ControlPolygon
    void OnStartControlPolygonCreation(wxCommandEvent& event);
        //AitekensInterpolant
    void OnAitekensInterpolantButton(wxCommandEvent& event);

    //BezierSpecific
    void OnElevateDegreeButton(wxCommandEvent& event);
    void OnSubdivideButtonQuarter(wxCommandEvent& event);
    void OnSubdivideButtonHalf(wxCommandEvent& event);
    void OnSubdivideButtonThreeQuarters(wxCommandEvent& event);
    bool Subdivide(int Identifier, double c);


    wxDECLARE_EVENT_TABLE();
};



