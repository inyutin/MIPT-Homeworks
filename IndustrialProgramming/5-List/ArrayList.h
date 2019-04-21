#pragma once
#include <iostream>

template <typename T>
class ArrayList {
public:
    explicit ArrayList(const size_t capacity = 64) {
        _capacity = capacity;
        _size = 0;

        _array = new Node[_capacity];
        _head = 1;
        _tail = 1;

        _array[0].next = 2;
    }

    ~ArrayList() {
        delete[] _array;
    }

    void Insert(const T& data) {
        if(_size == 0) {
            _array[_tail].data = data;
        } else {
            size_t index = getFreePos();
            _array[_tail].next = index;
            _array[index].data = data;
            _tail = index;
        }
        _size += 1;
    }

    T Get(const size_t pos) const {
        size_t tmp = _head;
        for (int i = 0; i < pos; i++) {
            tmp = _array[tmp].next;
        }
        return _array[tmp].data;
    }

    T Remove() {
        T tmpData = _array[_head].data;
        size_t tmpIndex = _array[_head].next;
        _array[_head].next = _array[FREE].next;
        _array[FREE].next = _head;
        _head = tmpIndex;
        _size--;
        return tmpData;
    }

    size_t Size() const {
        return _size;
    }

    size_t Capacity() const {
        return _capacity;
    }
private:
    const static size_t FREE = 0;

    struct Node {
        T data;
        size_t next;

        Node() {
            data = NULL;
            next = FREE;
        }
        explicit Node(const T& _data, const size_t _prev = FREE, const size_t _next = FREE) :
                data(_data), next(_next) {}
    };

    Node* _array;
    size_t _head;
    size_t _tail;

    size_t _capacity;
    size_t _size;

    size_t getFreePos() {
        if (_size == _capacity-1) {     // Расширение массива
            Node* temp = new Node[_capacity*2];
            std::copy(_array, _array + _capacity, temp);
            delete[] _array;
            _array = temp;
            _capacity*=2;
        }

        size_t index = _array[FREE].next;
        if (_array[index].next == FREE) {
            _array[FREE].next = index + 1;
        } else {
            _array[FREE].next = _array[index].next;
        }
        return index;
    };
};