#ifndef __CORE
#define __CORE
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <codecvt>

using namespace sf;
using namespace std;

#define watch(x) cout << (#x) << " is " << String(x).toAnsiString() << endl
#define wint(x) cout << (#x) << " is " << String(inttostr(x)).toAnsiString() << endl


typedef wchar_t Char;

typedef struct _settings {
    Vector2u winSize = Vector2u(480,270);
    bool fullscreen = false;
    bool fancywhitenoise = false;
    float noisevolume = 5;
    float sfxvolume = 100;
};



_settings Settings;
RenderTexture rentex;
RenderWindow  window;
Sprite rendersprite;
float charSize;
sf::Font font;
Shader shader;
Clock gameclock;

void CreateTerminalWindow(String title, int width, int height , int Style) {
    window.create(VideoMode(width, height), title, Style);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
}




void InitWindow(String title,bool fs) {
    charSize = 16 + fs * 16;

    if(!fs)
        CreateTerminalWindow(title,Settings.winSize.x, Settings.winSize.y, Style::Close);
    else   
        CreateTerminalWindow(title,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),Style::Fullscreen);
    rentex.create(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    Settings.fullscreen = fs;
}




void setFullscreen(bool fs) {
    if (!fs)
        CreateTerminalWindow("", Settings.winSize.x, Settings.winSize.y, Style::Close);
    else
        CreateTerminalWindow("", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), Style::Fullscreen);
    Settings.fullscreen = fs;
}


Vector2u posToXY(int x,int y) {
    return Vector2u(x * charSize/2,y*charSize*(9. / 8.));
}
int posToX(int x) {
    return x * charSize / 2;
 }
int posToY(int y) {
    return y*charSize*(9. / 8.);
}



#endif  // PHBCORE