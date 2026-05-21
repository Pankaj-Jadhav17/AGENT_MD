# NC — Numerical Computation Toolbox — Project Specification

> **Version:** 1.0
> **Created:** 2026-05-18
> **Last Updated:** 2026-05-18
> **Status:** 🟡 In Planning
>
> Living document — the AI agent updates **Codebase Inventory**, **Decision Log**,
> and **Current Focus** at the end of every working session. Do not edit those
> sections manually.

---

## Current Focus
<!-- The agent updates this at the start of each session. -->
<!-- Humans: glance here to see what is actively being worked on. -->

- Implementing proper OOP hierarchy (SLE inherits Matrix, Interpolation inherits Matrix, CurveFitting inherits Matrix, LeastSquares inherits CurveFitting).

---

## ⚠️ Critical Pre-Work (Do Before Any Feature Work)
<!-- Delete this section once all items are complete. -->

- [x] Initialise git repo with proper `.gitignore` (ignore `*.o`, `output/output.txt`, build artefacts)
- [x] Verify `make` builds cleanly on a fresh Linux/WSL environment with `g++ -std=c++17`
- [x] Create `input/` and `output/` directories with sample data files committed

---

## Project Overview

NC (Numerical Computation Toolbox) is an academic C++ library that implements classical numerical methods from scratch. It covers matrix operations, systems of linear equations, eigenvalue computation, polynomial interpolation, and least-squares fitting — all without external dependencies. The project serves as both a learning tool and a reference implementation for university-level numerical analysis coursework.

The toolbox is packaged as an interactive, menu-driven console application. Users select a category (Matrix Ops, Interpolation, or Least Squares), then choose a specific method. Input is read from text files in the `input/` directory, and results are written to both `stdout` and `output/output.txt`.

### Problem Statement

Students studying numerical methods need a hands-on, self-contained C++ implementation of core algorithms — Gaussian elimination, LU decomposition, eigenvalue solvers, interpolation, and curve fitting — that they can read, run, modify, and extend without wrestling with third-party library setup or opaque APIs. NC fills that gap with clean, from-scratch implementations that map directly to textbook algorithms.

### Target Users

- **Students** in numerical analysis or scientific computing courses who need working C++ implementations to study, test, and extend.
- **Instructors** who want a reference codebase for assignments or demonstrations.
- **Self-learners** exploring numerical methods in C++ without the overhead of libraries like Eigen or LAPACK.

Their primary goals are: understanding how algorithms work internally, verifying hand calculations against code output, and having a foundation to build upon for coursework.

### Current Operational Reality (as of 2026-05-18)

The project has a substantial working codebase with a strong OOP hierarchy:

- **Matrix class** — fully operational with operator overloading, determinant, inverse, transpose, file I/O, and multiple property checks.
- **SLE solvers** — SLE base class inherits from Matrix. Six subclasses implemented (Gauss Elimination, Gauss-Jacobi, Gauss-Seidel, LU Crout, LU Doolittle, LU Cholesky), each with a `solve()` interface.
- **EigenValue** — power method, inverse power method, QR algorithm, Gershgorin bounds, and condition number. Uses Matrix composition.
- **Interpolation** — base class inherits from Matrix (stores data as n×2 matrix). Subclass Lagrange implements interpolation.
- **CurveFitting** — inherits from Matrix (stores data as n×2). Performs polynomial fitting via normal equations.
- **LeastSquares** — inherits from CurveFitting, specialising in linear fits (`y = ax + b`).
- **Console app** — interactive menu-driven `main.cpp` with categories.
- **Planned but incomplete:** Newton's interpolation.

---

## Success Criteria (v1.0)
<!-- How do we know v1.0 is done? Make each criterion testable. -->

- [x] `make` produces a clean build with zero warnings under `g++ -std=c++17 -Wall` on Linux/WSL
- [x] All six SLE solvers return correct solutions for the provided `input_A.txt` / `input_b.txt` test matrices
- [x] `determinant()` and `inverse()` produce correct results for at least 2×2, 3×3, and 4×4 matrices
- [x] Eigenvalue methods (power, inverse power, QR) converge to correct values for symmetric and non-symmetric test matrices
- [x] Lagrange interpolation reproduces exact polynomial values at known data points from `input_interpolation.txt`
- [x] Least-squares linear fit computes correct `a`, `b` coefficients and RMS error for `input_leastsquares.txt`
- [x] All output is mirrored to both `stdout` and `output/output.txt`
- [x] Interactive menu navigates categories without crashes or undefined behaviour
- [ ] Newton's interpolation method is implemented and tested
- [x] CurveFitting module is completed with polynomial fitting support

