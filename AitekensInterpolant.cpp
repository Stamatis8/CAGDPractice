#include "AitekensInterpolant.h"

AitekensInterpolant::AitekensInterpolant(std::vector<std::vector<double>> PointsInput)
{
	ParameterRange.push_back(0);
	double parameter{ -1 };
	for (auto i : PointsInput) {
		parameter += 1;
		Pnts.push_back(std::vector<double> {parameter,i[0],i[1]});
	}
	ParameterRange.push_back(parameter);
}

AitekensInterpolant::AitekensInterpolant()
{
	//RiskyVV
	ParameterRange.push_back(0);
	ParameterRange.push_back(-1);
}

std::vector<double> AitekensInterpolant::compute(double t, int r, int i)
{
	if (Pnts.size() == 1) {
		return std::vector<double> {Pnts[0][1], Pnts[0][2]};
	}

	if (r == i && i == -1) {
		r = Pnts.size() - 1;
		i = 0;
	}

	if (r == 0) {
		return std::vector<double> {Pnts[i][1], Pnts[i][2]};
	}
	else {
		double CoefA = (Pnts[i + r][0] - t) / (Pnts[i + r][0] - Pnts[i][0]);
		double CoefB = (t - Pnts[i][0]) / (Pnts[i + r][0] - Pnts[i][0]);
		std::vector<double> vecA = compute(t, r - 1, i);
		std::vector<double> vecB = compute(t, r - 1, i + 1);
		return std::vector<double> {CoefA* vecA[0] + CoefB * vecB[0], CoefA* vecA[1] + CoefB * vecB[1]};
	}
	return std::vector<double> ();
}

std::vector<std::vector<double>> AitekensInterpolant::GetPoints()
{
	return this->Pnts;
}

bool AitekensInterpolant::AddPoint(std::vector<double> NewPnt)
{
	double x = NewPnt[0];
	double y = NewPnt[1];
	NewPnt.clear();
	NewPnt.push_back(ParameterRange[1] + 1);
	NewPnt.push_back(x);
	NewPnt.push_back(y);
	ParameterRange.clear();
	ParameterRange.push_back(0);
	ParameterRange.push_back(NewPnt[0]);
	Pnts.push_back(NewPnt);
	return true;
}

std::vector<double> AitekensInterpolant::GetParameterRange()
{
	return ParameterRange;
}

