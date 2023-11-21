#include <bits/stdc++.h>
#include "Grid.h"
#include "LineSolver.h"

using namespace std;

bool solve = false;
Grid grid;
vector<int> lineOfG;
vector<vector<int>> descriptions = vector<vector<int>>();

void fileInput()
{
    string inputStr;
    descriptions.clear();

    for (int i = 0; i < HOWBIG * 2; i++)
    {
        getline(cin, inputStr);
        istringstream iss(inputStr);
        vector<int> d;
        int num;
        while (iss >> num)
        {
            d.push_back(num);
        }

        descriptions.push_back(d);
    }
    grid = Grid();
}

bool isSolve(Grid &G)
{
    for(int i = 0; i < HOWBIG; i++){
        for (int j = 0; j < HOWBIG; j++){
            if (G.board[i][j] == 'u'){
                return false;
            }
        }
    }
    return true;
}

void propagate(Grid &G)
{
    vector<int>& lineOfG = G.getLineOfG();
    int lineNumber;


    while (lineOfG.size() != 0)
    {
        // cout << lineOfG.size() << endl;
        lineNumber = lineOfG.front();
        lineOfG.erase(lineOfG.begin());

        string lineString = G.getLine(lineNumber);
        vector<int> desc = descriptions[lineNumber];

        LineSolver lineSolver = LineSolver(lineString, desc);
        
        if (!lineSolver.fix())
        {
            G.state = "CONFLICT";
            return;
        }

        string painted = lineSolver.paint();
        
        G.setLine(lineNumber, painted);
    }
    if (isSolve(G))
    {
        G.state = "SOLVED";
    }
    else
    {
        G.state = "INCOMPLETE";
    }
}

Grid probeG(Grid grid, int x, int y, char c)
{
    grid.updatePixel(x, y, c);
    propagate(grid);

    return grid;
}

void updateOnAllG(Grid &G)
{
    G.isPainted = false;
}

void probe(Grid &grid, int x, int y)
{
    Grid g0 = probeG(grid, x, y, '0');
    Grid g1 = probeG(grid, x, y, '1');

    if (g0.state == "CONFLICT" and g1.state == "CONFLICT")
    {
        grid.state = "CONFLICT";
        return;
    }

    if (g0.state == "CONFLICT")
        grid.updateAllBoard(g1);
    else if (g1.state == "CONFLICT")
        grid.updateAllBoard(g0);
    else
        grid.updateAllBoard(g0, g1);

    if (grid.isPainted)
    {
        grid.state = "PAINTED";
    }
    else
        grid.state = "INCOMPLETE";
}

void FP1(Grid &G)
{
    do
    {
        propagate(G);

        if (G.state == "CONFLICT" or G.state == "SOLVED")
        {
            return;
        }

        G.isPainted = false;

        for (pair<int, int> x_y : G.getUnknowPixels())
        {
            probe(G, x_y.first, x_y.second);
            if (G.state == "CONFLICT" or G.state == "SOLVED")
            {
                return;
            }
            if (G.state == "PAINTED")
            {
                break;
            }
        }
    } while (G.isPainted);
}

void backtracking(Grid &G)
{
    FP1(G);
    if (G.state == "CONFLICT")
    {
        return;
    }
    if (G.state == "SOLVED")
    {
        solve = true;
        return;
    }

    pair<int, int> x_y = G.getUnknowPixels()[0];


    G.updatePixel(x_y.first, x_y.second, '0');
    backtracking(G);

    if (solve)
        return;


    G.updatePixel(x_y.first, x_y.second, '1');
    backtracking(G);
}

bool checkAns(Grid& grid) {
    for (int i = 0; i < HOWBIG * 2; i++){
        string line = grid.getLine(i);
        vector<int> realD = vector<int>();
        int tmp = 0;
        for(char c: line){
            if (c == '0' and tmp != 0){
                realD.push_back(tmp);
                tmp = 0;
            }else if(c == '1'){
                tmp += 1;
            }
        }
        if (tmp != 0){
            realD.push_back(tmp);
        }
        if (realD != descriptions[i]){
            return false;
        }
    }
    return true;
}

int main()
{
    int c = 0;
    string currentCase = "";
    while (getline(cin, currentCase))
    {
        c += 1;
        clock_t start, end;
        printf("case %d\n", c);
        start = clock();
        fileInput();

        // string tmp  = string(25, 'u');
        // LineSolver test = LineSolver(tmp, descriptions[0]);
        // cout << test.paint() << endl;

        backtracking(grid);

        // FP1(grid);

        end = clock();
        double diff = (double)(end - start) / CLOCKS_PER_SEC;
        // cout << grid.t`oString() << endl;
        
        cout << (checkAns(grid) ? "CORRECT, " : "WRONG ANSWER, ");
        cout << grid.state << endl;
            
        // cout << diff << endl;
        printf("take %f s\n", diff);
    }
    return 0;
}