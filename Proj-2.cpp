
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include <math.h>
# define INF 0x3f3f3f3f

using namespace std;
//defines edge
struct Edge
{
	string vertex1;
	string vertex2;
	int wei;
	Edge() {}
	Edge(string v1, string v2, int w):vertex1(v1), vertex2(v2),wei(w){
	}
};

class Graph
{
public:
	//lots of lists that belong to the functions of a graph
	unordered_map<string, vector<pair<string, int>>>adj;
	map<string, bool> visit;
	map<string, bool> visited;
	vector<Edge>edge;
	unordered_map<string, int>rank;
	unordered_map<string, string>parent;
	unordered_map<string,Edge>adjl;

	void addEdge(string w, vector<pair<string, int>> a);
	void DFS(string v);
	void BFS(string v);
	void PrimsMST();
	string find(string vertex);
	void Union(string r1, string r2);
	void makeset(string v);
	void kruskalMST();
	void Printgraph();

};

void Graph::addEdge(string w, vector<pair<string, int>> a) {
	adj[w] = a;
	for (int x = 0; x < a.size(); x++) {
		Edge e(w,a[x].first,a[x].second);
		edge.push_back(e);
		adjl[w] = e;

	}

}
void Graph::DFS(string v) {
	//implements DFS
	visit[v] = true;
	cout << v << " "<<endl;
	for (int w = 0; w < adj[v].size(); w++) {
		string r = adj[v][w].first;
		if (visit[r] == false)
			DFS(r);
	}
}
void Graph::BFS(string v) {
	//implements BFS
	list<string> que;
	visited[v] = true;
	que.push_back(v);

	while (!que.empty()) {
		v = que.front();
		cout << v << " "<<endl;
		que.pop_front();
		for (int w = 0; w < adj[v].size(); w++) {
			string r = adj[v][w].first;
			if (visited[r] == false) {
				visited[r] = true;
				que.push_back(r);
			}
		}
	}
}

void Graph::PrimsMST() {
	map<string, vector<Edge>> NMST;
	//actual mst
	map<string, bool> MST;
	//marks as visited
	string s = "Anderson County";
	//start
	map<string, int>N;
	//edge & weights
	for (auto a:adj) {
		N[a.first] = INF;
		//initalizes all vertexes as infinity
	}
	N[s] = 0;

	priority_queue< pair<int, string>, vector <pair<int, string>>,
		greater<pair<int, string>> > pq;
	//queue of sorted edges
	pq.push(make_pair(0,s));
	while (!pq.empty())
	{

		string u = pq.top().second;
		pq.pop();

		if (MST[u] == true) {
			continue;
		}

		MST[u] = true; 
		//marked true

		
		for (int i=0; i< adj[u].size();++i)
		{
			
			string v = adj[u][i].first;
			int weight = adj[u][i].second;


			if (MST[v] == false && N[v] > weight)
				//checks of not visited and min weight
			{
				N[v] = weight;
				pq.push(make_pair(N[v], v));
				//adds to priority queue
				Edge qw(u, v, weight);
				NMST[u].push_back(qw);
				//added to mst
			}
		}

	}
	cout << "writing Prims Mst" << endl;
	//print mst to file
	ofstream oFile("PrimMST.csv");
	oFile << "county" << "," << "ajacencylist" << endl;
	for (auto e : NMST) {
		oFile << e.first << ",";
		for (auto w : e.second) {
			oFile << "(" << w.vertex2 << "-" << w.wei << ")" << ";";
		}
		oFile << endl;
	}
	cout << "file written!!"<<endl;
}

	

string Graph::find(string vertex) {
	if (parent[vertex] == vertex) {
		return parent[vertex];
	}
	else {
		return find(parent[vertex]);
	}
}
void Graph::Union(string r1, string r2) {//union of 2 strings
	if (rank[r1] > rank[r2]) {
		parent[r2] = r1;
	}
	else if (rank[r2] > rank[r1]) {
		parent[r2] = r1;
	}
	else {
		parent[r1] = r2;
		rank[r2]++;
	}
}  
void Graph::makeset(string v) {
	parent[v] = v;
	rank[v] = 0;
}

