// Aiden Tsang
// CS216 Lab 5

#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <sstream>
#include <string>

using namespace std;

const int MIN_STAT = 40;
const int MAX_STAT = 160;
const string DEFAULT_NAME = "n/a";
const string DEFAULT_TYPE = "n/a";
const int DEFAULT_STAT = MIN_STAT;
const string DEFAULT_ARMY_NAME = "Unnamed";
const int DEFAULT_ARMY_SIZE = 0;
const int MIN_ARMY_NAME_ALPHA = 3;
const int MIN_ARMY_SIZE = 5;
const int NUM_TYPES = 4;
const string CREATURE_TYPES[NUM_TYPES] = {"dragon", "goblin", "elf", "troll"};
const string CREATURE_PREFIX = "Creature_";
const int NAME_COL = 15;
const int TYPE_COL = 12;
const int NUM_COL = 10;
const unsigned int RANDOM_SEED = 216;

enum MenuOption { CREATE = 1, QUIT };

class Creature {
  private:
    string name = DEFAULT_NAME;
    string type = DEFAULT_TYPE;
    int strength = DEFAULT_STAT;
    int health = DEFAULT_STAT;

  public:
    Creature();
    explicit Creature(const Creature &other);
    Creature(const string &newName, const string &newType);
    Creature(const string &newName, const string &newType, int newStrength, int newHealth);
    ~Creature();

    void setCreature(const string &newName, const string &newType, int newStrength, int newHealth);
    void setName(const string &newName);
    void setType(const string &newType);
    void setStrength(int newStrength);
    void setHealth(int newHealth);

    string getName() const;
    string getType() const;
    int getStrength() const;
    int getHealth() const;
    int getDamage() const;
    string toString() const;
};

class Army {
  private:
    string name = DEFAULT_ARMY_NAME;
    int size = DEFAULT_ARMY_SIZE;
    Creature **ppCreatures = nullptr;

    void releaseMemory();
    void allocateArray(int count);
    void loadArmy(int newSize);

  public:
    Army();
    explicit Army(const Army &other);
    Army(const string &newName, int newSize);
    ~Army();

    void setArmy(const string &newName, int newSize);
    string getName() const;
    int getSize() const;
    void print() const;
};

void clearCin(const string &errorMessage);
int readInt(const string &prompt);
int countAlphabetic(const string &text);
int randomStat();
string randomType();
void createArmyDemo();

int main() {
    srand(RANDOM_SEED);

    int menuChoice = 0;
    do {
        cout << "\n\nArmy Builder Menu:\n"
                "1. Create Army\n"
                "2. Quit\n"
                "Enter your choice: ";
        cin >> menuChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            menuChoice = 0;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (menuChoice) {
        case CREATE:
            createArmyDemo();
            break;
        case QUIT:
            cout << "\nGoodbye!\n";
            break;
        default:
            clearCin("Invalid menu choice");
        }
    } while (menuChoice != QUIT);

    return 0;
}

Creature::Creature() { setCreature(DEFAULT_NAME, DEFAULT_TYPE, DEFAULT_STAT, DEFAULT_STAT); }

Creature::Creature(const Creature &other) {
    setCreature(other.name, other.type, other.strength, other.health);
}

Creature::Creature(const string &newName, const string &newType) {
    setCreature(newName, newType, randomStat(), randomStat());
}

Creature::Creature(const string &newName, const string &newType, int newStrength, int newHealth) {
    setCreature(newName, newType, newStrength, newHealth);
}

Creature::~Creature() { cout << name << " " << type << " destructor\n"; }

void Creature::setCreature(const string &newName, const string &newType, int newStrength,
                           int newHealth) {
    if (newStrength < 0 || newHealth < 0) {
        cout << "\nInvalid creature data (negative strength or health); keeping current values\n";
    } else {
        name = newName;
        type = newType;
        strength = newStrength;
        health = newHealth;
    }
}

void Creature::setName(const string &newName) { setCreature(newName, type, strength, health); }

void Creature::setType(const string &newType) { setCreature(name, newType, strength, health); }

