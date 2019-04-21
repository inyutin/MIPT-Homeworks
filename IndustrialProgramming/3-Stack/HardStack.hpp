#include <limits>
#include <sstream>
#include <iostream>
#include <Errors.hpp>

template<typename T>
class HardStack {
public:
    HardStack() {
        std::signal(SIGINT, signal_handler);
        _size = 0;
        _hashSum = 0;
    }

    virtual ~HardStack() = default;

    virtual void Push(const T& item, int& error) = 0;
    virtual T Pop(int& error) = 0;

    virtual std::string getDump() = 0;
    virtual bool validate(const std::string& from) = 0;

    const size_t Size() {
        return _size;
    }
protected:
    std::hash<T> _hash;
    std::size_t _size;
    size_t _hashSum;
};

template<typename T>
class LinkedHardStack : public HardStack<T> {
public:
    LinkedHardStack() {
        _head = nullptr;
    }

    ~LinkedHardStack() {
        while (_head != nullptr) {
            freeHead();
        }
    }

    void Push(const T& item, int& error) {
        if (validate("Push")) {
            try {
                Node *node = new Node(item, _head);
                _head = node;
                _hashSum += _hash(item);
                _size++;
                if (validate("Push")) {
                    error = 0;
                    return;
                }
            } catch (std::bad_alloc &exc) {
                error = (int) HardStackError::MemoryAllocateFails;
                std::cerr << exc.what();
                return;
            }
        }
        error = (int) HardStackError::NodeInvalidate;

        std::error_code err = HardStackError::NodeInvalidate;
        std::cerr << err << " " << err.message() << std::endl;
    }

    T Pop(int& error) {
        if (validate("Pop")) {
            if (_head == nullptr) {
                error = (int) HardStackError::EmptyStack;

                std::error_code err = HardStackError::EmptyStack;
                std::cerr << err << " " << err.message() << std::endl;
                return NULL;
            }
            T item = _head->_data;
            freeHead();
            _hashSum -= _hash(item);
            _size--;
            if (validate("Pop")) {
                error = 0;
                return item;
            }
        }
        error = (int) HardStackError::NodeInvalidate;

        std::error_code err = HardStackError::NodeInvalidate;
        std::cerr << err << " " << err.message() << std::endl;
        return NULL;
    }

    std::string getDump() {
        std::stringstream output;
        Node *node = _head;
        output << "LinkedHardStack [" << this << "] dump" << std::endl;
        output << "Stack size: " << _size << std::endl;
        output << "Head pointer: " << _head << std::endl;
        while (node != nullptr) {
            output << node->getNodeDump();
            node = node->_next;
        }
        return output.str();
    }

    bool validate(const std::string &from = "Main") {
        Node *node = _head;
        size_t sum = 0;
        while (node != nullptr) {
            if (!node->validate()) {
                std::error_code err = HardStackError::NodeInvalidate;
                std::cerr << "From " << from << " " << err << " " << err.message() << std::endl;
                std::cerr << node->getNodeDump() << std::endl;
                std::cerr << "\n" << std::endl;
                std::cerr << getDump() << std::endl;
            }
            sum += _hash(node->_data);
            node = node->_next;
        }
        if (sum != _hashSum) {
            std::error_code err = HardStackError::HashInvalidate;
            std::cerr << err << " " << err.message() << std::endl;
            std::cerr << getDump() << std::endl;
        }
        return true;
    }

private:
    using HardStack<T>::_size;
    using HardStack<T>::_hash;
    using HardStack<T>::_hashSum;

    struct Node {
        double _leftCuckoo = std::numeric_limits<double>::quiet_NaN();

        T _data;
        Node *_next;

        double _rightCuckoo = std::numeric_limits<double>::quiet_NaN();

        Node(const T& data, Node* next) : _data(data), _next(next) {}

        bool validate() {
            return !(_leftCuckoo == _leftCuckoo || _rightCuckoo == _rightCuckoo);
        }

        std::string getNodeDump() {
            std::stringstream output;
            output << "Data: " << _data << ", " << "Next: " << _next << std::endl;
            return output.str();
        }
    };

    Node *_head;

    void freeHead() {
        Node *tmp = _head->_next;
        free(_head);
        _head = tmp;
    }
};

template<typename T>
class ConstHardStack : public HardStack<T> {
public:
    using HardStack<T>::_size;
    using HardStack<T>::_hash;
    using HardStack<T>::_hashSum;

    ConstHardStack(const size_t capacity, int& error) {
        try {
            array = new T[capacity];
        }
        catch (std::bad_alloc &exc) {
            error = (int) HardStackError::MemoryAllocateFails;
            std::cerr << exc.what();
            return;
        }
        capacity_ = capacity;
    }

    ~ConstHardStack() {
        delete[] array;
    }

    void Push(const T& item, int& error) {
        if (validate("Push")) {
            if (capacity_ == _size) {
                error = (int) HardStackError::FullStack;

                std::error_code err = HardStackError::FullStack;
                std::cerr << err << " " << err.message() << std::endl;
                return;
            }
            array[_size] = item;
            _hashSum += _hash(item);
            _size++;
            if (validate("Push")) {
                error = 0;
                return;
            }
        }
        error = (int) HardStackError::NodeInvalidate;

        std::error_code err = HardStackError::NodeInvalidate;
        std::cerr << err << " " << err.message() << std::endl;
    }

    T Pop(int& error) {
        if (validate("Pop")) {
            if (_size == 0) {
                error = (int) HardStackError::EmptyStack;

                std::error_code err = HardStackError::EmptyStack;
                std::cerr << err << " " << err.message() << std::endl;
                return NULL;
            }
            _size--;
            _hashSum -= _hash(array[_size]);
            if (validate("Pop")) {
                error = 0;
                return array[_size];
            }
        }
        error = (int) HardStackError::NodeInvalidate;

        std::error_code err = HardStackError::NodeInvalidate;
        std::cerr << err << " " << err.message() << std::endl;
        return NULL;
    }

    std::string getDump() {
        std::stringstream output;
        output << "ConstHardStack [" << this << "] dump" << std::endl;
        output << "Stack size: " << _size << std::endl;
        output << "Left cuckoo: " << &_leftCuckoo << std::endl;
        output << "Head pointer: " << array << std::endl;
        for (size_t i = 0; i < _size; i++) {
            output << "Data: " << array[i] << std::endl;
        }
        output << "Right cuckoo: " << &_rightCuckoo << std::endl;
        return output.str();
    };

    bool validate(const std::string& from = "Main") {
        if (_leftCuckoo == _leftCuckoo || _rightCuckoo == _rightCuckoo) {
            std::error_code err = HardStackError::CuckooInvalidate;
            std::cerr << "From " << from << " " << err << " " << err.message() << std::endl;
            std::cerr << getDump() << std::endl;
            return false;
        }
        size_t sum = 0;
        for (size_t i = 0; i < _size; i++) {
            sum += _hash(array[i]);
        }
        if (sum != _hashSum) {
            std::error_code err = HardStackError::HashInvalidate;
            std::cerr << err << " " << err.message() << std::endl;
            std::cerr << getDump() << std::endl;
            return false;
        }
        return true;
    }

private:
    double _leftCuckoo = std::numeric_limits<double>::quiet_NaN();
    T *array;
    double _rightCuckoo = std::numeric_limits<double>::quiet_NaN();
    size_t capacity_;
};
