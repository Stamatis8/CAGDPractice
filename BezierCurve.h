#pragma once
#include "matrix.h"
#include "Functions.h"
class BezierCurve
{
private:
	std::vector<std::vector<double>> ControlPoints;
	std::vector<double> ParameterRange ;
	//MatrixForm
	matrix ControlPointsMatrix;
	matrix BezierMatrix;//b^n(t) = BezierMatrix*(t matrix)

	//FormHandling
	std::vector<int> ActiveFormList;//1->MatrixForm
	/*
	Adding New Form, update "ToggleForm", "compute" , "Respective form initializer"
	*/

	//Affine Transformations
	/*(The way its set up it only applies for scaling and translations
	due to a 3*3 matrix instead of 4*4)
	*/
	matrix AffineMap;
	matrix TranslationVector;
public:
	//Constructors
	BezierCurve(
		std::vector<std::vector<double>> ControlPointsInput,
		std::vector<double> ParameterRangeInput = { 0,1 });

	//Compute
	std::vector<double> compute(double t);//returns b^n(t)

	//Intermediate Points
	std::vector<double> compute(double t, int r, int i);//returns b^{r}_{i}(t)

	//BernsteinPolynomials
	double BernsteinPolynomial(int r, int i, double t);

	//ForwardDifferencing
	std::vector<double> ForwardDifference(std::vector<std::vector<double>> Points, int degree);
	
	//Subdivision
	std::vector<std::vector<double>> Subdivide(double c);//returns new control points for both subdivisions

	//Degree Elevation
	std::vector<std::vector<double>> ElevateDegree();//Only for 2D

	//Affine Transformations
	bool Translate(double x, double y,double z = 0);
	bool Scale(double x, double y, double z = 1);
	bool ResetAffineTransform();
	bool Transform();//Applies Transformation map and translation vector

	//FormHandling
	bool FalseAllForms();
	bool ToggleForms();
	bool SetActiveForms(std::vector<int> Input);

	//Matrix Form
	bool InitializeMatrixForm();
	matrix TMatrix(double t);//returns the matrix: {t^0 , ... , t^n}

	//SetGet
	std::vector<double> GetParameterRange();
	std::vector<std::vector<double>> GetControlPoints();

	//Clear
	bool Clear();

};

