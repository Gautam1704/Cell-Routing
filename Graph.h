#ifndef GRAPH_H
#define GRAPH_H

// Network topology: towers are nodes, links are weighted undirected edges.
// Adjacency list built from hand-rolled linked lists. No STL containers.

class Graph {
public:
    struct Edge {
        int to;        // neighbour tower id
        int weight;    // link delay
        Edge* next;
        Edge(int t, int w) : to(t), weight(w), next(nullptr) {}
    };

private:
    Edge** adj;        // adj[i] = head of edge list for tower i
    int capacity;      // size of the adj array
    int towerCount;    // highest tower id seen + 1

    void grow(int needed) {
        int newCap = capacity;
        while (newCap <= needed) newCap *= 2;
        Edge** newAdj = new Edge*[newCap];
        for (int i = 0; i < newCap; ++i)
            newAdj[i] = (i < capacity) ? adj[i] : nullptr;
        delete[] adj;
        adj = newAdj;
        capacity = newCap;
    }

public:
    Graph(int initialCapacity = 16)
        : capacity(initialCapacity), towerCount(0) {
        adj = new Edge*[capacity];
        for (int i = 0; i < capacity; ++i) adj[i] = nullptr;
    }

    ~Graph() {
        for (int i = 0; i < capacity; ++i) {
            Edge* cur = adj[i];
            while (cur) {
                Edge* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] adj;
    }

    void addTower(int id) {
        if (id >= capacity) grow(id);
        if (id >= towerCount) towerCount = id + 1;
    }

    // Undirected link: push an edge onto both towers' lists.
    void addLink(int a, int b, int weight) {
        addTower(a);
        addTower(b);
        Edge* ea = new Edge(b, weight);
        ea->next = adj[a];
        adj[a] = ea;
        Edge* eb = new Edge(a, weight);
        eb->next = adj[b];
        adj[b] = eb;
    }

    Edge* neighbours(int id) const {
        if (id < 0 || id >= capacity) return nullptr;
        return adj[id];
    }

    int size() const { return towerCount; }
};

#endif
