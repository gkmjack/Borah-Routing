#include "helper.h"
#include "circuit.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

extern Circuit* c;


void parse_filename(char* filename, int* circ_size, int* num_pts)
{
  string s = string(filename);
  int uds0, uds1, dot;
  uds0 = s.find_first_of('_');
  uds1 = s.find_last_of('_');
  dot = s.find_last_of('.');
  *circ_size = stoi(s.substr(uds0+1, uds1-uds0-1));
  *num_pts = stoi(s.substr(uds1+1, dot-uds1-1));
}

void initialize(int circ_size)
{
  c = new Circuit(circ_size);
}

void parse_file(char* filename, int num_pts)
{
  ifstream file(filename);
  if (!file) {
    cout << "Couldn't open input file" << endl;
    exit(1);
  }
  int x, y;
  for (int i = 0; i < num_pts; i++) {
    file >> x;
    file >> y;
    cout << x << " " << y << endl;
    c->use(x, y, true);
  }
}

void confirm_output(char* filename, FILE** fd)
{
  *fd = fopen(filename, "w");
  if(*fd == NULL) {
    cout << "Can't create output file." << endl;
    exit(2);
  }
  return;
}
