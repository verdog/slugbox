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
        // create default brain
        auto stayHigh = [](){return 1.0;};

        mDefaultBrain.createInput(stayHigh);
        mDefaultBrain.createInput(stayHigh);
        mDefaultBrain.createInput(stayHigh);

        mDefaultBrain.createOutput();

        mDefaultBrain.fullyConnect();

        mDefaultBrain.addNodeOnRandomConnection();
        mDefaultBrain.addNodeOnRandomConnection();
        mDefaultBrain.addNodeOnRandomConnection();

        auto newSlug = std::unique_ptr<slug::Slug>(new Slug(mDefaultBrain));

        newSlug->setPosition(sf::Vector2f(128, 128));

        mEntities.push_back(std::move(newSlug));
    }

    void Box::handleInput() {
        if (mSim.mMouseInterface.isButtonPressedInstant(sf::Mouse::Left)) {
            // spawn slug
            auto newSlug = std::unique_ptr<slug::Slug>(new Slug(mDefaultBrain));
            newSlug->setPosition(mSim.mMouseInterface.getLocalMousePosition());
            mEntities.push_back(std::move(newSlug));
        }

        // find closest slug
        float closestMag = 99999.f;
        mClosestSlug = nullptr;
        for (auto &e : mEntities) {
            if (auto slug = dynamic_cast<slug::Slug*>(e.get())) {
                float dist = slug::math::magnitude(mSim.mMouseInterface.getLocalMousePosition() - slug->getPosition());
                if (dist < closestMag) {
                    mClosestSlug = slug;
                    closestMag = dist;
                }
            }
        }

        if (mSim.mMouseInterface.isButtonPressedInstant(sf::Mouse::Right)) {
            // mutate the brain
            if (mClosestSlug != nullptr) {
                mClosestSlug->getBrain().mutate();
            }
        }
    }

    void Box::update(const sf::Time &dTime) {
        for (auto &e : mEntities) {
            e->update(dTime);
        }
    }

    void Box::drawContents(sf::RenderWindow &window) {
        for (auto &e : mEntities) {
            window.draw(*e);
        }

        if (mClosestSlug != nullptr) {
            sf::RectangleShape bounds = sf::RectangleShape(sf::Vector2f(mClosestSlug->getLocalBounds().width, mClosestSlug->getLocalBounds().height));
            bounds.move(mClosestSlug->getPosition() - sf::Vector2f(mClosestSlug->getLocalBounds().width, mClosestSlug->getLocalBounds().height)/2.f);
            bounds.setFillColor(sf::Color::Transparent);
            bounds.setOutlineColor(sf::Color::Black);
            bounds.setOutlineThickness(2);
            mSim.mRenderWindow.draw(bounds);
            sf::Transform t;
            t.translate(mClosestSlug->getPosition() + sf::Vector2f(mClosestSlug->getLocalBounds().width, -mClosestSlug->getLocalBounds().height/2));
            mSim.mRenderWindow.draw(mClosestSlug->getBrain(), t);
        }
    }

} // slug