#ifndef SLE_HPP
#define SLE_HPP

#include "Matrix.hpp"
#include <vector>

// SLE inherits Matrix — the coefficient matrix A is stored as the Matrix itself.
// The right-hand side vector b is stored separately.
class SLE : public Matrix {
protected:
    vector<double> b;

public:
    SLE();
    SLE(int n);
    virtual ~SLE() = default;

    // Load coefficient matrix A from file (uses Matrix::readFromFile)
    // Load RHS vector b from file
    void loadSystem(const string& matFile, const string& vecFile);
    void setRHS(const vector<double>& b);
    vector<double> getRHS() const;

    // Pure virtual: each subclass implements its own solving strategy
    virtual vector<double> solve() = 0;

    // Display the system Ax = b
    void displaySystem(ostream& os = cout) const;

    // Display a solution vector
    static void displaySolution(const vector<double>& x, ostream& os = cout);
};

#endif // SLE_HPP
