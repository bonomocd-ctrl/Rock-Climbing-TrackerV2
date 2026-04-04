// ==========================
// DOCTEST + MAIN (TOP OF FILE)
// ==========================
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#endif

// Forward declare the interactive runner
int runInteractive();

int main(int argc, char** argv) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
    _CrtMemState before{}, after{}, diff{};
    _CrtMemCheckpoint(&before);

    int result = 0;
    {
        doctest::Context context(argc, argv);
        result = context.run();
    }

    _CrtMemCheckpoint(&after);
    _CrtMemCheckpoint(&before);
    if (_CrtMemDifference(&diff, &before, &after)) {
        std::cerr << "\nREAL leak(s) detected in YOUR code during test run:\n";
        _CrtMemDumpStatistics(&diff);
        // Optional detail:
        // _CrtMemDumpAllObjectsSince(&before);
    }
    else {
        std::cerr << "No memory leaks detected in YOUR code during test run.\n";
    }

    return result;
#else
    // Release build: run the menu program
    (void)argc; (void)argv;
    return runInteractive();
#endif
}
// ==========================
// HEADERS
// ==========================
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <cassert> //assert added by Chris Noonan for the week 11 assignment
using namespace std;
// ==========================
// CONSTANTS 
// ==========================
const int ADVANCED_HOURS = 160;
const int INTERMEDIATE_HOURS = 21;
const int FREQUENT_CLIMBER_DAYS = 80;
const int NEW_CLIMBER_DAYS = 10;
const double DEDICATED_SESSION_HOURS = 2.0;

// ==========================
// ENUM 
// ==========================
enum ClimbDifficulty { EASY = 1, MODERATE, HARD, EXTREME };

// ==========================
// ENUM HELPER
// ==========================
string difficultyToString(ClimbDifficulty d) {
    switch (d) {
    case EASY: return "Easy";
    case MODERATE: return "Moderate";
    case HARD: return "Hard";
    case EXTREME: return "Extreme";
    default: return "Unknown";
    }
}

// ==========================
// COLOR FUNCTION
// ==========================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ==========================
// CUSTOM EXCEPTION (NEW)
// ==========================
class IndexOutOfRange : public std::runtime_error {
public:
    explicit IndexOutOfRange(const std::string& msg)
        : std::runtime_error(msg) {}
};

//New code for week 11 assignment, added by Chris Noonan

template <class Type>
class arrayStack {
private:
    int maxStackSize;
    int stackTop;
    Type* list;

public:

    arrayStack(int stackSize = 100)  //default size 100
    {
        if (stackSize <= 0) {
            cout << "Size of stack must be positive; defaulting to size 100..." << endl;

            maxStackSize = 100;
        }
        else
            maxStackSize = stackSize;
        
        stackTop = 0;
        list = new Type[maxStackSize];
    }
    void initializeStack() 
    {
        stackTop = 0;
    }

    bool isEmptyStack() 
    {
        return (stackTop == 0);
    }

    bool isFullStack() 
    {
        return (stackTop == maxStackSize);
    }

    void push(const Type& newItem) 
    {
        if (!isFullStack()) {
            list[stackTop] = newItem;

            stackTop++;
        }
        else
            cout << "Cannot push to full stack" << endl;
    }

    Type top() const 
    {
        assert(stackTop != 0);

        return list[stackTop - 1];
    }

    void pop()
    {
        if (!isEmptyStack())
            stackTop--;
        else
            cout << "Cannot pop from empty stack" << endl;
    }

    ~arrayStack() 
    {
        delete[] list;
    }
};

template <class Type>
class arrayQueue {
private:
    int maxQueueSize;
    int count;
    int queueFront;
    int queueRear;
    Type* list;

public:

    arrayQueue(int queueSize = 100) //default size 100
    {
        if (queueSize <= 0) {
            cout << "Size of queue must be positive; defaulting to size 100..." << endl;

            maxQueueSize = 100;
        }
        else
            maxQueueSize = queueSize;
        queueFront = 0;
        queueRear = maxQueueSize - 1;
        count = 0;
        list = new Type[maxQueueSize];
    }

    void initializeQueue()
    {
        queueFront = 0;
        queueRear = maxQueueSize - 1;
        count = 0;
    }

    bool isEmptyQueue() const
    {
        return(count == 0);
    }

    bool isFullQueue() const
    {
        return(count == maxQueueSize);
    }

    Type front() const
    {
        assert(!isEmptyQueue());
        return list[queueFront];
    }

    Type back() const 
    {
        assert(!isEmptyQueue());
        return list[queueRear];
    }

    void addQueue(const Type& queueElement)
    {
        if (!isFullQueue()) {
            queueRear = (queueRear + 1) % maxQueueSize;

            count++;
            list[queueRear] = queueElement;
        }
        else
            cout << "Cannot add to a full queue." << endl;
    }

    void deleteQueue()
    {
        if (!isEmptyQueue()) {
            count--;
            queueFront = (queueFront + 1) % maxQueueSize;
        }
        else
            cout << "Cannot remove from an empty queue." << endl;
    }

