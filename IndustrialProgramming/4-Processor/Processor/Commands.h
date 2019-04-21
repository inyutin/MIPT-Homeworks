#pragma once

#include <string>
#include <stack>
#include <utility>
#include <math.h>
#include <fstream>

#include "StrSwitch.h"

struct Memory {
    Memory() = default;
    
    explicit Memory(size_t ram_size) {
        ram.assign(ram_size, 666);
    }
    int rax{0};
    int rbx{0};
    int rcx{0};
    int ip{0};
    std::vector<int> ram{};
};

struct ArgsInterface {
    ArgsInterface() = default;
    explicit ArgsInterface(std::string&& s) : register_name(std::move(s)) {}
    ArgsInterface(int value, bool ram) : ram_is_used(ram) {
        if (ram) {
            ram_index = value;
        } else {
            arg = value;
        }
    }
    int arg{0};
    std::string register_name{""};

    std::string label_name{""};

    int ram_index{0};
    bool ram_is_used{false};
};

class CommandInterface {
public:
    virtual std::string GetLabel() = 0;
    virtual void Execute(std::stack<int>&, Memory*, ArgsInterface*) = 0;
};


class Push : public CommandInterface {
public:
    std::string GetLabel() override {
        return "push";
    }

    void Execute(std::stack<int>& s, Memory* mem, ArgsInterface* args) override {
        if (args->ram_is_used) {
            try {
                auto index = args->ram_index;
                s.push(mem->ram[index]);
                return;
            } catch (std::exception& e) {
                std::cerr << "Maybe index out of range" << std::endl;
                std::cerr << e.what() << std::endl;
                exit(1);
            }
        }

        SWITCH(args->register_name)
        {
        CASE(""):
            s.push(args->arg);
            return;
        CASE("rax"):
            s.push(mem->rax);
            return;
        CASE("rbx"):
            s.push(mem->rbx);
            return;
        CASE("rcx"):
            s.push(mem->rcx);
            return;
        DEFAULT:
            throw std::runtime_error("Undefined argument for Push command");
        }
    }
};


class Pop : public CommandInterface {
public:
    std::string GetLabel() override {
        return "pop";
    }

    void Execute(std::stack<int>& s, Memory* mem, ArgsInterface* args) override {
        if (s.empty()) {
            throw std::runtime_error("Stack is empty in Pop command");
        }
        if (args->ram_is_used) {
            try {
                auto index = args->ram_index;
                mem->ram[index] = s.top();
                s.pop();
                return;
            } catch (std::exception& e) {
                std::cerr << "Maybe index out of range" << std::endl;
                std::cerr << e.what() << std::endl;
                exit(1);
            }
        }

        SWITCH(args->register_name)
        {
        CASE(""):
            s.pop();
            return;
        CASE("rax"):
            mem->rax = s.top();
            s.pop();
            return;
        CASE("rbx"):
            mem->rbx = s.top();
            s.pop();
            return;
        CASE("rcx"):
            mem->rcx = s.top();
            s.pop();
            return;
        DEFAULT:
            throw std::runtime_error("Undefined argument for Pop command");
        }
    }
};


class Add : public CommandInterface {
public:
    std::string GetLabel() override {
        return "add";
    }
    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.size() <= 1) {
            throw std::runtime_error("Stack's size <= 1. Unable to execute add command");
        }
        int a = s.top();
        s.pop();
        int b = s.top();
        s.pop();
        s.push(a + b);
    }
};


class Sub : public CommandInterface {
public:
    std::string GetLabel() override {
        return "sub";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.size() <= 1) {
            throw std::runtime_error("Stack's size <= 1. Unable to execute sub command");
        }
        int a = s.top();
        s.pop();
        int b = s.top();
        s.pop();
        s.push(a - b);
    }
};


class Mul : public CommandInterface {
public:
    std::string GetLabel() override {
        return "mul";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.size() <= 1) {
            throw std::runtime_error("Stack's size <= 1. Unable to execute mul command");
        }
        int a = s.top();
        s.pop();
        int b = s.top();
        s.pop();
        s.push(a * b);
    }
};


class Div : public CommandInterface {
public:
    std::string GetLabel() override {
        return "div";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.size() <= 1) {
            throw std::runtime_error("Stack's size <= 1. Unable to execute div command");
        }
        int a = s.top();
        s.pop();
        int b = s.top();
        s.pop();
        if (b == 0) {
            throw std::runtime_error("Dividing by zero");
        }
        s.push(a / b);
    }
};

class Sin : public CommandInterface {
public:
    std::string GetLabel() override {
        return "sin";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.empty()) {
            throw std::runtime_error("Stack is empty. Unable to execute sin command");
        }
        int a = s.top();
        s.pop();
        s.push(static_cast<int>(sin(a) * 1000));
    }
};

class Cos : public CommandInterface {
public:
    std::string GetLabel() override {
        return "cos";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.empty()) {
            throw std::runtime_error("Stack is empty. Unable to execute cos command");
        }
        int a = s.top();
        s.pop();
        s.push(static_cast<int>(cos(a) * 1000));
    }
};


class Sqrt : public CommandInterface {
public:
    std::string GetLabel() override {
        return "sqrt";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.empty()) {
            throw std::runtime_error("Stack is empty. Unable to execute sqrt command");
        }
        int a = s.top();
        s.pop();
        s.push(static_cast<int>(sqrt(a)));
    }
};


class In : public CommandInterface {
public:
    std::string GetLabel() override {
        return "in";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        int value;
        std::cin >> value;
        s.push(value);
    }
};


class Out : public CommandInterface {
public:
    std::string GetLabel() override {
        return "out";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        try {
            std::cout << s.top() << std::endl;
        } catch (std::exception& e) {
            std::cerr << "Stack is empty" << std::endl;
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
};
