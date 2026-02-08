
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
// CONSTANTS (UNCHANGED)
// ==========================
const int ADVANCED_HOURS = 160;
const int INTERMEDIATE_HOURS = 21;
const int FREQUENT_CLIMBER_DAYS = 80;
const int NEW_CLIMBER_DAYS = 10;
const double DEDICATED_SESSION_HOURS = 2.0;

// ==========================
// ENUM (Week 01 - REQUIRED)
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
// COLOR FUNCTION (UNCHANGED)
// ==========================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// =======================================================
// NEW OOP STRUCTURE (ADDED — DOES NOT REMOVE FEATURES)
// =======================================================

// ==========================
// BASE CLASS (REQUIRED)
// ==========================
enum ActivityType {
    TRAINING,
    CLIMB
};
class Activity {
protected:
    string name;
    int duration;
    ClimbDifficulty difficulty;
    ActivityType type;

public:
    // Default constructor (REQUIRED)
    Activity()
        : name(""), duration(0), difficulty(EASY), type(TRAINING) {
    }

    // Parameterized constructor (REQUIRED)
    Activity(string n, int d, ClimbDifficulty diff, ActivityType t)
        : name(n), duration(d), difficulty(diff), type(t) {
    }

    // Getters (REQUIRED)
    string getName() const { return name; }
    int getDuration() const { return duration; }
    ClimbDifficulty getDifficulty() const { return difficulty; }
    ActivityType getType() const { return type; }

    // Setters (REQUIRED)
    void setName(string n) { name = n; }
    void setDuration(int d) { duration = d; }
    void setDifficulty(ClimbDifficulty diff) { difficulty = diff; }

    // Base print (REQUIRED)
    void print() const {
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
        : Activity(n, d, diff, CLIMB),
        hours(h),
        location(loc) {
    }

    double getHours() const { return hours; }
    Location getLocation() const { return location; }

    // OVERRIDES base print (same name/signature)
    void print() const {
        Activity::print();   // REQUIRED base call
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
        : Activity(n, d, diff, TRAINING), reps(r) {
    }

    int getReps() const { return reps; }

    // OVERRIDES base print
    void print() const {
        Activity::print();   // REQUIRED
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

    // POLYMORPHISM: base-class pointers
    Activity* activities[100] = { 0 };

    int activityCount;

public:
    ClimbingTracker()
        : climberName(""),
        totalHours(0),
        climbingDays(0),
        activityCount(0) {
    }

    ~ClimbingTracker() {
        for (int i = 0; i < activityCount; i++) {
            if (activities[i] != 0) {
                delete activities[i];
            }
        }
    }

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
    // DOCTEST SUPPORT METHODS
    // ==========================
    void addSession(Activity* activity) {
        if (activityCount < 100 && activity != 0) {
            activities[activityCount++] = activity;
        }
    }

    int getActivityCount() const {
        return activityCount;
    }

    // ==========================
    // ADD CLIMB SESSION
    // ==========================
    void addClimbSession() {
        if (activityCount >= 100) {
            setColor(12);
            cout << "Maximum number of activities reached.\n";
            setColor(7);

            return;
        }

        cin.ignore(1000, '\n');

        string name;
        cout << "Enter climbing style: ";
        getline(cin, name);

        bool indoor = getYesNo("Is this climb indoor or outdoor? (Y=Indoor, N=Outdoor)");

        ClimbDifficulty diff = promptDifficulty();

        double hours = getValidatedDouble(
            "Hours climbed this session: ", 0.1, 24.0);

        Location loc("", indoor);

        activities[activityCount++] =
            new ClimbSession(name, 0, diff, hours, loc);

        totalHours += static_cast<int>(hours);

        setColor(10); // Green
        cout << "Climb session added.\n";
        setColor(7);

    }


    // ==========================
    // ADD TRAINING SESSION
    // ==========================
    void addTrainingSession() {
        if (activityCount >= 100) {
            cout << "Maximum number of activities reached.\n";
            return;
        }

        string name;
        cout << "Enter style of training: ";
        cin.ignore(1000, '\n');
        getline(cin, name);

        int duration = getValidatedInt(
            "Enter duration (hours): ", 1, 24);

        ClimbDifficulty diff = promptDifficulty();

        int reps = getValidatedInt(
            "Enter number of reps: ", 1, 1000);

        activities[activityCount] =
            new TrainingSession(name, duration, diff, reps);
        activityCount++;

        totalHours += duration;

        setColor(10);
        cout << "Training session added.\n";
        setColor(7);

    }

    // ==========================
    // DISPLAY ALL ACTIVITIES
    // ==========================
    void printActivity(const Activity* a) {
        if (a->getType() == CLIMB) {
            static_cast<const ClimbSession*>(a)->print();
        }
        else {
            static_cast<const TrainingSession*>(a)->print();
        }
    }

    void displayActivities() {
        if (activityCount == 0) {
            cout << "No activities recorded.\n";
            return;
        }

        for (int i = 0; i < activityCount; i++) {
            cout << "-----------------------------\n";
            printActivity(activities[i]);
        }
    }



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
    Location loc("Gym A", true);

    CHECK(loc.getPlace() == "Gym A");
    CHECK(loc.isIndoor() == true);
    CHECK(loc.formattedLocation() == "Gym A (Indoor)");
}


TEST_CASE("Derived class ClimbSession initializes and prints base data") {
    Location loc("Crag X", false);
    ClimbSession cs("Lead Route", 90, HARD, 2.5, loc);

    CHECK(cs.getName() == "Lead Route");
    CHECK(cs.getDuration() == 90);
    CHECK(cs.getDifficulty() == HARD);
    CHECK(cs.getHours() == doctest::Approx(2.5));
    CHECK(cs.getLocation().isIndoor() == false);
}

TEST_CASE("Derived class TrainingSession initializes correctly") {
    TrainingSession ts("Hangboard", 45, MODERATE, 6);

    CHECK(ts.getName() == "Hangboard");
    CHECK(ts.getDuration() == 45);
    CHECK(ts.getDifficulty() == MODERATE);
    CHECK(ts.getReps() == 6);
}

TEST_CASE("Polymorphism: base pointer calls derived print()") {
    Location loc("Gym B", true);
    Activity* a = new ClimbSession("Bouldering", 60, EASY, 1.5, loc);

    CHECK(a->getName() == "Bouldering");
    CHECK(a->getDifficulty() == EASY);

    delete a;
}

TEST_CASE("Tracker adds sessions correctly") {
    ClimbingTracker tracker;
    tracker.setClimberName("Alex");
    tracker.setClimbingDays(20);

    Location loc("Test Gym", true);
    tracker.addSession(
        new ClimbSession("Test Climb", 60, MODERATE, 1.0, loc)
    );

    CHECK(tracker.getActivityCount() == 1);
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
        cout << "4. View Summary Report\n";
        cout << "5. Save Report\n";
        cout << "6. Load Report\n";
        cout << "7. Exit\n";
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

        case 6:
            tracker.loadFromFile();
            break;
        case 7:
            cout << "Goodbye!\n";
            break;
        default:
            setColor(12); // Red
            cout << "Invalid choice.\n";
            setColor(7);

        }

    } while (choice != 7);

    return 0;
}
#endif
