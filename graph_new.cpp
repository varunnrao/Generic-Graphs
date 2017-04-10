#include<bits/stdc++.h>

template<typename T>
class Graph
{


	private:
		class Node
		{

			public:
				mutable int index_;
				T value_;
				constexpr explicit Node(int index = -1,T value = T())
				:index_(index),value_(value)
				{}
		
				friend bool operator<(const Node& lhs, const Node& rhs)
				{
					return lhs.value_ < rhs.value_;
				}
				
				friend std::ostream& operator<<(std::ostream& o ,const Node& x)
				{
					 //return o<< x.index_ <<"->"<<x.value_;
					 return o<<x.value_;
					
				}
	
		};
			
		std::set<Node> vertex_nodes_;
		std::map<Node,std::set<Node>> adj_list_;
		int size_;

	public:
		constexpr Graph()
		:size_(0)
		{
			vertex_nodes_.insert(Node());
			adj_list_[Node()];			
		}
		
		constexpr Graph(const std::set<T> vertices,const std::set<std::pair<T,T>> edges)
		:size_(0)
		{
		
			for(const auto& v : vertices)
			{
				vertex_nodes_.insert(Node(++size_, v));
				
			}
		
			for(const auto& e : edges)
			{
				auto f = vertex_nodes_.find(Node(-1,e.first));
				auto s = vertex_nodes_.find(Node(-1,e.second));
				adj_list_[*f].insert(*s);
				adj_list_[*s].insert(*f);			
			}

			vertex_nodes_.insert(Node());
			adj_list_[Node()];
				
		}
		
		#if 0 
		//meaningless ->cannot fill the graph with n empty vertices since all will be eliminated because their value fields
		are null and are same .
		constexpr Graph(const int vertices)
		:size_(vertices)
		{	
			vertex_nodes_.insert(Node());
			adj_list_[Node()];
		}
		#endif 
		
		template<typename U>
		constexpr Graph(const std::initializer_list<U> vertex_list)
		:size_(0)
		{
			std::set<U> s(vertex_list);
			std::for_each(s.begin(),s.end(),[&](const auto& e)
							{
								auto node = Node(++size_,e);
								vertex_nodes_.insert(node);
								adj_list_[node];
							});
			vertex_nodes_.insert(Node());
			adj_list_[Node()];
			
		}
		
		template<typename ptr_t>
		constexpr Graph(ptr_t begin,ptr_t end)
		:size_(0)
		{
			std::set<typename std::iterator_traits<ptr_t>::value_type> s(begin,end);
			std::for_each(s.begin(),s.end(),[&](const auto& e)
							{
								auto node = Node(++size_,e);
								vertex_nodes_.insert(node);
								adj_list_[node];
							});
			vertex_nodes_.insert(Node());
			adj_list_[Node()];
			
		}
		
		constexpr Graph(const Graph& rhs)
		{
			vertex_nodes_=std::set<Node>(rhs.vertex_nodes_.begin(),rhs.vertex_nodes_.end());
			adj_list_=std::map<Node,std::set<Node>>(rhs.adj_list_.begin(),rhs.adj_list_.end());
			size_=rhs.size_;
		}
		constexpr Graph(Graph&& rhs)
		{
			vertex_nodes_=std::set<Node>(rhs.vertex_nodes_.begin(),rhs.vertex_nodes_.end());
			adj_list_=std::map<Node,std::set<Node>>(rhs.adj_list_.begin(),rhs.adj_list_.end());
			size_=rhs.size_;
			rhs.size_=0;
			rhs.adj_list_=std::map<Node,std::set<Node>>();
			rhs.adj_list_[Node()];
			rhs.vertex_nodes_=std::set<Node>();
			rhs.vertex_nodes_.insert(Node());
					
		}
		constexpr Graph& operator=(const Graph& rhs)
		{
			if(this != &rhs)
			{
				vertex_nodes_=std::set<Node>(rhs.vertex_nodes_.begin(),rhs.vertex_nodes_.end());
				adj_list_=std::map<Node,std::set<Node>>(rhs.adj_list_.begin(),rhs.adj_list_.end());
				size_=rhs.size_;
			}
			
			return *this;
		}
		
