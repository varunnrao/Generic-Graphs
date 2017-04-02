#include<iostream>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

template <typename T>
class Graph
{
	public:
	int size = 0;
	Graph(set<T> vertices, set<pair<T,T>> edges)	//Very heavy constructor; need to optimize the algo which builds the adjList
	{						//vertices argument isn't even used here
							

		for(auto e : edges)
		{
			auto f = vertexNodes.find(Node(-1,e.first));
			auto s = vertexNodes.find(Node(-1,e.second));
			
			if(f == vertexNodes.end() || s == vertexNodes.end())
			{

				if(f == vertexNodes.end())
				{
					
					vertexNodes.insert(Node(++size, e.first));
					f = vertexNodes.find(Node(-1,e.first));
					
				}
				
				if(s == vertexNodes.end())
				{
					vertexNodes.insert(Node(++size, e.second));
					s = vertexNodes.find(Node(-1,e.second));
					
				}
				
			}
			
			adjList[*f].push_back(*s);
			adjList[*s].push_back(*f);				
		}
		
		vertexNodes.insert(Node(-1,T()));		// creating a dummy node which would be our 'one past the end'
		adjList[Node(-1,T())];			// it is essentially a unconnected vertex of the graph
				
		for(auto& pair : adjList)		// sorting the vector so that neighbors are reached in order of their indices
		{
			sort(pair.second.begin(),pair.second.end(),[](auto one, auto two){return one.index < two.index;});
		}
		
		
/********************************************************TEST*************************************************		
		auto it = adjList.begin();
		while(it != adjList.end())
		{
			cout << it->first.index << " : \t";
			for_each(it->second.begin(),it->second.end(),[](auto elem){cout << elem.index << "\t";});
			cout << "\n";
			++it;
		}	
*************************************************************************************************************/		
	}
	
	
	template<typename PT>		// not sure if this must be a template class or not
	class Iterator
	{

		public:
		Iterator(auto nodeIterator, Graph* g): graph(g)
		{
			s.push(nodeIterator);
			visited.insert(nodeIterator->index);
			curr = nodeIterator;
		}
		T operator*()
		{
			return this->curr->value;
		}
		
		bool operator!=(auto rhs)
		{
			return (
				curr->index != rhs.curr->index  	//need to modify this;
			);						// must check stack, visited set inequality as well
		}
		
		Iterator& operator++()
		{
			
			//till stack becomes empty or a push happens: 
				// look at top
				// if there is no unvisited neighbor, pop
				// else, get the first unvisited neighbor and push. Set that as curr
			bool isPushed = false;
			while(!s.empty() && !isPushed)
			{
				auto nodeAtTop = s.top();
				auto neighborLast = (graph->adjList[*nodeAtTop]).end();
				auto neighborFirst = (graph->adjList[*nodeAtTop]).begin();				
				
				while(neighborFirst != neighborLast )
				{
					if(visited.find(neighborFirst->index) == visited.end()) // found an unvisited neighbor
					{
						visited.insert(neighborFirst->index);
						curr = graph->vertexNodes.find(*neighborFirst);
						s.push(curr);
						isPushed = true;
						break;
					}
					++neighborFirst;
				}
				if(!isPushed)
				{
					s.pop();
				}
								
			}
			
			if(!isPushed && s.empty())		// finished DFS traversal, make iterator equal to graph.end()
				curr = graph->vertexNodes.find(Node(-1,T()));	

			return *this;
		}
		
		private:
		Graph* graph;	// dirty hack to enable access to outer class attributes; unsure of alternatives
		PT curr;	// this is a set::iterator to the Node where the Graph::iterator is currently at.
		stack<PT> s;
		set<int> visited; 
		
		
	};
	
	
	/*
	* Node with index 1 is the begin iterator
	* Node with index -1 is the end iterator
	* This end Node is created in the graph constructor
	* I think it is possible to make the Iterator class non-generic; would make begin() and end() look prettier
	*/
	auto begin()
	{
		return Iterator<_Rb_tree_const_iterator<Graph<T>::Node>>(find_if(vertexNodes.begin(), vertexNodes.end(), [](auto elem){return elem.index == 1;}),this);
	}
	
	auto end()
	{
		return Iterator<_Rb_tree_const_iterator<Graph<T>::Node>>(find_if(vertexNodes.begin(), vertexNodes.end(), [](auto elem){return elem.index == -1;}),this);
	}

	private:
	class Node
	{
		public:
		int index;
		T value;
		
		Node(int i, T val): index(i), value(val)
		{
		}
		
		friend bool operator<(Node lhs, Node rhs) 
		{
			return lhs.value < rhs.value;
		}
		
		
	};
	
	set<Node> vertexNodes;	// is a set of all vertices, encapsulated inside a Node object (along with index)
	map<Node,vector<Node>> adjList;

	
	
};


class Rect
{
	public:
	 Rect(): length(0), breadth(0)
	 {
	 }
	 Rect(int l, int b) : length(l),breadth(b)
	 {
	 }

	 friend bool operator<(Rect lhs, Rect rhs) 
	 {
	 	return lhs.length < rhs.length;
	 }
	 
	 
	friend ostream& operator<<(ostream& os, const Rect& r)
	{
	    os << r.length<<" : "<< r.breadth << "\n";
    // write obj to stream
	    return os;
	}
	private:
	 int length;
	 int breadth;
};


int main()
{



//	Couldn't make it work for user defined types

	Rect a(3,5);
	Rect b(4,5);
	Rect c(1,2);
	Rect d(7,3);
	Rect e(9,10);

	set<Rect> vertices {a,b,c,d,e};
	set<pair<Rect,Rect>> edges {
		make_pair(a,b),
		make_pair(a,e),
		make_pair(b,e),
		make_pair(c,e),
		make_pair(d,e)
	};

	Graph<Rect> myg(vertices, edges);


/*	char a = 'a';
	char b = 'b';	
	char c = 'c';
	char d = 'd';
	char e = 'e';


	set<char> vertices {a,b,c,d,e};
	set<pair<char,char>> edges {
		make_pair(a,b),
		make_pair(a,e),
		make_pair(b,e),
		make_pair(c,e),
		make_pair(d,e)
	
	};
	Graph<char> myg(vertices, edges);
*/	
	auto it = myg.begin();
	while(it != myg.end())
	{
		cout << *it << "\t"; 	//should display in DFS
		++it;
	}
	
}


/******************************
TEST CASE : 
		a------b
		 \    /
		  \  /
 		   e
		  / \ 
		 c   d

*******************************/
