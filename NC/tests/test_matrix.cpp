#include "../include/Matrix.hpp"
#include "../include/SLE.hpp"
#include "../include/GaussElimination.hpp"
#include "../include/GaussJacobi.hpp"
#include "../include/GaussSeidel.hpp"
#include "../include/LUCrout.hpp"
#include "../include/LUDoolittle.hpp"
#include "../include/LUCholesky.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

// Tolerance for floating-point comparisons
const double EPS = 1e-6;
// Tolerance for SLE solver tests (looser for iterative methods)
const double SLE_TOL = 1e-4;

// Helper: Compare vectors with tolerance
bool vectorsEqual(const vector<double>& a, const vector<double>& b, double tol = EPS) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if (fabs(a[i] - b[i]) > tol) return false;
    return true;
}

// Test: Matrix addition
void test_matrix_addition() {
    Matrix A(2, 2), B(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;
    
    Matrix C = A + B;
    assert(fabs(C(0, 0) - 6) < EPS);
    assert(fabs(C(0, 1) - 8) < EPS);
    assert(fabs(C(1, 0) - 10) < EPS);
    assert(fabs(C(1, 1) - 12) < EPS);
    cout << "PASS: Matrix addition" << endl;
}

// Test: Matrix subtraction
void test_matrix_subtraction() {
    Matrix A(2, 2), B(2, 2);
    A(0, 0) = 5; A(0, 1) = 6;
    A(1, 0) = 7; A(1, 1) = 8;
    
    B(0, 0) = 1; B(0, 1) = 2;
    B(1, 0) = 3; B(1, 1) = 4;
    
    Matrix C = A - B;
    assert(fabs(C(0, 0) - 4) < EPS);
    assert(fabs(C(0, 1) - 4) < EPS);
    assert(fabs(C(1, 0) - 4) < EPS);
    assert(fabs(C(1, 1) - 4) < EPS);
    cout << "PASS: Matrix subtraction" << endl;
}

// Test: Matrix multiplication
void test_matrix_multiplication() {
    Matrix A(2, 2), B(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;
    
    Matrix C = A * B;
    // [1*5+2*7, 1*6+2*8] = [19, 22]
    // [3*5+4*7, 3*6+4*8] = [43, 50]
    assert(fabs(C(0, 0) - 19) < EPS);
    assert(fabs(C(0, 1) - 22) < EPS);
    assert(fabs(C(1, 0) - 43) < EPS);
    assert(fabs(C(1, 1) - 50) < EPS);
    cout << "PASS: Matrix multiplication" << endl;
}

// Test: Transpose
void test_transpose() {
    Matrix A(2, 3);
    A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
    A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;
    
    Matrix AT = A.transpose();
    assert(AT.getRows() == 3 && AT.getCols() == 2);
    assert(fabs(AT(0, 0) - 1) < EPS && fabs(AT(2, 1) - 6) < EPS);
    cout << "PASS: Transpose" << endl;
}

// Test: Determinant (2x2)
void test_determinant_2x2() {
    Matrix A(2, 2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;
    
    double det = A.determinant();
    // det = 1*4 - 2*3 = -2
    assert(fabs(det - (-2.0)) < EPS);
    cout << "PASS: Determinant (2x2)" << endl;
}

// Test: Determinant (3x3)
void test_determinant_3x3() {
    Matrix A(3, 3);
    A(0, 0) = 4; A(0, 1) = 1; A(0, 2) = 0;
    A(1, 0) = 1; A(1, 1) = 3; A(1, 2) = 1;
    A(2, 0) = 0; A(2, 1) = 1; A(2, 2) = 2;
    
    double det = A.determinant();
    // det = 4*(3*2 - 1*1) - 1*(1*2 - 1*0) + 0 = 4*5 - 2 = 18
    assert(fabs(det - 18.0) < EPS);
    cout << "PASS: Determinant (3x3)" << endl;
}

// Test: Inverse (2x2)
void test_inverse_2x2() {
    Matrix A(2, 2);
    A(0, 0) = 4; A(0, 1) = 7;
    A(1, 0) = 2; A(1, 1) = 6;
    
    Matrix Ainv = A.inverse();
    Matrix I = A * Ainv;
    
    // Check if result is identity
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            double expected = (i == j) ? 1.0 : 0.0;
            assert(fabs(I(i, j) - expected) < EPS);
        }
    cout << "PASS: Inverse (2x2)" << endl;
}

// Test: isSymmetric
void test_is_symmetric() {
    Matrix A(3, 3);
    A(0, 0) = 4; A(0, 1) = 1; A(0, 2) = 0;
    A(1, 0) = 1; A(1, 1) = 3; A(1, 2) = 1;
    A(2, 0) = 0; A(2, 1) = 1; A(2, 2) = 2;
    
    assert(A.isSymmetric());
    
    Matrix B(2, 2);
    B(0, 0) = 1; B(0, 1) = 2;
    B(1, 0) = 3; B(1, 1) = 4;
    assert(!B.isSymmetric());
    
    cout << "PASS: isSymmetric" << endl;
}

// Test: isDiagonallyDominant
void test_is_diagonally_dominant() {
    Matrix A(3, 3);
    A(0, 0) = 4; A(0, 1) = 1; A(0, 2) = 0;
    A(1, 0) = 1; A(1, 1) = 3; A(1, 2) = 1;
    A(2, 0) = 0; A(2, 1) = 1; A(2, 2) = 2;
    
    assert(A.isDiagonallyDominant());
    
    Matrix B(2, 2);
    B(0, 0) = 1; B(0, 1) = 5;
    B(1, 0) = 6; B(1, 1) = 2;
    assert(!B.isDiagonallyDominant());
    
    cout << "PASS: isDiagonallyDominant" << endl;
}

// Test: isPositiveDefinite - positive definite matrix
void test_is_positive_definite_true() {
    Matrix A(3, 3);
    A(0, 0) = 4; A(0, 1) = 1; A(0, 2) = 0;
    A(1, 0) = 1; A(1, 1) = 3; A(1, 2) = 1;
    A(2, 0) = 0; A(2, 1) = 1; A(2, 2) = 2;
    
    assert(A.isPositiveDefinite());
    cout << "PASS: isPositiveDefinite (positive-definite matrix)" << endl;
}

// Test: isPositiveDefinite - non-positive-definite matrix
void test_is_positive_definite_false() {
    Matrix A(2, 2);
    A(0, 0) = 2; A(0, 1) = -3;
    A(1, 0) = -3; A(1, 1) = 1;
    
    assert(!A.isPositiveDefinite());
    cout << "PASS: isPositiveDefinite (non-positive-definite matrix)" << endl;
}

// Test: isPositiveDefinite - near-singular matrix
void test_is_positive_definite_near_singular() {
    Matrix A(2, 2);
    A(0, 0) = 1.0; A(0, 1) = 0.999;
    A(1, 0) = 0.999; A(1, 1) = 1.0;
    
    // This is symmetric and positive definite (but nearly singular)
    assert(A.isPositiveDefinite());
    cout << "PASS: isPositiveDefinite (near-singular positive-definite matrix)" << endl;
}

// Reference test case setup
void setup_reference_system(Matrix& A, vector<double>& b) {
    A = Matrix(3, 3);
    A(0, 0) = 4; A(0, 1) = 1; A(0, 2) = 0;
    A(1, 0) = 1; A(1, 1) = 3; A(1, 2) = 1;
    A(2, 0) = 0; A(2, 1) = 1; A(2, 2) = 2;
    
    b = {1.0, 2.0, 3.0};
}

// Helper: Verify solution by checking Ax = b
bool verifySolution(const Matrix& A, const vector<double>& x, const vector<double>& b, double tol = SLE_TOL) {
    int n = A.getRows();
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++)
            sum += A(i, j) * x[j];
        if (fabs(sum - b[i]) > tol)
            return false;
    }
    return true;
}

