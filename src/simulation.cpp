/* simulation.cpp
 * class that contains everything
*/

#include <iostream>

#include "simulation.hpp"
#include "box.hpp"

namespace slug {

    Simulation::Simulation() 
    : mResolution(920, 720)
    , mMouseInterface(*this)
    {
        mRenderWindow.create(sf::VideoMode(mResolution.x, mResolution.y), "slugs");
        mRenderWindow.setVerticalSyncEnabled(true);
    }

    Simulation::~Simulation() {
        std::cout << "Simulation::~Simulation()\n";
    }

    void Simulation::start() {
        srand(std::time(nullptr));
        mBoxes.push_back(std::unique_ptr<slug::Box>(new Box(*this)));
        run();
    }

    sf::Clock dTimer;
    sf::Time dTime;

    void Simulation::run() {
        while(mRenderWindow.isOpen()) {
            // start timer
            dTime = dTimer.getElapsedTime();
            dTimer.restart();

            // Process events
            sf::Event event;
            while (mRenderWindow.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    stop();

                // keep track of window focus
                if (event.type == sf::Event::GainedFocus) mWindowHasFocus = true;
                if (event.type == sf::Event::LostFocus) mWindowHasFocus = false;
                
                // Keypress
                if (event.type == sf::Event::KeyReleased) {
                    // Escape key: exit
                    if (event.key.code == sf::Keyboard::Escape) {
                        stop();
                    }

                    // "r": restart
                    if (event.key.code == sf::Keyboard::R) {
                        mBoxes.pop_back();
                        mBoxes.push_back(std::unique_ptr<slug::Box>(new Box(*this)));
                    }
                }
            }
            // Clear screen
            mRenderWindow.clear(sf::Color::Magenta);

            // Update
            for (auto &b : mBoxes) {
                b->handleInput();
                b->update(dTime);
                b->drawContents(mRenderWindow);
            }

            // Update the window
            mRenderWindow.display();
        }
    }

    void Simulation::stop() {
        mRenderWindow.close();
    }

} // slug