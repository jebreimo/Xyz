//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-01-31.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include <isocline.h>
#include "Operations.hpp"
#include "ParserTools/DelimiterFinders.hpp"
#include "ParserTools/StringTokenizer.hpp"

// Defined below the command table, which it lists.
bool help(std::vector<ValueType>& stack);

std::unordered_map<std::string, std::function<bool(std::vector<ValueType>&)>> commands = {
    {"+", addition},
    {"-", subtraction},
    {"*", multiplication},
    {"/", division},
    {"neg", negate},
    {"pow", power},
    {"sqrt", square_root},
    {"sin", sine},
    {"cos", cosine},
    {"tan", tangent},
    {"asin", arcsine},
    {"acos", arccosine},
    {"atan", arctangent},
    {"atan2", arctangent2},
    {"log", logarithm},
    {"dot", inner_product},
    {"cross", cross_product},
    {"vec3", vec3},
    {"vec4", vec4},
    {"mat3", matrix3},
    {"id3", identity3},
    {"id4", identity4},
    {"rot2", rotate2},
    {"rot_x", rotate_x},
    {"rot_y", rotate_y},
    {"rot_z", rotate_z},
    {"trans2", translate2},
    {"trans3", translate3},
    {"scale2", scale2},
    {"scale3", scale3},
    {"transpose", transpose},
    {"inv", invert},
    {"norm", normalize},
    {"dup", duplicate},
    {"swap", swap},
    {"rot", rotate},
    {"pop", pop},
    {"clear", clear},
    {"print", print_stack},
    {"help", help}
};

bool help(std::vector<ValueType>&)
{
    std::vector<std::string_view> names;
    for (const auto& [name, _] : commands)
        names.push_back(name);
    std::ranges::sort(names);

    std::cout << "Commands:";
    size_t column = 9;
    for (const auto& name : names)
    {
        if (column + 1 + name.size() > 78)
        {
            std::cout << "\n   ";
            column = 3;
        }
        std::cout << ' ' << name;
        column += 1 + name.size();
    }
    std::cout << "\n";
    return false;
}

void complete_command(ic_completion_env_t* cenv, const char* word)
{
    if (word[0] == '\0')
        return;
    for (const auto& [name, _] : commands)
    {
        if (ic_starts_with(name.c_str(), word))
            ic_add_completion(cenv, name.c_str());
    }
}

void completer(ic_completion_env_t* cenv, const char* input)
{
    ic_complete_word(cenv, input, complete_command, ic_char_is_nonwhite);
}

void parse_commands(std::string_view line, std::vector<ValueType>& stack)
{
    ParserTools::FindWhitespace find_ws;
    for (auto word : ParserTools::tokenize(line, find_ws))
    {
        auto token = word.string();
        if (token.empty())
            continue;
        auto cmd_it = commands.find(std::string(token));
        if (cmd_it != commands.end())
        {
            try
            {
                if (cmd_it->second(stack))
                {
                    print(std::cout, stack.back());
                    std::cout << "\n";
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error executing command '"
                        << token << "': " << e.what() << "\n";
            }
        }
        else
        {
            try
            {
                double value = std::stod(std::string(token));
                stack.emplace_back(value);
            }
            catch (const std::exception&)
            {
                std::cerr << "Unknown command or invalid number: "
                        << token << "\n";
            }
        }
    }
}

int main(int argc, char** argv)
{
    // Loads the history file, and saves to it after every line.
    ic_set_history("history.txt", -1);
    ic_set_default_completer(completer, nullptr);

    std::vector<ValueType> stack;

    while (true)
    {
        // isocline appends the prompt marker "> " and adds non-empty
        // lines to the history.
        char* line = ic_readline("xyz-calc");
        if (line == nullptr)
            break;
        if (line[0] != '\0')
            parse_commands(line, stack);
        ic_free(line);
    }
    return 0;
}
