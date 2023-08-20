#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <FL/Fl_PNG_Image.H>
#include <cmath>
#include <random>
#include <cstdlib>
#include <algorithm>
#include<unistd.h>
#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi = 3.141592653589793238462643383279502884L;
#endif

using namespace std;
const int windowWidth = 1000;
const int windowHeight = 1000;
const double refreshPerSecond = 50;


/*--------------------------------------------------
Point structure
--------------------------------------------------*/
struct Point {
  int x, y;
};

/*--------------------------------------------------
Move structure
source, dest
--------------------------------------------------*/
struct Move{
  Point t;
  Point p;
};

/*--------------------------------------------------
CandyFlavor enum
Translate from int to string to get Candy's flavor
--------------------------------------------------*/
enum CandyFlavor {
    A,
    B, 
    C,
    D,
    E,
    F,
    Null
};

/*--------------------------------------------------
Trnaslation struct
Push translation into a matrix, then modifies candy's center
--------------------------------------------------*/
struct Translation {
  Translation(Point p) {
    fl_push_matrix();
    fl_translate(p.x, p.y);
  }
  ~Translation() {
    fl_pop_matrix();
  }
};


/*--------------------------------------------------
Switch to get type::Candy from randomizer
--------------------------------------------------*/
CandyFlavor RandomCandy(int n){
    switch (n) {
    case 1:
        return CandyFlavor::A;
        break;
    case 2:
        return CandyFlavor::B;
        break;
    case 3:
        return CandyFlavor::C;
        break;
    case 4:
        return CandyFlavor::D;
        break;
    case 5:
        return CandyFlavor::E;
        break;
    case 6:
        return CandyFlavor::F;
        break;
    default:
      return CandyFlavor::Null;
      break;
    }
}