#include "BezierCurve.h"
bool BezierCurve_IsIn(int i, std::vector<int> vector)
{
	for (int j : vector) {
		if (j == i)return true;
		else continue;
	}
	return false;
}

BezierCurve::BezierCurve(std::vector<std::vector<double>> ControlPointsInput, std::vector<double> ParameterRangeInput)
{
	ControlPoints = ControlPointsInput;
	ParameterRange = ParameterRangeInput;

	//Initializing AffineMap and TranslationVec
	std::vector<double> Row1{ 1,0,0 };
	std::vector<double> Row2{ 0,1,0 };
	std::vector<double> Row3{ 0,0,1 };
	std::vector<std::vector<double>> Rows{ Row1,Row2,Row3 };
	this->AffineMap.SetValues(Rows);

	std::vector<double> Column1{ 0,0,0 };
	std::vector<std::vector<double>> Columns{ Column1 };
	this->TranslationVector.SetValues(Columns, false);
}


std::vector<double> BezierCurve::compute(double t)
{
	if (BezierCurve_IsIn(1,this->ActiveFormList)) {//MatrixForm
		std::vector<double> point;
		for (int i{ 1 };i < this->ControlPoints[0].size() + 1;i++) {
			if (t == this->ParameterRange[0]) {
				point = this->ControlPoints[0];
				break;
			}
			else if (t == this->ParameterRange[1]) {
				point = this->ControlPoints[ControlPoints.size() - 1];
				break;
			}
			point.push_back((this->BezierMatrix * this->TMatrix(t)).GetValue(i, 1));
		}
		return point;
	}
	else {
		
	}
}

std::vector<double> BezierCurve::compute(double t, int r, int i)
{
	//[1]Checks Validity
	if (ControlPoints.size()>0
		&&r < ControlPoints.size() 
		&& i >= 0 
		&& i <= ControlPoints.size() - r) {

	}
	else {
		//*
		//Error Is Handled Here
		//*
		return std::vector<double>();
	}
	//End[1]

	//[2]Initializes variables
	std::vector<std::vector<double>> ControlPointsCopy;
	std::vector<double> FinalPoint{0,0,0};
	std::vector<double> TempControlPoint;
	double BernsteinPolynomialValue;
	//End[2]

	//[3]If needed convert 2D control points to 3D
	if (ControlPoints[0].size() == 2) {
		for (std::vector<double> k : ControlPoints) {
			k.push_back(0);
			ControlPointsCopy.push_back(k);
		}
	}
	else ControlPointsCopy = ControlPoints;
	//End[3]

	//[*]Handles r = 0
	if (r == 0) {
		return ControlPointsCopy[i];
	}
	//End[*]

	//[4]Loop Starts
	for (int j{ 0 };j <= r;j++) {
		TempControlPoint = ControlPointsCopy[i + j];
		BernsteinPolynomialValue = BernsteinPolynomial(r, j, t);
		FinalPoint[0] = FinalPoint[0] + TempControlPoint[0] * BernsteinPolynomialValue;
		FinalPoint[1] = FinalPoint[1] + TempControlPoint[1] * BernsteinPolynomialValue;
		FinalPoint[2] = FinalPoint[2] + TempControlPoint[1] * BernsteinPolynomialValue;
		TempControlPoint.clear();
	}
	//End[4]

	return FinalPoint;
}

double BezierCurve::BernsteinPolynomial(int r, int i, double t)
{
	return NChooseI(r,i)*Raise(t,i)*Raise((1-t),(r-i));
}

std::vector<double> BezierCurve::ForwardDifference(std::vector<std::vector<double>> Points, int degree)
{
	if (degree + 1 <= Points.size()) {
		if (degree == 0)return Points[0];
		else {
			//[1]Creating next points object;
			std::vector<std::vector<double>> NextPoints;
			for (int i{ 0 };i < Points.size();i++) {
				if(i!=0)NextPoints.push_back(Points[i]);
			}
			//
			std::vector<double> D1 = ForwardDifference(NextPoints,degree-1);
			std::vector<double> D0 = ForwardDifference(Points,degree-1);
			if (D1.size() == 2)
				return std::vector<double> {D1[0] - D0[0], D1[1] - D0[1]};
			else if(D1.size()==3)
				return std::vector<double> {D1[0] - D0[0], D1[1] - D0[1],D1[2]-D0[2]};
		}
	}
	else {
		//*
		//Error, not enough points in "Points"
		//*
	}
	return std::vector<double>();
}

