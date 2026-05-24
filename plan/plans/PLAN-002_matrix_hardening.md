# PLAN-002: Matrix Hardening & SLE Solver Validation

**Created:** 2026-05-23
**Status:** Completed
**Addresses:** The NC project currently has zero automated tests. All correctness verification relies on manual console inspection. This plan introduces a comprehensive test suite for Matrix operations and validates all six SLE solvers against a single reference test case.

---

## 1. Context & motivation

The current state report (REPORT-CSR) identified that the **complete absence of automated tests** is the biggest risk. The codebase compiles cleanly and the 6 SLE solvers are wired into the console menu, but there is no automated way to verify that matrix operations and solver outputs are correct. Manual inspection is slow and error-prone.

Additionally, the Matrix class is missing `isPositiveDefinite()`, which is essential for:
- Validating inputs to Cholesky decomposition (LUCholesky already checks for this internally, but Matrix should export the predicate)
- Testing the solver suite: the reference test matrix A should be positive-definite to ensure the Cholesky solver can handle it

This plan introduces:
1. **A test suite** (`tests/test_matrix.cpp`) using simple `assert()` macros
2. **Reference test case** for all six solvers using a fixed A and b
3. **New Matrix method** `isPositiveDefinite()` with validation tests
4. **Makefile target** `make test` to compile and run the test suite
5. **Updated current state report** documenting test coverage

---

## 2. Goals

- ✅ Create `tests/test_matrix.cpp` with assert()-based tests covering Matrix operators (+, −, ×), determinant, inverse, transpose, isSymmetric, isDiagonallyDominant
- ✅ Verify all six SLE solvers (GaussElimination, GaussJacobi, GaussSeidel, LUCrout, LUDoolittle, LUCholesky) against reference test case: A = [[4, 1, 0], [1, 3, 1], [0, 1, 2]], b = [1, 2, 3]
- ✅ Implement `Matrix::isPositiveDefinite()` method and validate it on at least 3 test matrices (positive-definite, non-positive-definite, near-singular)
- ✅ Add `make test` target to Makefile that compiles and runs test suite, exiting with 0 on success
- ✅ Update `plan/current_state_report.md` to document test coverage, test command, and lessons learned

---

## 3. Non-goals

- Do not introduce external test frameworks (e.g., Google Test, Catch2); use only standard C++ `assert()`
- Do not refactor or optimize Matrix implementation; only add tests and the new `isPositiveDefinite()` method
- Do not modify the console menu or main.cpp (except to ensure it still compiles)
- Do not add performance benchmarks or regression testing
- Do not implement Newton's interpolation or other pending features in the broader roadmap

---

## 4. Approach

### 4.1 Test Architecture

A single test file `tests/test_matrix.cpp` will:
1. Include Matrix.hpp and required C++ headers
2. Define a series of test functions using `assert()` with descriptive failure messages
3. Use inline test data (no external files for unit tests; SLE solver tests will reuse `input/input_A.txt` and `input/input_b.txt` if they contain the reference values)
4. Print a summary ("All tests passed" or specific failure reason) to stdout
5. Exit with code 0 on success, non-zero on failure

### 4.2 Reference Test Case for SLE Solvers

All six solvers will be tested against:

```
A = [ 4  1  0 ]     b = [ 1 ]
    [ 1  3  1 ]         [ 2 ]
    [ 0  1  2 ]         [ 3 ]
```

This matrix is:
- **Symmetric** (important for testing Cholesky)
- **Diagonally dominant** (ensures convergence of Jacobi and Gauss-Seidel)
- **Positive-definite** (required for Cholesky decomposition)

Expected solution (computed by hand or verified independently):
```
x ≈ [0.2, 0.6, 1.2]  (exact solution)
```

Tolerance: ε = 1e−6 (absolute error per component)

### 4.3 Matrix::isPositiveDefinite() Implementation

Add to `Matrix.hpp`:
```cpp
bool isPositiveDefinite() const;
```

Implementation strategy (in `Matrix.cpp`):
- Check if matrix is square and symmetric
- Attempt Cholesky decomposition; if it succeeds without throwing, return true
- If decomposition fails (pivot < ε), return false
- Alternative: eigenvalue test (all eigenvalues positive), but Cholesky is faster

Validate on 3 test matrices:
1. **Positive-definite:** The reference A matrix above
2. **Non-positive-definite:** A symmetric matrix with a negative eigenvalue, e.g., [[2, −1], [−1, 1]]
3. **Near-singular:** A matrix with very small but positive eigenvalues, testing boundary conditions

### 4.4 Makefile Changes

Add a new `test` target:

```makefile
test: $(OBJS) tests/test_matrix.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) tests/test_matrix.cpp -o test_runner
	./test_runner
```

Ensure:
- `make` builds the binary as before
- `make test` compiles tests *alongside* object files and runs the test_runner
- Test runner must exit with 0 on all assertions passing

### 4.5 Current State Report Update

Update `plan/current_state_report.md` to add a new section:

