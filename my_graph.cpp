#include<map>
#include<set>
#include<iostream>
#include<iterator>
#include<algorithm>
#include<vector>
#include<string>
#include<stack>
#include<deque>
#include<typeinfo>
#include<numeric>

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
	Student()
		:name("")
		,usn("")
	{}
	
	string& get_name()
	{
		return this->name;
	}
	/*
	bool operator<(const Student& rhs) const
	{
		return usn < rhs.usn;
	}*/
	
	bool operator==(const Student& rhs)
	{	
		return this->name == rhs.name;
	}
	
	Student operator+(const Student& rhs) const
	{
		Student t;	
		if((this->name).empty())
		{
			t.name = rhs.name;
			t.usn = rhs.usn;
		}
		else
		{
			t.name =  this->name + ";" + rhs.name;		
			t.usn =  this->usn +";" + rhs.usn;
		}				
		return t;
		
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
	Node()
	{}
	Node(val_t data)
		: data(data)
		, index(count++)
	{}
	bool operator< (const Node<val_t>& rhs) const
	{
		return index < rhs.index;
	}	
	
	
};


template<typename val_t>
int Node<val_t>::count = -1;//Need to do this hack for incr of non-const static variables of a class


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
	{
		//this is a sentinel node to mark the logical one 
		//past end in the graph traversal
		val_t v;		
		Node<val_t> end(v);
		set<Node<val_t>> empty;
		graph.insert(make_pair(end,empty));
	}
	
	//method to add edges to the graph
	void add_edge(Node<val_t> src, Node<val_t> dst);
	
	//method to display contents of the graph
	void disp();
	
	//DFS graph traversal
	void dfs(Node<val_t> start_node);
	
	
	//class Iterator : public iterator<forward_iterator_tag, typename map< Node<val_t>, set< Node<val_t> >>::const_iterator>
	class Iterator
	{
		private:
		
		Graph* g;
		typedef typename map< Node<val_t>, set< Node<val_t> >>::const_iterator map_it;
		map_it current_ptr;
		vector<bool> visited {vector<bool>(g->num_vertices,false)};
		deque<Node<val_t>> s;//using a deque as a stack
		
		public:
		
		typedef std::ptrdiff_t difference_type;
		typedef Node<val_t>  value_type;
		typedef Node<val_t>* pointer;
		typedef Node<val_t>& reference;
		typedef size_t size_type;
		typedef std::forward_iterator_tag iterator_category;
		
		Iterator(map_it it, Graph* g):g(g)
		{
			s.push_front((*it).first);
			if((*it).first.index==0)
				visited[(*it).first.index]=true;
			current_ptr = it;
		}
		
		val_t operator*() 
		{
			
			return (*current_ptr).first.data;
		}
		
		bool operator!=(const Iterator& rhs) const
		{
			return !(*this == rhs);
		}
		
		bool operator==(const Iterator& rhs) const
		{
			return current_ptr == rhs.current_ptr;
		}
		Iterator& operator++()
		{
			
			
			while(!s.empty() )
			{
				Node<val_t> t=s.back();
				s.pop_back();	
						
				for(auto i = (g->graph[t]).begin();i!=(g->graph[t]).end();i++)
				{
					if(!visited[(*i).index])
						s.push_front(*i);
				}
				
				if(!visited[t.index])
				{
					current_ptr = g->graph.find(t);					
					visited[t.index]=true;
					break;
				}
				
			}
			if(s.empty())
			{
				val_t s;
				Node<val_t> test;
				test.index=-1;
				current_ptr = g->graph.find(test);
			}
			
			return *this;
		}	
			
	};
	Iterator begin() 
	{
		val_t s;
		Node<val_t> test;
		test.index=0;			
		return Iterator(graph.find(test),this);
	}
	Iterator end() 
	{
		val_t s;
		Node<val_t> test;
		test.index=-1;
		return Iterator(graph.find(test),this);
	}
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
		if(p.first.index != -1)
		{
			cout<<p.first.data<<" ";
			for(auto e: p.second)
				cout<<"->"<<e.data;
			cout<<"\n";
		}
	}
}

template<typename ptr_t>
void display(ptr_t first, ptr_t last)
{
	while(first!=last)
	{
		
		cout<<(*first)<<'\n';		
		++first;
		
	}
}

int main()
{


	Student s1("s1","CS01");
	Student s2("s2","CS02");
	Student s3("s3","CS03");
	Student s4("s4","CS04");
	Student s5("s5","CS05");
	/*
	
	//TEST CASE #1
	
	Graph<Student> g(5,7);//must be called before node object is created for data to be initialised with -1
	
	Node<Student> n1(s1);
	Node<Student> n2(s2);
	Node<Student> n3(s3);
	Node<Student> n4(s4);
	Node<Student> n5(s5);
	
	
	g.add_edge(n1,n2);
	g.add_edge(n1,n3);
	g.add_edge(n1,n4);
	g.add_edge(n2,n3);
	g.add_edge(n4,n3);
	g.add_edge(n4,n5);
	g.add_edge(n5,n3);	
	//g.disp();	
	//g.dfs(n1);
	
	display(g.begin(),g.end());
	*/
	
	
	//TEST CASE #2
	
	Graph<Student> g(5,5);
	
	Node<Student> n1(s1);
	Node<Student> n2(s2);
	Node<Student> n3(s3);
	Node<Student> n4(s4);
	Node<Student> n5(s5);
	
	g.add_edge(n1,n2);
	g.add_edge(n1,n5);
	g.add_edge(n2,n5);
	g.add_edge(n5,n3);
	g.add_edge(n5,n4);
	
	display(g.begin(),g.end());
	
	
	{auto it=find(g.begin(),g.end(),s2);
	if(it == g.end())
	{
		cout<<"not found"<<std::endl;
	}
	else
	{
		std::cout<<"found"<<std::endl;
	}
	}
	auto it=find_if(g.begin(),g.end(),[](auto e){return e.get_name() == "s2";});
	if(it == g.end())
	{
		cout<<"not found"<<std::endl;
	}
	else
	{
		std::cout<<"found"<<std::endl;
	}
	cout<<"Copy Method\n====================\n";
	vector<Student> v(5);
	copy(g.begin(),g.end(),v.begin());
	for(auto e: v)
		cout<<e<<"\n";
	Student t;
	cout<<accumulate(g.begin(),g.end(),t)<<"\n";
		
	v.clear();
	/*
	cout<<"Copy If method\n==================\n";  NOT WORKING!!
	copy_if(g.begin(),g.end(),v.begin(),[](auto e){return e.get_name()>"s2";});
	for(auto e: v)
		cout<<e<<"\n";
		
	//cout<<("s2">"s1")<<"\n";*/
	
	
}

/* Test Case #1

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


/* Test Case #2

1------2
 \    /
  \  /
   5
  / \ 
 3   4
 
*/
