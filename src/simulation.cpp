/* simulation.cpp
 * class that contains everything
*/

#include <iostream>

#include "simulation.hpp"

namespace slug {

    Simulation::Simulation() 
    : mResolution(920, 720)
    {
        mRenderWindow.create(sf::VideoMode(mResolution.x, mResolution.y), "slugs");
    }

    Simulation::~Simulation() {
        std::cout << "Simulation::~Simulation()\n";
    }

    void Simulation::start() {
        mBoxes.push_back(std::unique_ptr<slug::Box>(new Box));
        run();
    }

    void Simulation::run() {
        while(mRenderWindow.isOpen()) {
            // Process events
            sf::Event event;
            while (mRenderWindow.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    stop();
                
                // Escape key: exit
                if (event.type == sf::Event::KeyReleased) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        stop();
                    }
                }
            }
            // Clear screen
            mRenderWindow.clear(sf::Color::Magenta);

            // Update
            for (auto &b : mBoxes) {
                b->update();
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