# REPORT-CSR: Current State Report — NC Numerical Computation Toolbox

**Plan:** N/A (state audit, not tied to a specific plan)
**Completed:** 2026-05-18
**Author:** AI Agent

---

## 1. Summary

The NC project is in **good structural health**. The codebase compiled cleanly to Exit code 0 under `g++ -std=c++17 -Wall -Wextra` with zero errors and zero warnings. All core numerical modules (Matrix, SLE with 6 solvers, EigenValue, Lagrange Interpolation, LeastSquares, CurveFitting) are implemented and wired into an interactive 4-category menu-driven console application. The OOP inheritance hierarchy is correctly implemented as specified.

The **biggest risk** is the complete absence of automated tests. All correctness verification relies on manual console inspection. A secondary risk is that input file paths are hardcoded as relative strings (`"input/input_A.txt"`), meaning the binary must be run from the `NC/` directory — silently fails if invoked from anywhere else.

---

## 2. Source Code Inventory

### 2.1 Headers (`include/`)

| File | LOC | Role | Inherits |
|---|---|---|---|
| `Matrix.hpp` | 64 | Base class: 2D matrix storage, operators, algebra, property checks, file I/O | — |
| `SLE.hpp` | 34 | SLE base class: coefficient matrix is `this`, RHS vector `b` stored separately | `Matrix` |
| `GaussElimination.hpp` | 13 | Gauss Elimination with partial pivoting | `SLE` → `Matrix` |
| `GaussJacobi.hpp` | 17 | Jacobi iterative solver | `SLE` → `Matrix` |
| `GaussSeidel.hpp` | 17 | Gauss-Seidel iterative solver | `SLE` → `Matrix` |
| `LUCrout.hpp` | 13 | LU Crout decomposition | `SLE` → `Matrix` |
| `LUDoolittle.hpp` | 13 | LU Doolittle decomposition | `SLE` → `Matrix` |
| `LUCholesky.hpp` | 13 | LU Cholesky (symmetric positive-definite only) | `SLE` → `Matrix` |
| `EigenValue.hpp` | 33 | Power method, inverse power, QR algorithm, Gershgorin bounds, condition number | Composition of `Matrix` |
| `Interpolation.hpp` | 29 | Interpolation base: stores data as n×2 matrix, `xAt(i)`/`fAt(i)` accessors | `Matrix` |
| `Lagrange.hpp` | 16 | Lagrange basis polynomials and interpolation evaluation | `Interpolation` → `Matrix` |
| `CurveFitting.hpp` | 38 | Polynomial fitting via normal equations, arbitrary degree | `Matrix` |
| `LeastSquares.hpp` | 27 | Linear fit `y = ax + b`, `linearFit()`, `rmsError()` | `CurveFitting` → `Matrix` |

**Header total: 331 LOC**

### 2.2 Source (`src/`)

| File | LOC | Role |
|---|---|---|
| `Matrix.cpp` | 267 | Full implementation: determinant (row reduction), inverse (Gauss-Jordan), all operators and properties |
| `SLE.cpp` | 49 | `loadSystem()`, `setRHS()`, `displaySystem()`, `displaySolution()` |
| `GaussElimination.cpp` | 45 | Forward elimination with partial pivoting + back substitution |
| `GaussJacobi.cpp` | 31 | Jacobi iteration until convergence or maxIter |
| `GaussSeidel.cpp` | 30 | Gauss-Seidel iteration; uses latest values immediately |
| `LUCrout.cpp` | 50 | Crout factorisation: L has computed diagonal, U has unit diagonal |
| `LUDoolittle.cpp` | 50 | Doolittle factorisation: L has unit diagonal, U has computed diagonal |
| `LUCholesky.cpp` | 51 | Cholesky `A = L * L^T`; checks symmetry, positive-definiteness |
| `EigenValue.cpp` | 158 | Power method, inverse power, QR via Gram-Schmidt, Gershgorin, condition number |
| `Interpolation.cpp` | 36 | `readDataFromFile()`, `xAt()`, `fAt()`, `displayData()` |
| `Lagrange.cpp` | 25 | `basisPoly(i, xval)`, `interpolate(xval)` |
| `LeastSquares.cpp` | 56 | `linearFit()`, `evaluate(x,a,b)`, `rmsError(a,b)`, `displayLinearFit()` |
| `CurveFitting.cpp` | 142 | `readDataFromFile()`, `fit(degree)`, `evaluate()`, `rmsError()`, `displayFit()` |

**Source total: 990 LOC**

### 2.3 Entry Point

| File | LOC | Role |
|---|---|---|
| `main.cpp` | 384 | Menu-driven console app with `TeeBuf` dual output; 4 category sub-menus |

### 2.4 Build & Config

| File | LOC | Role |
|---|---|---|
| `Makefile` | 44 | Compiles all 13 `.cpp` + `main.cpp`; `dirs` target creates `output/` |

**Project Total: ~1,749 LOC** (all files combined)

---

## 3. Configuration Audit

### 3.1 Makefile Flags

| Flag | Effect | Assessment |
|---|---|---|
| `-std=c++17` | Enables C++17 features (structured bindings used in `LeastSquares.cpp`) | ✅ Required and correct |
| `-Wall` | Standard warnings | ✅ Good |
| `-Wextra` | Extended warnings | ✅ Good |
| `-Iinclude` | Adds `include/` to header search path | ✅ Correct |

