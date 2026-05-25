#ifndef GAUSSELIMINATION_HPP
#define GAUSSELIMINATION_HPP

#include "SLE.hpp"

class GaussElimination : public SLE {
public:
    GaussElimination();
    GaussElimination(int n);
    vector<double> solve() override;
};

#endif 
