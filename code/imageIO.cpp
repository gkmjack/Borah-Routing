#include "circuit.h"
#include "point.h"
#include "net.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void Circuit::dump_iteration(int n) {
  ofstream myfile;
  std::string out_string;
  std::stringstream ss;
  ss << n;
  out_string = ss.str();
  string s = "./Outputs/" + out_string + ".pts";
  myfile.open(s.c_str());
  //cout << s << endl;

  for(auto it = nets.begin(); it!=nets.end(); it++) {
    Point* head = (*it)->getHead();
    Point* tail = (*it)->getTail();
    myfile << head->x << " " << head->y << " " << tail->x << " " << tail->y << "\n";
  }
  myfile.close();
}