std::vector<std::vector<double>> BezierCurve::Subdivide(double c)
{
	if (c >= this->ParameterRange[0] && c <= this->ParameterRange[1]) {
		std::vector<std::vector<double>> Result;
		//[1]Creating First Control Polygon
		for (int i{ 0 };i < ControlPoints.size();i++) {
			Result.push_back(compute(c, i, 0));
		}
		//End[1]
		//[2]Creating Second Control Polygon
		int n = ControlPoints.size() - 1;
		for (int i{ 0 };i < ControlPoints.size();i++) {
			Result.push_back(compute(c, n - i, i));
		}
		//End[2]

		return Result;
	}
	else {
		//*
		//Error Handled Here
		//*
		return std::vector<std::vector<double>>();
	}

}

std::vector<std::vector<double>> BezierCurve::ElevateDegree()
{
	std::vector<std::vector<double>> NewControlPoints;
	std::vector<double> NewPoint;
	NewControlPoints.push_back(ControlPoints[0]);
	int n = ControlPoints.size()-1;
	for(int i{1};i<n+1;i++){
		NewPoint.push_back(ControlPoints[i - 1][0] * (i / double(n + 1)) + ControlPoints[i][0] * (1 - (i / double(n + 1))));
		NewPoint.push_back(ControlPoints[i - 1][1] * (i / double(n + 1)) + ControlPoints[i][1] * (1 - (i / double(n + 1))));
		NewControlPoints.push_back(NewPoint);
		NewPoint.clear();
	}
	NewControlPoints.push_back(ControlPoints[n]);
	return NewControlPoints;
}

bool BezierCurve::Translate(double x, double y,double z)
{
	this->TranslationVector.ChangeValue(1, 1, x);
	this->TranslationVector.ChangeValue(2, 1, y);
	this->TranslationVector.ChangeValue(3, 1, z);
	this->Transform();
	this->ResetAffineTransform();
	return true;
}

bool BezierCurve::Scale(double x, double y, double z)
{
	this->AffineMap.ChangeValue(1, 1, x);
	this->AffineMap.ChangeValue(2, 2, y);
	this->AffineMap.ChangeValue(3, 3, z);
	//Moving it back to origin
	this->TranslationVector.ChangeValue(1, 1, this->ControlPoints[0][0] - this->ControlPoints[0][0] * x);
	this->TranslationVector.ChangeValue(2, 1, this->ControlPoints[0][1] - this->ControlPoints[0][1] * y);
	if(this->ControlPoints[0].size() == 3)this->TranslationVector.ChangeValue(3, 1, this->ControlPoints[0][2] - this->ControlPoints[0][2] * z);
	
	this->Transform();
	this->ResetAffineTransform();
	return true;
}

bool BezierCurve::ResetAffineTransform()
{
	this->AffineMap.Clear();
	std::vector<double> Row1{ 1,0,0 };
	std::vector<double> Row2{ 0,1,0 };
	std::vector<double> Row3{ 0,0,1 };
	std::vector<std::vector<double>> Rows{ Row1,Row2,Row3 };
	this->AffineMap.SetValues(Rows);

	this->TranslationVector.Clear();
	std::vector<double> Column1{ 0,0,0 };
	std::vector<std::vector<double>> Columns{ Column1 };
	this->TranslationVector.SetValues(Columns, false);

	return true;
}

