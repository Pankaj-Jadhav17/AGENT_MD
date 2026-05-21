#include "Matrix.hpp"

// --- Constructors ---

Matrix::Matrix() : rows(0), cols(0) {}

Matrix::Matrix(int r, int c) : rows(r), cols(c), data(r, vector<double>(c, 0.0)) {}

Matrix::Matrix(int r, int c, double val) : rows(r), cols(c), data(r, vector<double>(c, val)) {}

// --- Access ---

int Matrix::getRows() const { return rows; }
int Matrix::getCols() const { return cols; }

double& Matrix::operator()(int i, int j) { return data[i][j]; }
double Matrix::operator()(int i, int j) const { return data[i][j]; }

vector<double> Matrix::getRow(int i) const { return data[i]; }

vector<double> Matrix::getCol(int j) const {
    vector<double> col(rows);
    for (int i = 0; i < rows; i++) col[i] = data[i][j];
    return col;
}

void Matrix::setRow(int i, const vector<double>& row) { data[i] = row; }

void Matrix::swapRows(int i, int j) { swap(data[i], data[j]); }

// --- Arithmetic Operators ---

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Matrix dimensions must match for addition");
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i, j) = data[i][j] + other(i, j);
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Matrix dimensions must match for subtraction");
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i, j) = data[i][j] - other(i, j);
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw runtime_error("Matrix dimensions incompatible for multiplication");
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < other.cols; j++)
            for (int k = 0; k < cols; k++)
                result(i, j) += data[i][k] * other(k, j);
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i, j) = data[i][j] * scalar;
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;
    const double eps = 1e-9;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (fabs(data[i][j] - other(i, j)) > eps) return false;
    return true;
}

// --- Matrix Operations ---

double Matrix::determinant() const {
    if (!isSquare())
        throw runtime_error("Matrix must be square for determinant");
    int n = rows;
    Matrix temp = *this;
    double det = 1.0;

    for (int i = 0; i < n; i++) {
        // Partial pivoting
        int maxRow = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(temp(k, i)) > fabs(temp(maxRow, i)))
                maxRow = k;

        if (maxRow != i) {
            temp.swapRows(i, maxRow);
            det *= -1.0;
        }

        if (fabs(temp(i, i)) < 1e-12) return 0.0;

        det *= temp(i, i);

        for (int k = i + 1; k < n; k++) {
            double factor = temp(k, i) / temp(i, i);
            for (int j = i; j < n; j++)
                temp(k, j) -= factor * temp(i, j);
        }
    }
    return det;
}

Matrix Matrix::inverse() const {
    if (!isSquare())
        throw runtime_error("Matrix must be square for inverse");
    int n = rows;

    // Augmented matrix [A | I]
    Matrix aug(n, 2 * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            aug(i, j) = data[i][j];
        aug(i, n + i) = 1.0;
    }

    // Gauss-Jordan elimination
    for (int i = 0; i < n; i++) {
        // Partial pivoting
        int maxRow = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(aug(k, i)) > fabs(aug(maxRow, i)))
                maxRow = k;
        aug.swapRows(i, maxRow);

        if (fabs(aug(i, i)) < 1e-12)
            throw runtime_error("Matrix is singular, cannot compute inverse");

        double pivot = aug(i, i);
        for (int j = 0; j < 2 * n; j++)
            aug(i, j) /= pivot;

        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = aug(k, i);
                for (int j = 0; j < 2 * n; j++)
                    aug(k, j) -= factor * aug(i, j);
            }
        }
    }

    // Extract inverse from right half
    Matrix inv(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv(i, j) = aug(i, n + j);
    return inv;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(j, i) = data[i][j];
    return result;
}

// --- Properties ---

bool Matrix::isSquare() const { return rows == cols; }

bool Matrix::isSymmetric() const {
    if (!isSquare()) return false;
    const double eps = 1e-9;
    for (int i = 0; i < rows; i++)
        for (int j = i + 1; j < cols; j++)
            if (fabs(data[i][j] - data[j][i]) > eps) return false;
    return true;
}

bool Matrix::isDiagonal() const {
    if (!isSquare()) return false;
    const double eps = 1e-9;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (i != j && fabs(data[i][j]) > eps) return false;
    return true;
}

bool Matrix::isDiagonallyDominant() const {
    if (!isSquare()) return false;
    for (int i = 0; i < rows; i++) {
        double diag = fabs(data[i][i]);
        double sum = 0.0;
        for (int j = 0; j < cols; j++)
            if (j != i) sum += fabs(data[i][j]);
        if (diag < sum) return false;
    }
    return true;
}

bool Matrix::isNull() const {
    const double eps = 1e-9;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (fabs(data[i][j]) > eps) return false;
    return true;
}

// --- I/O ---

void Matrix::readFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open())
        throw runtime_error("Cannot open file: " + filename);

    fin >> rows >> cols;
    data.assign(rows, vector<double>(cols, 0.0));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fin >> data[i][j];
    fin.close();
}

void Matrix::displayToFile(const string& filename) const {
    ofstream fout(filename, ios::app);
    if (!fout.is_open())
        throw runtime_error("Cannot open file for writing: " + filename);

    fout << fixed << setprecision(6);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            fout << setw(12) << data[i][j];
        fout << "\n";
    }
    fout.close();
}

void Matrix::display(ostream& os) const {
    os << fixed << setprecision(6);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            os << setw(12) << data[i][j];
        os << "\n";
    }
}

// --- Utility ---

Matrix Matrix::identity(int n) {
    Matrix I(n, n);
    for (int i = 0; i < n; i++) I(i, i) = 1.0;
    return I;
}

double Matrix::norm() const {
    // Infinity norm: max row sum of absolute values
    double maxSum = 0.0;
    for (int i = 0; i < rows; i++) {
        double sum = 0.0;
        for (int j = 0; j < cols; j++)
            sum += fabs(data[i][j]);
        maxSum = max(maxSum, sum);
    }
    return maxSum;
}
