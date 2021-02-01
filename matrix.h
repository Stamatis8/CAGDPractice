#pragma once
#include <vector>
class matrix
{
private:
	std::vector<std::vector<double>> rows;
	std::vector<std::vector<double>> columns;
	int NumberOfRows{ 0 };
	int NumberOfColumns{ 0 };
	int Rank{ -1 };

public:
	//Constructors
	matrix(std::vector<int> Size, int Type = 0);//0-> 0 matrix
	matrix(std::vector<std::vector<double>> Input_Vecs, bool IsRow = true);
	matrix(std::vector<double> Vector, bool IsRow = true);
	matrix();

	//Clear
	void Clear();

	//GetSet
	std::vector<std::vector<double>> GetRows();
	std::vector<std::vector<double>> GetColumns();
	std::vector<int> GetSize();
	double GetValue(int i, int j);
	bool SetValues(std::vector<std::vector<double>> Data, bool IsRow = true);

	//Basic Operations ON Object
	bool add(double c);//adds c to all entries of A
	bool add(matrix B);
	bool multiply(double c);
	bool subtract(matrix B);

	//Operators
	matrix operator+ (matrix B);
	matrix operator- (matrix B);
	matrix operator* (matrix B);

	//Checks
	bool IsSizeEqual(matrix B);
	bool IsSizeEqual(int i, int j);
	bool IsInSizeBoundaries(int i, int j);

	//Change A Specific Value
	bool ChangeValue(int i, int j, double NewValue);
};




