#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;
using std::string;

class Matrix {
public:
    int matrix_size;
    double** matrix_data;

    Matrix(int size = 0) : matrix_size(size) {
        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
        }
    }

    Matrix(const Matrix& other) {
        matrix_size = other.matrix_size;
        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
            for (int j = 0; j < matrix_size; ++j) {
                matrix_data[i][j] = other.matrix_data[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) {
            return *this;
        }

        for (int i = 0; i < matrix_size; ++i) {
            delete[] matrix_data[i];
        }
        delete[] matrix_data;

        matrix_size = other.matrix_size;
        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
            for (int j = 0; j < matrix_size; ++j) {
                matrix_data[i][j] = other.matrix_data[i][j];
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& other) {
        Matrix ret_matrix(this->matrix_size);
        for (int i = 0; i < ret_matrix.matrix_size; i++) {
            for (int j = 0; j < ret_matrix.matrix_size; j++) {
                ret_matrix.matrix_data[i][j] = this->matrix_data[i][j] + other.matrix_data[i][j];
            }
        }
        return ret_matrix;
    }

    Matrix operator*(const Matrix& other) {
        Matrix ret_matrix(this->matrix_size);
        for (int i = 0; i < ret_matrix.matrix_size; i++) {
            for (int j = 0; j < ret_matrix.matrix_size; j++) {
                ret_matrix.matrix_data[i][j] = 0;
                for (int k = 0; k < ret_matrix.matrix_size; k++) {
                    ret_matrix.matrix_data[i][j] += this->matrix_data[i][k] * other.matrix_data[k][j];
                }
            }
        }
        return ret_matrix;
    }

    ~Matrix() {
        for (int i = 0; i < matrix_size; ++i) {
            delete[] matrix_data[i];
        }
        delete[] matrix_data;
    }
};

void read_matrix_from_file(const string& file_name, Matrix& matrix_1, Matrix& matrix_2) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::runtime_error("File ( " + file_name + " ) could not be opened");
    }

    int size;
    file >> size;
    matrix_1 = Matrix(size);
    matrix_2 = Matrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            file >> matrix_1.matrix_data[i][j];
        }
    }


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            file >> matrix_2.matrix_data[i][j];
        }
    }

    file.close();
}

void print_matrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.matrix_size; i++) {
        for (int j = 0; j < matrix.matrix_size; j++){
            cout << matrix.matrix_data[i][j] << " ";
        }
        cout << endl;
    }
}

void print_matrix(const Matrix& matrix_1, const Matrix& matrix_2) {
    cout << "Matrix 1" << endl;
    print_matrix(matrix_1);
    cout << "Matrix 2" << endl;
    print_matrix(matrix_2);
}

Matrix add_matrices(const Matrix& matrix_1, const Matrix& matrix_2) {
    Matrix ret_matrix(matrix_1.matrix_size);
    for (int i = 0; i < ret_matrix.matrix_size; i++) {
        for (int j = 0; j < ret_matrix.matrix_size; j++) {
            ret_matrix.matrix_data[i][j] = matrix_1.matrix_data[i][j] + matrix_2.matrix_data[i][j];
        }
    }

    return ret_matrix;
}

Matrix multiply_matrices(const Matrix& matrix_1, const Matrix& matrix_2) {
    Matrix ret_matrix(matrix_1.matrix_size);
    for (int i = 0; i < ret_matrix.matrix_size; i++) {
        for (int j = 0; j < ret_matrix.matrix_size; j++) {
            ret_matrix.matrix_data[i][j] = 0;
            for (int k = 0; k < ret_matrix.matrix_size; k++) {
                ret_matrix.matrix_data[i][j] += matrix_1.matrix_data[i][k] * matrix_2.matrix_data[k][j];
            } 
        }
    }

    return ret_matrix;
}


void get_diagonal_sum(const Matrix& matrix) {
    int sum = 0;
    const int size = matrix.matrix_size;

    for (int i = 0; i < size; i++) {
        sum += matrix.matrix_data[i][i];
    }

    for (int i = 0; i < size; i++) {
        sum += matrix.matrix_data[i][size - i - 1];
    }

    if (size % 2 == 1) {
        sum -= matrix.matrix_data[size / 2][size / 2];
    }

    cout << "Diagonal sum: " << sum << endl;
}

void swap_matrix_row(Matrix& matrix, int row1, int row2) {
    const int size = matrix.matrix_size;

    if (row1 >= size || row1 < 0 || row2 >= size || row2 < 0) {
        throw std::runtime_error("Swap matrix has out of bounds rows");
    }

    for (int i = 0; i < size; i++) {
        double temp = matrix.matrix_data[row1][i];
        matrix.matrix_data[row1][i] = matrix.matrix_data[row2][i];
        matrix.matrix_data[row2][i] = temp;
    }

    print_matrix(matrix);
}

int main(int argc, char* argv[]) {
    Matrix matrix_1, matrix_2;
    read_matrix_from_file("matrix.txt", matrix_1, matrix_2);

    cout << "print_matrix" << endl;
    print_matrix(matrix_1, matrix_2);

    Matrix add_result_1 = add_matrices(matrix_1, matrix_2);
    Matrix add_result_2 = matrix_1 + matrix_2;
    cout << "add_matrices result #1:" << endl;
    print_matrix(add_result_1);
    cout << "add_matrices result #2:" << endl;
    print_matrix(add_result_2);

    Matrix multiply_result_1 = multiply_matrices(matrix_1, matrix_2);
    Matrix multiply_result_2 = matrix_1 * matrix_2;
    cout << "multiply_matrices result #1:" << endl;
    print_matrix(multiply_result_1);
    cout << "multiply_matrices result #2:" << endl;
    print_matrix(multiply_result_2);

    cout << "get matrix diagonal sum:" << endl;
    get_diagonal_sum(matrix_1);

    cout << "swap matrix rows:" << endl;
    swap_matrix_row(matrix_1, 0, 1);

    return 0;
}