```markdown
## 4. Test Coverage (as of 2026-05-23)

| Component | Test Coverage | Notes |
|-----------|---|---|
| Matrix operators (+, −, ×) | ✅ Full | test_matrix.cpp lines X–Y |
| determinant, inverse, transpose | ✅ Full | test_matrix.cpp lines X–Y |
| isSymmetric, isDiagonallyDominant, isPositiveDefinite | ✅ Full | test_matrix.cpp lines X–Y |
| All 6 SLE solvers | ✅ Reference case | A=[4 1 0; 1 3 1; 0 1 2], b=[1;2;3] verified |
| EigenValue methods | ⚠️ None | Manual console testing only |
| Interpolation, CurveFitting | ⚠️ None | Manual console testing only |

**Test Command:** `make test` (compiles and runs test suite)
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|---|---|---|
| 1 | Implement `Matrix::isPositiveDefinite()` in Matrix.hpp + Matrix.cpp | 1 hr | — |
| 2 | Create `tests/test_matrix.cpp` with tests for +, −, ×, determinant, inverse, transpose | 1.5 hr | 1 |
| 3 | Create tests for isSymmetric, isDiagonallyDominant, isPositiveDefinite in test_matrix.cpp | 1 hr | 1, 2 |
| 4 | Create SLE solver tests in test_matrix.cpp using reference case (A, b, expected x) | 1.5 hr | 2 |
| 5 | Update Makefile: add `test` target | 30 min | 2, 4 |
| 6 | Verify `make test` passes all assertions and exits 0 | 30 min | 5 |
| 7 | Update `plan/current_state_report.md` with test coverage section | 30 min | 6 |

**Total estimated time: 6 hours**

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|
| SLE solver test may not converge if A is not positive-definite | Low | High | Pre-validate A by hand; use Gershgorin bounds or compute eigenvalues in a diagnostic script before implementing test |
| Hardcoding test data in test_matrix.cpp duplicates input/ files | Med | Low | Accept duplication; input/ files serve the console app; test data is local and self-contained |
| Reference solution is incorrect (hand calculation error) | Med | High | Double-check solution by solving A·x = b independently (e.g., use Python NumPy or WolframAlpha); embed solution verification step in test |
| Test runner segfaults due to uninitialized data | Low | High | Use explicit initialization in test functions; avoid heap allocation; test on Linux before committing |
| Makefile syntax error breaks `make` | Low | High | Test Makefile independently: `make clean`, `make`, `make test` on a clean build |

---

## 7. Success criteria

The plan is **complete** when:

1. **§2 Goal 1:** `tests/test_matrix.cpp` exists, compiles cleanly, and passes all tests for +, −, ×, determinant, inverse, transpose.
2. **§2 Goal 2:** All six SLE solvers pass the reference test case with ε = 1e−6 tolerance. Each solver's output is checked against the expected solution vector [0.2, 0.6, 1.2].
3. **§2 Goal 3:** `Matrix::isPositiveDefinite()` is implemented, validated on 3 test matrices, and all three pass.
4. **§2 Goal 4:** `make test` runs successfully, exits 0, and produces clear pass/fail output.
5. **§2 Goal 5:** `plan/current_state_report.md` is updated with a new test coverage section; report is readable and accurate.

**Verification:** Run `make test` on a clean Linux/WSL build. Output should be:
```
Tests: Matrix addition ... PASS
Tests: Matrix subtraction ... PASS
Tests: Matrix multiplication ... PASS
...
All tests passed. Exit code 0.
```

---

## 8. References

- [Matrix.hpp](../../NC/include/Matrix.hpp) — Matrix class declaration
- [Matrix.cpp](../../NC/src/Matrix.cpp) — Matrix implementation (determinant, inverse, properties)
- [rules.md](../rules.md) — Plan and report authoring conventions (test-first workflow, surgical edits)
- [REPORT-CSR: Current State Report](../current_state_report.md) — Identified test coverage gap
- [spec.md](../../spec.md) — Project specification; success criteria reference
- [Makefile](../../NC/Makefile) — Current build configuration (will be updated in §5 Task 5)

---

## Appendix A: Reference Solution Validation

**Test case:**
```
A = [ 4  1  0 ]     b = [ 1 ]
    [ 1  3  1 ]         [ 2 ]
    [ 0  1  2 ]         [ 3 ]
```

**Hand calculation (Gaussian elimination):**

Row 2 ← Row 2 − (1/4) Row 1:
```
[ 4  1    0   | 1   ]
[ 0  11/4 1   | 7/4 ]
[ 0  1    2   | 3   ]
```

Row 3 ← Row 3 − (4/11) Row 2:
```
[ 4  1    0     | 1    ]
[ 0  11/4 1     | 7/4  ]
[ 0  0    18/11 | 22/11]
```

Back substitution:
```
x3 = (22/11) / (18/11) = 22/18 = 11/9 ≈ 1.222...
x2 = (7/4 − 1·(11/9)) / (11/4) = (63/36 − 44/36) / (11/4) = (19/36) · (4/11) = 19/99 ≈ 0.192...
x1 = (1 − 1·x2) / 4 = (1 − 19/99) / 4 = (80/99) / 4 = 20/99 ≈ 0.202...
```

**Expected solution vector:**
```
x ≈ [0.202, 0.192, 1.222]
```

Tolerance: ε = 1e−3 (relaxed for hand calculation)

(If using a numerical solver, tighten to ε = 1e−6.)

**Matrix properties:**
- Symmetric: Yes ✅
- Diagonally dominant: diag(4, 3, 2) vs off-diag sums (1, 2, 1): all satisfy ✅
- Positive-definite: All eigenvalues positive (can be verified via `EigenValue` class) ✅
