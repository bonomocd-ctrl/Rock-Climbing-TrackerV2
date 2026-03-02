// ==========================
// DOCTEST + MAIN (TOP OF FILE)
// ==========================
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#ifdef RUN_TESTS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#endif

// Forward declare the interactive runner
int runInteractive();

int main(int argc, char** argv) {
#ifdef RUN_TESTS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
    _CrtMemState before{}, after{}, diff{};
    _CrtMemCheckpoint(&before);

    int result = 0;
    {
        doctest::Context context(argc, argv);
        result = context.run();
    }

    _CrtMemCheckpoint(&after);

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
//MANAGER CLASS 
class ActivityManager {
private:
    DynamicArray<Activity*> items;

public:
    // Constructor
    ActivityManager(int cap = 5) : items(cap) {}

    ActivityManager(const ActivityManager& other) : items(other.getSize()) {
        for (int i = 0; i < other.items.getSize(); i++) {
            items.add(other.items[i]->clone());
        }
    }
    ActivityManager& operator=(const ActivityManager& other) {
        if (this != &other) {
            clear();  // delete current owned memory

            for (int i = 0; i < other.items.getSize(); i++) {
                items.add(other.items[i]->clone());
            }
        }
        return *this;
    }
    // Destructor (prevents ALL memory leaks)
    ~ActivityManager() {
        clear();
    }

    // Add a new activity (takes ownership of pointer)
    void add(Activity* act) {
        items.add(act);
    }

    // Remove activity at index (and delete memory)
    void remove(int index) {
        if (index < 0 || index >= items.getSize()) {
            throw IndexOutOfRange("ActivityManager::remove - invalid index");
        }

        delete items[index];
        items.remove(index);
    }
    // Delete everything safely
    void clear() {
        while (items.getSize() > 0) {
            delete items[0];   // delete first object
            items.remove(0);   // remove it from array
        }
    }

    // Accessors
    int getSize() const {
        return items.getSize();
    }

    Activity* get(int index) const {
        if (index < 0 || index >= items.getSize())
            return nullptr;
        return items[index];
    }
    // operator[]
    Activity* operator[](int index) const {
        // throws if invalid (DynamicArray now throws)
        return items[index];
    }
    // operator+= (add)
    ActivityManager& operator+=(Activity* act) {
        this->add(act);     // explicit this pointer usage 
        return *this;
    }

    // operator-= (remove by index)
    ActivityManager& operator-=(int index) {
        remove(index);
        return *this;
    }
    // Example report function
    void displayAll() const {
        for (int i = 0; i < items.getSize(); i++) {
            if (items[i] != nullptr)
                items[i]->print(); //correct polymorphic function
        }
    }
};

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
#ifdef RUN_TESTS
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