void Creature::setStrength(int newStrength) { setCreature(name, type, newStrength, health); }

void Creature::setHealth(int newHealth) { setCreature(name, type, strength, newHealth); }

string Creature::getName() const { return name; }

string Creature::getType() const { return type; }

int Creature::getStrength() const { return strength; }

int Creature::getHealth() const { return health; }

int Creature::getDamage() const {
    int damage = 0;
    if (strength > 0) {
        damage = rand() % strength + 1;
    }
    return damage;
}

string Creature::toString() const {
    ostringstream oss;
    oss << left << setw(NAME_COL) << name << setw(TYPE_COL) << type << right << setw(NUM_COL)
        << strength << setw(NUM_COL) << health;
    return oss.str();
}

Army::Army() {
    name = DEFAULT_ARMY_NAME;
    size = DEFAULT_ARMY_SIZE;
    ppCreatures = nullptr;
}

Army::Army(const Army &other) {
    setArmy(other.name, other.size);
    if (size > 0 && other.ppCreatures != nullptr) {
        allocateArray(size);
        if (ppCreatures != nullptr) {
            try {
                for (int i = 0; i < size; i++) {
                    ppCreatures[i] = new Creature(*(other.ppCreatures[i]));
                }
            } catch (const bad_alloc &error) {
                cout << "\nMemory allocation failed; rolling back and releasing everything "
                        "allocated so far\n";
                releaseMemory();
                name = DEFAULT_ARMY_NAME;
            }
        }
    }
}

Army::Army(const string &newName, int newSize) {
    setArmy(newName, newSize);
    if (size >= MIN_ARMY_SIZE) {
        allocateArray(size);
        if (ppCreatures != nullptr) {
            try {
                loadArmy(size);
            } catch (const bad_alloc &error) {
                cout << "\nMemory allocation failed; rolling back and releasing everything "
                        "allocated so far\n";
                releaseMemory();
                name = DEFAULT_ARMY_NAME;
            }
        }
    }
}

Army::~Army() {
    cout << "Army " << name << " destructor in progress\n";
    releaseMemory();
}

void Army::setArmy(const string &newName, int newSize) {
    if (countAlphabetic(newName) < MIN_ARMY_NAME_ALPHA || newSize < MIN_ARMY_SIZE) {
        cout << "\nInvalid army data (name needs 3+ letters, size needs to be 5+); keeping current "
                "values\n";
    } else {
        name = newName;
        size = newSize;
    }
}

void Army::allocateArray(int count) {
    try {
        ppCreatures = new Creature *[count] { nullptr };
    } catch (const bad_alloc &error) {
        cout << "\nMemory allocation failed; rolling back and releasing everything allocated so "
                "far\n";
        ppCreatures = nullptr;
    }
}

void Army::loadArmy(int newSize) {
    if (ppCreatures != nullptr) {
        for (int i = 0; i < newSize; i++) {
            ppCreatures[i] = new Creature(CREATURE_PREFIX + to_string(i + 1), randomType(),
                                          randomStat(), randomStat());
        }
    }
}

void Army::releaseMemory() {
    if (ppCreatures != nullptr) {
        for (int i = 0; i < size; i++) {
            if (ppCreatures[i] != nullptr) {
                delete ppCreatures[i];
                ppCreatures[i] = nullptr;
            }
        }
        delete[] ppCreatures;
        ppCreatures = nullptr;
    }
    size = DEFAULT_ARMY_SIZE;
}

string Army::getName() const { return name; }

int Army::getSize() const { return size; }

void Army::print() const {
    cout << "\nArmy: " << name << " (size " << size << ")\n";
    if (ppCreatures == nullptr || size == 0) {
        cout << "This army is empty.\n";
    } else {
        cout << left << setw(NAME_COL) << "Name" << setw(TYPE_COL) << "Type" << right
             << setw(NUM_COL) << "Strength" << setw(NUM_COL) << "Health" << '\n';
        for (int i = 0; i < size; i++) {
            cout << ppCreatures[i]->toString() << '\n';
        }
    }
}

