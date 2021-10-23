#pragma once
#include "Core.hpp"

const Color DOSCOLORS[16] = { Color::Black,
                             Color::Blue,
                             Color::Green,
                             Color::Cyan,
                             Color(176, 83, 54),
                             Color(245,0,245),
                             Color::Yellow,
                             Color::White,
                             Color(100,100,100),
                             Color(200,245,255),
                             Color(200,255,245),
                             Color(200,255,255),
                             Color(255,230,230),
                             Color(255,230,255),
                             Color(245,245,200),
                             Color(200,200,200) };





vector<wstring> Split(const wstring& str, const wstring& delim)
{
    vector<wstring> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        wstring token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}



wstring arg(wstring l, int num) {
    return (Split(l, L" ")[num + 1].size() == 0) ? L" " : Split(l, L" ")[num + 1];
}




typedef struct {
    String name, value;
}Variable;

vector<Variable> vars;

wstring getValue(wstring n) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i].name == n) return vars[i].value;
    return L"0";
}

wstring getIO() {
  return vars[0].value == ""  ? L" " : vars[0].value;
}

void setValue(wstring n,wstring v) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i].name == n)  vars[i].value = v;
}

int getVar(wstring name) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i].name == name) return i;
    return 0;
}
void addVar(wstring s) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i].name == s) return;
    wstring n, v;
    n = arg(s, 0);
    v = L"NULL";
    if (Split(s, L" ").size() > 2) v = arg(s, 1);
    Variable temp;
    temp.name = n;
    temp.value = v;
    vars.push_back(temp);
}
void addVar(wstring n, wstring v) {
    Variable temp;
    temp.name = n;
    temp.value = v;
    vars.push_back(temp);
}





std::wstring readFile(const char* filename)
{
    std::wifstream wif(String(string(filename)).toAnsiString().c_str());
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}



std::wstring inttostr(int i) {
    std::wostringstream ss;
    ss << i;
    return ss.str();
}
int strtoint(std::wstring s) {
    int num = NULL;
    for (int i = 0; i < s.length(); i++)
        num = num * 10 + s[i] - 0x30;
    return num;
}


double frandom(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

String func(wstring l) {
    if (Split(l, L" ").size() == 0) return L" ";
    return (Split(l, L" ")[0].size() == 0) ? L" " : Split(l, L" ")[0];
}

wstring replace(wstring s,wstring from,wstring to) {
   size_t posn;
   while (std::wstring::npos != (posn = s.find(from)))  
     s.replace(posn, from.length(), to);
   return s;
}



wstring getString(wstring line,int num) {
    wstring res = arg(line,num);
    if(func(line)==L"button") return Split(line, L"\"")[1]; 
    while (arg(line, num)[arg(line, num).size() - 1] != L'\"') { 
        res = res + L' '+ arg(line, num+1);  
        num++;
    }
    res.pop_back();
    res.erase(0, 1);
    return res;
}

wstring getArg(wstring line) {
    if (arg(line, 0)[0] == L'\"')
        return getString(line, 0);
    else
        return getValue(arg(line, 0));
}

wstring getArg(wstring line,int n) {
    if (arg(line, n)[0] == L'\"')
        return getString(line, n);
    else
        return getValue(arg(line, n));
}

string OnOff(bool s) {
    return s ? "ON":"OFF";
}

Glsl::Vec4 light = sf::Glsl::Vec4(0.1, 0.1, 0.1, 1);
void LightOff() {
    light = sf::Glsl::Vec4(0.1, 0.1, 0.1, 1);
}
void LightOn() {
    light = sf::Glsl::Vec4(0.2, 0.3, 0.2, 1);
}
void LightColor(int c) {
    Color cl = DOSCOLORS[c];
    light = sf::Glsl::Vec4(cl.r / 255., cl.g / 255., cl.b / 255., cl.a / 255.);
}

void setVar(wstring s) {
    setValue(arg(s, 0), getArg(s, 1));
}