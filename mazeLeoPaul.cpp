#include<iostream>
#include "node.h"
#include <stack>
#include <queue>

#include<fstream>
#include<typeinfo>

using namespace std;

enum block{wall, enter, exit, space, path};

class square{
public:
    int rpos, cpos; //position
    block type;
    bool visit;
    square();
    square(char, int, int);
    int isAdjacent(square);
};
square::square(){
    rpos= cpos =0;
}
square::square(char ch, int r, int c){
    if(ch=='#')
        type = wall;
    else if(ch == 'o')
        type = enter;
    else if(ch == '*')
        type = exit;
    else if(ch == '.')
        type = space;
    //else
      //  throw 123;

    rpos = r;
    cpos = c;
}

class maze{
public:
    int rows, cols;
    square start, fin;
    square** laMaze;
    maze(string, int, int);
    ~maze();
    string tostring();
};

string maze::tostring(){
    string str = "";
    for(int i= 0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(laMaze[i][j].type == wall)
                str = str + "#";
            else if(laMaze[i][j].type == enter)
                str = str + "o";
            else if(laMaze[i][j].type == exit)
                str = str + "*";
            else if(laMaze[i][j].type == space)
                str = str + ".";
            else
                str = str + "X";
        }
        str = str + "\n";
    }

    return str;
}

maze::maze(string ch, int r, int c){
    rows = r;
    cols = c;

    //square *tmp;

    laMaze = new square*[rows];
    for (int i = 0; i < rows; i++)
        laMaze[i] = new square[cols];

    for(int i= 0; i<rows; i++){
        for(int j=0; j<cols; j++){
                square tmp(ch[(cols+1)*i + j], i, j);
                //tmp = new square(ch[i*cols + j], i, j);
                laMaze[i][j]= tmp;
                if(tmp.type == enter){
                    start = tmp;
                    start.visit = true;
                }
                else if(tmp.type == exit)
                    fin = tmp;

        }
    }

    //delete tmp;
}

maze::~maze(){
    for(int i=0; i<rows; i++)
        delete[] laMaze[i];

    delete[] laMaze;

}

template <class T>
class agenda{
private:
    T *adt;
    int type; //1 for stack, 2 for queue
public:
    agenda();
    ~agenda(){delete adt;}
    void add(square);
    bool remove();
    bool isempty();
    int getsize();
    square peek();
    int getType();
};

template <class T>
agenda<T>::agenda(){
    adt = new T();
    string t1 = typeid(*adt).name();
    string t2 = typeid(stack<square>).name();
    string t3 = typeid(queue<square>).name();
    cout<<t1<<endl<<t2<<endl<<t3<<endl;
    if(t1.compare(t2)==0)
        type = 1;
    else
    if(t1.compare(t3)==0)
        type = 2;
}

template <class T>
void agenda<T>::add(square s){
    //if(type == 1)
        adt->push(s);

}

template <class T>
bool agenda<T>::remove(){
    if(isempty())
        return false;
    else{
       // if(type ==1)
            adt->pop();
        //else
           // adt->dequeue();
        return true;
    }
}

template <class T>
bool agenda<T>::isempty(){
    if(adt->empty())
        return true;
    else
        return false;
}

template <class T>
int agenda<T>::getsize(){
    return adt->size();
}

template <class T>
square agenda<T>::peek(){
    //if(type == 1)
      //  return adt->top();
  //else
      return adt->front();
}

template <class T>
int agenda<T>::getType(){
    return type;
}

template <class T>
class mazesolver{
private:
    agenda<T> *a;
    maze *m;

public:
    mazesolver(maze*);
    ~mazesolver();
    void solve();
    void writesolution();
};

template <class T>
mazesolver<T>::mazesolver(maze *mz){
    m = new maze(mz->tostring(), mz->rows, mz->cols);
    a = new agenda<T>;
}

template <class T>
mazesolver<T>::~mazesolver(){
    delete a;
    delete m;
}

