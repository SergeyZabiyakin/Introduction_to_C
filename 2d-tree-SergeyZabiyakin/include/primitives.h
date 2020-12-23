#pragma once

#include <memory>
#include <ostream>
#include <optional>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <functional>
#include <stack>
#include <iostream>
#include <set>
#include <limits>
#include <memory>

class Point {
public:

    Point(double x, double y) {
        X = x;
        Y = y;
    }

    Point() {
        X = 0;
        Y = 0;
    }

    double x() const {
        return this->X;
    }

    double y() const {
        return this->Y;
    }

    double distance(const Point &p) const {
        return sqrt(pow(X - p.x(), 2) + pow(Y - p.y(), 2));
    }

    bool operator<(const Point &p) const {
        return p.x() != this->X ? this->X < p.x() : this->Y < p.y();
    }

    bool operator>(const Point &p) const {
        return p.x() != this->X ? this->X > p.x() : this->Y > p.y();
    }

    bool operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point &p) const {
        return *this > p || *this == p;
    }

    bool operator==(const Point &p) const {
        return this->X == p.x() && this->Y == p.y();
    }

    bool operator!=(const Point &p) const {
        return !(*this == p);
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << "(" << p.x() << " , " << p.y() << ")";
        return os;
    }

private:
    double X;
    double Y;
};

class Rect {
public:

    Rect(const Point &left_bottom, const Point &right_top) {
        Xmin = left_bottom.x();
        Ymin = left_bottom.y();
        Xmax = right_top.x();
        Ymax = right_top.y();
    }

    [[nodiscard]] double xmin() const {
        return Xmin;
    }

    [[nodiscard]] double ymin() const {
        return Ymin;
    }

    [[nodiscard]] double xmax() const {
        return Xmax;
    }

    [[nodiscard]] double ymax() const {
        return Ymax;
    }

    double distance(const Point &p) const {
        if (contains(p)) return 0;
        if (p.x() >= Xmin && p.x() <= Xmax) {
            return std::min(std::abs(p.y() - Ymin), std::abs(p.y() - Ymax));
        }
        if (p.y() >= Ymin && p.y() <= Ymax) {
            return std::min(std::abs(p.x() - Xmin), std::abs(p.x() - Xmax));
        }
        return std::min(std::min(Point(xmin(), ymin()).distance(p),
                                 Point(xmax(), ymin()).distance(p)),
                        std::min(Point(xmin(), ymax()).distance(p),
                                 Point(xmax(), ymax()).distance(p)));
    }

    bool contains(const Point &p) const {
        return p.x() >= Xmin && p.x() <= Xmax && p.y() >= Ymin && p.y() <= Ymax;
    }

    bool intersects(const Rect &r) const {
        return contains(Point(r.xmin(), r.ymin()))
               || contains(Point(r.xmax(), r.ymin()))
               || contains(Point(r.xmin(), r.ymax()))
               || contains(Point(r.xmax(), r.ymax()))
               || r.contains(Point(xmin(), ymin()))
               || r.contains(Point(xmax(), ymin()))
               || r.contains(Point(xmin(), ymax()))
               || r.contains(Point(xmax(), ymax()));
    }

    std::pair<std::optional<Rect>, std::optional<Rect>> splitX(double x) const {
        if (x >= Xmin && x <= Xmax) {
            return std::pair(Rect(Point(Xmin, Ymin), Point(x, Ymax)),
                             Rect(Point(x, Ymin), Point(Xmax, Ymax)));
        } else if (x > Xmax) {
            return std::pair(*this, std::nullopt);
        } else {
            return std::pair(std::nullopt, *this);
        }
    }

    std::pair<std::optional<Rect>, std::optional<Rect>> splitY(double y) const {
        if (y >= Ymin && y <= Ymax) {
            return std::pair(Rect(Point(Xmin, Ymin), Point(Xmax, y)),
                             Rect(Point(Xmin, y), Point(Xmax, Ymax)));
        } else if (y > Ymax) {
            return std::pair(*this, std::nullopt);
        } else {
            return std::pair(std::nullopt, *this);
        }
    }

private:
    double Xmin;
    double Ymin;
    double Xmax;
    double Ymax;
};

class Iterator : public std::iterator<std::forward_iterator_tag, Point> {
public:
    Iterator(std::vector<Point> vec, std::size_t c = 0) : vector(std::move(vec)) {
        cur = c;
    }

    Iterator() {
        cur = 0;
    }

    Iterator(const Iterator &it) : vector(it.vector) {
        cur = it.cur;
    }

    Iterator(const Iterator &it, std::size_t c ) : vector(it.vector) {
        cur = c;
    }

    const Point &operator*() const {
        return vector[cur];
    }

