#include "Plot.h"
#include "CustBitmapData.h"
#include "BezierCurve.h"
wxBEGIN_EVENT_TABLE(Plot, wxPanel)
EVT_ENTER_WINDOW(EnableSpecialCursor)
EVT_LEAVE_WINDOW(DisableSpecialCursor)
EVT_LEFT_DOWN(OnLeftDown)
wxEND_EVENT_TABLE();

Plot::Plot(wxWindow* parent,
    wxBitmap BackroundInput,
    wxWindowID winid,
    const wxPoint& pos,
    long style)
    :wxPanel(parent,winid,pos,BackroundInput.GetSize(),style,"")
{
    Backround = BackroundInput;
    Origin.push_back(0);
    Origin.push_back(BackroundInput.GetSize().y);
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(Plot::OnPaint));

    //Adding PointSelection Object
    PlotObject SelectedPoints(std::vector<std::vector<double>> {std::vector<double>{0, 0}}, "SelectedPoints");
    this->AddInvisibleIdentifier(-2);
    this->AddObject(SelectedPoints);

}

bool Plot::SetObjectPrecision(int Identifier,double Precision)
{
    if (IsIn(Identifier, this->Identifiers)) {
        this->ObjectsNow[this->GetObjectPosition(Identifier)].SetPrecision(Precision);
    }
    return false;
}

bool Plot::SetSpecialCursor(wxCursor SpecialCursorInput)
{
    SpecialCursor = SpecialCursorInput;
    return true;
}

void Plot::EnableSpecialCursor(wxMouseEvent& event)
{
    if (DoSpecialCursor) {
        this->SetCursor(SpecialCursor);
    }
}

void Plot::DisableSpecialCursor(wxMouseEvent& event)
{
    this->SetCursor(wxCursor());
}

void Plot::OnPaint(wxPaintEvent& event) {
    //UpdatingRefit
    if (RefitOnDraw) {
        this->PseudoRefitObjects();
    }

    //Drawing Backround
    wxPaintDC dc(this);
    dc.DrawBitmap(Backround, wxPoint(0, 0));

    //Drawing objects
    std::vector<std::vector<double>> Points;
    int CurrentIdentifier;
    for (PlotObject obj : this->ObjectsNow) {
        CurrentIdentifier = obj.GetIdentifier();
        dc.SetPen(*wxBLACK_PEN);
        //InvisibleObjects
        if (IsIn(CurrentIdentifier, this->InvisibleIdentifiers)) {
            continue;
        }

        //ColourChanging
        if (IsIn(CurrentIdentifier, this->SelectedIdentifiers)) {
            dc.SetPen(wxPen(this->SelectionColour,2));
        }
        else if (obj.GetColour() != "default") {
            dc.SetPen(wxPen(obj.GetColour()));
        }

        //DrawingPoint
        if (obj.GetType() == "Point" || obj.GetType() == "AitekensInterpolationPoints") {
            Points = obj.GetRefittedPoints();
            for (int i{ 0 };i < Points.size();i++) {
                dc.DrawCircle(wxPoint(Points[i][0], Origin[1] - Points[i][1]),2);
            }
            Points.clear();
        }
        else if (obj.GetType() == "SelectedPoints") {
            dc.SetPen(wxPen(this->PointSelectionColour, 4));
            for (auto i : obj.GetRefittedPoints()) {
                dc.DrawCircle(wxPoint(i[0], Origin[1] - i[1]), 2);
            }
        }
        else {
            Points = obj.GetRefittedPoints();
            for (int i{ 0 };i < Points.size();i++) {
                if (i == 0) {
                    dc.DrawPoint(wxPoint(Points[i][0], Origin[1] - Points[i][1]));
                }
                else {
                    dc.DrawLine(wxPoint(Points[i - 1][0], Origin[1] - Points[i - 1][1]), wxPoint(Points[i][0], Origin[1] - Points[i][1]));
                }
            }
            Points.clear();
            
        }
    }
}

void Plot::OnLeftDown(wxMouseEvent& event)
{
    CustPixelWindow* window = dynamic_cast<CustPixelWindow*>(this->GetParent());
    wxPoint PositionOnPlot(event.GetPosition());
    event.SetPosition(wxPoint(PositionOnPlot.x + this->GetPosition().x, PositionOnPlot.y + this->GetPosition().y));
    window->OnLeftDown(event);
}

