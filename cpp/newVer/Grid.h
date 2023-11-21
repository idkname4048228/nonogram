#include <bits/stdc++.h>

using namespace std;
#define HOWBIG 25

class Grid
{
public:
    char board[HOWBIG][HOWBIG];
    string state = "";
    vector<int> lineOfG;

    bool isPainted;

    Grid()
    {
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                board[i][j] = 'u';
            }
            lineOfG.push_back(i * 2);
            lineOfG.push_back(i * 2 + 1);
        }
        state = "";
    }

    Grid(const Grid &G)
    {
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                board[i][j] = G.board[i][j];
            }
        }
        state = G.state;
        for (int lineNumber : G.lineOfG)
        {
            lineOfG.push_back(lineNumber);
        }
    }

    string getLine(int index)
    {
        string lineString = "";
        if (index < HOWBIG)
        { // column
            for (int i = 0; i < HOWBIG; i++)
            {
                lineString += board[i][index];
            }
        }
        else
        {
            for (int i = 0; i < HOWBIG; i++)
            {
                lineString += board[index % HOWBIG][i];
            }
        }
        return lineString;
    }

    void setLine(int index, string changed)
    {
        int num = index % HOWBIG;
        if (index < HOWBIG)
        { // column
            for (int i = 0; i < HOWBIG; i++)
            {
                if (changed[i] != 'u' and board[i][num] != changed[i])
                {
                    updatePixel(i, num, changed[i], true);
                }
            }
        }
        else
        {
            for (int i = 0; i < HOWBIG; i++)
            {
                if (changed[i] != 'u' and board[num][i] != changed[i])
                {
                    updatePixel(num, i, changed[i], false);
                }
            }
        }
    }

    vector<int> &getLineOfG()
    {
        return lineOfG;
    }

    int findInsertPosition(int target)
    {
        int low = 0;
        int high = lineOfG.size();

        while (low < high)
        {
            int mid = low + (high - low) / 2;
            if (lineOfG[mid] < target)
            {
                low = mid + 1;
            }
            else
            {
                high = mid;
            }
        }
        return low;
    }

    void insertToLineOfG(int value)
    {
        if (lineOfG.size() == 0){
            lineOfG.push_back(value);
            return ;
        }
        int insertPosition = findInsertPosition(value);
        if (lineOfG[insertPosition] != value)
        {
            lineOfG.insert(lineOfG.begin() + insertPosition, value);
        }        
    }

    void updatePixel(int x, int y, char c, bool changeRow)
    {
        isPainted = true;
        board[x][y] = c;
        if (changeRow)
        {
            insertToLineOfG(x + HOWBIG);
        }
        else
        {
            insertToLineOfG(y);
        }
    }

    void updatePixel(int x, int y, char c)
    {
        isPainted = true;
        board[x][y] = c;
        insertToLineOfG(x+HOWBIG);
        insertToLineOfG(y);
    }

    vector<pair<int, int>> getUnknowPixels()
    {
        vector<pair<int, int>> returnList;
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                if (board[i][j] == 'u')
                {
                    returnList.push_back({i, j});
                }
            }
        }
        return returnList;
    }

    void updateAllBoard(Grid &paint0Grid, Grid &paint1Grid)
    {
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                if (paint0Grid.board[i][j] == paint1Grid.board[i][j] && paint0Grid.board[i][j] != 'u' and board[i][j] == 'u')
                {
                    updatePixel(i, j, paint0Grid.board[i][j]);
                }
            }
        }
    }

    void updateAllBoard(Grid &paintGrid)
    {
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                if (board[i][j] == 'u')
                {
                    updatePixel(i, j, paintGrid.board[i][j]);
                }
            }
        }
    }

    string toString()
    {
        string s = "";
        for (int i = 0; i < HOWBIG; i++)
        {
            for (int j = 0; j < HOWBIG; j++)
            {
                s += board[i][j];
                s += (j == HOWBIG - 1 ? " \n" : " ");
            }
        }
        return s;
    }
};