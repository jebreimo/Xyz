//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-01-31.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <functional>
#include <iostream>
#include <string>
#include <strings.h>
#include <unordered_map>

#include "linenoise/linenoise.h"
#include "Operations.hpp"
#include "ParserTools/DelimiterFinders.hpp"
#include "ParserTools/StringTokenizer.hpp"

void completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
    }
}

char *hints(const char *buf, int *color, int *bold) {
    if (!strcasecmp(buf,"hello")) {
        *color = 35;
        *bold = 0;
        return const_cast<char*>(" World");
    }
    return nullptr;
}

std::unordered_map<std::string, std::function<bool(std::vector<ValueType>&)>> commands = {
    {"+", addition},
    {"-", subtraction},
    {"*", multiplication},
    {"dot", inner_product},
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
    {"dup", duplicate},
};

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
    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(hints);
    linenoiseHistoryLoad("history.txt");

    std::vector<ValueType> stack;

    while (true)
    {
        char *line = linenoise("xyz-calc> ");
        if (line == nullptr)
            break;
        if (line[0] != '\0') {
            parse_commands(line, stack);
            linenoiseHistoryAdd(line);
            linenoiseHistorySave("history.txt");
        }
        free(line);
    }
    return 0;
}
