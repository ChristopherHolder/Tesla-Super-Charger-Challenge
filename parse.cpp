/*
Functions to parse files.
*/
#include "geo.cpp"

//Splits line into tokens.
template <typename T = vector<string>>
T split(const string& s, const char * delims) {
    T container;
    char* temp = new char[s.size() + 1];
    temp[s.size()] = '\0';

    strcpy(temp, s.c_str());

    char * tok = strtok(temp, delims);
    while (tok != NULL) {
        container.push_back(tok);
        tok = strtok(NULL, delims);
    }
    delete [] temp;
    return container;
}

//Populates entries into the relavent data structures for example a name->index map.
void parse_and_load(vector<Point>& chargers, ifstream& is, 
                    unordered_map<string, unsigned int>& index) {
    string line;
    size_t i = 0;
    while (getline(is, line)) {
        vector<string> temp = split(line, ",");
        assert (temp.size() == 3);//Throws if incorrect format.
        index[temp[0]] = i;
        chargers.push_back(Point(i, temp[0], stod(temp[1]), stod(temp[2])));
        i++;
    }
}