---

## Constraints & Non-Negotiables

- **Team:** Solo developer (academic project)
- **Timeline:** Semester-bound; core features complete, planned features added incrementally
- **Budget:** Zero — no paid dependencies, services, or infrastructure
- **Compliance:** None (academic project)
- **Non-negotiables:**
  - Pure C++17 — no external libraries (no Eigen, Boost, LAPACK)
  - Must build with `make` (Makefile + `g++ -std=c++17`)
  - Must run on Linux and WSL
  - All output must go to both `stdout` and `output/output.txt`
  - Input data read from text files in `input/`

---

## Tech Stack

### Current (what exists today)

| Layer | Technology | Notes |
|---|---|---|
| Language | C++17 | `g++ -std=c++17` |
| Build System | GNU Make | Single `Makefile` at project root |
| Compiler | g++ (GCC) | Must compile with `-std=c++17` |
| Testing | Manual / console output | No formal test framework yet |
| Platform | Linux / WSL | No Windows-native build support required |
| External Libs | None | All algorithms implemented from scratch |
| I/O | File-based (`input/` → `output/`) | Text format, space/newline delimited |

### Target (what we are building toward)

| Layer | Technology | Notes |
|---|---|---|
| Testing | Manual verification + potential future unit tests | Consider adding a lightweight test harness |
| Documentation | Doxygen-style comments (future) | Not yet implemented |

---

## Architectural Principles
<!-- These are the rules the AI agent follows when making technical decisions. -->

- **From-scratch implementation**: Every numerical algorithm is coded manually — no delegation to external libraries. This is the core pedagogical value of the project.
- **Modular OOP class hierarchy**: Strong use of inheritance for related numerical concepts.
  - `SLE` inherits from `Matrix`
  - `Interpolation` inherits from `Matrix`
  - `CurveFitting` inherits from `Matrix`
  - `LeastSquares` inherits from `CurveFitting`
- **Textbook-faithful**: Implementations should map clearly to textbook descriptions. Variable names and loop structures should be recognisable to someone reading references.
- **Separation of I/O and computation**: Data is loaded from files via inherited methods; computation classes operate on in-memory data; output goes to both console and file. The `main.cpp` menu layer handles user interaction.
- **No over-engineering**: This is an academic tool, not a production library. Prefer clarity over abstraction. Avoid templates or metaprogramming unless they genuinely simplify the code.

---

## Codebase Inventory
<!-- The agent updates this table when files are created, moved, or deleted. -->
<!-- Do not edit manually — let the agent maintain it. -->

