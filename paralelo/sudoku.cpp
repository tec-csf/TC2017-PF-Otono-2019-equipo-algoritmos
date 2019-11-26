#include <iostream>
#include <vector>
#include <algorithm> // for count
#include <assert.h>
#include <omp.h>
using namespace std;

// Possible values that can be entered in a cell
// Elimination
class PossibleValues{
  vector<bool> b; // Each boolean uses 1 bit, very optimal
public:
  PossibleValues(): b(9, true) {} //9 cells
  bool activeVal(int v) const   { return b[v - 1];  }
  void deleteVal(int v)            { b[v - 1] = false; }
  int numActiveVal()    const   { return count(b.begin(), b.end(), true); }  // Check how many values are on "true" (usable values)
  int valueInCell()     const   {
    // simply check if there's only one value in true, then that's the value for the cell
    // this returns an iterator to where the true value is positioned on
    vector<bool>::const_iterator it = find(b.begin(), b.end(), true);
    return 1 + (it - b.begin());
  }
  string str(int width) const;
};

// concatenate possible values
string PossibleValues::str(int width) const {
  string s(width, ' '); //this is simply for the printing part
  int j = 0;
  for(int i = 1; i <= 9; i++){
    if(activeVal(i)) s[j++] = ('0' + i);
  }
  return s;
}

class Sudoku {
  vector<PossibleValues> cells;
  // Groups (for lines, columns and subgrids) that will have to apply the rules
  // GropusOf: Which groups does a certain cell belong to
  // Neighbors: All of the cells a certain cell is related to
  static vector < vector <int> > groups, groupsOf, neighbors;
public:
  Sudoku(string s); //representation of sudoku
  static void initialize();

  PossibleValues possibleVal(int k) const { return cells[k]; } // know the possible values of k
  bool solved() const; // know if a sudoku is solved or not
  bool assign(int k, int val); // to cell k, assign value val, bool return value is to know if we have succeeded or failed
  bool deleteVal(int k, int val); // to cell k, delete value val, bool return value is to know if we have succeeded or failed
  void print(ostream& o) const;
  int lessPossibilites() const; //will return the value of the cell that has the less possibilites, when they have 2 or more possible values
};

vector < vector <int> > Sudoku::groups(27), Sudoku::groupsOf(81), Sudoku::neighbors(81);

void Sudoku::initialize() {  
  int i = 0, j = 0, x = 0, num = 9, k = 0 , num_grupos = 3;
  int nhilos = omp_get_num_threads();
  int id = omp_get_thread_num();
  //In this section the columns are distributed between the threads
  #pragma omp parallel private(i, j, x) if(num >= 16)
  {
    int div = num/nhilos;
    int inicio = id * div;
    int fin = inicio + div;
    if(id == (nhilos - 1)) { fin = num;}

    for (i = inicio; i < fin; ++i) {
      for (j = 0; j < num; ++j) {
        const int k = i * num + j; //Getting the value of a cell
        const int g[3] = { i, num + j, 18 + (i/3)*3 + (j/3) }; // index of groups the k cell belongs to
        //filling the groups and groupsOf of a cell k
        for(x = 0; x < 3; ++x) {
          groups[g[x]].push_back(k);
          groupsOf[k].push_back(g[x]);
        }
      }
    }
  }
  for(k = 0; k < 81; ++k) {
    for(x = 0; x < 3; ++x) {
      const int g = groupsOf[k][x];
      for(i = 0; i < 9; ++i) {
        const int k2 = groups[g][i];
        if(k2 != k) {
          neighbors[k].push_back(k2);
        }
      }
    }
  }
}

// using numActiveVal we can easily know if the sudoku has been solved or not. If, for each cell the numActiveVal = 1, then it is solved
bool Sudoku::solved() const {
  for(int k = 0; k < cells.size(); ++k){
    if(cells[k].numActiveVal() != 1){
      return false;
    }
  }
  return true;
}

bool Sudoku::assign(int k, int val) {
  //  cout << "Assign value (" << k << ", " << val << ")" << endl;
  for(int i = 1; i <= 9;  ++i) {
    if(val != i) {
      if(!deleteVal(k, i)) return false; // erase all the other values in the cell that are not the given value, if it cannot delete a value, return false --> there's an error
    }
  }
  return true;
}

