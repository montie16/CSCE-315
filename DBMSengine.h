#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

bool isDigit(string x);
class RelationNames
{
public:
    RelationNames() {}
    vector<vector<string> > vec;
	vector<int> attr_size;
    
    string identity;
	

	int deleteAtt(vector<int> index);
	int update(string attribute, string newVal, vector<int> index);
	int insert(vector<string> val);
	int insert_Rel(vector<vector<string> > tbl);
	void show();
	
};

struct DBMS
{
    RelationNames * getRel(string relName);
    int createTable(string name, vector<string> attributes, vector<int> attr_sz, vector<string> primary);
    int openRelation(string relName);
    int closeRelation(string relName);
	int deleteTbl(string relName);
    int writeRelation(string relation);
	vector<vector<string> > set_union(vector<vector<string> > a, vector<vector<string> > b);
	vector<vector<string> > cart_prod(vector<vector<string> > a, vector<vector<string> > b);
	vector<vector<string> > set_difference(vector<vector<string> > x, vector<vector<string> > y);
	vector<vector<string> > selection( vector< vector <string> > tbl, vector<int> index);
	vector<vector<string> > projection(vector<string> attr, vector< vector <string> > tbl);
	vector<vector<string> > renaming(vector<string> attr, vector< vector <string> > tbl);
	vector< vector<string> > project(vector<string> input);
	vector< vector<string> > select(vector<string> input);
	vector< vector<string> > rename(vector<string> input);
	vector< vector<string> > set_union_parse(vector<string> input);
	vector< vector<string> > difference(vector<string> input);
	vector< vector<string> > product(vector<string> input);
	vector<string> parse(string input);
	vector< vector<string> > query(string relName, vector<string> query);
    int execute(string input);
	vector<int> condition(vector<string> cond, vector< vector <string> > vec);
	vector< vector<string> > atomicExp(vector<string> input);
    int exit();
    
    vector<RelationNames> relations;
    
    
};

