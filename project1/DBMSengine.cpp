//  File.cpp
//  315project1
//
//  Created by Eduardo Cirilo on 9/1/13.
//  Copyright (c) 2013 Eduardo Cirilo. All rights reserved.
//
#include "DBMSengine.h"
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

RelationNames * DBMS::getRel(string relName)
{
    for(int i = 0; i < relations.size(); i++)
    {
        if(relName == relations[i].identity)
        {
            return& relations[i];
        }
    }
}

bool isDigit(string x)

 {

    for(int i = 0; i < x.size(); i++)

    {

        int xx = (int) x[i];

        if ( xx <= 57 && xx >= 48 )

        {

            return true;

        }

        else

        {

            return false;

        }

    }

 }


int DBMS::createTable(string name, vector<string> attributes, vector<int> attr_sz, vector<string> primary)
{
    RelationNames newRelation;
    if(primary.size() > attributes.size()){
		cout << "More primary values than actual values\n";
		return -1;
	}
	if(primary[0] != attributes[0]){
		cout << "Please list primary keys first\n";
		return -1;
	}
    newRelation.identity = name;
    newRelation.attr_size = attr_sz;
    vector<string> strVec;
    newRelation.vec.resize(attributes.size(),strVec);
    for(int i=0; i <attributes.size(); i++)
    {
		newRelation.vec[i].push_back (attributes[i]);

    }
    relations.push_back(newRelation);
	return 0;
    
    
}

int DBMS::openRelation(string relName){
	ifstream readIn;
	//string ext = ".db";
	string line;
	string filename = relName + ".db";
	vector<string> attr;
	vector<string> values;
	vector<string> sizes;
	vector<int> sizesNum;
	readIn.open(filename.c_str());
	if(!readIn.is_open()){
		return -1;
	}
	getline(readIn, line);
	sizes =parse(line);
	for(int i=0; i<sizes.size(); i++){
		sizesNum.push_back(atoi(sizes[i].c_str()));
	}
	line = "";
	getline(readIn, line);
	attr = parse(line);
	createTable(relName, attr, sizesNum, attr);
	RelationNames *tbl = getRel(relName);
	tbl->attr_size = sizesNum;

	while(line != ""){
		line = "";
		getline(readIn, line);
		values = parse(line);
		relations[relations.size() - 1].insert(values);	
	}
	readIn.close();
}
int DBMS::writeRelation(string relation)
{
	string temp = relation;
    ofstream relFile;
    relation += ".db";
    relFile.open(relation.c_str());
	RelationNames *rel = getRel(temp);
	vector<int> attr_sz = rel->attr_size; //this line
	if(!relFile.is_open()){
		return -1;
	}
	for(int i=0; i < attr_sz.size(); i++)
    {
		if (i != 0){
                relFile << " ";
		}
		relFile << attr_sz[i];
	}
	relFile << "\n";
    for(int i=0; i < rel->vec[0].size(); i++)
    {
        for(int j=0; j < rel->vec.size(); j++)
        {
            if (j != 0){
                relFile << " ";
			}
            relFile << rel->vec[j][i];
        }
        relFile << "\n";
	}
    relFile << endl;
    relFile.close();
}



int DBMS::closeRelation(string relation)

{

    ofstream relFile;

    relation += ".db";
	
	if(deleteTbl(relation) == -1){
		return -1;
	}
	
	RelationNames *rel = getRel(relation);
	vector<int> attr_sz = rel->attr_size;
	relFile.open(relation.c_str());
	if(!relFile.is_open()){
		return -1;
	}
	
	for(int i=0; i < attr_sz.size(); i++)
    {
		if (i != 0){

                relFile << " ";
		}
		relFile << attr_sz[i];
	}
	
	relFile << "\n";

    for(int i=0; i < rel->vec[0].size(); i++)

    {

        for(int j=0; j < rel->vec.size(); j++)

        {

            if (j != 0){

                relFile << " ";
			}
            relFile << rel->vec[j][i];

            

        }

        relFile << "\n";

	}

    relFile << endl;

    relFile.close();

}

