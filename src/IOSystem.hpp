#ifndef PHBIOS
#define PHBIOS
#include "Core.hpp"


const int ConsoleWidth = 60; 
const int ConsoleHeight = 16;

enum MODES {
    DEFAULT_CHAR_MODE = 0,
    BLINK_MODE = 1,
    NOMOVE_MODE
};


class _system_io {
private:
    
    typedef struct _char {
        wchar_t chr = 0;
        Color back = Color::Black, fore = Color(100, 100, 100);
        unsigned int mode = DEFAULT_CHAR_MODE;
    };

    sf::Text textRender;
    sf::VertexArray backRender;


   
   


    String *input;
    int inputCursor = 0;
    Text cursor;
   
    float counter;
    float tvel = 0.005, tcounter,tc = 0;
    String tstr;


public:
    Vector2u pointer = Vector2u(0, 0);
    bool mouseVisible = true, cursorVisible = true;
    Color backColor = Color::Black, foreColor = Color(100, 100, 100);
    int nowmode = DEFAULT_CHAR_MODE;
    _char buf[ConsoleWidth][ConsoleHeight];
    bool WRITE_MODE = false;
    bool DEBUG_MODE = true;
    bool PRINT_MODE = false;
    void setBack(int x, int y, Color c) {
        backRender[0].position = sf::Vector2f(x * charSize / 2, y*charSize*(9. / 8.));
        backRender[1].position = sf::Vector2f(x * charSize / 2+ charSize / 2, y*charSize*(9. / 8.));
        backRender[2].position = sf::Vector2f(x * charSize / 2+ charSize / 2, y*charSize*(9. / 8.)+ charSize * (9. / 8.));
        backRender[3].position = sf::Vector2f(x * charSize / 2, y*charSize*(9. / 8.)+ charSize * (9. / 8.));
       
        backRender[0].color = c;
            backRender[1].color = c;
            backRender[2].color = c;
            backRender[3].color = c;
    }
    _system_io() {
      
        font.loadFromFile("charset.ttf");
        cursor.setFont(font);
        cursor.setCharacterSize(charSize);
        cursor.setString('_');
        cursor.setFont(font);
       
        textRender.setFont(font);
        textRender.setCharacterSize(charSize);
       
        backRender = VertexArray(sf::Quads, 4);
        backRender[0].position = sf::Vector2f(0, 0);
        backRender[1].position = sf::Vector2f(charSize / 2, 0);
        backRender[2].position = sf::Vector2f(charSize / 2, charSize*(9. / 8.));
        backRender[3].position = sf::Vector2f(0, charSize*(9. / 8.));

    }

