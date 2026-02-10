# Flight Route Finder (C++)

A flight route planning system implemented in **C++**, designed around core
**data structures and graph algorithms** commonly used in large-scale
flight search and route planning systems.

The project models airline routes using a **directed multigraph** and computes
optimal paths using **Dijkstra’s shortest path algorithm**, combined with a
custom-built **hash table cache** for performance optimization.

---

## Project Context

Modern flight search platforms (e.g., Skyscanner-like systems) rely on
graph-based route planning to evaluate multiple route alternatives under
different optimization criteria.

This project focuses on the **algorithmic core** of such systems:
- Modeling flight networks
- Evaluating alternative routes
- Balancing trade-offs between cost and time
- Optimizing repeated route queries through caching

The emphasis is on system logic and performance rather than user interface
or business-layer concerns.

---

## Project Overview

Airports are represented as nodes and flights as directed edges.
Multiple flights (parallel edges) between the same airport pair are supported,
each with its own airline identifier, ticket cost, and flight duration.

Given a source and destination airport, the system computes the most suitable
route by balancing **cost** and **time** using a tunable weighting parameter `alpha`.

---

## Engineering Highlights

### Graph Modeling
- Implemented a **directed multigraph** supporting parallel edges
- Each edge stores two independent weights (cost and duration)
- Adjacency structures correctly handle multiple airlines operating between
  the same airports

### Route Computation
- Implemented **Dijkstra’s algorithm** on top of the multigraph abstraction
- Introduced a weighted objective function controlled by `alpha`
- Enables flexible route optimization under different user preferences

### Flight State Management
- Designed mechanisms to dynamically **halt and resume flights**
- Route computation adapts automatically to availability changes
- Ensures consistency without reconstructing the graph

### Performance Optimization with Caching
- Designed and implemented a custom **templated hash table**
- Collision resolution via **quadratic probing**
- Integrated an **LRU eviction policy**
- Cached route queries to avoid repeated shortest-path computation

---

## What I Implemented

This project was developed based on a predefined problem specification.
I implemented the complete system behavior by designing and integrating:

- The multigraph data structure and its operations
- The shortest path computation logic
- The caching layer and eviction strategy
- Flight availability control mechanisms
- System-level integration of all components

---

## Implementation Notes

- Written in standard **C++**
- Manual memory management with careful pointer usage
- Clear separation between graph, routing, and caching layers
- Exception-based handling of invalid operations

---

## Project Structure

- `MultiGraph.cpp` — directed multigraph implementation
- `HashTableImpl.h` — custom hash table with quadratic probing and LRU eviction
- `CENGFlight.cpp` — high-level flight routing logic and system operations
- `Exceptions.h`, `IntPair.h` — supporting utilities
- `main.cpp` — entry point (used for testing and integration)

---

## Notes

The focus of this repository is on **data structure design, algorithmic reasoning,
and system-level implementation** inspired by real-world flight route planning systems.
