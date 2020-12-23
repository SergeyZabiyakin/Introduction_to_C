#include "primitives.h"


Node::Node(const Point &p, int m) : point(p.x(), p.y()) {
    this->mod = m;
}

const Point &Node::getPoint() const {
    return point;
}

std::shared_ptr<Node> &Node::getLeftNode() {
    return leftNode;
}

std::shared_ptr<Node> &Node::getRightNode() {
    return rightNode;
}

void Node::setLeftNode(Node *node) {
    Node::leftNode.reset(node);
}

void Node::setRightNode(Node *node) {
    Node::rightNode.reset(node);
}

bool Node::isAlive() const {
    return alive;
}

bool Node::dependence(const Point &p) const {
    if (mod == 0) {
        return p.x() < point.x();
    } else {
        return p.y() < point.y();
    }
}

void Node::setAlive(bool live) {
    Node::alive = live;
}


void Tree::put(const Point &p) {
    if (!p_node) {
        p_node.reset(new Node(p, 0));
    } else {
        reallyPut(p_node, p);
    }
}

void Tree::reallyPut(std::shared_ptr<Node> &node, const Point &p) {
    if (node->dependence(p)) {
        if (node->getLeftNode()) {
            reallyPut(node->getLeftNode(), p);
        } else {
            node->setLeftNode(new Node(p, (node->mod + 1) % 2));
        }
    } else {
        if (node->getRightNode()) {
            (reallyPut(node->getRightNode(), p));
        } else {
            node->setRightNode(new Node(p, (node->mod + 1) % 2));
        }
    }
}

std::shared_ptr<Node> Tree::getPNode() const {
    return p_node;
}

