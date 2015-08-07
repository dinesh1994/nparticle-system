#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

template <class T> class Stack{
	private:
			vector<T> data;
	public:
			void pop(){   data.pop_back(); }
			T top(){ return data.back(); }
			void push(T ele){ data.push_back(ele); }
			bool empty(){ return data.empty(); }
			};



int main()
{

Stack<char> stack;

string s;

ifstream in("readme.txt",ios::in);

	if(!in)
	{
 		cerr<<"Cannot open file "<<endl;
 		return 0;
	}

	char ch = in.get();
	while(ch!=EOF)
	{
		stack.push(ch);
        s += stack.top();
        ch = in.get();
	}
	in.close();
    fflush(stdout);

	cout<<s<<endl;

return 1;
}
