#include <iostream>
#include <thread>
#include <vector>

using namespace std;

bool flag[2] = {false, false};
int turn;

int shared_data = 0; // shared database record

void process(int i) {
    int j = 1 - i; // other process

    for (int k = 0; k < 5; k++) {

        // Entry section
        flag[i] = true;
        turn = j;

        while (flag[j] && turn == j) {
            // wait (busy wait)
        }

        // 🔴 Critical Section
        cout << "Process " << i << " is updating data\n";
        int temp = shared_data;
        temp++;
        shared_data = temp;

        // Exit section
        flag[i] = false;

        // Remainder section
    }
}

int main() {
    thread t1(process, 0);
    thread t2(process, 1);

    t1.join();
    t2.join();

    cout << "Final shared data: " << shared_data << endl;

    return 0;
}