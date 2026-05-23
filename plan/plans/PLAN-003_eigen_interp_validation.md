# PLAN-003: EigenValue, Interpolation & LeastSquares Validation

**Created:** 2026-05-23
**Status:** Draft
**Addresses:** PLAN-002 (Matrix Hardening) established a test infrastructure with `make test` and verified Matrix operators and SLE solvers. This plan extends coverage to the remaining three numerical modules: EigenValue (power/QR methods, condition number), Interpolation (Lagrange basis polynomials), and LeastSquares (linear regression). The CurveFitting base class currently has a working implementation but no validation tests.

---

## 1. Context & motivation

The current state report (REPORT-CSR) identified that EigenValue, Interpolation, and CurveFitting/LeastSquares modules have **zero automated test coverage** — relying entirely on manual console menu testing. PLAN-002 fixed this for Matrix and SLE solvers. This plan completes the unit test suite for the remaining three modules.

**Key risks addressed:**
- EigenValue methods (powerMethod, qrAlgorithm, conditionNumber) are numerically sensitive; without automated tests, convergence issues or edge cases (identity matrix, near-singular matrices) would go undetected.
- Lagrange interpolation depends on careful basis polynomial evaluation; a single indexing error or sign flip can cause subtle failures.
- LeastSquares linearFit is the simplest fitting function; it must pass a perfect-fit test (residual = 0) and validate RMS error calculation.
- CurveFitting::fit() is not tested at all, leaving polynomial fitting validation to manual inspection.

---

## 2. Goals

- ✅ Create `tests/test_eigenvalue.cpp` with automated tests:
  - Power method on identity matrix returns 1.0 (within ε = 1e−9)
  - QR algorithm on a diagonal matrix extracts eigenvalues from diagonal
  - Gershgorin circle bounds on a 2×2 matrix are correctly computed
  - Condition number of identity matrix returns 1.0 (within ε = 1e−9)

- ✅ Create `tests/test_interpolation.cpp` with automated tests:
  - Linear interpolation (3 data points on y = x line) is exact at any query point (error < 1e−12)
  - Quadratic interpolation (3 data points on y = x² parabola) reproduces y at x = 2.5 within 1e−10

- ✅ Create `tests/test_leastsquares.cpp` with automated tests:
  - Perfect linear fit: 3 data points on y = 2x + 1 line have zero residual (< 1e−9)
  - RMS error validation: 3 noisy data points produce expected RMS value (< 1e−6 tolerance)

- ✅ Integrate all three test files into Makefile `test` target (compile and run sequentially)

- ✅ Review CurveFitting implementation and either:
  - (A) Confirm it is complete and document its status in the plan report, OR
  - (B) Identify any stub code and remove it with justification

---

## 3. Non-goals

- Do not create integration tests that combine multiple modules (e.g., fit data, then interpolate result)
- Do not add performance benchmarks or convergence rate analysis
- Do not refactor EigenValue, Lagrange, or LeastSquares implementations (test only)
- Do not implement missing EigenValue methods (Rayleigh quotient, Jacobi method); test only what exists
- Do not modify the console menu or main.cpp (except to ensure it still compiles after test additions)

---

## 4. Approach

### 4.1 Test File Architecture

Three independent test files, each with the same structure:
1. Include necessary headers (Matrix.hpp, EigenValue.hpp, etc.)
2. Define test functions using `assert()` with descriptive messages
3. Use inline test data (no external files)
4. Print "PASS" or "FAIL" to stdout
5. Exit with code 0 on all assertions passing, non-zero on failure

Tests can be compiled and run individually or as part of `make test`.

### 4.2 EigenValue Tests (test_eigenvalue.cpp)

**Test 1: powerMethod() on identity matrix**
```
Matrix I = Matrix::identity(3)
EigenValue ev(I);
assert(fabs(ev.powerMethod() - 1.0) < 1e-9)
```
Rationale: Identity has all eigenvalues = 1; power method should converge to largest, which is 1.