int DBMS::deleteTbl(string relName)
{
   for(int i = 0; i < relations.size(); i++)
   {
		if(relations[i].identity == relName){
			relations.erase(relations.begin()+i);
			return 0;
		}
   }
   cout << "Relation not found, so could not be closed\n";
   return -1;
}

vector<vector<string> > DBMS::selection( vector< vector <string> > tbl, vector<int> index)
{
    vector<vector<string> > newTbl(tbl.size());
	for(int i = 0; i < tbl.size(); i++){
		newTbl[i].push_back(tbl[i][0]);
	}
	for(int i = 0; i < tbl.size(); i++)
	{
		for(int j = 0; j < index.size(); j++)
			{
				newTbl[i].push_back(tbl[i][index[j]]);		
			}
	}
	return newTbl;
}



vector<vector<string> > DBMS::projection(vector<string> attr, vector< vector <string> > tbl)
{
    vector<vector<string> > newTbl;

    for(int i = 0; i < tbl.size(); i++)
    {
		for(int k=0; k < attr.size(); k++){
			if(tbl[i][0] == attr[k])
			{
			 vector<string> newVec;
				for(int j = 0; j < tbl[i].size(); j++)
				{
					newVec.push_back(tbl[i][j]);
				}
				newTbl.push_back(newVec);
			}
		}
    }
    
    return newTbl;
}


vector<vector<string> > DBMS::renaming(vector<string> attr, vector< vector <string> > tbl)
{
    vector<vector<string> > newTbl = tbl;
	vector<vector<string> > errVec;
    if(attr.size() != tbl.size()){
		cout << "Must enter correct number of new attribute names\n";
		return errVec;
	}
    for(int i = 0; i < tbl.size(); i++)
    {
		newTbl[i][0] = attr[i][0];
    }
    
    return newTbl;
}

int RelationNames::deleteAtt(vector<int> index)
{
    int size = 0;
    for(int i = 0; i < index.size(); i++)
    {
        vec.erase(vec.begin()+index[i]);
    }
	return 0;
}

int RelationNames::update(string attribute, string newVal, vector<int> index)
{
    for(int i = 0; i < index.size(); i++)
    {
		 if(vec[i][0] == attribute)
        {
			vec[i][index[i]] = newVal;
		}
	
	}
	
}

int RelationNames::insert(vector<string> val)
{
	cout << "IN INSERT\n";
	for(int i=0; i<val.size(); i++){
		cout << val[i] <<endl;
	}
	cout << "attributes size val's\n";
	for(int i=0; i<attr_size.size(); i++){
		cout << attr_size[i] <<endl;
	}
    for(int i = 0; i < val.size(); i++)
    {
		if(attr_size[i] == 0){
			if(isDigit(val[i])){
				cout << "integer pushed back\n";
				vec[i].push_back(val[i]);
			}
			else{
				cout << "Integer expected for given attribute type\n";
				return -1;
			}
		}
		else{
			if(val[i].size() <= attr_size[i]){
				cout << "string pushed back\n";
				vec[i].push_back(val[i]);
			}
			else{
				cout << "Entry exceeds acceptable word size.\n";
				return -1;
			}
		}
    }
	return 0;
}

int RelationNames::insert_Rel(vector<vector<string> > tbl){
	for(int i = 1; i < tbl[0].size(); i++){
		for(int j =0; j < tbl.size(); j++){
			vec[j].push_back(tbl[j][i]);
		}
	}
	return 0;
}

vector<vector<string> > DBMS::set_union(vector<vector<string> > a, vector<vector<string> > b)
{
    vector<vector<string> > set_union;
    bool unique = true;
    if(a.size() != b.size())
        cout << "error";
    for(int i =0; i< a.size(); i++){
        set_union.push_back(a[i]);
    }
    for(int k=1; k<b[0].size(); k++){
        for(int j=1; j< a[0].size(); j++){
            if(b[0][k] == a[0][j]){
				unique = false;  //as is: only bases unique-ness on specified key. is okay as long as creation of attributes is well-controlled (ie keys are determined as unique to each table upon table creation)
			}
        }
        if(unique){
            for(int f=0; f< a.size(); f++){
                set_union[f].push_back(b[f][k]);
            }
        }
        unique = true;
    }
    
    return set_union;
}


