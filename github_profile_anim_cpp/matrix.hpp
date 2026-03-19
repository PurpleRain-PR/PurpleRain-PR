#ifndef _NEURO_MATRIX_
#define _NEURO_MATRIX_

#include <string.h>
#include <stdio.h>


class Matrix {//no need to use constant matrices
	public:
		typedef double MatrixValue;
		MatrixValue *data;
		unsigned int size;
		unsigned int width;
		unsigned int height;

		Matrix():
			data(NULL),
			size(0),
			width(0),
			height(0) {};

		Matrix(unsigned int Height, unsigned int Width):
			data(new MatrixValue[Height * Width]),
			size(Height * Width),
			width(Width),
			height(Height) {};

		Matrix(Matrix &&mat):
			data(mat.data),
			size(mat.size),
			width(mat.width),
			height(mat.height) {
			mat.data = NULL;
		};

		Matrix(Matrix &mat):
			data(new MatrixValue[mat.size]),
			size(mat.size),
			width(mat.width),
			height(mat.height) {
			memcpy(this->data, mat.data, mat.size * sizeof(MatrixValue));
		};
		
		~Matrix() {
			delete[] this->data;
		}

		Matrix& operator= (Matrix &&mat) noexcept;
		Matrix& operator= (const Matrix &mat);
		MatrixValue* operator[] (const unsigned int row) const;
		Matrix operator* (const MatrixValue factor) const;
		Matrix operator* (const Matrix &mat) const;
		Matrix operator+ (const Matrix &mat) const;
		Matrix operator- (const Matrix &mat) const;
		Matrix operator~ () const;

