# Railway Route Optimization System

## Project Overview

A console-based mini-project implemented in **C** that simulates a real-world railway network. The system allows users to model railway stations and routes as a **graph**, and find optimal paths between stations using **Dijkstra's Algorithm** (shortest distance) and **BFS** (minimum stops). All data can be persistently stored and loaded using file handling.

---

## Objectives

1. Model a railway network using standard data structures in C.
2. Compute the shortest path between two stations by distance (Dijkstra).
3. Compute the path with fewest stops between two stations (BFS).
4. Support dynamic operations: add/delete stations and routes.
5. Persist data using file I/O.
6. Demonstrate academic understanding of data structures for viva.

---

## Data Structures Used

| Data Structure | Where Used | Why |
|---|---|---|
| **Graph (Adjacency List)** | Core network representation | Efficiently stores sparse railway networks. Adding stations/routes is O(1). |
| **Linked List** | Each station's neighbor list | Dynamic size—different stations have different numbers of connections. |
| **Array** | `dist[]`, `visited[]`, `parent[]` | Fast O(1) indexed access for per-station information during algorithms. |
| **Min-Heap** | Dijkstra's priority queue | Efficiently extracts the station with minimum tentative distance in O(log n). |
| **Queue** | BFS traversal | Ensures level-by-level exploration—guarantees minimum hop count. |
| **Stack** | Path reconstruction | The `parent[]` array gives reversed path; a stack reverses it back correctly. |

---

## Algorithms Used

### 1. Dijkstra's Algorithm
- **Purpose**: Find the shortest path by *distance* (km).
- **Approach**: Greedy — always expand the nearest unvisited station.
- **Data Structures**: Min-Heap + `dist[]` + `visited[]` + `parent[]`
- **Time Complexity**: `O((V + E) log V)` with a binary min-heap.

### 2. Breadth-First Search (BFS)
- **Purpose**: Find the path with *minimum number of stops*.
- **Approach**: Level-order traversal of the graph.
- **Data Structures**: Queue + `visited[]` + `parent[]`
- **Time Complexity**: `O(V + E)`

### 3. Graph Operations
- **Add Station**: O(1)
- **Add Route** (undirected edge): O(1)
- **Delete Station**: O(V + E)
- **Delete Route**: O(degree of vertex)

---

## Time Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|---|---|---|
| Add Station | O(1) | O(1) |
| Add Route | O(1) | O(1) |
| Delete Station | O(V + E) | O(1) |
| Delete Route | O(degree) | O(1) |
| Display Network | O(V + E) | O(1) |
| Dijkstra | O((V+E) log V) | O(V) |
| BFS | O(V + E) | O(V) |
| Save/Load File | O(V + E) | O(V + E) |

> V = number of stations, E = number of routes

---

## How to Compile and Run

### Prerequisites
- GCC compiler (MinGW on Windows, or GCC on Linux/macOS)

### Compile
```bash
gcc -Wall -Wextra -g src/main.c -o bin/railway.exe
```
Or use the included PowerShell build script:
```powershell
./build.ps1
```

### Run
```bash
./bin/railway.exe
```

---

## Sample Input/Output

```
  Welcome to the Railway Route Optimization System!

╔══════════════════════════════════════════╗
║   RAILWAY ROUTE OPTIMIZATION SYSTEM     ║
╠══════════════════════════════════════════╣
║ 10. Load Demo Data                       ║
...
  Enter choice: 10
[✓] Demo data loaded (7 stations, 11 routes).

  Enter choice: 5
========= RAILWAY NETWORK =========
  [0] Mumbai
       --> Surat (270 km)
       --> Nashik (160 km)
       --> Pune (150 km)
  [1] Pune
  ...

  Enter choice: 6
Enter source station: Mumbai
Enter destination station: Nagpur
===== DIJKSTRA RESULT (Shortest Distance) =====
  Source      : Mumbai
  Destination : Nagpur
  Distance    : 850 km
  Path: Mumbai --> Pune --> Nagpur
================================================
```

---

## Learning Outcomes

After completing this project, a student will understand:

1. How to model real-world problems using **graphs**.
2. The difference between **adjacency matrix** and **adjacency list** (and why lists are preferred for sparse graphs).
3. How **Dijkstra's algorithm** works step-by-step.
4. How **BFS** finds shortest paths by hop count.
5. Practical use of **linked lists**, **queues**, **stacks**, and **heaps** in C.
6. **Dynamic memory management** (`malloc`, `free`) in C.
7. **File I/O** for data persistence in C.
8. **Modular programming** using functions and structures.

---

## References

- **Cormen, T.H. et al.** – *Introduction to Algorithms (CLRS)*, 3rd Ed. — Chapter 24 (Single Source Shortest Paths)
- **Kernighan, B.W. & Ritchie, D.M.** – *The C Programming Language*, 2nd Ed.
- **Reema Thareja** – *Data Structures Using C*, Oxford University Press
- **GeeksForGeeks** – Dijkstra's Algorithm using Min-Heap: https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation/
- **Wikipedia** – Dijkstra's Algorithm: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