vector<vector<string> > DBMS::cart_prod(vector<vector<string> > a, vector<vector<string> > b)
{
    int col = a.size() + b.size();
    vector<vector<string> > product(col);
    for(int k=0; k< a[0].size(); k++) {
        for(int j = 1; j< b[0].size(); j++) {
            for(int f = 0; f < a.size(); f++){
				if(k == 0) {
					if(j == 1)
						product[f].push_back(a[f][k]);
				}
				else
					product[f].push_back(a[f][k]);
            }
            for(int g = a.size(); g < col; g++){	
                product[g].push_back(b[g - a.size()][j]);
            }
        }	
    }
    for(int i=0; i<b.size(); i++ ){
        product[i +a.size()][0] = b[i][0];
    }
    return product;
}



vector<vector<string> > DBMS::set_difference(vector<vector<string> > x, vector<vector<string> > y)
{
	vector<vector<string> > difference;
	bool unique = true;
	if(x.size() != y.size())	//check size
		cout << "error"<<endl;
	difference.resize(x.size());
	for(int i=0; i < x.size(); i++)
	{
		difference[i].push_back(x[i][0]);
	}
    
    
	for(int t=1; t < y[0].size(); t++)
	{
		
		for(int s=1; s < x[0].size(); s++)
		{
            
            
			if(x[0][t] == y[0][s])
				unique = false;
		}
		if(unique)
		{
			for(int l=0; l< x.size(); l++)
			{
				difference[l].push_back(x[l][t]);
			}
		}
		unique = true;
	}
	for(int k=1; k < y[0].size(); k++)
	{
		
		for(int j=1; j < x[0].size(); j++)
		{
            
            
			if(y[0][k] == x[0][j])
				unique = false;
		}
		if(unique)
		{
			for(int f=0; f< x.size(); f++)
			{
				difference[f].push_back(y[f][k]);
			}
		}
		unique = true;
	}
    
	return difference;
}


void RelationNames::show()
{
	cout << "vec's size = " << vec[0].size() <<endl;
    for(int i=0; i < vec[0].size(); i++)
    {
		cout << "1111";
        for(int j=0; j < vec.size(); j++)
        {
            if (j != 0)
                cout << "\t";
            cout << setw(15);
            cout << vec[j][i];
            
        }
		cout << "\n22222222";
        cout<<endl;
	}
	cout << "\n33333333";
    cout<<endl;
}

void print_vecxvec(vector<vector<string> > vec)
{
	if(vec.empty())
		return;
	for(int i=0; i < vec[0].size(); i++)
    {
        for(int j=0; j < vec.size(); j++)
        {
            if (j != 0)
                cout << "\t";
            cout << setw(15);
            cout << vec[j][i];
            
        }
        cout<<endl;
	}
    cout<<endl;
}
               
