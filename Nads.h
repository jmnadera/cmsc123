#include <stack>
#include <queue>
#include <array>
#include <vector>
#include <fstream>
#include <typeinfo>

using namespace std;

enum element_type{ wall, space, source, destination };

class square{
public:
	char element_type;
	int loc_x, loc_y;
	bool visited;
	square();
	square(char, int, int);
};

square::square(){
	element_type = wall;
}
square::square(char c, int x, int y){
	if(c == '#') element_type = wall;
	else if(c == '.') element_type = space;
	else if(c == 'o') element_type = source;
	else if(c == '*') element_type = destination;
	this->loc_x = x;
	this->loc_y = y;
	visited = false;
}

class maze{
private:
	int num_rows, num_columns;
public:
	square **maze_map;
  int origin_x, origin_y;
	//maze(list);
	maze(vector<string>);
	~maze();
//  square operator[][](int, int);
  int getLocation_origin_x(){return origin_x;}
  int getLocation_origin_y(){return origin_y;}
	void display();
	string toString();

};

maze::maze(vector<string> list_of_lines){ 	//create a 2D array of sqaures for cloning the maze
  int n = list_of_lines.size(); // n number of rows
  int m = list_of_lines[0].length(); // number of columns
	maze_map = new square*[n];
	for(int i = 0; i < n; i++)
		maze_map[i] = new square[m];
	num_rows = n;
	num_columns = m;
	for(int i = 0; i < num_columns; i++){
		for(int j = 0; j < num_rows; j++){
			maze_map[i][j] = square(list_of_lines[i][j],i, j); // converts a character into a square w/ location
      if(maze_map[i][j].element_type == source){
          origin_x = i;
          origin_y = j;
      }
    }
	}
}

maze::~maze(){
	for(int i = 0; i < num_rows; i++)
		delete[] maze_map[i];
	delete[] maze_map;
}
/*
square maze::operator[][](int x, int y){
  return maze_map[x][y];
}
*/
void maze::display(){
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_columns; j++)
			cout << maze_map[i][j].element_type;
		cout << endl;
		}
}

string maze::toString(){
	string str = "";
	for(int i = 0; i < num_rows;i++){
		for(int j=0; i < num_columns; j++)
			str+=maze_map[i][j].element_type;
		str+="\n";
	}
	return str;
}

enum adt_type { Stack, Queue };
template <class T >
class agenda{
private:
	T *adt;
	int adt_type; //1 for stack, 2 for queue
public:
	agenda();
	~agenda(){delete adt;}
	void add(square);
	bool remove();
	bool isEmpty();
	int getSize();
	square peek();
};

template <class T >
agenda< T >::agenda(){
	adt = new T();
	string t1 = typeid(*adt).name();
	string t2 = typeid(stack<square>).name();
	string t3 = typeid(queue<square>).name();
	if(t1.compare(t2) == 0)	adt_type = 1;
	else if(t1.compare(t3) == 0) adt_type = 2;
}

template <class T >
void agenda< T >::add(square a){
	if(adt_type == Stack)	adt.push(a);
	else adt.enqueue(a);
}

template <class T >
bool agenda <T >::remove(){
	if(adt.empty())
				return false;
	else{
		if(adt_type == Stack) adt.pop();
		else	adt.dequeue();
  }
	return true;
}

template <class T >
bool agenda< T >::isEmpty(){
		return adt.empty();
}

template <class T >
int agenda< T >::getSize(){
	return adt.size();
}

template <class T >
square agenda< T >::peek(){
	if(adt_type = Stack)	return adt.top();
	else return adt.front();
}

template <class T >
class mazeSolver{
private:
	maze *m;
  stack< stack<square> > options;
  stack<square> path;
public:
  mazeSolver();
	mazeSolver(vector<string>);
	~mazeSolver();
	bool solve();
	void writesolution(string);
};

template <class T >
mazeSolver< T >::mazeSolver(){
  m = NULL;
}

template< class T >
mazeSolver< T >::mazeSolver( vector<string> list_of_lines){
  m = new maze(list_of_lines);
}

template <class T >
mazeSolver< T >::~mazeSolver(){
  delete m;
}

template <class T >
bool mazeSolver< T >::solve(){
  square current_square = m -> maze_map [m->getLocation_origin_x()] [m->getLocation_origin_y()];

  do{
    square left_sqr   = m->maze_map[current_square.loc_x][current_square.loc_y-1];
    square up_sqr     = m->maze_map[current_square.loc_x-1][current_square.loc_y];
    square right_sqr  = m->maze_map[current_square.loc_x][current_square.loc_y+1];
    square buttom_sqr = m->maze_map[current_square.loc_x+1][current_square.loc_y];

    if(options.empty() || !options.top().isEmpty()){
      agenda< T > a;
      if(!options.empty()) current_square = options.top().peek();
      if(left_sqr.element_type   != wall && !(left_sqr.visited))   a.add(left_sqr);
      if(up_sqr.element_type     != wall && !(up_sqr.visited))     a.add(up_sqr);
      if(right_sqr.element_type  != wall && !(right_sqr.visited))  a.add(right_sqr);
      if(buttom_sqr.element_type != wall && !(buttom_sqr.visited)) a.add(buttom_sqr);

      options.push(a);
      path.push(current_square);
      current_square.visited = true;
      if(!a.empty())
        current_square = a.peek();
    }
    else{
      options.pop();
      path.pop();
      options.top().remove();
      if(!options.top().isEmpty())
        current_square = options.top().peek();
    }
    if(current_square.element_type == destination)
      return true;
  } while (current_square.element_type!= destination || !(options.empty()));
  return false;
}

template <class T >
void mazeSolver< T >::writesolution(string filename){
    ofstream output_file(filename, ios_base::app); // file where we are about to write
    output_file << m->toString();
    output_file << "\n" << endl;
    output_file.close();
}
