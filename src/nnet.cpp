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
    }

    void Connection::printInfo() {
        std::cout 
            << "innovNum: " << innovNum << ", "
            << input->nodeID << " -> " << output->nodeID << ", "
            << "weight: " << weight << ", "
            << "enabled: " << enabled << "\n"
        ;
    }

    bool Connection::operator==(const Connection &b) {
        return input->nodeID == b.input->nodeID && output->nodeID == b.output->nodeID;
    }

////////////////////////////////////////////////////////////////////////////////

    unsigned int NNNode::nextNodeID = 0;

    NNNode::NNNode() 
    : nodeID {nextNodeID++}
    , seen {false}
    , mValue {0.0f}
    {}

    NNNode::~NNNode() {
        std::cout << "~NNNode()\n";
    }

    float NNNode::getValue() {
        return mValue;
    }

    void NNNode::setValue(float value) {
        mValue = value;
    }

    float NNNode::activationFunction(float input) {
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
    }

////////////////////////////////////////////////////////////////////////////////

    NeuralNetwork::NeuralNetwork() 
    : mDirty {true}
    , fitness {0}
    {
    }

    NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) {
        std::cout << "NeuralNetwork::NeuralNetwork(const NeuralNetwork& other)\n";
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
                return copy;
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
                return copy;
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
                return copy;
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
                // std::cout << "cloning connection... ";
                // copy->printInfo();
                copy->input = newNodeMap.at(copy->input->nodeID);
                copy->output = newNodeMap.at(copy->output->nodeID);

                return copy;
            }
        );
    }   

    NeuralNetwork::~NeuralNetwork() {
    }

    NNNode& NeuralNetwork::createInput(std::function<float ()> func) {
        mInputNodes.push_back(std::unique_ptr<NNInputNode>(new NNInputNode(func)));
        return *mInputNodes.back();
    }

    NNNode& NeuralNetwork::createOutput() {
        mOutputNodes.push_back(std::unique_ptr<NNNode>(new NNNode()));
        return *mOutputNodes.back();
    }

    NNNode& NeuralNetwork::createFloatingHidden() {
        mHiddenNodes.push_back(std::unique_ptr<NNNode>(new NNNode()));
        return *mHiddenNodes.back();
    }

    void NeuralNetwork::addNodeOnConnection(Connection &conn) {
        // disable the connection
        conn.enabled = false;

        // create a node
        NNNode &newHidden = createFloatingHidden();

        // create two new connections
        mConnections.push_back(std::unique_ptr<Connection>(new Connection(*conn.input, newHidden)));
        // weight = 1 by default
        mConnections.push_back(std::unique_ptr<Connection>(new Connection(newHidden, *conn.output)));
        mConnections.back()->weight = conn.weight;
    }

    void NeuralNetwork::addNodeOnRandomConnection() {
        addNodeOnConnection(*mConnections[slug::math::randi(0, mConnections.size() - 1)]);
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

        NNNode &output = *outputCandidates[math::randi(0, outputCandidates.size()-1)];

        // check if the new connection is a duplicate
        for (auto const &conn : mConnections) {
            if (input.nodeID == conn->input->nodeID && output.nodeID == conn->output->nodeID) {
                std::cout << "Duplicate connection created: (" << conn->input->nodeID << " -> " << conn->output->nodeID << "). Discarding.\n";
                return;
            }
        }

        auto newConnection = std::unique_ptr<Connection>(new Connection(input, output));
        std::cout << "createNewRandomConnection done. " << newConnection->input->nodeID << " -> " << newConnection->output->nodeID << ".\n";
        mConnections.push_back(std::move(newConnection));
    }

    void NeuralNetwork::mutate() {
        // addNodeOnConnection(*mConnections[math::randi(0, mConnections.size() - 1)]);
        createNewRandomConnection();

        // test run
        std::vector<float> results = run();
        for (auto f : results) {
            std::cout << f << " ";
        }
        std::cout << "\n";

        std::cout << "mutate() done.\n";
    }

    NeuralNetwork NeuralNetwork::mate(NeuralNetwork &other) const {
        std::cout << "Combining two brains to make a new one...\n";

        NeuralNetwork newBrain;
        std::map<unsigned int, NNNode*> newNodes;

        // merge connections
        std::cout << "connections of first:\n";
        for (auto &c : mConnections) {
            c->printInfo();
        }

        std::cout << "connections of second:\n";
        for (auto &c : other.mConnections) {
            c->printInfo();
        }

        auto firstIt = mConnections.begin();
        auto otherIt = other.mConnections.begin();

        while (firstIt != mConnections.end() && otherIt != other.mConnections.end()) {
            if ((*firstIt)->innovNum == (*otherIt)->innovNum) {
                std::cout << "both innovNums match. (" << (*firstIt)->innovNum << ")\n";
                
                // same innov num, choose random
                if (math::randi(1, 100) <= 50) {
                    // chose first
                    newBrain.mConnections.push_back(std::unique_ptr<Connection>(new Connection(**firstIt)));
                } else {
                    // chose other
                    newBrain.mConnections.push_back(std::unique_ptr<Connection>(new Connection(**otherIt)));
                }

                // move on
                firstIt++;
                otherIt++;
            } else { 
                // process the pointer w/ the smallest innovnum
                auto& smallIt = firstIt;
                if ((*firstIt)->innovNum > (*otherIt)->innovNum) {
                    smallIt = otherIt;
                }

                // TODO: only inherit if parent has higher fitness

                std::cout << "innovNums don't match. processing innovNum " << (*smallIt)->innovNum << ".\n";

                newBrain.mConnections.push_back(std::unique_ptr<Connection>(new Connection(**smallIt)));

                smallIt++;
            }
        }

        // finish up the one that isn't at the end yet
        while (firstIt != mConnections.end()) {
            std::cout << "finishing up. processing innovNum " << (*firstIt)->innovNum << ".\n";
            newBrain.mConnections.push_back(std::unique_ptr<Connection>(new Connection(**firstIt)));
            firstIt++;
        }

        while (otherIt != other.mConnections.end()) {
            std::cout << "finishing up. processing innovNum " << (*otherIt)->innovNum << ".\n";
            newBrain.mConnections.push_back(std::unique_ptr<Connection>(new Connection(**otherIt)));
            otherIt++;
        }

        std::cout << "connections of new:\n";
        for (auto &c : newBrain.mConnections) {
            c->printInfo();
        }

        // copy inputs and outputs
        // make clones of input nodes
        std::transform(
            mInputNodes.begin(),
            mInputNodes.end(),
            std::back_inserter(newBrain.mInputNodes),
            [&newNodes](const std::unique_ptr<NNInputNode>&n) {
                auto copy = std::unique_ptr<NNInputNode>(new NNInputNode(n->mInputFunc));
                *copy = *n;
                // save id in newNodes for later use in reconnecting connections
                newNodes.insert(std::pair<unsigned int, NNNode*>(copy->nodeID, copy.get()));
                return copy;
            }
        );

        // make clones of output nodes
        std::transform(
            mOutputNodes.begin(),
            mOutputNodes.end(),
            std::back_inserter(newBrain.mOutputNodes),
            [&newNodes](const std::unique_ptr<NNNode> &n) {
                auto copy = std::unique_ptr<NNNode>(new NNNode());
                *copy = *n;
                // save id in newNodes for later use in reconnecting connections
                newNodes.insert(std::pair<unsigned int, NNNode*>(copy->nodeID, copy.get()));
                return copy;
            }
        );

        // scan connections and create hidden layers' structure
        for (const auto &c : newBrain.mConnections) {
            // if the connection's input/output hasn't been created, create it now
            if (newNodes.count(c->input->nodeID) == 0) {
                newBrain.mHiddenNodes.push_back(std::unique_ptr<NNNode>(new NNNode(*c->input)));
                newNodes.insert({c->input->nodeID, newBrain.mHiddenNodes.back().get()});
            }

            if (newNodes.count(c->output->nodeID) == 0) {
                newBrain.mHiddenNodes.push_back(std::unique_ptr<NNNode>(new NNNode(*c->output)));
                newNodes.insert({c->output->nodeID, newBrain.mHiddenNodes.back().get()});
            }

            // at this point the connection points back into the parent nodes
            // let's fix that by adjusting the pointers to point to nodes in our map
            c->input = newNodes.at(c->input->nodeID);
            c->output = newNodes.at(c->output->nodeID);
        }

        // done
        return newBrain;
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

        // mark all nodes as unseen
        for (auto &node : mInputNodes) {
            node->seen = false;
        }
        for (auto &node : mHiddenNodes) {
            node->seen = false;
        }
        for (auto &node : mOutputNodes) {
            node->seen = false;
        }

        // recursively calculate the value of each output node
        for (auto &outputNode : mOutputNodes) {
            outputs.push_back(calculateValue(outputNode.get()));
        }

        return outputs;
    }

    float NeuralNetwork::calculateValue(NNNode *node) {
        float sum = 0;
        std::vector<NNNode*> pendingNodes;
        std::map<NNNode*, float> pendingWeights;

        if (node->seen) {
            // we've already been here, just return the current value
            return node->getValue();
        }

        // mark node as seen
        node->seen = true;

        // build vector of nodes that need to be calculated first
        for (auto const &conn : mConnections) {
            if (*conn->output == *node && conn->enabled) {
                pendingNodes.push_back(conn->input);
                pendingWeights.insert(std::pair<NNNode*, float>(conn->input, conn->weight));
            }
        }

        // if the vector is empty, node is an input node.
        if (pendingNodes.empty()) {
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