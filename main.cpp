#include <iostream>
#include <string>
#include "HashMap.h"
#include "Graph.h"
#include "MinHeap.h"
#include "Queue.h"


static const int INF = 1000000000;

static int towerId(const std::string& name) {
    if (name.size() == 1 && name[0] >= 'A' && name[0] <= 'Z')
        return name[0] - 'A';
    return std::stoi(name);
}

static char towerName(int id) {
    return (char)('A' + id);
}

static void printPath(int* previous, int src, int dst) {
    
    int len = 0;
    for (int at = dst; at != -1; at = previous[at]) ++len;
    int* path = new int[len];
    int idx = len - 1;
    for (int at = dst; at != -1; at = previous[at]) path[idx--] = at;
    std::cout << "  Path: ";
    for (int i = 0; i < len; ++i) {
        std::cout << towerName(path[i]);
        if (i + 1 < len) std::cout << " -> ";
    }
    std::cout << "\n";
    delete[] path;
}

static void routeDijkstra(const Graph& net, int src, int dst) {
    int n = net.size();
    int* dist = new int[n];
    int* previous = new int[n];
    bool* done = new bool[n];
    for (int i = 0; i < n; ++i) { dist[i] = INF; previous[i] = -1; done[i] = false; }

    dist[src] = 0;
    MinHeap pq;
    pq.push(src, 0);

    while (!pq.isEmpty()) {
        int u, d;
        pq.pop(u, d);
        if (done[u]) continue;   
        done[u] = true;
        for (Graph::Edge* e = net.neighbours(u); e; e = e->next) {
            int v = e->to;
            int nd = dist[u] + e->weight;
            if (nd < dist[v]) {
                dist[v] = nd;
                previous[v] = u;
                pq.push(v, nd);
            }
        }
    }

    if (dist[dst] == INF) {
        std::cout << "  No route from " << towerName(src)
                  << " to " << towerName(dst) << "\n";
    } else {
        std::cout << "  Connected. Total delay: " << dist[dst] << "\n";
        printPath(previous, src, dst);
    }

    delete[] dist;
    delete[] previous;
    delete[] done;
}

static void routeBFS(const Graph& net, int src, int dst) {
    int n = net.size();
    int* hops = new int[n];
    bool* seen = new bool[n];
    for (int i = 0; i < n; ++i) { hops[i] = -1; seen[i] = false; }

    Queue q;
    q.push(src);
    seen[src] = true;
    hops[src] = 0;

    while (!q.isEmpty()) {
        int u = q.pop();
        for (Graph::Edge* e = net.neighbours(u); e; e = e->next) {
            if (!seen[e->to]) {
                seen[e->to] = true;
                hops[e->to] = hops[u] + 1;
                q.push(e->to);
            }
        }
    }

    if (hops[dst] == -1)
        std::cout << "  No route from " << towerName(src)
                  << " to " << towerName(dst) << "\n";
    else
        std::cout << "  Fewest hops from " << towerName(src)
                  << " to " << towerName(dst) << ": " << hops[dst] << "\n";

    delete[] hops;
    delete[] seen;
}

int main() {
    HashMap registry;
    Graph net;

    std::cout << "CellRoute ready. Commands:\n"
              << "  ADD_TOWER <name>\n"
              << "  ADD_LINK <a> <b> <weight>\n"
              << "  REGISTER <number> <tower>\n"
              << "  MOVE <number> <tower>\n"
              << "  CALL <caller> <receiver>\n"
              << "  HOPS <caller> <receiver>\n"
              << "  QUIT\n\n";

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "ADD_TOWER") {
            std::string name; std::cin >> name;
            net.addTower(towerId(name));
            std::cout << "  Tower " << name << " added\n";
        }
        else if (cmd == "ADD_LINK") {
            std::string a, b; int w;
            std::cin >> a >> b >> w;
            net.addLink(towerId(a), towerId(b), w);
            std::cout << "  Link " << a << "-" << b << " (delay " << w << ")\n";
        }
        else if (cmd == "REGISTER" || cmd == "MOVE") {
            std::string number, tower;
            std::cin >> number >> tower;
            registry.put(number, towerId(tower));
            std::cout << "  " << number << " is now at tower " << tower << "\n";
        }
        else if (cmd == "CALL" || cmd == "HOPS") {
            std::string caller, receiver;
            std::cin >> caller >> receiver;
            int fromTower, toTower;
            if (!registry.get(caller, fromTower)) {
                std::cout << "  Caller " << caller << " not registered\n";
                continue;
            }
            if (!registry.get(receiver, toTower)) {
                std::cout << "  Receiver " << receiver << " not registered\n";
                continue;
            }
            std::cout << "  Caller at tower " << towerName(fromTower)
                      << ", receiver at tower " << towerName(toTower) << "\n";
            if (cmd == "CALL") routeDijkstra(net, fromTower, toTower);
            else               routeBFS(net, fromTower, toTower);
        }
        else if (cmd == "QUIT") {
            break;
        }
        else {
            std::cout << "  Unknown command: " << cmd << "\n";
        }
    }
    return 0;
}
