#ifndef PLACEMENT_HPP 
#define PLACEMENT_HPP 

#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include "Geometry.hpp"

using namespace std;

const int cellHeight = 5;
const int padWidth = 30;
const int padHeight = 150;

enum TYPE { 
    CELL, 
    PAD 
};

class StandardCell : public Rectangle {
    public:
        StandardCell() {}
        StandardCell(std::string cellName, TYPE type)
            :_cellName(cellName), _type(type) {}
        ~StandardCell() {}

        void setCenter(double x, double y) { _center.setXY(x, y); }       
        void setName(std::string cellName) { _cellName = cellName; }
        void setType(TYPE type) { _type = type; }
        void addNet(string cellName) { _nets.push_back(cellName); }

        unsigned numFanouts() { return _nets.size(); }
        double centerX() { return _center.x(); }
        double centerY() { return _center.y(); }
        string cellName() { return _cellName; }
        string net(int idx) { return _nets[idx]; }
        TYPE type() { return _type; }
        Point & center() { return _center; }

    private:
        std::string _cellName;
        Point _center;
        TYPE _type;
        vector<string> _nets;
};

class Placement{
    public:
        Placement() {}
        Placement(int coreWidth, int coreHeight)
            :_coreWidth(coreWidth), _coreHeight(coreHeight) {}
        ~Placement() {}

        void plParse(string plFile);
        void blifParse(string blifFile);
        void initial();

        bool checkCorrectness();
        bool adjacentCellOverlap(StandardCell & cellOne, StandardCell & cellTwo)
        { return cellTwo.leftBoundary() < cellOne.rightBoundary(); }
        double calcHPWL();
        string model() { return _model; }

        void printCells();
        void printRowCells();
        void printNets();
    private:
        int _coreWidth, _coreHeight;
        int _coreLeftBoundary = 200;
        int _coreDownBoundary = 200;
        int _coreRightBoundary; 
        int _coreUpBoundary; 
        int _AND_COUNT = 0;
        int _OR_COUNT = 0;
        int _NOT_COUNT = 0;
        string _model;
        Point _bottomLeftCorner;

        map<string, StandardCell> _cells;
        set<string> _pads;

        set<string> _placedCells; 
        map<int, vector<StandardCell>> _rowPlacedCells; 
};

#endif
