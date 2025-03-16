#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
void lexer(string source) {
  ifstream file;
  file.open(source);
  if (!file.is_open()) {
    throw ifstream::failure("Error opening file");
  }
  char symbol = ' ';
  while ((symbol = file.get()) != EOF) {
  }

  file.close();
}

int main() {
  string file = "messages.json";
  try {
    lexer(file);
  } catch (const ifstream::failure& ex) {
    cerr << ex.what();
  }
  return 0;
}