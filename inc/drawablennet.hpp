/* drawablennet
 * a neural network class with added SFML drawing capabilites
*/

#pragma once

#include <SFML/Graphics.hpp>

#include <nnet.hpp>

namespace slug {

    class DrawableNeuralNet : public NeuralNetwork, public sf::Drawable {
        public:
            DrawableNeuralNet();
            ~DrawableNeuralNet();

            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        private:
    };

}