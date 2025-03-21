#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

// Students can add global variables or functions here (e.g., for capacities, flows, BFS, etc.)

const int SIZE = 64;
const int s = 0;
const int t = SIZE - 1;
const int A_CHARCODE = 65;
const int ZERO_CHARCODE = 48;

const int FLIGHT_OFFSET = 1;
const int PILOT_OFFSET = 28;
#define is_char(x) ( 1<= (x) && (x) <= 27)
#define is_pilot(y) ( 1<= (y) && (y) > 27)
std::vector<std::vector<int> > paths; // stores the max-flow paths
class Graph {
	public:
		int size;
		/*
		 * flights are assumed to be all Capital letters, so we map these to numbers 1-27 (offset by 1 to make the source the 0th element)
		 * pilots are offset by 27
		 * so: source = 0th element, flights = 1-27th elements, pilots = 28-nth elements sink = n+1th element
		 * that means that M[u] = a vector containing all the pilots assigned to flight u
		 * */
		std::vector< std::vector<int> >M;
		std::vector<bool> is_pilot_free;
		Graph(int size): size(size), M(size, vector<int>(size, 0)), is_pilot_free(size, true){}

		void add_edge(int u, int v, int c){
			if(u >= size || v >= size)
				std::runtime_error("index out of bounds");
			M[u][v] = c;	
		}
		bool bfs(int s, int t, std::vector<int> &parent){
			std::vector<bool>visited(size, false); 
			std:queue<int>q;
			q.push(s);
			visited[s] = true;

			while( !q.empty() ) {
				int u = q.front();
				q.pop();
				for(int i = 0; i < M[u].size(); i++){
					if(!visited[i] && M[u][i] > 0){
						//TODO: I am getting that P goes to 5, so why does the output show 5 goes to P?
						q.push(i);
						visited[i] = true;
						parent[i] = u;
					}
				}
			}
			return visited[t];
		}
		
		int edmonds_karp(int source, int sink){
			
			std::vector<int>parent(size, -1);
			int max_flow = 0;

			while( Graph::bfs(source, sink, parent) ){
				int path_flow = INT_MAX;
				int s = sink;	
				while(s != source){
					path_flow = min(path_flow, M[ parent[s] ][s]);
					s = parent[s];
				}
				
				max_flow += path_flow;
				int v = sink;
				while(v != source) {

					int u = parent[v];
					//at this point we reverse edges
					M[u][v] -= path_flow;
					M[v][u] += path_flow;
					v = parent[v];
				}
				std::vector<int>path;
				v = sink;
				while(v != source){
					if( is_char(v) ||  is_pilot(v) && is_pilot_free[v] ){
						if( is_pilot(v) )
							is_pilot_free[v] = false;
						path.push_back(v);
						v = parent[v];
					} else { // look for another free pilot
						for(int i = PILOT_OFFSET; i < M[ parent[v] ].size(); i++){
							int p = M[ parent[v] ][i];
							if( is_pilot(p) && is_pilot_free[v] ){
								path.push_back(v);
								v = parent[v];
							}
						}
					}
				}
				path.push_back(s);
				paths.push_back(path);
				
				//At this point we have the current path, and path_flow
			}
			
			return max_flow;
		}
	
};

// Stores flight-pilot data: flightInfo (e.g. "A3"), pilotInfo (e.g. "012")
static vector<pair<string, string> > flightPilotPairings;

// All lines from stdin
static vector<string> inputLines;

/**
 * Solve one day's allocation using Edmonds-Karp for max-flow.
 * TODO (students):
 *   1) Construct your graph and capacities from flightPilotPairings.
 *   2) Implement the Edmonds-Karp algorithm (BFS in the residual graph).
 *   3) Output the 10-character assignment string or "!" if impossible.
 */


void verify_solution() {

	std::vector<char> result(paths.size(), ' ');
	for (int i = 0; i < paths.size(); i++){
		std::vector<int> path = paths[i];
		//if you want to see, how the positions are obtained from paths you can print
		//them uncommenting this code
		for(int k = path.size() - 1; k >= 0; k--) {
			if( 1 <= path[k] && path[k] <= 27) //remove the +1 offset and convert back to char
				cout << (char)((path[k]-1)+A_CHARCODE) << " ";
			else if (path[k] >= 28) //any value greater than 28 is a pilot
				// we add a + 1, to differentiate between sink (0) and pilot (0) which is now 1 with the offset
				cout << (path[k] - PILOT_OFFSET) + 1 << " ";
			else 
				cout << path[k] << " ";
		}
		cout << std::endl;

		//retrieve flight and convert it back to char
		//remove the +1 offset 
		char flight = (char)((path[2]-1)+A_CHARCODE);

		//retrieve pilot and use it as to index our flight in the result vector
		char pilot =  (int)(path[1] - PILOT_OFFSET);
		
		if(pilot >= result.size())
			std::runtime_error("index: pilot out of bounds");
		result[pilot] = flight;	
	}

	for(int i = 0; i < result.size(); i++)
		std::cout << result[i];
	cout << std::endl;
}
void solveAllocationProblem()
{

			
	//Initialize the graph 
	Graph G(SIZE);
	const int c = 1;

	/*
	 * the source goes once to each pilot node
	 * since a flight requires to have n pilots
	 * that means flights must have n incoming edges
	 *
	 * the sink c = 1, represents the number of flights
	 * that successfully gathered the required # of pilots
	 * */

	for(int i = 0; i < flightPilotPairings.size(); i++){
		//offset by 1 to account for source node
		const int flight = (flightPilotPairings[i].first[0] - A_CHARCODE) + FLIGHT_OFFSET;
		const int flight_capacity = flightPilotPairings[i].first[1] - ZERO_CHARCODE;
			
		// add edges from sink to flights 
		G.add_edge(s, flight, flight_capacity);

		for(int j = 0; j < flightPilotPairings[i].second.length(); j++){
			const int pilot = (flightPilotPairings[i].second[j] - ZERO_CHARCODE) + PILOT_OFFSET;
			
			// we send n units of flow from source to flights, 
			// each one has a capacity of 1
			// so flights must find n pilots to satisfy the flow of the incoming edge from source

			// add edges from flight to pilot 
			G.add_edge(flight, pilot, c);
			//add edges from pilots to sink 
			G.add_edge(pilot, t, c);
			

		}
		
	}
	
	int max_flow = G.edmonds_karp(s, t);
	verify_solution();
}

/**
 * Process input data day by day; each day ends when an empty line is encountered.
 */
void processInputData(const vector<string>& lines)
{
    int i = 0;
    while (i < (int)lines.size())
    {
        flightPilotPairings.clear();

        while (i < (int)lines.size() && !lines[i].empty())
        {
            string line = lines[i++];
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (ss >> token) { tokens.push_back(token); }
            if (tokens.size() < 2) continue;

            string flightInfo = tokens[0];
            string pilotInfo  = tokens[1];
            if (!pilotInfo.empty() && pilotInfo.back() == ';') pilotInfo.pop_back();
	
            flightPilotPairings.push_back(std::pair<std::string, std::string>(flightInfo, pilotInfo));
        }

        if (!flightPilotPairings.empty())
        {
            solveAllocationProblem();
            cout << endl;
        }

        while (i < (int)lines.size() && lines[i].empty()) { i++; }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line))
    {
        inputLines.push_back(line);
    }

    processInputData(inputLines);
    return 0;
}
