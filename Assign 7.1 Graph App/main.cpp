/*
MS549 Data Structures and Testing
12/10/24
Jeff O'Hara
Assignment 7.1 Graph Application - In this app, we will be utilizing the boost library to complete our graph
and use Dijkstra's method for finding the quickest route between cities. We have created a vector of 20 cities
using numerical values for the user to select a city of their choice and then have printed to the screen a 
"navigation" of the possible routes from all 20 cities. You will also have a BFS function to run a Breadth First
Function of the routes. Finally, there will be an option to run the Connected Components algorithm to find how 
many distinct connected subgraphs exist within the vector.
*/

// Libraries
#include <iostream>
#include <string>
#include <vector>
#include <utility>    // for std::pair
#include <limits>     // for std::numeric_limits
#include <cstdlib>    // for std::system("cls") or std::system("clear")
#include <queue>      // for std::queue in BFS
#include <tuple>      // for std::tuple in edges
// Boost libraries to handle AL, DSP, BFS & CC algorithms
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/connected_components.hpp>

// Define the type of our graph using Boost Graph Library (BGL)
// Here we use an undirected graph with edge weights.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_weight_t, double>> Graph;

// A simple struct to hold city information (ID and Name)
struct City {
    int id;
    std::string name;
};

// Function prototypes
void build_graph(Graph& g, std::vector<City>& cities);
void print_menu();
void run_dijkstra(const Graph& g, const std::vector<City>& cities);
void run_bfs(const Graph& g, const std::vector<City>& cities);
void run_connected_components(const Graph& g, const std::vector<City>& cities);
void clear_screen();

