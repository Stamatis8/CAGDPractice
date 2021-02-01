#include "matrix.h"

matrix::matrix(std::vector<int> Size, int Type)
{
	if (Type == 0) {
		std::vector<double> Vec;
		//Creating Rows
		for (int i{ 0 }; i < Size[1];i++) {
			Vec.push_back(0);
		}
		for (int i{ 0 }; i < Size[0];i++) {
			this->rows.push_back(Vec);
		}
		Vec.clear();
		//Creating Columns
		for (int i{ 0 };i < Size[0];i++) {
			Vec.push_back(0);
		}
		for (int i{ 0 };i < Size[1];i++) {
			this->columns.push_back(Vec);
		}
		//Setting Size
		this->NumberOfRows = Size[0];
		this->NumberOfColumns = Size[1];
		//Setting Rank
		this->Rank = 0;
	}


}

matrix::matrix(std::vector<std::vector<double>> Input_Vec, bool IsRow) {
	std::vector<std::vector<double>> Variable_Vec{ Input_Vec };
	//**
	//Checks and adjustments for size must happen here
	//**
	if (IsRow) {
		for (auto i : Variable_Vec) {
			this->rows.push_back(i);
			this->NumberOfRows++;
		}
		this->NumberOfColumns = Variable_Vec[0].size();
		std::vector<double> Temp;
		for (int i{ 0 };i < this->NumberOfColumns; i++) {
			for (int j{ 0 };j < this->NumberOfRows; j++) {
				Temp.push_back(this->rows[j][i]);
			}
			this->columns.push_back(Temp);
			Temp.clear();
		}
	}
	else {
		for (auto i : Variable_Vec) {
			this->columns.push_back(i);
			this->NumberOfColumns++;
		}
		this->NumberOfRows = Variable_Vec[0].size();
		std::vector<double> Temp;
		for (int i{ 0 };i < this->NumberOfRows; i++) {
			for (int j{ 0 };j < this->NumberOfColumns; j++) {
				Temp.push_back(this->columns[j][i]);
			}
			this->rows.push_back(Temp);
			Temp.clear();
		}
	}
}

matrix::matrix(std::vector<double> Vector, bool IsRow) {
	matrix(std::vector<std::vector<double>> {Vector}, IsRow);
}

matrix::matrix()
{
}

void matrix::Clear()
{
	this->rows.clear();
	this->columns.clear();
	this->NumberOfRows = 0;
	this->NumberOfColumns = 0;
	this->Rank = -1;
}

std::vector<std::vector<double>> matrix::GetRows()
{
	return this->rows;
}

std::vector<std::vector<double>> matrix::GetColumns()
{
	return this->columns;
}

std::vector<int> matrix::GetSize()
{
	return std::vector<int> {this->NumberOfRows, this->NumberOfColumns};
}

double matrix::GetValue(int i, int j)
{
	if (this->IsInSizeBoundaries(i, j)) {
		return this->rows[i - 1][j - 1];
	}
	else {
		//*
		//Error Should Be Detected Here
		//*
		return 1;
	}
}

bool matrix::SetValues(std::vector<std::vector<double>> Data, bool IsRow)
{
	matrix A(Data, IsRow);
	this->Clear();
	*this = A;
	return true;
}

bool matrix::add(double c)
{
	for (int i{ 1 }; i < this->GetSize()[0] + 1;i++) {
		for (int j{ 1 };j < this->GetSize()[1] + 1;j++) {
			this->ChangeValue(i, j, this->GetValue(i, j) + c);
		}
	}
	return true;
}

bool matrix::add(matrix B)
{
	if (!this->IsSizeEqual(B))return false;
	else {
		for (int i{ 1 };i < this->GetSize()[0] + 1;i++) {
			for (int j{ 1 }; j < this->GetSize()[1] + 1; j++) {
				this->ChangeValue(i, j, this->GetValue(i, j) + B.GetValue(i, j));
			}
		}
	}
	return true;
}

bool matrix::multiply(double c)
{
	for (int i{ 1 };i < this->GetSize()[0] + 1; i++) {
		for (int j{ 1 };j < this->GetSize()[1] + 1; j++) {
			this->ChangeValue(i, j, this->GetValue(i, j) * c);
		}
	}
	return true;
}

bool matrix::subtract(matrix B)
{
	if (this->IsSizeEqual(B)) {
		B.multiply(-1);
		this->add(B);
		B.multiply(-1);
	}
	else return false;
}

matrix matrix::operator+(matrix B)
{
	matrix C{ this->GetSize(),0 };
	C.add(*this);
	C.add(B);
	return C;
}

matrix matrix::operator-(matrix B)
{
	B.multiply(-1);
	matrix C{ *this + B };
	B.multiply(-1);
	return C;
}

matrix matrix::operator*(matrix B)
{
	if (this->NumberOfColumns != B.GetSize()[0]) {
		//*
		//Error Should be handled here, multiplication not possible
		//*
		matrix C;
		return C;
	}
	else {
		std::vector<std::vector<double>> CRows;
		std::vector<double> CRow;
		double CRowValue{ 0 };
		for (int i{ 1 };i < this->NumberOfRows + 1;i++) {
			for (int j{ 1 };j < B.GetSize()[1] + 1;j++) {
				//CRowValue (i,j) Starts
				for (int k{ 1 };k < this->NumberOfColumns + 1;k++) {
					CRowValue += this->GetValue(i, k) * B.GetValue(k, j);
				}
				//CRow i Starts
				CRow.push_back(CRowValue);
				CRowValue = 0;
				//CRowValue (i,j) Ends
			}
			CRows.push_back(CRow);
			CRow.clear();
			//CRow i Ends
		}
		matrix C{ CRows };
		return C;
	}
}

bool matrix::IsSizeEqual(matrix B)
{
	if (this->GetSize()[0] == B.GetSize()[0] && this->GetSize()[1] == B.GetSize()[1])return true;
	return false;
}

bool matrix::IsSizeEqual(int i, int j)
{
	if (this->GetSize()[0] == i && this->GetSize()[1] == j)return true;
	else return false;

}

bool matrix::IsInSizeBoundaries(int i, int j)
{
	if (i > this->GetSize()[0] || i < 0 || j > this->GetSize()[1] || j < 0)return false;
	else return true;

}

bool matrix::ChangeValue(int i, int j, double NewValue)
{
	if (!this->IsInSizeBoundaries(i, j)) return false;
	else {
		this->rows[i - 1][j - 1] = NewValue;
		this->columns[j - 1][i - 1] = NewValue;
	}

	return true;
}

