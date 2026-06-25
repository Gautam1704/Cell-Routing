#ifndef MINHEAP_H
#define MINHEAP_H

// Array-based binary min-heap used as Dijkstra's priority queue.
// Stores (tower, distance) pairs, ordered by smallest distance. No STL.

class MinHeap {
private:
    struct Item {
        int tower;
        int dist;
    };

    Item* data;
    int count;
    int capacity;

    void grow() {
        int newCap = capacity * 2;
        Item* newData = new Item[newCap];
        for (int i = 0; i < count; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

    void swap(int i, int j) {
        Item tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[i].dist < data[parent].dist) {
                swap(i, parent);
                i = parent;
            } else {
                break;
            }
        }
    }

    void siftDown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < count && data[left].dist < data[smallest].dist)
                smallest = left;
            if (right < count && data[right].dist < data[smallest].dist)
                smallest = right;
            if (smallest == i) break;
            swap(i, smallest);
            i = smallest;
        }
    }

public:
    MinHeap(int initialCapacity = 16)
        : count(0), capacity(initialCapacity) {
        data = new Item[capacity];
    }

    ~MinHeap() {
        delete[] data;
    }

    bool isEmpty() const { return count == 0; }

    void push(int tower, int dist) {
        if (count == capacity) grow();
        data[count].tower = tower;
        data[count].dist = dist;
        ++count;
        siftUp(count - 1);
    }

    // Remove and return the (tower, dist) with smallest dist.
    void pop(int& tower, int& dist) {
        tower = data[0].tower;
        dist = data[0].dist;
        data[0] = data[count - 1];
        --count;
        if (count > 0) siftDown(0);
    }
};

#endif
