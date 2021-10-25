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
#include <filesystem>

using namespace sf;
using namespace std;

#define watch(x) cout << (#x) << " is " << String(x).toAnsiString() << endl
#define wint(x) cout << (#x) << " is " << String(inttostr(x)).toAnsiString() << endl


typedef wchar_t Char;

typedef struct _settings {
    Vector2u winSize = Vector2u(480,270);
    bool fullscreen = true;
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
    CreateTerminalWindow(title,Settings.winSize.x*(fs+1), Settings.winSize.y*(fs + 1), Style::Close);
    rentex.create(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    Settings.fullscreen = fs;
}




void setFullscreen(bool fs) {
    CreateTerminalWindow("", Settings.winSize.x*(fs + 1), Settings.winSize.y*(fs + 1), Style::Close);
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