bool Sudoku::deleteVal(int k, int val) {
  //cout << "Delete value (" << k << ", " << val << ")" << endl;
  if(!cells[k].activeVal(val)) {
    return true; // We can easily delete a value because it wasn't active
  }
  cells[k].deleteVal(val);

  const int N = cells[k].numActiveVal();

  if(N == 0) { // If there are 0 possible values --> error
    return false;
  } else if(N == 1) { // If there is only 1 possible values, delete the value in the neighbors
    const int v2 = cells[k].valueInCell();
    for(int i = 0; i < neighbors[i].size(); i++) {
      if(!deleteVal(neighbors[k][i], v2)) return false; // propagate the false values when something fails
    }
  }

  for(int x = 0; x < 3; x++) {
    const int g = groupsOf[k][x]; // the group we're currently analyzing
    int n = 0, k2;
    for(int i = 0; i < 9; i++) {
      const int kp = groups[g][i];
      if(cells[kp].activeVal(val)) {
        n++, k2 = kp;
      }
    }
    if(n == 0) {
      return false; // if a value has been deleted but it isn't present in any other of the neighbors, there's an error
    } else if (n == 1) { // simply assign a value if it's an onl value
      if(!assign(k2, val)) return false;
    }
  }

  return true;

}

int Sudoku::lessPossibilites() const {
  int kMin = -1, min;
  for(int k = 0; k < cells.size(); k++) {
    const int possNums = cells[k].numActiveVal();
    if(possNums > 1 && (kMin == -1 || possNums < min)) {
      min = possNums;
      kMin = k; // cell with less possibilites
    }
  }
  return kMin;
}

void Sudoku::print(ostream& o) const {
  int width = 2;
  for(int k = 0; k < cells.size(); ++k) {
    width = max(width, 1 + cells[k].numActiveVal());
  }
  string separator(3*width, '-');
  cout << endl;
  for(int i = 0; i < 9; i++) {
    if(i == 3 || i == 6) {
      cout << separator << "+-" << separator << "+-"<< separator << endl;
    }
    for(int j = 0; j < 9; j++) {
      const int k = i*9 + j;
      if(j == 3 || j == 6) {
        cout << "| ";
      }
      cout << cells[k].str(width);
    }
    cout << endl;
  }

}

Sudoku::Sudoku(string s): cells(81) { //initialize with default values
  int k = 0;
  for(int i = 0; i < s.size(); i++){
    if(s[i] >= '1' && s[i] <= '9') {
      if (!assign(k, s[i] - '0')){//reduce ASCII so we get a NUMBER
        cout << "Error trying to read the Sudoku. Check input" << endl;
        // we're checking in case we the input value doesn't follow the rules
      }
      k++;
    } else if (s[i] == '0' || s[i] == '.') {
      k++;
    }
  }
  assert(k == 81); //program will break if k != of 81
}

// This function is used to "guess" a value when there are multiple possibilites to choose from resulting in an unsolved Sudoku. It forms a "tree" os possibilites, erasing branches as soon as they aren't working
Sudoku *solve(Sudoku* S) {
  if(S == NULL || S->solved()) { // Base case, either it's solved or it's empty
    return S;
  }
  const int k = S->lessPossibilites();
  const PossibleValues pV = S->possibleVal(k);
  for(int i = 1; i <= 9; i++) {
    if(pV.activeVal(i)) {
      Sudoku *S1 = new Sudoku(*S); //Reference to a copy of a Sudoku
      if(S1->assign(k, i)) {
        Sudoku *S2 = solve(S1);
        if(S2 != NULL) {
          if(S2 != S1) delete S1; // the solution is another one, it's not S1
            return S2;
        }
      }
      delete S1; //the "tree" branch is not this one; erease it completely
    }
  }
  return NULL;
}

int main() {
  double t_inicial = 0, t_final = 0;
  t_inicial = omp_get_wtime();

  Sudoku::initialize();
  
  t_final = omp_get_wtime();
  printf("Inicializacion tardo: %.5f\n", t_final - t_inicial);
 
  string s, line;
  while(getline(cin, line)) s += line;
  Sudoku *S = solve(new Sudoku(s));
  S->print(cout);
}
