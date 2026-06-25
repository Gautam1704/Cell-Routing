#ifndef QUEUE_H
#define QUEUE_H


class Queue {
private:
    int* data;
    int capacity;
    int head;   
    int tail;   
    int count;

    void grow() {
        int newCap = capacity * 2;
        int* newData = new int[newCap];
        for (int i = 0; i < count; ++i)
            newData[i] = data[(head + i) % capacity];
        delete[] data;
        data = newData;
        capacity = newCap;
        head = 0;
        tail = count;
    }

public:
    Queue(int initialCapacity = 16)
        : capacity(initialCapacity), head(0), tail(0), count(0) {
        data = new int[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    bool isEmpty() const { return count == 0; }

    void push(int value) {
        if (count == capacity) grow();
        data[tail] = value;
        tail = (tail + 1) % capacity;
        ++count;
    }

    int pop() {
        int value = data[head];
        head = (head + 1) % capacity;
        --count;
        return value;
    }
};

#endif
