# PLAN-004: Newton Interpolation & Numerical Integration

**Created:** 2026-05-23
**Status:** Draft
**Addresses:** PLAN-002 and PLAN-003 completed the test infrastructure and validated Matrix, SLE, EigenValue, and LeastSquares modules. The spec.md marks Newton's Interpolation (Feature 8) as a planned but incomplete P1 feature. Additionally, Numerical Integration is a new capability gap — the project currently lacks methods for approximating definite integrals, which is a common numerical analysis topic alongside interpolation.

---

## 1. Context & motivation

The project specification (spec.md) explicitly lists Newton's Interpolation as a planned feature under "Current Operational Reality" with status "Planned but incomplete". Success Criteria (v1.0) includes `[ ] Newton's interpolation method is implemented and tested` as an unchecked box.

Newton's method (divided differences) is a classic alternative to Lagrange interpolation. While Lagrange interpolation is already implemented, Newton's method offers pedagogical value:
- Reveals structure of polynomial interpolation through divided difference tables
- Efficient for adding new data points incrementally
- Numerically more stable in some contexts

Numerical Integration (trapezoidal rule, Simpson's rule) is a natural companion to interpolation — both are foundational numerical analysis topics. Students completing a course using this toolbox should have access to basic quadrature methods.

This plan completes Newton's Interpolation and introduces a new Integration module, rounding out the core numerical methods covered by the toolbox.

---

## 2. Goals

- ✅ Implement Newton interpolation (divided differences) with three methods:
  - `buildDividedDiff()` — compute the divided difference table from data points
  - `interpolate(x)` — evaluate the Newton polynomial at a query point
  - `printTable()` — display the divided difference table for inspection

- ✅ Create `include/Newton.hpp` and `src/Newton.cpp` following the Interpolation class hierarchy (Newton inherits from Interpolation)

- ✅ Integrate Newton into the console menu: add it as option 2 (after Lagrange) in the Interpolation category (Category 2)

- ✅ Update `NC/README.md` to document input file format for interpolation (x, f(x) pairs)

- ✅ Create `tests/test_newton.cpp` validating Newton interpolation on the same linear and quadratic test cases as Lagrange, ensuring both methods produce identical results within 1e−10

- ✅ Implement basic numerical integration with two methods:
  - `trapezoidal(a, b, n)` — composite trapezoidal rule with n subintervals
  - `simpsons(a, b, n)` — composite Simpson's rule with n subintervals

- ✅ Create `include/Integration.hpp` and `src/Integration.cpp` (no inheritance required; utilities composition or standalone methods)

- ✅ Add Numerical Integration as a new menu category (Category 4) in main.cpp with options for both methods

- ✅ Create `tests/test_integration.cpp` validating:
  - Trapezoidal rule: ∫₀¹ x² dx ≈ 0.333 (exact 1/3)
  - Simpson's rule: ∫₀^π sin(x) dx ≈ 2.0 (exact 2.0)

- ✅ Update spec.md Feature Index to add Numerical Integration (Feature 9) with status [ ], P1 priority

- ✅ Mark Newton's Interpolation (Feature 8) as ✅ Complete in Feature Index after implementation

---

## 3. Non-goals

- Do not implement Gaussian quadrature, adaptive quadrature, or other advanced integration methods; trapezoidal and Simpson's are sufficient for pedagogical purposes
- Do not add Newton-Cotes formulas beyond Simpson's rule
- Do not create file I/O for integration (functions are specified analytically, not via data files)
- Do not refactor or optimize the Interpolation base class; Newton simply inherits and extends it
- Do not modify existing Lagrange tests or implementation
- Do not add 3D interpolation, spline fitting, or other advanced interpolation variants
- Do not implement graphical visualization of interpolation or integration results

---

## 4. Approach

### 4.1 Newton Interpolation Architecture

**Class Hierarchy:**
```
Matrix (base)
  ↑
Interpolation (inherits Matrix; stores n×2 data)
  ↑
