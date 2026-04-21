#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;

bool is_writing = false; // indicates if a thread is writing

void write_log(int id) {
    // Entry section
    unique_lock<mutex> lock(mtx);

    // wait efficiently (no busy spinning)
    cv.wait(lock, [] { return !is_writing; });

    // now safe to write
    is_writing = true;

    // Critical Section
    cout << "Thread " << id << " is writing log...\n";
    this_thread::sleep_for(chrono::seconds(10)); // simulate file writing

    cout << "Thread " << id << " finished writing\n";

    // Exit section
    is_writing = false;

    lock.unlock();   // unlock আগে
    cv.notify_one(); // অন্য thread কে notify
}

int main() {
    vector<thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(write_log, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}