		constexpr Graph& operator=(Graph&& rhs)
		{
			if(this != &rhs)
			{
				vertex_nodes_=std::set<Node>(rhs.vertex_nodes_.begin(),rhs.vertex_nodes_.end());
				adj_list_=std::map<Node,std::set<Node>>(rhs.adj_list_.begin(),rhs.adj_list_.end());
				size_=rhs.size_;
				rhs.size_=0;
				rhs.adj_list_=std::map<Node,std::set<Node>>();
				rhs.adj_list_[Node()];
				rhs.vertex_nodes_=std::set<Node>();
				rhs.vertex_nodes_.insert(Node());	
			}
		}
		
		class dfs_iterator  //: public std::iterator<std::forward_iterator_tag, T>
		{
		
			
			typedef std::_Rb_tree_const_iterator<Graph<T>::Node> itr_t;
			
			
			public:
			
				typedef std::ptrdiff_t difference_type;
				typedef T value_type;
				typedef T* pointer;
				typedef T& reference;
				typedef size_t size ;
				typedef std::forward_iterator_tag iterator_category;
				
				
				constexpr dfs_iterator(itr_t node_iterator, Graph<T>* graph)
				:graph_(graph)
				{
					stack_.push(node_iterator);
					visited_.insert(node_iterator->index_);
					current_ = node_iterator;
				}
				
				constexpr dfs_iterator() =delete;// need to implement but how?
				
				 
				constexpr dfs_iterator(const dfs_iterator& itr)
				{
					//created an alias
					this->stack_ = itr.stack_;
					this->graph_ = itr.graph_;
					this->visited_ = itr.visited_;
					this->current_ = itr.current_;
					
				}
				T operator*()
				{
					return this->current_->value_;
				}

				bool operator==(const dfs_iterator& rhs)
				{
					return  current_->index_ == rhs.current_->index_ ;						
				}
		
				bool operator!=(const dfs_iterator& rhs)
				{
					return current_->index_ != rhs.current_->index_ ;						
				}
		
				dfs_iterator& operator++()
				{
			
					bool is_pushed = false;
					while(!stack_.empty() && !is_pushed)
					{
						auto node_at_top = stack_.top();
						auto neighbor_last = (graph_->adj_list_[*node_at_top]).end();
						auto neighbor_first = (graph_->adj_list_[*node_at_top]).begin();				
				
						while(neighbor_first != neighbor_last )
						{
							if(visited_.find(neighbor_first->index_) == visited_.end()) 
							{
								visited_.insert(neighbor_first->index_);
								current_ = graph_->vertex_nodes_.find(*neighbor_first);
								stack_.push(current_);
								is_pushed = true;
								break;
							}
							++neighbor_first;
						}
						if(!is_pushed)
						{
							stack_.pop();
						}
								
					}
			
					if(!is_pushed && stack_.empty())		
						current_ = graph_->vertex_nodes_.find(Node());	

					return *this;
				}
				
				dfs_iterator operator++(int)
				{
					dfs_iterator old(*this);
					++*this;
					return old;
				}
		
			private:
				Graph<T>* graph_;	
				itr_t current_;	
				std::stack<itr_t> stack_;
				std::set<int> visited_; 
		
		
		};
		
		
		dfs_iterator begin()
		{
			return dfs_iterator(std::find_if(vertex_nodes_.begin(), vertex_nodes_.end(), [](auto e){return e.index_ == 1;}),this);
		}
	
		dfs_iterator end()
		{
			return dfs_iterator(std::find_if(vertex_nodes_.begin(), vertex_nodes_.end(), [](auto e){return e.index_ == -1;}),this);
		}
		
		
		