    void setChar(String chr, int x = -1, int y = -1) {
        if (x == -1) x = pointer.x;
        if (y == -1) y = pointer.y;
        buf[x][y].chr = wchar_t(chr[0]);
        buf[x][y].fore = foreColor;
        buf[x][y].back = backColor;
        buf[x][y].mode = nowmode;
    }
    void insertChar(String chr) {
        int spaceplace = 0;
        int x = pointer.x+pointer.y*ConsoleWidth;

       
        for (int X = (ConsoleWidth * ConsoleHeight)-1; X > x; X--)  {
            if (buf[(X % ConsoleWidth)][X / ConsoleWidth].mode != NOMOVE_MODE)  {
            if((X%ConsoleWidth)> 0)
            buf[X % ConsoleWidth][X / ConsoleWidth] = buf[(X % ConsoleWidth)-1][X / ConsoleWidth];
            else
            buf[X % ConsoleWidth][X / ConsoleWidth] = buf[ConsoleWidth-1][X / ConsoleWidth-1];
            }
        }
        setChar(chr[0]);
    }
    void write(String s) {
        for (int i = 0; i < s.getSize(); i++)
            addChar(s[i]);
    }
    void updateCharSize(float chs) {
        charSize = chs;
     
        textRender.setCharacterSize(charSize);
        cursor.setCharacterSize(charSize);
      
    }
    void update(float tick) {
        if (PRINT_MODE)
        if(tcounter < tstr.getSize()) { 
            updateCharSize(16 * Settings.fullscreen + 16); //Это блять костыль
            if (int(tcounter) - int(tc) >= 1) {
                tc = tcounter;        
                if (tstr[int(tcounter)] == L'\n') nextLine(); else addChar(tstr[int(tcounter)]);
            }
            if (tvel == 0) {
                tcounter++;
                for (tcounter+1; tcounter < tstr.getSize(); tcounter++)  
                    if (tstr[int(tcounter)] == L'\n') { nextLine(); tcounter++; }
                    else addChar(tstr[int(tcounter)]);
            }
                else tcounter += tvel * tick; 
        }
        else { PRINT_MODE = false; tvel = 0.005; }
        counter += 0.0005 * tick;
        counter *= (counter < 2);
        cursor.setString(("_ ")[int(counter) % 2]);
       
    }
    Vector2f getMouse() {
        return Vector2f(int(Mouse::getPosition().x / (charSize / 2)),
            int(Mouse::getPosition().y / (charSize*(9. / 8.))));
    }
    void eraseChar(int x = -1, int y = -1) {
        if (x == -1) x = pointer.x;
        if (y == -1) y = pointer.y;
        if (x > 0) {
            pointer.x--;
            buf[pointer.x][pointer.y].chr = 0;
            buf[pointer.x][pointer.y].fore = foreColor;
            buf[pointer.x][pointer.y].back = backColor;
            buf[pointer.x][pointer.y].mode = DEFAULT_CHAR_MODE;
        }
        else if (y > 0) {
            pointer.y--;
            pointer.x = ConsoleWidth - 1;
            buf[pointer.x][pointer.y].chr = 0;
            buf[pointer.x][pointer.y].fore = foreColor;
            buf[pointer.x][pointer.y].back = backColor;
            buf[pointer.x][pointer.y].mode = DEFAULT_CHAR_MODE;
        }else
            buf[0][0].chr = 0;
           buf[0][0].fore = foreColor;
           buf[0][0].back = backColor;
           buf[0][0].mode = DEFAULT_CHAR_MODE;
        x = pointer.x + pointer.y*ConsoleWidth;
        for (int X = x; X < ConsoleWidth*ConsoleHeight; X++) 
        {
            if((X % ConsoleWidth) + 1 < ConsoleWidth)
            buf[X % ConsoleWidth][X / ConsoleWidth] = buf[(X % ConsoleWidth) +1][X / ConsoleWidth];
          
        }
    }

    wstring getCenterOffset(wstring s) {
       
        wstring r;
        for (int i = 0; i <= (ConsoleWidth - s.size()) / 2; i++)
            r.push_back(L' ');
        return r;
    }

    void center(wstring c) {
        print(getCenterOffset(c) + c);
    }
    void println(String str) {
        tstr = str+'\n';
        tcounter = 0;
        tc = -1;
        PRINT_MODE = true;
    }    
    void println(String str,float tv) {
        tstr = str + '\n';
        tcounter = 0;
        tc = -1;
        tvel = 0;
        PRINT_MODE = true;
    }
    void print(String str) {
        tstr = str;
        tcounter = 0;
        tc = -1;
        PRINT_MODE = true;
    }
    void addChar(String chr) {
          buf[pointer.x][pointer.y].chr = wchar_t(chr[0]);
          buf[pointer.x][pointer.y].fore = foreColor;
          buf[pointer.x][pointer.y].back = backColor;
          buf[pointer.x][pointer.y].mode = nowmode;
          if (pointer.x < (ConsoleWidth-1)) pointer.x++; else { pointer.x *= 0; pointer.y++; }
    }

    void read(String &rd) {
        PRINT_MODE = false;
        WRITE_MODE = true;
        rd = "";
        input = &rd;
        inputCursor = 0;
    }


