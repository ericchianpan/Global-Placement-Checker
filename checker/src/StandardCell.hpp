#ifndef STANDARDCELL_H 
#define STANDARDCELL_H 

#include <iostream>
#include <string>

#define cellHeight 5;

// class Rectangle{
    // public:
        // int centerX,
            // centerY;
        // int minX, 
            // minY, 
            // maxX, 
            // maxY;
    // private:
// };

class StandardCell{
    public:
        StandardCell() {}
        StandardCell(std::string cellName): _cellName(cellName) {};
        ~StandardCell() {}

        void setX(int x) { _x = x; }       
        void setY(int y) { _y = y; }       
        void setLocation(int x, int y) { _x = x; _y = y; }
        void setWidth(int width) { _width = width; }       
        void setName(std::string cellName) { _cellName = cellName; }

        int x() { return _x; }
        int y() { return _y; }
        int width() { return _width; }
        int height() { return _height; }
        std::string cellName() { return _cellName; }

    private:
        int _width;
        int _height = cellHeight;
        int _x;
        int _y;
        std::string _cellName;
};

#endif