    ~arrayQueue()
    {
        delete[] list;
    }
};


// ==========================
// BASE CLASS 
// ==========================
class Activity {
protected:
    string name;
    int duration;
    ClimbDifficulty difficulty;

public:
    Activity()
        : name(""), duration(0), difficulty(EASY) {
    }

    Activity(string n, int d, ClimbDifficulty diff)
        : name(n), duration(d), difficulty(diff) {
    }

    // NEW REQUIRED virtual destructor
    virtual ~Activity() {}

    // ===== SETTERS =====
    void setName(const string& n) { name = n; }
    void setDuration(int d) { duration = d; }
    void setDifficulty(ClimbDifficulty diff) { difficulty = diff; }

    // ===== GETTERS =====
    string getName() const { return name; }
    int getDuration() const { return duration; }
    ClimbDifficulty getDifficulty() const { return difficulty; }

    // NEW PURE VIRTUAL FUNCTION
    virtual string getType() const = 0;

    // keep print virtual
    virtual void print() const {
        cout << "Name: " << name << endl;
        cout << "Duration: " << duration << " minutes" << endl;
        cout << "Difficulty: " << difficultyToString(difficulty) << endl;
    }
    // ===== STREAM SUPPORT (for operator<< polymorphism) ===== NEW
    virtual void toStream(ostream& os) const {
        os << name << " | "
            << duration << " mins | "
            << difficultyToString(difficulty);
    }
    virtual Activity* clone() const = 0;
};



// ==========================
// COMPOSITION CLASS 
// ==========================
class Location {
private:
    string place;
    bool indoor;

public:
    Location() : place(""), indoor(true) {}
    Location(string p, bool i) : place(p), indoor(i) {}

    string getPlace() const { return place; }
    bool isIndoor() const { return indoor; }

    void setPlace(string p) { place = p; }
    void setIndoor(bool i) { indoor = i; }

    // Helper method 
    string formattedLocation() const {
        return place + (indoor ? " (Indoor)" : " (Outdoor)");
    }
};

// ==========================
// DERIVED CLASS #1 
// ==========================
class ClimbSession : public Activity {
private:
    double hours;
    Location location;

public:
    ClimbSession(string n, int d, ClimbDifficulty diff,
        double h, Location loc)
        : Activity(n, d, diff), hours(h), location(loc) {
    }
    // ===== OPERATOR== (identity comparison) =====
    bool operator==(const ClimbSession& other) const {
        return name == other.name &&
            hours == other.hours &&
            location.getPlace() == other.location.getPlace() &&
            location.isIndoor() == other.location.isIndoor();
    }

    // ===== STREAM OVERRIDE ===== 
    void toStream(ostream& os) const override {
        os << "[Climb] "
            << name << " | "
            << hours << " hrs | "
            << location.formattedLocation();
    }
    //  PURE VIRTUAL IMPLEMENTATION
    string getType() const override {
        return "Climb Session";
    }

    void setHours(double h) { hours = h; }
    void setLocation(const Location& loc) { location = loc; }

    double getHours() const { return hours; }
    Location getLocation() const { return location; }

    void print() const override {
        Activity::print();
        cout << "Hours Climbed: " << hours << endl;
        cout << "Location: " << location.formattedLocation() << endl;
    }
    Activity* clone() const override {
        return new ClimbSession(*this);
    }
};




// ==========================
// DERIVED CLASS #2 
// ==========================
class TrainingSession : public Activity {
private:
    int reps;

public:
    TrainingSession(string n, int d, ClimbDifficulty diff, int r)
        : Activity(n, d, diff), reps(r) {
    }

    //  PURE VIRTUAL IMPLEMENTATION
    string getType() const override {
        return "Training Session";
    }
    // ===== STREAM OVERRIDE ===== 
    void toStream(ostream& os) const override {
        os << "[Training] "
            << name << " | "
            << reps << " reps";
    }
    void setReps(int r) { reps = r; }
    int getReps() const { return reps; }

    void print() const override {
        Activity::print();
        cout << "Reps: " << reps << endl;
    }
    Activity* clone() const override {
        return new TrainingSession(*this);
    }
};

// ===== GLOBAL STREAM OPERATOR =====
ostream& operator<<(ostream& os, const Activity& a) {
    a.toStream(os);   // polymorphic call
    return os;
}
// ==========================
// BANNER
// ==========================
void displayBanner() {
    setColor(11);
    cout << "=========================================\n";
    cout << "        CLIMBING ACTIVITY TRACKER         \n";
    cout << "=========================================\n";
    setColor(7);
}

// ==========================
// INPUT VALIDATION
// ==========================
int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    do {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number between "
                << min << " and " << max << ".\n";
        }
    } while (value < min || value > max);
    return value;
}

double getValidatedDouble(const string& prompt, double min, double max) {
    double value;
    do {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a value between "
                << min << " and " << max << ".\n";
        }
    } while (value < min || value > max);
    return value;
}

