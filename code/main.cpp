#include "circuit.h"
#include "point.h"
#include "net.h"
#include "helper.h"
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

#include <list>
#include <set>

using namespace std;

Circuit* c;

int main(int argc, char* argv[])
{
  if (argc != 3) {
    cout << "Usage: [EXECUTABLE] [FILENAME.pts] [OUTPUT.TXT]" << endl;
    exit(1);
  }
  // Check input arguments

  int circ_size, num_pts;
  parse_filename(argv[1], &circ_size, &num_pts);
  cout << "Number of points: " << num_pts << endl;
  cout << "Circuit size: " << circ_size << endl;
  // Parse the command line argument

  initialize(circ_size);
  // Set up the circuit

  parse_file(argv[1], num_pts);
  // Read in the input file

  FILE* output;
  confirm_output(argv[2], &output);
  // Generate output file

  c->generateMST();
  cout << "Total weight: " << c->totalCost() << endl;
  clock_t start = clock();
  c->borah_route(output);
  clock_t dur = clock() - start;
  printf("Execution time: %.3f sec\n", (float)dur/CLOCKS_PER_SEC);
}
