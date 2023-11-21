#include <bits/stdc++.h>

using namespace std;
#define HOWBIG 25
class LineSolver
{
public:
    vector<int> d;
    string s;

    LineSolver(string& currentS, vector<int>& block_sections)
    {
        d = block_sections;
        s = currentS;
    }

    bool fix()
    {
        return fix(HOWBIG - 1, d.size() - 1);
    }

    bool fix(int i, int j)
    {
        if (i < 0)
        {
            return j < 0;
        }
        else
        {
            return fix0(i, j) || fix1(i, j);
        }
    }

    bool fix0(int i, int j)
    {
        if (s[i] != '1')
            return fix(i - 1, j);
        return false;
    }

    bool fix1(int i, int j)
    {
        bool leadingZero = (j >= 0 && i >= d[j]) && matchSigma(i, j);

        bool leadingOne = (j == 0 && (i + 1) - d[j] == 0);
        if (leadingOne)
        {
            for (int index = 0; index < d[j]; index++)
            {
                if (s[index] == '0')
                {
                    leadingOne = false;
                    break;
                }
            }
        }

        if (leadingOne)
        {
            return true;
        }
        if (leadingZero)
        {
            return fix(i - d[j] - 1, j - 1);
        }
        return false;
    }

    bool matchSigma(int i, int j)
    {
        if (s[i - d[j]] != '1')
        {
            for (int index = 0; index < d[j]; index++)
            {
                if (s[i - d[j] + 1 + index] == '0')
                    return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    string paint()
    {
        return paint(HOWBIG - 1, d.size() - 1);
    }

    string paint(int i, int j)
    {
        if (i < 0)
        {
            return "";
        }

        if (j < 0)
        {
            return string(i + 1, '0');
        }

        bool p0 = fix0(i, j), p1 = fix1(i, j);

        if (p0 && !p1)
        {
            return paint0(i, j);
        }
        else if (!p0 && p1)
        {
            return paint1(i, j);
        }
        else
        {
            string t0 = paint0(i, j), t1 = paint1(i, j);
            return merge(t0, t1);
        }
    }

    string paint0(int i, int j)
    {
        return paint(i - 1, j) + "0";
    }

    string paint1(int i, int j)
    {
        if (d[j] == i + 1)
        {
            return string(d[j], '1'); // drop the beat
        }
        return paint(i - d[j] - 1, j - 1) + "0" + string(d[j], '1');
    }

    string merge(string s1, string s2)
    {
        string re = "";
        for (int i = 0; i < s2.length(); i++)
        {
            if (s1[i] == s2[i])
            {
                re += s1[i];
            }
            else
            {
                re += "u";
            }
        }
        return re;
    }
};