// ==========================
// YES / NO VALIDATION
// ==========================
bool getYesNo(const string& prompt) {
    char choice;
    do {
        cout << prompt << " (y/n): ";
        cin >> choice;

        if (choice == 'Y') choice = 'y';
        if (choice == 'N') choice = 'n';

    } while (choice != 'y' && choice != 'n');

    return choice == 'y';
}

// ==========================
// DIFFICULTY PROMPT
// ==========================
ClimbDifficulty promptDifficulty() {
    cout << "Select Difficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Moderate\n";
    cout << "3. Hard\n";
    cout << "4. Extreme\n";
    int choice = getValidatedInt("Choice: ", 1, 4);
    return static_cast<ClimbDifficulty>(choice);
}

// ==========================
// EXPERIENCE LEVEL
// ==========================
string determineExperienceLevel(int totalHours) {
    if (totalHours >= ADVANCED_HOURS)
        return "Advanced";
    if (totalHours >= INTERMEDIATE_HOURS)
        return "Intermediate";
    return "Beginner";
}

// ==========================
// CLIMBING FREQUENCY
// ==========================
string determineClimberType(int climbingDays) {
    if (climbingDays >= FREQUENT_CLIMBER_DAYS)
        return "Frequent Climber";
    if (climbingDays >= NEW_CLIMBER_DAYS)
        return "Regular Climber";
    return "New Climber";
}

// ==========================
// PERFORMANCE RATING
// ==========================
string performanceRating(double hoursPerSession) {
    if (hoursPerSession >= DEDICATED_SESSION_HOURS)
        return "Highly Dedicated";
    if (hoursPerSession >= 1.0)
        return "Moderately Dedicated";
    return "Casual";
}

// ==========================
// FILE SAVE (UNCHANGED)
// ==========================
void saveReport(const string& filename, const string& report) {
    ofstream outFile(filename);
    if (outFile) {
        outFile << report;
        outFile.close();
        cout << "Report saved to " << filename << endl;
    }
    else {
        cout << "Error saving report.\n";
    }
}

// ==========================
// FILE LOAD 
// ==========================
string loadReport(const string& filename) {
    ifstream inFile(filename);
    string content, line;
    if (inFile) {
        while (getline(inFile, line)) {
            content += line + "\n";
        }
        inFile.close();
    }
    return content;
}
// ===== DYNAMIC ARRAY TEMPLATE (FINAL - NO LEAKS, NO SHALLOW COPIES) =====
template <typename T>
class DynamicArray {
private:
    T* arr;
    int capacity;
    int size;

    // resize helper
    void resize(int newCapacity) {
        T* newArr = new T[newCapacity];

        for (int i = 0; i < size; i++)
            newArr[i] = arr[i];

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

public:
    // ==========================
    // CONSTRUCTOR
    // ==========================
    DynamicArray(int cap = 5)
        : capacity(cap), size(0) {
        arr = new T[capacity];
    }

    // ==========================
    // 🚫 DISABLE COPYING
    // ==========================
    // Prevents shallow copy of pointers (CRITICAL for your assignment)
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    // ==========================
    // DESTRUCTOR
    // ==========================
    ~DynamicArray() {
        delete[] arr;
    }

    // ==========================
    // ADD ELEMENT
    // ==========================
    void add(const T& value) {
        if (size == capacity)
            resize(capacity * 2);

        arr[size++] = value;
    }

    // ==========================
    // REMOVE ELEMENT
    // ==========================
    void remove(int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray::remove - index out of range");
        }

        for (int i = index; i < size - 1; i++)
            arr[i] = arr[i + 1];

        size--;
    }

    // ==========================
    // INDEXING
    // ==========================
    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray::operator[] - index out of range");
        }
        return arr[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray::operator[] const - index out of range");
        }
        return arr[index];
    }
    // ==========================
    // SIZE ACCESSOR
    // ==========================
    int getSize() const {
        return size;
    }
};
// ==========================
// LINKED LIST ADT
// Unordered singly linked list
// ==========================
class ActivityLinkedList {
private:
    struct Node {
        Activity* data;
        Node* next;

        Node(Activity* d, Node* n = nullptr)
            : data(d), next(n) {
        }
    };