| File | Role | Status | Last Updated |
|---|---|---|---|
| `AGENT_MD_v1/spec.md` | Living project specification | ✅ Active | 2026-05-18 |
| `AGENT_MD_v1/plan/rules.md` | AI agent authoring conventions | ✅ Active | 2026-05-18 |
| `NC/main.cpp` | Interactive menu-driven console entry point | ✅ Active | 2026-05-18 |
| `NC/Makefile` | Build configuration | ✅ Active | 2026-05-18 |
| `NC/include/Matrix.hpp` | Matrix class declaration | ✅ Active | 2026-05-18 |
| `NC/src/Matrix.cpp` | Matrix class implementation | ✅ Active | 2026-05-18 |
| `NC/include/SLE.hpp` | SLE base class header (inherits Matrix) | ✅ Active | 2026-05-18 |
| `NC/src/SLE.cpp` | SLE base class implementation | ✅ Active | 2026-05-18 |
| `NC/include/GaussElimination.hpp` | Gauss Elimination SLE solver header | ✅ Active | 2026-05-18 |
| `NC/src/GaussElimination.cpp` | Gauss Elimination SLE solver implementation | ✅ Active | 2026-05-18 |
| `NC/include/GaussJacobi.hpp` | Gauss-Jacobi iterative solver header | ✅ Active | 2026-05-18 |
| `NC/src/GaussJacobi.cpp` | Gauss-Jacobi iterative solver implementation | ✅ Active | 2026-05-18 |
| `NC/include/GaussSeidel.hpp` | Gauss-Seidel iterative solver header | ✅ Active | 2026-05-18 |
| `NC/src/GaussSeidel.cpp` | Gauss-Seidel iterative solver implementation | ✅ Active | 2026-05-18 |
| `NC/include/LUCrout.hpp` | LU Crout decomposition header | ✅ Active | 2026-05-18 |
| `NC/src/LUCrout.cpp` | LU Crout decomposition implementation | ✅ Active | 2026-05-18 |
| `NC/include/LUDoolittle.hpp` | LU Doolittle decomposition header | ✅ Active | 2026-05-18 |
| `NC/src/LUDoolittle.cpp` | LU Doolittle decomposition implementation | ✅ Active | 2026-05-18 |
| `NC/include/LUCholesky.hpp` | LU Cholesky decomposition header | ✅ Active | 2026-05-18 |
| `NC/src/LUCholesky.cpp` | LU Cholesky decomposition implementation | ✅ Active | 2026-05-18 |
| `NC/include/EigenValue.hpp` | Eigenvalue methods header | ✅ Active | 2026-05-18 |
| `NC/src/EigenValue.cpp` | Eigenvalue methods implementation | ✅ Active | 2026-05-18 |
| `NC/include/Interpolation.hpp` | Interpolation base header (inherits Matrix) | ✅ Active | 2026-05-18 |
| `NC/src/Interpolation.cpp` | Interpolation base implementation | ✅ Active | 2026-05-18 |
| `NC/include/Lagrange.hpp` | Lagrange interpolation header | ✅ Active | 2026-05-18 |
| `NC/src/Lagrange.cpp` | Lagrange interpolation implementation | ✅ Active | 2026-05-18 |
| `NC/include/CurveFitting.hpp` | Curve fitting header (inherits Matrix) | ✅ Active | 2026-05-18 |
| `NC/src/CurveFitting.cpp` | Curve fitting implementation | ✅ Active | 2026-05-18 |
| `NC/include/LeastSquares.hpp` | Least-squares fitting header (inherits CurveFitting) | ✅ Active | 2026-05-18 |
| `NC/src/LeastSquares.cpp` | Least-squares fitting implementation | ✅ Active | 2026-05-18 |
| `NC/input/input_A.txt` | Matrix A test data | ✅ Active | 2026-05-18 |
| `NC/input/input_Atilde.txt` | Perturbed matrix A test data | ✅ Active | 2026-05-18 |
| `NC/input/input_b.txt` | Vector b test data | ✅ Active | 2026-05-18 |
| `NC/input/input_btilde.txt` | Perturbed vector b test data | ✅ Active | 2026-05-18 |
| `NC/input/input_interpolation.txt` | Interpolation data points | ✅ Active | 2026-05-18 |
| `NC/input/input_leastsquares.txt` | Least-squares data points | ✅ Active | 2026-05-18 |

---

## Feature Index
<!-- Status: [ ] Not started | 🔄 In progress | ✅ Complete | ⏸ Blocked -->
<!-- Agent updates Status column as work progresses. -->

| # | Feature | Status | Priority | Notes |
|---|---|---|---|---|
| 1 | Matrix Operations | ✅ Complete | P0 | Operators, determinant, inverse, transpose, properties, file I/O |
| 2 | SLE Solvers | ✅ Complete | P0 | 6 methods: Gauss Elim, Jacobi, Seidel, LU Crout/Doolittle/Cholesky |
| 3 | Eigenvalue Computation | ✅ Complete | P0 | Power, inverse power, QR, Gershgorin, condition number |
| 4 | Lagrange Interpolation | ✅ Complete | P0 | `basisPoly()`, `interpolate()` |
| 5 | Least-Squares Linear Fit | ✅ Complete | P0 | Inherits CurveFitting. `linearFit()`, `evaluate()`, `rmsError()` |
| 6 | Interactive Console Menu | ✅ Complete | P0 | Category-based menu in `main.cpp` with TeeBuf for dual output |
| 7 | Curve Fitting (polynomial) | ✅ Complete | P1 | Inherits Matrix. Supports arbitrary degree fitting via normal equations |
| 8 | Newton's Interpolation | [ ] | P1 | Divided differences method — planned feature |

---

## Known Issues & Technical Debt
<!-- Agent appends issues discovered during implementation. Humans can add items too. -->

- No formal test suite — correctness is verified manually via console output comparison.
- No input validation on file formats — malformed input files may cause undefined behaviour.
- Dual output (stdout + file) logic uses a custom `TeeBuf` implementation; verify robust exception safety.

---

## Known Unknowns
<!-- Deliberately undecided. Revisit as the project matures. -->

- Whether to add a lightweight test framework (e.g., Catch2 header-only) or keep manual verification.
- Whether to add numerical integration and ODE solvers in the future as planned, and how they fit into the OOP hierarchy.

---

## Decision Log
<!-- The agent appends one row here at the end of every session. -->
<!-- Do not edit manually — let the agent maintain it. -->

