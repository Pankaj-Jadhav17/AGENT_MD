#ifndef LUCROUT_HPP
#define LUCROUT_HPP

#include "SLE.hpp"

class LUCrout : public SLE {
public:
    LUCrout();
    LUCrout(int n);
    vector<double> solve() override;
};

#endif // LUCROUT_HPP
