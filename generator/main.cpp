#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

typedef struct {
  int x;
  int y;
} point_t;

int main(int argc, char* argv[])
{
  srand(time(0));
  // Randomize
  if(argc != 4) {
    cout << "Usage: [generate] [FILE] [SIZE] [POINTS]" << endl;
    exit(2);
  }
  FILE* output = fopen(argv[1], "w");
  if(output == NULL) {
    cout << "Can't open file. Exiting." << endl;
    exit(2);
  }
  int size = stoi(argv[2]);
  int count = stoi(argv[3]);



  point_t* points = (point_t*)malloc(sizeof(point_t)*size*size);
  for(int i=0; i < count; i++) {
    while(1) {
      int x = rand()%size+1;
      int y = rand()%size+1;
      bool found = false;
      for(int j=0; j < i; j++) {
        if(points[j].x == x && points[j].y == y) {
          found = true;
          break;
        }
      }
      if(!found) {
        points[i].x = x;
        points[i].y = y;
        fprintf(output, "%d %d\n",x,y);
        break;
      }
    }
  }
}
