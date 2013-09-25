/*void DBMS::execute(string query)
{
}*/

#include "DBMSengine.h"
#include <cstdlib>
#include <stdlib.h>

using namespace std;




vector< vector<string> > DBMS::atomicExp(vector<string> input){
	vector< vector<string> > errVec;
	 if(input.size() == 3){
		if(input[0] == "(" && input[2] == ")"){
			return query("", parse(input[1]));
		}
		else{
			cout << "in atomicExp: parentheses wrong.\n";
			return errVec;
		}
	}
	else{
		cout << "in atomicExp: input vector size wrong\n";
		return errVec;
	}
}

vector< vector<string> > DBMS::query(string relName, vector<string> query){
     RelationNames newRel;
	 vector< vector<string> > newVec;
	 vector< vector<string> > errVec;
	 
	 for(int i=0; i<query.size(); i++){
		cout << "query: " << query[i] <<endl;
	 }

     
       if(query.size() >= 4){
			cout << "IN QUERY SIZE RIGHT\n";
               if(query[0] == "project" && query[1] == "(" && query[3] == ")" && query.size() >= 5)
               {
					cout << "IN PROJECT CASE\n";
					newVec = project(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
               }
               else if(query[0] == "select" && query[1] == "(" && query[3] == ")" && query.size() >= 5)
               {
					newVec = select(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
                      
               }
               else if(query[0] == "rename" && query[1] == "(" && query[3] == ")" && query.size() >= 5)
               {
                       newVec = rename(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
               }
			   for(int i= 0;i < query.size(); i++){
					cout << query[i] << " " << endl;
					if(query[i] == "+" && i < query.size() -1){
						cout << "IN PLUS\n";
						newVec = set_union_parse(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
					}
					else if (query[i] == "-" && i < query.size() -1){
						newVec = difference(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
					}
					else if (query[i] == "*" && i < query.size() -1){
						newVec = product(query);
                       if(relName != ""){
							newRel.identity = relName;
							newRel.vec = newVec;
							relations.push_back(newRel);
					   }
							
                       return newVec;
					}
			   }
			   return errVec;
			   
       }
       else
	   {
			cout << "Incorrect syntax\n";
		   return errVec;
	   }
}
vector< vector<string> > DBMS::project(vector<string> input){
	for(int i=0; i <input.size(); i++){
		cout << input[i] << endl;
	}
	vector<string> attr_clean;
	vector< vector<string> > errVec;
	string relName;
    if(input.size() >= 5){
		if(input[0] == "project" && input[1] == "(" && input[3] == ")")
		{
			vector<string> attr = parse(input[2]);
			bool attrList = false;
			for(int i=0; i< attr.size(); i++){
				cout << "in for loop " << (i%2)<<"\n";
					if((i%2) != 0 && i != attr.size() -1){
							if(attr[i] != ","){
									//error
									cout << "project: Incorrect syntax\n";
									attrList = false;
							}
					} 
					else if( (i%2) == 0 && attr[i] == ","){
							cout << "project: Incorrect syntax\n";
							attrList =false;
					}
					else if ((i%2)==0){
						cout << "modulo is zero\n";
						attr_clean.push_back(attr[i]);
						attrList = true;
					}
			}
			if(!attrList){
				cout << "attrlist false\n";
				return errVec;
			}
			
			if(input.size() == 5){
				cout << "about to call projection\n";
				relName = input[4];
				return projection(attr_clean, getRel(relName)->vec);
			}
			else{
				vector<string> atomic (input.begin()+4, input.end());
				return projection(attr_clean, atomicExp(atomic));
			}
			
		}
		cout << "in project: wrong word placement\n";
	}
	cout << "in project: wrong size or word placement\n";
	return errVec;
}

vector< vector<string> > DBMS::select(vector<string> input){
	vector< vector<string> > errVec;
	string relName;
    if(input.size() >= 5){
		if(input[0] == "select" && input[1] == "(" && input[3] == ")")
		{
			if(input.size() == 5){
				relName = input[4];
				return selection(getRel(relName)->vec, condition(parse(input[2]), getRel(relName)->vec ));
			}
			else{
				vector<string> last (input.begin()+4, input.end());
				RelationNames rel;
				rel.vec = atomicExp(last);
				return selection(rel.vec, condition(parse(input[2]), rel.vec));
			}
			
		}
		cout << "in select: wrong word placement\n";
	}
	cout << "in select: wrong size or word placement\n";
	return errVec;
}

vector< vector<string> > DBMS::rename(vector<string> input){
	vector< vector<string> > errVec;
	string relName;
	vector<string> attr_clean;
    if(input.size() >= 5){
		if(input[0] == "rename" && input[1] == "(" && input[3] == ")")
		{
			vector<string> attr = parse(input[2]);
			bool attrList = false;
			for(int i=0; i< attr.size(); i++){
					if((i%2) != 0 && i != attr.size() -1){
							if(attr[i] != ","){
									//error
									cout << "in rename: comma not where expected\n";
									attrList = false;
							}
					}
					else if( (i%2) == 0 && attr[i] == ","){
							cout << "in rename: comma where not expected\n";
							attrList =false;
					}
					else if((i%2) == 0){
						attr_clean.push_back(attr[i]);
						attrList == true;
					}
			}
			
			if(!attrList){
				return errVec;
			}
			
			if(input.size() == 5){
				relName = input[4];
				return renaming(attr_clean, getRel(relName)->vec);
			}
			else{
				vector<string> atomic (input.begin()+4, input.end());
				return renaming(attr_clean, atomicExp(atomic));
			}
			
		}
		cout << "in rename: wrong word placement\n";
	}
	cout << "in rename: wrong size or word placement\n";
	return errVec;
}
vector< vector<string> > DBMS::set_union_parse(vector<string> input)
{
	vector< vector<string> > errVec;
    if(input.size() >= 3){
		for(int i = 0; i< input.size();i++){
			if(input[i] == "+" && i != 0 && i!= input.size()-1)
			{
					vector<string> left(input.begin(), input.begin()+(i));
					vector<string> right(input.begin()+(i+1), input.end());
					if(left.size() == 1 && right.size() == 1){
						cout << "case 1\n";
						return set_union(getRel(left[0])->vec, getRel(right[0])->vec);
					}
					else if(left.size() == 1){
					cout << "case 2\n";
						return set_union(getRel(left[0])->vec, atomicExp(right));
					}
					else if(right.size() == 1){
						cout << "case 3\n";
						return set_union(getRel(right[0])->vec, atomicExp(left));
					}
					else {
						cout << "case 4\n";
						return set_union(atomicExp(left), atomicExp(right));
					}
			}
		}
	}
	cout << "in +: wrong size or word placement\n";
	return errVec;
   
}
vector< vector<string> > DBMS::difference(vector<string> input)
{
	vector< vector<string> > errVec;
    if(input.size() >= 3){
		for(int i = 0; i< input.size();i++){
			if(input[i] == "-" && i != 0 && i!= input.size()-1)
			{
					vector<string> left(input.begin(), input.begin()+(i));
					vector<string> right(input.begin()+(i+1), input.end());
					if(left.size() == 1 && right.size() == 1){
						return set_difference(getRel(left[0])->vec, getRel(right[0])->vec);
					}
					else if(left.size() == 1){
						return set_difference(getRel(left[0])->vec, atomicExp(right));
					}
					else if(right.size() == 1){
						return set_difference(getRel(right[0])->vec, atomicExp(left));
					}
					else {
						return set_difference(atomicExp(left), atomicExp(right));
					}
			}
		}
	}
	cout << "in -: wrong size or word placement\n";
	return errVec;
}
vector< vector<string> > DBMS::product(vector<string> input)
{
	vector< vector<string> > errVec;
    if(input.size() >= 3){ 
		for(int i = 0; i< input.size();i++){
			if(input[i] == "*" && i != 0 && i!= input.size()-1)
			{
					vector<string> left(input.begin(), input.begin()+(i));
					vector<string> right(input.begin()+(i+1), input.end());
					if(left.size() == 1 && right.size() == 1){
						return cart_prod(getRel(left[0])->vec, getRel(right[0])->vec);
					}
					else if(left.size() == 1){
						return cart_prod(getRel(left[0])->vec, atomicExp(right));
					}
					else if(right.size() == 1){
						return cart_prod(getRel(right[0])->vec, atomicExp(left));
					}
					else {
						return cart_prod(atomicExp(left), atomicExp(right));
					}
			}
		}
	}
	cout << "in *: wrong size or word placement\n";
	return errVec;
}


int DBMS::execute(string input)
{
    cout<<"ENTERED EXECUTE"<<endl;
	vector <vector <string> > check;
	vector<string> inputVec = parse(input);
	if(inputVec.empty()){
		return -1;
	}
	
	if(input[input.size() -1] != ';'){
			cout << "Command is missing semi-colon\n";
			return -1;
	}
   
    if(inputVec.size() > 2){
		cout << "GREATER THAN TWO " << inputVec[1]<< " "<< inputVec[1] <<"\n";
        if(inputVec[1] == "<-"){
            cout<<"ARROW"<<endl;
            check = query(inputVec[0], parse(inputVec[2]));
			if(check.empty()){
				cout << "QUERY FAILED\n";
				return -1;
			}
			else{
				return 0;
			}
        }
    }
	if(inputVec[0] == "OPEN"){
		   
		cout<<"OPEN "<<endl;
		if(inputVec.size() != 2){
			//error
			cout << "in OPEN: wrong size\n";
			return -1;
		}
		else{
			return openRelation(inputVec[1]);
		}
	}
	if(inputVec[0] == "CLOSE"){
		if(inputVec.size() != 2){
				//error
				cout << "in CLOSE: wrong size\n";
				return -1;
		}
		else{
			return closeRelation(inputVec[1]);
		}
	}
	if(inputVec[0] == "WRITE"){
		if(inputVec.size() != 2){
			//error
			cout << "in WRITE: wrong size\n";
			return -1;
		}
		else{
			//call write
			return writeRelation(inputVec[1]);
		}
	}
	if(inputVec[0] == "EXIT"){
		if(inputVec.size() != 1){
			//error
			cout << "in EXIT: wrong size\n";
			return -1;
		}
		else{
			//call exit
			return 1;
		}
	}
	if(inputVec[0] == "SHOW"){
		cout<<"SHOW, size of inputVec= "<< inputVec.size() << endl;
		if(inputVec.size() == 2){
			//call show
			RelationNames *rel = getRel(inputVec[1]);
			if(rel){
				rel->show();
				return 0;
			}
			else{
				return -1;
			}
		}
		else if(inputVec.size() == 1){
			//error
			cout << "in SHOW: wrong size\n";
			return -1;
		}
		else{
			vector<string> expr (inputVec.begin()+1, inputVec.end());
			check = query("", expr);
			if(check.empty()){
				return -1;
			}
			else{
				return 0;
			}
		}
	}
	if(inputVec.size() >= 11){
		if(inputVec[0] == "CREATE" && inputVec[1] == "TABLE"){
				string relName = inputVec[2];
				vector<string> attr_rough;
				vector<string> attr;
				vector<string> prime_rough;
				vector<string> prime;
				vector<int> attr_sz;
				if(inputVec.size() != 11 && inputVec[3] != "(" && inputVec[5] != ")" && inputVec[6] != "PRIMARY" && inputVec[7] != "KEY" && inputVec[8] != "(" && inputVec[10] != ")"){
						//error
						cout << "in CREATE: wrong setup\n";
						return -1;
				}
				else{
						attr_rough = parse(inputVec[4]);
						cout << "ATTR ROUGH DONE\n";
						for(int i =1; i< attr_rough.size(); i++){
							if(attr_rough[i] == "VARCHAR" && i+3 <= attr_rough.size() -1){
									attr.push_back(attr_rough[i-1]);
								
										if(attr_rough[i+1] != "(" || attr_rough[i+3] != ")" || !isDigit(attr_rough[i+2])){
											//error
											cout << "in create_varchar: wrong setup\n";
											return -1;
										}
								
										else{
										attr_sz.push_back(atoi(attr_rough[i+2].c_str()));
										cout << "ATOI IN CREATE REURNS " << atoi(inputVec[i+2].c_str()) << " " << inputVec[i+2] <<endl;
										if(i+4 == attr_rough.size()){
												i = i+3;
										}
										else if(i+4 <= attr_rough.size() -1){
											if(attr_rough[i+4] == ","){
												i = i+5;
											}
											else{
												//error
												cout << "in CREATE_VARCHAR: bad list\n";
												return -1;
											}
										}
										else{
												//error
												cout << "in CREATE_VARCHAR: bad list\n";
												return -1;
										}
									}
								   
							}
							else if(attr_rough[i] == "INTEGER"){
									attr.push_back(attr_rough[i-1]);
									attr_sz.push_back(0);
									if(i+1 == attr_rough.size()){
											i = i+1;
									}
									else if(i+1 <= attr_rough.size() -1){
										if(attr_rough[i+1] == ","){
											i = i+2;
										}
									}
									else{
											//error
											cout << "in CREATE_INTEGER: bad list\n";
											return -1;
									}
								   
							}
							else{
									//error
									cout << "in CREATE: bad type" << i << "\n";
									return -1;
							}
					}

						prime_rough = parse(inputVec[9]);
						for(int i = 0; i< prime_rough.size(); i++){
								prime.push_back(prime_rough[i]);
								if(i+1 == attr_rough.size()){
										cout << "prime1\n";
										i = i+1;
								}
								else if(i+1 < prime_rough.size()){
									cout << "prime2\n";
									if(prime_rough[i+1] == ","){
										cout << "prime3\n";
										i = i+1;
									}
									else{
										//error
										cout << "1in CREATE_PRIME: bad list\n";
										return -1;
									}
								}
						}
									   
				}
				//call createTable(relName, attr, prime);
				for(int i=0; i < attr_sz.size() ;i++){
				cout << "ATTR SIIIIIIIIIZE" << attr_sz[i] <<endl;
				}				
				return createTable(relName, attr, attr_sz, prime);
		}
	}
	if(inputVec.size() >= 8){
		int i;
		if(inputVec[0] == "UPDATE" && inputVec[2] == "SET")
		{
			vector<string> attr;
			vector<string> lit;
			
			if(inputVec[4] == "=" && inputVec[6] == "WHERE" )
			{
				string relName = inputVec[1];
				string attrName = inputVec[3];
				string literal = inputVec[5];
				vector<string> vec (inputVec.begin()+7, inputVec.end());
				vector<int> index = condition(vec, getRel(inputVec[1])->vec);
				return getRel(relName)->update(attrName, literal, index);
			}
			else if(inputVec[4] == "=" && inputVec[6] == ",")
			{
				attr.push_back(inputVec[3]);
				lit.push_back(inputVec[5]);
				for( i=6; inputVec[i] != "WHERE"; i+4)
				{
						if(inputVec[i] == ",")
						{
								if(inputVec[i+2] != "=")
								{
										cout<< "in update: bad list\n";
										return -1;
								}
								attr.push_back(inputVec[i+1]);
								lit.push_back(inputVec[i+3]);
						}
						else{
					   			cout<< "in update: bad list2\n";
								return -1;
						}
				}
			}
			else{
				cout << "in UPDATE: bad format\n";
				return -1;
			}
			vector<string> vec(inputVec.begin()+i+1, inputVec.end());
			vector<int> index = condition(vec, getRel(inputVec[1])->vec);
			int total =0;
			for(int j=0; j<attr.size(); j++){
				total+= getRel(inputVec[1])->update(attr[j], lit[j], index);
			}
			if(total == 0){
				return 0;
			}
			else{
			
				return -1;
			}
		}
	}
	if(inputVec.size() >= 5)
	{
			if(inputVec[0] == "DELETE" && inputVec[1] == "FROM" && inputVec[3] == "WHERE"){
				vector<string> ex(inputVec.begin()+4, inputVec.end());
				return getRel(inputVec[4])->deleteAtt(condition(ex, getRel(inputVec[2])->vec));
				
			}
	}
	if(inputVec.size() >= 8)
	{
			if(inputVec[0] == "INSERT" && inputVec[1] == "INTO"){
					if(inputVec[3] != "VALUES" &&inputVec[4] != "FROM"){
							//error
							cout << "in INPUT: bad format\n";
							return -1;
					}
					else if(inputVec[5] == "RELATION"){
							vector<string> expr (inputVec.begin()+6, inputVec.end());
							if(expr.size() == 1){
								return getRel(inputVec[2])->insert_Rel(getRel(expr[0])->vec);
							}
							else{
							cout<<"GOING IN HERE"<<endl;
								return getRel(inputVec[2])->insert_Rel(query("", expr));
							}
					}
					else if (inputVec[5] == "(" && inputVec[7] == ")") {
							vector<string> literal = parse(inputVec[6]);
							vector<string> lit_clean;
							for(int i=0; i< literal.size(); i++){
									if((i%2) != 0 && i != literal.size() -1){
											if(literal[i] != ","){
													//error
													cout << "in INPUT: bad list\n";
													return -1;
											}
									}
									else if( (i%2) == 0 && literal[i] == ","){
											cout << "bad list 2\n";
											return -1;
									}
									else if((i%2) == 0){
										lit_clean.push_back(literal[i]);
									}
									
							}
						   return getRel(inputVec[2])->insert(lit_clean);
					}
				   
			}
	}
	return 0;
       
}


vector<string> DBMS::parse(string input)
{
        int last_delim=-1;
        int parentheses=0;
        int closed=0;
        vector<string> tokens;
        vector<string> errorVector;
        bool errFound = false;
        string errMessage;
        string before, after;
        char c;
		cout << "THIS IS MY INPUT \n" << input <<endl;
        for(int i=0; i<input.size(); i++){
                //cout << "char\n";
                c = input[i];
                switch(c)
                {      
                        case '<':
                                //cout << "case <\n";
                                if(i != input.size() -1){
									cout << "case << not last\n";
                                        if(input[i+1] == '-'){
											cout << "second thing is -\n";
                                                before ="";
                                                after = "";
												if(input[i-1] != ' '){
													for(int j=0; j<i; j++){
															before= before + input[j];
													}
												}
                                                for(int j=i+2; j<input.size(); j++){
                                                        after= after + input[j];
                                                }
                                                if(before != ""){
													tokens.push_back(before);
												}
                                                tokens.push_back("<-");
                                                if(after != ""){
													tokens.push_back(after);
												}
												for(int i =0; i<tokens.size();i++){
													cout << tokens[i]<<endl;
												}
                                                return tokens;
                                        }
                                        else if(input[i+1] == '='){
                                                        before ="";
                                                        for(int k=last_delim+1; k<i; k++){
                                                                before= before +input[k];
                                                        }
														if(before != ""){
															tokens.push_back(before);
														}       
														tokens.push_back("<=");
                                                        last_delim= i+1;
                                                }
                                        else {
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                                       
                                                tokens.push_back("<");
                                                last_delim= i;
                                        }
                                }
                                else{
                                        before ="";
                                        for(int k=last_delim+1; k<i; k++){
                                                before= before +input[k];
                                        }
										if(before != ""){
											tokens.push_back(before);
										}                                                    
                                        tokens.push_back("<");
                                        last_delim= i;
                                }
                                break;
                        case '+':
                                //cout << "case +\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
								if(before != ""){
									tokens.push_back(before);
								}                                                       
                                tokens.push_back("+");
                                last_delim= i;
                                break;
                        case '-':
                                //cout << "case -\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
								if(before != ""){
									tokens.push_back(before);
								}                                                 
                                tokens.push_back("-");
                                last_delim= i;
                                break;
                        case '*':
                                //cout << "case *\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
								if(before != ""){
									tokens.push_back(before);
								}                                                       
                                tokens.push_back("*");
                                last_delim= i;
                                break;
                        case '=':
                                if(i != input.size() -1){
                                        cout << "case =\n";
                                        if(input[i+1] == '='){
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                                        
                                                tokens.push_back("==");
                                                last_delim= i+1;
												i=i+1;
                                        }
                                        else{
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                                      

                                                tokens.push_back("=");
                                                last_delim= i;
                                        }
                                }
                                else{
                                        before ="";
                                        for(int k=last_delim+1; k<i; k++){
                                                before= before +input[k];
                                        }
										if(before != ""){
											tokens.push_back(before);
										}                                                    

                                        tokens.push_back("=");
                                        last_delim= i;
                                }
                                break;
                        case '!':
                                //cout << "case !\n";
                                if(i != input.size() -1){
                                        if(input[i+1] == '='){
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                               

                                                tokens.push_back("!=");
                                                last_delim= i+1;
                                        }
                                        else{
                                                errMessage = "'!' is not used properly. Please use correct syntax\n";
                                                errFound = true;
                                                i = input.size() -1;
                                        }
                                }
                                else{
                                        errMessage = "'!' is not used properly. Please use correct syntax\n";
                                        errFound = true;
                                        i = input.size() -1;
                                }
                                break;
                        case '>':
                               // cout << "case >\n";
                                if(i != input.size() -1){
                                        if(input[i+1] == '='){
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}     

                                                tokens.push_back(">=");
                                                last_delim= i+1;
                                        }
                                        else{
                                                before ="";
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                                

                                                tokens.push_back(">");
                                                last_delim= i;
                                        }
                                }
                                else{
                                        before ="";
                                        for(int k=last_delim+1; k<i; k++){
                                                before= before +input[k];
                                        }
										if(before != ""){
											tokens.push_back(before);
										}                                                

                                        tokens.push_back(">");
                                        last_delim= i;
                                }
								break;
                        case '&':
                               // cout << "case &\n";
                                if(i != input.size() -1){
                                        if(input[i+1] == '&'){
                                                before ="";
                                                i+=1; // added this because there was a bug. not sure if the last delimiter thing is working the way it was originally intended.
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
                                                }
												if(before != ""){
													tokens.push_back(before);
												}                                                   

                                                tokens.push_back("&&");
                                                last_delim= i+1;
                                        }
                                        else{
                                                errMessage = "'&' is not used properly. Please use correct syntax\n";
                                                errFound = true;
                                                i = input.size() -1;
                                        }
                                }
                                else{
                                        errMessage = "'&' is not used properly. Please use correct syntax\n";
                                        errFound = true;
                                        i = input.size() -1;
                                }
                                break;
                        case ',':
                               // cout << "case ,\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
								if(before != ""){
									tokens.push_back(before);
								}                                      

                                tokens.push_back(",");
                                last_delim= i;
                                break;
                               
       
                        case '(':
                               // cout << "case (\n";
                                before ="";
                                after = "";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
                                for(int j = i+1; j<input.size(); j++){
                                        if(input[j] == ')' && parentheses ==0){
										//		cout << "1\n";
                                                closed = j;
                                                j=input.size();
                                        }
                                        else if(input[j] == ')' && parentheses !=0){
										//		cout << "2\n";
                                                parentheses = parentheses -1;
                                                after= after +input[j];
                                        }
                                        else if(input[j] == '(' ){
										//		cout << "3\n";
                                                parentheses = parentheses +1;
                                                after= after +input[j];
                                        }
                                        else{
									//			cout << "4\n";
                                                after= after +input[j];
                                        }
                                }
								//cout << "5\n";
								cout << closed <<endl;
                                last_delim=closed;
                                i=last_delim;
								if(before != ""){
									tokens.push_back(before);
								}
                                tokens.push_back("(");
								if(after != ""){
									tokens.push_back(after);
								}
                                tokens.push_back(")");
                                break;
                        case '|':
                               // cout << "case |\n";
                                if(i != input.size() -1){
                                        if(input[i+1] == '|'){
                                                before ="";
                                                i+=1; // added this because there was a bug. not sure if the last delimiter thing is working the way it was originally intended.
                                                for(int k=last_delim+1; k<i; k++){
                                                        before= before +input[k];
												}
												if(before != ""){
													tokens.push_back(before);
												}                                        

                                                tokens.push_back("||");
                                                last_delim= i+1;
                                        }
                                        else{
                                                errMessage = "'|' is not used properly. Please use correct syntax\n";
                                                errFound = true;
                                                i = input.size() -1;
                                        }
                                }
                                else{
                                        errMessage = "'|' is not used properly. Please use correct syntax\n";
                                        errFound = true;
                                        i = input.size() -1;
                                }
                                break;
                        case '"':
                               // cout << "case quote\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
                                last_delim =i;
								if(before != ""){
									tokens.push_back(before);
								}                                                       

                                break;
                        case ';':
                               // cout << "case ;\n";
                                if(i != input.size()-1){
                                        errMessage = "';' is not used properly. Please use correct syntax\n";
                                        errFound = true;
                                        i = input.size() -1;
                                }
                                else {
                                    before ="";
                                    for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                    }
									if(before != ""){
										tokens.push_back(before);
									}                                        

                                }
                                break;
						case ' ':
                                //cout << "case ' '\n";
                                before ="";
                                for(int k=last_delim+1; k<i; k++){
                                        before= before +input[k];
                                }
                                last_delim =i;
								if(before != ""){
									tokens.push_back(before);
								}            

                                break;
                        default:
							if(i == input.size() -1){
								before ="";
                                for(int k=last_delim+1; k<=i; k++){
                                        before= before +input[k];
                                }
								if(before != ""){
									tokens.push_back(before);
								}  
							}
                            break;
                       
                               
                }
        }
        if(errFound){
                cout << errMessage;
                return errorVector;
        }
   
    cout<<"PARSE IS DONE"<<endl;
	for(int i = 0; i<tokens.size(); i++){
		cout << tokens[i] << endl;
	}
        return tokens;
}

//for this week, returns true if proper syntax, false if not. will need to implement way to apply comparison to relations next week
//originally, pass a vector whose first and last entries are NOT parentheses. so for example: "select (...) animals", the parser will have ... saved as one token. call "condition(parse(**vector entry of ...**))"

int main()
{
	string input;
	cout << "Please enter commands below.\n";
	int exit_check=0;
	DBMS dbms;
	while(exit_check != 1){
		getline(cin, input);
		exit_check = dbms.execute(input);
	}
	
	
	
	
	
}




