#include <iostream>
#include <vector>
#include <cmath>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

string console;
bool logging = true;

double dist(const pair<int, int>& a, const pair<int, int>& b) {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}
void log(const string& x) {
    console.append(x);
    if (logging) cout << x;
}

class Wall {
    public:
        int x;
        int y;
        Wall(int x, int y) {
            this -> x = x;
            this -> y = y;
        }
};

class Gate {
    public:
        int x;
        int y;
        int facing;
        Gate(int x, int y, int facing) {
            this -> x = x;
            this -> y = y;
            this -> facing = facing;
        }
};

class Pheromone {
    public:
        int x;
        int y;
        Pheromone(int x, int y) {
            this -> x = x;
            this -> y = y;
        }
};

class Ant {
    public:
        int direction = 0;
        int x;
        int y;
        Pheromone t;

        Ant(int x, int y) : t(x, y) {
            this -> x = x;
            this -> y = y;
        }
        /*
         * 0 = Left
         * 1 = Up
         * 2 = Right
         * 3 = Down
         * */

        Ant& setFacing(int dir) {
            direction = dir;
            return *this;
        }

        Ant& step(const vector<Wall> & walls, const vector<Pheromone> & pheromones, const vector<Gate> & gates) {
            if (dist({ t.x, t.y }, { x, y }) == 0)
                updatePh(walls, pheromones, gates);

            if (t.x == x && t.y != y && (direction != 1 || direction != 3))
                setFacing(t.y < y ? 1 : 3);
            else if (t.y == y && t.x != x && (direction != 0 || direction != 2))
                setFacing(t.x < x ? 0 : 2);

            switch(direction) {
                case 0:
                    x--;
                    break;
                case 1:
                    y--;
                    break;
                case 2:
                    x++;
                    break;
                case 3:
                    y++;
                    break;
                default:
                    break;
            }
            return *this;
        }

        /*
         * O(m * n)
         * m is walls.size()
         * n is pheromones.size()
         */

        Ant& updatePh(const vector<Wall> & walls, const vector<Pheromone> & pheromones, const vector<Gate> & gates) {
            int upperBound = -999999; int lowerBound = 999999;
            int leftBound = -999999; int rightBound = 999999;

            Pheromone target(x - 1, y);
            double bestDist = 999999;

            for (const Wall & wall : walls) {
                if (wall.x == x) {
                    if (wall.y > y)
                        lowerBound = min(wall.y, lowerBound);
                    else if (wall.y < y)
                        upperBound = max(wall.y, upperBound);
                } else if (wall.y == y) {
                    if (wall.x > x)
                        rightBound = min(wall.x, rightBound);
                    else if (wall.x < x)
                        leftBound = max(wall.x, leftBound);
                }
            }

            for (const Gate & gate : gates) {
                if (gate.x == x) {
                    if (gate.y > y && gate.facing == 1)
                        lowerBound = min(gate.y, lowerBound);
                    else if (gate.y < y && gate.facing == 3)
                        upperBound = max(gate.y, upperBound);
                } else if (gate.y == y) {
                    if (gate.x > x && gate.facing == 0)
                        rightBound = min(gate.x, rightBound);
                    else if (gate.x < x && gate.facing == 2)
                        leftBound = max(gate.x, leftBound);
                }
            }

            for (const Pheromone & pher : pheromones) {
                double d = dist({ pher.x, pher.y }, { x, y });
                if (0 < d && d < bestDist &&
                        upperBound < pher.y && pher.y < lowerBound &&
                        leftBound < pher.x && pher.x < rightBound) {
                    bestDist = d;
                    target = pher;
                }
            }

            this -> t = target;

            if (target.x < x)
                setFacing(0);
            else if (target.y < y)
                setFacing(1);
            else if (x < target.x)
                setFacing(2);
            else if (y < target.y)
                setFacing(3);

            return *this;
        }
};

class Command {
    public:
        string name;
        vector<string> args;
        vector<int> stack;
        Command(const string & name, const vector<string> & args, const vector<int> & stack) {
            this -> name = name;
            this -> args = args;
            this -> stack = stack;
        }

        int popStack() {
            int a = stack[stack.size() - 1];
            stack.pop_back();
            return a;
        }

        void pushStack(int x) {
            stack.push_back(x);
        }

