#include<map>
#include<set>
#include<iostream>
#include<iterator>
#include<algorithm>
#include<vector>
#include<string>
#include<stack>
#include<deque>

using namespace std;

//Student ADT

class Student
{
	private:
	string name;
	string usn;
	
	public:
	Student(string name, string usn)
		: name(name)
		, usn(usn)
	{}
	bool operator<(const Student& rhs) const
	{
		return usn < rhs.usn;
	}
	friend ostream& operator<<(ostream& os, const Student& s);
};

//overload operator<< for display of student
ostream& operator<<(ostream& os, const Student& s)
{
    os<<"{"<<s.name<<", "<<s.usn<<"}";
    // write obj to stream
    return os;
}

//Node ADT

template<typename val_t>
class Node
{
	//graph is implemented as an adjacency list
	//Abstraction of a node : 
	//	Attributes:	
	//	- data
	//	- set of adjacent nodes
	
	
	public:
	static int count;
	val_t data;
	int index;	
	
	Node(val_t data)
		: data(data)
		, index(count++)
	{}
	bool operator< (const Node<val_t>& rhs) const
	{
		return data < rhs.data;
	}
};

template<typename val_t>
int Node<val_t>::count = 0;//Need to do this hack for incr of non-const static variables of a class


//Graph ADT

template<typename val_t>
class Graph
{
	private:
	int num_vertices;
	int num_edges;
	map< Node<val_t>, set<Node<val_t>> > graph;
	
	public:
	
	//default ctor
	Graph()
		: num_vertices(0)
		, num_edges(0)
	{}
	
	//parameterized ctor
	Graph(int num_vertices, int num_edges)
		: num_vertices(num_vertices)
		, num_edges(num_edges)
	{}
	
	//method to add edges to the graph
	void add_edge(Node<val_t> src, Node<val_t> dst);
	
	//method to display contents of the graph
	void disp();
	
	//DFS graph traversal
	void dfs(Node<val_t> start_node);
};

template<typename val_t>
void Graph<val_t> :: dfs(Node<val_t> t)
{
	vector<bool> visited(num_vertices,false);
	deque<Node<val_t>> s;//using a deque as a stack	
	s.push_front(t);
	while(!s.empty())
	{
		t = s.back();
		s.pop_back();
		
		if(!visited[t.index])
		{
			cout<<t.data<<"\n";
			visited[t.index] = true;
		}
		for(auto i = graph[t].begin();i!=graph[t].end();i++)
		{
			if(!visited[(*i).index])
				s.push_front(*i);
		}
	}

}

template<typename val_t>
void Graph<val_t> :: add_edge(Node<val_t> src, Node<val_t> dst)
{
	
	//check if src exists
	// doesnt exist - add a pair
	//check if dst exists
	// doesnt exist - add a pair
	//if src exists add dst to the adjacency vector
	//if dst exists add src to the adjacency vector 
	auto it_src = graph.find(src);
	
	if(it_src == graph.end())
	{
		//src doesnt exist
		//must be inserted into the adjacency list
		set< Node<val_t> > v;
		v.insert(dst);
		//graph.insert(make_pair(src, (vector< Node<val_t> >()).push_back(dst)));
		graph.insert(make_pair(src, v));		 
	}
	else
	{
		//src exists
		(*it_src).second.insert(dst);
	}
	
	auto it_dst = graph.find(dst);
	
	if(it_dst == graph.end())
	{
		//dst doesnt exist
		//must be inserted into adjacency list
		
		set< Node<val_t> > v;
		v.insert(src);
		graph.insert(make_pair(dst, v));
		
		//graph.insert(make_pair(dst, (vector< Node<val_t> >()).push_back(src)));
	}	
	else
	{
		//dst exists
		(*it_dst).second.insert(src);
	}	
}

template<typename val_t>
void Graph<val_t> :: disp()
{
	for(auto p: graph)
	{
		cout<<p.first.data<<" ";
		for(auto e: p.second)
			cout<<"->"<<e.data;
		cout<<"\n";
	}
}

int main()
{


	Student s1("s1","CS01");
	Student s2("s2","CS02");
	Student s3("s3","CS03");
	Student s4("s4","CS04");
	Student s5("s5","CS05");
	
	Node<Student> n1(s1);
	Node<Student> n2(s2);
	Node<Student> n3(s3);
	Node<Student> n4(s4);
	Node<Student> n5(s5);
	
	Graph<Student> g(5,7);
	g.add_edge(n1,n2);
	g.add_edge(n1,n3);
	g.add_edge(n1,n4);
	g.add_edge(n2,n3);
	g.add_edge(n4,n3);
	g.add_edge(n4,n5);
	g.add_edge(n5,n3);	
	g.disp();	
	g.dfs(n3);
}

/* Test Case

1 ------- 2
|\		  | 
| \       |
|  \      |
|   \     |
|    \    |
|     \   |
|      \  |
|       \ |
|        \| 
4 --------3
 \        /
  \      /
   \    / 
     5
*/