**Test 2: qrAlgorithm() on diagonal matrix**
```
Matrix D(3, 3);
D(0,0)=5; D(1,1)=3; D(2,2)=2; // rest are 0
EigenValue ev(D);
vector<double> evals = ev.qrAlgorithm();
// evals should contain {5, 3, 2} in some order
assert eigenvalues match {5, 3, 2}
```
Rationale: Diagonal matrix eigenvalues are its diagonal entries. QR should extract them exactly (within ε = 1e−9).

**Test 3: gershgorinBounds() on 2×2 matrix**
```
Matrix A(2, 2);
A(0,0)=2; A(0,1)=1; // row 0: center=2, radius=|1|=1, bounds=[1,3]
A(1,0)=1; A(1,1)=4; // row 1: center=4, radius=|1|=1, bounds=[3,5]
EigenValue ev(A);
auto bounds = ev.gershgorinBounds();
assert(bounds[0] == {1, 3});
assert(bounds[1] == {3, 5});
```
Rationale: Gershgorin bounds must match formula exactly.

**Test 4: conditionNumber() on identity matrix**
```
Matrix I = Matrix::identity(3);
EigenValue ev(I);
assert(fabs(ev.conditionNumber() - 1.0) < 1e-9);
```
Rationale: Identity has condition number 1.0 (perfectly conditioned); κ = ||A|| × ||A^−1|| = 1 × 1.

### 4.3 Interpolation Tests (test_interpolation.cpp)

**Test 1: Linear interpolation (y = x)**
```
Lagrange L(3);
// Data: (1, 1), (2, 2), (3, 3)
L(0, 0)=1; L(0, 1)=1;
L(1, 0)=2; L(1, 1)=2;
L(2, 0)=3; L(2, 1)=3;

double y_at_2_5 = L.interpolate(2.5);
assert(fabs(y_at_2_5 - 2.5) < 1e-12); // should be exactly 2.5
```
Rationale: Linear polynomial through 3 collinear points should interpolate exactly at any point.

**Test 2: Quadratic interpolation (y = x²)**
```
Lagrange L(3);
// Data: (0, 0), (1, 1), (2, 4)
L(0, 0)=0; L(0, 1)=0;
L(1, 0)=1; L(1, 1)=1;
L(2, 0)=2; L(2, 1)=4;

double y_at_2_5 = L.interpolate(2.5);
double expected = 2.5 * 2.5; // 6.25
assert(fabs(y_at_2_5 - expected) < 1e-10); // quadratic interpolation
```
Rationale: Lagrange polynomial through 3 points on y = x² should reproduce the parabola exactly within machine precision.

### 4.4 LeastSquares Tests (test_leastsquares.cpp)

**Test 1: Perfect linear fit (residual = 0)**
```
LeastSquares ls(3);
// Data on line y = 2x + 1: (1, 3), (2, 5), (3, 7)
ls(0, 0)=1; ls(0, 1)=3;
ls(1, 0)=2; ls(1, 1)=5;
ls(2, 0)=3; ls(2, 1)=7;

auto [a, b] = ls.linearFit();
assert(fabs(a - 2.0) < 1e-9);  // slope
assert(fabs(b - 1.0) < 1e-9);  // intercept
assert(ls.rmsError(a, b) < 1e-9); // residual ≈ 0
```
Rationale: If data lies exactly on a line, linear fit must recover the slope and intercept exactly, with zero residual.

**Test 2: RMS error validation**
```
LeastSquares ls(3);
// Data: (1, 2.1), (2, 4.0), (3, 6.1)
// Best fit: y = 2x + 0 (approximately)
// Residuals: 0.1, 0, 0.1
// RMS = sqrt((0.1^2 + 0^2 + 0.1^2) / 3) = sqrt(0.02/3) ≈ 0.0816
ls(0, 0)=1; ls(0, 1)=2.1;
ls(1, 0)=2; ls(1, 1)=4.0;
ls(2, 0)=3; ls(2, 1)=6.1;

auto [a, b] = ls.linearFit();
double rms = ls.rmsError(a, b);
double expected_rms = sqrt((0.1*0.1 + 0*0 + 0.1*0.1) / 3.0);
assert(fabs(rms - expected_rms) < 1e-6);
```
Rationale: RMS error calculation must match the formula; tolerance 1e−6 accounts for floating-point rounding.