    Node* head;
    Node* tail;
    int size;

public:
    // ==========================
    // ITERATOR CLASS
    // ==========================
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* start) : current(start) {}

        bool hasCurrent() const {
            return current != nullptr;
        }

        void next() {
            if (current != nullptr) {
                current = current->next;
            }
        }

        Activity* getData() const {
            if (current == nullptr) {
                return nullptr;
            }
            return current->data;
        }
    };

    // ==========================
    // CONSTRUCTOR
    // ==========================
    ActivityLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // ==========================
    // COPY CONSTRUCTOR
    // deep copy
    // ==========================
    ActivityLinkedList(const ActivityLinkedList& other)
        : head(nullptr), tail(nullptr), size(0) {
        Node* current = other.head;
        while (current != nullptr) {
            insertBack(current->data->clone());
            current = current->next;
        }
    }

    // ==========================
    // COPY ASSIGNMENT
    // deep copy
    // ==========================
    ActivityLinkedList& operator=(const ActivityLinkedList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                insertBack(current->data->clone());
                current = current->next;
            }
        }
        return *this;
    }

    // ==========================
    // DESTRUCTOR
    // ==========================
    ~ActivityLinkedList() {
        clear();
    }

    // ==========================
    // INSERT FRONT
    // ==========================
    void insertFront(Activity* act) {
        Node* newNode = new Node(act, head);
        head = newNode;

        if (tail == nullptr) {
            tail = newNode;
        }

        size++;
    }

    // ==========================
    // INSERT BACK
    // ==========================
    void insertBack(Activity* act) {
        Node* newNode = new Node(act);

        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    // ==========================
    // DELETE AT POSITION
    // returns true if removed
    // ==========================
    bool deleteAtPosition(int index) {
        if (index < 0 || index >= size) {
            return false;
        }

        Node* toDelete = nullptr;

        if (index == 0) {
            toDelete = head;
            head = head->next;

            if (head == nullptr) {
                tail = nullptr;
            }
        }
        else {
            Node* prev = head;
            for (int i = 0; i < index - 1; i++) {
                prev = prev->next;
            }

            toDelete = prev->next;
            prev->next = toDelete->next;

            if (toDelete == tail) {
                tail = prev;
            }
        }

        delete toDelete->data;
        delete toDelete;
        size--;
        return true;
    }

    // ==========================
    // SEARCH BY NAME
    // returns index or -1
    // ==========================
    int searchByName(const string& target) const {
        Node* current = head;
        int index = 0;

        while (current != nullptr) {
            if (current->data != nullptr && current->data->getName() == target) {
                return index;
            }
            current = current->next;
            index++;
        }

        return -1;
    }

    // ==========================
    // GET AT POSITION
    // ==========================
    Activity* getAtPosition(int index) const {
        if (index < 0 || index >= size) {
            return nullptr;
        }

        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }

    // ==========================
    // PRINT/TRAVERSE
    // uses iterator
    // ==========================
    void printList() const {
        Iterator it(head);

        while (it.hasCurrent()) {
            Activity* act = it.getData();
            if (act != nullptr) {
                act->print();
            }
            it.next();
        }
    }

    // ==========================
    // CLEAR
    // ==========================
    void clear() {
        Node* current = head;

        while (current != nullptr) {
            Node* temp = current;
            current = current->next;

            delete temp->data;
            delete temp;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // ==========================
    // SIZE
    // ==========================
    int getSize() const {
        return size;
    }

    // ==========================
    // BEGIN ITERATOR
    // ==========================
    Iterator begin() const {
        return Iterator(head);
    }
};

// ==========================
// MANAGER CLASS
// now uses custom linked list ADT
// ==========================
class ActivityManager {
private:
    ActivityLinkedList items;

public:
    // Constructor
    ActivityManager() = default;

    // Copy constructor
    ActivityManager(const ActivityManager& other)
        : items(other.items) {
    }

    // Copy assignment
    ActivityManager& operator=(const ActivityManager& other) {
        if (this != &other) {
            items = other.items;
        }
        return *this;
    }

    // Destructor
    ~ActivityManager() = default;

    // Add activity at back
    void add(Activity* act) {
        items.insertBack(act);
    }

    // Optional second insertion position
    void addToFront(Activity* act) {
        items.insertFront(act);
    }

    // Remove activity at index
    void remove(int index) {
        if (!items.deleteAtPosition(index)) {
            throw IndexOutOfRange("ActivityManager::remove - invalid index");
        }
    }

    // Clear all activities
    void clear() {
        items.clear();
    }

    // Size
    int getSize() const {
        return items.getSize();
    }

    // Get by index
    Activity* get(int index) const {
        return items.getAtPosition(index);
    }

    // operator[]
    Activity* operator[](int index) const {
        Activity* act = items.getAtPosition(index);
        if (act == nullptr) {
            throw IndexOutOfRange("ActivityManager::operator[] - invalid index");
        }
        return act;
    }

    // operator+=
    ActivityManager& operator+=(Activity* act) {
        add(act);
        return *this;
    }

    // operator-=
    ActivityManager& operator-=(int index) {
        remove(index);
        return *this;
    }

    // Display all
    void displayAll() const {
        items.printList();
    }

    // Recursive count
    int countTypeRecursive(const string& type, int index = 0) const {
        if (index >= items.getSize()) {
            return 0;
        }

        Activity* act = items.getAtPosition(index);
        int match = (act != nullptr && act->getType() == type) ? 1 : 0;

        return match + countTypeRecursive(type, index + 1);
    }

    // Search
    int sequentialSearchByName(const string& target) const {
        return items.searchByName(target);
    }

    // using iterator 
    void displayAllWithIterator() const {
        ActivityLinkedList::Iterator it = items.begin();

        while (it.hasCurrent()) {
            Activity* act = it.getData();
            if (act != nullptr) {
                act->print();
            }
            it.next();
        }
    }
};
/*
    // ==========================
    // NEW: Binary Search
    // Vector must already be sorted
    // ==========================
    int binarySearchByName(const string& target) const {
        int low = 0;
        int high = static_cast<int>(items.size()) - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            string midName = items.at(mid)->getName();

            if (midName == target) {
                return mid;
            }
            else if (midName < target) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return -1;
    }
};
*/
class ClimbingTracker {
private:
    string climberName;
    int totalHours;
    int climbingDays;
    ActivityManager manager;   // handles memory automatically

public:
    // ==========================
    // CONSTRUCTOR / DESTRUCTOR
    // ==========================
    ClimbingTracker() : climberName(""), totalHours(0), climbingDays(0) {}
    ~ClimbingTracker() = default; // manager cleans up Activities automatically

    // ==========================
    // SETTERS
    // ==========================
    void setClimberName(const string& name) { climberName = name; }
    void setClimbingDays(int days) { climbingDays = days; }

    // ==========================
    // NON-INTERACTIVE ADD (FOR TESTS)
    // ==========================
    void addSession(Activity* activity) {
        manager.add(activity);  // manager takes ownership
        if (ClimbSession* cs = dynamic_cast<ClimbSession*>(activity))
            totalHours += static_cast<int>(cs->getHours());
    }

    int getActivityCount() const { return manager.getSize(); }

    // ==========================
    // INTERACTIVE ADD CLIMB SESSION
    // ==========================
    void addClimbSession() {
        cin.ignore(1000, '\n');
        string name;
        cout << "Enter climbing style: ";
        getline(cin, name);

        bool indoor = getYesNo("Is this climb indoor or outdoor? (Y=Indoor, N=Outdoor)");
        ClimbDifficulty diff = promptDifficulty();
        double hours = getValidatedDouble("Hours climbed this session: ", 0.1, 24.0);

        // Allocate and immediately give to manager
        manager.add(new ClimbSession(name, 0, diff, hours, Location(name, indoor)));

        totalHours += static_cast<int>(hours);
    }

    // ==========================
    // INTERACTIVE ADD TRAINING SESSION
    // ==========================
    void addTrainingSession() {
        cin.ignore(1000, '\n');
        string name;
        cout << "Enter training name: ";
        getline(cin, name);

        ClimbDifficulty diff = promptDifficulty();
        int reps = getValidatedInt("Enter reps: ", 1, 100);

        manager.add(new TrainingSession(name, 0, diff, reps));

        setColor(10);
        cout << "Training session added.\n";
        setColor(7);
    }

    // ==========================
    // DISPLAY ACTIVITIES
    // ==========================
    void displayActivities() const {
        if (manager.getSize() == 0) {
            cout << "No activities recorded.\n";
            return;
        }

        for (int i = 0; i < manager.getSize(); i++) {
            cout << "-----------------------------\n";
            manager.get(i)->print();
        }
    }

    // ==========================
    // REMOVE ACTIVITY
    // ==========================
    void removeActivity(int index) { manager.remove(index); }
    int getManagerSize() const { return manager.getSize(); }

    // ==========================
    // REPORT GENERATION
    // ==========================
    void generateReport() const {
        double avgHours = (climbingDays > 0) ? static_cast<double>(totalHours) / climbingDays : 0.0;

        string level = determineExperienceLevel(totalHours);
        string frequency = determineClimberType(climbingDays);
        string rating = performanceRating(avgHours);
        // Generate a table
        setColor(11);
        cout << "\n=================================\n";
        cout << "       CLIMBING SUMMARY\n";
        cout << "=================================\n";
        setColor(7);

        cout << left << setw(25) << "Name:" << climberName << endl;
        cout << left << setw(25) << "Total Hours:" << totalHours << endl;
        cout << left << setw(25) << "Climbing Days:" << climbingDays << endl;
        cout << left << setw(25) << "Avg Hours / Session:" << fixed << setprecision(1) << avgHours << endl;
        cout << left << setw(25) << "Experience Level:" << level << endl;
        cout << left << setw(25) << "Climber Type:" << frequency << endl;
        cout << left << setw(25) << "Performance Rating:" << rating << endl;

        cout << left << setw(25) << "Climb Sessions:"
            << manager.countTypeRecursive("Climb Session") << endl;

        cout << left << setw(25) << "Training Sessions:"
            << manager.countTypeRecursive("Training Session") << endl;

        cout << "=================================\n";
    }
    // ==========================
    // SAVE / LOAD
    // ==========================
    void saveToFile() const {
        string filename;
        cout << "Enter filename to save report: ";
        cin >> filename;

        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error saving report.\n";
            return;
        }

        double avgHours = (climbingDays > 0) ? static_cast<double>(totalHours) / climbingDays : 0.0;

        outFile << "Name: " << climberName << "\n";
        outFile << "Total Hours: " << totalHours << "\n";
        outFile << "Climbing Days: " << climbingDays << "\n";
        outFile << fixed << setprecision(1);
        outFile << "Avg Hours / Session: " << avgHours << "\n";
        outFile << "Experience Level: " << determineExperienceLevel(totalHours) << "\n";
        outFile << "Climber Type: " << determineClimberType(climbingDays) << "\n";
        outFile << "Performance Rating: " << performanceRating(avgHours) << "\n";

        outFile.close();
        cout << "Report saved to " << filename << endl;
    }

    void loadFromFile() const {
        string filename;
        cout << "Enter filename to load report: ";
        cin >> filename;

        string report = loadReport(filename);
        cout << "\n----- LOADED REPORT -----\n";
        cout << report << endl;
    }

    // ==========================
    // UTILITY TEMPLATE
    // ==========================
    template <typename T>
    T maxValue(T a, T b) {
        return (a > b) ? a : b;
    }
};
#ifdef _DEBUG
// =======================================================
// DOCTEST UNIT TESTS 
// =======================================================

TEST_CASE("Base class constructor initializes correctly") {
    ClimbSession a("Warmup", 30, EASY, 1.0, Location("Gym", true));
    CHECK(a.getName() == "Warmup");
    CHECK(a.getDuration() == 30);
    CHECK(a.getDifficulty() == EASY);
}

TEST_CASE("Composition class Location works correctly") {
    Location loc("indoor", true);

    CHECK(loc.getPlace() == "indoor");
    CHECK(loc.isIndoor() == true);
    CHECK(loc.formattedLocation() == "indoor (Indoor)");
}

TEST_CASE("Derived class ClimbSession initializes base and derived data correctly") {
    Location loc("Outdoor", false);
    ClimbSession cs("Lead Route", 0, HARD, 2.5, loc);

    CHECK(cs.getName() == "Lead Route");
    CHECK(cs.getDifficulty() == HARD);
    CHECK(cs.getHours() == doctest::Approx(2.5));
    CHECK(cs.getLocation().isIndoor() == false);
}

TEST_CASE("Derived class TrainingSession initializes correctly") {
    TrainingSession ts("Hangboard", 0, MODERATE, 6);

    CHECK(ts.getName() == "Hangboard");
    CHECK(ts.getDifficulty() == MODERATE);
    CHECK(ts.getReps() == 6);
}

TEST_CASE("Tracker adds sessions correctly") {
    ClimbingTracker tracker;
    tracker.setClimberName("Alex");
    tracker.setClimbingDays(20);

    Location loc("Indoor", true);

    tracker.addSession(
        new ClimbSession("Indoor", 0, MODERATE, 1.0, loc)
    );

    CHECK(tracker.getActivityCount() == 1);

    
    tracker.removeActivity(0);
}

TEST_CASE("Activity setters update fields correctly (via derived class)") {
    Location loc("Gym", true);
    ClimbSession cs("Temp", 10, EASY, 1.0, loc);

    cs.setName("Cooldown");
    cs.setDuration(15);
    cs.setDifficulty(MODERATE);

    CHECK(cs.getName() == "Cooldown");
    CHECK(cs.getDuration() == 15);
    CHECK(cs.getDifficulty() == MODERATE);
}

TEST_CASE("Derived class setters work correctly") {
    Location loc("Indoor", true);
    ClimbSession cs("Bouldering", 0, EASY, 1.0, loc);

    cs.setHours(2.0);
    Location newLoc("Outdoor", false);
    cs.setLocation(newLoc);

    CHECK(cs.getHours() == doctest::Approx(2.0));
    CHECK(cs.getLocation().isIndoor() == false);
}

// ===== MANAGER TESTS 
TEST_CASE("Manager adds and removes activities") {
    ActivityManager mgr;

    Location loc("Inside", true);

    mgr.add(new ClimbSession("Route", 0, EASY, 1.0, loc));
    mgr.add(new TrainingSession("Hangboard", 1, MODERATE, 5));

    CHECK(mgr.getSize() == 2);

    mgr.remove(0);
    CHECK(mgr.getSize() == 1);

   
    mgr.clear();
}

TEST_CASE("Polymorphic getType works") {
    Location loc("Inside", true);

    Activity* a1 = new ClimbSession("Route", 0, EASY, 1.0, loc);
    Activity* a2 = new TrainingSession("Hangboard", 1, HARD, 5);

    CHECK(a1->getType() == "Climb Session");
    CHECK(a2->getType() == "Training Session");

    delete a1;
    delete a2;
}

TEST_CASE("ClimbSession equality works") {
    Location loc("Gym", true);

    ClimbSession a("Route", 0, EASY, 1.0, loc);
    ClimbSession b("Route", 0, EASY, 1.0, loc);
    ClimbSession c("Different", 0, EASY, 1.0, loc);

    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST_CASE("operator<< outputs correct string") {
    Location loc("Gym", true);
    ClimbSession cs("Route", 0, EASY, 1.5, loc);

    std::ostringstream out;
    out << cs;

    CHECK(out.str().find("Route") != std::string::npos);
}

TEST_CASE("Manager operator[] valid index returns correct item") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr += new ClimbSession("A", 0, EASY, 1.0, loc);

    CHECK(mgr[0] != nullptr);

    mgr.clear();
}

TEST_CASE("Manager operator[] invalid index throws") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr += new ClimbSession("A", 0, EASY, 1.0, loc);

    CHECK_THROWS_AS(mgr[5], IndexOutOfRange);
    CHECK_THROWS_AS(mgr[-1], IndexOutOfRange);

    mgr.clear();
}
TEST_CASE("Manager operator-= invalid removal throws") {
    ActivityManager mgr;

    CHECK_THROWS_AS(mgr -= 0, IndexOutOfRange);

    Location loc("Gym", true);
    mgr += new ClimbSession("A", 0, EASY, 1.0, loc);

    CHECK_THROWS_AS(mgr -= 5, IndexOutOfRange);

    mgr.clear();
}
TEST_CASE("Manager += and -= works") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr += new ClimbSession("A", 0, EASY, 1.0, loc);
    mgr += new TrainingSession("B", 0, MODERATE, 5);

    CHECK(mgr.getSize() == 2);

    mgr -= 0;

    CHECK(mgr.getSize() == 1);


    mgr.clear();
}