bool BezierCurve::Transform()
{
	std::vector<double> ColumnVector;
	std::vector<std::vector<double>> OldPointMatrixColumn;
	matrix OldPointMatrix;

	matrix NewPointMatrix;
	std::vector<double> NewPoint;
	std::vector<std::vector<double>> NewPoints;

	if (this->ControlPoints[0].size() == 2) {
		for (auto i : this->ControlPoints) {
			ColumnVector.push_back(i[0]);
			ColumnVector.push_back(i[1]);
			ColumnVector.push_back(0);
			OldPointMatrixColumn.push_back(ColumnVector);
			OldPointMatrix.SetValues(OldPointMatrixColumn, false);
			NewPointMatrix = this->AffineMap * OldPointMatrix + this->TranslationVector;
			NewPoint.push_back(NewPointMatrix.GetValue(1, 1));
			NewPoint.push_back(NewPointMatrix.GetValue(2, 1));
			NewPoints.push_back(NewPoint);

			OldPointMatrixColumn.clear();
			ColumnVector.clear();
			OldPointMatrix.Clear();
			NewPointMatrix.Clear();
			NewPoint.clear();
		}
	}
	else if (this->ControlPoints[0].size() == 3) {
		for (auto i : this->ControlPoints) {
			ColumnVector = i;
			OldPointMatrixColumn.push_back(ColumnVector);
			OldPointMatrix.SetValues(OldPointMatrixColumn, false);
			NewPointMatrix = this->AffineMap * OldPointMatrix + this->TranslationVector;
			NewPoint.push_back(NewPointMatrix.GetValue(1, 1));
			NewPoint.push_back(NewPointMatrix.GetValue(2, 1));
			NewPoint.push_back(NewPointMatrix.GetValue(3, 1));
			NewPoints.push_back(NewPoint);

			ColumnVector.clear();
			OldPointMatrix.Clear();
			NewPointMatrix.Clear();
			NewPoint.clear();
		}
	}

	BezierCurve Result(NewPoints, this->GetParameterRange());
	Result.SetActiveForms(this->ActiveFormList);
	*this = Result;
	this->ToggleForms();

	return true;
}

bool BezierCurve::FalseAllForms()
{
	this->ActiveFormList.clear();
	this->ToggleForms();
	return true;
}

bool BezierCurve::ToggleForms()
{
	if (BezierCurve_IsIn(1, this->ActiveFormList)) {
		this->InitializeMatrixForm();
		return true;
	}
	else {
		return false;
	}
}

bool BezierCurve::SetActiveForms(std::vector<int> Input)
{
	this->ActiveFormList = Input;
	return true;
}

bool BezierCurve::InitializeMatrixForm()
{
	if(!BezierCurve_IsIn(1,this->ActiveFormList))this->ActiveFormList.push_back(1);
	this->ControlPointsMatrix = matrix(this->ControlPoints,false);
	std::vector<std::vector<double>> Rows;
	std::vector<double> Row;
	double value;
	int n = this->ControlPointsMatrix.GetSize()[1] - 1;
	for (int i{ 0 };i < n+1;i++) {
		for (int j{ 0 };j < n+1;j++) {
			double value = Raise(-1, j - i) * NChooseI(n, j) * NChooseI(j, i);
			Row.push_back(value);
		}
		Rows.push_back(Row);
		Row.clear();
	}
	this->BezierMatrix = this->ControlPointsMatrix*matrix(Rows);

	
	return true;
}

matrix BezierCurve::TMatrix(double t)
{
		std::vector<std::vector<double>> Columns;
		std::vector<double> Column;
		double n = this->ControlPoints.size();
		for (int i{ 0 };i < n;i++) {
			if (i == 0 && t == 0) {
				Column.push_back(0);
				continue;
			}
			Column.push_back(Raise(t, i));
		}
		Columns.push_back(Column);
		return matrix(Columns, false);
}

std::vector<double> BezierCurve::GetParameterRange()
{
	return this->ParameterRange;
}

std::vector<std::vector<double>> BezierCurve::GetControlPoints()
{
	return this->ControlPoints;
}

bool BezierCurve::Clear()
{
	this->ControlPoints.clear();
	this->ControlPointsMatrix.Clear();
	this->AffineMap.Clear();
	this->BezierMatrix.Clear();
	this->FalseAllForms();
	this->ParameterRange.clear();
	this->TranslationVector.Clear();
	return false;
}