    const Point *operator->() const {
        return &vector[cur];
    }

    Iterator &operator++() {
        ++cur;
        return *this;
    }

    Iterator &operator=(const Point &p) {
        if (vector.size() < cur) {
            vector.resize(cur);
        }
        vector.push_back(p);
        return *this;
    }

    Iterator operator++(int) {
        auto tmp = *this;
        operator++();
        return tmp;
    }

    friend bool operator==(const Iterator &l, const Iterator &r) {
        return l.vector == r.vector && l.cur == r.cur;
    }

    friend bool operator!=(const Iterator &l, const Iterator &r) {
        return !(l == r);
    }

private:
    std::vector<Point> vector;
    std::size_t cur;
};

namespace rbtree {

    class PointSet {
    public:

        using ForwardIt = Iterator;

        PointSet() {
            Size = 0;
        }

        bool empty() const {
            return Size == 0;
        }

        std::size_t size() const {
            return Size;
        }

        void put(const Point &p) {
            if (!contains(p)) {
                rbmap.insert(p);
                iterator = p;
                ++iterator;
                ++Size;
            }
        }

        bool contains(const Point &p) const {
            return rbmap.count(p) != 0;
        }

        std::pair<ForwardIt, ForwardIt> range(const Rect &r) const {
            Iterator it = Iterator();
            for (auto point : rbmap) {
                if (r.contains(point)) {
                    it = point;
                    ++it;
                }
            }
            return std::pair(Iterator(it, 0), it);
        }

        ForwardIt begin() const {
            return Iterator(iterator, 0);
        }

        ForwardIt end() const {
            return Iterator(iterator);
        }

        std::optional<Point> nearest(const Point &p) const {
            if (Size == 0) return std::nullopt;
            Point pmin = *iterator;
            double min_distance = std::numeric_limits<double>::max();;
            for (Point point : rbmap) {
                if (point.distance(p) < min_distance) {
                    pmin = point;
                    min_distance = point.distance(p);
                }
            }
            return pmin;
        }


        std::pair<ForwardIt, ForwardIt> nearest(const Point &p, std::size_t k) const {
            Iterator it = Iterator();
            std::vector<bool> vector(Size);
            auto end = Iterator(iterator);
            for (std::size_t i = 0; i < k; i++) {
                double min_distance = std::numeric_limits<double>::max();
                auto begin = Iterator(iterator, 0);
                Point pmin = *begin;
                std::size_t jpmin = 0;
                for (std::size_t j = 0; j < Size; j++) {
                    if ((*begin).distance(p) < min_distance && vector[j] == false) {
                        pmin = *begin;
                        jpmin = j;
                        min_distance = (*begin).distance(p);
                    }
                    ++begin;
                }
                it = pmin;
                ++it;
                vector[jpmin] = true;
            }
            return std::pair(Iterator(it, 0), it);
        }

        friend std::ostream &operator<<(std::ostream &os, const PointSet &pointSet) {
            os << "{";
            auto point = pointSet.begin();
            while (point != pointSet.end()) {
                os << *point;
            }
            os << "}";
            return os;
        }

    private:
        size_t Size;
        std::set<Point> rbmap;
        Iterator iterator = Iterator();
    };

}

class Node {
public:
    Node(const Point &p, int m);

    const Point &getPoint() const;

    bool isAlive() const;

    bool dependence(const Point &point) const;

    void setAlive(bool live);

    std::shared_ptr<Node> &getLeftNode();

    std::shared_ptr<Node> &getRightNode();

    void setLeftNode(Node *leftNode);

    void setRightNode(Node *rightNode);


    int mod;
private:
    bool alive = true;
    Point point;
    std::shared_ptr<Node> leftNode;
    std::shared_ptr<Node> rightNode;
};

class Tree {
public:

    void put(const Point &p);

    std::shared_ptr<Node> getPNode() const;

private:
    void reallyPut(std::shared_ptr<Node> &node, const Point &p);

    std::shared_ptr<Node> p_node;
};

namespace kdtree {
    class PointSet {
    public:

        using ForwardIt = Iterator;

        PointSet() {
            Size = 0;
        }

        bool empty() const {
            return Size == 0;
        }

        std::size_t size() const {
            return Size;
        }

        void put(const Point &p) {
            if (!contains(p)) {
                if (p.x() < Xmin)Xmin = p.x();
                if (p.x() > Xmax)Xmax = p.x();
                if (p.y() < Ymin)Ymin = p.y();
                if (p.y() > Ymax)Ymax = p.y();
                tree.put(p);
                iterator = p;
                ++iterator;
                Size++;
            }
        }

