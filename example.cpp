#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include<bits/stdc++.h>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
	os << "[";
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin()) os << ", ";
		os << *it;
	}
	return os << "]";
}

int split(string& str, char l,char delim = ' '){
        stringstream ss(str);
        string token;
        int c=0,lit=0;
        while (getline(ss, token, delim)) {
            c++;
            if(c==3){
               lit = stoi(token);
               break;
            }
            //cout<<lit<<endl;
        }
        return lit;
}
int lit=0;

vector<vector<int>> parse_dimacs(string filename) {
	vector<vector<int>> clauses;
	ifstream in(filename);
	string line;
	while (!in.eof()) {
		getline(in, line);
		if (line == "") continue;
		if (line.front() == 'c') continue;
		if (line.front() == 'p'){
            lit=split(line,lit,' ');
            continue;
		}
		clauses.emplace_back();
		const char* begin = line.c_str();
		char* end = nullptr;
		while (true) {
			auto id = strtoll(begin, &end, 10);
			begin = end;
			if (id == 0) break;
			clauses.back().emplace_back(id);
		}
		if (clauses.back().empty()) clauses.pop_back();
	}
	return clauses;
}

struct T{
    int x=0;
    int v=0;
    bool b=false;
} t,p;

vector <T> trail;
vector <int> myVector;

bool BCP(vector<vector<int>> clauses){
    for(int i=0;i<clauses.size();i++){
        int c=0,myValue=0,flag=0;
        int row=clauses[i].size();
        for(int j=0;j<row;j++){
            int index=abs(clauses[i][j]);
            if(clauses[i][j]>0 && myVector[index]==1){
                flag=1;
            }
            else if(clauses[i][j]<0 && myVector[index]==0){
                flag=1;
            }
        }
        if(flag==1) {
            //cout<<"SAT\n";
            continue;
        }
        for(int j=0;j<row;j++){
            int index=abs(clauses[i][j]);
            bool flag2=0;
            if(myVector[index]!=10) c++;
            else myValue = clauses[i][j];
        }
        if(c==row-1){
            //cout<<"UNIT\n";
            //cout<<myValue<<endl;
            if(myValue>0){
                t.x=myValue;
                t.v=1;
                t.b=true;
                trail.push_back(t);
                myVector[t.x]=1;
                //return true;
            }
            else if(myValue<0){
                t.x=abs(myValue);
                t.v=0;
                t.b=true;
                trail.push_back(t);
                myVector[t.x]=0;
                //return true;
            }
        }
        else if(c==row) return false;
    }
    return true;
}

bool decide(){
    //cout<<"size :"<<trail.size()<<endl;
    //cout<<"DECIDE"<<endl;
    if(trail.empty()) {
        t.x=1;
        t.v=0;
        t.b=false;
        trail.push_back(t);
        myVector[t.x]=0;
        return true;
    }
    for(int i=1;i<=lit;i++){
        int flag=0;
        for(int j=0;j<trail.size();j++){
            if(trail[j].x==i) flag=1;
        }
        if(flag==0) {
            t.x=i;
            t.v=0;
            t.b=false;
            trail.push_back(t);
            myVector[t.x]=0;
            return true;
        }
    }
    return false;
}

bool backtrack(){
    while(true){
        if(trail.empty()) return false;
        p=trail.back();
        trail.pop_back();
        myVector[p.x]=10;
        if(!p.b && p.v==0){
            p.v=1;
            p.b=true;
            trail.push_back(p);
            myVector[p.x]=1;
            return true;
        }
        else if(!p.b && p.v==1){
            p.v=0;
            p.b=true;
            trail.push_back(p);
            myVector[p.x]=0;
            return true;
        }
    }
}

bool DPLL(vector<vector<int>> clauses){
    while(trail.size()>0) trail.pop_back();
    if(!BCP(clauses)) return false;
    while(true){
        if(!decide()) return true;
        while(!BCP(clauses))
            if(!backtrack()) return false;
    }
}

int main(int argc, char* argv[]) {
	assert(argc == 2);
	auto clauses = parse_dimacs(argv[1]);

    for(int i=1;i<=lit;i++){
        myVector.push_back(10);
    }
	//cout<<lit<<endl;
	//cout << clauses << endl;

    if(DPLL(clauses)){
       cout<<"SAT"<<endl;
       exit(10);
    }
    else if(!DPLL(clauses)){
        cout<<"UNSAT"<<endl;
        exit(20);
    }
}
