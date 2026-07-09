# Architecture

## Overview

The Quantitative Finance Library is a modern C++20 library for pricing financial derivatives and computing risk measures.

The library is designed around a clear separation of responsibilities. Each module has a single, well-defined purpose and depends only on the modules below it.

The current architecture is:

```text
Products
    │
    ▼
Payoffs
    │
    ▼
Pricing
    ▲
    │
Models ──────► PathGeneration
    ▲               ▲
    │               │
Random ─────────────┘

Statistics
```

Lower-level modules should never depend on higher-level modules.

---

## Design Principles

The architecture follows several guiding principles.

- Single Responsibility Principle (SRP)
- Composition over inheritance
- Separation of interfaces and implementations
- RAII for resource management
- Modern C++20 throughout
- Incremental refactoring over large rewrites

The goal is to build reusable components that can be combined to create multiple pricing engines and financial models.

---

## Module Overview

### Products

Defines financial products and product-specific types.

Examples include:

- European options

Future additions include:

- Barrier options
- Asian options
- American options
- Digital options

Products describe *what* is being priced.

---

### Payoffs

Contains payoff functions.

Examples:

- `Payoff`
- `EuropeanCallPayoff`
- `EuropeanPutPayoff`

A payoff maps a simulated path to a numerical payoff.

```text
Path
    │
    ▼
Payoff
    │
    ▼
double
```

Pricing engines should depend only on the `Payoff` interface rather than concrete payoff implementations.

---

### Models

Models describe the stochastic evolution of an underlying asset.

Current implementation:

- `GeometricBrownianMotion`

Future models include:

- Local Volatility
- Heston
- SABR

A model is responsible only for evolving the state of the underlying process.

It is **not** responsible for pricing derivatives.

---

### PathGeneration

Generates sample paths from stochastic models.

Current classes:

- `Path`
- `PathGenerator`

A `PathGenerator` combines:

- a stochastic model,
- a random number generator,

to produce simulated paths.

The path generation module is intentionally independent of any pricing algorithm so that it can be reused by multiple numerical methods.

Future users include:

- Monte Carlo pricing
- Longstaff-Schwartz
- Historical simulation
- Stress testing

---

### Pricing

Pricing engines compute the present value of financial products.

Current implementations:

- `BlackScholesPricer`
- `MonteCarloPricer`

Pricing engines combine:

- market data,
- a payoff,
- a stochastic model (where applicable),

to produce a price.

All pricing engines should expose a consistent public interface whenever practical.

---

### Risk

The Risk module computes sensitivities (Greeks).

Current implementation:

- Analytical Greeks through `BlackScholesPricer`
- Monte Carlo Greeks through `MonteCarloPricer`

Future work may extract common finite-difference infrastructure into reusable components as additional pricing engines are introduced.

---

### Statistics

Contains reusable statistical utilities.

Current implementation:

- `RunningStats`

Statistics should remain independent of pricing and financial concepts.

Future additions include:

- covariance estimators
- online correlation
- confidence intervals
- statistical accumulators

---

### Random

Provides random number generation.

Current implementation: `RandomGenerator`

- wraps `std::mt19936_64`
- provides normally distributed random variates
- supports deterministic seeding
- supports cloning

Future additions should include:

- uniform random variates
- Sobol sequences
- Halton sequences
- Antithetic variates
- Low-discrepancy generators

Random number generation is intentionally separated from stochastic models so that different generators can be reused across pricing engines and numerical methods.

---

## Dependency Rules

Modules should depend only on lower-level modules.

The preferred dependency direction is:

```text
Pricing
    │
    ├── Payoffs
    ├── Models
    ├── PathGeneration
    ├── Statistics
    └── Random
```

Models should never depend on pricing engines.

Payoffs should never depend on pricing engines.

Statistics should never depend on financial concepts.

Keeping lower-level modules independent maximizes reuse.

---

## Public API

Only the contents of `include/qf/` are considered part of the public API.

Implementation details should remain private whenever possible.

Internal helper functions should not be exposed unless they provide reusable functionality.

---

## Error Handling

Constructor arguments are validated using `assert()`.

Assertions are intended to detect programmer errors during development.

Runtime error handling should be introduced only when appropriate for library users.

---

## Numerical Methods

The library currently supports two pricing approaches.

### Analytical

Analytical pricing provides exact closed-form solutions where available.

Current implementation:

- Black-Scholes

---

### Monte Carlo

Monte Carlo pricing estimates expectations through stochastic simulation.

Current implementation includes:

- Geometric Brownian Motion
- Central finite-difference Greeks with common random numbers
- Statistical error estimation

Future work includes:

- Antithetic variates
- Control variates
- Importance sampling
- Stratified sampling
- Quasi-Monte Carlo

---

# Documentation Philosophy

Public interfaces are documented using Doxygen.

Long-form explanations of financial theory, numerical methods and mathematical derivations belong in the `docs/` directory rather than in API documentation.

---

# Future Direction

The long-term goal is to provide a reusable framework supporting multiple products, pricing engines and stochastic models while maintaining a modular architecture.

New functionality should integrate into the existing module structure without requiring major redesigns.
