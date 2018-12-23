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
            unsigned int nodeID;
            virtual float getValue();
            float activationFunction(float input);

            void setValue(float value);

            bool operator == (const NNNode &b);
        private:
            float mValue;
            static unsigned int nextNodeID;
    };

    class NNInputNode : public NNNode {
        friend class NeuralNetwork;
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
            NNNode *input;
            NNNode *output;
            float weight;
            bool enabled;
            unsigned int innovNum;

            bool operator == (const Connection &b);
        private:
            static unsigned int nextInnovNum;
    };

    class NeuralNetwork {
        public:
            NeuralNetwork();
            NeuralNetwork(const NeuralNetwork& other);
            ~NeuralNetwork();

            NNNode& createInput(std::function<float ()> func);
            NNNode& createOutput();

            void mutate();
            void fullyConnect();
            void randomizeWeights();

            std::vector<float> run();

        protected:
            std::vector<std::unique_ptr<NNInputNode>> mInputNodes;
            std::vector<std::unique_ptr<NNNode>> mHiddenNodes;
            std::vector<std::unique_ptr<NNNode>> mOutputNodes;
            std::vector<std::unique_ptr<Connection>> mConnections;

            void addNodeOnConnection(Connection &conn);
            NNNode& createFloatingHidden();
            void createNewRandomConnection();

            float calculateValue(NNNode *node);

            bool mDirty;
    };

} // slug