        Command& execute() {
            if (name == "p")
                for (const string& arg : args) log(arg);
            else if (name == "l") {
                for (const string& arg : args) log(arg);
                log("\n");
            }
            else if (name == "i") {
                string b;
                cin >> b;
                stringstream a(b);
                int x;
                a >> x;
                pushStack(x);
            } else if (name == "s")
                pushStack(popStack() - popStack());
            else if (name == "a")
                pushStack(popStack() + popStack());
            else if (name == "d")
                pushStack(popStack() / popStack());
            else if (name == "m")
                pushStack(popStack() * popStack());
            else if (name == "b") {
                int x = popStack();
                pushStack(x);
                pushStack(x);
            }
            else if (name == "t") {
                stringstream a(args[0]);
                int x;
                a >> x;
                pushStack(x);
            } else if (name == "P")
                log(to_string(popStack()));
            else if (name == "L") {
                log(to_string(popStack()));
                log("\n");
            }
            return *this;
        }

        vector<int>& getStack() {
            return stack;
        }

};

class Reader {
    public:
        vector<Wall> wls;
        vector<Pheromone> phrs;
        vector<Ant> nts;
        vector<Gate> gts;
        vector<string> lns;
        vector<int> stack;

        int popStack() {
            int a = stack[stack.size() - 1];
            stack.pop_back();
            return a;
        }

        void pushStack(int x) {
            stack.push_back(x);
        }

        Reader& read(const string & text) {
            vector<string> lines;
            string::size_type pos;
            string::size_type prev = 0;
            int width = 0;

            string s;
            while ((pos = text.find('\n', prev)) != string::npos) {
                lines.push_back(s = text.substr(prev, pos - prev));
                int sz = (int)(s.size());
                width = max(sz, width);
                prev = pos + 1;
            } lines.push_back(text.substr(prev));

            for (auto & line : lines) while (line.size() < width)
                line.append(" ");

            this -> lns = lines;

            for (int y = 0; y < lns.size(); y++) for (int x = 0; x < lns[y].size(); x++) {
                char c = lns[y].at(x);
                if (c == ':')
                    nts.emplace_back(Ant(x, y));
                else if (c == '.')
                    phrs.emplace_back(Pheromone(x, y));
                else if (c == '-' || c == '|')
                    wls.emplace_back(Wall(x, y));
                else if (c == '>')
                    gts.emplace_back(Gate(x, y, 2));
                else if (c == '^')
                    gts.emplace_back(Gate(x, y, 1));
                else if (c == '<')
                    gts.emplace_back(Gate(x, y, 0));
                else if (c == '~')
                    gts.emplace_back(Gate(x, y, 3));
            }

            return *this;
        }

        Reader& compile() {
            while (true) {
                for (Ant & ant : nts) {
                    ant.step(wls, phrs, gts);
                    if (0 > ant.y || ant.y >= lns.size() || 0 > ant.x || ant.x >= lns[ant.y].size())
                        return *this;
                    string current(1, lns[ant.y].at(ant.x));
                    if (current == "q") return *this;
                    else if (current == "z") {
                        if (stack[stack.size() - 1] == 0) { ant.updatePh(wls, phrs, gts); }
                    }
                    else if (regex_match(current, regex("[A-Za-z]"))) {
                        string name = current;
                        ant.step(wls, phrs, gts);
                        if (0 < ant.y >= lns.size() || 0 < ant.x >= lns[ant.y].size())
                            return *this;
                        current = string(1, lns[ant.y].at(ant.x));
                        vector<string> args;
                        string arg;
                        while (current != "`") {
                            ant.step(wls, phrs, gts);
                            if (0 < ant.y >= lns.size() || 0 < ant.x >= lns[ant.y].size())
                                return *this;
                            if (current == ";") {
                                args.push_back(arg);
                                arg = string();
                            } else
                                arg.append(current);
                            if (0 < ant.y >= lns.size() || 0 < ant.x >= lns[ant.y].size())
                                return *this;
                            current = string(1, lns[ant.y].at(ant.x));
                        } args.push_back(arg);
                        stack = Command(name, args, stack).execute().getStack();
                    }
                }
            }
        }

        string represent() {
            string out;
            for (auto & ln : lns) {
                for (char x : ln)
                    if (x != ':')
                        out.append(string(1, x));
                    else
                        out.append(" ");
                out.append("\n");
            }
            for (Ant & ant : nts)
                out[ant.y * (lns[0].size()+1) + ant.x] = ':';
            return out;
        }

};


string compile(const string &text) {
    Reader().read(text).compile();
    return console;
}

int main() {
    ifstream inFile;
    inFile.open("yourinputfiledirectory");
    stringstream strStream;
    strStream << inFile.rdbuf();
    string text = strStream.str();
    compile(text);
    return 0;
}