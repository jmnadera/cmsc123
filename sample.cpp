#include <iostream>
#include <stack>

using namespace std;
int main(){
  stack<int> a; // can't declare a stack successfully :< Why!?
  int x = 5 ;
  a.push(x);
  a.push(6);
  a.pop();
  a.pop();
   a.top();
   // just wanted to know what's the return
   // value of pop()
/*
  char c = 'a';
  string str = "bc";
  string abc = c + str;
  cout<< abc << endl;
*/
  return 0;
}
