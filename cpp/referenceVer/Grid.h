#include <bits/stdc++.h>
#include "Line.h"

using namespace std;

class Grid
{
public:
    struct xyc
    {
        int x;
        int y;
        char color;
    };

    vector<Line*> lines;
    string state = "";
    vector<xyc> updatedPixels;

    Grid(){
    }

    Grid(vector<Line*> inputLines)
    {
        lines = inputLines;
        state = "";
        updatedPixels = vector<xyc>();
    }


    Grid(const Grid& G){
        for (Line* line: G.lines){
            Line* newLine = new Line(line->d);
            newLine -> s = line -> s;
            lines.push_back(newLine);
        }
        state = G.state;
        for (auto xy : G.updatedPixels){
            updatedPixels.push_back(xy);
        }
    }

    void updatePixel(xyc xyc){
        updatePixel(xyc.x, xyc.y, xyc.color);
    }

    void updatePixel(int x, int y, char c){

        // cout << lines[x].s[y] << endl;

        // cout << "x: " << x << " y: " << y << " " << c<< endl;
        lines[x] -> updatePixel(y, c);
        // cout << "x: " << y + (x >= HOWBIG ? 0 : HOWBIG) << " y: " << x % 25 << " " << c<< endl;
        lines[y + (x >= HOWBIG ? 0 : HOWBIG)] -> updatePixel(x % 25, c);

        updatedPixels.push_back(xyc{x, y ,c});
    }
    
    vector<pair<int, int>> getUnknowPixels(){
        vector<pair<int, int>> returnList;
        for(int i = 0; i < HOWBIG; i++){
            Line* line = lines[i];
            for (int j = 0; j < HOWBIG; j++){
                if (line -> s[j] == 'u'){
                    returnList.push_back({i, j});
                }
            }

        }
        return returnList;
    }

    vector<xyc> getSameValue(Grid& compareGrid, Grid& original){
        vector<xyc> returnList;
        
        for(int i = 0; i < HOWBIG; i++){
            Line* la = lines[i];
            Line* lb = compareGrid.lines[i];
            Line* lo = original.lines[i];

            for(int j = 0; j <HOWBIG; j++){
                if (la->s[j] == lb->s[j] and la->s[j] != lo->s[j]){
                    // cout << "x: " << i << " y: " << j << " " << la->s[j] << endl;
                    returnList.push_back({i, j, la->s[j]});
                }
            }
        }
        return returnList;
    }

    string toString(){
        string s = "";
        int count = 0;
        while (count < HOWBIG){
            s += lines[count + HOWBIG] -> toString();
            s += '\n';
            count += 1;
        }
        return s;
    }

};