        bool contains(const Point &p) const {
            if (Size == 0) return false;
            return utilityForContains(tree.getPNode(), p);
        }

        std::pair<ForwardIt, ForwardIt> range(const Rect &rect) const {
            Iterator it = Iterator();
            std::shared_ptr<Node> node = tree.getPNode();
            std::optional<Rect> r = rect;
            utilityForRange(r, it, node);
            return std::pair(Iterator(it, 0), it);
        }

        ForwardIt begin() const {
            return Iterator(iterator, 0);
        }

        ForwardIt end() const {
            return Iterator(iterator);
        }

        std::optional<Point> nearest(const Point &p) const {
            if (Size == 0) return std::nullopt;
            std::stack<std::shared_ptr<Node>> stack;
            stack.push(std::make_shared<Node>(p, 0));
            double minDistance = std::numeric_limits<double>::max();
            utilityForNearest(Rect(Point(Xmin, Ymin), Point(Xmax, Ymax)), tree.getPNode(), stack, p, minDistance);
            stack.top()->setAlive(true);
            return stack.top()->getPoint();
        }

        std::pair<ForwardIt, ForwardIt> nearest(const Point &p, std::size_t k) const {
            if (k > Size) {
                k = Size;
            }
            Iterator it = Iterator();
            std::stack<std::shared_ptr<Node>> stack;
            for (std::size_t i = 0; i < k; i++) {
                stack.push(std::make_shared<Node>(p, 0));
                double minDistance = std::numeric_limits<double>::max();
                utilityForNearest(Rect(Point(Xmin, Ymin),
                                       Point(Xmax, Ymax)), tree.getPNode(), stack, p, minDistance);
                it = (stack.top()->getPoint());
                ++it;
            }
            while (!stack.empty()) {
                stack.top()->setAlive(true);
                stack.pop();
            }
            return std::pair(Iterator(it, 0), it);
        }

        friend std::ostream &operator<<(std::ostream &os, const PointSet &pointSet) {
            os << "{";
            auto point = pointSet.begin();
            while (point != pointSet.end()) {
                os << *point;
            }
            os << "}";
            return os;
        }

    private:
        bool utilityForContains(std::shared_ptr<Node> node, const Point &p) const {
            if (node->getPoint() == p) { return true; }
            if (node->dependence(p)) {
                if (node->getLeftNode()) {
                    return utilityForContains(node->getLeftNode(), p);
                } else {
                    return false;
                }
            } else {
                if (node->getRightNode()) {
                    return utilityForContains(node->getRightNode(), p);
                } else {
                    return false;
                }
            }
        }

        void utilityForNearest(Rect rect, std::shared_ptr<Node> node, std::stack<std::shared_ptr<Node>> &stack,
                               const Point &p, double &minDistance) const {
            if (node->isAlive() && node->getPoint().distance(p) < minDistance) {
                minDistance = node->getPoint().distance(p);
                std::shared_ptr<Node> n = stack.top();
                stack.pop();
                n->setAlive(true);
                node->setAlive(false);
                stack.push(node);
            }
            std::pair<std::optional<Rect>, std::optional<Rect>> pair;
            if (node->mod == 0) {
                pair = rect.splitX(node->getPoint().x());
            } else {
                pair = rect.splitY(node->getPoint().y());
            }
            if (pair.first.has_value() && pair.first->distance(p) < minDistance && node->getLeftNode()) {
                utilityForNearest(pair.first.value(), node->getLeftNode(), stack, p, minDistance);
            }
            if (pair.second.has_value() && pair.second->distance(p) < minDistance && node->getRightNode()) {
                utilityForNearest(pair.second.value(), node->getRightNode(), stack, p, minDistance);
            }
        }

        void utilityForRange(std::optional<Rect> &rect, Iterator &it, std::shared_ptr<Node> &node) const {
            if (!rect.has_value()) {
                return;
            }
            if (rect->contains(node->getPoint())) {
                it = node->getPoint();
                ++it;
            }
            std::pair<std::optional<Rect>, std::optional<Rect>> pair;
            if (node->mod == 0) {
                pair = rect->splitX(node->getPoint().x());
            } else {
                pair = rect->splitY(node->getPoint().y());
            }

            if (node->getLeftNode()) {
                utilityForRange(pair.first, it, node->getLeftNode());
            }
            if (node->getRightNode()) {
                utilityForRange(pair.second, it, node->getRightNode());
            }
        }

        size_t Size;
        Tree tree;
        Iterator iterator = Iterator();
        double Xmin = std::numeric_limits<double>::max();
        double Ymin = std::numeric_limits<double>::max();
        double Xmax = std::numeric_limits<double>::min();
        double Ymax = std::numeric_limits<double>::min();
    };
}