TEST_CASE("Function template maxValue works") {
    ClimbingTracker tracker;

    CHECK(tracker.maxValue(5, 10) == 10);
    CHECK(tracker.maxValue(2.5, 1.1) == doctest::Approx(2.5));
}

TEST_CASE("DynamicArray template works") {
    DynamicArray<int> arr;

    arr.add(1);
    arr.add(2);

    CHECK(arr.getSize() == 2);

    arr.remove(0);
    CHECK(arr.getSize() == 1);
}
TEST_CASE("DynamicArray resizes when capacity exceeded") {
    DynamicArray<int> arr(2); // small capacity to force resize fast

    arr.add(10);
    arr.add(20);
    arr.add(30); // should trigger resize

    CHECK(arr.getSize() == 3);
    CHECK(arr[0] == 10);
    CHECK(arr[1] == 20);
    CHECK(arr[2] == 30);

    arr.remove(1); // remove middle, should shift
    CHECK(arr.getSize() == 2);
    CHECK(arr[0] == 10);
    CHECK(arr[1] == 30);
}
TEST_CASE("DynamicArray throws on invalid index") {
    DynamicArray<int> arr;
    arr.add(1);

    CHECK_THROWS_AS(arr[-1], IndexOutOfRange);
    CHECK_THROWS_AS(arr[1], IndexOutOfRange);
}

