#include "geometry.hpp"

Point::Point (double x, double y) {
    X = x;
    Y = y;
}

double Point::getX() {
    return X;
}

double Point::getY() {
    return Y;
}

void Point::setX(double x) {
    X = x;
}

void Point::setY(double y) {
    Y = y;
}
