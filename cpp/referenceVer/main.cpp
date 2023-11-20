#include <bits/stdc++.h>
#include "Grid.h"

using namespace std;

bool solve = false;
Grid grid;

void fileInput()
{
    string inputStr;
    vector<Line*> lines;

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
        Line* line = new Line(d);

        lines.push_back(line);
    }
    grid = Grid(lines);
    solve = false;
}

bool isSolve(Grid &G)
{
    for (Line* line : G.lines)
    {
        for (char c : line -> s)
        {
            if (c == 'u')
                return false;
        }
    }
    return true;
}

void propagate(Grid &G)
{
    vector<Line*> lines = G.lines;
    vector<int> lineOfG;
    int lineNumber;

    G.updatedPixels.clear();
    for (int i = 0; i < HOWBIG * 2; ++i)
    {
        lineOfG.push_back(i);
    }

    while (lineOfG.size() != 0)
    {
        lineNumber = lineOfG.front();
        lineOfG.erase(lineOfG.begin());

        Line* line = lines[lineNumber];

        if (!line -> fix())
        {
            G.state = "CONFLICT";
            return;
        }

        string painted = line->paint();
        vector<int> diffs = line->diff(painted);
        line->painted();

        for (int num : diffs)
        {
            int changeLineNumber = num + (lineNumber < HOWBIG ? HOWBIG : 0);
            Line* changeLine = lines[changeLineNumber];
            char updatingPixel = painted[num];
            int updatingRow = lineNumber % HOWBIG;

            // cout << "changeLineNumber: " << changeLineNumber << " updatingRow: " << updatingRow << endl;
            // cout << "change " << changeLine->s[updatingRow] << " to " << updatingPixel << endl;

            G.updatedPixels.push_back(Grid::xyc{lineNumber, num, updatingPixel});

            changeLine->updatePixel(updatingRow, updatingPixel);

            if (find(lineOfG.begin(), lineOfG.end(), changeLineNumber) == lineOfG.end())
            {
                lineOfG.push_back(changeLineNumber);
            }
        }
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

void updateOnAllG(Grid &G, vector<struct Grid::xyc> &pixels)
{
    G.updatedPixels.clear();
    if (pixels.size() == 0)
    {
        return;
    }
    for (struct Grid::xyc pixel : pixels)
    {
        G.updatePixel(pixel);
        G.updatedPixels.push_back(pixel);
    }
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
    vector<struct Grid::xyc> pixels;
    if (g0.state == "CONFLICT")
        pixels = g1.updatedPixels;
    else if (g1.state == "CONFLICT")
        pixels = g0.updatedPixels;
    else
        pixels = g0.getSameValue(g1, grid);
    if (pixels.size() != 0)
    {
        updateOnAllG(grid, pixels);
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

        G.updatedPixels.clear();

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
    } while (G.updatedPixels.size() != 0);
}

void backtracking(Grid &G)
{
    if (solve)
        return;
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
    G.updatePixel(x_y.first, x_y.second, '1');
    backtracking(G);
}

int main()
{
    int c = 0;
    string currentCase = "";
    while (getline(cin, currentCase))
    {
        c += 1;
        clock_t start, end;
        start = clock();
        fileInput();

        backtracking(grid);

        
        end = clock();
        double diff = (double)(end - start) / CLOCKS_PER_SEC;
        // cout << grid.toString() << endl;    
        cout << grid.state << endl;
        // cout << diff << endl;
        printf("case %d take %f s\n", c, diff);
    }
    return 0;
}