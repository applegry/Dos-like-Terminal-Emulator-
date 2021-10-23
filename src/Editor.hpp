#pragma once
#include "Core.hpp"
#include "Utils.hpp"
#include "IOSystem.hpp"

using namespace std;

static String cpu[30000];

void brainfuck(string fname) {
    memset(cpu,0,30000*sizeof(String));
    vector<char> acc;
    char ch;
    ifstream infile(fname);
    while (infile) {
        infile.get(ch);
        acc.push_back(ch);
    }
    infile.close();
    unsigned int j = 0;
    int brc = 0;
    for (int i = 0; i < acc.size(); ++i) {
        if (acc[i] == '>')
            j++;
        if (acc[i] == '<')
            j--;
        if (acc[i] == '#')
            Console.clear();
        if (acc[i] == '+')
            cpu[j][0]++;
        if (acc[i] == '-')
            cpu[j][0]--;
        if (acc[i] == '.')
            Console.write(String(cpu[j])[0]);
        if (acc[i] == ',')
            Console.read(cpu[j]);
        if (acc[i] == '[') {
            if (!cpu[j][0]) {
                ++brc;
                while (brc) {
                    ++i;
                    if (acc[i] == '[')
                        ++brc;
                    if (acc[i] == ']')
                        --brc;
                }
            }
            else
                continue;
        }
        else if (acc[i] == ']') {
            if (!cpu[j][0])
                continue;
            else {
                if (acc[i] == ']')
                    brc++;
                while (brc) {
                    --i;
                    if (acc[i] == '[')
                        brc--;
                    if (acc[i] == ']')
                        brc++;
                }
                --i;
            }
        }
    }
}