### 4.5 Makefile Integration

Update the `test` target in Makefile to compile and run three test runners:

```makefile
test: $(OBJS) \
      tests/test_matrix.cpp \
      tests/test_eigenvalue.cpp \
      tests/test_interpolation.cpp \
      tests/test_leastsquares.cpp
	@echo "=== Running Matrix Tests ==="
	$(CXX) $(CXXFLAGS) $(OBJS) tests/test_matrix.cpp -o test_matrix
	./test_matrix
	@echo "=== Running EigenValue Tests ==="
	$(CXX) $(CXXFLAGS) $(OBJS) tests/test_eigenvalue.cpp -o test_eigenvalue
	./test_eigenvalue
	@echo "=== Running Interpolation Tests ==="
	$(CXX) $(CXXFLAGS) $(OBJS) tests/test_interpolation.cpp -o test_interpolation
	./test_interpolation
	@echo "=== Running LeastSquares Tests ==="
	$(CXX) $(CXXFLAGS) $(OBJS) tests/test_leastsquares.cpp -o test_leastsquares
	./test_leastsquares
	@echo "=== All test suites passed! ==="
```

The target must:
- Compile each test independently with all object files
- Run each test sequentially; if any fails, the Makefile stops
- Print section headers for readability
- Exit with code 0 only if all four tests pass

### 4.6 CurveFitting Review

The CurveFitting class (`NC/include/CurveFitting.hpp` and `NC/src/CurveFitting.cpp`) has:
- `readDataFromFile()` — loads n×2 data matrix
- `fit(degree)` — solves normal equations via Gauss elimination; returns polynomial coefficients
- `evaluate(xval, coeffs)` — evaluates polynomial at a point
- `rmsError(coeffs)` — computes RMS error for a fitted model
- `displayData()`, `displayFit()` — I/O helpers

**Status:** ✅ Complete. No stub code. Used by LeastSquares (inherits) and the console menu.

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|---|---|---|
| 1 | Create `tests/test_eigenvalue.cpp` with 4 test cases | 1.5 hr | — |
| 2 | Create `tests/test_interpolation.cpp` with 2 test cases | 1 hr | — |
| 3 | Create `tests/test_leastsquares.cpp` with 2 test cases | 1 hr | — |
| 4 | Update Makefile `test` target to compile and run all 3 new test files | 1 hr | 1, 2, 3 |
| 5 | Verify `make test` passes all tests and exits 0 | 30 min | 4 |
| 6 | Confirm CurveFitting has no stub code; document status | 30 min | — |
| 7 | Update `plan/current_state_report.md` with EigenValue, Interpolation, LeastSquares coverage | 30 min | 5, 6 |

**Total estimated time: 6 hours**

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|
| EigenValue power method converges to local extremum, not global | Low | Med | Test on identity and diagonal matrices where all eigenvalues are known; avoid near-zero eigenvalues |
| QR algorithm fails to converge on non-symmetric matrix | Low | Med | Test only on symmetric or diagonal matrices in this plan; defer non-symmetric testing to later |
| Lagrange basis polynomial has sign error or off-by-one indexing | Med | Med | Use hand-calculated expected values for all three interpolation points; visually inspect basis poly code before testing |
| LeastSquares perfect-fit test inadvertently has rounding error in expected RMS | Med | Med | Use exact fractions or compute expected value symbolically before hardcoding; review arithmetic |
| Makefile test target has syntax error or compilation fails for test files | Low | High | Test Makefile changes on a clean build; compile each test file individually first |
| CurveFitting::fit() uses deprecated std:: feature or has uninitialized variable | Low | Med | Review CurveFitting.cpp for warnings under `-Wall -Wextra`; no code changes, documentation only |

---

## 7. Success criteria

The plan is **complete** when:

