#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>

class Point
{
    friend std::ostream & operator<<(std::ostream & output, const Point & point)
    {
        output << "( " << point.x() << " " << point.y() << " )";
        return output ;
    }
    public:
        Point() {}
        ~Point() {}

        double x() { return _x; }
        double y() { return _y; } 
        double x() const { return _x; }
        double y() const { return _y; } 

        void setXY(double x, double y) { _x = x; _y = y;  }
    private:
        double _x;
        double _y; 
};
class Rectangle
{
    friend std::ostream & operator<<(std::ostream & output, const Rectangle & rectangle)
    {
        // output << "center: " << "( " << rectangle.x() << " " << rectangle.y() << " )" << " ";
        // output << "width: " << rectangle.width() << " " << "length: " << rectangle.length() << " " ;
        // output << "degree: " << rectangle.degree() << " ";
        // output << "left: " << rectangle.left() << " ";
        // output << "right: " << rectangle.right() << " ";
        // output << "up: " << rectangle.up() << " ";
        // output << "down: " << rectangle.down() << " ";
        return output ;
    }
    public:
        Rectangle() {}
        Rectangle(int width, int height, int x, int y)
            :_width(width), _height(height) 
        {
            _bottomLeftCorner.setXY(x, y);
        }
        ~Rectangle() {}

        void setWidth(int width) { _width = width; }
        void setHeight(int height) { _height = height; }
        void setBottomLeftCorner(int x, int y) { _bottomLeftCorner.setXY(x, y); }

        int width() { return _width; }
        int height() { return _height; } 
        int leftBoundary() { return _bottomLeftCorner.x(); }
        int DownBoundary() { return _bottomLeftCorner.y(); }
        int rightBoundary() { return _bottomLeftCorner.x() + _width; }
        int upBoundary() { return _bottomLeftCorner.y() + _height; }
        int bottomLeftCornerX() { return _bottomLeftCorner.x(); }
        int bottomLeftCornerY() { return _bottomLeftCorner.y(); }
        Point & bottomLeftCorner() { return _bottomLeftCorner; } 

    private:
        int _width;
        int _height = 5;
        Point _bottomLeftCorner;
};

#endif
