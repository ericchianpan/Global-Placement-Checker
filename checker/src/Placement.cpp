#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cfloat>
#include <algorithm>
#include "Placement.hpp"

using namespace std;

void Placement::initial()
{
    _bottomLeftCorner.setXY(200, 200);
    _coreRightBoundary = 200 + _coreWidth;
    _coreUpBoundary = 200 + _coreHeight;
}
void Placement::plParse(std::string plFile)
{
    fstream fin(plFile, fstream::in);
    if(!fin) {
        cout << "can not open pl file" << endl;
        exit(1);
    }

    string content;
    while(getline(fin, content)) {
        istringstream iss(content);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
        map<string, StandardCell>::iterator it = _cells.find(tokens[0]);
        if(it == _cells.end()) {
            cout << "can not find cell " << tokens[0] << endl;
            exit(1);
        }

        StandardCell & cell = it->second;
        cell.setBottomLeftCorner(stoi(tokens[1]), stoi(tokens[2]));
        _placedCells.insert(tokens[0]);
        if(cell.type() == CELL) {
            cell.setCenter(cell.bottomLeftCornerX() + (double)cell.width() / 2, cell.bottomLeftCornerY() + (double)cellHeight / 2);
            map<int, vector<StandardCell>>::iterator row = _rowPlacedCells.find(stoi(tokens[2]));
            if(row == _rowPlacedCells.end()) _rowPlacedCells.insert(make_pair(stoi(tokens[2]), vector<StandardCell>{cell}));
            else row->second.push_back(cell);
        }
        else if(cell.type() == PAD) {
            if(cell.bottomLeftCornerX() == 0 || cell.bottomLeftCornerX() == 250 + _coreWidth) 
                cell.setCenter(cell.bottomLeftCornerX() + (double)padHeight / 2, cell.bottomLeftCornerY() + (double)padWidth / 2);
            else if(cell.bottomLeftCornerY() == 0 || cell.bottomLeftCornerY() == 250 + _coreHeight) 
                cell.setCenter(cell.bottomLeftCornerY() + (double)padWidth / 2, cell.bottomLeftCornerY() + (double)padHeight / 2);
            // else {
                // cout << "pad position is illegal" << endl;
                // cout << cell.cellName() << endl;
                // cout << cell.bottomLeftCorner() << endl;
                // exit(1);
            // }
        }
        else {
            cout << "cell type is undefined" << endl;
            exit(1);
        }
    }
}
void Placement::blifParse(string blifFile)
{
    fstream fin(blifFile, fstream::in);
    if(!fin) {
        cout << "can not open blif file" << endl;
        exit(1);
    }

    string content;
    while(fin >> content) {
        if(content == ".model") { 
            fin >> content;
            _model = content; 
        }

        if(content == ".inputs") {
            while(true) {
                fin >> content;
                if(content != "\\") {
                    _cells.insert(make_pair(content, StandardCell(content, PAD)));
                    _pads.insert(content);
                    if(fin.peek() == '\n') break;
                }
            }
        }

        if(content == ".outputs") {
            while(true) {
                fin >> content;
                if(content != "\\") {
                    StandardCell cell(content, PAD);
                    _cells.insert(make_pair(content, StandardCell(content, CELL)));
                    _cells.insert(make_pair(content + "_opad", StandardCell(content + "_opad", PAD)));
                    _pads.insert(content + "_opad");
                    if(fin.peek() == '\n') break;
                }
            }
        }

        if(content == ".names") {
            vector<string> logicVertexs, logics;
            while(true) {
                fin >> content;
                logicVertexs.push_back(content);
                if(fin.peek() == '\n') break;
            }
            while(true) {
                string str;
                getline(fin, str);
                if(!str.empty()) logics.push_back(str);
                if(fin.peek() == '.') break;
            }
            
            if(logics.size() == 2) {
                _OR_COUNT++;
            }
            else {
                if(logicVertexs.size() == 3) _AND_COUNT++;
                else _NOT_COUNT++;
            }

            // construct nets
            for(unsigned i = 0; i < logicVertexs.size() - 1; ++i) {
                _cells.find(logicVertexs[i])->second.addNet(logicVertexs.back());
            }

            StandardCell cell(logicVertexs.back(), CELL);
            cell.setWidth(logicVertexs.size() - 1 + logics.size());
            _cells.insert(make_pair(logicVertexs.back(), cell));
        }
        if(content == ".end") break;
    }
    fin.close();

    // cout << "AND Count: " << _AND_COUNT << endl;
    // cout << "OR Count: " << _OR_COUNT << endl;
    // cout << "NOT Count: " << _NOT_COUNT << endl;
}
void Placement::printCells()
{
    for(map<string, StandardCell>::iterator it = _cells.begin(); it != _cells.end(); ++it) {
            StandardCell & cell = it->second;
            cout << "cell name: " << cell.cellName() << ", " << "cell width: " << cell.width() << endl;
    }
}
void Placement::printRowCells()
{
    for(map<int, vector<StandardCell>>::iterator row = _rowPlacedCells.begin(); row != _rowPlacedCells.end(); ++row) {
        cout << "row " << row->first << ": ";
        sort(row->second.begin(), row->second.end(), 
                [](StandardCell & cellOne, StandardCell & cellTwo)->bool { return cellOne.bottomLeftCornerX() < cellTwo.bottomLeftCornerX(); });
        for(unsigned i = 0; i < row->second.size(); ++i) {
            StandardCell & cell = row->second[i];
            cout << cell.cellName() << endl;
            cout << "bottom left corner: " << cell.bottomLeftCorner() << endl;
            cout << "width: " << cell.width() << endl;
            cout << "height: " << cell.height() << endl;
        }
        cout << endl;
    }
}
double Placement::calcHPWL()
{
    double HPWL = 0;
    for(map<string, StandardCell>::iterator it = _cells.begin(); it != _cells.end(); ++it) {
        StandardCell & cell = it->second;
        if(cell.numFanouts() == 0) continue;
        double xMin = DBL_MAX; double yMin = DBL_MAX;
        double xMax = 0; double yMax = 0;
        cell.addNet(cell.cellName());
        for(unsigned i = 0; i < cell.numFanouts(); ++i) {
            StandardCell & netCell = _cells.find(cell.net(i))->second;
            if(netCell.centerX() > xMax) xMax = netCell.centerX();
            if(netCell.centerY() > yMax) yMax = netCell.centerY();
            if(netCell.centerX() < xMin) xMin = netCell.centerX();
            if(netCell.centerY() < yMin) yMin = netCell.centerY();
        }
        HPWL = HPWL + (xMax - xMin) + (yMax - yMin);
    }
    return HPWL;
}
bool Placement::checkCorrectness()
{
    /* check all cell and pad are placed
       check all placed locations are legal
       check wheather overlap */

    /* check all cell and pad are placed, pad location */
    for(map<string, StandardCell>::iterator it = _cells.begin(); it != _cells.end(); ++it) {
        if(_placedCells.find(it->first) == _placedCells.end()) {
            cout << "Cell " << it->first << " has not placed" << endl;
            return false;
        }

        StandardCell & cell = it->second;
        if(cell.type() == PAD) {
            if(cell.bottomLeftCornerX() != 0 && cell.bottomLeftCornerY() != 0 &&
               cell.bottomLeftCornerX() != 250 + _coreWidth && cell.bottomLeftCornerY() != 250 + _coreHeight) {
                cout << "Position of I/O pad " << cell.cellName() << " is illegal" << endl;
                return false;
            }
        }
    }

    cout << "[check] All cell and I/O pad are placed" << endl;
    cout << "[check] I/O pad locations are legal" << endl;

    /* check all placed location are legal and wheather overlap */
    for(map<int, vector<StandardCell>>::iterator row = _rowPlacedCells.begin(); row != _rowPlacedCells.end(); ++row) {
        if(row->first % 5 != 0 || row->first > _coreUpBoundary - 5) {
            cout << "cell ";
            for(unsigned i = 0; i < row->second.size(); ++i) {
                cout << row->second[i].cellName();
                if(i != row->second.size() - 1) cout << ", ";
            }
            cout << " are not in the cell row" << endl;
            return false;
        }
        // sort the cells according to the x coorsinate in each row
        sort(row->second.begin(), row->second.end(), 
                [](StandardCell & cellOne, StandardCell & cellTwo)->bool { return cellOne.bottomLeftCornerX() < cellTwo.bottomLeftCornerX(); });
        for(unsigned i = 0; i < row->second.size() - 1; ++i) {
            if(i == 0) {
                if(row->second[i].leftBoundary() < _coreLeftBoundary) {
                    cout << "Cell " << row->second[i].cellName() << " is out of core left boundary" << endl;
                    return false;
                }
            }
            if(i == row->second.size() - 2) {
                if(row->second[i + 1].rightBoundary() > _coreRightBoundary) {
                    cout << "Cell " << row->second[i].cellName() << " is out of core right boundary" << endl;
                    return false;
                }
            }

            if(adjacentCellOverlap(row->second[i], row->second[i + 1])) {
                cout << "Cell " << row->second[i].cellName() << " and " << row->second[i + 1].cellName() << " are overlap" << endl;
                return false;
            }
        }
    }
    
    cout << "[check] Cell locations are legal" << endl;
    cout << "[check] No cell overlap" << endl;

    return true;
}
void Placement::printNets()
{
    for(map<string, StandardCell>::iterator it = _cells.begin(); it != _cells.end(); ++it) {
        StandardCell & cell = it->second;
        cout << "cell " << cell.cellName() << " fanout: ";
        for(unsigned i = 0; i < cell.numFanouts(); ++i) {
            cout << cell.net(i) << " ";
        }
        cout << endl;
    }
}
