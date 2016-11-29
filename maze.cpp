#include <iostream>
#include <stack>
#include <queue>
#include <array>
#include <vector>
#include <fstream>
#include <typeinfo>

using namespace std;

class square{
public:
	char element_type;
	int loc_x, loc_y;
	bool visited;
	square();
	square(char, int, int);
};

square::square(){
	element_type = '#';
}

square::square(char c, int x, int y){
	element_type = c;
	this->loc_x = x;
	this->loc_y = y;
	visited = false;
}


class maze{
public:
	int num_rows, num_columns;
	square **maze_map;
  int origin_x, origin_y;
	maze(vector<string>);
	~maze();
  int getLocation_origin_x(){return origin_x;}
  int getLocation_origin_y(){return origin_y;}
	string toString();

};

maze::maze(vector<string> list_of_lines){ 	//create a 2D array of sqaures for cloning the maze
  num_rows = list_of_lines.size(); // n number of rows
  num_columns = list_of_lines[0].size(); // number of columns
	maze_map = new square*[num_columns];
	for(int i = 0; i < num_columns; i++){
		maze_map[i] = new square[num_rows];
		for(int j = 0; j < num_rows; j++){
			maze_map[i][j] = square(list_of_lines[i][j],i, j); // converts a character into a square w/ location
      if(maze_map[i][j].element_type == 'o'){
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

string maze::toString(){
	string str = "";
	for(int i = 0; i < num_rows;i++){
		for(int j=0; i < num_columns; j++)
			str+=maze_map[i][j].element_type;
		str+="\n";
	}
	return str;
}


template <class T>
class agenda{};

template <>
class agenda<stack <square> >{
private:
	stack <square> adt;
public:
	void add(square sqr){
		adt.push(sqr);}
	bool del(){
		adt.pop();}
	bool isEmpty(){
		return adt.empty();}
	int getSize(){
		adt.size();}
	square peek(){
		return adt.top();}
};

template<>
class agenda <queue <square> >{
private:
	queue<square> adt;
public:
	void add(square sqr){
		adt.push(sqr);}
	bool del(){
		adt.pop();}
	bool isEmpty(){
		return adt.empty();}
	int getSize(){
		adt.size();}
	square peek(){
		return adt.front();}
};

template <class T >
class mazeSolver{
private:
	maze *m;
  stack< agenda<T > > options;
  stack<square> track;
public:
	mazeSolver(vector<string>);
	~mazeSolver();
	void solve();
	void writesolution(string);
};

template< class T >
mazeSolver< T >::mazeSolver( vector<string> list_of_lines){
  m = new maze(list_of_lines);
}

template <class T >
mazeSolver< T >::~mazeSolver(){
  delete m;
}

template <class T >
void mazeSolver<T >::solve(){
  square current_square = m->maze_map [m->getLocation_origin_x()] [m->getLocation_origin_y()];

  do{
    square left_sqr   = m->maze_map[current_square.loc_x][current_square.loc_y-1];
    square up_sqr     = m->maze_map[current_square.loc_x-1][current_square.loc_y];
    square right_sqr  = m->maze_map[current_square.loc_x][current_square.loc_y+1];
    square buttom_sqr = m->maze_map[current_square.loc_x+1][current_square.loc_y];

    if(!options.top().isEmpty()){
      agenda< T > agnd;
      if(!options.empty()) current_square = options.top().peek();
      if(left_sqr.element_type   != '#' && !left_sqr.visited)   agnd.add(left_sqr);
      if(up_sqr.element_type     != '#' && !up_sqr.visited)     agnd.add(up_sqr);
      if(right_sqr.element_type  != '#' && !right_sqr.visited)  agnd.add(right_sqr);
      if(buttom_sqr.element_type != '#' && !buttom_sqr.visited) agnd.add(buttom_sqr);

			m->maze_map[current_square.loc_x][current_square.loc_y].visited = true;
			options.push(agnd);

      if(!agnd.isEmpty()){
        current_square = m->maze_map[options.top().peek().loc_x][options.top().peek().loc_y];
				track.push(current_square);
				}
    	}
    else{
			if(!options.empty())
      	options.pop();
      if(!track.empty())
				track.pop();
      options.top().del();
      if(!options.top().isEmpty() && !options.empty()){
				options.top().del();
        current_square = m->maze_map[options.top().peek().loc_x][options.top().peek().loc_y];
				track.push(current_square);
			}
    }
  } while (current_square.element_type!= '*' && !(options.empty()));
	if(current_square.element_type == '*')
		track.pop();
}

template <class T >
void mazeSolver<T >::writesolution(string filename){
		ofstream output_file;
		while(!track.empty()){
			m->maze_map[track.top().loc_x][track.top().loc_y].element_type = 'x';
			track.pop();
		}
		output_file.open(filename.c_str(), ios_base::app);
    output_file << m->toString();
    output_file << endl;
    output_file.close();
}


int main(){
	ifstream input_file("maze.in");
	if(input_file.is_open()){
			int number_testcases;
			input_file >> number_testcases;

				for(int i = 0; i < number_testcases; i++){
					int m, n;
					input_file >> m;
					input_file >> n;
					vector<string> list_of_lines;
					for(int j = 0; j < m; j++){
						string line;
						input_file >> line;
						list_of_lines.push_back(line);
					}
					// Time to PLAY the maze :D
					mazeSolver<queue <square> > usingQueue(list_of_lines);
					mazeSolver<stack <square> > usingStack(list_of_lines);

					usingQueue.solve();
					usingQueue.writesolution("samplemaze.out");
					usingStack.solve();
					usingStack.writesolution("samplemaze.out");

				}
		input_file.close();
	}
	return 0;
}
