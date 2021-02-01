#pragma once
#include "vector"
class AitekensInterpolant
{
private:
	std::vector<std::vector<double>> Pnts;//{{ParameterValue,x,y,z}}
	std::vector<double> ParameterRange;
public:
	//Constructor
	AitekensInterpolant(std::vector<std::vector<double>> PointsInput);
	AitekensInterpolant();
	//Computer
	std::vector<double> compute(double t, int r = -1, int i = -1);
	
	std::vector<std::vector<double>> GetPoints();
	bool AddPoint(std::vector<double> NewPnt);
	std::vector<double> GetParameterRange();
};

