#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <typeinfo>

using namespace std;

enum element{
	wall, space, start, goal
};

class square{
public:
	element block;
	char type;
	bool visited;
	square();
	square(char);
	int x;
	int y;
};

square::square(){

}

square::square(char c){
	if(c == '#')
		block = wall;
	else if(c == '.')
		block = space;
	else if(c == 'o')
		block = start;
	else if(c == '*')
		block = goal;

	c = type;
	visited = false;
	x = 0;
	y = 0;
}

class maze{
public:
	square **maze_map;
	int x_origin;
	int y_origin;
	maze(int, int);
	maze(maze*);
	~maze();
	void display();
	int rows;
	int columns;
	string tostring();
};

maze::maze(int rows, int columns){
	maze_map = new square*[rows];
	for(int i = 0; i < rows; i++)
		maze_map[i] = new square[columns];
}

maze::maze(maze* m){
    maze_map = new square*[m->rows];
    for(int i = 0; i < m->rows; i++)
        maze_map[i] = new square[m->columns];

	for(int i = 0; i < m->rows; i++){
		for(int j = 0; j < m->columns; j++){
			maze_map[i][j] = m->maze_map[i][j];
			if(maze_map[i][j].type == 'o') {
				x_origin = j;
				y_origin = i;
			}
			maze_map[i][j].x = j;
			maze_map[i][j].y = i;
			maze_map[i][j].visited = false;
		}
	}
}

maze::~maze(){
	for(int i = 0; i < rows; i++)
		delete[] maze_map[i];
	delete[] maze_map;
}

void maze::display(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			cout<<maze_map[i][j].type;
		cout<<endl;
	}
}

string maze::tostring(){
	string str_maze = "";
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			str_maze = str_maze + maze_map[i][j].type;
		str_maze = str_maze + "\n";
	}
	return str_maze;
}

template <class T>
class agenda{
private:
	T *adt;
	int type; //1 for stack, 2 for queue
public:
	agenda();
	~agenda();
	void add(square);
	bool remove();
	bool isEmpty();
	int getSize();
	square peek();
};

template <class T>
agenda<T>::agenda(){
	adt = new T();
	string t1 = typeid(*adt).name();
	string t2 = typeid(stack<square>).name();
	string t3 = typeid(queue<square>).name();
	if(t1.compare(t2) == 0)
		type = 1;
	else if(t1.compare(t3) == 0)
		type = 2;
}

template <class T>
agenda<T>::~agenda(){
	delete adt;
}

template <class T>
void agenda<T>::add(square s){
	adt->push(s);
}

template <class T>
bool agenda<T>::remove(){
	if(adt->empty())
		return false;
	else{
		adt->pop();
		return true;
	}
}

template <class T>
bool agenda<T>::isEmpty(){
	if(adt->empty())
		return true;
	else
		return false;
}

template <class T>
int agenda<T>:: getSize(){
	return adt->size();
}

template <class T>
square agenda<T>::peek(){
	if(type == 1)
		return ((stack<square>*)adt)->top();
	else
		return ((queue<square>*)adt)->front();
}

template <class T>
class mazesolver{
private:
	agenda<T> *a;
	maze *m;
	stack<square> *track;
public:
    mazesolver();
	mazesolver(maze*);
	~mazesolver();
	void solve();
	void writesolution();
};

template <class T>
mazesolver<T>::mazesolver(){

}

template <class T>
mazesolver<T>::mazesolver(maze *themaze){
	a = new agenda<T>();
	m = themaze;
	track = new stack<square>();
}

template <class T>
mazesolver<T>::~mazesolver(){
	delete a;
	for(int i = 0; i < m->rows; i++)
        delete[] m->maze_map[i];
	delete[] m;
	delete track;
}

template <class T>
void mazesolver<T>::solve(){
	int x = m->x_origin;
	int y = m->y_origin;
	int flag = 0;
//	maze *dupli = new maze(m);
	maze *dupli = m;

	track->push(dupli->maze_map[y][x]);
	while(track->top().block != goal){
		x = track->top().x;
		y = track->top().y;
		square cur = track->top();
		cur.visited = true;
		dupli->maze_map[y][x] = cur;

			a->add(dupli->maze_map[y][x-1]);
			a->add(dupli->maze_map[y-1][x]);
			a->add(dupli->maze_map[y][x+1]);
			a->add(dupli->maze_map[y+1][x]);

		while(a->peek().block == wall || a->peek().visited == true){
			a->remove();
			if(a->isEmpty()){
				flag = 1;
				break;
			}
		}

		if(flag == 1){
			track->pop();
			flag = 0;
		}
		else
			track->push(a->peek());

		while(!a->isEmpty())
			a->remove();
	}
	track->pop();
}

template <class T>
void mazesolver<T>::writesolution(){
	int x, y;
	maze *dupli = new maze(m);
	ofstream outputFile("mazeSample.out");

	while(track->size() > 1){
		track->top().type = 'x';
		x = track->top().x;
		y = track->top().y;
		dupli->maze_map[y][x] = track->top();
		track->pop();
	}
	track->pop();

	for(int i = 0; i < m->rows; i++){
		for(int j = 0; j < m->columns; j++)
			outputFile<<dupli->maze_map[i][j].type;
		outputFile<<endl;
	}
	outputFile<<endl;
	outputFile.close();
}

int main(){
	ifstream inputFile("maze.in");
	if(inputFile.is_open()){
		while(!inputFile.eof()){
			int testcases;
			inputFile >> testcases;
			for(int i = 0; i < testcases; i++){
				int rows, columns;
				inputFile >> rows >> columns;

				maze* themaze = new maze(rows, columns);

				for(int k = 0; k < rows; k++){
					string squaretype;
					inputFile >> squaretype;
					for(int l = 0; l < columns; l++){
						square s(squaretype[l]);
						themaze->maze_map[k][l] = s;
					}
				}
				mazesolver< queue<square> > ms(themaze);
				ms.solve();
				ms.writesolution();
				mazesolver< stack<square> > ss(themaze);
				ss.solve();
				ss.writesolution();
			}
		}
		inputFile.close();
	}
	return 0;
}
