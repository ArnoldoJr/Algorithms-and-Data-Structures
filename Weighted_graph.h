/****************************************
 *  Copyright © 2018 Arnoldo Rodriguez  *
 ****************************************/
 
 // Signature type methods provided by Douglas W. Harder https://ece.uwaterloo.ca/~dwharder/
 
#include <iostream>
#include <limits>

class Weighted_graph {
	private:
        // Private members
        double *matrix;             // This list contains all entries and its edges to other entries.
        int num_vertices;           // The number of vertices in the graph.
        int num_edges;              // The number of edges in the graph.
        int *vertice_degree_array;  // Array of size n containg the degree of each vertice.
    
        // For Dijkistra's algorithm purposes
        double *vertices;           // Table with the entries and 2 columns per entry.
        bool empty;                 // Flag for deleting the table when an insertion is made.
        double *shortest_dist;      // List of shortest distances between nodes.
        bool *updated;              // Updated vertices flags.
    
		static const double INF;    // Infinity constant.

	public:
		Weighted_graph( int = 50 );
		~Weighted_graph();

		int degree( int ) const;
		int edge_count() const;
		double adjacent( int, int ) const;
		double distance( int, int );
		void insert( int, int, double );
};

const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

//////////////////////////////////////////////////////////////////////
//                    Constructor & Destructor                      //
//////////////////////////////////////////////////////////////////////

// Constructor
Weighted_graph::Weighted_graph(int n):
num_edges(0),
empty(true){
    
    if(n <=0){
        n = 1;
    }
    // Initializing and/or updating private members.
    num_vertices = n;
    vertice_degree_array = new int[n];
    updated = new bool[n];
    vertices = new double [n*2];
    matrix = new double [n*n];
    shortest_dist = new double [n*n];
    
    // Using -1 for infinity instead of INF to speed up the comparison process.
    for (int i = 0; i < n*n; ++i) {
        matrix[i] = -1;
        shortest_dist[i] = -1;
        if (i<n) {
            matrix[num_vertices*i + i] = 0;
            vertice_degree_array[i] = 0;
            updated[i] = true;
            shortest_dist[num_vertices*i + i] = 0;
        }
    }
}

// Destructor
Weighted_graph::~Weighted_graph(){
    
    delete [] matrix;
    delete [] shortest_dist;
    delete [] vertice_degree_array;
    delete [] vertices;
    delete [] updated;
}

//////////////////////////////////////////////////////////////////////
//                             Accessors                            //
//////////////////////////////////////////////////////////////////////

// Returns the degree of the vertex n.
int Weighted_graph::degree(int d) const{
    // Throw exception if the argument does not correspond to an existing vertex
    if(d >= num_vertices || d < 0 ){
        throw illegal_argument();
    }
    return vertice_degree_array[d];
}

// Returns the number of edges in the graph.
int Weighted_graph::edge_count() const{
    return num_edges;
}

// Returns the weight of the edge connecting vertices m and n.
double Weighted_graph::adjacent( int a, int b) const{
    // Throw exception if the argument does not correspond to an existing vertex
    if(a >= num_vertices || a < 0 || b >= num_vertices || b < 0){
        throw illegal_argument();
    }
    return (matrix[a*num_vertices + b] == -1)?INF:matrix[a*num_vertices + b];
}

//////////////////////////////////////////////////////////////////////
//                             Mutators                             //
//////////////////////////////////////////////////////////////////////

// Inserts or re-weights an edge between two existing vertices.
void Weighted_graph::insert(int m, int n, double w){
    
    // Throw exception if parameters are incompatible.
    if(w <= 0 || m == n || m >= num_vertices || m < 0 || n >= num_vertices || n < 0){
        throw illegal_argument();
    }
    
    // Increment the number of edges and the vertices' degree whenever we create a new edge.
    if (matrix[m*num_vertices +n] == -1) {
        num_edges++;
        vertice_degree_array[n] = vertice_degree_array[n] + 1;
        vertice_degree_array[m] = vertice_degree_array[m] + 1;
    }
    
    // Updating the matrix with the new edges.
    matrix[m*num_vertices + n] = w;
    matrix[n*num_vertices + m] = w;
    
    // Setting empty flag to true. It means that the values in the shortest_dist table are no longer correct.
    empty = true;
}

