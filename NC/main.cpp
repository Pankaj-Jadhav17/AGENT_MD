// NC — Numerical Computation Toolbox
// Interactive menu-driven console application
// All output is mirrored to both stdout and output/output.txt

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

#include "Matrix.hpp"
#include "SLE.hpp"
#include "GaussElimination.hpp"
#include "GaussJacobi.hpp"
#include "GaussSeidel.hpp"
#include "LUCrout.hpp"
#include "LUDoolittle.hpp"
#include "LUCholesky.hpp"
#include "EigenValue.hpp"
#include "Interpolation.hpp"
#include "Lagrange.hpp"
#include "LeastSquares.hpp"
#include "CurveFitting.hpp"

using namespace std;

// ─── Tee stream buffer: writes to two streambufs simultaneously ───
class TeeBuf : public streambuf {
    streambuf* sb1;
    streambuf* sb2;
protected:
    int overflow(int c) override {
        if (c == EOF) return !EOF;
        int r1 = sb1->sputc(c);
        int r2 = sb2->sputc(c);
        return (r1 == EOF || r2 == EOF) ? EOF : c;
    }
    int sync() override {
        int r1 = sb1->pubsync();
        int r2 = sb2->pubsync();
        return (r1 == 0 && r2 == 0) ? 0 : -1;
    }
public:
    TeeBuf(streambuf* s1, streambuf* s2) : sb1(s1), sb2(s2) {}
};

// ─── Utility ───
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getChoice() {
    int choice;
    cin >> choice;
    if (cin.fail()) { clearInput(); return -1; }
    return choice;
}

void pause() {
    cout << "\nPress Enter to continue...";
    clearInput();
    cin.get();
}

// ─── Helper: load an SLE subclass from files ───
template <typename T>
void loadAndSolve(const string& matFile, const string& vecFile, const string& name) {
    T solver;
    solver.loadSystem(matFile, vecFile);
    cout << "\n--- " << name << " ---\n";
    vector<double> x = solver.solve();
    SLE::displaySolution(x);
}