vector<int> DBMS::condition(vector<string> cond, vector< vector <string> > vec) //moved into DBMSengine
{
    vector<int> empt;
    vector<int> left;
    vector<int> right;
    vector<int> intVec; //added
    vector<int> intVec2;
    vector<int> intVec3;
    for(int i=0; i< cond.size(); i++){
        if(cond[i] == "||"){
            if(i == 3){
                if(cond[i-1] == ")"){
                    left = condition(parse(cond[i-2]), vec);
                    intVec.clear();
                }
                else{
                    vector<string> leftVec(cond.begin(), cond.begin() + (i));
                    left = condition(leftVec, vec);
                    intVec.clear();
                }
                if(i != cond.size()-1){
                    if(cond[i+1] == "("){
                        right = condition(parse(cond[i+2]), vec);
                        intVec.clear();
                    }
                    else{
                        vector<string> rightVec(cond.begin() + (i+1), cond.end());
                        right = condition(rightVec, vec);
                        intVec.clear();
                    }
                }
                else{
                    cout << "in condition: || at end of condition\n";
                    return empt;
                }
            }
            else{
                //error
                cout << "in condition: || misplaced\n";
                return empt;
            }
            for (int i = 0; i < right.size(); i++)
            {
                left.push_back(right[i]);
            }
            for(int j = 0; j < left.size(); j++)
            {
                for(int k = j + 1; k < left.size(); k++)
                {
                    if(left[j] == left[k])
                    {
                        left.erase(left.begin()+k);
                    }
                }
            }
            return left;
        }
        
    }
    for(int i=0; i< cond.size(); i++){
        if(cond[i] == "&&"){
            if(i == 3){
                if(cond[i-1] == ")"){
                    left = condition(parse(cond[i-2]), vec);
                    intVec.clear();
                }
                else{
                    vector<string> leftVec(cond.begin(), cond.begin() + (i));
                    left = condition(leftVec, vec);
                    intVec.clear();
                }
                if(i != cond.size()-1){
                    if(cond[i+1] == "("){
                        right = condition(parse(cond[i+2]), vec);
                        intVec.clear();
                    }
                    else{
                        vector<string> rightVec(cond.begin() + (i+1), cond.end());
                        right = condition(rightVec, vec);
                        intVec.clear();
                    }
                }
                else{
                    cout << "in condition: && at end of condition\n";
                    return empt;
                }
            }
            else{
                //error
                cout << "in condition: && mislpaced";
                return empt;
            }
            for (int i = 0; i < right.size(); i++)
            {
                left.push_back(right[i]);
            }
            for(int j = 0; j < left.size(); j++)
            {
                for(int k = j + 1; k < left.size(); k++)
                {
                    if(left[j] == left[k])
                    {
                        intVec2.push_back(left[j]);
                    }
                }
            }
            return intVec2;
        }
        
    }
    
    if(cond.size() != 3){
        //error
        cout << "Simplified condition's size != 3\n";
        return empt;
    }
    if(cond[1] == "=="){
        //(cond[0] == cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
                    if(cond[2] == vec[i][j])
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else if(cond[1] == "!="){
        //(cond[0] != cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
                    if(cond[2] != vec[i][j])
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else if(cond[1] == "<"){
        //(cond[0] < cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
                    if(atoi(vec[i][j].c_str()) < atoi(cond[2].c_str()) )
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else if(cond[1] == ">"){
        //(cond[0] > cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
                    if(atoi(vec[i][j].c_str()) > atoi(cond[2].c_str())  )
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else if(cond[1] == "<="){
        //(cond[0] <= cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
                    if(atoi(vec[i][j].c_str())<= atoi(cond[2].c_str())  )
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else if(cond[1] == ">="){
        //(cond[0] >= cond[]2);
        for(int i = 0; i < vec.size(); i++)
        {
            if(cond[0] == vec[i][0])
            {
                for(int j = 0; j < vec[i].size(); j++)
                {
					if(atoi(vec[i][j].c_str()) >= atoi(cond[2].c_str())  )
                        intVec.push_back(j);
                }
            }
        }
        return intVec;
    }
    else{
        //error
        cout << "Unmatched operator in simplified condition\n";
        return empt;
    }
}
   
               




///////////END PARSING STUFF///////////////
/*int main()
{
    DBMS db;
    vector<string> dbVec;
    dbVec.push_back("key");
    dbVec.push_back("firstname");
    dbVec.push_back("lastname");
    dbVec.push_back("phone");
    
    
    vector<string> joe_smith;
    joe_smith.push_back("829138");
        joe_smith.push_back("joe");
        joe_smith.push_back("smith");
        joe_smith.push_back("712 555 8456");
		
	vector<string> ed_jones;
    ed_jones.push_back("154685");
        ed_jones.push_back("ed");
        ed_jones.push_back("jones");
        ed_jones.push_back("713 890 4331");
		
	vector<string> sarah_lewis;
    sarah_lewis.push_back("671549");
        sarah_lewis.push_back("sarah");
        sarah_lewis.push_back("lewis");
        sarah_lewis.push_back("789 373 8461");

    
    
    db.createTable("employee_1", dbVec);
    cout<<"Test create table(employee_1)"<<endl;
    db.getRel("employee_1")->show();
	
	db.createTable("employee_2", dbVec);
    cout<<"Test create table (employee_2)"<<endl;
    db.getRel("employee_2")->show();
    
    
    db.getRel("employee_1")->insert(joe_smith);
    cout<<"Inserting Joe Smith into empl0yee_1"<<endl;
    db.getRel("employee_1")->show();
	
	db.getRel("employee_1")->insert(ed_jones);
    cout<<"Inserting Ed Jones into employee_1"<<endl;
    db.getRel("employee_1")->show();
	
	db.getRel("employee_2")->insert(ed_jones);
    cout<<"Inserting Ed Jones into employee_2"<<endl;
    db.getRel("employee_2")->show();
    
	db.getRel("employee_2")->insert(sarah_lewis);
    cout<<"Inserting Sarah Lewis into employee_2"<<endl;
    db.getRel("employee_2")->show();
    
    db.getRel("employee_1")->update("key","829138","newkey");
    cout<<"Updating Joe Smith's ID from 154685 to 'new key' in employee_1"<<endl;
    db.getRel("employee_1")->show();
	
	cout<<"Test selection"<<endl;
	vector< vector<string> > sel_joe = db.getRel("employee_1")->selection("firstname", "joe");
	cout<<"First name = joe (employee_1)"<<endl;
    print_vecxvec(sel_joe);
	
	cout<<"Test selection"<<endl;
	vector< vector<string> > sel_jones = db.getRel("employee_1")->selection("lastname", "jones");
	cout<<"Last name = jones (employee_1)"<<endl;
	print_vecxvec(sel_jones);
	
	cout<<"Testing union of employee_1 and employee_2"<<endl;
	vector< vector<string> > test_union = db.set_union(db.getRel("employee_1")->vec, db.getRel("employee_2")->vec);
	print_vecxvec(test_union);
	
	cout<<"Testing cartesian product of employee_1 and employee_2"<<endl;
	vector< vector<string> > test_product = db.cart_prod(db.getRel("employee_1")->vec, db.getRel("employee_2")->vec);
	print_vecxvec(test_product);
    
    cout << "Testing the difference of employee_1 & employee_2" << endl;
    vector< vector<string> > test_difference = db.difference(db.getRel("employee_1")->vec, db.getRel("employee_2")->vec);
    print_vecxvec(test_difference);
    
    cout<<"Testing the projection of employee_2's firstname"<<endl;
    vector<vector<string> > test_projection = db.getRel("employee_2")->projection("firstname");
    print_vecxvec(test_projection);

    cout<<"Testing delete attribute of employee_1"<<endl;
    db.getRel("employee_1")->deleteAtt("lastname", "smith");
    db.getRel("employee_1")->show();
    
    db.deleteTbl();
    cout<<"Test delete table"<<endl;
    
}*/

/*
notes from class
 
 
 Recursive descent parser
 
 Sentence ::= query | command
 bool sentence () {
 return query() || command();
 }
 
 bool query(){
 return rel_name()
 && assign()
 && expr();
 }
 
 
 
 
 
 
 
 
 
 bool isDigit(string x)
 {
    for(int i = 0; i < x.size(); i++)
    {
        int xx = (int) x[i];
        if ( xx <= 57 && xx >= 48 )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
 }
 
 //checks if it is
 bool isAlpha(char x)
 {
 int xx = (int)x;
 if((xx >= 97 && xx <= 122) || (xx >= 65 && xx <= 90) || xx == 95)
 {
 return true;
 } else
 {
 return false;
 }
 }
 
 //returns true if it is a digit, false if it is a alphabetic character
 bool idCheck(char x){
 if(isDigit(x))
 {
 return true;
 } else
 {
 return false;
 }
 }
*/