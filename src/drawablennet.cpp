/* drawablennet
 * a neural network class with added SFML drawing capabilites
*/

#include <iostream>
#include <map>

#include <drawablennet.hpp>

namespace slug {

    DrawableNeuralNet::DrawableNeuralNet() {
        std::cout << "dNNet()\n";
    }

    DrawableNeuralNet::~DrawableNeuralNet() {
        std::cout << "~dNNet()\n";
    }

    void DrawableNeuralNet::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        sf::Transform transform;
        transform.translate(320, 320);
        states.transform = transform;

        // draw circles for each node
        std::map<unsigned int, sf::CircleShape> nodeCircles;
        std::vector<sf::VertexArray> nodeConnections;
        
        // add inputs
        int offsetx = 0;
        int offsety = 0;
        for (auto const &node : mInputNodes) {
            auto newCircle = sf::CircleShape(8, 6);
            newCircle.setOrigin(8, 8);
            newCircle.setOutlineColor(sf::Color::Black);
            newCircle.setOutlineThickness(2);
            newCircle.setFillColor(sf::Color::Transparent);
            newCircle.move(offsetx, offsety++ * 24);
            nodeCircles[node.nodeID] = newCircle;
        }

        // add outputs
        offsetx = 128;
        offsety = 0;
        for (auto const &node : mOutputNodes) {
            auto newCircle = sf::CircleShape(8, 6);
            newCircle.setOrigin(8, 8);
            newCircle.setOutlineColor(sf::Color::Black);
            newCircle.setOutlineThickness(2);
            newCircle.setFillColor(sf::Color::Transparent);
            newCircle.move(offsetx, offsety++ * 24);
            nodeCircles[node.nodeID] = newCircle;
        }

        // add anything in between
        offsetx = offsetx/2;
        offsety = 0;
        for (auto const &node : mHiddenNodes) {
            auto newCircle = sf::CircleShape(8, 6);
            newCircle.setOrigin(8, 8);
            newCircle.setOutlineColor(sf::Color::Black);
            newCircle.setOutlineThickness(2);
            newCircle.setFillColor(sf::Color::Transparent);
            newCircle.move(offsetx, offsety++ * 24);
            nodeCircles[node.nodeID] = newCircle;
        }

        // draw connections
        for (auto const &conn : mConnections) {
            if (conn.enabled == false) continue;
            auto newConnection = sf::VertexArray(sf::Lines, 2);
            newConnection[0].position = nodeCircles[conn.input.nodeID].getPosition();
            newConnection[1].position = nodeCircles[conn.output.nodeID].getPosition();
            newConnection[0].color = sf::Color::Black;
            newConnection[1].color = sf::Color::Black;
            nodeConnections.push_back(newConnection);
        }

        // draw em
        for (auto const &pair : nodeCircles) {
            target.draw(pair.second, states);
        }

        for (auto const &line : nodeConnections) {
            target.draw(line, states);
        }
    }

}