void Graph::kruskalMST() {
	map<string,vector<Edge>>A;
	for (auto i:adj) {//sets all parent nodes to themselves and thier weight is set to 0
		makeset(i.first);
	}
	sort(edge.begin(),edge.end(),[]
	(Edge x, Edge y){return x.wei < y.wei; });//sorts all edges
	for (Edge e : edge) {
		string r1=find(e.vertex1);
		string r2=find(e.vertex2);
		if (r1!=r2) {
			A[e.vertex1].push_back(e);//adds to MST
			Union(r1,r2);
		}
	}
	//writes file
	cout << "writing kruskals MST...." << endl;
	ofstream outFile("KruskalMST.csv");
	outFile << "county" <<","<< "ajacencylist" << endl;
	for (auto e:A) {
		outFile << e.first<<",";
		for (auto w : e.second) {
			outFile <<"("<< w.vertex2<<"-"<<w.wei<<")"<<";";
		}
		outFile << endl;
}
	cout << "file written!!"<<endl;
}
void Graph::Printgraph() {
	for (auto const& pair : adj) {
		cout << pair.first<<endl;
		for (int i = 0; i < pair.second.size(); i++) {
			cout << pair.second[i].first << pair.second[i].second << endl;
		}
	}
}
void readitems(Graph& data) {
	//parses file in the worst way possibe
	cout << "Reading file...." << endl;
	ifstream inputFile;
	inputFile.open("texas_counties_adjacency.csv");
	string str;
	getline(inputFile, str);
	while (inputFile)
	{
		string s;
		if (!getline(inputFile, s)) break;

		istringstream ss(s);
		vector<string>r;
		while (ss)
		{
			string s;
			if (!getline(ss, s, ',')) break;
			r.push_back(s);

		}
		for (int i = 0; i < r[1].size(); i++) {
			if (r[1][i] == ')' || r[1][i] == '(') {
				r[1].erase(i, 1);
			}
		}
		vector<string> result;
		stringstream s_stream(r[1]);
		while (s_stream.good()) {
			string substr;
			getline(s_stream, substr, ';');
			result.push_back(substr);
		}
		vector<pair<string, int>>PV;
		for (int i = 0; i < result.size(); i++) {
			pair<string, int>f;
			stringstream s_stream(result[i]);
			vector<string> tmp;
			while (s_stream.good()) {
				string substr;
				getline(s_stream, substr, '-');
				tmp.push_back(substr);
			}
			if (tmp[0][0]==' ') {
				tmp[0].erase(0, 1);
			}
			if (!tmp[0].empty()) {
				tmp[0].pop_back();
			}

			f.first = tmp[0];
			stringstream gv(tmp[1]);
			int x;
			gv >> x;
			f.second=x;
			PV.push_back(f);
		}

		data.addEdge(r[0],PV);
	}

	if (!inputFile.eof())
	{
		cerr << "end!\n";
	}

}



int main()
{
	Graph g;
	readitems(g);
	cout << "Select an option number from the following menu:\r\n" <<
		"Option  Action\r\n" <<
		"------  -----------------------------------------\r\n" <<
		"   1    DFS Texas Counties.\r\n" <<
		"   2    BFS Texas Counties.\r\n" <<
		"   3    Write Prims Ajacency List.\r\n" <<
		"   4    Write Kruskals Ajacency List.\r\n" <<
		"   5    Quit.\r\n" <<
		"Enter your choice number 1-5:" <<
		"" << endl;
	bool loop = true;
	int x;
	while (loop) {
		while (true) {
			cin >> x;
			if (!cin)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "invalid imput!" << endl;

			}

			else {
				break;
			}
		}

		switch (x)
		{
		case 1:
			cout << "DFS result:" << endl;
			g.DFS("Anderson County");
			cout << "Select an option number from the following menu:\r\n" <<
				"Option  Action\r\n" <<
				"------  -----------------------------------------\r\n" <<
				"   1    DFS Texas Counties.\r\n" <<
				"   2    BFS Texas Counties.\r\n" <<
				"   3    Write Prims Ajacency List.\r\n" <<
				"   4    Write Kruskals Ajacency List.\r\n" <<
				"   5    Quit.\r\n" <<
				"Enter your choice number 1-5:" <<
				"" << endl;
			break;
		case 2:
			cout << "BFS result:" << endl;
			g.BFS("Anderson County");
			cout << "Select an option number from the following menu:\r\n" <<
				"Option  Action\r\n" <<
				"------  -----------------------------------------\r\n" <<
				"   1    DFS Texas Counties.\r\n" <<
				"   2    BFS Texas Counties.\r\n" <<
				"   3    Write Prims Ajacency List.\r\n" <<
				"   4    Write Kruskals Ajacency List.\r\n" <<
				"   5    Quit.\r\n" <<
				"Enter your choice number 1-5:" <<
				"" << endl;
			break;
		case 3:
			g.PrimsMST();
			cout << "Select an option number from the following menu:\r\n" <<
				"Option  Action\r\n" <<
				"------  -----------------------------------------\r\n" <<
				"   1    DFS Texas Counties.\r\n" <<
				"   2    BFS Texas Counties.\r\n" <<
				"   3    Write Prims Ajacency List.\r\n" <<
				"   4    Write Kruskals Ajacency List.\r\n" <<
				"   5    Quit.\r\n" <<
				"Enter your choice number 1-5:" <<
				"" << endl;
			break;
		case 4:
			g.kruskalMST();
			cout << "Select an option number from the following menu:\r\n" <<
				"Option  Action\r\n" <<
				"------  -----------------------------------------\r\n" <<
				"   1    DFS Texas Counties.\r\n" <<
				"   2    BFS Texas Counties.\r\n" <<
				"   3    Write Prims Ajacency List.\r\n" <<
				"   4    Write Kruskals Ajacency List.\r\n" <<
				"   5    Quit.\r\n" <<
				"Enter your choice number 1-5:" <<
				"" << endl;
			break;
		case 5:
			cout << "goodbye..." << endl;
			loop = false;
			break;
		default:
			cout << "Invalid imput" << endl;
			break;
		}

	}
}