Newton (inherits Interpolation; adds divided difference computation)
```

**Key Design Decisions:**
- Newton inherits from Interpolation (like Lagrange) to reuse data storage and accessors (xAt(i), fAt(i), getN())
- Divided difference table is computed and cached once in a private member variable `diffTable[n][n]`
- `interpolate(x)` uses Horner's method for numerically stable polynomial evaluation using Newton form: P(x) = f₀ + (x−x₀)·f[x₀,x₁] + (x−x₀)(x−x₁)·f[x₀,x₁,x₂] + ...
- `printTable()` displays the divided difference triangle for pedagogical value

**Divided Differences Formula:**
- Zero-order: f[xᵢ] = f(xᵢ)
- First-order: f[xᵢ, xᵢ₊₁] = (f[xᵢ₊₁] − f[xᵢ]) / (xᵢ₊₁ − xᵢ)
- Higher-order: f[xᵢ, …, xᵢ₊ₖ] = (f[xᵢ₊₁, …, xᵢ₊ₖ] − f[xᵢ, …, xᵢ₊ₖ₋₁]) / (xᵢ₊ₖ − xᵢ)

### 4.2 Numerical Integration Architecture

**Class Structure:**
```
class Integration {
  public:
    static double trapezoidal(double a, double b, int n, double (*f)(double));
    static double simpsons(double a, double b, int n, double (*f)(double));
};
```

No inheritance; static methods that accept function pointers. This is simpler and aligns with the fact that integration operates on functions, not data matrices.

**Algorithm Details:**

**Trapezoidal Rule (n subintervals):**
```
h = (b − a) / n
I ≈ (h/2) · [f(a) + 2·f(a+h) + 2·f(a+2h) + … + 2·f(a+(n−1)h) + f(b)]
```

**Simpson's Rule (n subintervals; n must be even):**
```
h = (b − a) / n
I ≈ (h/3) · [f(a) + 4·f(a+h) + 2·f(a+2h) + 4·f(a+3h) + … + f(b)]
    (alternating 4, 2 coefficients for interior points)
```

### 4.3 Menu Integration

**Category 2 (Interpolation) — Add Newton as option 2:**
```
Interpolation Menu:
  1. Lagrange (existing)
  2. Newton (new)
  (load from input_interpolation.txt)
```

**New Category 4 (Numerical Integration):**
```
Integration Menu:
  1. Trapezoidal Rule — ∫ x² dx [0, 1]
  2. Simpson's Rule — ∫ sin(x) dx [0, π]
  (predefined test functions)
```

### 4.4 Input Format Documentation (NC/README.md)

Add section documenting interpolation input file format:
```
## Input Files

### Interpolation Data (input_interpolation.txt)
Format:
  n          (number of data points)
  x₁ f(x₁)   (pairs of x and f(x), one per line)
  x₂ f(x₂)
  …
  xₙ f(xₙ)

Example: 3 points for y = x²
  3
  0 0
  1 1
  2 4
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|---|---|---|
| 1 | Create `include/Newton.hpp` with class declaration (buildDividedDiff, interpolate, printTable, private diffTable) | 30 min | — |
| 2 | Create `src/Newton.cpp` implementing divided differences and Horner's method | 1.5 hr | 1 |
| 3 | Create `tests/test_newton.cpp` with linear and quadratic test cases (same as Lagrange tests) | 45 min | 1, 2 |
| 4 | Update `NC/main.cpp` to import Newton.hpp and add Category 2 option 2 in interpolationMenu() | 45 min | 1, 2 |
| 5 | Create `include/Integration.hpp` with static method declarations (trapezoidal, simpsons) | 30 min | — |
| 6 | Create `src/Integration.cpp` implementing composite trapezoidal and Simpson's rules | 1 hr | 5 |
| 7 | Create `tests/test_integration.cpp` with two test cases (∫x² [0,1] and ∫sin(x) [0,π]) | 45 min | 5, 6 |
| 8 | Update `NC/main.cpp` to import Integration.hpp and add Category 4 (Integration) with both methods | 45 min | 5, 6 |
| 9 | Create or update `NC/README.md` documenting input file formats and menu options | 45 min | 1, 5 |
| 10 | Update `spec.md` Feature Index: mark Feature 8 (Newton) as ✅ Complete, add Feature 9 (Integration) as [ ] | 15 min | 2, 6 |
| 11 | Integrate all tests into `make test` target in Makefile | 30 min | 3, 7 |
| 12 | Verify `make test` passes all 6 test suites (Matrix, EigenValue, Interpolation, LeastSquares, Newton, Integration) and exits 0 | 30 min | 11 |

