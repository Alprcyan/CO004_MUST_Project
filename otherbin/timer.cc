#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[]) {
    int time = stoi(argv[1]);
    for (int i = 0; i < time; ++i) {
        cout << argv[0] << "[" << getpid() << "]: " << i + 1 << endl;
        usleep(1000000);
    }
}