TEST_CASE("DynamicArray throws on invalid remove") {
    DynamicArray<int> arr;
    arr.add(1);

    CHECK_THROWS_AS(arr.remove(-1), IndexOutOfRange);
    CHECK_THROWS_AS(arr.remove(5), IndexOutOfRange);
}
TEST_CASE("operator<< outputs correct string for TrainingSession") {
    TrainingSession ts("Hangboard", 0, HARD, 12);

    std::ostringstream out;
    out << ts;

    // should include key identity fields from TrainingSession::toStream
    CHECK(out.str().find("Hangboard") != std::string::npos);
    CHECK(out.str().find("12") != std::string::npos);
}
TEST_CASE("ActivityManager recursive count returns correct number of climb sessions") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("Route 1", 0, EASY, 1.0, loc));
    mgr.add(new TrainingSession("Hangboard", 0, MODERATE, 10));
    mgr.add(new ClimbSession("Route 2", 0, HARD, 2.0, loc));

    CHECK(mgr.countTypeRecursive("Climb Session") == 2);
    CHECK(mgr.countTypeRecursive("Training Session") == 1);

    mgr.clear();
}

TEST_CASE("ActivityManager recursive count returns 0 when empty") {
    ActivityManager mgr;

    CHECK(mgr.countTypeRecursive("Climb Session") == 0);
}