**Total estimated time: 8.5 hours**

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|
| Newton divided differences suffer numerical instability for unevenly spaced or closely clustered data | Med | Med | Test on evenly spaced data (linear, quadratic) first; document limitation in code; offer printTable() for inspection |
| Horner's method implementation has indexing error or sign flip | Med | High | Manually trace through 2-point (linear) case step-by-step; verify against hand calculation |
| Simpson's rule requires n to be even; user passes odd n | Low | Med | Add assertion in Integration.cpp that n is even; if odd, either round up or throw exception |
| Integration test functions (x², sin) use hardcoded tolerance; may fail on different compiler optimizations | Low | Med | Use slightly loose tolerance (ε = 1e−3) for integration tests; document that numerical integration is inherently approximate |
| Menu option numbering conflicts if new categories added; Category 4 should be "next available" | Low | Low | Check current main.cpp structure; confirm no Category 4 exists; update clearly in code |
| Newton interpolation inherits data storage from Interpolation but doesn't use matrix properties; confusing design | Low | Low | Document inheritance choice in Newton.hpp comments; note that Interpolation as a base is the established pattern |

---

## 7. Success criteria

The plan is **complete** when:

1. **§2 Goal 1–3:** Newton.hpp/cpp exist, implement buildDividedDiff(), interpolate(), printTable(). Both are integrated into main.cpp as Category 2 option 2.

2. **§2 Goal 4:** NC/README.md documents the interpolation input file format (x, f(x) pairs) clearly.

3. **§2 Goal 5:** tests/test_newton.cpp exists, compiles cleanly, and both test cases pass (linear and quadratic within 1e−10 error). Results match Lagrange interpolation exactly.

4. **§2 Goal 6–8:** Integration.hpp/cpp exist, implement trapezoidal() and simpsons() correctly. Both are integrated into main.cpp as a new Category 4.

5. **§2 Goal 9:** tests/test_integration.cpp exists, compiles cleanly, and both test cases pass:
   - Trapezoidal rule: ∫₀¹ x² dx within 1e−3 of 0.333
   - Simpson's rule: ∫₀^π sin(x) dx within 1e−6 of 2.0

6. **§2 Goal 10–11:** spec.md Feature Index is updated. Feature 8 marked ✅ Complete, Feature 9 added as [ ]. All 6 test suites integrated into `make test`.

7. **§2 Goal 12:** `make test` runs all six test suites (Matrix, EigenValue, Interpolation, LeastSquares, Newton, Integration) and exits 0 with clear output:
```
=== All test suites passed! ===
Exit code 0.
```

**Verification:** On clean Linux/WSL build:
```bash
make clean && make && make test
```
Must produce zero warnings, zero build errors, and all tests passing.

---

## 8. References

- [spec.md](../../spec.md) — Project specification; Feature Index (Features 8, 9)
- [rules.md](../rules.md) — Plan and report authoring conventions
- [Interpolation.hpp](../../NC/include/Interpolation.hpp) — Base class for Newton
- [Lagrange.hpp/cpp](../../NC/include/Lagrange.hpp) — Reference implementation (same inheritance pattern)
- [main.cpp](../../NC/main.cpp) — Console menu; will be updated to add Newton and Integration categories
- [Makefile](../../NC/Makefile) — Build configuration; test target will be extended
- [PLAN-002](../plans/PLAN-002_matrix_hardening.md) — Matrix and SLE test suite
- [PLAN-003](../plans/PLAN-003_eigen_interp_validation.md) — EigenValue, Interpolation, LeastSquares tests