		Graph& display() 
		{
			for(auto p:adj_list_)
			{
				if(p.first.index_ != -1)
				{
					std::cout<< p.first << "::";
					for(auto c: p.second) std::cout<< c << "," ;
					std::cout<<std::endl;
				}
				
			}
			return *this;
		}
		
		
		inline Graph& add_edge(const T& v1,const T& v2)
		{		
			return add_edge(std::pair<T,T>(v1,v2));	
		}
		
		inline Graph& add_edge(const std::pair<T,T>& e)
		{
			auto f = vertex_nodes_.find(Node(-1,e.first));
			auto s = vertex_nodes_.find(Node(-1,e.second));
			if(f != vertex_nodes_.end() && s!=vertex_nodes_.end())
			{
				adj_list_[*f].insert(std::move(*s));
				adj_list_[*s].insert(std::move(*f));
			}	
			return *this;		
		}
		
		Graph& remove_edge(const T& v1,const T& v2)
		{
			return remove_edge(std::pair<T,T>(v1,v2));
		}
		Graph& remove_edge(const std::pair<T,T>& e)
		{
			auto f = vertex_nodes_.find(Node(-1,e.first));
			auto s = vertex_nodes_.find(Node(-1,e.second));
			if(f != vertex_nodes_.end() && s!=vertex_nodes_.end())
			{
				adj_list_[*f].erase(std::move(*s));
				adj_list_[*s].erase(std::move(*f));
			}
			return *this;
		}
		
		inline Graph& add_vertex(const T& v)
		{
			auto it = vertex_nodes_.find(Node(-1,v));
			if(it == vertex_nodes_.end())
			{
				vertex_nodes_.insert(Node(++size_, v));
				adj_list_[Node(size_, v)];
			}
			return *this;
			
		}
		
		Graph& remove_vertex(T& v)
		{
			auto to_remove = vertex_nodes_.find(Node(-1,v));
			if(to_remove !=vertex_nodes_.end())
			{
				//for vertex_nodes
				for(auto& every_vertex: vertex_nodes_)
				{
					if(every_vertex.index_ != -1 && (every_vertex.index_ > to_remove->index_))
						--(every_vertex.index_);
				}
				
				vertex_nodes_.erase(*to_remove);
				

				//for adj_list_
				for(auto& every_adj_pair: adj_list_)
				{
					//remove from every adj list except the node's itself and the one with index=-1
					if(every_adj_pair.first.index_!= -1 && (every_adj_pair.first.value_ != to_remove->value_ ))
						adj_list_[every_adj_pair.first].erase(std::move(*to_remove));
					
					//decrement the index value for those nodes whose index is greater than that of the to be removed node
					if(every_adj_pair.first.index_ != -1 && (every_adj_pair.first.index_ > to_remove->index_ ))
					{
						--(every_adj_pair.first.index_);
					}
						
					for(auto& every_list_ele: every_adj_pair.second)
					{
						if(every_list_ele.index_ != -1 && (every_list_ele.index_ > to_remove->index_))
							--(every_list_ele.index_);
					}
				}
			
				adj_list_.erase(std::move(*to_remove));
				--size_;
			}
			return *this;
			
		}
		
		int vsize()
		{
			return size_;
		}
		
		//make sure that isolated nodes are also visited in some order;

	
};


/*test class*/

class Rect
{
	public:
	 Rect()
	 :length(0), breadth(0)
	 {
	 }
	 Rect(int l, int b) 
	 :length(l),breadth(b)
	 {
	 }

	 friend bool operator<(Rect lhs, Rect rhs) 
	 {
	 	return lhs.length < rhs.length;
	 }

	 friend bool operator!=(Rect lhs, Rect rhs) 
	 {
	 	return !(lhs == rhs);
	 }
	 
	 friend bool operator==(Rect lhs, Rect rhs) 
	 {
	 	return (lhs.length == rhs.length) && (lhs.breadth == rhs.breadth);
	 }
	 
