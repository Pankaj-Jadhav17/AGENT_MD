#ifndef LUCHOLESKY_HPP
#define LUCHOLESKY_HPP

#include "SLE.hpp"

class LUCholesky : public SLE {
public:
    LUCholesky();
    LUCholesky(int n);
    vector<double> solve() override;
};

#endif // LUCHOLESKY_HPP
