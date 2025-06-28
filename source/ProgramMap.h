// Created by David Bizon on 28/06/2025.
#pragma once
#include <vector>
#include <string>

struct Bank
{
    int bankNumberMsb{0}; // Most significant byte of the bank number
    int bankNumberLsb{0}; // Least significant byte of the bank number
    std::string bankName; // Name of the bank
    std::vector<std::string> programNames;
};

class ProgramMap
{
public:
    ProgramMap();
    ~ProgramMap();

    static const std::vector<Bank>& getProgramMap();
};