| Date | Decision | Rationale | Alternatives Considered |
|---|---|---|---|
| 2026-05-18 | Created v1.0 spec.md | Project initialisation from brain dump | n/a |
| 2026-05-18 | Refactored OOP hierarchy | Ensure `SLE`, `Interpolation`, and `CurveFitting` inherit from `Matrix`, and `LeastSquares` from `CurveFitting`. | Using composition instead of inheritance (rejected to follow user request). |

---

---

# FEATURE 1 — Matrix Operations

## Goal

Provide a general-purpose `Matrix` class with arithmetic operators, matrix algebra operations (determinant, inverse, transpose), property checks, and file-based I/O. This is the foundational data structure used by all other modules.

## Existing Code to Reference
<!-- Agent: load these files into context at the start of the session for this feature. -->

- `NC/include/Matrix.hpp`
- `NC/src/Matrix.cpp`

## Tasks

- [x] Implement `Matrix` class with dynamic 2D storage
- [x] Overload `+`, `-`, `*`, `==` operators
- [x] Implement `determinant()` (cofactor expansion or row reduction)
- [x] Implement `inverse()` (adjugate or Gauss-Jordan method)
- [x] Implement `transpose()`
- [x] Implement property checks: `isSquare`, `isSymmetric`, `isDiagonal`, `isDiagonallyDominant`, `isNull`
- [x] Implement `readFromFile()` and `displayToFile()`

## Acceptance Criteria

- [x] `Matrix A = B + C` produces correct element-wise addition
- [x] `determinant()` returns correct value for 2×2, 3×3, 4×4 test matrices
- [x] `inverse()` satisfies `A * A.inverse() ≈ I` within floating-point tolerance
- [x] `transpose()` swaps rows and columns correctly
- [x] File round-trip: `readFromFile()` → `displayToFile()` preserves data

---

# FEATURE 2 — Systems of Linear Equations (SLE) Solvers

## Goal

Implement six classical methods for solving `Ax = b`, each as a subclass of a common `SLE` base class inheriting from `Matrix`, with a uniform `solve()` interface returning `vector<double>`.

## Existing Code to Reference

- `NC/include/SLE.hpp` / `NC/src/SLE.cpp`
- `NC/include/GaussElimination.hpp` / `NC/src/GaussElimination.cpp`
- `NC/include/GaussJacobi.hpp` / `NC/src/GaussJacobi.cpp`
- `NC/include/GaussSeidel.hpp` / `NC/src/GaussSeidel.cpp`
- `NC/include/LUCrout.hpp` / `NC/src/LUCrout.cpp`
- `NC/include/LUDoolittle.hpp` / `NC/src/LUDoolittle.cpp`
- `NC/include/LUCholesky.hpp` / `NC/src/LUCholesky.cpp`

## Tasks

- [x] Define `SLE` base class inheriting from `Matrix` with virtual `solve()` method
- [x] Implement Gauss Elimination with back substitution
- [x] Implement Gauss-Jacobi iterative method
- [x] Implement Gauss-Seidel iterative method
- [x] Implement LU Crout decomposition and solve
- [x] Implement LU Doolittle decomposition and solve
- [x] Implement LU Cholesky decomposition and solve (requires symmetric positive-definite A)

## Acceptance Criteria

- [x] All six solvers return the same solution (within tolerance) for the same well-conditioned system
- [x] Iterative methods (Jacobi, Seidel) converge for diagonally dominant matrices
- [x] Cholesky correctly rejects non-symmetric or non-positive-definite input
- [x] Solutions verified against `input_A.txt` / `input_b.txt` reference data

---

# FEATURE 3 — Eigenvalue Computation

## Goal

Compute eigenvalues and related quantities (condition number, Gershgorin bounds) for square matrices using iterative and decomposition methods.

## Existing Code to Reference

- `NC/include/EigenValue.hpp`
- `NC/src/EigenValue.cpp`

## Tasks

- [x] Implement power method for dominant eigenvalue
- [x] Implement inverse power method for smallest eigenvalue
- [x] Implement QR algorithm for full eigenvalue spectrum
- [x] Implement Gershgorin circle bounds
- [x] Implement condition number computation

## Acceptance Criteria

- [x] Power method converges to the dominant eigenvalue of a known test matrix
- [x] Inverse power method converges to the smallest eigenvalue
- [x] QR algorithm produces all eigenvalues for a symmetric test matrix
- [x] Gershgorin bounds correctly contain all eigenvalues
- [x] Condition number matches `‖A‖ · ‖A⁻¹‖` computation

---

# FEATURE 4 — Lagrange Interpolation

## Goal

