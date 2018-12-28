/* drawablennet
 * a neural network class with added SFML drawing capabilites
*/

#include <iostream>
#include <map>

#include <drawablennet.hpp>

namespace slug {

    DrawableNeuralNet::DrawableNeuralNet() {
        std::cout << "dNNet()\n";
        if (!mFont.loadFromFile("./resources/fonts/NotoMono-Regular.ttf")) {
            std::cerr << "DrawableNeuralNet failed to load font!\n";
        }
    }

    DrawableNeuralNet::DrawableNeuralNet(const NeuralNetwork &other) 
    : NeuralNetwork(other)
    {
        std::cout << "dNNet()\n";
        if (!mFont.loadFromFile("./resources/fonts/NotoMono-Regular.ttf")) {
            std::cerr << "DrawableNeuralNet failed to load font!\n";
        }
    }

    DrawableNeuralNet::~DrawableNeuralNet() {
        std::cout << "~dNNet()\n";
    }

    void DrawableNeuralNet::draw(sf::RenderTarget &target, sf::RenderStates states) const {

        // draw circles for each node
        std::map<unsigned int, sf::CircleShape> nodeCircles;
        std::vector<sf::VertexArray> nodeConnections;
        std::vector<sf::Text> labels;
        
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
            nodeCircles[node->nodeID] = newCircle;

            // push back label
            labels.push_back(sf::Text(std::to_string(node->nodeID) + ": " + std::to_string(node->getValue()), mFont, 16));
            labels.back().setFillColor(sf::Color::Black);
            labels.back().setPosition(newCircle.getPosition());
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
            nodeCircles[node->nodeID] = newCircle;

            // push back label
            labels.push_back(sf::Text(std::to_string(node->nodeID) + ": " + std::to_string(node->getValue()), mFont, 16));
            labels.back().setFillColor(sf::Color::Black);
            labels.back().setPosition(newCircle.getPosition());
        }

        // add anything in between
        offsetx = offsetx/2;
        offsety = -1;
        for (auto const &node : mHiddenNodes) {
            auto newCircle = sf::CircleShape(8, 6);
            newCircle.setOrigin(8, 8);
            newCircle.setOutlineColor(sf::Color::Black);
            newCircle.setOutlineThickness(2);
            newCircle.setFillColor(sf::Color::Transparent);
            newCircle.move(offsetx, offsety-- * 24);
            nodeCircles[node->nodeID] = newCircle;

            // push back label
            labels.push_back(sf::Text(std::to_string(node->nodeID) + ": " + std::to_string(node->getValue()), mFont, 16));
            labels.back().setFillColor(sf::Color::Black);
            labels.back().setPosition(newCircle.getPosition());
        }

        // calculate connections
        for (auto const &conn : mConnections) {
            if (conn->enabled == false) continue;
            auto newConnection = sf::VertexArray(sf::Lines, 2);
            newConnection[0].position = nodeCircles[conn->input->nodeID].getPosition();
            newConnection[1].position = nodeCircles[conn->output->nodeID].getPosition();

            float weight = conn->weight;
            short int r = weight < 0 ? -weight * 255 : 0;
            short int b = weight > 0 ? weight * 255 : 0;

            sf::Color color = sf::Color(r, 0, b);

            newConnection[0].color = color;
            newConnection[1].color = color;
            nodeConnections.push_back(newConnection);
        }

        // draw em
        for (auto const &pair : nodeCircles) {
            target.draw(pair.second, states);
        }

        for (auto const &line : nodeConnections) {
            target.draw(line, states);
        }

        for (auto const &label : labels) {
            target.draw(label, states);
        }
    }

}