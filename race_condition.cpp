#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <mutex>

using namespace std;

int next_id = 1;
vector<int> assigned_ids;

mutex vec_mtx; 

void admit_students(int count) {
    for (int i = 0; i < count; i++) {
        int id = next_id;  
        next_id++;

    
        vec_mtx.lock();
        assigned_ids.push_back(id);
        vec_mtx.unlock();
    }
}

int main() {
    int students_per_thread = 1000;

    thread t1(admit_students, students_per_thread);
    thread t2(admit_students, students_per_thread);
    thread t3(admit_students, students_per_thread);

    t1.join();
    t2.join();
    t3.join();

    set<int> unique_ids(assigned_ids.begin(), assigned_ids.end());

    cout << "Total IDs assigned: " << assigned_ids.size() << endl;
    cout << "Unique IDs: " << unique_ids.size() << endl;

    if (assigned_ids.size() != unique_ids.size()) {
        cout << "Duplicate IDs detected (Race Condition!)" << endl;
    } else {
        cout << "No duplicates" << endl;
    }

    return 0;
}