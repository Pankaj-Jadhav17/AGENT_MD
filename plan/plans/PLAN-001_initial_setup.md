# PLAN-001: Initial Setup & Housekeeping

**Created:** 2026-05-18
**Status:** Draft
**Addresses:** Gaps #2, #3 from `plan/current_state_report.md` §6 — missing `.gitignore`, hardcoded input paths, and absence of project-level documentation.

---

## 1. Context & motivation

The Current State Report (`plan/current_state_report.md`) identified several housekeeping issues that, while not blocking core functionality, create friction for development and pose risks for collaboration:

- **Issue #3:** No `.gitignore` exists. Build artefacts (`*.o`, `nc` binary, `output/output.txt`) will be committed to git, polluting the repository and inflating its size.
- **Issue #2:** All input/output file paths in `main.cpp` are hardcoded as relative strings (e.g., `"input/input_A.txt"`). The binary silently fails if invoked from any directory other than `NC/`.
- There is no `README.md` inside `NC/` explaining how to build and run the project. A new contributor or grader has no quick-start guide.
- The `spec.md` Codebase Inventory must be kept current after any file additions or changes.

This plan addresses all of the above in a single, small-scoped session.

---

## 2. Goals

- **G1:** A `.gitignore` file exists at `NC/` root and prevents `*.o`, `nc`, `matrix_app`, and `output/output.txt` from being tracked by git.
- **G2:** `main.cpp` contains no dead or commented-out code blocks. Every line is reachable.
- **G3:** The binary correctly locates `input/` and `output/` regardless of the working directory from which it is invoked, OR provides a clear error message indicating how to fix the issue.
- **G4:** A `NC/README.md` file exists and documents: prerequisites, build steps, run steps, project structure, and input file format.
- **G5:** `AGENT_MD_v1/spec.md` Codebase Inventory table reflects all files added or modified by this plan.

---

## 3. Non-goals

- Adding automated tests — that is a separate plan (PLAN-002 scope).
- Adding CI/CD pipelines — out of scope for this housekeeping plan.
- Refactoring the OOP class hierarchy — the hierarchy is already correct.
- Implementing Newton's Interpolation or any new numerical methods.
- Adding `-O2` optimisation to the Makefile — low priority, deferred.

---

## 4. Approach

### 4.1 `.gitignore`

We create a `.gitignore` file in `NC/` that covers:
- Object files (`*.o`)
- Binary targets (`nc`, `matrix_app`)
- Generated output (`output/output.txt`)
- Editor swap files (`*.swp`, `*~`)

This is a single-file addition with no code changes.

### 4.2 Dead code removal

We audit `main.cpp` for any commented-out blocks, unreachable code, or orphaned functions. The current state report notes `main.cpp` is 384 lines with 4 clean sub-menus. If any dead code is found, we remove it surgically — touching only the dead lines, not reformatting adjacent code.

### 4.3 Hardcoded input paths

Two approaches were considered:

1. **`argv[0]`-based detection:** Use `std::filesystem::path` (C++17) to derive the directory containing the binary, then prepend it to all relative paths. This is the cleanest fix — requires no user action.
2. **`--input-dir` CLI flag:** Accept an optional command-line argument `--input-dir <path>` that overrides the default `"input/"` prefix. Falls back to the current directory if not specified.

**Decision:** We implement approach 1 (auto-detect from `argv[0]`) as the primary mechanism, with a clear error message if the `input/` directory is not found. This avoids burdening the user with CLI flags while still fixing the silent-failure issue. The `<filesystem>` header is available in C++17 which is already our standard.

### 4.4 README.md

We create `NC/README.md` with these sections:
- Project title and one-line description
- Prerequisites (g++, make, Linux/WSL)
- Build instructions (`make`, `make clean`)
- Run instructions (`./nc`)
- Project structure diagram
- Input file format description

### 4.5 spec.md inventory update

After all changes, we update the Codebase Inventory table in `AGENT_MD_v1/spec.md` to add:
- `NC/.gitignore`
- `NC/README.md`

And update `Last Updated` for `NC/main.cpp`.

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Create `NC/.gitignore` with entries for `*.o`, `nc`, `matrix_app`, `output/output.txt`, editor files | 10 min | — |
| 2 | Audit `main.cpp` for dead/commented-out code blocks and remove them | 15 min | — |
| 3 | Refactor `main.cpp` to detect project root via `std::filesystem::path(argv[0]).parent_path()` and prepend to all input/output paths; add error message if `input/` not found | 45 min | 2 |
| 4 | Create `NC/README.md` with build, run, and structure documentation | 20 min | — |
| 5 | Update `AGENT_MD_v1/spec.md` Codebase Inventory: add `.gitignore`, `README.md`; update `main.cpp` last-updated date | 10 min | 1, 2, 3, 4 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| `std::filesystem` not available on older g++ | Low | High | Confirmed: project already requires C++17 and the test build uses `-std=c++17`. `<filesystem>` is part of C++17 standard. Add `-lstdc++fs` to linker flags if needed on older GCC versions. |
| `argv[0]` contains only the binary name (no path) when invoked via `PATH` | Low | Medium | Fall back to current working directory if `parent_path()` is empty. Print a clear warning directing the user to run from `NC/`. |
| Removing "dead code" accidentally removes live code | Low | High | Verify every removal by re-running `make clean && make` and testing the menu before marking task complete. |

---

## 7. Success criteria

- [ ] `NC/.gitignore` exists and `git status` in `NC/` does not show `*.o`, `nc`, or `output/output.txt` as untracked
- [ ] `main.cpp` contains zero commented-out code blocks (verified by grep for `// .*cout` blocks longer than 3 lines)
- [ ] Running `cd / && /full/path/to/NC/nc` either works correctly or prints a clear error message with instructions
- [ ] `NC/README.md` exists and contains build + run instructions
- [ ] `AGENT_MD_v1/spec.md` Codebase Inventory includes `.gitignore` and `README.md` entries
- [ ] `make clean && make` still exits with code 0 and zero warnings after all changes

---

## 8. References

- `AGENT_MD_v1/spec.md` — Feature 0 (Pre-Work), Codebase Inventory
- `AGENT_MD_v1/plan/current_state_report.md` — §3.2 (hardcoded paths), §3.3 (missing `.gitignore`), §6 (issues #2, #3)
- `AGENT_MD_v1/plan/rules.md` — plan authoring conventions