1. **§2 Goal 1:** `tests/test_eigenvalue.cpp` exists, compiles cleanly, and all 4 tests pass (power method on identity = 1.0, QR on diagonal extracts eigenvalues, Gershgorin bounds correct, condition number of identity = 1.0).
2. **§2 Goal 2:** `tests/test_interpolation.cpp` exists, compiles cleanly, and both tests pass (linear interpolation error < 1e−12, quadratic interpolation error < 1e−10).
3. **§2 Goal 3:** `tests/test_leastsquares.cpp` exists, compiles cleanly, and both tests pass (perfect fit residual < 1e−9, RMS error matches expected within 1e−6).
4. **§2 Goal 4:** Makefile `test` target compiles all four test files and runs them sequentially, exiting 0 only if all pass. Output shows section headers and final "All test suites passed!" message.
5. **§2 Goal 5:** CurveFitting is confirmed complete with no stub code; status documented.

**Verification:** Run `make test` on a clean Linux/WSL build. Output should be:
```
=== Running Matrix Tests ===
Tests: Matrix addition ... PASS
...
=== Running EigenValue Tests ===
Tests: powerMethod on identity ... PASS
...
=== Running Interpolation Tests ===
Tests: linear interpolation ... PASS
...
=== Running LeastSquares Tests ===
Tests: perfect linear fit ... PASS
...
=== All test suites passed! ===
Exit code 0.
```

---

## 8. References

- [EigenValue.hpp](../../NC/include/EigenValue.hpp) — EigenValue class declaration
- [EigenValue.cpp](../../NC/src/EigenValue.cpp) — Power method, QR algorithm, Gershgorin, condition number implementations
- [Lagrange.hpp](../../NC/include/Lagrange.hpp) — Lagrange interpolation declaration
- [Lagrange.cpp](../../NC/src/Lagrange.cpp) — Basis polynomial and interpolation implementations
- [LeastSquares.hpp](../../NC/include/LeastSquares.hpp) — Linear fit class declaration
- [LeastSquares.cpp](../../NC/src/LeastSquares.cpp) — linearFit(), evaluate(), rmsError() implementations
- [CurveFitting.hpp](../../NC/include/CurveFitting.hpp) — Polynomial fitting base class
- [CurveFitting.cpp](../../NC/src/CurveFitting.cpp) — fit(), evaluate(), rmsError() implementations
- [rules.md](../rules.md) — Plan and report authoring conventions
- [PLAN-002](../plans/PLAN-002_matrix_hardening.md) — Matrix and SLE solver test suite (predecessor to this plan)
- [Makefile](../../NC/Makefile) — Build configuration (will be updated in §5 Task 4)

---

## Appendix A: Test Data Justification

### A.1 EigenValue Tests

**Identity Matrix Eigenvalues:**
All eigenvalues of I_n are 1. Power method, QR, and condition number must reflect this.

**Diagonal Matrix Eigenvalues:**
For diagonal D with entries 5, 3, 2, eigenvalues are exactly those values (on the main diagonal).

**Gershgorin Bounds on 2×2:**
```
A = [ 2  1 ]  →  Row 0: center=2, radius=1, bounds=[1,3]
    [ 1  4 ]      Row 1: center=4, radius=1, bounds=[3,5]
```

---

### A.2 Interpolation Tests

**Linear Interpolation (y = x):**
Three points (1,1), (2,2), (3,3) lie on y = x. Lagrange polynomial through them is the line itself. Query at x=2.5 must return exactly 2.5.

**Quadratic Interpolation (y = x²):**
Three points (0,0), (1,1), (2,4) lie on y = x². The unique degree-2 Lagrange polynomial through them is x² exactly. Query at x=2.5 must return 6.25 within machine precision (ε = 1e−10).

---

### A.3 LeastSquares Tests

**Perfect Linear Fit (y = 2x + 1):**
Three points (1,3), (2,5), (3,7) satisfy y = 2x + 1 exactly. linearFit() must recover a=2, b=1 with zero residual (< 1e−9).

**Noisy Linear Fit (y ≈ 2x):**
Three points (1, 2.1), (2, 4.0), (3, 6.1) are approximately on y = 2x (within ±0.1). Expected RMS error:
```
Residuals: 0.1, 0, 0.1
RMS = sqrt((0.01 + 0 + 0.01) / 3) = sqrt(0.02/3) ≈ 0.0816
```
Tolerance: ε = 1e−6 (floating-point rounding).