// This method returns the shortest path between vertices m and n.
double Weighted_graph::distance(int m, int n){
    
    // Throw exception if the argument does not correspond to an existing vertex
    if(m >= num_vertices || m < 0 || n >= num_vertices || n < 0){
        throw illegal_argument();
    }
    
    // The distance of a vertice to itself is 0.
    if (m == n ) {
        return 0;
    }
    
    // If the value was already calculated from a previous search just return that value.
    if(!empty){
        if (updated[m]) {
            return (shortest_dist[m*num_vertices + n] == -1)?INF:shortest_dist[m*num_vertices + n];
        }
        else if (updated[n]) {
            return (shortest_dist[n*num_vertices + m] == -1)?INF:shortest_dist[n*num_vertices + m];
        }
    }
    
    // Dijkistra Algorithm
    // 1st column: Visited = 1, non visited = 0
    // 2nd column: distance from m
    
    // Filling the 3 column values per vertice in the Dijkistra table
    for (int i = 0; i < num_vertices; ++i) {
        
        // Filling the 3 column values per vertice in the Dijkistra table
        vertices[i*2] = 0;
        vertices[i*2 + 1] = -1;
        
        // Setting the shortest distances of the m vertice to infinity.
        shortest_dist[m*num_vertices + i] = -1;
        
        // Setting the flags of all vetices to false if an insertion was made.
        if (empty) {
            updated[i] = false;
        }
    }
    
    // Setting the 'm' vertice flag to "updated" since we will do so shortly.
    updated[m] = true;
    
    // Setting to zero the distance of m to itself
    vertices[m*2 + 1] = 0;
    
    // The distance between the two vertices.
    double result = -1;
    
    int current_vertice = m;
    
    // Dijkistra Algorithm, looping to find the shortest path.
    for (int a = 0; a < num_vertices; ++a) {
        
        // If we reach the destination vertice just return its distance to from m.
        if(current_vertice == n){
            result = vertices[current_vertice*2 + 1];
        }
        
        // Marking the current vertice as visited.
        vertices[current_vertice*2] = 1;
        
        // Initializing variables for the dijkstra algorithm.
        int min = 0;
        double min_edge = INF;
        
        // Finding all vertices that are connected to current_vertice and updating its distances if their existent path was higher.
        for (int i = 0; i < num_vertices; ++i) {
            if((vertices[i*2] == 0) && (matrix[current_vertice*num_vertices + i] != -1) && (matrix[current_vertice*num_vertices +i]!= 0)){
                if(vertices[i*2 + 1] == -1 || vertices[i*2 + 1] > vertices[current_vertice*2 + 1] + matrix[current_vertice*num_vertices +i]){
                    // Assign new shortest path to vertice if the new path is shorter than the existent path.
                    vertices[i*2 + 1] = vertices[current_vertice*2 + 1] + matrix[current_vertice*num_vertices +i];
                }
            }
            
            // Find the shortest non visited destination in the table (There is probably a better way)
            if(vertices[i*2] == 0 && vertices[i*2 + 1] < min_edge && vertices[i*2 + 1] >= 0){
                min_edge = vertices[i*2 + 1];
                min = i;
            }
        }
        
        // If all non visited edges are have infinite value 'm' and 'n' are disconnected so we break and return INF.
        if(min_edge == INF){
            break;
        }
        // Setting the current_vertice to be the closest non-visited vertice.
        current_vertice = min;
        
        // Updating shortest distance list with the current vertice.
        shortest_dist[current_vertice*num_vertices + m] = vertices[current_vertice*2 + 1];
        shortest_dist[m*num_vertices + current_vertice] = vertices[current_vertice*2 + 1];
    }
    
    // Updating flag.
    empty = false;
    
    return (result == -1)?INF:result;
}
