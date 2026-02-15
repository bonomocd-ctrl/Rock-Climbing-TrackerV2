
// DOCTEST CONFIG (CI ONLY)
#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif
// ==========================
// HEADERS
// ==========================
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>

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
// BASE CLASS (REQUIRED)
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

    // ✅ REQUIRED virtual destructor
    virtual ~Activity() {}

    // ===== SETTERS =====
    void setName(const string& n) { name = n; }
    void setDuration(int d) { duration = d; }
    void setDifficulty(ClimbDifficulty diff) { difficulty = diff; }

    // ===== GETTERS =====
    string getName() const { return name; }
    int getDuration() const { return duration; }
    ClimbDifficulty getDifficulty() const { return difficulty; }

    // ✅ PURE VIRTUAL FUNCTION (NEW)
    virtual string getType() const = 0;

    // keep print virtual
    virtual void print() const {
        cout << "Name: " << name << endl;
        cout << "Duration: " << duration << " minutes" << endl;
        cout << "Difficulty: " << difficultyToString(difficulty) << endl;
    }
};



// ==========================
// COMPOSITION CLASS (REQUIRED)
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

    // Helper method (REQUIRED)
    string formattedLocation() const {
        return place + (indoor ? " (Indoor)" : " (Outdoor)");
    }
};

// ==========================
// DERIVED CLASS #1 (REQUIRED)
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

    // ✅ PURE VIRTUAL IMPLEMENTATION
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
};




// ==========================
// DERIVED CLASS #2 (REQUIRED)
// ==========================
class TrainingSession : public Activity {
private:
    int reps;

public:
    TrainingSession(string n, int d, ClimbDifficulty diff, int r)
        : Activity(n, d, diff), reps(r) {
    }

    // ✅ PURE VIRTUAL IMPLEMENTATION
    string getType() const override {
        return "Training Session";
    }

    void setReps(int r) { reps = r; }
    int getReps() const { return reps; }

    void print() const override {
        Activity::print();
        cout << "Reps: " << reps << endl;
    }
};


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
// FILE LOAD (UNCHANGED)
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
class ActivityManager {
private:
    Activity** items;
    int size;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        Activity** newArray = new Activity * [newCapacity];

        for (int i = 0; i < size; i++) {
            newArray[i] = items[i];
        }

        delete[] items;
        items = newArray;
        capacity = newCapacity;
    }

public:
    ActivityManager(int cap = 5)
        : size(0), capacity(cap) {
        items = new Activity * [capacity];
    }

    ~ActivityManager() {
        for (int i = 0; i < size; i++) {
            delete items[i];
        }
        delete[] items;
    }

    void add(Activity* a) {
        if (size == capacity) {
            resize();
        }
        items[size++] = a;
    }

    void remove(int index) {
        if (index < 0 || index >= size) return;

        delete items[index];

        for (int i = index; i < size - 1; i++) {
            items[i] = items[i + 1];
        }

        size--;
    }

    Activity* get(int index) const {
        if (index < 0 || index >= size) return nullptr;
        return items[index];
    }

    int getSize() const {
        return size;
    }
};



// =======================================================
// STEP 3 — TRACKER CLASS + POLYMORPHIC STORAGE
// (Original logic preserved, upgraded to OOP)
// =======================================================

// ==========================
// TRACKER CLASS
// ==========================
class ClimbingTracker {
private:
    string climberName;
    int totalHours;
    int climbingDays;

    ActivityManager manager;

public:
    ClimbingTracker()
        : climberName(""),
        totalHours(0),
        climbingDays(0) {
    }

    // ❌ REMOVE destructor (manager already deletes memory)

    // ==========================
    // SETUP
    // ==========================
    void setClimberName(const string& name) {
        climberName = name;
    }

    void setClimbingDays(int days) {
        climbingDays = days;
    }

    // ==========================
    // DOCTEST SUPPORT
    // ==========================
    void addSession(Activity* activity) {
        manager.add(activity);
    }

    int getActivityCount() const {
        return manager.getSize();
    }

