#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "pch.hpp"

class Point {
public:
    Point (double x = 0.0, double y = 0.0);

    double getX();
    double getY();

    void setX(double x);
    void setY(double y);

    bool compare(Point &point, double margin) {
        if(std::fabs(X) - std::fabs(point.X) < margin && std::fabs(Y) - std::fabs(point.Y) < margin) return true;

        return false;
    }

    Point operator+(Point const& p) {
        return Point(X + p.X, Y + p.Y);
    }

    Point operator-(Point const& p) {
        return Point(X - p.X, Y - p.Y);
    }

    void operator+=(Point const& p) {
        X += p.X;
        Y += p.Y;
    }

    void operator-=(Point const& p) {
        X -= p.X;
        Y -= p.Y;
    }

private:
    double X = 0.0, Y = 0.0;
};

#endif
