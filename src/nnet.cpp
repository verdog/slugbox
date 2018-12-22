/* nnet
 * neural network designed for use with NEAT
*/

#include <iostream>
#include <cassert>
#include <cmath>
#include <set>

#include "nnet.hpp"
#include "vectorutils.hpp"

namespace slug {

    unsigned int Connection::nextInnovNum = 0;   

    Connection::Connection(NNNode &in, NNNode &out) 
    : input {in}
    , output {out}
    , innovNum {nextInnovNum++}
    {
        weight = 1.0f;
        enabled = true;

        std::cout 
            << "Connection(): "
            << input.nodeID << " -> " << output.nodeID << ", "
            << "weight: " << weight << ", "
            << "enabled: " << enabled << ", "
            << "innovNum: " << innovNum << "\n"
        ;
    }

////////////////////////////////////////////////////////////////////////////////

    unsigned int NNNode::nextNodeID = 0;

    NNNode::NNNode() 
    : nodeID {nextNodeID++}
    {

    }

    NNNode::~NNNode() {

    }

    float NNNode::getValue() {
        return mValue;
    }

    void NNNode::setValue(float value) {
        mValue = value;
    }

    float NNNode::activationFunction(float input) {
        std::cout << "activationFunction(" << input << ")\n";
        return 1 / (1 + std::exp(-input));
    }

    bool NNNode::operator==(const NNNode &b) {
        return nodeID == b.nodeID;
    }

////////////////////////////////////////////////////////////////////////////////

    NNInputNode::NNInputNode(std::function<float ()> func) 
    : mInputFunc {func}
    {

    }

    NNInputNode::~NNInputNode() {

    }

    float NNInputNode::getValue() {
        return mInputFunc();
        std::cout << "NNInputNode::getValue() done.\n";
    }

////////////////////////////////////////////////////////////////////////////////

    NeuralNetwork::NeuralNetwork() 
    : mDirty {true}
    {
        // create bias node
        createInput([](){return 1.0;});

        std::cout << "NNet()\n";
    }

    NeuralNetwork::~NeuralNetwork() {
        std::cout << "~NNet()\n";
    }

    NNNode& NeuralNetwork::createInput(std::function<float ()> func) {
        mInputNodes.push_back(NNInputNode(func));

        std::cout << "createInput() done.\n";

        mDirty = true;

        return mInputNodes.back();
    }

    NNNode& NeuralNetwork::createOutput() {
        mOutputNodes.push_back(NNNode());

        std::cout << "createOutput() done.\n";

        mDirty = true;

        return mOutputNodes.back();
    }

    NNNode& NeuralNetwork::createFloatingHidden() {
        mHiddenNodes.push_back(NNNode());

        std::cout << "createFloatingHidden() done.\n";

        return mHiddenNodes.back();
    }

    void NeuralNetwork::addNodeOnConnection(Connection &conn) {
        // disable the connection
        conn.enabled = false;

        // create a node
        NNNode &newHidden = createFloatingHidden();

        // create two new connections
        mConnections.push_back(Connection(conn.input, newHidden));
        mConnections.back().weight = conn.weight;
        mConnections.push_back(Connection(newHidden, conn.output));
        // weight = 1 by default
    }

    void NeuralNetwork::createNewRandomConnection() {
        std::vector<NNNode*> inputCandidates;
        std::transform(mInputNodes.begin(), mInputNodes.end(), std::back_inserter(inputCandidates), [](NNNode& n) {return &n;});
        std::transform(mHiddenNodes.begin(), mHiddenNodes.end(), std::back_inserter(inputCandidates), [](NNNode& n) {return &n;});
        NNNode &input = *inputCandidates[math::randi(0, inputCandidates.size()-1)];

        std::vector<NNNode*> outputCandidates;
        std::transform(mHiddenNodes.begin(), mHiddenNodes.end(), std::back_inserter(outputCandidates), [](NNNode& n) {return &n;});
        std::transform(mOutputNodes.begin(), mOutputNodes.end(), std::back_inserter(outputCandidates), [](NNNode& n) {return &n;});

        // erase input from list, if it's in there
        auto inputIter = std::find(outputCandidates.begin(), outputCandidates.end(), &input);
        if (inputIter != outputCandidates.end()) {
            outputCandidates.erase(inputIter);
        }
        NNNode &output = *outputCandidates[math::randi(0, outputCandidates.size()-1)];

        // check for dups here

        mConnections.push_back(Connection(input, output));
    }

    void NeuralNetwork::mutate() {
        addNodeOnConnection(mConnections[math::randi(0, mConnections.size() - 1)]);
        createNewRandomConnection();
    }

    void NeuralNetwork::fullyConnect() {
        // connect each input to each output
        for (auto &inputNode : mInputNodes) {
            for (auto &outputNode : mOutputNodes) {
                mConnections.push_back(Connection(inputNode, outputNode));
            }
        }

        mDirty = false;

        std::cout << "fullyConnect() done. \n";
    }

    void NeuralNetwork::randomizeWeights() {
        // randomize the weight of each connection
        for (auto &connection : mConnections) {
            connection.weight = math::randf(-1, 1);
        }

        std::cout << "randomizeWeights() done.\n";
    }

    std::vector<float> NeuralNetwork::run() {
        std::vector<float> outputs;

        // recursively calculate the value of each output node
        for (auto &outputNode : mOutputNodes) {
            outputs.push_back(calculateValue(&outputNode));
        }

        std::cout << "run() done.\n";

        return outputs;
    }

    float NeuralNetwork::calculateValue(NNNode *node) {
        float sum = 0;
        std::set<NNNode*> dependancies;

        // build set of nodes that need to be calculated first
        for (auto &conn : mConnections) {
            if (conn.output == *node && conn.enabled) {
                dependancies.insert(&conn.input);
            }
        }

        // if the set is empty, node is an input node.
        if (dependancies.empty()) {
            return node->getValue(); // calls input generating function
        } else {
            // sum up the value
            for (auto nodePtr : dependancies) {
                sum += calculateValue(nodePtr);
            }
        }

        // set my value
        node->setValue(node->activationFunction(sum));

        std::cout << "Set value of node ID " << node->nodeID << " to " << node->getValue() << ".\n";

        return node->getValue();
    }

}