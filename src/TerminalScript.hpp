#ifndef PHBSCRIPT
#define PHBSCRIPT
#include "Core.hpp"
#include "IOSystem.hpp"
#include "Utils.hpp"
#include "IOCursor.hpp"
#include "Editor.hpp"

class _parser {
private:
    
    vector<wstring> lines;

    int pointer;
    int ms_delay;
public:
    _parser(){
        addVar(L"io",L"NONE"); //for io stream
    }
    ~_parser() {
       
    }
    void delay(int ms) {
        ms_delay = ms;
    }

    void runScript(String s) {
        wstring t = readFile(s.toAnsiString().c_str());
        lines.push_back(L"empty"); //This for fucking encoding bug
        for (int i = 0; i < Split(t, L"\n").size(); i++) {
            lines.push_back(Split(t, L"\n")[i]);
        }
    }
    void eval(wstring s) { 
        for (int i = Split(s, L";").size()-1; i >=0; i--)
        lines.insert(lines.begin() + pointer+1, Split(s, L";")[i]);
        lines.insert(lines.begin() + pointer + 1+ Split(s, L";").size(), "-d "+ inttostr(Split(s, L";").size()));
    }
    void showFile() {
        for (int i = 0; i < lines.size(); i++)
       cout << String(lines[i]).toAnsiString() << '\n';
    }
    void showVars() {
        for (int i = 0; i < vars.size(); i++)
            cout <<  String("Id: "+inttostr(i)+" name: \""+vars[i].name+"\"  value: "+vars[i].value).toAnsiString() << '\n';
    }
    void update(float t) {
        if (!Console.WRITE_MODE && !Console.PRINT_MODE && pointer < lines.size() && ms_delay <=0) {
            if (lines[pointer] == "empty"){}
            if (func(lines[pointer]) == "blink") Console.setBlinkMode(strtoint(arg(lines[pointer], 0)));
            if (func(lines[pointer]) == "$py-") Console.pointer.y-= strtoint(arg(lines[pointer], 0));
            if (func(lines[pointer]) == "$py+") Console.pointer.y+= strtoint(arg(lines[pointer], 0));
            if (func(lines[pointer]) == "$px-") Console.pointer.x-= strtoint(arg(lines[pointer], 0));
            if (func(lines[pointer]) == "$px+") Console.pointer.x+= strtoint(arg(lines[pointer], 0));
            if (func(lines[pointer]) == "$set") Console.write(getArg(lines[pointer]));
            if (func(lines[pointer]) == "print") Console.print(getArg(lines[pointer]));
            if (func(lines[pointer]) == "light") { arg(lines[pointer], 0) == L"on" ? LightOn() : LightOff(); }
            if (func(lines[pointer]) == "cursor") {  arg(lines[pointer], 0) == L"show" ? Console.cursorVisible = true : Console.cursorVisible = false; }
            if (func(lines[pointer]) == "mouse") { arg(lines[pointer], 0) == L"show" ? MouseCursor.mcv = true : MouseCursor.mcv = false; }
            if (func(lines[pointer]) == L"forecolor") Console.foreColor = DOSCOLORS[strtoint(arg(lines[pointer], 0)) % 16];
            if (func(lines[pointer]) == L"backcolor") Console.backColor = DOSCOLORS[strtoint(arg(lines[pointer], 0)) % 16];
            if (func(lines[pointer]) == L"set") setVar(lines[pointer]);
            if (func(lines[pointer]) == "println") Console.println(getArg(lines[pointer]));
            if (func(lines[pointer]) == "center") { Console.center(getArg(lines[pointer])); }
            if (func(lines[pointer]) == "delay")  delay(strtoint(arg(lines[pointer],0)));  
            if (func(lines[pointer]) == "frame") Console.createFrame(strtoint(arg(lines[pointer], 0)),strtoint(arg(lines[pointer], 1)),strtoint(arg(lines[pointer], 2)),strtoint(arg(lines[pointer], 3)),getArg(lines[pointer], 4));
            if (lines[pointer] == "clear")  Console.clear(); 
            if (func(lines[pointer]) == L"var") {  addVar(arg(lines[pointer], 0), getArg(lines[pointer], 1)); }
            if (func(lines[pointer]) == "input") { Console.read(vars[getVar(arg(lines[pointer],0))].value); }
            if (func(lines[pointer]) == "eval") { eval(getArg(lines[pointer])); }
            if (func(lines[pointer]) == "call") { wstring callname = getArg(lines[pointer]);  resetScript(false); runScript(callname); return; }
            if (func(lines[pointer]) == "-d") {for (int i = 0; i < strtoint(arg(lines[pointer],0)); i++) {lines.erase(lines.begin()+pointer-1); pointer--;}lines.erase(lines.begin() + pointer);}
            pointer++;
        }
        else if (ms_delay > 0) ms_delay -= 0.2*t;

    }


    void resetScript(bool c = true) {
         lines.clear();
        pointer = 0;
        if (c) Console.clear();
        Console.WRITE_MODE = false;
        Console.PRINT_MODE = false;
    }
    void resetToScript(String s){
       

        resetScript();
        runScript(s);
    }
};


_parser Parser;



void IOSYSTEM() {
        if (!getIO().size() > 0 || Console.PRINT_MODE) return;
        if (func(getIO()) == L"clear") Console.clear(); else
        if (func(getIO()) == L"help") { Parser.resetScript(false); Parser.runScript("system\\help.pbs"); }else
        if (func(getIO()) == L"clear") Console.clear(); else
        if (func(getIO()) == L"exit") window.close(); else
        if (func(getIO()) == L"cls") Console.clear(); else
        if (func(getIO()) == L"help") { Parser.resetScript(false); Parser.runScript("system\\core\\prompt.pbs"); }else
        if (func(getIO()) == L"forecolor") Console.foreColor = DOSCOLORS[strtoint(arg(getIO(),0)) % 16]; else
        if (func(getIO()) == L"backcolor") Console.backColor = DOSCOLORS[strtoint(arg(getIO(), 0)) % 16]; else
        if (func(getIO()) == L"file") { Parser.resetScript(); Parser.runScript(arg(getIO(),0)); }else
        if (func(getIO()) == L"bfile") { brainfuck("system/scripts/"+ arg(getIO(), 0) +".bf"); }else
        if (func(getIO()) == L"light") { arg(getIO(), 0) == L"on" ? LightOn() : LightOff(); } 
        else   Console.println(L"Неизвестная команда. \n Введите help для просмотра списка команд.", true);
}













#endif