    // ==========================
    // ADD CLIMB SESSION
    // ==========================
    void addClimbSession() {

        cin.ignore(1000, '\n');

        string name;
        cout << "Enter climbing style: ";
        getline(cin, name);

        bool indoor = getYesNo("Is this climb indoor or outdoor? (Y=Indoor, N=Outdoor)");

        ClimbDifficulty diff = promptDifficulty();

        double hours = getValidatedDouble(
            "Hours climbed this session: ", 0.1, 24.0);

        Location loc(name, indoor);

        manager.add(new ClimbSession(name, 0, diff, hours, loc));

        totalHours += static_cast<int>(hours);

        setColor(10);
        cout << "Climb session added.\n";
        setColor(7);
    }

    // ==========================
    // ADD TRAINING SESSION
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
    // DISPLAY ALL ACTIVITIES
    // ==========================
    void displayActivities() {

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
    void removeActivity(int index) {
        manager.remove(index);
    }

    int getManagerSize() const {
        return manager.getSize();
    }

    // ====== (rest of your report functions stay unchanged)


    // ==========================
    // REPORT GENERATION
    // ==========================

    void generateReport() const {
        double avgHours =
            (climbingDays > 0)
            ? static_cast<double>(totalHours) / climbingDays
            : 0.0;

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

        cout << left << setw(25)
            << "Avg Hours / Session:"
            << fixed << setprecision(1)
            << avgHours << endl;

        cout << left << setw(25) << "Experience Level:" << level << endl;
        cout << left << setw(25) << "Climber Type:" << frequency << endl;
        cout << left << setw(25) << "Performance Rating:" << rating << endl;

        cout << "=================================\n";
    }


    // ==========================
    // SAVE REPORT
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

        double avgHours =
            (climbingDays > 0)
            ? static_cast<double>(totalHours) / climbingDays
            : 0.0;

        outFile << "Name: " << climberName << "\n";
        outFile << "Total Hours: " << totalHours << "\n";
        outFile << "Climbing Days: " << climbingDays << "\n";
        outFile << fixed << setprecision(1);
        outFile << "Avg Hours / Session: " << avgHours << "\n";
        outFile << "Experience Level: "
            << determineExperienceLevel(totalHours) << "\n";
        outFile << "Climber Type: "
            << determineClimberType(climbingDays) << "\n";
        outFile << "Performance Rating: "
            << performanceRating(avgHours) << "\n";

        outFile.close();
        cout << "Report saved to " << filename << endl;
    }



    // ==========================
    // LOAD REPORT
    // ==========================
    void loadFromFile() const {
        string filename;
        cout << "Enter filename to load report: ";
        cin >> filename;

        string report = loadReport(filename);
        cout << "\n----- LOADED REPORT -----\n";
        cout << report << endl;
    }
};
// =======================================================
// STEP 4 — MAIN + DOCTEST (FINAL)
// =======================================================

#ifdef RUN_TESTS
// =======================================================
// DOCTEST UNIT TESTS
// =======================================================

TEST_CASE("Base class constructor initializes correctly") {
    Activity a("Warmup", 30, EASY);
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
}

TEST_CASE("Activity setters update fields correctly") {
    Activity a;

    a.setName("Cooldown");
    a.setDuration(15);
    a.setDifficulty(MODERATE);

    CHECK(a.getName() == "Cooldown");
    CHECK(a.getDuration() == 15);
    CHECK(a.getDifficulty() == MODERATE);
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
TEST_CASE("Manager adds and removes activities") {
    ActivityManager mgr;

    Location loc("Gym", true);

    mgr.add(new ClimbSession("Route", 0, EASY, 1.0, loc));
    mgr.add(new TrainingSession("Hangboard", 1, MODERATE, 5));

    CHECK(mgr.getSize() == 2);

    mgr.remove(0);

    CHECK(mgr.getSize() == 1);
}
TEST_CASE("Polymorphic getType works") {
    Location loc("Gym", true);

    Activity* a1 = new ClimbSession("Route", 0, EASY, 1.0, loc);
    Activity* a2 = new TrainingSession("Hangboard", 1, HARD, 5);

    CHECK(a1->getType() == "Climb Session");
    CHECK(a2->getType() == "Training Session");

    delete a1;
    delete a2;
}


#else

// =======================================================
// INTERACTIVE MAIN (NOT USED IN CI)
// =======================================================

int main() {
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

            tracker.removeActivity(index);

            cout << "Deleted.\n";
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
