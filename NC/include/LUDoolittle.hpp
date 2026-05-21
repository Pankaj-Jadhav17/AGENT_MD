#ifndef LUDOOLITTLE_HPP
#define LUDOOLITTLE_HPP

#include "SLE.hpp"

class LUDoolittle : public SLE {
public:
    LUDoolittle();
    LUDoolittle(int n);
    vector<double> solve() override;
};

#endif // LUDOOLITTLE_HPP
