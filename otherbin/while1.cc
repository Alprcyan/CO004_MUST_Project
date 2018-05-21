#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[]) {
    for (unsigned long long i = 0;; ++i) {
        cout << argv[0] << "[" << getpid() << "]: " << i + 1 << endl;
        usleep(1000000);
    }
}
