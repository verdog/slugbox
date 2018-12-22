/* nnet
 * neural network designed for use with NEAT
*/

#pragma once

#include <vector>
#include <functional>
#include <memory>

namespace slug {

    class NNNode {
        public:
            NNNode();
            ~NNNode();
            const unsigned int nodeID;
            float activationFunction(float input);

            virtual float getValue();
            void setValue(float value);

            bool operator == (const NNNode& b);
        private:
            float mValue;
            static unsigned int nextNodeID;
    };

    class NNInputNode : public NNNode {
        public:
            NNInputNode(std::function<float ()> inputFunc);
            ~NNInputNode();
            virtual float getValue();
        private:
            std::function<float ()> mInputFunc;
    };

    class Connection {
        public:
            Connection(NNNode &in, NNNode &out);
            NNNode &input;
            NNNode &output;
            float weight;
            bool enabled;
            const unsigned int innovNum;
        private:
            static unsigned int nextInnovNum;
    };

    class NeuralNetwork {
        public:
            NeuralNetwork();
            ~NeuralNetwork();

            NNNode& createInput(std::function<float ()> func);
            NNNode& createOutput();

            void mutate();
            void fullyConnect();
            void randomizeWeights();

            std::vector<float> run();

        protected:
            std::vector<NNInputNode> mInputNodes;
            std::vector<NNNode> mHiddenNodes;
            std::vector<NNNode> mOutputNodes;
            std::vector<Connection> mConnections;

            void addNodeOnConnection(Connection &conn);
            NNNode& createFloatingHidden();
            void createNewRandomConnection();

            float calculateValue(NNNode *node);

            bool mDirty;
    };

} // slug
