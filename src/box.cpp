/* box.cpp
 * the box that contains the slugs
*/

#include <iostream>
#include <memory>

#include "simulation.hpp"
#include "vectorutils.hpp"
#include "box.hpp"
#include "slug.hpp"

namespace slug {

    Box::Box(Simulation &sim) 
    : mSim {sim}
    {
        spawn();
    }

    Box::~Box() {
        std::cout << "Box::~Box()\n";
    }

    void Box::spawn() {
        mEntities.push_back(std::shared_ptr<slug::Slug>(new Slug));
    }

    void Box::handleInput() {
        if (mSim.mMouseInterface.isButtonPressedInstant(sf::Mouse::Left)) {
            // spawn slug
            auto newSlug = std::shared_ptr<slug::Slug>(new Slug);
            newSlug->setPosition(mSim.mMouseInterface.getLocalMousePosition());
            mEntities.push_back(std::move(newSlug));
        }
    }

    void Box::update(const sf::Time &dTime) {
        for (auto &e : mEntities) {
            e->update(dTime);
        }

        // find closest slug
        float closestMag = 99999.f;
        slug::Slug* closestSlug = nullptr;
        for (auto &e : mEntities) {
            if (auto slug = std::dynamic_pointer_cast<slug::Slug>(e)) {
                float dist = slug::math::magnitude(mSim.mMouseInterface.getLocalMousePosition() - slug->getPosition());
                if (dist < closestMag) {
                    closestSlug = slug.get();
                    closestMag = dist;
                }
            }
        }

        if (closestSlug != nullptr) {
            sf::RectangleShape bounds = sf::RectangleShape(sf::Vector2f(closestSlug->getLocalBounds().width, closestSlug->getLocalBounds().height));
            bounds.move(closestSlug->getPosition() - sf::Vector2f(closestSlug->getLocalBounds().width, closestSlug->getLocalBounds().height)/2.f);
            bounds.setFillColor(sf::Color::Transparent);
            bounds.setOutlineColor(sf::Color::Black);
            bounds.setOutlineThickness(2);
            mSim.mRenderWindow.draw(bounds);
            sf::Transform t;
            t.translate(closestSlug->getPosition() + sf::Vector2f(closestSlug->getLocalBounds().width, -closestSlug->getLocalBounds().height/2));
            mSim.mRenderWindow.draw(closestSlug->getBrain(), t);
        }
    }

    void Box::drawContents(sf::RenderWindow &window) {
        for (auto &e : mEntities) {
            window.draw(*e);
        }
    }

} // slug