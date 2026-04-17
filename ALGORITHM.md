# Algorithm Documentation – Railway Route Optimization System

---

## Algorithm 1: Add Station

```
Step 1:  Start
Step 2:  If station count >= MAX_STATIONS, print "Limit reached" and Stop
Step 3:  Input station name from user
Step 4:  Search all active stations for a duplicate name
Step 5:  If duplicate found, print "Already exists" and Stop
Step 6:  Assign the name to stations[count]
Step 7:  Set stations[count].active = 1
Step 8:  Set stations[count].head = NULL (empty adjacency list)
Step 9:  Increment count by 1
Step 10: Print confirmation
Step 11: Stop
```

---

## Algorithm 2: Add Route (Undirected Weighted Edge)

```
Step 1:  Start
Step 2:  Input source station name, destination station name, distance (km)
Step 3:  If source not found OR destination not found, print error and Stop
Step 4:  If distance <= 0, print error and Stop
Step 5:  Allocate a new AdjNode n1 {dest=v, weight=dist}
Step 6:  Set n1.next = stations[u].head
Step 7:  Set stations[u].head = n1       (prepend to u's list)
Step 8:  Allocate a new AdjNode n2 {dest=u, weight=dist}
Step 9:  Set n2.next = stations[v].head
Step 10: Set stations[v].head = n2       (prepend to v's list, undirected)
Step 11: Print confirmation
Step 12: Stop
```

---

## Algorithm 3: Delete Station

```
Step 1:  Start
Step 2:  Input station name
Step 3:  If station not found, print error and Stop
Step 4:  Let idx = index of station
Step 5:  Set cur = stations[idx].head
Step 6:  While cur != NULL:
    Step 6a:    nb = cur.dest
    Step 6b:    Traverse stations[nb].head to find edge pointing to idx
    Step 6c:    Remove that edge node from stations[nb]'s list
    Step 6d:    Free cur
    Step 6e:    cur = cur.next
Step 7:  Set stations[idx].head = NULL
Step 8:  Set stations[idx].active = 0
Step 9:  Print confirmation
Step 10: Stop
```

---

## Algorithm 4: Dijkstra's Algorithm (Shortest Path by Distance)

```
Step 1:  Start
Step 2:  Input source station (u) and destination station (v)
Step 3:  If either not found, print error and Stop
Step 4:  Initialize dist[i] = INF for all i; dist[u] = 0
Step 5:  Initialize parent[i] = -1 for all i
Step 6:  Initialize visited[i] = 0 for all i
Step 7:  Create a Min-Heap H; insert (u, dist=0)
Step 8:  While H is not empty:
    Step 8a:    Extract node with minimum dist → call it 'node'
    Step 8b:    If visited[node] == 1, skip (continue)
    Step 8c:    Set visited[node] = 1
    Step 8d:    For each neighbor 'nb' of 'node' with edge weight 'w':
        Step 8d-i:  If NOT visited[nb] AND dist[node] + w < dist[nb]:
                        dist[nb] = dist[node] + w
                        parent[nb] = node
                        Push (nb, dist[nb]) into H
Step 9:  If dist[v] == INF, print "No path found"
Step 10: Else print dist[v] and reconstruct path using printPath(parent, u, v)
Step 11: Stop
```

**Path Reconstruction (using Stack):**
```
Step 1:  Initialize empty stack S
Step 2:  cur = dest (v)
Step 3:  While cur != -1:
    Push cur onto S
    cur = parent[cur]
Step 4:  While S is not empty:
    node = pop from S
    Print stations[node].name
    If S not empty, print " --> "
Step 5:  Stop
```

---

## Algorithm 5: BFS (Minimum Stops / Hops)

```
Step 1:  Start
Step 2:  Input source station (u) and destination station (v)
Step 3:  If either not found, print error and Stop
Step 4:  Initialize visited[i] = 0, parent[i] = -1, stops[i] = INF for all i
Step 5:  Set visited[u] = 1, stops[u] = 0
Step 6:  Create empty Queue Q; enqueue(u)
Step 7:  While Q is not empty:
    Step 7a:    node = dequeue(Q)
    Step 7b:    For each neighbor 'nb' of 'node':
        Step 7b-i:  If NOT visited[nb]:
                        visited[nb] = 1
                        parent[nb] = node
                        stops[nb] = stops[node] + 1
                        If nb == v, GOTO Step 8 (early exit)
                        enqueue(nb)
Step 8:  If stops[v] == INF, print "No path found"
Step 9:  Else print stops[v]-1 intermediate stops, and reconstruct path
Step 10: Stop
```

---

## Algorithm Comparison

| Property | Dijkstra | BFS |
|---|---|---|
| **Optimizes** | Total distance (km) | Number of stops (hops) |
| **Edge Weights** | Uses them | Ignores them |
| **Data Structure** | Min-Heap (Priority Queue) | Queue |
| **Complexity** | O((V+E) log V) | O(V+E) |
| **Guarantee** | Shortest weighted path | Fewest hops path |

---

## Key Definitions

- **V** = Number of stations (vertices)
- **E** = Number of routes (edges)
- **INF** = A very large value (INT_MAX) representing "no path found yet"
- **Relaxation** (Dijkstra): Updating `dist[nb]` if a shorter path is found through `node`
