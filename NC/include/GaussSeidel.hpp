#ifndef GAUSSSEIDEL_HPP
#define GAUSSSEIDEL_HPP

#include "SLE.hpp"

class GaussSeidel : public SLE {
private:
    int maxIter;
    double tolerance;

public:
    GaussSeidel();
    GaussSeidel(int n, int maxIter = 1000, double tol = 1e-10);
    vector<double> solve() override;
};

#endif // GAUSSSEIDEL_HPP