int main() {
    Graph g;
    std::vector<City> cities;
    build_graph(g, cities);

    bool running = true;
    while (running) {
        print_menu();
        int choice;
        std::cin >> choice;
        switch (choice) {
        case 1:
            // Run Dijkstra's Algorithm
            run_dijkstra(g, cities);
            break;
        case 2:
            // Run BFS
            run_bfs(g, cities);
            break;
        case 3:
            // Run Connected Components
            run_connected_components(g, cities);
            break;
        case 4:
            // Exit the program
            std::cout << "Exiting program. Goodbye!\n";
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    }

    return 0;
}

// This function builds a graph with at least 20 vertices and 20 edges.
// It assigns city names, IDs, and creates edges representing roads between these cities.
// Each edge has a "distance" weight.
void build_graph(Graph& g, std::vector<City>& cities) {
    // A sample set of city names
    std::vector<std::string> cityNames = {
        "New York", "Boston", "Philadelphia", "Washington D.C.", "Baltimore",
        "Pittsburgh", "Cleveland", "Detroit", "Chicago", "St. Louis",
        "Indianapolis", "Cincinnati", "Columbus", "Buffalo", "Toronto",
        "Montreal", "Ottawa", "Quebec City", "Richmond", "Raleigh"
    };

    // Assign IDs and store cities
    for (int i = 0; i < (int)cityNames.size(); ++i) {
        City c;
        c.id = i;
        c.name = cityNames[i];
        cities.push_back(c);
    }

    // Edges with distances. Each tuple is (city_u, city_v, distance)
    std::vector<std::tuple<int, int, double>> edges = {
        {0, 1, 215.0},   // New York - Boston
        {0, 2, 94.0},    // New York - Philadelphia
        {2, 3, 140.0},   // Philadelphia - Washington D.C.
        {3, 4, 40.0},    // Washington D.C. - Baltimore
        {2, 5, 305.0},   // Philadelphia - Pittsburgh
        {5, 6, 133.0},   // Pittsburgh - Cleveland
        {6, 7, 170.0},   // Cleveland - Detroit
        {7, 8, 285.0},   // Detroit - Chicago
        {8, 9, 296.0},   // Chicago - St. Louis
        {9, 10, 250.0},  // St. Louis - Indianapolis
        {10, 11, 180.0}, // Indianapolis - Cincinnati
        {11, 12, 110.0}, // Cincinnati - Columbus
        {12, 5, 185.0},  // Columbus - Pittsburgh
        {5, 13, 215.0},  // Pittsburgh - Buffalo
        {13, 0, 400.0},  // Buffalo - New York
        {13, 14, 100.0}, // Buffalo - Toronto
        {14, 15, 310.0}, // Toronto - Montreal
        {15, 16, 200.0}, // Montreal - Ottawa
        {16, 17, 250.0}, // Ottawa - Quebec City
        {3, 18, 110.0},  // Washington D.C. - Richmond
        {18, 19, 160.0}  // Richmond - Raleigh
    };

    // Add the edges to the graph
    for (auto& e : edges) {
        int u, v;
        double w;
        std::tie(u, v, w) = e;
        boost::add_edge(u, v, w, g);
    }
}

// Prints a simple menu to the user
void print_menu() {
    clear_screen();
    std::cout << "=========================================\n";
    std::cout << "          Graph Analysis Menu\n";
    std::cout << "=========================================\n";
    std::cout << "1. Run Dijkstra's Shortest Path\n";
    std::cout << "2. Run BFS (Breadth-First Search)\n";
    std::cout << "3. Find Connected Components\n";
    std::cout << "4. Exit\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Enter your choice: ";
}

// Runs Dijkstra's shortest path algorithm from a user-chosen source city
void run_dijkstra(const Graph& g, const std::vector<City>& cities) {
    std::cout << "\nEnter source city ID for Dijkstra (0 to " << cities.size() - 1 << "): ";
    int src;
    std::cin >> src;
    if (src < 0 || src >= (int)cities.size()) {
        std::cout << "Invalid city ID.\n";
        return;
    }

    // Initialize distance and predecessor arrays
    std::vector<double> distances(boost::num_vertices(g), (std::numeric_limits<double>::max)());
    std::vector<Graph::vertex_descriptor> parents(boost::num_vertices(g));

    // Set the source distance to 0
    distances[src] = 0;
    auto src_v = boost::vertex(src, g);

    // Run Dijkstra's algorithm
    boost::dijkstra_shortest_paths(g, src_v,
        boost::distance_map(&distances[0]).predecessor_map(&parents[0]));

    // Print the results
    std::cout << "\nDijkstra's shortest paths from " << cities[src].name << ":\n";
    for (size_t i = 0; i < distances.size(); ++i) {
        std::cout << "To " << cities[i].name << ": ";
        if (distances[i] == std::numeric_limits<double>::max()) {
            std::cout << "No path\n";
        }
        else {
            std::cout << distances[i] << " units\n";
        }
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Runs a Breadth-First Search (BFS) from a chosen source city
void run_bfs(const Graph& g, const std::vector<City>& cities) {
    std::cout << "\nEnter source city ID for BFS (0 to " << cities.size() - 1 << "): ";
    int src;
    std::cin >> src;
    if (src < 0 || src >= (int)cities.size()) {
        std::cout << "Invalid city ID.\n";
        return;
    }

    // distances array will hold the "level" or the number of edges from the source
    std::vector<int> distances(boost::num_vertices(g), -1);
    distances[src] = 0;

    std::queue<Graph::vertex_descriptor> q;
    q.push(boost::vertex(src, g));

    // BFS loop
    while (!q.empty()) {
        auto u = q.front(); q.pop();
        Graph::adjacency_iterator ai, ai_end;
        for (boost::tie(ai, ai_end) = boost::adjacent_vertices(u, g); ai != ai_end; ++ai) {
            if (distances[*ai] == -1) {
                distances[*ai] = distances[u] + 1;
                q.push(*ai);
            }
        }
    }

    // Print BFS results
    std::cout << "\nBFS levels from " << cities[src].name << ":\n";
    for (size_t i = 0; i < distances.size(); ++i) {
        std::cout << cities[i].name << ": ";
        if (distances[i] == -1) {
            std::cout << "Not reachable\n";
        }
        else {
            std::cout << distances[i] << " edge(s) away\n";
        }
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Runs Connected Components algorithm to find how many distinct connected subgraphs exist
void run_connected_components(const Graph& g, const std::vector<City>& cities) {
    std::vector<int> component(boost::num_vertices(g));
    int num = boost::connected_components(g, &component[0]);

    std::cout << "\nGraph has " << num << " connected component(s)\n";
    // Print which cities fall into which components
    for (int c = 0; c < num; ++c) {
        std::cout << "Component " << c << ": ";
        for (size_t i = 0; i < component.size(); ++i) {
            if (component[i] == c) {
                std::cout << cities[i].name << ", ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Clears the console screen. This uses system calls and may not be portable on all systems.
void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
