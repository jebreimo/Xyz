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
#include <memory>
#include <string>
#include <unordered_map>

#include <isocline.h>
#include "Operations.hpp"
#include "ParserTools/DelimiterFinders.hpp"
#include "ParserTools/StringTokenizer.hpp"

struct Data
{
    std::vector<std::unique_ptr<Operation>> operations;
    std::unordered_map<std::string, const Operation*> commands;

    Data()
    {
        operations.emplace_back(std::make_unique<Addition>());
        operations.emplace_back(std::make_unique<Subtraction>());
        operations.emplace_back(std::make_unique<Multiplication>());
        operations.emplace_back(std::make_unique<Division>());
        operations.emplace_back(std::make_unique<Negate>());
        operations.emplace_back(std::make_unique<Power>());
        operations.emplace_back(std::make_unique<SquareRoot>());
        operations.emplace_back(std::make_unique<Sine>());
        operations.emplace_back(std::make_unique<Cosine>());
        operations.emplace_back(std::make_unique<Tangent>());
        operations.emplace_back(std::make_unique<Arcsine>());
        operations.emplace_back(std::make_unique<Arccosine>());
        operations.emplace_back(std::make_unique<Arctangent>());
        operations.emplace_back(std::make_unique<Arctangent2>());
        operations.emplace_back(std::make_unique<Logarithm>());
        operations.emplace_back(std::make_unique<InnerProduct>());
        operations.emplace_back(std::make_unique<CrossProduct>());
        operations.emplace_back(std::make_unique<Vector3>());
        operations.emplace_back(std::make_unique<Vector4>());
        operations.emplace_back(std::make_unique<Matrix3>());
        operations.emplace_back(std::make_unique<Identity3>());
        operations.emplace_back(std::make_unique<Identity4>());
        operations.emplace_back(std::make_unique<Rotate2>());
        operations.emplace_back(std::make_unique<RotateX>());
        operations.emplace_back(std::make_unique<RotateY>());
        operations.emplace_back(std::make_unique<RotateZ>());
        operations.emplace_back(std::make_unique<Translate2>());
        operations.emplace_back(std::make_unique<Translate3>());
        operations.emplace_back(std::make_unique<Scale2>());
        operations.emplace_back(std::make_unique<Scale3>());
        operations.emplace_back(std::make_unique<Transpose>());
        operations.emplace_back(std::make_unique<Invert>());
        operations.emplace_back(std::make_unique<Normalize>());
        operations.emplace_back(std::make_unique<Duplicate>());
        operations.emplace_back(std::make_unique<Swap>());
        operations.emplace_back(std::make_unique<Rotate>());
        operations.emplace_back(std::make_unique<Pop>());
        operations.emplace_back(std::make_unique<Clear>());
        operations.emplace_back(std::make_unique<PrintStack>());
        operations.emplace_back(std::make_unique<Help>(commands));

        for (const auto& op : operations)
            commands.emplace(op->name, op.get());
    }
};

Data data;

void complete_command(ic_completion_env_t* cenv, const char* word)
{
    if (word[0] == '\0')
        return;
    for (const auto& [name, _] : data.commands)
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
        auto cmd_it = data.commands.find(std::string(token));
        if (cmd_it != data.commands.end())
        {
            try
            {
                (*cmd_it->second)(stack);
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
