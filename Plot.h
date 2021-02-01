#pragma once
#include "wx/wx.h"
#include "CustBitmapData.h"
#include "Functions.h"
#include "CustPixelWindow.h"
#include "AitekensInterpolant.h"
class PlotObject
{
private:
    //General
    std::vector<std::vector<double>> Points;
    std::vector<std::vector<double>> RefittedPoints;
    bool IsEmpty{ true };
    double Precision{ 1 };//In types that generate points this applies to how many points are generated
    wxString Colour{ "default" };

    //Identifier System
    double Identifier{ -1 };//Set this to desired value, for identification
    std::vector<int> IdentifierTail;//List of other objects identifiers that are dependant on this one
    std::vector<int> ConnectedIdentifierTail;//List of other objects not dependant on this one but connected; Any transformations to the original one happen to these too

    //Type System
    wxString Type{ "default" };
    /*Types:
    1) default   ->
    2) BezierCurve   -> 
    3) BezierControlPolygon ->
    4) AitekensInterpolant ->
    5) AitekensInterpolationPoints ->
      Special Types:
    1) SelectedPoints ->
    */

public:
    //Constructors
    PlotObject();
    PlotObject(std::vector<std::vector<double>> PointsInput, wxString TypeInput = "default", double PrecisionInput = 1);
    PlotObject(const PlotObject& obj);

    //CalculateMinMaxBox
    std::vector<double> GetMinMaxBox();

    //Get/Set
        //Points
    std::vector<std::vector<double>> GetPoints();
    bool SetPoints(std::vector<std::vector<double>> PointsInput);
    bool AddPoint(std::vector<double> PointInput);
    bool ClearPoints();
        //RefittedPoints
    std::vector<std::vector<double>> GetRefittedPoints();
    bool ClearRefittedPoints();
    bool AddRefittedPoint(std::vector<double> point);
    bool SetRefittedPoints(std::vector<std::vector<double>> NewRefittedPoints);
        //Identifier
    double GetIdentifier();
    bool SetIdentifier(double Input);
        //IdentifierTail
    std::vector<int> GetIdentifierTail();
    bool AddToIdentifierTail(int Identifier);
        //ConnectedIdentifiers
    std::vector<int> GetConnectedIdentifierTail();
    bool SetConnectedIdentifierTail(std::vector<int> ConnectedIdentifierTailInput);
    bool AddConnectedIdentifier(int ConnectedIdentifierInput);
        //Type
    wxString GetType();
    bool SetType(wxString TypeInput);
        //Colour
    wxString GetColour();
    bool SetColour(wxString ColourInput);
        //Precision
    double GetPrecision();
    bool SetPrecision(double PrecisionInput);

    //Clear
    void Clear();
};

class Plot : public wxPanel
{
private:
    //General
    wxBitmap Backround;
    std::vector<int> Origin;
    std::vector<PlotObject> ObjectsNow;

    //Rescaling System
    bool RefitOnDraw = true;
    bool UpdateMinMaxBoxOnDraw = true;
    std::vector<double> ObjMinMaxBox;//Xmin,Xman,Ymin,Ymax
    std::vector<double> TransformData;//Xtranslate,Ytranslate,Xscale,Yscale

    //Colours
    wxColour EraserColour = "#f9d381";
    wxColour SelectionColour = "#da0505";
    wxColour PointSelectionColour = "#da0505";
    wxColour SecondaryColour = "#6a6a6a";

    //IdentifierSystem
    int MaxIdentifier{ -1 };
    std::vector<int> Identifiers;
    std::vector<int> SelectedIdentifiers;
    std::vector<int> InvisibleIdentifiers;

    //PlotRefreshing System
    bool ReplaceObjectWantsToRefresh = true;
    bool ChangePointWantsToRefresh = true;
    bool BezierSubdivisionWantsToRefresh = true;

