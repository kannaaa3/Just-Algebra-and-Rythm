#include "Level.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string readFileIntoString(const string &path) {
  ifstream input_file(path);
  if (!input_file.is_open()) {
    cerr << "Could not open the file - '" << path << "'" << endl;
    exit(EXIT_FAILURE);
  }
  return string((std::istreambuf_iterator<char>(input_file)),
                std::istreambuf_iterator<char>());
}

Level::Level() {}

Level::~Level() {}

void Level::loadMedia(string path) {
  string line;
  ifstream loadedFile(path);
  while (getline(loadedFile, line)) {
    istringstream iss(line);
    vector<float> args;
    while (getline(iss, line, ' ')) {
      // TODO: do sth with stof and r
      float u = stof(line);
      args.push_back(u);
      cout << u << ", ";
    }
    switch ((int)args[0]) {
    case ENEMY:
      break;
    case SQR_SNAKE:
      // enemyToRender.push_back(SqrSnake({(int)args[0], (int)args[1]}, args[2],
                                       // args[3], args[4], args[5], args[6]));
      break;
    default:

      break;
    }
    cout << endl;
  }
}

void Level::setNumLevel() {
  ofstream num_level;
  num_level.open("../../data/level.txt", ofstream::trunc);
  num_level << numLevel;
  num_level.close();
}

int Level::getNumLevel() {
  return stoi(readFileIntoString("../../data/level.txt"));
}

// int main(int argc, char *argv[]) {
//   Level x;
//   x.loadMedia("testLevel.txt");
//
//   // string s = readFileIntoString("testLevel.txt");
//   // cout << s << endl;
//
//   return 0;
// }