		static void inPlaceAdd(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept;//ensure the matrices CAN be added before call the function!
		static void inPlaceSub(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept;//ensure the matrices CAN be subtracted before call the function!
		static void inPlaceMul(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept;//ensure the matrices CAN be multiplied before call the function!
		static void inPlaceMulTrans(const Matrix &mat1, const Matrix &Tmat2, Matrix &result) noexcept;
		static void inPlaceTransMul(const Matrix &Tmat1, const Matrix &mat2, Matrix &result) noexcept;
		static void inPlaceAddMul(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept;
		static void inPlaceAddMulTrans(const Matrix &mat1, const Matrix &Tmat2, Matrix &result) noexcept;
		static void inPlaceAddTransMul(const Matrix &Tmat1, const Matrix &mat2, Matrix &result) noexcept;
		static void inPlaceTrans(const Matrix &mat, Matrix &result) noexcept;
		static void inPlaceNeg(const Matrix &mat, Matrix &result) noexcept;
		static void inPlaceScale(const Matrix &mat, const MatrixValue factor, Matrix &result) noexcept;
		static void inPlaceScaleByElement(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept;
		static void inPlaceBroadcastColumnAdd(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept;//before call broadcast functions
		static void inPlaceBroadcastRowAdd(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept;//ensure the shape of the matrix and the vector FIT for the operation
		static void inPlaceBroadcastColumnSub(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept;//or it will lead to undefined behaviour
		static void inPlaceBroadcastRowSub(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept;

		void clear();
};

void Matrix::clear() {
	memset(this->data, 0, this->size * sizeof(MatrixValue));
}

Matrix::MatrixValue* Matrix::operator[] (const unsigned int row) const {
	return this->data + (row * this->width);
}

Matrix& Matrix::operator= (Matrix &&mat) noexcept {
    if (this == &mat) return *this;
	this->height = mat.height;
	this->width = mat.width;
	this->size = mat.size;
    delete[] this->data;
    this->data = mat.data;
    mat.data = NULL;
    return *this;
}

Matrix& Matrix::operator= (const Matrix &mat) {
    if (this == &mat) return *this;
	this->height = mat.height;
	this->width = mat.width;
	if(this->size != mat.size) {
		this->size = mat.size;
		delete[] this->data;
		this->data = new MatrixValue[mat.size];
	}
	memcpy(this->data, mat.data, sizeof(MatrixValue) * mat.size);
	return *this;
}

Matrix Matrix::operator~ () const {
	Matrix result(this->width, this->height);
//	for(unsigned int row = 0; row < this->height; row++) {
//		for(unsigned int column = 0; column < this->width; column++) {
//			result[column][row] = (*this)[row][column];
//		}
//	}
	Matrix::inPlaceTrans(*this, result);
	return result;
}

Matrix Matrix::operator* (const Matrix &mat) const {
	Matrix result(this->height, mat.width);
//	memset(result.data, 0, result.size * sizeof(double));
//	for(unsigned int row = 0; row < result.height; row++) {
//		for(unsigned int index = 0; index < this->width; index++) {
//			for(unsigned int column = 0; column < result.width; column++) {
//				result[row][column] += (*this)[row][index] * mat[index][column];
//			}
//		}
//	}
	Matrix::inPlaceMul(*this, mat, result);
	return result;
}

Matrix Matrix::operator+ (const Matrix &mat) const {
	Matrix result(this->height, this->width);
//	for(unsigned int index = 0; index < this->size; index++) {
//		result.data[index] = this->data[index] + mat.data[index];
//	}
	Matrix::inPlaceAdd(*this, mat, result);
	return result;
}

Matrix Matrix::operator- (const Matrix &mat) const {
	Matrix result(this->height, this->width);
//	for(unsigned int index = 0; index < this->size; index++) {
//		result.data[index] = this->data[index] - mat.data[index];
//	}
	Matrix::inPlaceSub(*this, mat, result);
	return result;
}

Matrix Matrix::operator* (const MatrixValue factor) const {
	Matrix result(this->height, this->width);
//	for(unsigned int index = 0; index < this->size; index++) {
//		result.data[index] = factor * this->data[index];
//	}
	Matrix::inPlaceScale(*this, factor, result);
	return result;
}

void Matrix::inPlaceAdd(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [size of mat1]
	for(unsigned int index = 0; index < mat1.size; index++) {
		result.data[index] = mat1.data[index] + mat2.data[index];
	}
}

void Matrix::inPlaceSub(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [size of mat1]
	for(unsigned int index = 0; index < mat1.size; index++) {
		result.data[index] = mat1.data[index] - mat2.data[index];
	}
}

void Matrix::inPlaceScale(const Matrix &mat, const MatrixValue factor, Matrix &result) noexcept {//guiding criterion: [size of mat1]
	for(unsigned int index = 0; index < mat.size; index++) {
		result.data[index] = factor * mat.data[index];
	}
}

void Matrix::inPlaceScaleByElement(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [size of mat1]
	for(unsigned int index = 0; index < mat1.size; index++) {
		result.data[index] = mat1.data[index] * mat2.data[index];
	}
}

void Matrix::inPlaceMul(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [width of mat1] [height of mat1] [width of mat2] [width of result] [size of result]
	result.clear();//implicit use of result's size
	for(unsigned int row = 0; row < mat1.height; row++) {
		MatrixValue *resultRow = result[row];//implicit use of result's width
		MatrixValue *mat1Row = mat1[row];//implicit use of mat1's width
		for(unsigned int index = 0; index < mat1.width; index++) {
			const MatrixValue temp = mat1Row[index];
			MatrixValue *mat2Index = mat2[index];
			for(unsigned int column = 0; column < mat2.width; column++) {
				resultRow[column] += temp * mat2Index[column];
			}
		}
	}
}

void Matrix::inPlaceTrans(const Matrix &mat, Matrix &result) noexcept {//guiding criterion: [width of mat] [height of mat] [width of result]
	for(unsigned int row = 0; row < mat.height; row++) {
		for(unsigned int column = 0; column < mat.width; column++) {
			result[column][row] = mat[row][column];//implacit use of both's width
		}
	}
}

void Matrix::inPlaceMulTrans(const Matrix &mat1, const Matrix &Tmat2, Matrix &result) noexcept {//guiding criterion: [width of mat1] [height of mat1] [width of Tmat2] [height of Tmat2] [width of result]
	for(unsigned int row = 0; row < mat1.height; row++) {
		MatrixValue *mat1Row = mat1[row];
		MatrixValue *resultRow = result[row];
		for(unsigned int column = 0; column < Tmat2.height; column++) {
			MatrixValue *mat2Row = Tmat2[column];
			MatrixValue sum = 0;
			for(unsigned int index = 0; index < mat1.width; index++) {
				sum += mat1Row[index] * mat2Row[index];
			}
			resultRow[column] = sum;
		}
	}
}

void Matrix::inPlaceTransMul(const Matrix &Tmat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [width of Tmat1] [width of mat2] [height of mat2] [width of result] [size of result]
	result.clear();//implicit use of result's size
	for(unsigned int index = 0; index < mat2.height; index++) {
		MatrixValue *mat1Row = Tmat1[index];
		MatrixValue *mat2Row = mat2[index];
		for(unsigned int row = 0; row < Tmat1.width; row++) {
			const MatrixValue temp = mat1Row[row];
			MatrixValue *resultRow = result[row];
			for(unsigned int column = 0; column < mat2.width; column++) {
				resultRow[column] += temp * mat2Row[column];
			}
		}
	}
}

void Matrix::inPlaceAddMul(const Matrix &mat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [width of mat1] [height of mat1] [width of mat2] [width of result]
	for(unsigned int row = 0; row < mat1.height; row++) {
		MatrixValue *resultRow = result[row];//implicit use of result's width
		MatrixValue *mat1Row = mat1[row];//implicit use of mat1's width
		for(unsigned int index = 0; index < mat1.width; index++) {
			const MatrixValue temp = mat1Row[index];
			MatrixValue *mat2Index = mat2[index];
			for(unsigned int column = 0; column < mat2.width; column++) {
				resultRow[column] += temp * mat2Index[column];
			}
		}
	}
}

void Matrix::inPlaceAddMulTrans(const Matrix &mat1, const Matrix &Tmat2, Matrix &result) noexcept {//guiding criterion: [width of mat1] [height of mat1] [width of Tmat2] [height of Tmat2] [width of result]
	for(unsigned int row = 0; row < mat1.height; row++) {
		MatrixValue *mat1Row = mat1[row];
		MatrixValue *resultRow = result[row];
		for(unsigned int column = 0; column < Tmat2.height; column++) {
			MatrixValue *mat2Row = Tmat2[column];
			MatrixValue sum = 0;
			for(unsigned int index = 0; index < mat1.width; index++) {
				sum += mat1Row[index] * mat2Row[index];
			}
			resultRow[column] += sum;
		}
	}
}

void Matrix::inPlaceAddTransMul(const Matrix &Tmat1, const Matrix &mat2, Matrix &result) noexcept {//guiding criterion: [width of Tmat1] [width of mat2] [height of mat2] [width of result]
	for(unsigned int index = 0; index < mat2.height; index++) {
		MatrixValue *mat1Row = Tmat1[index];
		MatrixValue *mat2Row = mat2[index];
		for(unsigned int row = 0; row < Tmat1.width; row++) {
			const MatrixValue temp = mat1Row[row];
			MatrixValue *resultRow = result[row];
			for(unsigned int column = 0; column < mat2.width; column++) {
				resultRow[column] += temp * mat2Row[column];
			}
		}
	}
}

void Matrix::inPlaceNeg(const Matrix &mat, Matrix &result) noexcept {//guiding criterion: [size of mat]
	for(unsigned int index = 0; index < mat.size; index++) {
		result.data[index] = -mat.data[index];
	}
}

void Matrix::inPlaceBroadcastColumnAdd(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept {//guiding criterion: [width of mat] [height of mat] [width of result]
	for(unsigned int row = 0; row < mat.height; row++) {
		MatrixValue temp = vec.data[row];
		MatrixValue *resultRow = result[row];
		MatrixValue *matRow = mat[row];
		for(unsigned int column = 0; column < mat.width; column++) {
			resultRow[column] = matRow[column] + temp;
		}
	}
}

void Matrix::inPlaceBroadcastRowAdd(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept {//guiding criterion: [width of mat] [height of mat] [width of result]
	for(unsigned int row = 0; row < mat.height; row++) {
		MatrixValue *resultRow = result[row];
		MatrixValue *matRow = mat[row];
		for(unsigned int column = 0; column < mat.width; column++) {
			resultRow[column] = matRow[column] + vec.data[column];
		}
	}
}

void Matrix::inPlaceBroadcastColumnSub(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept {//guiding criterion: [width of mat] [height of mat] [width of result]
	for(unsigned int row = 0; row < mat.height; row++) {
		MatrixValue temp = vec.data[row];
		MatrixValue *resultRow = result[row];
		MatrixValue *matRow = mat[row];
		for(unsigned int column = 0; column < mat.width; column++) {
			resultRow[column] = matRow[column] - temp;
		}
	}
}

void Matrix::inPlaceBroadcastRowSub(const Matrix &mat, const Matrix &vec, Matrix &result) noexcept {//guiding criterion: [width of mat] [height of mat] [width of result]
	for(unsigned int row = 0; row < mat.height; row++) {
		MatrixValue *resultRow = result[row];
		MatrixValue *matRow = mat[row];
		for(unsigned int column = 0; column < mat.width; column++) {
			resultRow[column] = matRow[column] - vec.data[column];
		}
	}
}

void print(Matrix *mat, const char str[]) {
	putchar('\n');
	puts(str);
	for(unsigned int i = 0; i < mat->height; i++) {
		printf("    |");
		for(unsigned int j = 0; j < mat->width; j++) {
			if((*mat)[i][j] == 0.) {
				printf("\033[31m");
			}
			else {
				printf("\033[32m");
			}
			printf("\t%.3f ", (*mat)[i][j]);
		}
		printf("\033[0m");
		printf("\t |\n");
	}
	putchar('\n');
}

void printSize(Matrix *mat, const char str[]) {
	putchar('\n');
	puts(str);
	printf(" +----[%u]-->\n", mat->width);
	printf(" |[%u]\n", mat->size);
	puts(" |");
	printf("[%u]\n", mat->height);
	puts(" |");
	puts(" v");
	putchar('\n');
}

void printAlign(Matrix *mat1, Matrix *mat2) {
	printf("align [%I64d]\n", mat2->data - mat1->data);
	putchar('\n');
}

#endif