---

## Appendix A: Newton Divided Differences — Hand Calculation Example

**Data Points:** (0, 0), (1, 1), (2, 4) — on y = x²

**Divided Difference Table:**

```
x     f(x)    f[·,·]    f[·,·,·]
0      0
            1−0 = 1
1      1              4−1 = 3/2 − 1 = 1/2
            4−1 = 3
2      4

```

**Newton Polynomial:**
```
P(x) = 0 + 1·(x−0) + (1/2)·(x−0)(x−1)
     = x + (1/2)(x² − x)
     = (1/2)x² + (1/2)x
```

Evaluate at x = 2.5:
```
P(2.5) = 2.5 + (1/2)·(2.5−0)·(2.5−1)
       = 2.5 + (1/2)·2.5·1.5
       = 2.5 + 1.875
       = 4.375

(Error vs x² = 6.25: 6.25 − 4.375 = 1.875 — but this is quadratic on 3 points, so should be exact!)
```

Recalculation (careful with formula):
```
P(x) = f[0] + f[0,1]·(x−0) + f[0,1,2]·(x−0)(x−1)

f[0] = 0
f[0,1] = (f[1]−f[0])/(1−0) = (1−0)/1 = 1
f[0,1,2] = (f[1,2]−f[0,1])/(2−0)

f[1,2] = (f[2]−f[1])/(2−1) = (4−1)/1 = 3

f[0,1,2] = (3−1)/(2−0) = 2/2 = 1

P(x) = 0 + 1·(x−0) + 1·(x−0)(x−1)
     = x + x(x−1)
     = x + x² − x
     = x²  ✓ Exact!

P(2.5) = 2.5² = 6.25 ✓
```

---

## Appendix B: Integration Test Functions

### B.1 Trapezoidal Rule Validation

**Function:** f(x) = x²
**Interval:** [0, 1]
**Exact integral:** ∫₀¹ x² dx = [x³/3]₀¹ = 1/3 ≈ 0.333333...

**Test with n = 4 subintervals:**
```
h = 1/4 = 0.25
x: 0, 0.25, 0.5, 0.75, 1
f: 0, 0.0625, 0.25, 0.5625, 1

I ≈ (h/2) · [f(0) + 2·f(0.25) + 2·f(0.5) + 2·f(0.75) + f(1)]
  = (0.25/2) · [0 + 2·0.0625 + 2·0.25 + 2·0.5625 + 1]
  = 0.125 · [0 + 0.125 + 0.5 + 1.125 + 1]
  = 0.125 · 2.75
  = 0.34375

Error: 0.34375 − 1/3 ≈ 0.01 (acceptable)
```

### B.2 Simpson's Rule Validation

**Function:** f(x) = sin(x)
**Interval:** [0, π]
**Exact integral:** ∫₀^π sin(x) dx = [−cos(x)]₀^π = −cos(π) − (−cos(0)) = −(−1) − (−1) = 1 + 1 = 2

**Test with n = 4 subintervals:**
```
h = π/4 ≈ 0.7854
x: 0, π/4, π/2, 3π/4, π
f: 0, sin(π/4)≈0.707, 1, sin(3π/4)≈0.707, 0

I ≈ (h/3) · [f(0) + 4·f(π/4) + 2·f(π/2) + 4·f(3π/4) + f(π)]
  = (π/(4·3)) · [0 + 4·0.707 + 2·1 + 4·0.707 + 0]
  = (π/12) · [2.828 + 2 + 2.828]
  ≈ 0.2618 · 7.656
  ≈ 2.005

Error: 2.005 − 2 ≈ 0.005 (excellent!)
```

Expected test results:
- Trapezoidal: within 1e−3 of 0.333
- Simpson's: within 1e−6 of 2.0
