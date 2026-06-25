# Cell-Routing

A simulation of how mobile phone networks locate subscribers and route calls in
real time, written from scratch in C++ using custom data structures (no STL
containers for the core logic).

When a call is placed, the network must answer two questions almost instantly:
*where is this subscriber right now*, and *what is the fastest path to reach
them*. Cell-Routing models both, using classic data structures as the building
blocks:

- A **HashMap** (subscriber registry) maps each phone number to its current cell tower.
- A **Graph** (network topology) stores towers as nodes and links as weighted edges.
- A **Min-Heap** (priority queue) powers Dijkstra's shortest-path search.
- A **Queue** powers a BFS hop-count search, for contrast with Dijkstra.

## Build

```
g++ -std=c++17 -o cellroute main.cpp
```

## Run

```
./cellroute
```

Enter commands interactively, or pipe a script in from a file:

```
./cellroute < example.txt
```

## Commands

| Command | Description |
| --- | --- |
| `ADD_TOWER <name>` | Add a tower (single letter A-Z, or a number) |
| `ADD_LINK <a> <b> <weight>` | Add an undirected link between two towers with a delay weight |
| `REGISTER <number> <tower>` | Register a phone number at a tower |
| `MOVE <number> <tower>` | Move an already-registered number to a different tower |
| `CALL <caller> <receiver>` | Route a call using Dijkstra (lowest total delay) |
| `HOPS <caller> <receiver>` | Find fewest hops using BFS (ignores weights) |
| `QUIT` | Exit |

Commands use underscores, e.g. `ADD_TOWER`, not `ADD TOWER`.

## Example session

```
ADD_TOWER A
ADD_TOWER B
ADD_TOWER C
ADD_TOWER D
ADD_TOWER E
ADD_LINK A B 9
ADD_LINK A C 2
ADD_LINK B C 4
ADD_LINK B D 7
ADD_LINK C D 3
ADD_LINK D E 5
ADD_LINK C E 8
REGISTER 9876543210 A
REGISTER 9123456789 E
CALL 9876543210 9123456789
HOPS 9876543210 9123456789
QUIT
```

`CALL` reports the lowest-delay route and its total cost. `HOPS` reports the
fewest number of towers in between. The two can differ, which is the whole
point: the shortest path by hop count is not always the fastest by delay.

## Project structure

```
main.cpp     CLI, Dijkstra routing, BFS routing
HashMap.h    subscriber registry (phone number -> tower)
Graph.h      network topology (adjacency list of weighted edges)
MinHeap.h    array-based binary min-heap (Dijkstra's priority queue)
Queue.h      circular queue (BFS frontier)
```

## How it works

A call flows through five stages:

1. The caller dials a number.
2. The HashMap looks up the receiver's registered tower in O(1) (the locate step).
3. The caller's own tower is identified the same way.
4. Dijkstra runs between the two towers over the weighted graph, using the
   min-heap to always expand the closest tower next.
5. The resulting path and its total delay are reported as the established
   connection.

## Implementation notes

The Dijkstra implementation uses lazy deletion: rather than performing a
decrease-key on the heap, it pushes a fresh entry whenever a shorter distance is
found and skips stale entries when they are popped. This keeps the standard
O((V + E) log V) time complexity without needing a decrease-key operation.

## Complexity

| Operation | Complexity |
| --- | --- |
| Subscriber lookup (HashMap) | O(1) average |
| Add tower / add link (Graph) | O(1) |
| Call routing (Dijkstra) | O((V + E) log V) |
| Hop-count routing (BFS) | O(V + E) |
