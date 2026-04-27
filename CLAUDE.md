# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

Xyz is a header-only-style C++ library (C++20) for 3D geometry and linear algebra. Almost all functionality lives in `include/Xyz/` as template headers; only three files (`Face.cpp`, `IntersectionType.cpp`, `RandomNumberGenerator.cpp`) have separate `.cpp` translation units.

## Build

```bash
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

The library is consumed via `Xyz::Xyz` (CMake alias). It can also be embedded in another project's build tree — `XYZ_MASTER_PROJECT` controls whether tests and extras are built by default.

## Tests

Tests use Catch2 (fetched automatically via `FetchContent` at configure time).

```bash
# Build and run all tests
cmake --build cmake-build-debug --target CatchXyzTest
cd cmake-build-debug && ctest

# Run a single test file's cases (Catch2 syntax)
./cmake-build-debug/tests/CatchXyzTest/CatchXyzTest "[vector]"
```

Test sources live in `tests/CatchXyzTest/`, one file per header under test (e.g., `test_Vector.cpp`, `test_Matrix.cpp`).

## Architecture

### Core types

- `Vector<T, N>` — fixed-size vector backed by `std::array<T, N>`. All arithmetic is free functions (not member operators).
- `Matrix<T, M, N>` — row-major matrix, indexed via `RowCol{row, col}`. Specialisations exist for 2×2, 3×3, and 4×4.
- Most geometric primitives (`Line`, `LineSegment`, `Plane`, `Triangle`, `Pgram3`, `OrientedRectangle`, …) are thin structs that hold `Vector` members; algorithms are free functions in the same header.

### Naming conventions

- Types: `PascalCase`
- Free functions and member functions: `snake_case`
- Template parameters: `T` (scalar), `M`/`N` (dimensions)

### Header organisation

`include/Xyz/Xyz.hpp` is the convenience umbrella header. Individual headers are self-contained and can be included directly. Dependencies flow inward: geometric algorithms include `Vector.hpp` / `Matrix.hpp` but not each other (except where geometry genuinely composes, e.g. `LineLineIntersection.hpp` includes `Line.hpp`).

### Extras

`extras/xyz_calc/` is a REPL-style calculator built on top of the library (Unix only — excluded on Windows). It is a standalone executable, not part of the library itself.

### Floating-point conventions

`Constants<T>` (in `Constants.hpp`) provides `PI`, `E`, etc. typed to `T`. Approximate comparisons use `Approx<T>` (in `Approx.hpp`) and `ComplexApprox<T>`. The `clean()` utility (in `Utilities.hpp`) zeros out values below a threshold to suppress floating-point noise.