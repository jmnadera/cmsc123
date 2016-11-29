#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

/* SQUARE CLASS */
class square{
public:
	bool visited;
	char type;
	int x,y;
	square();
	square(char c,int,int);
};

square::square(){
	type = '#';
}

square::square(char c,int x,int y){
	this->x = x;
	this->y = y;
	type = c;
	visited = false;
}

/* MAZE CLASS */
class maze{
public:
	int m,n;
	int or_x,or_y;
	square **map;
	maze(vector<string> s);
	~maze();
	string toString();
};

maze::maze(vector<string> s){
	m = s.size();
	n = s[0].size();
	map = new square*[m];
	for(int i = 0; i < m; i++){
		map[i] = new square[n];
		for(int j = 0; j < n; j++){
			map[i][j] = square(s[i][j],i,j);
			if(map[i][j].type == 'o'){
				or_x = i; or_y = j;
			}
		}
	}
}

maze::~maze(){
	for(int i = 0; i < m ; i++)
		delete[] map[i];
	delete[] map;
}

string maze::toString(){
	string s = "";
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++) s+=map[i][j].type;
		s+="\n";
	}
	return s;
}

/* AGENDA CLASS */
template <class T>
class agenda{};

template <>
class agenda<stack<square> >{
private:
	stack<square> adt;
public:
	void add(square a){adt.push(a);}
	bool del(){adt.pop();}
	bool isEmpty(){return adt.empty();}
	int getSize(){return adt.size();}
	square peek(){return adt.top();}
};

template<>
class agenda<queue<square> >{
private:
	queue<square> adt;
public:
	void add(square a){adt.push(a);}
	bool del(){adt.pop();}
	bool isEmpty(){return adt.empty();}
	int getSize(){return adt.size();}
	square peek(){return adt.front();}
};

/* MAZE SOLVER CLASS */
template <class T >
class mazeSolver{
private:
	maze *m;
	stack<agenda<T > > choices;
	stack<square> path;
public:
	mazeSolver(vector<string> s){m = new maze(s);}
	~mazeSolver(){delete m;}
	void solve();
	void writeSolution(string);
};

template <class T >
void mazeSolver<T >::solve(){
	//point current to origin.
	square cur = m->map[m->or_x][m->or_y];
	int flag = 1;
	//while current is not the goal && stack of agendas is not empty.
	do{
		if(flag || !choices.top().isEmpty()){
			agenda<T> a;
			// Check surroundings.
			if(m->map[cur.x][cur.y-1].type!='#' && !m->map[cur.x][cur.y-1].visited) a.add(m->map[cur.x][cur.y-1]);
			if(m->map[cur.x-1][cur.y].type!='#' && !m->map[cur.x-1][cur.y].visited)	a.add(m->map[cur.x-1][cur.y]);
			if(m->map[cur.x][cur.y+1].type!='#' && !m->map[cur.x][cur.y+1].visited) a.add(m->map[cur.x][cur.y+1]);
			if(m->map[cur.x+1][cur.y].type!='#' && !m->map[cur.x+1][cur.y].visited) a.add(m->map[cur.x+1][cur.y]);
			m->map[cur.x][cur.y].visited = true;
			choices.push(a);
			if(!a.isEmpty()){
				cur = m->map[choices.top().peek().x][choices.top().peek().y];
				path.push(cur);
			}
		}
		else{
			if(!choices.empty()) choices.pop();
			if(!path.empty()) path.pop();
			if(!choices.empty()&&!choices.top().isEmpty()) choices.top().del();
			if(!choices.empty()&&!choices.top().isEmpty()){
				cur = m->map[choices.top().peek().x][choices.top().peek().y];
				path.push(cur);
			}
		}
		flag = 0;
	}while(!choices.empty()&&cur.type!='*');
	if(cur.type == '*') path.pop();
}

template <class T >
void mazeSolver<T >::writeSolution(string s){
	for(;!path.empty();m->map[path.top().x][path.top().y].type = 'x',path.pop());
	ofstream outPut;
	outPut.open(s.c_str(),std::ios_base::app);
	outPut << m->toString() << endl;
	outPut.close();
}

/* TESTER */
int main(){
	int x;
	ifstream input("maze.in");
	input >> x; // n is the number of test cases.
	for(int i = 0; i < x; i++){
		int m,n;
		input >> m;
		input >> n;
		vector<string> sList;
		for(int j = 0; j < m; j++){
			string s;
			input >> s;
			sList.push_back(s);
		}
		mazeSolver<stack<square> > stackSolver(sList);
		mazeSolver<queue<square> > queueSolver(sList);
		stackSolver.solve();
		stackSolver.writeSolution("mazeSample.out");
		queueSolver.solve();
		queueSolver.writeSolution("mazeSample.out");
	}
	input.close();
	return 0;
}