// ─── Category 1: Matrix Operations ───
void matrixOpsMenu() {
    while (true) {
        cout << "\n====================================\n";
        cout << "    MATRIX OPERATIONS\n";
        cout << "====================================\n";
        cout << "  1. Display Matrix & Properties\n";
        cout << "  2. Determinant\n";
        cout << "  3. Inverse\n";
        cout << "  4. Transpose\n";
        cout << "  5. Solve SLE (all methods)\n";
        cout << "  6. Eigenvalue Analysis\n";
        cout << "  7. Compare A vs A~ and b vs b~\n";
        cout << "  0. Back to Main Menu\n";
        cout << "------------------------------------\n";
        cout << "  Choice: ";

        int choice = getChoice();
        if (choice == 0) return;

        try {
            Matrix A;
            A.readFromFile("input/input_A.txt");

            switch (choice) {
            case 1: {
                cout << "\nMatrix A (" << A.getRows() << "x" << A.getCols() << "):\n";
                A.display();
                cout << "\nProperties:\n";
                cout << "  Square:              " << (A.isSquare() ? "Yes" : "No") << "\n";
                cout << "  Symmetric:           " << (A.isSymmetric() ? "Yes" : "No") << "\n";
                cout << "  Diagonal:            " << (A.isDiagonal() ? "Yes" : "No") << "\n";
                cout << "  Diagonally Dominant: " << (A.isDiagonallyDominant() ? "Yes" : "No") << "\n";
                cout << "  Null:                " << (A.isNull() ? "Yes" : "No") << "\n";
                break;
            }
            case 2: {
                cout << "\nDeterminant of A: " << fixed << setprecision(6)
                     << A.determinant() << "\n";
                break;
            }
            case 3: {
                Matrix inv = A.inverse();
                cout << "\nInverse of A:\n";
                inv.display();
                cout << "\nVerification A * A^(-1):\n";
                (A * inv).display();
                break;
            }
            case 4: {
                cout << "\nTranspose of A:\n";
                A.transpose().display();
                break;
            }
            case 5: {
                string mf = "input/input_A.txt";
                string vf = "input/input_b.txt";

                loadAndSolve<GaussElimination>(mf, vf, "Gauss Elimination");
                loadAndSolve<GaussJacobi>(mf, vf, "Gauss-Jacobi");
                loadAndSolve<GaussSeidel>(mf, vf, "Gauss-Seidel");
                loadAndSolve<LUCrout>(mf, vf, "LU Crout");
                loadAndSolve<LUDoolittle>(mf, vf, "LU Doolittle");

                if (A.isSymmetric()) {
                    loadAndSolve<LUCholesky>(mf, vf, "LU Cholesky");
                } else {
                    cout << "\n--- LU Cholesky ---\n";
                    cout << "  Skipped: matrix is not symmetric.\n";
                }
                break;
            }
            case 6: {
                EigenValue ev(A);
                cout << "\n--- Eigenvalue Analysis ---\n";
                cout << fixed << setprecision(6);

                cout << "\nPower Method (dominant eigenvalue): "
                     << ev.powerMethod() << "\n";
                cout << "Inverse Power Method (smallest eigenvalue): "
                     << ev.inversePowerMethod() << "\n";

                cout << "\nQR Algorithm (all eigenvalues):\n";
                vector<double> eigs = ev.qrAlgorithm();
                for (int i = 0; i < (int)eigs.size(); i++)
                    cout << "  lambda[" << i << "] = " << eigs[i] << "\n";

                cout << "\n";
                ev.displayGershgorin();
                cout << "\nCondition Number (inf-norm): "
                     << ev.conditionNumber() << "\n";
                break;
            }
            case 7: {
                Matrix Atilde;
                Atilde.readFromFile("input/input_Atilde.txt");

                cout << "\n--- Comparison: Original vs Perturbed ---\n";
                cout << "\nA:\n";  A.display();
                cout << "\nA~:\n"; Atilde.display();

                cout << "\nSolution of Ax = b:\n";
                loadAndSolve<GaussElimination>(
                    "input/input_A.txt", "input/input_b.txt", "Original");
                cout << "\nSolution of A~x = b~:\n";
                loadAndSolve<GaussElimination>(
                    "input/input_Atilde.txt", "input/input_btilde.txt", "Perturbed");

                EigenValue ev(A);
                cout << "\nCondition Number of A: "
                     << ev.conditionNumber() << "\n";
                break;
            }
            default:
                cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
        pause();
    }
}

// ─── Category 2: Interpolation ───
void interpolationMenu() {
    while (true) {
        cout << "\n====================================\n";
        cout << "    INTERPOLATION\n";
        cout << "====================================\n";
        cout << "  1. Lagrange Interpolation\n";
        cout << "  0. Back to Main Menu\n";
        cout << "------------------------------------\n";
        cout << "  Choice: ";

        int choice = getChoice();
        if (choice == 0) return;

        try {
            switch (choice) {
            case 1: {
                Lagrange lag;
                lag.readDataFromFile("input/input_interpolation.txt");
                lag.displayData();

                cout << "\nEnter x value to interpolate: ";
                double xval;
                cin >> xval;

                cout << fixed << setprecision(6);
                cout << "\nP(" << xval << ") = " << lag.interpolate(xval) << "\n";

                // Verify at data points
                cout << "\nVerification at data points:\n";
                cout << "  " << setw(12) << "x_i" << setw(12) << "f(x_i)"
                     << setw(16) << "P(x_i)" << "\n";
                for (int i = 0; i < lag.getN(); i++)
                    cout << "  " << setw(12) << lag.xAt(i) << setw(12) << lag.fAt(i)
                         << setw(16) << lag.interpolate(lag.xAt(i)) << "\n";
                break;
            }
            default:
                cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
        pause();
    }
}

// ─── Category 3: Least Squares ───
void leastSquaresMenu() {
    while (true) {
        cout << "\n====================================\n";
        cout << "    LEAST SQUARES\n";
        cout << "====================================\n";
        cout << "  1. Linear Fit (y = ax + b)\n";
        cout << "  0. Back to Main Menu\n";
        cout << "------------------------------------\n";
        cout << "  Choice: ";

        int choice = getChoice();
        if (choice == 0) return;

        try {
            switch (choice) {
            case 1: {
                LeastSquares ls;
                ls.readDataFromFile("input/input_leastsquares.txt");
                ls.displayData();
                ls.displayLinearFit();
                break;
            }
            default:
                cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
        pause();
    }
}

// ─── Category 4: Curve Fitting ───
void curveFittingMenu() {
    while (true) {
        cout << "\n====================================\n";
        cout << "    CURVE FITTING\n";
        cout << "====================================\n";
        cout << "  1. Polynomial Curve Fitting\n";
        cout << "  0. Back to Main Menu\n";
        cout << "------------------------------------\n";
        cout << "  Choice: ";

        int choice = getChoice();
        if (choice == 0) return;

        try {
            switch (choice) {
            case 1: {
                CurveFitting cf;
                cf.readDataFromFile("input/input_leastsquares.txt");
                cf.displayData();

                cout << "\nEnter polynomial degree: ";
                int degree;
                cin >> degree;

                cf.displayFit(degree);

                // Show fitted values
                auto coeffs = cf.fit(degree);
                cout << "\nFitted values:\n";
                cout << "  " << setw(12) << "x" << setw(12) << "f(x)"
                     << setw(16) << "y_fit" << setw(14) << "residual" << "\n";
                for (int i = 0; i < cf.getN(); i++) {
                    double xi = cf.xAt(i);
                    double fi = cf.fAt(i);
                    double yi = cf.evaluate(xi, coeffs);
                    cout << fixed << setprecision(6);
                    cout << "  " << setw(12) << xi << setw(12) << fi
                         << setw(16) << yi << setw(14) << (fi - yi) << "\n";
                }
                break;
            }
            default:
                cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
        pause();
    }
}

// ─── Main Menu ───
int main() {
    // Set up dual output: stdout + output/output.txt
    ofstream outFile("output/output.txt");
    if (!outFile.is_open()) {
        cerr << "Warning: Could not open output/output.txt for writing.\n";
    }

    TeeBuf* teeBuf = nullptr;
    streambuf* origBuf = cout.rdbuf();

    if (outFile.is_open()) {
        teeBuf = new TeeBuf(origBuf, outFile.rdbuf());
        cout.rdbuf(teeBuf);
    }

    cout << "========================================\n";
    cout << "  NC — Numerical Computation Toolbox\n";
    cout << "  C++17 Academic Numerical Methods\n";
    cout << "========================================\n";

    while (true) {
        cout << "\n========================================\n";
        cout << "           MAIN MENU\n";
        cout << "========================================\n";
        cout << "  1. Matrix Operations\n";
        cout << "     (Det / Inverse / SLE / Eigenvalues)\n";
        cout << "  2. Interpolation (Lagrange)\n";
        cout << "  3. Least Squares (y = ax + b)\n";
        cout << "  4. Curve Fitting (Polynomial)\n";
        cout << "  0. Exit\n";
        cout << "----------------------------------------\n";
        cout << "  Choice: ";

        int choice = getChoice();

        switch (choice) {
        case 1: matrixOpsMenu();     break;
        case 2: interpolationMenu(); break;
        case 3: leastSquaresMenu();  break;
        case 4: curveFittingMenu();  break;
        case 0:
            cout << "\nGoodbye!\n";
            cout.rdbuf(origBuf);
            if (outFile.is_open()) outFile.close();
            delete teeBuf;
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}
