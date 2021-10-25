#pragma once
#include "Core.hpp"
#include "Utils.hpp"
#include "IOSystem.hpp"



class Account {
private:
    String uname, password;
public:
    Account(String u, String p) {
        uname = u;
        password = p;
    }
    String getUsername() { return uname; } //getter and setter for properties
    void setUsername(String username) { uname = username; }
    String getPassword() { return password; }
    void setPassword(String pass) { password = pass; }
};



class AccManagerClass {
private:
    vector<Account> Users;
    String CurrentAcc, CurrentPass;
public:
    AccManagerClass() {}
    void CreateAccount(String u,String p) {
        Users.push_back(Account(u, p));
    }
    void DeleteAcc(String u,String p) {
        if (IsAccExist(u)) {
            if(IsTruePass(u,p)){
            for (int i = 0; i < Users.size(); ++i)
                if (Users[i].getUsername() == u) Users.erase(Users.begin()+i);
            Console.write(L"Аккаунт \"" + u + L"\" удален");
            }else Console.write(L"Введен неверный пароль");
        }else
         Console.write(L"Не существует аккаунта с именем \"" + u + "\"");
    }
    bool IsAccExist(String name) {
        for (int i = 0; i < Users.size(); ++i)
            if (Users[i].getUsername() == name) return true;
            return false;
    }
    bool IsTruePass(String name,String pass) {
        for (int i = 0; i < Users.size(); ++i)
            if (Users[i].getUsername() == name && Users[i].getPassword() == pass) return true;
        return false;
    }
    void LogInAccount(String u, String p) {
        if(IsAccExist(u)){
            if(IsTruePass(u,p)){
        CurrentAcc  = u; 
        CurrentPass = p;
        Console.write(L"Вход выполнен");
            }else
        Console.write(L"Введен неверный пароль");
        }
        else Console.write(L"Не существует аккаунта с именем \"" + u + "\"");
    }
    void RegInAccount(String u, String p) {
        Console.write(L"Имя: \"" + u + L"\"  Пароль: \""+p+"\"");
        Console.println(L"\nАккаунт был создан");
        CreateAccount(u, p);
    }
    void ShowAcc() {
        if(CurrentAcc != L"")
            Console.write(L"Текущий аккаунт: \""+CurrentAcc+"\"");
        else
            Console.write(L"Вы не вошли ни в один аккаунт");
    }
    bool commands(String line) {
        bool res = false;
        if (func(getIO()) == L"users") { for (int i = 0; i < Users.size(); i++) {
                                            Console.write(inttostr(i+1)+". Name: "+ Users[i].getUsername()+'\n');
                                            Console.println('\n');
                                            }
                                            Console.println("\nTotal: " + inttostr(Users.size()) + " users"); 
                                            res = true;
        } else
            if (func(getIO()) == L"login") { LogInAccount(arg(getIO(), 0), arg(getIO(), 1)); res = true; }else
            if (func(getIO()) == L"register") { RegInAccount(arg(getIO(), 0), arg(getIO(), 1)); res = true;  }
            if (func(getIO()) == L"quit") { CurrentAcc = CurrentPass = L""; Console.write(L"Вы вышли из всех аккаунтов."); res = true; }
            if (func(getIO()) == L"account") { ShowAcc(); res = true; }
            if (func(getIO()) == L"delete") { DeleteAcc(arg(getIO(), 0), arg(getIO(), 1)); res = true; }
        return res;
    }
};


AccManagerClass OS; //И да, сперва я хотел сделать класс OS но птом как-то не получилось, да и зачем, она и так есть