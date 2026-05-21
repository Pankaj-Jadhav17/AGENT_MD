#ifndef GAUSSJACOBI_HPP
#define GAUSSJACOBI_HPP

#include "SLE.hpp"

class GaussJacobi : public SLE {
private:
    int maxIter;
    double tolerance;

public:
    GaussJacobi();
    GaussJacobi(int n, int maxIter = 1000, double tol = 1e-10);
    vector<double> solve() override;
};

#endif // GAUSSJACOBI_HPP