    void nextLine() {
        pointer = Vector2u(0, (pointer.y + 1) % ConsoleHeight);
        if (pointer.y+1 == ConsoleHeight) clear();
    }
    void setBlinkMode(int bm) {
        nowmode = bm;
    }
    void clear() {
        for (int x = 0; x < ConsoleWidth; x++)
            for (int y = 0; y < ConsoleHeight; y++) {
                buf[x][y].chr = 0;
                buf[x][y].fore = foreColor;
                buf[x][y].back = backColor;
                buf[x][y].mode = nowmode;
            }
        pointer = Vector2u(0,0);
    }

    
    void eventHandler(Event event,void(*iosys)()) {
        cursorVisible = true;
        if (event.type == Event::KeyPressed) {
         
            if (event.key.code == Keyboard::F11) {
                charSize = 16 + (16 * !Settings.fullscreen);
                textRender.setCharacterSize(charSize);
                cursor.setCharacterSize(charSize);
              
              
            }
            if(WRITE_MODE) {
                if (event.key.code == Keyboard::Left) 
                    if(abs(inputCursor)<input->getSize()) {
                        inputCursor--;
                    if (pointer.x > 0) pointer.x--; else { pointer.x = ConsoleWidth - 1; pointer.y--; }
                }
                if (event.key.code == Keyboard::Right) 
                    if (inputCursor < 0) {
                         inputCursor++; if (pointer.x < (ConsoleWidth - 1)) pointer.x++; else { pointer.x *= 0; pointer.y++; }
                }
              
            }
            if(event.key.code == Keyboard::Enter&&(DEBUG_MODE||WRITE_MODE)&&!PRINT_MODE)
                nextLine();
            if (PRINT_MODE && event.key.code == Keyboard::LControl)
                tvel = 0;
        }
        if(event.type == Event::KeyReleased)
            if (event.key.code == Keyboard::LControl)
                tvel = 0.005;
        if(WRITE_MODE)
            if(event.type == Event::TextEntered)
            {

                if (event.text.unicode == 13) { iosys();  WRITE_MODE = false; return; }
                else
                if (event.text.unicode == 8) { if((input->getSize()+inputCursor) > 0) {input->erase(input->getSize() - 1 + inputCursor, 1); eraseChar();} }
                
                else{
                    insertChar(Char(event.text.unicode));
                
                    if (pointer.x < (ConsoleWidth - 1)) pointer.x++; else { pointer.x *= 0; pointer.y++; }
                    input->insert(input->getSize() + inputCursor,Char(event.text.unicode));
                }
            }

    }

    void drawBuf() {
        
        for(int x = 0;x<ConsoleWidth;x++)
            for (int y = 0; y < ConsoleHeight; y++) 
                if(buf[x][y].chr > 0) {
                textRender.setFillColor(buf[x][y].fore);
                textRender.setPosition(x * charSize/2, y*charSize*(9./8.)-charSize/8.);
                textRender.setString(buf[x][y].chr);
      
                setBack(x, y, buf[x][y].back);

                if(buf[x][y].mode == BLINK_MODE) {
                if(buf[x][y].mode != int(counter)){
                    rentex.draw(backRender);
                    rentex.draw(textRender);
                }
                } else{
                    
                  rentex.draw(backRender);
                  rentex.draw(textRender);
                }
                   
                }
        cursor.setPosition(pointer.x * charSize / 2, pointer.y*charSize*(9./8.));
        if (cursorVisible) rentex.draw(cursor);

       
    }

    void createFrame(int x, int y, int w, int h, String inframe) {
        updateCharSize(16 * Settings.fullscreen + 16); 
        w += x;
        h += y;
        for (int i = x; i < w; i++)
            for (int j = y; j < h; j++) {
                if (j == y || j == h - 1) setChar(L'═',i,j);
                if (i == x || i == w - 1)  setChar(L'║', i, j);

                if (i == x && j == y) setChar(L'╔',i,j);
                if (i == w - 1 && j == h - 1) setChar(L'╝', i, j);
                if (i == w - 1 && j == y) setChar(L'╗', i, j);
                if (i == x && j == h - 1) setChar(L'╚', i, j);

            }
        for (int i = 0; i < inframe.getSize(); i++)
            setChar(inframe[i], x + 1 + i, y + 1);
    }

    void createButton(int x, int y, int w, int h, String inframe) {
        updateCharSize(16 * Settings.fullscreen + 16); 
        w += x;
        h += y;
        for (int i = x; i < w; i++)
            for (int j = y; j < h; j++) {
                if (j == y || j == h - 1) setChar(L'─', i, j);
                if (i == x || i == w - 1) setChar(L'│', i, j);

                if (i == x && j == y) setChar(L'┌', i, j);
                if (i == w - 1 && j == h - 1) setChar(L'┘', i, j);
                if (i == w - 1 && j == y) setChar(L'┐', i, j);
                if (i == x && j == h - 1) setChar(L'└', i, j);

            }
        for (int i = 0; i < inframe.getSize(); i++)
            setChar(inframe[i], x + 1 + i,y + 1);
    }

    void setTextTo(String t, int x, int y) {
        for (int i = 0; i < t.getSize(); i++)
            setChar(t[i], x + i, y);
    }



};

_system_io Console;



#endif