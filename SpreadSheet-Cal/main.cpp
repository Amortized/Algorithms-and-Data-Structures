#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <fstream>
#include <climits>
#include "graph.cpp"
#include <stack>



using namespace std;

class IO
{
public:

	void stringSplitter(string line,vector<string>& s)
	{
		istringstream iss(line);
	    do
	    {
	        string sub;
	        iss >> sub;
	        s.push_back(sub);
	    } while (iss);
	}

	int stringToInt(string s)
	{
		int result;
		stringstream(s) >> result;
		return result;
	}

	void read(vector< vector<string> >& spreadsheet)
	{
		string line;
		int col,row;
		//Read col and row
		getline(cin, line);
		if(!line.empty())
		{
			vector<string> s;
			stringSplitter(line,s);
			col = stringToInt(s[0]);
			row = stringToInt(s[1]);
		}

		for(int i = 0; i < row; i++)
		{
		  vector<string> temp;
		  for(int j = 0; j < col; j++)
		  {
           //Read this row
			getline(cin, line);
			if(line.empty())
				break;
			temp.push_back(line);
		  }
		  spreadsheet.push_back(temp);
		}
	}

	void write(const vector< vector<float> >& spreadsheet)
	{
	    cout << "\n ---------------- Output --------------------------------------\n" << endl;
	    for(int i = 0; i < spreadsheet.size(); i++)
	    {
	    	for(int j = 0; j < spreadsheet[i].size(); j++)
	    	{
	    		cout << spreadsheet[i][j] << "  ";
	    	}
	    	cout << endl;
	    }
	}

};


void evaluate(string s, vector< vector<string> >& spreadsheet, int row, int col, vector< vector<float> >& spreadsheet_new)
{
	stack<float> expr;
	vector<string> split;
	stringSplitter(s, split);
    int row_temp = 0;
    int col_temp = 0;
    float temp1 = 0;
    float temp2 = 0;
    float result = 0;
	for(int i = 0; i < split.size(); i++)
	{
		if(split[i].empty())
			continue;
		if(split[i].find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos)
		{
			row_temp = convertToRow(split[i].at(0)) - 1;
			col_temp = stringToInt(split[i].substr(1)) - 1;
			temp1 = spreadsheet_new[row_temp][col_temp];
			expr.push( temp1 );
		} else if (split[i].find_first_of("*/+-") != string::npos )
		{

			temp1 = expr.top();
			expr.pop();
			temp2 = expr.top();
			expr.pop();
			if (split[i] == "*")
			{	result = temp2 * temp1;
			}
			else if (split[i] == "+")
				result = temp2 + temp1;
			else if (split[i] == "/")
				result = temp2 / temp1;
			else if (split[i] == "-")
				result = temp2 - temp1;
			expr.push(result);
		} else
		{
			temp1 = stringToFloat(split[i]);
			expr.push(temp1);
		}
	}

	temp1 = expr.top();
	expr.pop();
	spreadsheet_new[row][col] = temp1;

}



int main() {
	vector< vector<string> > spreadsheet;
	vector< vector<float> > spreadsheet_new;

	IO *r = new IO();
	r->read(spreadsheet);
	//Build a Graph
	graph g;
	read_graph(&g,true, spreadsheet);
	//print_graph(&g);
	vector<int> sorted;
	topSort(&g, true, sorted);

    for(int i = 0; i < spreadsheet.size(); i++)
    {
    	vector<float> temp;
    	for(int j = 0; j < spreadsheet[i].size(); j++)
    	{
    		temp.push_back(0.0);
    	}
    	spreadsheet_new.push_back(temp);
    }


    if(sorted[0] == -1)
    {
    	cout << "There are Cyclic dependencies in the Spreadsheet " << endl;
    } else
    {

    	int temp1;
    	int temp2;
    	for(int i = sorted.size() -1; i >= 0; i--)
    	{
    		//Get a cell

    		if ( (sorted[i] % spreadsheet[0].size()) == 0)
    		{
    			temp1 = ((int)(sorted[i] / spreadsheet[0].size())) - 1;
    			temp2 = spreadsheet[0].size() - 1;
    		}
    		else
    		{
    			temp1 = (int)(sorted[i] / spreadsheet[0].size());
    			temp2 = (int) (sorted[i] % spreadsheet[0].size()) - 1;
    		}


    		evaluate(spreadsheet[temp1][temp2], spreadsheet, temp1, temp2, spreadsheet_new);
    	}

    	r->write(spreadsheet_new);

    }



	return 0;
}