	friend std::ostream& operator<<(std::ostream& os, const Rect& r)
	{
	    os << r.length<<" : "<< r.breadth << "  ";
	    return os;
	}
	
	 int length;
	 int breadth;
};


int main()
{
	#if 1
	char a = 'a';
	char b = 'b';	
	char c = 'c';
	char d = 'd';
	char e = 'e';
	char f = 'f';


	std::set<char> vertices {a,b,c,d,e,f};
	
	std::set<std::pair<char,char>> edges 
	{
		std::make_pair(a,b),
		std::make_pair(a,e),
		std::make_pair(b,e),
		std::make_pair(c,e),
		std::make_pair(d,e),
		std::make_pair(f,c)
	
	};
	//Graph<char> myg(vertices, edges);
	//std::vector<char> v{a,b,c,d,e,f};
	//Graph<char> myg(v.begin(),v.end());
	//Graph<char> myg({a,b,c,d,e,f});
	#endif
	
	Graph<char> myg;
	myg.add_vertex(a);
	myg.add_vertex(b);
	myg.add_vertex(c);
	myg.add_vertex(d);
	myg.add_vertex(e);
	myg.add_vertex(f);
	myg.add_edge(a,b);
	myg.add_edge(a,e);
	myg.add_edge(e,c);
	myg.add_edge(e,d);
	myg.add_edge(b,e);
	myg.add_edge(c,f);
	//myg.remove_vertex(b);
	std::cout << "no of vertices :" << myg.vsize() << "\n";
	myg.display();
	
	
	std::cout<<"copy ctor"<<std::endl;
	Graph<char> new_g=myg;
	std::cout << "no of vertices :" << new_g.vsize() << "\n";
	new_g.display();
	
	
	std::cout<<"move ctor"<<std::endl;
	Graph<char> move_g(std::move(new_g));
	std::cout << "no of vertices :" << move_g.vsize() << "\n";
	move_g.display();
	
	std::cout << "no of vertices :" << new_g.vsize() << "\n";
	new_g.display();
	
	std::cout<<"copy assignment"<<std::endl;
	new_g=move_g;
	std::cout << "no of vertices :" << new_g.vsize() << "\n";
	new_g.display();

	
	Graph<char> move_new_g;
	std::cout<<"move assignment"<<std::endl;
	move_new_g=std::move(new_g);
		std::cout << "no of vertices :" << new_g.vsize() << "\n";
	new_g.display();
	
		std::cout << "no of vertices :" << move_new_g.vsize() << "\n";
	move_new_g.display();
	
	
	
	//myg.remove_edge(a,b);
	//myg.remove_edge(std::pair<char,char>(b,c));
	//myg.add_edge(d,b);
	//myg.remove_vertex(b);
	//myg.remove_vertex
	//myg.display();
	
	#if 0
	Rect a(3,5);
	Rect b(4,5);
	Rect c(1,2);
	Rect d(7,3);
	Rect e(9,10);

	std::set<Rect> vertices {a,b,c,d,e};
	std::set<std::pair<Rect,Rect>> edges {
		std::make_pair(a,b),
		std::make_pair(a,e),
		std::make_pair(b,e),
		std::make_pair(c,e),
		std::make_pair(d,e)
	};

	Graph<char> myg;
	
	#endif
	
	#if 1
	//myg.display();

	
	auto it = myg.begin();
	//std::cout << typeid(it).name() << '\n';
	while(it != myg.end())
	{
		std::cout << *it << "\n";
		++it;
		//it++;
	}
	#endif
	#if 0
	#endif 
	
	std::vector<char> v{move_new_g.begin(),move_new_g.end()};
	std::copy(v.begin(),v.end(),std::ostream_iterator<char>(std::cout,","));
}

/*
		a------b
		 \    /
		  \  /
 		   e
		  / \ 
		 c   d
		/
	       / 
	      f
*/