bool Plot::ShouldIRefresh()
{
    if (this->BezierSubdivisionWantsToRefresh) {
        if (this->ChangePointWantsToRefresh) {
            if (this->ReplaceObjectWantsToRefresh) {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
    return true;
}

bool Plot::PseudoRefitObjects(int PixelGap)
{
    if (this->UpdateMinMaxBoxOnDraw) {
        this->UpdateMinMaxBox();
    }

    if (this->ObjMinMaxBox.size() == 0) {//Checking zero points case
        return false;
    }

    std::vector<double> TargetBox{ double(PixelGap),double(this->Backround.GetWidth() - PixelGap),double(PixelGap),double(this->Backround.GetHeight() - PixelGap) };
    std::vector<double> OriginBox = this->ObjMinMaxBox;
    //this->TransformData is created
    double TBW{ TargetBox[1] - TargetBox[0] };
    double TBH{ TargetBox[3] - TargetBox[2] };
    double OBW{ OriginBox[1] - OriginBox[0] };
    double OBH{ OriginBox[3] - OriginBox[2] };
    if (OBW == 0 || OBH == 0) {
        return true;
    }
    double Xscale{ TBW / OBW };
    double Yscale{ TBH / OBH };
    if (Yscale > Xscale)Yscale = Xscale;
    else if (Yscale < Xscale)Xscale = Yscale;
    double Xtranslate{ TargetBox[0] - OriginBox[0] * Xscale };
    double Ytranslate{ TargetBox[2] - OriginBox[2] * Yscale };
    this->TransformData.clear();
    this->TransformData.push_back(Xtranslate);
    this->TransformData.push_back(Ytranslate);
    this->TransformData.push_back(Xscale);
    this->TransformData.push_back(Yscale);

    std::vector<double> Data = this->TransformData;
    matrix AffineMatrix(std::vector<std::vector<double>> { {Data[2], 0, 0}, { 0,Data[3],0 }, { 0,0,1 } });
    matrix TranslationVec(std::vector<std::vector<double>> { {Data[0], Data[1], 0}}, false);
    matrix FinalPoint;


    std::vector<std::vector<double>> NewRefittedPoints;
    //[loop]All Objects Iterated
    for (PlotObject obj : this->ObjectsNow) {
        //Checking Empty Object
        if (obj.GetPoints().size() == 0)continue;
        //RefitPoints Cleared
        obj.ClearRefittedPoints();
        //[loop]All Object Points Iterated
        for (auto pnt : obj.GetPoints()) {
            //From Each Point A New Point Is Created
            matrix Point(std::vector<std::vector<double>> { {pnt[0], pnt[1], 0}}, false);
            FinalPoint = AffineMatrix * Point + TranslationVec;
            //New Point Saved On RefitPoints
            NewRefittedPoints.push_back(std::vector<double> {FinalPoint.GetValue(1, 1), FinalPoint.GetValue(2, 1)});
        }
        //[1]Changing RefittedPoints of object Start
        this->ObjectsNow[this->GetObjectPosition(obj.GetIdentifier())].SetRefittedPoints(NewRefittedPoints);
        //[1]End
        NewRefittedPoints.clear();
    }

    this->RefitOnDraw = false;
    this->UpdateMinMaxBoxOnDraw = false;

    return true;
}

bool Plot::UpdateMinMaxBox()
{
    bool IsFirstIteration = true;
    std::vector<double> Box;
    std::vector<double> objBox;
    for (auto obj : this->ObjectsNow) {
        //Invisible objects are not taken into account
        if (IsIn(obj.GetIdentifier(), this->InvisibleIdentifiers)) {
            continue;
        }
        //
        if (IsFirstIteration){
            Box = obj.GetMinMaxBox();
            IsFirstIteration = false;
            continue;
        }
        objBox = obj.GetMinMaxBox();
        if (objBox.size() != 0 ) {
            if (Box.size() == 0) {
                Box = objBox;
            }
            else {
                if (objBox[0] < Box[0])Box[0] = objBox[0];
                if (objBox[1] > Box[1])Box[1] = objBox[1];
                if (objBox[2] < Box[2])Box[2] = objBox[2];
                if (objBox[3] > Box[3])Box[3] = objBox[3];
            }
        }
        objBox.clear();
    }
    this->ObjMinMaxBox.clear();
    this->ObjMinMaxBox = Box;
    return true;
}

bool Plot::CheckFit(PlotObject obj, std::vector<double> MinMaxBox)
{
    std::vector<double> objBox = obj.GetMinMaxBox();
    if (objBox[0] < MinMaxBox[0] || objBox[1] > MinMaxBox[1] || objBox[2] < MinMaxBox[2] || objBox[3] > MinMaxBox[3])return false;
    return true;
}

std::vector<double> Plot::ConvertCoordinatesToActual(std::vector<double> RefitCoordinates)
{
    std::vector<double> OriginBox{ 0,double(this->Backround.GetWidth()),0,double(this->Backround.GetHeight()) };
    std::vector<double> TargetObjBox = this->ObjMinMaxBox;
    if (TargetObjBox.size() < 4) {
        return RefitCoordinates;
    }
    std::vector<double> OriginObjBoxBottomLeftCorner{ this->ConvertCoordinatesToRefit(
        std::vector<double> {TargetObjBox[0],TargetObjBox[2]}
    ) };
    std::vector<double> OriginObjBoxTopRightCorner{ this->ConvertCoordinatesToRefit(
    std::vector<double> {TargetObjBox[1],TargetObjBox[3]}
    ) };
    double TargetBoxWidth = (OriginBox[1] - OriginBox[0]) * (TargetObjBox[1] - TargetObjBox[0]) / (
        -OriginObjBoxBottomLeftCorner[0] + OriginObjBoxTopRightCorner[0]
        );
    double TargetBoxHeight = (OriginBox[3] - OriginBox[2]) * (TargetObjBox[3] - TargetObjBox[2]) / (
        -OriginObjBoxBottomLeftCorner[1] + OriginObjBoxTopRightCorner[1]
        );
    std::vector<double> TargetBox;
    double Tolerance = 0.01*(TargetBoxHeight+TargetBoxWidth)/2;
    if (IsNear(TargetBoxWidth, TargetObjBox[1] - TargetObjBox[0],Tolerance)){
        TargetBox.push_back(TargetObjBox[0]);
        TargetBox.push_back(TargetObjBox[1]);
        if (IsNear(OriginObjBoxBottomLeftCorner[1],OriginBox[2],Tolerance)) {
            TargetBox.push_back(TargetObjBox[2]);
            TargetBox.push_back(TargetObjBox[2] + TargetBoxHeight);
        }
        else {
            TargetBox.push_back(TargetObjBox[3] - TargetBoxHeight);
            TargetBox.push_back(TargetObjBox[3]);

        }
    }
    else {
        if (IsNear(OriginObjBoxBottomLeftCorner[0],OriginBox[0],Tolerance)) {
            TargetBox.push_back(TargetObjBox[0]);
            TargetBox.push_back(TargetObjBox[0] + TargetBoxWidth );
        }
        else {
            TargetBox.push_back(TargetObjBox[1] - TargetBoxWidth);
            TargetBox.push_back(TargetObjBox[1]);
        }
        TargetBox.push_back(TargetObjBox[2]);
        TargetBox.push_back(TargetObjBox[3]);
    }
    if (TargetBox.size() == 0)return RefitCoordinates;
    double TBW{ TargetBox[1] - TargetBox[0] };
    double TBH{ TargetBox[3] - TargetBox[2] };
    double OBW{ OriginBox[1] - OriginBox[0] };
    double OBH{ OriginBox[3] - OriginBox[2] };
    if (TBW == 0 || TBH == 0) {
        return RefitCoordinates;
    }
    double Xscale{ TBW / OBW };
    double Yscale{ TBH / OBH };
    if (Yscale > Xscale)Yscale = Xscale;
    else if (Yscale < Xscale)Xscale = Yscale;
    double Xtranslate{ TargetBox[0] - OriginBox[0] * Xscale };
    double Ytranslate{ TargetBox[2] - OriginBox[2] * Yscale };

    return std::vector<double> {RefitCoordinates[0] * Xscale + Xtranslate, RefitCoordinates[1] * Yscale + Ytranslate};

}

std::vector<double> Plot::ConvertCoordinatesToRefit(std::vector<double> ActualCoordinates)
{
    std::vector<double> TargetBox{ double(2),double(this->Backround.GetWidth() - 2),double(2),double(this->Backround.GetHeight() - 2) };
    std::vector<double> OriginBox = this->ObjMinMaxBox;
    double TBW{ TargetBox[1] - TargetBox[0] };
    double TBH{ TargetBox[3] - TargetBox[2] };
    double OBW{ OriginBox[1] - OriginBox[0] };
    double OBH{ OriginBox[3] - OriginBox[2] };
    if (OBW == 0 || OBH == 0) {
        return ActualCoordinates;
    }
    double Xscale{ TBW / OBW };
    double Yscale{ TBH / OBH };
    if (Yscale > Xscale)Yscale = Xscale;
    else if (Yscale < Xscale)Xscale = Yscale;
    double Xtranslate{ TargetBox[0] - OriginBox[0] * Xscale };
    double Ytranslate{ TargetBox[2] - OriginBox[2] * Yscale };
    return std::vector<double> {ActualCoordinates[0] * Xscale + Xtranslate, ActualCoordinates[1] * Yscale + Ytranslate};
}

std::vector<double> Plot::GetObjMinMaxBox()
{
    return this->ObjMinMaxBox;
}

bool Plot::RemoveIdentifier(int i)
{
    if (IsIn(i, this->Identifiers)) {
        std::vector<int> NewIdentifiers;
        for (int j : this->Identifiers) {
            if (j == i)continue;
            else NewIdentifiers.push_back(j);
        }
        this->Identifiers.clear();
        this->Identifiers = NewIdentifiers;
    }
    else return false;
}

bool Plot::AddIdentifier(int i)
{
    if (IsIn(i, this->Identifiers)) return false;
    else this->Identifiers.push_back(i);

    return true;
}

std::vector<int> Plot::GetIdentifiers()
{
    return this->Identifiers;
}

int Plot::GetMaxIdentifier()
{
    return this->MaxIdentifier;
}

bool Plot::SetSelectedIdentifiers(std::vector<int> SelectedIdentifiersInput)
{
    this->SelectedIdentifiers.clear();
    this->SelectedIdentifiers = SelectedIdentifiersInput;
    return true;
}

bool Plot::SetSelectedIdentifiers(int SelectedIdentifierInput)
{
    this->SelectedIdentifiers.clear();
    this->SelectedIdentifiers.push_back(SelectedIdentifierInput);

    this->Update();

    return true;
}

bool Plot::AddSelectedIdentifier(int SelectedIdentifierInput)
{
    this->SelectedIdentifiers.push_back(SelectedIdentifierInput);
    return true;
}

std::vector<int> Plot::GetSelectedIdentifiers()
{
    return this->SelectedIdentifiers;
}

bool Plot::SetInvisibleIdentifiers(std::vector<int> InvisibleIdentifiersInput)
{
    this->InvisibleIdentifiers.clear();
    this->InvisibleIdentifiers = InvisibleIdentifiersInput;
    return true;
}

bool Plot::AddInvisibleIdentifier(int InvisibleIdentifierInput)
{
    if (IsIn(InvisibleIdentifierInput, this->GetInvisibleIdentifiers())) {
        return false;
    }
    else {
        this->InvisibleIdentifiers.push_back(InvisibleIdentifierInput);
        return true;
    }
}

bool Plot::RemoveInvisibeIdentifier(int InvisibleIdentifierInput)
{
    if (!IsIn(InvisibleIdentifierInput, this->InvisibleIdentifiers)) {
        return false;
    }
    else {
        std::vector<int> NewInvisibleIdentifiers;
        for (int i : this->InvisibleIdentifiers) {
            if (i == InvisibleIdentifierInput)continue;
            else NewInvisibleIdentifiers.push_back(i);
        }
        this->InvisibleIdentifiers.clear();
        this->InvisibleIdentifiers = NewInvisibleIdentifiers;
        return true;
    }
}

bool Plot::ClearInvisibleIdentifiers()
{
    this->InvisibleIdentifiers.clear();
    return true;
}

std::vector<int> Plot::GetInvisibleIdentifiers()
{
    return this->InvisibleIdentifiers;
}

void Plot::AddObject(PlotObject Input, int Identifier, int Position)
{
    //[1]Updating Refit
    this->RefitOnDraw = true;
    this->UpdateMinMaxBoxOnDraw = true;
    //End[1]

    if (Input.GetType() == "BezierCurve") {
        //[2]Initializing Curve
        PlotObject Curve;
        Curve.SetType("BezierCurve");
        Curve.SetPrecision(Input.GetPrecision());
        BezierCurve ActualCurve(Input.GetPoints());
        ActualCurve.InitializeMatrixForm();
        double Step{ (ActualCurve.GetParameterRange()[1] - ActualCurve.GetParameterRange()[0]) / (Input.GetPrecision() + 1) };
        for (double t{ ActualCurve.GetParameterRange()[0] };t < ActualCurve.GetParameterRange()[1];t += Step) {
            Curve.AddPoint(ActualCurve.compute(t));
        }
        Curve.AddPoint(ActualCurve.compute(ActualCurve.GetParameterRange()[1]));

       
        PlotObject ControlPolygon(Input.GetPoints());
        ControlPolygon.SetColour(wxToString(this->SecondaryColour));
        ControlPolygon.SetType("BezierControlPolygon");
        //End[2]

        //[3]Setting Identifier
        if (Identifier != -1 && !IsIn(Identifier, this->GetIdentifiers())) {
            //ControlPolygonIdentifier
            ControlPolygon.SetIdentifier(Identifier - 1);
            this->AddIdentifier(Identifier - 1);
            Curve.AddToIdentifierTail(Identifier - 1);
            //

            //CurveIdentifier
            Curve.SetIdentifier(Identifier);
            this->AddIdentifier(Identifier);
            if (Identifier > this->MaxIdentifier)this->MaxIdentifier = Identifier;
            //
        }
        else if (Input.GetIdentifier() != -1 && !IsIn(Input.GetIdentifier(), this->GetIdentifiers())) {
            //ControlPolygonIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            ControlPolygon.SetIdentifier(MaxIdentifier);
            Curve.AddToIdentifierTail(MaxIdentifier);
            //

            //CurveIdentifier
            Curve.SetIdentifier(Input.GetIdentifier());
            this->AddIdentifier(Input.GetIdentifier());
            if (Input.GetIdentifier() > this->MaxIdentifier)this->MaxIdentifier = Input.GetIdentifier();
            //
        }
        else {
            //ControlPolygonIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            ControlPolygon.SetIdentifier(MaxIdentifier);
            Curve.AddToIdentifierTail(MaxIdentifier);
            //

            //CurveIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            Curve.SetIdentifier(this->MaxIdentifier);
            //
        }
        //End[3]

        //[4]Inserting into ObjectsNow
        if (Position == -1 || Position >= this->ObjectsNow.size()) {
            this->ObjectsNow.push_back(ControlPolygon);
            this->ObjectsNow.push_back(Curve);
        }
        else {
            std::vector<PlotObject> NewObjects;
            std::vector<PlotObject> OldObjects = this->ObjectsNow;
            this->ObjectsNow.clear();
            for (int i{ 0 };i < OldObjects.size();i++) {
                if (i == Position) {
                    OldObjects.push_back(ControlPolygon);
                    OldObjects.push_back(Curve);
                    NewObjects.push_back(OldObjects[i]);
                }
                else NewObjects.push_back(OldObjects[i]);
            }
            this->ObjectsNow = NewObjects;
        }
        //End[4]
        
    }
    else if (Input.GetType() == "AitekensInterpolant") {
        //[1]Initializing Curve
        PlotObject Curve;
        Curve.SetType("AitekensInterpolant");
        Curve.SetPrecision(Input.GetPrecision());
        if (Input.GetPoints().size() == 1) {
            Curve.AddPoint(Input.GetPoints()[0]);
        }
        else {
            AitekensInterpolant Interpolant(Input.GetPoints());
            double Step = (Interpolant.GetParameterRange()[1] - Interpolant.GetParameterRange()[0]) / (Input.GetPrecision() + 1);
            for (double i{ Interpolant.GetParameterRange()[0] };i <= Interpolant.GetParameterRange()[1];i = i + Step) {
                Curve.AddPoint(Interpolant.compute(i));
            }
            Curve.AddPoint(Interpolant.compute(Interpolant.GetParameterRange()[1]));
        }
        //End[1]

        //[2]Creating InterpolationPointsObject
        PlotObject InterpolationPoints(Input.GetPoints(),"AitekensInterpolationPoints");
        InterpolationPoints.SetColour(wxToString(SecondaryColour));
        //End[2]

        //[3]Setting Identifier
        if (Identifier != -1 && !IsIn(Identifier, this->GetIdentifiers())) {
            //ControlPolygonIdentifier
            InterpolationPoints.SetIdentifier(Identifier - 1);
            this->AddIdentifier(Identifier - 1);
            Curve.AddToIdentifierTail(Identifier - 1);
            //

            //CurveIdentifier
            Curve.SetIdentifier(Identifier);
            this->AddIdentifier(Identifier);
            if (Identifier > this->MaxIdentifier)this->MaxIdentifier = Identifier;
            //
        }
        else if (Input.GetIdentifier() != -1 && !IsIn(Input.GetIdentifier(), this->GetIdentifiers())) {
            //ControlPolygonIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            InterpolationPoints.SetIdentifier(MaxIdentifier);
            Curve.AddToIdentifierTail(MaxIdentifier);
            //

            //CurveIdentifier
            Curve.SetIdentifier(Input.GetIdentifier());
            this->AddIdentifier(Input.GetIdentifier());
            if (Input.GetIdentifier() > this->MaxIdentifier)this->MaxIdentifier = Input.GetIdentifier();
            //
        }
        else {
            //ControlPolygonIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            InterpolationPoints.SetIdentifier(MaxIdentifier);
            Curve.AddToIdentifierTail(MaxIdentifier);
            //

            //CurveIdentifier
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            Curve.SetIdentifier(this->MaxIdentifier);
            //
        }
        //End[3]

                //[4]Inserting into ObjectsNow
        if (Position == -1 || Position >= this->ObjectsNow.size()) {
            this->ObjectsNow.push_back(InterpolationPoints);
            this->ObjectsNow.push_back(Curve);
        }
        else {
            std::vector<PlotObject> NewObjects;
            std::vector<PlotObject> OldObjects = this->ObjectsNow;
            this->ObjectsNow.clear();
            for (int i{ 0 };i < OldObjects.size();i++) {
                if (i == Position) {
                    OldObjects.push_back(InterpolationPoints);
                    OldObjects.push_back(Curve);
                    NewObjects.push_back(OldObjects[i]);
                }
                else NewObjects.push_back(OldObjects[i]);
            }
            this->ObjectsNow = NewObjects;
        }
        //End[4]

    }
    else if (Input.GetType() == "SelectedPoints") {
        //[1]Setting Identifier
        Input.SetIdentifier(-2);
        //End[1]

        //[2]Adding to objects Now
        this->ObjectsNow.push_back(Input);
        //End[2]

    }
    else{//default case
        //[2]Setting Identifier
        if (Identifier != -1 && !IsIn(Identifier, this->GetIdentifiers())) {
            Input.SetIdentifier(Identifier);
            this->AddIdentifier(Identifier);
            if (Identifier > this->MaxIdentifier)this->MaxIdentifier = Identifier;
        }
        else if (Input.GetIdentifier() != -1 && !IsIn(Input.GetIdentifier(), this->GetIdentifiers())) {
            this->AddIdentifier(Input.GetIdentifier());
            if (Input.GetIdentifier() > this->MaxIdentifier)this->MaxIdentifier = Input.GetIdentifier();
        }
        else {
            this->MaxIdentifier++;
            this->AddIdentifier(MaxIdentifier);
            Input.SetIdentifier(this->MaxIdentifier);
        }
        //End[2]

        //[3]Inserting into ObjectsNow
        if (Position == -1 || Position >= this->ObjectsNow.size())this->ObjectsNow.push_back(Input);
        else {
            std::vector<PlotObject> NewObjects;
            std::vector<PlotObject> OldObjects = this->ObjectsNow;
            this->ObjectsNow.clear();
            for (int i{ 0 };i < OldObjects.size();i++) {
                if (i == Position) {
                    NewObjects.push_back(Input);
                    NewObjects.push_back(OldObjects[i]);
                }
                else NewObjects.push_back(OldObjects[i]);
            }
            this->ObjectsNow = NewObjects;
        }
        //End[3]       
    }



    //[*]Updating Plot
    if (this->ShouldIRefresh()) {
        this->Refresh();
        this->Update();
    }
    //End[*]

}

bool Plot::RemoveObject(int Identifier,bool RemoveTail)
{
    //[1]Checking if Identifier Exists
    if (!IsIn(Identifier, this->Identifiers))return false;
    //End[1]

    //[2]Updating Refit
    this->RefitOnDraw = true;
    this->UpdateMinMaxBoxOnDraw = true;
    //End[2]

    //[3]RemovingObject from ObjectsNow
    std::vector<PlotObject> Temp;
    PlotObject Culprit;
    for (PlotObject i : this->ObjectsNow) {
        if (i.GetIdentifier() == Identifier) {
            Culprit = i;
        }
        else Temp.push_back(i);
    }
    this->ObjectsNow.clear();
    this->ObjectsNow = Temp;
    //End[3]

    //[4]Removing Identifier from Identifier List
    std::vector<int> IdentifierList = this->Identifiers;
    this->Identifiers.clear();
    for (int i : IdentifierList) {
        if (i == Culprit.GetIdentifier())continue;
        else this->AddIdentifier(i);
    }
    //End[4]

    //[5]Removing Tail from ObjectsNow
    if (RemoveTail) {
        for (int i : Culprit.GetIdentifierTail()) {
            this->RemoveObject(i);
        }
    }
    //End[5]

    //[6]Updating Plot
    if (this->ShouldIRefresh()) {
        this->Refresh();
        this->Update();
    }
    //[6]

    return true;
}

void Plot::ReplaceObject(int Identifier, PlotObject NewObj)
{
    this->ReplaceObjectWantsToRefresh = false;

    //[1]Setting Previous Object Static Settings To New Object
    PlotObject OldObj = this->GetObject(Identifier);
    NewObj.SetConnectedIdentifierTail(OldObj.GetConnectedIdentifierTail());
    //End[1]

    int OldPosition = this->GetObjectPosition(Identifier);

    //[2]Removing OldObject
    this->RemoveObject(Identifier);
    //End[2]

    this->ReplaceObjectWantsToRefresh = true;

    //[3]Adding New Object
    this->AddObject(NewObj, Identifier, OldPosition);
    //End[3]
    

}

PlotObject Plot::GetObject(int Identifier)
{
    if (IsIn(Identifier, this->Identifiers)) {
        for (auto obj : this->ObjectsNow) {
            if (obj.GetIdentifier() == Identifier)return obj;
        }
    }
    else {
        return PlotObject() ;
        //*
        //Error Handling
        //*
    }
}

int Plot::GetObjectPosition(int Identifier)
{
    int Position{ -1 };
    for (auto i : this->ObjectsNow) {
        Position++;
        if (i.GetIdentifier() == Identifier)break;
    }
    return Position;
}

bool Plot::AddPoint(int Identifier, std::vector<double> Point, int Position)
{
    PlotObject OldObj = this->GetObject(Identifier);

    //[1]Creates New Points
    std::vector<std::vector<double>> OldPoints = OldObj.GetPoints();
    std::vector<std::vector<double>> NewPoints;
    if (Position <= -1 || Position >= OldPoints.size()) {
        OldPoints.push_back(Point);
        NewPoints = OldPoints;
    }
    else {
        for (int i{ 0 };i < OldPoints.size();i++) {
            if (i == Position) {
                NewPoints.push_back(Point);
                NewPoints.push_back(OldPoints[i]);
            }
            else NewPoints.push_back(OldPoints[i]);
        }
    }
    //End[1]

    //[2]Creates New Object
    OldObj.SetPoints(NewPoints);
    //End[2]

    //[3]Replaces Old Object
    this->ReplaceObject(Identifier, OldObj);
    //End[3]

    return true;
}

bool Plot::RemovePoint(int Identifier, int Position)
{
    PlotObject OldObj = this->GetObject(Identifier);

    //[1]Creates New Points
    std::vector<std::vector<double>> OldPoints = OldObj.GetPoints();
    std::vector<std::vector<double>> NewPoints;
    if (Position <= -1) {
        Position = 0;
    }
    else if (Position >= OldPoints.size()) {
        Position = OldPoints.size() - 1;
    }
    
    for (int i{ 0 };i < OldPoints.size();i++) {
        if (i == Position) {
            continue;
        }
        else NewPoints.push_back(OldPoints[i]);
    }
    //End[1]

    //[2]Creates New Object
    if (OldObj.GetType() == "BezierControlPolygon") {
        OldObj.SetType("BezierCurve");
        OldObj.SetPoints(NewPoints);
        OldObj.SetPrecision(this->GetObject(OldObj.GetIdentifier() + 1).GetPrecision());
        Identifier++;
    }
    else {
        OldObj.SetPoints(NewPoints);
    }

    //End[2]

    //[3]Replaces Old Object
    this->ReplaceObject(Identifier, OldObj);
    //End[3]
    
    return true;
}

bool Plot::ChangePoint(int Identifier, int PointPosition, std::vector<double> NewPoint)
{
    this->ChangePointWantsToRefresh = false;

    this->RemovePoint(Identifier, PointPosition);

    this->ChangePointWantsToRefresh = true;

    this->AddPoint(Identifier, NewPoint, PointPosition);

    return true;
}

bool Plot::ChangeControlPoint(int Identifier,int Position , std::vector<double> NewControlPoint)
{
    PlotObject OldBezier = this->GetObject(Identifier);
    if (OldBezier.GetType() == "BezierCurve") {
        std::vector<std::vector<double>> OldControlPolygon = this->GetObject(OldBezier.GetIdentifierTail()[0]).GetPoints();
        std::vector<std::vector<double>> NewControlPolygon;
        if (Position < 0) {
            Position = 0;
        }
        else if (Position >= OldControlPolygon.size()) {
            Position = OldControlPolygon.size() - 1;
        }

        for (int i{ 0 };i < OldControlPolygon.size();i++) {
            if (Position == i) {
                NewControlPolygon.push_back(NewControlPoint);
            }
            else NewControlPolygon.push_back(OldControlPolygon[i]);
        }

        PlotObject NewBezier(NewControlPolygon, "BezierCurve", OldBezier.GetPrecision());
        this->ReplaceObject(Identifier, NewBezier);
    }
    else {
        this->ChangePoint(Identifier, Position, NewControlPoint);
        return false;
    }

    return true;
}

bool Plot::AddControlPoint(int Identifier,std::vector<double> NewControlPoint, int Position)
{
    PlotObject OldBezier = this->GetObject(Identifier);
    if (OldBezier.GetType() == "BezierCurve") {
        std::vector<std::vector<double>> OldControlPolygon = this->GetObject(OldBezier.GetIdentifierTail()[0]).GetPoints();
        std::vector<std::vector<double>> NewControlPolygon;
        for (int i{ 0 };i < OldControlPolygon.size();i++) {
            if (Position == i) {
                NewControlPolygon.push_back(NewControlPoint);
                NewControlPolygon.push_back(OldControlPolygon[i]);
            }
            else NewControlPolygon.push_back(OldControlPolygon[i]);
        }
        if (Position >= OldControlPolygon.size() || Position < 0) {
            NewControlPolygon.push_back(NewControlPoint);
        }

        PlotObject NewBezier(NewControlPolygon, "BezierCurve", OldBezier.GetPrecision());
        this->ReplaceObject(Identifier, NewBezier);
        return true;
    }
    return false;
}

bool Plot::TranslateControlPoint(int Identifier, int Position, int x, int y)
{
    std::vector<std::vector<double>> OldControlPolygon = this->GetObject(this->GetObject(Identifier).GetIdentifierTail()[0]).GetPoints();
    this->ChangeControlPoint(Identifier, Position, std::vector<double> {OldControlPolygon[Position][0] + x, OldControlPolygon[Position][1] + y });
    return true;
}

bool Plot::Subdivide(int Identifier, double c)
{
    if (GetObject(Identifier).GetType() == "BezierCurve") {
        BezierCurve ActualCurve(GetObject(Identifier - 1).GetPoints());
        std::vector<std::vector<double>> NewControlPoints = ActualCurve.Subdivide(c);
        int n = GetObject(Identifier - 1).GetPoints().size();

        std::vector<std::vector<double>> FirstControlPoints;
        std::vector<std::vector<double>> SecondControlPoints;
        for (int i{ 0 };i < n;i++) {
            FirstControlPoints.push_back(NewControlPoints[i]);
        }
        for (int i{ 0 };i < n;i++) {
            SecondControlPoints.push_back(NewControlPoints[n+i]);
        }
        PlotObject FirstSubdivision(FirstControlPoints, "BezierCurve", GetObject(Identifier).GetPrecision());
        PlotObject SecondSubdivision(SecondControlPoints, "BezierCurve", GetObject(Identifier).GetPrecision());
        this->BezierSubdivisionWantsToRefresh = false;
        this->RemoveObject(Identifier);
        this->AddObject(FirstSubdivision);
        this->BezierSubdivisionWantsToRefresh = true;
        this->AddObject(SecondSubdivision);
    }
    return false;
}

bool Plot::ElevateDegree(int Identifier)
{
    if (GetObject(Identifier).GetType() == "BezierCurve") {
        BezierCurve Curve(this->GetObject(Identifier - 1).GetPoints());
        PlotObject NewCurve(Curve.ElevateDegree(), "BezierCurve", this->GetObject(Identifier).GetPrecision());
        ReplaceObject(Identifier, NewCurve);
    }
    else if (GetObject(Identifier).GetType() == "BezierControlPolygon") {
        ElevateDegree(Identifier + 1);
    }
    else {
        return false;
    }
    return true;
}

bool Plot::AddInterpolationPoint(int Identifier, std::vector<double> NewInterpolationPoint, int Position)
{
    PlotObject obj = this->GetObject(Identifier);
    if (obj.GetType() == "AitekensInterpolant") {

        std::vector<std::vector<double>> NewInterPolationPoints;
        std::vector<std::vector<double>> OldInterPolationPoints = this->GetObject(obj.GetIdentifierTail()[0]).GetPoints();
        for (int i{ 0 };i < OldInterPolationPoints.size();i++) {
            if (i == Position) {
                NewInterPolationPoints.push_back(NewInterpolationPoint);
                NewInterPolationPoints.push_back(OldInterPolationPoints[i]);
            }
            else NewInterPolationPoints.push_back(OldInterPolationPoints[i]);
        }
        if (Position < 0 || Position >= OldInterPolationPoints.size()) {
            NewInterPolationPoints.push_back(NewInterpolationPoint);
        }

        PlotObject NewObj(NewInterPolationPoints, obj.GetType(), obj.GetPrecision());
        this->ReplaceObject(Identifier, NewObj);

        return true;
    }
    return false;
}

bool Plot::ChangeInterpolationPoint(int Identifier, int Position, std::vector<double> NewControlPoint)
{
    PlotObject OldInterpolant = this->GetObject(Identifier);
    if (OldInterpolant.GetType() == "AitekensInterpolant") {
        std::vector<std::vector<double>> OldControlPolygon = this->GetObject(OldInterpolant.GetIdentifierTail()[0]).GetPoints();
        std::vector<std::vector<double>> NewControlPolygon;
        if (Position < 0) {
            Position = 0;
        }
        else if (Position >= OldControlPolygon.size()) {
            Position = OldControlPolygon.size() - 1;
        }

        for (int i{ 0 };i < OldControlPolygon.size();i++) {
            if (Position == i) {
                NewControlPolygon.push_back(NewControlPoint);
            }
            else NewControlPolygon.push_back(OldControlPolygon[i]);
        }

        PlotObject NewBezier(NewControlPolygon, "AitekensInterpolant", OldInterpolant.GetPrecision());
        this->ReplaceObject(Identifier, NewBezier);
    }
    else {
        this->ChangePoint(Identifier, Position, NewControlPoint);
        return false;
    }

    return true;
}

bool Plot::TranslateInterpolationPoint(int Identifier, int Position, int x, int y)
{
    std::vector<std::vector<double>> OldControlPolygon = this->GetObject(this->GetObject(Identifier).GetIdentifierTail()[0]).GetPoints();
    this->ChangeInterpolationPoint(Identifier, Position, std::vector<double> {OldControlPolygon[Position][0] + x, OldControlPolygon[Position][1] + y });
    return true;
}

bool Plot::Select(int Identifier, bool RemoveOtherSelections)
{
    if (IsIn(Identifier, this->Identifiers)) {
        this->SetSelectedIdentifiers(Identifier);
        return true;
    }
    else return false;
}

bool Plot::ClearSelectedPoints()
{
    int Position = this->GetObjectPosition(-2);
    this->ObjectsNow[Position].ClearPoints();
    this->ObjectsNow[Position].ClearRefittedPoints();
    this->AddInvisibleIdentifier(-2);

    this->Update();
    this->Refresh();

    return true;
}

bool Plot::SetSelectedPoints(std::vector<std::vector<double>> Points)
{
    //[1]
    this->ClearSelectedPoints();
    //End[1]
    //[2]
    int Position = this->GetObjectPosition(-2);
    this->ObjectsNow[Position].SetPoints(Points);
    //End[2]
    //[3]Refitting SelectedPoints
    std::vector<std::vector<double>> RefittedPoints;
    std::vector<double> A = this->TransformData;
    for (auto i : Points) {
        RefittedPoints.push_back(std::vector<double> {i[0] * A[2] + A[0], i[1] * A[3] + A[1]});
    }
    this->ObjectsNow[Position].SetRefittedPoints(RefittedPoints);
    //End[3]
    //[4]
    this->RemoveInvisibeIdentifier(-2);
    //End[4]
    //[5]
    this->Update();
    //End[5]
    return true;
}

bool Plot::Translate(int Identifier, double x, double y)
{
    PlotObject obj = this->GetObject(Identifier);
    if (obj.GetType() == "BezierCurve" || obj.GetType() == "AitekensInterpolant") {
        std::vector<std::vector<double>> OldControlPolygon = this->GetObject(obj.GetIdentifierTail()[0]).GetPoints();
        std::vector<std::vector<double>> NewControlPolygon;
        for (auto i : OldControlPolygon) {
            NewControlPolygon.push_back(std::vector<double> {i[0] + x, i[1] + y});
        }
        PlotObject NewCurve(NewControlPolygon, obj.GetType(), obj.GetPrecision());
        NewCurve.SetPrecision(obj.GetPrecision());
        this->ReplaceObject(Identifier, NewCurve);
    }
    else if (obj.GetType() == "BezierControlPolygon" || obj.GetType() == "AitekensInterpolationPoints") {
        this->Translate(Identifier + 1, x, y);
    }
    else {//default case
        std::vector<std::vector<double>> OldPoints = obj.GetPoints();
        std::vector<std::vector<double>> NewPoints;
        std::vector<double> NewPoint;
        std::vector<double> OldPoint;
        for (int i{ 0 };i < OldPoints.size();i++) {
            NewPoint.clear();
            OldPoint = OldPoints[i];
            NewPoint.push_back(OldPoint[0] + x);
            NewPoint.push_back(OldPoint[1] + y);
            NewPoints.push_back(NewPoint);
        }
        PlotObject NewObj(NewPoints);
        this->ReplaceObject(obj.GetIdentifier(), NewObj);
    }
    for (int id : obj.GetConnectedIdentifierTail())this->Translate(id, x, y);
    return true;
}

bool Plot::Scale(int Identifier, double x, double y)
{
    PlotObject obj = this->GetObject(Identifier);
    if (obj.GetType() == "BezierCurve" || obj.GetType() == "AitekensInterpolant") {
        std::vector<std::vector<double>> OldControlPolygon = this->GetObject(obj.GetIdentifierTail()[0]).GetPoints();
        std::vector<std::vector<double>> NewControlPolygon;
        for (auto i : OldControlPolygon) {
            NewControlPolygon.push_back(std::vector<double> {i[0] * x, i[1] * y});
        }
        PlotObject NewCurve(NewControlPolygon, "BezierCurve", obj.GetPrecision());
        NewCurve.SetPrecision(obj.GetPrecision());
        this->ReplaceObject(Identifier, NewCurve);
    }
    else if (obj.GetType() == "BezierControlPolygon" || obj.GetType() == "AitekensInterpolationPoints") {
        this->Scale(Identifier + 1, x, y);
    }
    else {//default case
        std::vector<std::vector<double>> OldPoints = obj.GetPoints();
        std::vector<std::vector<double>> NewPoints;
        std::vector<double> NewPoint;
        std::vector<double> OldPoint;
        for (int i{ 0 };i < OldPoints.size();i++) {
            NewPoint.clear();
            OldPoint = OldPoints[i];
            NewPoint.push_back(OldPoint[0] * x);
            NewPoint.push_back(OldPoint[1] * y);
            NewPoints.push_back(NewPoint);
        }
        PlotObject NewObj(NewPoints);
        this->ReplaceObject(obj.GetIdentifier(), NewObj);
    }
    for (int id : obj.GetConnectedIdentifierTail())this->Translate(id, x, y);

    return true;
}

//PLOT OBJECT VV ------------------------
PlotObject::PlotObject()
{
    IsEmpty = true;
}

PlotObject::PlotObject(std::vector<std::vector<double>> PointsInput, wxString TypeInput, double PrecisionInput)
{
    IsEmpty = false;
    Precision = PrecisionInput;
    Type = TypeInput;
    Points = PointsInput;
    RefittedPoints = PointsInput;
}

PlotObject::PlotObject(const PlotObject& obj)
{
    this->Points = obj.Points;
    this->RefittedPoints = obj.RefittedPoints;
    this->IsEmpty = obj.IsEmpty;
    this->Precision = obj.Precision;
    this->Colour = obj.Colour;

    this->Identifier = obj.Identifier;
    this->IdentifierTail = obj.IdentifierTail;
    this->ConnectedIdentifierTail = obj.ConnectedIdentifierTail;

    this->Type = obj.Type;
}

std::vector<std::vector<double>> PlotObject::GetPoints()
{
    if (!IsEmpty) {
        return this->Points;
    }
    //*
    //Handles Error of empty request
    //*
}

std::vector<std::vector<double>> PlotObject::GetRefittedPoints()
{
    return this->RefittedPoints;
}

bool PlotObject::SetPoints(std::vector<std::vector<double>> PointsInput)
{
    Points.clear();
    Points = PointsInput;
    this->IsEmpty = false;
    return true;
}

bool PlotObject::SetType(wxString TypeInput)
{
    this->Type = TypeInput;
    return true;
}

bool PlotObject::SetIdentifier(double Input)
{
    this->Identifier = Input;
    return true;
}

bool PlotObject::SetColour(wxString ColourInput)
{
    this->Colour = ColourInput;
    return true;
}

bool PlotObject::AddToIdentifierTail(int Identifier)
{
    if(IsIn(Identifier,this->IdentifierTail))return false;
    else {
        this->IdentifierTail.push_back(Identifier);
        return true;
    }
}

double PlotObject::GetIdentifier()
{
    return this->Identifier;
}

double PlotObject::GetPrecision()
{
    return this->Precision;
}

bool PlotObject::SetPrecision(double PrecisionInput)
{
    this->Precision = PrecisionInput;
    return true;
}

std::vector<double> PlotObject::GetMinMaxBox()
{
    if (!this->IsEmpty) {
        double Xmin;
        double Xmax;
        double Ymin;
        double Ymax;
        bool IsFirstIteration = true;
        for (auto pnt : this->Points) {
            if (IsFirstIteration) {
                Xmin = pnt[0];
                Xmax = pnt[0];
                Ymin = pnt[1];
                Ymax = pnt[1];
                IsFirstIteration = false;
                continue;
            }
            if (pnt[0] < Xmin)Xmin = pnt[0];
            else if (pnt[0] > Xmax)Xmax = pnt[0];

            if (pnt[1] < Ymin)Ymin = pnt[1];
            else if (pnt[1] > Ymax)Ymax = pnt[1];
        }
        return std::vector<double> {Xmin, Xmax, Ymin, Ymax};
    }
    else {
        //*
        //Throws Error
        //*
    }
    return std::vector<double>();
}

std::vector<int> PlotObject::GetIdentifierTail()
{
    return this->IdentifierTail;
}

std::vector<int> PlotObject::GetConnectedIdentifierTail()
{
    return this->ConnectedIdentifierTail;
}

bool PlotObject::SetConnectedIdentifierTail(std::vector<int> ConnectedIdentifierTailInput)
{
    this->ConnectedIdentifierTail.clear();
    this->ConnectedIdentifierTail = ConnectedIdentifierTailInput;
    return true;
}

bool PlotObject::AddConnectedIdentifier(int ConnectedIdentifierInput)
{
    this->ConnectedIdentifierTail.push_back(ConnectedIdentifierInput);
    return true;
}

wxString PlotObject::GetColour()
{
    return this->Colour;
}

wxString PlotObject::GetType()
{
    return this->Type;
}

bool PlotObject::ClearPoints()
{
    this->Points.clear();
    return true;
}

bool PlotObject::AddPoint(std::vector<double> PointInput)
{
    this->Points.push_back(PointInput);
    this->RefittedPoints.clear();
    this->RefittedPoints = this->Points;
    this->IsEmpty = false;
    return true;
}

bool PlotObject::ClearRefittedPoints()
{
    this->RefittedPoints.clear();
    return true;
}

bool PlotObject::AddRefittedPoint(std::vector<double> point)
{
    this->RefittedPoints.push_back(point);
    return true;
}

bool PlotObject::SetRefittedPoints(std::vector<std::vector<double>> NewRefittedPoints)
{
    this->RefittedPoints.clear();
    this->RefittedPoints = NewRefittedPoints;
    return true;
}

void PlotObject::Clear()
{
    this->IsEmpty = true;
    Points.clear();
    Identifier = -1;
}
