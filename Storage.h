#ifndef STORAGE_H
#define STORAGE_H

template <typename T>
class Storage {
private:
    T* data;
    int capacity;
    int size;


    void resize() {
        capacity = (capacity == 0) ? 10 : capacity * 2;
        T* newData = new T[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    Storage() : capacity(10), size(0) {
        data = new T[capacity];
    }

    ~Storage() {
        delete[] data;
    }

    Storage(const Storage& other) : capacity(other.capacity), size(other.size) {
        data = new T[capacity];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }

    Storage& operator=(const Storage& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (int i = 0; i < size; i++) data[i] = other.data[i];
        }
        return *this;
    }

    void add(const T& item) {
        if (size >= capacity) resize();
        data[size++] = item;
    }

    bool removeByID(int id) {
        for (int i = 0; i < size; i++) {
            if (data[i].getID() == id) {
                for (int j = i; j < size - 1; j++) {
                    data[j] = data[j + 1];
                }
                size--;
                return true;
            }
        }
        return false;
    }

    T* findByID(int id) {
        for (int i = 0; i < size; i++) {
            if (data[i].getID() == id) return &data[i];
        }
        return nullptr;
    }

    T* getAll() { return data; }
    int getSize() const { return size; }
};

#endif