#pragma once

#include "GlobalVariables.h"
#include "Text.h"

#include <iostream>

std::string MySubStr(size_t l, size_t r, std::string& str) {
    return str.substr(l, r - l + 1);
}

void Compile(const std::string &input) {
    const std::string output = input + ".bit";

    std::vector<char*> code;
    Text text(input.c_str(), code);
    std::ofstream fout(output.c_str());
    int instruction_pointer = 0;

    for (const auto &line : code){
        if (line[0] == ':') {
            std::string label_name;
            int i = 2;
            while (line[i] != '\0') {
                label_name += line[i];
                ++i;
            }
            label_to_num[label_name] = instruction_pointer;
            continue;
        }
        instruction_pointer++;
    }

    instruction_pointer = 0;
    for (const auto &line : code){
        if (line[0] == ':') {
            continue;
        }
        instruction_pointer++;
        int i = 0;
        std::string command;
        for (char symbol = line[0]; symbol != '\0'; symbol = line[++i]) {
            if (symbol == ' ') {
                break;
            }
            command += symbol;
        }
        if (command.empty()) {
            continue;
        }
        if (line[i] == '\0') {
            fout << gv::func_to_num.find(command)->second << std::endl;
            continue;
        }
        std::string parameter;
        for (char symbol = line[++i]; symbol != '\0'; symbol = line[++i]) {
            parameter += symbol;
        }
        if (!command.empty()) {
            if (command == "je" || command == "jmp") {
                fout << gv::func_to_num.find(command)->second << ' ' << label_to_num[parameter] << std::endl;
            } else {
                fout << gv::func_to_num.find(command)->second << ' ' << parameter << std::endl;
            }
        }
    }
}

void Execute(const std::string &input) {
    std::vector<char*> code;
    Text text(input.c_str(), code);

    Memory memory{10};
    std::stack<int> s;
    memory.ip = 0;
    while (memory.ip != code.size()) {
        auto line = code[memory.ip];
        int i = 0;
        std::string command;
        for (char symbol = line[0]; symbol != '\0'; symbol = line[++i]) {
            if (symbol == ' ') {
                break;
            }
            command += symbol;
        }

        memory.ip++;

        if (command.empty()) {
            continue;
        }
        CommandInterface* a = gv::num_to_comm[std::stoi(command)];

        if (line[i] == '\0') {
            ArgsInterface args{};
            a->Execute(s, &memory, &args);
            continue;
        }
        std::string parameter;
        for (char symbol = line[++i]; symbol != '\0'; symbol = line[++i]) {
            parameter += symbol;
        }

        if (a->GetLabel()[0] == 'j') {
            ArgsInterface args;
            args.label_name = parameter;
            a->Execute(s, &memory, &args);
            continue;
        }

        if (parameter[0] == '[') {
            ArgsInterface args(std::stoi(MySubStr(1, parameter.size() - 2, parameter)), true);
            a->Execute(s, &memory, &args);
        } else {
            if (parameter[0] == 'r') {
                ArgsInterface args{std::move(parameter)};
                a->Execute(s, &memory, &args);

            } else {
                ArgsInterface args(std::stoi(parameter), false);
                a->Execute(s, &memory, &args);
            }
        }
    }
}
