#include "Level.h"

using namespace std;

// Some util function
string readFileIntoString(const string &path) {
  ifstream input_file(path);
  if (!input_file.is_open()) {
    cerr << "Could not open the file - '" << path << "'" << endl;
    exit(EXIT_FAILURE);
  }
  return string((std::istreambuf_iterator<char>(input_file)),
                std::istreambuf_iterator<char>());
}

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

Level::Level() { txtVRender.clear(); }

Level::~Level() {}

void Level::loadMedia(string path) {
  string line;
  ifstream loadedFile(path);
  while (getline(loadedFile, line)) {
    istringstream iss(line);
    vector<string> args;
    vector<float> f_args;
    while (getline(iss, line, ' ')) {
      // TODO: do sth with stof and r
      args.push_back(line);
      cout << line << ", ";
    }
    if (args.size() == 0) continue;
    else if (isFloat(args[1])) {
        for (auto arg : args) 
          f_args.push_back(stof(arg));
      }
    
    cout << '\n';

    switch (stoi(args[0])) {
    case ENEMY: {
          // Moving Entity: x, y, w, h, vel, dir, angle, angleVel
        MovingEntity *me = new MovingEntity({{(int) f_args[1], (int) f_args[2], (int) f_args[3], (int) f_args[4]}, {f_args[5], f_args[6], f_args[7], f_args[8]}});
        Enemy::EnemyTimer t = {f_args[9], f_args[10], f_args[11], f_args[12]};
        Enemy *e = new Enemy(*me , t,
          // EnemyTimer: start, idle, splash, normal
          (int) f_args[13], //alpha
          (int) f_args[14], // splash_extend
         (Enemy::State) f_args[15] // state = IDLE = 0
          );
        enemyRender.push_back(*e);

      break;
    }
    case SQR_SNAKE: {
      SqrSnake ss =
          SqrSnake(stof(args[1]), {stoi(args[2]), stoi(args[3])}, stof(args[4]),
                   stof(args[5]), stof(args[6]), stof(args[7]));
      snkRender.push_back(ss);
      // enemyToRender.push_back(make_shared(ss)); PRenderable tmp =
      // enemyToRender.back();
      break;
    }
    case ALLY_TEXT: {
      replace(args[1].begin(), args[1].end(), '-', ' ');
      BouncingText *bt = new BouncingText(args[1], stoi(args[2]), stoi(args[3]), stoi(args[4]),
                      stof(args[5]), stof(args[6]), stof(args[7]));
        // cout <<"1 done" <<endl;
      txtRender.push_back(*bt);
      break;
    }
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