template <class T>
void mazesolver<T>::solve(){
    stack<agenda<T> > lePath;
    square *curr = &(m->start);

    a->add(*curr);
    int flag=0;
    do{
        cout<<"ewrwe"<<endl;
        //push to agenda
        cout<<curr->rpos<<"  "<<curr->cpos<<endl;
        //cout<<a->peek().rpos << "  "<<a->peek().cpos<<endl;
        square *currLeft = &(m->laMaze[curr->rpos][curr->cpos-1]);
        square *currUp = &(m->laMaze[curr->rpos-1][curr->cpos]);
        square *currRight = &(m->laMaze[curr->rpos][curr->cpos+1]);
        square *currDown = &(m->laMaze[curr->rpos+1][curr->cpos]);
        if(a->getType() == 1 && curr->visit==false)
            a->add(*curr);
        if(currLeft->type == exit || currUp->type == exit || currRight->type == exit || currDown->type == exit){
            lePath.push(*a);
            break;
        }
        if(currLeft->type == space && currLeft->visit == false){
            a->add(*currLeft);
        }
        if(currUp->type == space && currUp->visit == false){
            a->add(*currUp);
        }
        if(currRight->type == space && currRight->visit == false){
            a->add(*currRight);
        }
        if(currDown->type == space && currDown->visit == false){
            a->add(*currDown);
        }
        if(a->getType()==2 && curr->visit==false){
            a->add(*curr);
            cout<<"ewrwe"<<endl;
        }
        if(flag == 0)
            lePath.push(*a);
         cout<<a->peek().rpos<<"   "<<a->peek().cpos<<"   a"<<a->getsize() <<endl;
        if(a->getsize()==1){
            cout<<"1"<<endl;
            lePath.pop();
            flag=1;
            cout<<"1.1"<<endl;
            agenda<T> tmp1 = lePath.top();
            cout<<"1.2"<<endl;
            a = &tmp1;
            cout<<"1.3"<<endl;
            m->laMaze[curr->rpos][curr->cpos].visit = true;
            cout<<"1.4"<<endl;
            square tmp = lePath.top().peek();
            cout<<"1.5"<<endl;
            curr = &(m->laMaze[tmp.rpos][tmp.cpos]);
            cout<<"1.6"<<endl;
        }

        else{
            //cout<<"2"<<endl;

             //cout<<"2.1"<<endl;
             cout<<"sasa  "<<a->peek().rpos << "  "<<a->peek().cpos<<endl;
             flag = 0;
            m->laMaze[curr->rpos][curr->cpos].visit = true;
            curr = &(m->laMaze[a->peek().rpos][a->peek().cpos]);
            //m->laMaze[curr->rpos][curr->cpos].visit = true;
            //cout<<"2.2"<<endl;
            a->remove();
            //cout<<"2.3"<<endl;
            a = new agenda<T>;
            //cout<<"2.4"<<endl;
            //m->laMaze[curr->rpos][curr->cpos].visit = true;
            //cout<<"2.5"<<endl;
        }
        cout<<"curr  "<<curr->rpos<<"sds"<<curr->cpos<<endl;

    }
    while(!(lePath.empty()));

    while(lePath.size()>1){
        while(lePath.top().getsize()>1)
            lePath.top().remove();
        m->laMaze[lePath.top().peek().rpos][lePath.top().peek().cpos].type=path;
        lePath.pop();
    }

}

template <class T>
void mazesolver<T>::writesolution(){

    ofstream ofp("maze.out", ofstream::out | ofstream::app);
    ofp << m->tostring() <<endl;
    ofp.close();
}

int main(){
    ifstream fp("maze.in");
    char tmp;
    int m, n;
    int cases;
    fp >> cases;
    for(int j=0; j < cases; j++){
        fp >> m >> n;
        string mazeChar = "";

        for (int i = 0; i < m*n; i++){
            fp >> tmp;
            mazeChar = mazeChar + tmp;
            if(i%n == n-1){
                mazeChar = mazeChar + "\n";
            }
        }

        cout << mazeChar;

        maze *aMaze = new maze(mazeChar, m, n);
        cout << aMaze->tostring();

        //agenda<stack<square> > a; //how to know what type of agenda is a?
        //agenda<queue<square> > b; //how to know what type of agenda is b?

       // mazesolver<stack<square> >  *ms = new mazesolver<stack<square> > (aMaze);
        //ms->solve();
       mazesolver<queue<square> >  *mq = new mazesolver<queue<square> > (aMaze);
       mq->solve();
        mq->writesolution();
    }
    fp.close();
    return 0;
}