No `-O2`/`-O3` optimisation flags are set. Acceptable for academic use; not a blocking issue.

### 3.2 Input File Dependencies

All six input files are hardcoded as **relative paths** within `main.cpp`:

| Hardcoded Path | Used In | Risk |
|---|---|---|
| `"input/input_A.txt"` | `matrixOpsMenu()` | ⚠️ Must run `./nc` from `NC/` |
| `"input/input_Atilde.txt"` | `matrixOpsMenu()` case 7 | ⚠️ Same |
| `"input/input_b.txt"` | `loadAndSolve<>()` calls | ⚠️ Same |
| `"input/input_btilde.txt"` | `matrixOpsMenu()` case 7 | ⚠️ Same |
| `"input/input_interpolation.txt"` | `interpolationMenu()` | ⚠️ Same |
| `"input/input_leastsquares.txt"` | `leastSquaresMenu()`, `curveFittingMenu()` | ⚠️ Same |
| `"output/output.txt"` | `main()` TeeBuf setup | ⚠️ Same |

**Risk:** If `./nc` is invoked from any directory other than `NC/`, all file opens silently fail and the program either crashes or produces incorrect output. No `chdir()` guard or error message indicates the actual cause.

### 3.3 Missing Configuration Files

| File | Status | Impact |
|---|---|---|
| `.gitignore` | ❌ Missing | `*.o`, `nc` binary, `output/output.txt` will be committed to git |

---

## 4. Test Suite Status

**There are zero automated tests in this project.**

| Test Type | Status | Notes |
|---|---|---|
| Unit tests | ❌ None | No test framework (e.g., Catch2, GoogleTest, or custom) |
| Integration tests | ❌ None | No scripted end-to-end runs |
| Regression tests | ❌ None | No golden-file comparisons |
| Manual verification | ⚠️ Only method | User runs `./nc` and reads console output |

**Gap:** Correctness of all numerical algorithms — determinant, inverse, all 6 solvers, eigenvalue methods, interpolation, least-squares, curve fitting — is currently unverified except by manual inspection. Any future refactor could silently break algorithms with no safety net.

**Recommended next step:** Add a `tests/` directory with a simple test harness that runs known inputs through each algorithm and compares against expected outputs.

---

## 5. Infrastructure

### 5.1 Build System

| Component | Status | Notes |
|---|---|---|
| GNU Make | ✅ Working | `make clean && make` exits with code 0 |
| g++ (GCC) | ✅ Working | C++17 structured bindings compile correctly |
| `-Wall -Wextra` | ✅ Zero warnings | Last build: clean |
| `make dirs` | ✅ Working | Creates `output/` directory before linking |
| `make clean` | ✅ Working | Removes all `.o` files and the `nc` binary |

### 5.2 CI/CD

| Component | Status | Notes |
|---|---|---|
| GitHub Actions | ❌ None | No `.github/workflows/` directory |
| Any CI pipeline | ❌ None | No automated build-on-push |

### 5.3 Platform

Tested and confirmed working on Linux. WSL compatibility is expected given pure POSIX standard library usage.

---

## 6. Known Issues & Technical Debt

| # | Severity | Issue | Location | Recommendation |
|---|---|---|---|---|
| 1 | 🔴 High | **No unit tests — zero test coverage** | Entire project | Add a `tests/` directory with at minimum one test per algorithm verifying a known numerical result |
| 2 | 🟡 Medium | **Input file paths are hardcoded relative** — binary must be run from `NC/` directory | `main.cpp` lines 100, 133–134, 173, 180–184, 219, 266, 299 | Use `realpath(argv[0])` to derive the base directory at runtime, or accept input directory as a CLI argument |
| 3 | 🟡 Medium | **No `.gitignore`** — build artefacts (`*.o`, `nc` binary, `output/output.txt`) will be committed | Repo root | Add `.gitignore` with entries for `*.o`, `nc`, `output/output.txt` |
| 4 | 🟡 Medium | **No CI/CD pipeline** — correctness not checked on push | Repo | Add a minimal GitHub Actions workflow that runs `make` and checks exit code 0 |
| 5 | 🟢 Low | **`TeeBuf` memory management** — `teeBuf` is heap-allocated with `new` and only deleted on clean exit; exceptions or early returns leak the buffer | `main.cpp` lines 345, 378 | Use `unique_ptr<TeeBuf>` and RAII |
| 6 | 🟢 Low | **No `-O2` optimisation flag** — QR algorithm with maxIter=1000 may be slow on large matrices | `Makefile` line 5 | Add `-O2` to `CXXFLAGS` for release builds |
| 7 | 🟢 Low | **`EigenValue` does not inherit `Matrix`** — design inconsistency with rest of hierarchy | `EigenValue.hpp` | Not blocking; composition is appropriate here since `EigenValue` is a computation class, not a data class |
| 8 | 🟢 Low | **Newton's Interpolation not yet implemented** | `include/`, `src/` | FEATURE 8 in spec.md; `NewtonInterpolation.hpp/.cpp` files do not exist yet |

---

*Report generated by AI agent on 2026-05-18. Verified against live codebase at `AGENT_MD_v1/NC/`.*