// Test: SLE Gauss Elimination
void test_sle_gaussian_elimination() {
    GaussElimination ge(3);
    // Set matrix data
    ge(0, 0) = 4; ge(0, 1) = 1; ge(0, 2) = 0;
    ge(1, 0) = 1; ge(1, 1) = 3; ge(1, 2) = 1;
    ge(2, 0) = 0; ge(2, 1) = 1; ge(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    ge.setRHS(b);
    vector<double> x = ge.solve();
    
    assert(verifySolution(ge, x, b));
    cout << "PASS: SLE Gaussian Elimination" << endl;
}

// Test: SLE Gauss Jacobi
void test_sle_gaussian_jacobi() {
    GaussJacobi gj(3);
    // Set matrix data
    gj(0, 0) = 4; gj(0, 1) = 1; gj(0, 2) = 0;
    gj(1, 0) = 1; gj(1, 1) = 3; gj(1, 2) = 1;
    gj(2, 0) = 0; gj(2, 1) = 1; gj(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    gj.setRHS(b);
    vector<double> x = gj.solve();
    
    assert(verifySolution(gj, x, b));
    cout << "PASS: SLE Gaussian Jacobi" << endl;
}

// Test: SLE Gauss Seidel
void test_sle_gaussian_seidel() {
    GaussSeidel gs(3);
    // Set matrix data
    gs(0, 0) = 4; gs(0, 1) = 1; gs(0, 2) = 0;
    gs(1, 0) = 1; gs(1, 1) = 3; gs(1, 2) = 1;
    gs(2, 0) = 0; gs(2, 1) = 1; gs(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    gs.setRHS(b);
    vector<double> x = gs.solve();
    
    assert(verifySolution(gs, x, b));
    cout << "PASS: SLE Gaussian Seidel" << endl;
}

// Test: SLE LU Crout
void test_sle_lu_crout() {
    LUCrout luc(3);
    // Set matrix data
    luc(0, 0) = 4; luc(0, 1) = 1; luc(0, 2) = 0;
    luc(1, 0) = 1; luc(1, 1) = 3; luc(1, 2) = 1;
    luc(2, 0) = 0; luc(2, 1) = 1; luc(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    luc.setRHS(b);
    vector<double> x = luc.solve();
    
    assert(verifySolution(luc, x, b));
    cout << "PASS: SLE LU Crout" << endl;
}

// Test: SLE LU Doolittle
void test_sle_lu_doolittle() {
    LUDoolittle lud(3);
    // Set matrix data
    lud(0, 0) = 4; lud(0, 1) = 1; lud(0, 2) = 0;
    lud(1, 0) = 1; lud(1, 1) = 3; lud(1, 2) = 1;
    lud(2, 0) = 0; lud(2, 1) = 1; lud(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    lud.setRHS(b);
    vector<double> x = lud.solve();
    
    assert(verifySolution(lud, x, b));
    cout << "PASS: SLE LU Doolittle" << endl;
}

// Test: SLE LU Cholesky
void test_sle_lu_cholesky() {
    LUCholesky luch(3);
    // Set matrix data
    luch(0, 0) = 4; luch(0, 1) = 1; luch(0, 2) = 0;
    luch(1, 0) = 1; luch(1, 1) = 3; luch(1, 2) = 1;
    luch(2, 0) = 0; luch(2, 1) = 1; luch(2, 2) = 2;
    // Set RHS
    vector<double> b = {1.0, 2.0, 3.0};
    luch.setRHS(b);
    vector<double> x = luch.solve();
    
    assert(verifySolution(luch, x, b));
    cout << "PASS: SLE LU Cholesky" << endl;
}

// Main test runner
int main() {
    int test_count = 0;
    int pass_count = 0;
    
    try {
        test_matrix_addition(); test_count++; pass_count++;
        test_matrix_subtraction(); test_count++; pass_count++;
        test_matrix_multiplication(); test_count++; pass_count++;
        test_transpose(); test_count++; pass_count++;
        test_determinant_2x2(); test_count++; pass_count++;
        test_determinant_3x3(); test_count++; pass_count++;
        test_inverse_2x2(); test_count++; pass_count++;
        test_is_symmetric(); test_count++; pass_count++;
        test_is_diagonally_dominant(); test_count++; pass_count++;
        test_is_positive_definite_true(); test_count++; pass_count++;
        test_is_positive_definite_false(); test_count++; pass_count++;
        test_is_positive_definite_near_singular(); test_count++; pass_count++;
        test_sle_gaussian_elimination(); test_count++; pass_count++;
        test_sle_gaussian_jacobi(); test_count++; pass_count++;
        test_sle_gaussian_seidel(); test_count++; pass_count++;
        test_sle_lu_crout(); test_count++; pass_count++;
        test_sle_lu_doolittle(); test_count++; pass_count++;
        test_sle_lu_cholesky(); test_count++; pass_count++;
        
        cout << "\n========================================" << endl;
        cout << "All " << pass_count << " tests passed!" << endl;
        cout << "========================================" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
