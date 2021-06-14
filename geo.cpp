/*
  Funcionts for the main algorithm.
*/
#include <bits/stdc++.h>
#ifndef _INCL_GUARD
#define _INCL_GUARD

#define PRECISION 20

#define FULL_RANGE_KM 320
#define EARTH_RADIUS_KM 6356.752
#define PI_OVER_180 0.01745329251
#define CHARGE_RATE_KMH 800
#define SPEED_KMH 105

using namespace std;

struct Point {
  uint32_t id;
  string name;
  double lat;
  double lon;
  Point(uint32_t _id, string _name, double _lat, double _lon) : 
    id(_id), name(_name), lat(_lat), lon(_lon) {}
};

struct Edge {                                                                                                                        
    uint32_t node = 0;                                                                                                                                                                                                                                              
    double weight = 0;
    uint32_t order = 0; 

    Edge(uint32_t _node = 0, double _weight = 0, uint32_t _order = 0 ) : 
      node(_node), weight(_weight), order(_order) {

    } 
}; 

// Greater route distance between 2 points in a circle.
double haversine(double lat1, double lon1, double lat2, double lon2){
  double dLat = (lat2 - lat1) * PI_OVER_180;
  double dLon = (lon2 - lon1) * PI_OVER_180;
  lat1 = (lat1) * PI_OVER_180;
  lat2 = (lat2) * PI_OVER_180;
  double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
  double rad = 6371;
  double c = 2 * asin(sqrt(a));
  double temp = rad * c;
  return temp;
}

//Creates edges in graph from the available chargers.
void load_edges_approx(vector<vector<Edge>>& graph, vector<Point>& chargers) {
  double stage = FULL_RANGE_KM / PRECISION;
  double stage_charge_time = stage / CHARGE_RATE_KMH;
  uint32_t vertices = chargers.size() * PRECISION;
  graph.resize(vertices);

  //Connects vertices in same station group.
  for (uint32_t i = 0; i < vertices; ) {
    for (uint32_t j = 0; j < PRECISION; i++, j++) {
      if (j != PRECISION - 1) {
        graph[i].push_back(Edge(i + 1, stage_charge_time));
      }
    }
  }
  //Connects vertices in different stations.
  for (uint32_t i = 0; i < vertices; i++) {
    uint32_t i_div = i / PRECISION;
    uint32_t i_mod = i % PRECISION;
    for (uint32_t j = 0; j < vertices; j++) {
      uint32_t j_div = j / PRECISION;
      uint32_t j_mod = j % PRECISION;

      if (i_div != j_div) {
        double dist = haversine(chargers[i_div].lat, chargers[i_div].lon, chargers[j_div].lat, chargers[j_div].lon);
        if ((i_mod > j_mod) && ((i_mod - j_mod) * stage >= dist)) {
          graph[i].push_back(Edge(j, dist / SPEED_KMH));
        }        
      }
    }
  }
}
//Prints path, given a parent vector.
//De-allocates pair pointers in parent vector.
void get_path(vector<pair<uint32_t, double>*>& parent, vector<Point>& chargers, uint32_t idx, ostream& os) {
  vector<uint32_t> path;

  while (parent[idx]) {
    path.push_back(idx);
    idx = parent[idx]->first;
  }
  path.push_back(idx);
  
  if (path.size() <= 1) {
    os << "Path does not exist." << endl;
    return;
  }

  //No charge needed.
  if (path.size() == 2) {
    os << chargers[path.back()/ PRECISION].name << ", "<< chargers[path.front() / PRECISION].name << endl;
    return;
  }

  uint32_t prev = path.back();

  os << chargers[prev / PRECISION].name << ", ";

  double charge_start = 0; 
  for (uint32_t i = path.size() - 2;i != -1; i--) {
    if (path[i] / PRECISION == path[i+1] / PRECISION) {
      charge_start += parent[path[i]]->second;
    }
    else {
      if (charge_start > 0) {
        os << charge_start << ", ";
        charge_start = 0;
      }
      os << chargers[path[i] / PRECISION].name << ((i == 0) ? "\n":", "); 
    }
  }
  for (uint32_t i = 0; i < parent.size(); i++) {
    if (parent[i] != nullptr) {
      delete parent[i];
    }
  }
}
//Main SSSP algorithm for stations.
void dijkstra(vector<vector<Edge>>& graph, vector<Point>& chargers, unordered_map<string, uint32_t>& name_idx, 
              string start, string end, ostream& os) {
  if (name_idx[start] == name_idx[end]) {
    os << "Same start and end destination" << endl;
    return;
  }
  uint32_t vertices = chargers.size() * PRECISION;
  vector<double> dist(vertices, numeric_limits<double>::max());
  vector<pair<uint32_t, double>*> parent(vertices, nullptr);

  //Starts at full charge.
  uint32_t start_i = name_idx[start] * PRECISION  + PRECISION - 1; 
  //Ends at no charge.
  uint32_t end_i = name_idx[end] * PRECISION;
  
  auto comp = [](const Edge& a, const Edge& b) -> bool {
            if (a.weight == b.weight) {
                return a.order > b.order;
            }
            return a.weight > b.weight;
  };
  dist[start_i] = 0;
  
  uint32_t order = 0;
  priority_queue<Edge, vector<Edge>, decltype(comp)> pq(comp);
  
  pq.push(Edge(start_i, 0, order++));

  while (!pq.empty()) {
    Edge u = pq.top(); pq.pop();
    for (const Edge& v: graph[u.node]) {
      double new_dist = dist[u.node] + v.weight ;
      if (new_dist < dist[v.node]) {
        dist[v.node] = new_dist;
        pq.push(Edge(v.node, new_dist, order++));
        parent[v.node] = new pair<uint32_t, double>();
        parent[v.node]->first = u.node;
        parent[v.node]->second = v.weight;  
      }  
    }
  }

  get_path(parent, chargers, end_i, os);
  os << '\n';
}

#endif