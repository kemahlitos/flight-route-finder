# Flight Route Finder (C++)

A flight route planning system implemented in **C++**, built on top of core
data structures and graph algorithms.

The project models airline routes using a **directed multigraph** and computes
optimal paths using **Dijkstra’s shortest path algorithm**, enhanced with a
custom **hash table–based caching layer** for performance.

---

## Project Overview

The system represents airports as nodes and flights as directed edges.
Multiple flights (parallel edges) between the same airport pair are supported,
each with its own airline identifier and two separate weights.

For a given source and destination airport, the system computes the most suitable
route based on a weighted combination of **cost** and **time**, controlled by a
user-defined parameter.

---

## Core Features

### Directed MultiGraph
- Supports **parallel edges** between the same nodes
- Each edge stores:
  - Ticket cost
  - Flight duration
  - Airline identifier
- Implemented using an adjacency-list–style structure customized for multigraph behavior

---

### Shortest Path Computation
- Routes are computed using **Dijkstra’s algorithm**
- A tunable parameter `alpha` controls the trade-off between:
  - Minimizing cost
  - Minimizing time
- Enables flexible route selection under different optimization preferences

---

### Flight Availability Control
To simulate real-world constraints:
- Flights can be temporarily **halted** (disabled)
- Halted flights can later be **re-enabled**
- Route computation dynamically respects current flight availability

---

### Caching with Custom Hash Table
To reduce repeated computation:
- A custom **templated hash table** is used to cache route queries
- Collision handling via **quadratic probing**
- Cache eviction follows an **LRU (Least Recently Used)** policy when capacity is exceeded

This layer demonstrates practical performance optimization using data structures.

---

## Implementation Highlights

- Written in standard **C++**
- Manual memory management and pointer-safe design
- Clear separation between:
  - Graph structure
  - Routing logic
  - Caching mechanism
- Exception-based error handling for invalid operations

---

## Project Structure

- `MultiGraph.cpp` — directed multigraph implementation
- `HashTableImpl.h` — custom hash table with quadratic probing and LRU eviction
- `CENGFlight.cpp` — high-level flight routing logic and system operations
- `Exceptions.h`, `IntPair.h` — supporting utilities
- `main.cpp` — entry point (used for testing and integration)

---

## Notes on Authorship

This project was originally provided as a structured programming assignment
specification.

All core logic, data structure implementations, and system behavior were
implemented by me by completing the missing components and integrating the
overall design into a working flight route finder.

Test data and the original evaluation environment are not included.
