#include "SLE.hpp"

SLE::SLE() {}

SLE::SLE(int n) : Matrix(n, n) {
    b.resize(n, 0.0);
}

void SLE::loadSystem(const string& matFile, const string& vecFile) {
    // Load coefficient matrix A using inherited Matrix::readFromFile
    readFromFile(matFile);

    // Load RHS vector b
    ifstream fin(vecFile);
    if (!fin.is_open())
        throw runtime_error("Cannot open file: " + vecFile);
    int size;
    fin >> size;
    b.resize(size);
    for (int i = 0; i < size; i++)
        fin >> b[i];
    fin.close();
}

void SLE::setRHS(const vector<double>& b) {
    this->b = b;
}

vector<double> SLE::getRHS() const {
    return b;
}

void SLE::displaySystem(ostream& os) const {
    os << "Coefficient Matrix A (" << getRows() << "x" << getCols() << "):\n";
    display(os);
    os << "\nRHS Vector b:\n  [";
    for (int i = 0; i < (int)b.size(); i++) {
        if (i > 0) os << ", ";
        os << b[i];
    }
    os << "]\n";
}

void SLE::displaySolution(const vector<double>& x, ostream& os) {
    os << fixed << setprecision(6);
    os << "Solution vector:\n";
    for (int i = 0; i < (int)x.size(); i++)
        os << "  x[" << i << "] = " << x[i] << "\n";
}
