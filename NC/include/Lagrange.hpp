#ifndef LAGRANGE_HPP
#define LAGRANGE_HPP

#include "Interpolation.hpp"

// Lagrange inherits Interpolation (which inherits Matrix)
class Lagrange : public Interpolation {
public:
    Lagrange();
    Lagrange(int n);

    double basisPoly(int i, double xval) const;
    double interpolate(double xval) override;
};

#endif // LAGRANGE_HPP
