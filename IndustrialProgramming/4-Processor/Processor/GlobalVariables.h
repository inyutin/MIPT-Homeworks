#pragma once
#include <map>
#include <vector>

#include "Commands.h"
#include "Jump.h"

namespace gv {

    Push push;
    CommandInterface* push_based = &push;

    Pop pop;
    CommandInterface* pop_based = &pop;

    Add add;
    CommandInterface* add_based = &add;

    Sub sub;
    CommandInterface* sub_based = &sub;

    Mul mul;
    CommandInterface* mul_based = &mul;

    Div div;
    CommandInterface* div_based = &div;

    Sin sin;
    CommandInterface* sin_based = &sin;

    Cos cos;
    CommandInterface* cos_based = &cos;

    Sqrt sqrt;
    CommandInterface* sqrt_based = &sqrt;

    In in;
    CommandInterface* in_based = &in;

    Out out;
    CommandInterface* out_based = &out;

    Jump jmp;
    CommandInterface* jmp_based = &jmp;

    JumpEqual je;
    CommandInterface* je_based = &je;

    const std::map<std::string, int> func_to_num = {
            {"push", 0},
            {"pop", 1},
            {"add", 2},
            {"sub", 3},
            {"mul", 4},
            {"div", 5},
            {"sin", 6},
            {"cos", 7},
            {"sqrt", 8},
            {"in", 9},
            {"out", 10},
            {"jmp", 11},
            {"je", 12}
    };

    const std::vector<CommandInterface*> num_to_comm = {
            push_based,
            pop_based,
            add_based,
            sub_based,
            mul_based,
            div_based,
            sin_based,
            cos_based,
            sqrt_based,
            in_based,
            out_based,
            jmp_based,
            je_based
    };
}