TEST_CASE("ActivityManager recursive count returns 0 for type not found") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("Route 1", 0, EASY, 1.0, loc));

    CHECK(mgr.countTypeRecursive("Training Session") == 0);

    mgr.clear();
}
//New
TEST_CASE("Linked list insert back into empty manager works") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("A", 0, EASY, 1.0, loc));

    CHECK(mgr.getSize() == 1);
    CHECK(mgr[0]->getName() == "A");

    mgr.clear();
}

TEST_CASE("Linked list insert front works") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("B", 0, EASY, 1.0, loc));
    mgr.addToFront(new ClimbSession("A", 0, EASY, 1.0, loc));

    CHECK(mgr.getSize() == 2);
    CHECK(mgr[0]->getName() == "A");
    CHECK(mgr[1]->getName() == "B");

    mgr.clear();
}

TEST_CASE("Linked list delete removes existing node") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("A", 0, EASY, 1.0, loc));
    mgr.add(new ClimbSession("B", 0, EASY, 1.0, loc));

    mgr.remove(0);

    CHECK(mgr.getSize() == 1);
    CHECK(mgr[0]->getName() == "B");

    mgr.clear();
}

TEST_CASE("Linked list delete node that does not exist throws") {
    ActivityManager mgr;

    CHECK_THROWS_AS(mgr.remove(0), IndexOutOfRange);
}

