#include "Core.hpp"
#include "IOSystem.hpp"
#include "Utils.hpp"
//#include "IOButtons.hpp"
#include "IOCursor.hpp"
#include "TerminalScript.hpp"
#include "Editor.hpp"


void updateShader(float tick) {
    shader.setUniform("time", tick / 3 * 200);
    shader.setUniform("resolution", sf::Glsl::Vec2(window.getSize().x, window.getSize().y));
    shader.setUniform("texture", rentex.getTexture());
    shader.setUniform("pw", float(0.45));
    shader.setUniform("clr", light);
}


int main(){  



    InitWindow("Console", false);
    Console.mouseVisible = true;
    Console.cursorVisible = true;
    Console.DEBUG_MODE = true;
    shader.loadFromFile("screen.frag", Shader::Fragment);
    Parser.resetScript();
    Parser.runScript("system\\core\\bios.pbs");
   
    float fps;
    sf::Clock clock = sf::Clock::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    while (window.isOpen())
    {
        float timer = gameclock.getElapsedTime().asMicroseconds(); timer /=314; gameclock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        { 
            Console.eventHandler(event, IOSYSTEM);


            if (event.type == sf::Event::Closed) window.close();
           
            if (event.type == Event::KeyPressed) {

                if (event.key.code == Keyboard::F10) LightOff();
                if (event.key.code == Keyboard::F9) LightOn();
                if (event.key.code == Keyboard::F8) {
                    sf::Vector2u windowSize = window.getSize();
                    sf::Texture texture;
                    texture.create(windowSize.x, windowSize.y);
                    texture.update(window);
                    sf::Image Screenshot = texture.copyToImage();
                    Screenshot.saveToFile("screenshot.png"); 
                }
            }
        }



        //----------------------  Update ------------------------
        Parser.update(timer);
        Console.update(timer);
        MouseCursor.update();
        //----------------------  Render ------------------------
    
   
        window.clear();
        rentex.clear();
        updateShader(timer);
        Console.drawBuf();   

        rentex.draw(MouseCursor);
        {
            rentex.display(); 
            rendersprite.setTexture(rentex.getTexture());
            window.draw(rendersprite,&shader);
        }

      
        window.display();


        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        window.setTitle("fps ="+ inttostr(floor(fps)));
        previousTime = currentTime;


    }

    return 0;
}