#include "Matrix.h"
//constructor #1, if rows = 0, dont create double** for data
Matrix::Matrix(unsigned int Rows, unsigned int Cols, double fill) {
	rows = Rows;
	cols = Cols;
	if (rows != 0) {//in order to avoid 0 byte leak
		data = new double *[rows];
		for (unsigned int i = 0; i < rows; i++) {
			data[i] = new double[cols];
			for (unsigned int j = 0; j < cols; j++) {
				data[i][j] = fill;
			}
		}
	}
}
//constructor #2, initiate the matrix with both rows and cols are 0
Matrix::Matrix() {
	rows = 0; cols = 0;
}
//constructor #3, construct a copy of designated matrix
Matrix::Matrix(const Matrix& m) {
	rows = m.num_rows(); 
	cols = m.num_cols();
	if (rows != 0) {//in order to avoid 0 byte leak
		data = new double *[rows];
		for (unsigned int i = 0; i < rows; i++) {
			data[i] = new double[cols];
			for (unsigned int j = 0; j < cols; j++) {
				data[i][j] = m.matrix()[i][j];
			}
		}
	}
}
// destructor
Matrix::~Matrix() {
	clear();
}
// functions that return a double*
//if the int is out of bound, return a NULL pointer
double *Matrix::get_row(unsigned int row) {
	if (row >= rows || row < 0) { return NULL; } 
	double *temp;
	temp = new double[cols];
	for (unsigned int i = 0; i < cols; i++) {
		temp[i] = data[row][i];
	}
	return temp;
}
double *Matrix::get_col(unsigned int col) {
	if (col >= cols||col<0) { return NULL; }
	double *temp;
	temp = new double [rows];
	for (unsigned int i = 0; i < rows; i++) {
		temp[i] = data[i][col];
	}
	return temp;
}
// functions return a bool 
// if the target is out of bound or two matirces dont match, return 0
bool Matrix::get(unsigned int Row, unsigned int Col, double &value) {
	if (Row < rows && Col < cols) { value = data[Row][Col]; return 1; }
	return 0;
}
bool Matrix::set(unsigned int Row, unsigned int Col, double value) {
	if (Row < rows && Col < cols) { data[Row][Col] = value; return 1; }
	return 0;
}

bool Matrix::add(const Matrix& m) {
	if (m.num_cols() != cols || m.num_rows() != rows) { return 0; }
	for (unsigned int i = 0; i < m.num_rows(); i++) {
		for (unsigned int j = 0; j < m.num_cols(); j++) {
			data[i][j] += m.matrix()[i][j];
		}
	}
	return 1;
}
bool Matrix::subtract(const Matrix& m) {
	if (m.num_cols() != cols || m.num_rows() != rows) { return 0; }
	for (unsigned int i = 0; i < m.num_rows(); i++) {
		for (unsigned int j = 0; j < m.num_cols(); j++) {
			data[i][j] -= m.matrix()[i][j];
		}
	}
	return 1;
}
bool Matrix::swap_row(unsigned int row1, unsigned int row2) { 
	//simply switch the order of pointer to swap rows
	if (row1 >= rows || row2 >= rows) { return 0; }
	double* temp = data[row1];
	data[row1] = data[row2];
	data[row2] = temp;
	return 1;
}
// void funcitons
void Matrix::clear() {
	//distructor
	//when rows = 0, dont delete
	//in order to avoid unaccessible heap
	for (unsigned int i = 0; i < rows; i++) {
		delete[]data[i];
	}
	if (rows != 0) { 
		delete[] data;
	}
	rows = 0; cols = 0;
}
void Matrix::multiply_by_coefficient(double coefficient) {
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < cols; j++) {
			data[i][j] *= coefficient;
		}
	}
}

void Matrix::transpose() {
	unsigned int new_rows = cols;
	unsigned int new_cols = rows;
	if (rows != 0) {//in order to avoid unaccessible heap
		double **new_data = new double *[new_rows];
		for (unsigned int i = 0; i < new_rows; i++) {
			new_data[i] = new double[new_cols];
			for (unsigned int j = 0; j < new_cols; j++) {
				new_data[i][j] = data[j][i];
			}
		}
		clear();
		data = new_data; rows = new_rows; cols = new_cols;
	}
}
void Matrix::resize(unsigned int Rows, unsigned int Cols, double fill) {
	if (Rows != 0) {
		Matrix new_matrix(Rows, Cols, fill);
		// create a new matrix of designated rows and cols 
		//then fill it with datas if the position of one entry is also in bounds of the old matrix
		for (unsigned int i = 0; i < Rows; i++) {
			for (unsigned int j = 0; j < Cols; j++) {
				if (i < rows && j < cols) {
					new_matrix.set(i, j, data[i][j]);
				}
			}
		}
		clear();//clear the old matrix
		*this = new_matrix;//make the pointer point to the new matrix
	}
}
Matrix* Matrix::quarter() {
	Matrix * result;
	//  first calculate the expected size of new matrices
	unsigned int new_rows = (rows + 1) / 2;
	unsigned int new_cols = (cols + 1) / 2;
	result = new Matrix[4];
	// I,J  is the position of one sub_matrix[0][0] in the parent_matrix
	unsigned int I; unsigned int J;
	for (unsigned int index = 0; index < 4; index++) {
		Matrix x(new_rows, new_cols, 0);
		result[index] = x;//construct a sub_matrix
		if (index / 2 == 0) { I = 0; } //decide where the sub_matrix starts in the parent_matrix
		else { I = rows - new_rows; }
		if (index % 2 == 0) { J = 0; }
		else { J = cols - new_cols; }
		for (unsigned int i = 0; i <new_rows; i++) {
			for (unsigned int j =0 ; j <new_cols; j++) {
				//the sub_matrix locally matches with the parent_matrix
				result[index].set(i, j, data[I+i][J+j]);
			}
		}
	}
	return result;
}
// to assign a matirx to another
Matrix Matrix::operator=(const Matrix& m) {
	rows = m.num_rows();
	cols = m.num_cols();
	data = new double *[rows];
	for (unsigned int i = 0; i < rows; i++) {
		data[i] = new double[cols];
		for (unsigned int j = 0; j < cols; j++) {
			data[i][j] = m.matrix()[i][j];
		}
	}
	return *this;
}
// make matrices printable with <<
ostream& operator<< (ostream &out, const Matrix& m) {
	out << m.num_rows()<< " x " <<m.num_cols() <<" matrix:"<<endl;
	out << "[";
	for (unsigned int i = 0; i < m.num_rows(); i++) {
		if (i != 0) { out << " "; }
		for (unsigned int j = 0; j < m.num_cols(); j++) {
			out << " " << m.matrix()[i][j];
		}
		if (i != m.num_rows() - 1) { out << endl; }
	}
	out << " ]"<<endl;
	
	return out;
}
// make matrices comparable with == and !=
bool operator==(const Matrix& m1, const Matrix& m2) {
	if (m1.num_rows() != m2.num_rows() || m1.num_cols() != m2.num_cols())
	{ return 0; }
	for (unsigned int i = 0; i < m1.num_rows(); i++) {
		for (unsigned int j = 0; j < m1.num_cols(); j++) {
			if (m1.matrix()[i][j] != m2.matrix()[i][j]) { return 0; }
		}
	}
	return 1;
}
bool operator!=(const Matrix& m1,const Matrix& m2) {return !(m1 == m2);}