    //SpecialCursor
    wxCursor SpecialCursor;
public:
    //Constructors
    Plot(wxWindow* parent,
        wxBitmap BackroundInput,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER
        );
    //Set Objects Precision
    bool SetObjectPrecision(int Identifier,double Precision);

    //SpecialCursor
    bool DoSpecialCursor = false;
    bool SetSpecialCursor(wxCursor SpecialCursorInput);
    void EnableSpecialCursor(wxMouseEvent& event);
    void DisableSpecialCursor(wxMouseEvent& event);

    //Paint Event Handling
    void OnPaint(wxPaintEvent& event);

    //UserHandling
    void OnLeftDown(wxMouseEvent& event);
    //Refreshing System
    bool ShouldIRefresh();

    //ReScaling System
    bool PseudoRefitObjects(int PixelGap=0);//Only changes Refit Points, not actual points
    bool UpdateMinMaxBox();
    bool CheckFit(PlotObject obj, std::vector<double> MinMaxBox);
    std::vector<double> ConvertCoordinatesToActual(std::vector<double> RefitCoordinates);
    std::vector<double> ConvertCoordinatesToRefit(std::vector<double> ActualCoordinates);
    std::vector<double> GetObjMinMaxBox();

    //Identifier System
        //All Identifiers
    bool RemoveIdentifier(int i);
    bool AddIdentifier(int i);
    std::vector<int> GetIdentifiers();
    int GetMaxIdentifier();
        //Selection Identifiers
    bool SetSelectedIdentifiers(std::vector<int> SelectedIdentifiersInput);
    bool SetSelectedIdentifiers(int SelectedIdentifierInput);
    bool AddSelectedIdentifier(int SelectedIdentifierInput);
    std::vector<int> GetSelectedIdentifiers();
        //Invisible Idetifiers
    bool SetInvisibleIdentifiers(std::vector<int> InvisibleIdentifiersInput);
    bool AddInvisibleIdentifier(int InvisibleIdentifierInput);
    bool RemoveInvisibeIdentifier(int InvisibleIdentifierInput);
    bool ClearInvisibleIdentifiers();
    std::vector<int> GetInvisibleIdentifiers();

    //PlotObject Editing System
    void AddObject(PlotObject Input, int Identifier = -1 ,int Position = -1);
    bool RemoveObject(int Identifier,bool RemoveTail = true);
    void ReplaceObject(int Identifier,PlotObject NewObj);
    PlotObject GetObject(int Identifier);
    int GetObjectPosition(int Identifier);
        //Transformations
    bool Translate(int Identifier, double x, double y);
    bool Scale(int Identifier, double x, double y);

    //Point Editing System
    bool AddPoint(int Identifier, std::vector<double> Point, int Position = -1);
    bool RemovePoint(int Identifier, int Position);
    bool ChangePoint(int Identifier, int Position, std::vector<double> NewPoint);
        
    //Bezier Specific
    bool ChangeControlPoint(int Identifier,int Position, std::vector<double> NewControlPoint);
    bool AddControlPoint(int Identifier, std::vector<double> NewControlPoint, int Position = -1);
    bool TranslateControlPoint(int Identifier, int Position, int x, int y);
    bool Subdivide(int Identifier, double c);
    /*
    * Creates Two New Bezier Curves and Destroys Previous One
    */
    bool ElevateDegree(int Identifier);//Replaces Curve

    //Aiteken Specific
    bool AddInterpolationPoint(int Identifier, std::vector<double> NewInterpolationPoint, int Position = -1);
    bool ChangeInterpolationPoint(int Identifier, int Position, std::vector<double> NewControlPoint);
    bool TranslateInterpolationPoint(int Identifier, int Position, int x, int y);

    //SelectionSystem
    bool Select(int Identifier,bool RemoveOtherSelections = true);
        //PlotObject Point Selection
    bool ClearSelectedPoints();
    bool SetSelectedPoints(std::vector<std::vector<double>> Points);

    wxDECLARE_EVENT_TABLE();
};



