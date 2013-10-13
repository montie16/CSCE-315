#include "ai.h"
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Matrix
{
public:
	string data [20][20];
    
	Matrix (void)
	{
		int i, j;
        
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 20; j++)
			{
				data [i][j] = "";
			}
		}
	}
};

Matrix m;

struct Space
{
    string player;
};

void createField()
{
   
    
   
    for(int i = 2; i <= 9; i++)
    {
        for(int j = 2; j <= 18; j++)
        {
            if(j%2 == 0)
                m.data[i][j] = "|";
            else
                m.data[i][j] = "_";
        }
    }
    
    for(int ii = 1; ii <= 17; ii++)
    {
        if(ii%2 == 0)
            m.data[1][ii] = "_";
        else
            m.data[1][ii] = " ";
    }
    
    
    m.data[2][19] = "1";
    m.data[3][19] = "2";
    m.data[4][19] = "3";
    m.data[5][19] = "4";
    m.data[6][19] = "5";
    m.data[7][19] = "6";
    m.data[8][19] = "7";
    m.data[9][19] = "8";
    
    m.data[10][1] = " ";
    m.data[10][2] = "a";
    m.data[10][3] = " ";
    m.data[10][4] = "b";
    m.data[10][5] = " ";
    m.data[10][6] = "c";
    m.data[10][7] = " ";
    m.data[10][8] = "d";
    m.data[10][9] = " ";
    m.data[10][10] = "e";
    m.data[10][11] = " ";
    m.data[10][12] = "f";
    m.data[10][13] = " ";
    m.data[10][14] = "g";
    m.data[10][15] = " ";
    m.data[10][16] = "h";
    
    m.data[5][9] = "O";
    m.data[6][9] = "@";
    m.data[5][11] = "@";
    m.data[6][11] = "O";
    
    for(int n = 1; n <= 10; n++)
    {
        for(int k = 1; k <= 19; k++)
        {
            cout<<m.data[n][k];
        }
        cout<<endl;
    }
    
}

void move(string mov)
{
    if(mov.size() != 2)
        cout<<"INVALID MOVE"<<endl;
    
    string sub1 = mov.substr(0,1);
    string sub2 = mov.substr(1,2);
    
    int sp1;
    int sp2;
    
    for(int i = 1; i <= 16; i++)
    {
        if(m.data[10][i] == sub1)
            sp1 = i + 1;
    }
    for(int j = 1; j <= 9; j++)
    {
        if(m.data[j][19] == sub2)
            sp2 = j;
    }
    
    //isValid(sp2,sp1);
    
    m.data[sp2][sp1] = "O";
    
    
    //redraw after move is done
    for(int n = 1; n <= 10; n++)
    {
        for(int k = 1; k <= 19; k++)
        {
            cout<<m.data[n][k];
        }
        cout<<endl;
    }
}

bool isValid(int ind1, int ind2)
{

}

bool checkTop(int ind1,int ind2)
{
    if(ind2 != 2)
    {
        if(m.data[ind1][ind2-1] != m.data[ind1][ind2] && m.data[ind1][ind2-1] != '_')
        {
            for(int i = ind2; i >= 2; i--)
            {
                if(m.data[ind1][i] == '_')
                    return false;
                if(m.data[ind1][i] == m.data[ind1][ind2])
                    return true;
            }
        }
    } else
    {
        return false;
    }
}

bool checkBottom(int ind1, int ind2)
{
    if(ind2 != 9)
    {
        if(m.data[ind1][ind2+1] != m.data[ind1][ind2] && m.data[ind1][ind2+1] != '_')
        {
            for(int i = ind2; i <= 8; i++)
            {
                if(m.data[ind1][i] == '_')
                    return false;
                if(m.data[ind1][i] == m.data[ind1][ind2])
                    return true;
            }
        }
    } else
    {
        return false;
    }
}

bool checkLeft(int ind1, int ind2)
{
    if(ind1 != 2)
    {
        if(m.data[ind1-2][ind2] != m.data[ind1][ind2] && m.data[ind1-2][ind2] != '_')
        {
            for(int i = ind1; i >= 2; i -= 2)
            {
                if(m.data[i][ind2] == '_')
                   return false;
                if(m.data[i][ind2] == m.data[in1][ind2])
                   return true;
            }
        }
    } else
    {
        return false;
    }
}

bool checkRight(int ind1, int ind2)
{
    if(ind1 != 16)
    {
        if(m.data[ind1+2][ind2] != m.data[ind1][ind2] && m.data[ind1+2][ind2] != '_')
        {
            for(int i = ind1; i <= 16; i += 2)
            {
                if(m.data[i][ind2] == '_')
                    return false;
                if(m.data[i][ind2] == m.data[in1][ind2])
                    return true;
            }
        }
    } else
    {
        return false;
    }
}

int main()
{
    createField();
    move("f6");
}