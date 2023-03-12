#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "Stockpile.h"
#include "Zones.h"

void Stockpile::Log(const std::string& str)
{
    if (debug)
    {
        m_AshitaCore->GetChatManager()->Write(0, true, str.c_str());
    }
}

void Stockpile::QueueCommand(int32_t mode, const std::string& str) 
{
    m_AshitaCore->GetChatManager()->QueueCommand(mode, str.c_str());
    m_AshitaCore->GetChatManager()->Write(0, true, str.c_str());
}

int Stockpile::RandomFV(int factor, int var) {
    int min = factor - var;
    int max = factor + var;
    return rand() % ((max - min) + 1) + min;
}

int Stockpile::RandomA(int factor) {
    int min = factor - (int)(factor * 0.2F);
    int max = factor + (int)(factor * 0.2F);
    return Random(min, max);
}

int Stockpile::Random(int min, int max) {
    return rand() % ((max - min) + 1) + min;
}

void Stockpile::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

void Stockpile::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

void Stockpile::trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

bool Stockpile::contains_find(std::vector<std::string> vec, std::string search)
{
    return std::find(vec.begin(), vec.end(), search) != vec.end();
}

bool Stockpile::contains_search(std::vector<std::string> vec, std::string search)
{
    for (std::string v : vec)
    {
        if (search.find(v) != std::string::npos)
        {
            return true;
        }
    }

    return false;
}

float Stockpile::distance(Pos p1, Pos p2)
{
    return float(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2) * 1.0));
}

#endif // HELPERS_H_INCLUDED