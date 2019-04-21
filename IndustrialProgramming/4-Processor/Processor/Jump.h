#include "Commands.h"
#include "GlobalVariables.h"

std::map<std::string, int> label_to_num;

class Jump : public CommandInterface {
public:
    std::string GetLabel() override {
        return "jmp";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        auto label_name = args->label_name;
        mem->ip = std::stoi(label_name);
    }
};

class JumpEqual : public CommandInterface {
public:
    std::string GetLabel() override {
        return "je";
    }

    void Execute(std::stack<int>& s, Memory* mem = nullptr, ArgsInterface* args = nullptr) override {
        if (s.size() <= 1) {
            throw std::runtime_error("Stack's size <= 1. Unable to execute JumpEqual command");
        }
        int a = s.top();
        s.pop();
        int b = s.top();
        s.push(a);

        auto label_name = args->label_name;
        if (a == b) {
            mem->ip = std::stoi(label_name);
        }
    }
};
