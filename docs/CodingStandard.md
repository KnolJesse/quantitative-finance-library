# Coding Standards

This document defines the coding conventions used throughout the library. 

The goal is to produce code that is consistent, readable, and maintainable.

## General Principles
- Target C++20.
- Prefer clarity over cleverness.
- Write self-documenting code through descriptive names.
- Keep functions focused on a single responsibility.
- Favor readability over minimizing the number of lines.
- Make invalid states difficult or impossible to represent.
- Refactor incrementally rather than performing large rewrites.
- Prioritize correctness before optimization.

---
## Naming

Avoid abbreviations 

### Types
Use **PascalCase**.

- `BlackScholesPricer`
- `RunningStats`
- `PathGenerator`

### Functions
Use **PascalCase**.

- `ComputeTerms()`
- `Delta()`
- `ComputeTerms()`

### Variables
Use **camelCase**.

- `spot`
- `riskFreeRate`
- `numberOfSteps`

### Data members
Prefix member variables with m_.

- `m_spot`
- `m_volatility`
- `m_randomGenerator`

### Constants
Use constexpr whenever possible.

```
constexpr double Pi = 3.14159265358979323846;
```

---

## Class Design
- Keep interfaces small.
- Prefer composition over inheritance.
- Use inheritance only when modelling a genuine "is-a" relationship.
- Keep helper functions private.
- Public interfaces should remain stable.

### Constructors
Use member initializer lists.

```
Example(double value)
    : m_value(value)
{
}
```

Validate constructor arguments using `assert()`.

```
assert(value > 0.0);
```

### Functions
Mark read-only member functions as `const`.

```
double Price() const;
```

Use `[[nodiscard]]` whenever ignoring a return value would likely indicate a programming error.

```
[[nodiscard]]
double Price() const;
```

Keep functions short and focused.

---

## Formatting

### Function braces
Opening braces for function definitions appear on a new line.

```
double Price() const
{
}
```

### Control statements
Opening braces for control statements appear on a new line.

```
if (condition)
{
}

for (...)
{
}

while (...)
{
}

switch (...)
{
}
```

### Initializer lists
Each member appears on its own line.

```
Example()
    : m_first(first)
    , m_second(second)
    , m_third(third)
{
}
```

### Long expressions
Break long expressions vertically into terms. 

```
return
    firstTerm
    +
    secondTerm
    -
    thirdTerm;
```

---

## Memory Management
- Never use raw `new` or `delete`.
- Prefer automatic storage duration whenever possible.
- Use RAII throughout.
- Use `std::unique_ptr` for exclusive ownership.
- Use `std::shared_ptr` only when shared ownership is required.

---

## Standard Library
- Prefer modern C++ facilities.
- Avoid C-style arrays and manual resource management.

---

## Assertions
Use `assert()` to validate programmer errors and invariants.

Examples include:

- invalid constructor arguments
- unsupported enum values
- impossible execution paths

Do not use `assert()` for normal runtime error handling.

---

## Documentation
Document the public API using Doxygen.

Focus documentation on:

- purpose
- assumptions
- parameters
- return values

Avoid documenting private implementation details unless necessary.

---

## Testing
Every new public feature should be accompanied by unit tests.

Tests should verify:

- correctness
- edge cases
- numerical accuracy
- regression behaviour

Benchmarking belongs in dedicated benchmark executables rather than unit tests.

---

## Numerical Code

Prefer expressing mathematical formulas directly in code.

Use descriptive intermediate variables:

```
const double sigmaSqrtT = m_volatility * std::sqrt(m_maturity);
```

Avoid unnecessary recomputation by caching intermediate quantities when appropriate.

--- 

## Philosophy

The library should favour:

- correctness
- readability
- maintainability
- extensibility

over premature optimization.

Optimizations should be introduced only after correctness has been established and profiling demonstrates a measurable benefit.