void clearCin(const string &errorMessage) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << '\n' << errorMessage << '\n';
}

int readInt(const string &prompt) {
    int value = 0;
    bool valid = false;
    while (!valid) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            clearCin("Invalid input; please enter a whole number");
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            valid = true;
        }
    }
    return value;
}

int countAlphabetic(const string &text) {
    int count = 0;
    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        char c = text[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            count++;
        }
    }
    return count;
}

int randomStat() { return (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT; }

string randomType() { return CREATURE_TYPES[rand() % NUM_TYPES]; }

void createArmyDemo() {
    cout << "\n--- Creature validation check ---\n";
    cout << "Attempting Creature(\"Rex\", \"dragon\", -5, 50):\n";
    {
        Creature badCreature("Rex", "dragon", -5, 50);
        cout << "Resulting creature: " << badCreature.toString() << '\n';
        cout << "Leaving creature scope:\n";
    }

    cout << "\n--- Create Army ---\n";
    string armyName;
    cout << "Enter army name (3+ letters): ";
    getline(cin, armyName);
    int armySize = readInt("Enter army size (minimum " + to_string(MIN_ARMY_SIZE) + "): ");

    cout << "\nCreating army \"" << armyName << "\" with " << armySize << " creatures...\n";
    Army army(armyName, armySize);
    army.print();

    cout << "\nCreating a deep copy of the army...\n";
    Army armyCopy(army);
    armyCopy.print();

    cout << "\nLeaving army scope; watch the destruction order (copy is destroyed first):\n";
}

/*Output
aidentsang@Aidens-MacBook-Pro src % "/Users/aidentsang/Pierce college Labs C++/CS216_L5_AT/src/main"


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 1

--- Creature validation check ---
Attempting Creature("Rex", "dragon", -5, 50):

Invalid creature data (negative strength or health); keeping current values
Resulting creature: n/a            n/a                 40        40
Leaving creature scope:
n/a n/a destructor

--- Create Army ---
Enter army name (3+ letters): No
Enter army size (minimum 5): -1

Creating army "No" with -1 creatures...

Invalid army data (name needs 3+ letters, size needs to be 5+); keeping current values

Army: Unnamed (size 0)
This army is empty.

Creating a deep copy of the army...

Invalid army data (name needs 3+ letters, size needs to be 5+); keeping current values

Army: Unnamed (size 0)
This army is empty.

Leaving army scope; watch the destruction order (copy is destroyed first):
Army Unnamed destructor in progress
Army Unnamed destructor in progress


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 1

--- Creature validation check ---
Attempting Creature("Rex", "dragon", -5, 50):

Invalid creature data (negative strength or health); keeping current values
Resulting creature: n/a            n/a                 40        40
Leaving creature scope:
n/a n/a destructor

--- Create Army ---
Enter army name (3+ letters): Aid
Enter army size (minimum 5): 6

Creating army "Aid" with 6 creatures...

Army: Aid (size 6)
Name           Type          Strength    Health
Creature_1     dragon              96        86
Creature_2     troll               58       113
Creature_3     elf                 59        89
Creature_4     goblin             143       113
Creature_5     troll              101        52
Creature_6     elf                 57       151

Creating a deep copy of the army...

Army: Aid (size 6)
Name           Type          Strength    Health
Creature_1     dragon              96        86
Creature_2     troll               58       113
Creature_3     elf                 59        89
Creature_4     goblin             143       113
Creature_5     troll              101        52
Creature_6     elf                 57       151

Leaving army scope; watch the destruction order (copy is destroyed first):
Army Aid destructor in progress
Creature_1 dragon destructor
Creature_2 troll destructor
Creature_3 elf destructor
Creature_4 goblin destructor
Creature_5 troll destructor
Creature_6 elf destructor
Army Aid destructor in progress
Creature_1 dragon destructor
Creature_2 troll destructor
Creature_3 elf destructor
Creature_4 goblin destructor
Creature_5 troll destructor
Creature_6 elf destructor


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: !


Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: q


Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 3


Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 2

Goodbye!
aidentsang@Aidens-MacBook-Pro src %
*/
