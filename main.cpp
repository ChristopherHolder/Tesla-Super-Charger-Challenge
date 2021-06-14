#include <bits/stdc++.h>
#include "geo.cpp"
#include "parse.cpp"

#define TEST 0

vector<Point> chargers;
unordered_map<string, unsigned int> name_index;
vector<vector<Edge>> graph;

using namespace std;

int main(int argc, char** argv) {

#if TEST
#else
    if (argc != 4) {
        std::cout << "Error: requires csv path, initial supercharger names, and final supercharger names" << std::endl;        
        return -1;
    }
    string initial_charger_name = argv[2];
    string goal_charger_name = argv[3];
#endif
    string charger_csv_path = argv[1];
    
    ifstream f;
    f.open(charger_csv_path);

    parse_and_load(chargers, f, name_index);
    
    load_edges_approx(graph, chargers);
      
#if TEST
    ofstream fout;
    fout.open("output.txt", fstream::out | fstream::app);
    vector<pair<string, string>> smoke_tests = {{"Albert_Lea_MN", "Mauston_WI"}, {"Salisbury_MD","Baker_City_OR"}, {"Baker_City_OR","Baker_City_OR"}, {"Miner_MO", "Worthington_MN"}, {"Council_Bluffs_IA","Cadillac_MI"}};
    for (const auto& test : smoke_tests) {
        dijkstra(graph, chargers, name_index, test.first, test.second, fout);
    }
#else
    dijkstra(graph, chargers, name_index, initial_charger_name, goal_charger_name, cout);
#endif
    f.close();
    return 0;
}