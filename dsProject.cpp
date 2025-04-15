#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

class Patient {
private:
    string name;
    int id;
    int priority;
    string diagnosis;
    string description;

public:
    Patient() : name(""), id(0), priority(0), diagnosis("Not determined"), description("---") {}
    Patient(string name, int id, int priority) : name(name), id(id), priority(priority), diagnosis("Not determined"), description("---") {}

    void set_diagnosis(string diagnosis) { this->diagnosis = diagnosis; }
    void set_description(string description) { this->description = description; }

    string get_name() { return name; }
    int get_id() { return id; }
    int get_priority() { return priority; }
    string get_diagnosis() { return diagnosis; }
    string get_description() { return description; }

    friend ostream& operator<<(ostream& os, const Patient& p) {
        os << "Name: " << p.name
            << ", ID: " << p.id
            << ", Priority: " << p.priority
            << ", Diagnosis: " << p.diagnosis
            << ", Description: " << p.description;
        return os;
    }
};





class patientList {
private:
    int count;
    int MAX;
    Patient heap[10];
    Patient overflowStack[10];
    int overflowTop;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    patientList(int MAX) : count(0), MAX(MAX), overflowTop(-1) {}

    void addToOverflow(Patient p);
    void manageOverFlow();
    void addPatient();
    void removePatient();
    void viewPatients();
    void nextPatient();
};



void patientList::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].get_priority() < heap[parent].get_priority()) {
            swap(heap[index], heap[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

void patientList::heapifyDown(int index) {
    while (2 * index + 1 < count) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (heap[left].get_priority() < heap[smallest].get_priority())
            smallest = left;
        if (right < count && heap[right].get_priority() < heap[smallest].get_priority())
            smallest = right;

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            index = smallest;
        }
        else {
            break;
        }
    }
}

void patientList::addToOverflow(Patient p) {
    if (overflowTop < 9) {
        overflowTop++;
        overflowStack[overflowTop] = p;
        cout << "Patient added to overflow." << endl;
    }
    else {
        cout << "Overflow stack is full!! Cannot add more patients." << endl;
    }
}

void patientList::manageOverFlow() {
    if (overflowTop == -1) return;

    cout << "Adding a patient from overflow to the ER..." << endl;
    Patient overflowPatient = overflowStack[overflowTop];
    overflowTop--;
    heap[count++] = overflowPatient;
    heapifyUp(count - 1);
    cout << "Patient added from overflow to the ER." << endl;
}

void patientList::addPatient() {
    string name;
    int id, priority;

    cout << "Enter patient name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter ID: ";
    cin >> id;

    while (true) {
        cout << "Enter priority (1 being most urgent and 5 being least): ";
        cin >> priority;
        if (priority >= 1 && priority <= 5) break;
        else cout << "Invalid priority level!! Try again..." << endl;
    }

    Patient p(name, id, priority);

    if (count == MAX) {
        cout << "Emergency Room is Full!! Adding to overflow..." << endl;
        addToOverflow(p);
        return;
    }

    heap[count++] = p;
    heapifyUp(count - 1);
    cout << "Patient added successfully!!!!" << endl;
}

void patientList::removePatient() {
    if (count == 0) {
        cout << "No patients to remove." << endl;
        return;
    }

    int id;
    cout << "Enter ID of patient to remove: ";
    cin >> id;

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (heap[i].get_id() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Patient not found." << endl;
        return;
    }

    count--;
    heap[index] = heap[count];
    heapifyDown(index);
    manageOverFlow();
    cout << "Patient removed." << endl;
}

void patientList::viewPatients() {
    if (count == 0) {
        cout << "Emergency Room is Empty" << endl;
    }
    else {
        cout << "There are " << count << " patients in the ER:" << endl;
        for (int i = 0; i < count; i++) {
            cout << heap[i] << endl;
        }
    }

    if (overflowTop != -1) {
        cout << "There are " << overflowTop + 1 << " overflow patients:" << endl;
        for (int i = 0; i <= overflowTop; i++) {
            cout << overflowStack[i] << endl;
        }
    }
}

void patientList::nextPatient() {
    if (count == 0) {
        cout << "No patients in the emergency room." << endl;
        return;
    }

    Patient& p = heap[0];
    cout << "Next patient (most urgent):" << endl;
    cout << p << endl;

    int choice;
    cout << "1. Evaluate\n2. Exit to main menu\nChoose: ";
    cin >> choice;

    if (choice == 1) {
        cin.ignore();
        string diagnosis, description;
        cout << "Enter diagnosis: ";
        getline(cin, diagnosis);
        cout << "Enter description: ";
        getline(cin, description);
        p.set_diagnosis(diagnosis);
        p.set_description(description);
        cout << "Patient evaluated." << endl;
    }
}

int main() {
    patientList er(10);
    int choice;

    while (true) {
        system("cls");
        cout << "========= Emergency Room Patient Management System =========" << endl;
        cout << "|-1. Add New Patient" << endl;
        cout << "|-2. Remove Patient by ID" << endl;
        cout << "|-3. View All Patients" << endl;
        cout << "|-4. View Next Patient" << endl;
        cout << "|-5. Exit" << endl;
        cout << "-----Enter an operation to perform: ";
        cin >> choice;

        switch (choice) {
        case 1: er.addPatient(); break;
        case 2: er.removePatient(); break;
        case 3: er.viewPatients(); break;
        case 4: er.nextPatient(); break;
        case 5: cout << "Exiting program..." << endl; return 0;
        default: cout << "Invalid choice!!! Try again..." << endl;
        }
        system("pause");
    }
}