Construct an interpolating polynomial through given data points using Lagrange basis polynomials, and evaluate it at arbitrary x values.

## Existing Code to Reference

- `NC/include/Lagrange.hpp` / `NC/src/Lagrange.cpp`
- `NC/include/Interpolation.hpp` / `NC/src/Interpolation.cpp`

## Tasks

- [x] Interpolation inherits Matrix and stores data points as n×2 matrix
- [x] Implement `basisPoly(i, x)` — compute the i-th Lagrange basis polynomial at x
- [x] Implement `interpolate(x)` — evaluate the full interpolating polynomial at x
- [x] Read data points from `input/input_interpolation.txt` via parent

## Acceptance Criteria

- [x] `interpolate(xi)` reproduces `fi` exactly at every data point
- [x] Interpolation of a known polynomial (e.g., quadratic through 3 points) matches expected values at intermediate points

---

# FEATURE 5 — Least-Squares Linear Fit

## Goal

Perform a linear least-squares fit (`y = ax + b`) to a set of data points, computing the best-fit coefficients, evaluating the fit, and reporting RMS error. Inherits from CurveFitting.

## Existing Code to Reference

- `NC/include/LeastSquares.hpp` / `NC/src/LeastSquares.cpp`

## Tasks

- [x] Inherit from `CurveFitting`
- [x] Implement `linearFit()` returning `{a, b}` coefficients
- [x] Implement `evaluate(x, a, b)` for prediction
- [x] Implement `rmsError(a, b)` for goodness-of-fit

## Acceptance Criteria

- [x] `linearFit()` produces correct slope and intercept for known test data
- [x] `rmsError()` matches hand-calculated RMS for `input_leastsquares.txt`
- [x] `evaluate()` at each xi approximates fi within expected residual

---

# FEATURE 6 — Interactive Console Menu

## Goal

Provide a user-friendly, menu-driven console interface that organises all numerical methods into logical categories and handles input/output routing.

## Existing Code to Reference

- `NC/main.cpp`

## Tasks

- [x] Implement top-level category selection (Matrix Ops / Interpolation / Least Squares / Curve Fitting)
- [x] Implement sub-menus for each category
- [x] Route input from `input/` files to the selected algorithm
- [x] Mirror all output to both `stdout` and `output/output.txt`

## Acceptance Criteria

- [x] User can navigate to any implemented method via numbered menu choices
- [x] Invalid menu input is handled gracefully (re-prompt, not crash)
- [x] Output file `output/output.txt` matches console output

---

# FEATURE 7 — Curve Fitting (Polynomial)

## Goal

Complete the CurveFitting module to support polynomial curve fitting of arbitrary degree using the normal equations approach.

## Existing Code to Reference

- `NC/include/CurveFitting.hpp` / `NC/src/CurveFitting.cpp`

## Tasks

- [x] Inherit from `Matrix` to store data as n×2 matrix
- [x] Implement polynomial fitting of degree n via normal equations (Vandermonde matrix approach)
- [x] Implement `fit(degree)` returning coefficient vector
- [x] Implement `evaluate(x, coefficients)` for prediction
- [x] Implement `rmsError(coefficients)` for goodness-of-fit
- [x] Add Curve Fitting to the console menu
- [x] Create `input/input_leastsquares.txt` with test data

## Acceptance Criteria

- [x] Degree-1 polynomial fit matches LeastSquares linear fit exactly
- [x] Higher-degree fits reduce RMS error on appropriate datasets
- [x] Coefficient vector is correct for a known polynomial dataset

---

# FEATURE 8 — Newton's Interpolation

## Goal

Implement Newton's divided difference interpolation as an alternative to Lagrange, providing the interpolating polynomial in Newton form.

## Existing Code to Reference

- `NC/include/Interpolation.hpp` / `NC/src/Interpolation.cpp` (base class)

## Tasks

- [ ] Create `NC/include/NewtonInterpolation.hpp` and `NC/src/NewtonInterpolation.cpp`
- [ ] Inherit from `Interpolation`
- [ ] Implement divided difference table construction
- [ ] Implement `interpolate(x)` using Newton's forward/backward difference formula
- [ ] Add Newton's method to the Interpolation sub-menu in `main.cpp`
- [ ] Update `Makefile` to compile the new source file

## Acceptance Criteria

- [ ] Newton and Lagrange methods produce identical interpolated values for the same data points
- [ ] Divided difference table is computed correctly for a known polynomial dataset
- [ ] Method appears in the console menu and runs end-to-end without errors

---

<!-- Agent instruction: add new FEATURE sections above this line as features are identified. -->