TEST_CASE("Linked list search finds node") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("Boulder", 0, EASY, 1.0, loc));
    mgr.add(new TrainingSession("Hangboard", 0, MODERATE, 10));

    CHECK(mgr.sequentialSearchByName("Hangboard") == 1);

    mgr.clear();
}

TEST_CASE("Linked list search returns -1 when not found") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("Boulder", 0, EASY, 1.0, loc));

    CHECK(mgr.sequentialSearchByName("Campus") == -1);

    mgr.clear();
}

TEST_CASE("Traversing empty linked list is safe") {
    ActivityManager mgr;

    CHECK(mgr.getSize() == 0);
    CHECK(mgr.sequentialSearchByName("Anything") == -1);
}

TEST_CASE("Iterator starts at front and advances correctly") {
    ActivityLinkedList list;
    Location loc("Gym", true);

    list.insertBack(new ClimbSession("A", 0, EASY, 1.0, loc));
    list.insertBack(new ClimbSession("B", 0, EASY, 1.0, loc));

    ActivityLinkedList::Iterator it = list.begin();

    REQUIRE(it.hasCurrent());
    CHECK(it.getData()->getName() == "A");

    it.next();
    REQUIRE(it.hasCurrent());
    CHECK(it.getData()->getName() == "B");

    it.next();
    CHECK_FALSE(it.hasCurrent());
}
TEST_CASE("Recursive count still works with linked list storage") {
    ActivityManager mgr;
    Location loc("Gym", true);

    mgr.add(new ClimbSession("Route 1", 0, EASY, 1.0, loc));
    mgr.add(new TrainingSession("Hangboard", 0, MODERATE, 10));
    mgr.add(new ClimbSession("Route 2", 0, HARD, 2.0, loc));

    CHECK(mgr.countTypeRecursive("Climb Session") == 2);
    CHECK(mgr.countTypeRecursive("Training Session") == 1);

    mgr.clear();
}
TEST_CASE("Printing empty linked list is safe") {
    ActivityManager mgr;
    CHECK_NOTHROW(mgr.displayAll());
}

//Doctests added by Chris Noonan as part of the Week 11 assignment
TEST_CASE("arrayStack basic operations")
{
    arrayStack<int> s(5);

    CHECK(s.isEmptyStack() == true);

    s.push(10);
    s.push(20);
    s.push(30);

    CHECK(s.isEmptyStack() == false);
    CHECK(s.top() == 30);

    s.pop();
    CHECK(s.top() == 20);

    s.pop();
    s.pop();

    CHECK(s.isEmptyStack() == true);
}

TEST_CASE("arrayQueue basic operations")
{
    arrayQueue<int> q(5);

    CHECK(q.isEmptyQueue() == true);

    q.addQueue(10);
    q.addQueue(20);
    q.addQueue(30);

    CHECK(q.front() == 10);
    CHECK(q.back() == 30);

    q.deleteQueue();

    CHECK(q.front() == 20);

    q.deleteQueue();
    q.deleteQueue();

    CHECK(q.isEmptyQueue() == true);
}

#else
// =======================================================
// INTERACTIVE MAIN (NOT USED IN CI)
// =======================================================

int runInteractive() {
    ClimbingTracker tracker;

    displayBanner();

    string name;
    setColor(14);  // Yellow
    cout << "Enter your full name: ";
    getline(cin, name);
    setColor(14);  // Yellow
    string climbingStyle;
    cout << "What style of climbing do you enjoy? ";
    setColor(14);  // Yellow
    getline(cin, climbingStyle);

    string location;
    setColor(14);  // Yellow
    cout << "Where do you usually climb? ";
    getline(cin, location);

    int days;
    cout << "About how many days do you climb per year? ";
    cin >> days;
    tracker.setClimbingDays(days);

    int choice;
    do {
        setColor(14);  // Yellow
        cout << "\n====== MENU ======\n";

        setColor(10);
        cout << "1. Add Climb Session\n";
        cout << "2. Add Training Session\n";
        cout << "3. View Activities\n";
        cout << "4. View Summary Report and save to file\n";
        cout << "5. Load report\n";
        cout << "6. Exit\n";
        cout << "7. Delete Activity\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            tracker.addClimbSession();
            break;
        case 2:
            tracker.addTrainingSession();
            break;

        case 3:
            tracker.displayActivities();
            break;




        case 4:
            tracker.generateReport();
            tracker.saveToFile();
            break;

        case 5:
            tracker.loadFromFile();
            break;
        case 6:
            cout << "Goodbye!\n";
            break;
        case 7:
        {
            int index = getValidatedInt(
                "Enter index to delete: ",
                0,
                tracker.getManagerSize() - 1
            );

            try {
                tracker.removeActivity(index);
                cout << "Deleted.\n";
            }
            catch (const std::exception& ex) {
                cout << "Delete failed: " << ex.what() << "\n";
            }
            break;
        }


        default:
            setColor(12); // Red
            cout << "Invalid choice.\n";
            setColor(7);

        }

    } while (choice != 6);
    return 0;
}
#endif
