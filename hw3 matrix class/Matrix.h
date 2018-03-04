#include <iostream>
using namespace std;

class Matrix {
public:

	//constructor:
	Matrix();
	Matrix(unsigned int Rows, unsigned int Cols, double fill);
	Matrix(const Matrix& m);
	~Matrix();
	unsigned int num_rows() const { return rows; }
	unsigned int num_cols() const { return cols; }
	double **matrix() const { return data; }
	bool get(unsigned int Row, unsigned int Col,double &value);
	double * get_row(unsigned int row);
	double * get_col(unsigned int col);
	bool set(unsigned int Row, unsigned int Col, double value);
	bool add(const Matrix& m);
	bool subtract(const Matrix& m);
	bool swap_row(unsigned int row1, unsigned int row2);
	Matrix operator=(const Matrix& m);
	void resize(unsigned int rows, unsigned int cols, double fill);
	void transpose();
	void multiply_by_coefficient(double coefficient);
	void clear();
	Matrix* quarter();
private:
	unsigned int rows; 
	unsigned int cols; 
	double** data; //where stores the specific datas
};

bool operator==(const Matrix& m1,const Matrix& m2);
bool operator!=(const Matrix& m1,const Matrix& m2);
ostream& operator<< (ostream &out, const Matrix& m);