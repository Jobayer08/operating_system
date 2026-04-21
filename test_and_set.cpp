#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

// shared bank balance
int balance = 1000;

// spinlock variable (atomic for hardware-level safety)
atomic_flag lock = ATOMIC_FLAG_INIT;

// acquire lock (test-and-set)
void acquire_lock() {
    while (lock.test_and_set(memory_order_acquire)) {
        // busy waiting (spin)
    }
}

// release lock
void release_lock() {
    lock.clear(memory_order_release);
}

// deposit function
void deposit(int id, int amount) {
    acquire_lock();

    cout << "User " << id << " depositing " << amount << endl;
    int temp = balance;
    temp += amount;
    this_thread::sleep_for(chrono::milliseconds(100)); // simulate delay
    balance = temp;

    cout << "New balance after deposit: " << balance << endl;

    release_lock();
}

// withdraw function
void withdraw(int id, int amount) {
    acquire_lock();

    cout << "User " << id << " withdrawing " << amount << endl;

    if (balance >= amount) {
        int temp = balance;
        temp -= amount;
        this_thread::sleep_for(chrono::milliseconds(100)); // simulate delay
        balance = temp;
    } else {
        cout << "Insufficient balance!\n";
    }

    cout << "New balance after withdraw: " << balance << endl;

    release_lock();
}

int main() {
    vector<thread> users;

    users.emplace_back(deposit, 1, 200);
    users.emplace_back(withdraw, 2, 150);
    users.emplace_back(withdraw, 3, 300);
    users.emplace_back(deposit, 4, 500);

    for (auto &t : users) {
        t.join();
    }

    cout << "\nFinal Balance: " << balance << endl;

    return 0;
}