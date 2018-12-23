/* nnet
 * neural network designed for use with NEAT
*/

#include <iostream>
#include <cassert>
#include <cmath>
#include <set>
#include <map>

#include "nnet.hpp"
#include "vectorutils.hpp"

namespace slug {

    unsigned int Connection::nextInnovNum = 0;   

    Connection::Connection(NNNode &in, NNNode &out) 
    : input {&in}
    , output {&out}
    , innovNum {nextInnovNum++}
    {
        weight = 1.0f;
        enabled = true;

        std::cout 
            << "Connection(): "
            << input->nodeID << " -> " << output->nodeID << ", "
            << "weight: " << weight << ", "
            << "enabled: " << enabled << ", "
            << "innovNum: " << innovNum << "\n"
        ;
    }

    bool Connection::operator==(const Connection &b) {
        return input->nodeID == b.input->nodeID && output->nodeID == b.output->nodeID;
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
        std::cout << "NNNode::getValue() done.\n";
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
        std::cout << "NNInputNode::getValue() done.\n";
        return mInputFunc();
    }

////////////////////////////////////////////////////////////////////////////////

    NeuralNetwork::NeuralNetwork() 
    : mDirty {true}
    {
        // create bias node
        createInput([](){return 1.0;});

        std::cout << "NNet()\n";
    }

    NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) {
        std::cout << "NeuralNetwork(const NeuralNetwork& other)\n";

        std::map<unsigned int, NNNode*> newNodeMap; // for reconstructing connections

        // make clones of input nodes
        std::transform(
            other.mInputNodes.begin(),
            other.mInputNodes.end(),
            std::back_inserter(mInputNodes),
            [&newNodeMap](const std::unique_ptr<NNInputNode>&n) {
                auto copy = std::unique_ptr<NNInputNode>(new NNInputNode(n->mInputFunc));
                *copy = *n;
                // save id in newNodeMap for later use in reconnecting connections
                newNodeMap.insert(std::pair<unsigned int, NNNode*>(copy->nodeID, copy.get()));
                return std::move(copy);
            }
        );

        // make clones of hidden nodes
        std::transform(
            other.mHiddenNodes.begin(),
            other.mHiddenNodes.end(),
            std::back_inserter(mHiddenNodes),
            [&newNodeMap](const std::unique_ptr<NNNode> &n) {
                auto copy = std::unique_ptr<NNNode>(new NNNode());
                *copy = *n;
                // save id in newNodeMap for later use in reconnecting connections
                newNodeMap.insert(std::pair<unsigned int, NNNode*>(copy->nodeID, copy.get()));
                return std::move(copy);
            }
        );

        // make clones of output nodes
        std::transform(
            other.mOutputNodes.begin(),
            other.mOutputNodes.end(),
            std::back_inserter(mOutputNodes),
            [&newNodeMap](const std::unique_ptr<NNNode> &n) {
                auto copy = std::unique_ptr<NNNode>(new NNNode());
                *copy = *n;
                // save id in newNodeMap for later use in reconnecting connections
                newNodeMap.insert(std::pair<unsigned int, NNNode*>(copy->nodeID, copy.get()));
                return std::move(copy);
            }
        );

        // make clones of connections
        std::transform(
            other.mConnections.begin(),
            other.mConnections.end(),
            std::back_inserter(mConnections),
            [&newNodeMap](const std::unique_ptr<Connection> &c) {
                auto copy = std::unique_ptr<Connection>(new Connection(*c->input, *c->output));
                *copy = *c;

                // at this point the connections in the copy still point to the old object.
                // let's fix that
                copy->input = newNodeMap.at(copy->input->nodeID);
                copy->output = newNodeMap.at(copy->output->nodeID);

                return std::move(copy);
            }
        );
    }   

    NeuralNetwork::~NeuralNetwork() {
        std::cout << "~NNet()\n";
    }

    NNNode& NeuralNetwork::createInput(std::function<float ()> func) {
        mInputNodes.push_back(std::unique_ptr<NNInputNode>(new NNInputNode(func)));

        std::cout << "createInput() done.\n";

        mDirty = true;

        return *mInputNodes.back();
    }

    NNNode& NeuralNetwork::createOutput() {
        mOutputNodes.push_back(std::unique_ptr<NNNode>(new NNNode()));

        std::cout << "createOutput() done.\n";

        mDirty = true;

        return *mOutputNodes.back();
    }

    NNNode& NeuralNetwork::createFloatingHidden() {
        mHiddenNodes.push_back(std::unique_ptr<NNNode>(new NNNode()));

        std::cout << "createFloatingHidden() done. NodeID: " << mHiddenNodes.back()->nodeID << "\n";

        return *mHiddenNodes.back();
    }

    void NeuralNetwork::addNodeOnConnection(Connection &conn) {
        // disable the connection
        conn.enabled = false;

        // create a node
        NNNode &newHidden = createFloatingHidden();

        // create two new connections
        mConnections.push_back(std::unique_ptr<Connection>(new Connection(*conn.input, newHidden)));
        mConnections.back()->weight = conn.weight;
        mConnections.push_back(std::unique_ptr<Connection>(new Connection(newHidden, *conn.output)));

        // weight = 1 by default
    }

    void NeuralNetwork::createNewRandomConnection() {
        // pick an input node
        std::vector<NNNode*> inputCandidates;
        std::transform(mInputNodes.begin(), mInputNodes.end(), std::back_inserter(inputCandidates), [](std::unique_ptr<NNInputNode> &n) {return n.get();});
        std::transform(mHiddenNodes.begin(), mHiddenNodes.end(), std::back_inserter(inputCandidates), [](std::unique_ptr<NNNode> &n) {return n.get();});
        NNNode &input = *inputCandidates[math::randi(0, inputCandidates.size()-1)];

        // pick an output node
        std::vector<NNNode*> outputCandidates;
        std::transform(mHiddenNodes.begin(), mHiddenNodes.end(), std::back_inserter(outputCandidates), [](std::unique_ptr<NNNode> &n) {return n.get();});
        std::transform(mOutputNodes.begin(), mOutputNodes.end(), std::back_inserter(outputCandidates), [](std::unique_ptr<NNNode> &n) {return n.get();});

        // erase input from list so that no loops are created
        auto inputIter = std::find(outputCandidates.begin(), outputCandidates.end(), &input);
        if (inputIter != outputCandidates.end()) {
            outputCandidates.erase(inputIter);
        }
        NNNode &output = *outputCandidates[math::randi(0, outputCandidates.size()-1)];

        // check if the new connection is a duplicate
        for (auto const &conn : mConnections) {
            if (input.nodeID == conn->input->nodeID && output.nodeID == conn->output->nodeID) {
                std::cout << "Duplicate connection created: (" << conn->input->nodeID << " -> " << conn->output->nodeID << "). Discarding.\n";
                return;
            }
        }

        auto newConnection = std::unique_ptr<Connection>(new Connection(input, output));
        mConnections.push_back(std::move(newConnection));
        std::cout << "createNewRandomConnection done. " << newConnection->input->nodeID << " -> " << newConnection->output->nodeID << ".\n";
    }

    void NeuralNetwork::mutate() {
        addNodeOnConnection(*mConnections[math::randi(0, mConnections.size() - 1)]);

        std::cout << "mutate() done.\n";

        // createNewRandomConnection();
    }

    void NeuralNetwork::fullyConnect() {
        // connect each input to each output
        for (auto &inputNode : mInputNodes) {
            for (auto &outputNode : mOutputNodes) {
                mConnections.push_back(std::unique_ptr<Connection>(new Connection(*inputNode, *outputNode)));
            }
        }

        mDirty = false;

        std::cout << "fullyConnect() done. \n";
    }

    void NeuralNetwork::randomizeWeights() {
        // randomize the weight of each connection
        for (auto &connection : mConnections) {
            connection->weight = math::randf(-1, 1);
        }

        std::cout << "randomizeWeights() done.\n";
    }

    std::vector<float> NeuralNetwork::run() {
        std::vector<float> outputs;

        // recursively calculate the value of each output node
        for (auto &outputNode : mOutputNodes) {
            outputs.push_back(calculateValue(outputNode.get()));
        }

        std::cout << "run() done.\n";

        return outputs;
    }

    float NeuralNetwork::calculateValue(NNNode *node) {
        float sum = 0;
        std::vector<NNNode*> pendingNodes;
        std::map<NNNode*, float> pendingWeights;

        std::cout << "calculating value for node " << node->nodeID << "\n";

        // build vector of nodes that need to be calculated first
        for (auto const &conn : mConnections) {
            if (*conn->output == *node && conn->enabled) {
                pendingNodes.push_back(conn->input);
                pendingWeights.insert(std::pair<NNNode*, float>(conn->input, conn->weight));
            }
        }

        // if the vector is empty, node is an input node.
        if (pendingNodes.empty()) {
            std::cout << "input node.\n";
            return node->getValue(); // calls input generating function
        } else {
            // sum up the value
            for (auto pendNode : pendingNodes) {
                sum += pendingWeights.at(pendNode) * calculateValue(pendNode);
            }
        }

        // set my value
        node->setValue(node->activationFunction(sum));

        // std::cout << "Set value of node ID " << node->nodeID << " to " << node->getValue() << ".\n";

        return node->getValue();
    }

}