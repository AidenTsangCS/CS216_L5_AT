// Aiden Tsang
// CS216 Lab 5

#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <string>

using namespace std;

// central configuration: every tunable rule in the program lives here,
// so a range or policy change never requires hunting through the code
const int MIN_STAT = 40;
const int MAX_STAT = 160;

const string DEFAULT_NAME = "n/a";
const string DEFAULT_TYPE = "n/a";
const int DEFAULT_STAT = MIN_STAT;

const string DEFAULT_ARMY_NAME = "n/a";
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

const string ERR_INVALID_CREATURE =
    "Invalid creature data (negative strength or health); rolling back to defaults";
const string ERR_INVALID_ARMY =
    "Invalid army data (name needs 3+ letters, size needs to be 5+); rolling back to defaults";
const string ERR_ALLOCATION =
    "Memory allocation failed; rolling back and releasing everything allocated so far";

enum MenuOption { CREATE = 1, QUIT };

// the building block of the system: one creature, fully self-managing;
// Army allocates these on the heap and relies on this class keeping
// itself valid at all times
class Creature {
  private:
    string name = DEFAULT_NAME;
    string type = DEFAULT_TYPE;
    int strength = DEFAULT_STAT;
    int health = DEFAULT_STAT;

  public:
    Creature();
    Creature(const Creature &other); // enables Army's deep copies
    explicit Creature(const string &newName, const string &newType);
    explicit Creature(const string &newName, const string &newType, int newStrength, int newHealth);
    ~Creature(); // telemetry hook for observing destruction order

    void setCreature(const string &newName, const string &newType, int newStrength, int newHealth);
    void setName(const string &newName);
    void setType(const string &newType);
    void setStrength(int newStrength);
    void setHealth(int newHealth);

    string getName() const;
    string getType() const;
    int getStrength() const;
    int getHealth() const;
    string toString() const;
};

// the memory owner of the system: holds the heap-allocated creatures and
// carries the no-leak and strong-exception guarantees for the whole program
class Army {
  private:
    string name = DEFAULT_ARMY_NAME;
    int size = DEFAULT_ARMY_SIZE;
    Creature **creatures = nullptr; // the resource the Rule of Three exists to protect

    void releaseMemory(); // the single point where memory is ever returned

  public:
    Army();
    Army(const Army &other); // Rule of Three: deep copy, all-or-nothing
    explicit Army(const string &newName, int newSize);
    ~Army(); // Rule of Three: guarantees cleanup at end of life

    void setArmy(const string &newName, int newSize); // gateway for all name/size rules

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

// menu shell: the only entry point a user interacts with; everything else
// is driven from these two options
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
            cout << "\nInvalid menu choice\n";
        }
    } while (menuChoice != QUIT);
    return 0;
}

Creature::Creature() { setCreature(DEFAULT_NAME, DEFAULT_TYPE, DEFAULT_STAT, DEFAULT_STAT); }

// serves Army's deep copy: every duplicated creature is born through here
Creature::Creature(const Creature &other) {
    setCreature(other.name, other.type, other.strength, other.health);
}

// the constructor Army provisions with: caller supplies identity,
// the creature rolls its own stats within the configured range
Creature::Creature(const string &newName, const string &newType) {
    setCreature(newName, newType, randomStat(), randomStat());
}

Creature::Creature(const string &newName, const string &newType, int newStrength, int newHealth) {
    setCreature(newName, newType, newStrength, newHealth);
}

// telemetry: makes destruction visible so creation/destruction order
// can be verified against expectations during testing
Creature::~Creature() { cout << name << " " << type << " destructor\n"; }

// the single gateway for creature state: every constructor and setter
// funnels through here, so any future validation rule is added once
// and instantly covers every path into the object
void Creature::setCreature(const string &newName, const string &newType, int newStrength,
                           int newHealth) {
    if (newStrength < 0 || newHealth < 0) {
        name = DEFAULT_NAME;
        type = DEFAULT_TYPE;
        strength = DEFAULT_STAT;
        health = DEFAULT_STAT;
        cout << '\n' << ERR_INVALID_CREATURE << '\n';
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

// feeds Army::print(): each creature knows how to present itself as one
// table row, so the table format lives with the data it displays
string Creature::toString() const {
    string result;
    result += name;
    int pad = NAME_COL - static_cast<int>(name.length());
    int i = 0;
    while (i < pad) {
        result += ' ';
        i++;
    }
    result += type;
    pad = TYPE_COL - static_cast<int>(type.length());
    i = 0;
    while (i < pad) {
        result += ' ';
        i++;
    }
    string strengthStr = to_string(strength);
    pad = NUM_COL - static_cast<int>(strengthStr.length());
    i = 0;
    while (i < pad) {
        result += ' ';
        i++;
    }
    result += strengthStr;
    string healthStr = to_string(health);
    pad = NUM_COL - static_cast<int>(healthStr.length());
    i = 0;
    while (i < pad) {
        result += ' ';
        i++;
    }
    result += healthStr;
    return result;
}

Army::Army() {
    name = DEFAULT_ARMY_NAME;
    size = DEFAULT_ARMY_SIZE;
    creatures = nullptr;
}

// deep copy leg of the Rule of Three: produces an independent army so the
// copy and the original can be destroyed separately without double-deletes;
// on failure it leaves a valid empty army instead of a half-built one
Army::Army(const Army &other) {
    if (other.creatures != nullptr && other.size > 0) {
        setArmy(other.name, other.size);
        try {
            creatures = new Creature *[size]; // pointer slots only; objects come next
            int i = 0;
            while (i < size) {
                creatures[i] = nullptr; // the safe state the rollback depends on
                i++;
            }
            i = 0;
            while (i < size) {
                creatures[i] = new Creature(*(other.creatures[i]));
                i++;
            }
        } catch (const bad_alloc &error) {
            cout << '\n' << ERR_ALLOCATION << '\n';
            releaseMemory();
            name = DEFAULT_ARMY_NAME;
        }
    }
}

// all-or-nothing provisioning: the user gets the full requested army or a
// clean default one -- never a partial delivery; size is committed before
// the creature loop so the rollback path knows its bounds mid-failure
Army::Army(const string &newName, int newSize) {
    setArmy(newName, newSize);
    if (size >= MIN_ARMY_SIZE) {
        try {
            creatures = new Creature *[size]; // pointer slots only; objects come next
            int i = 0;
            while (i < size) {
                creatures[i] = nullptr; // the safe state the rollback depends on
                i++;
            }
            i = 0;
            while (i < size) {
                creatures[i] = new Creature(CREATURE_PREFIX + to_string(i + 1), randomType());
                i++;
            }
        } catch (const bad_alloc &error) {
            cout << '\n' << ERR_ALLOCATION << '\n';
            releaseMemory();
            name = DEFAULT_ARMY_NAME;
        }
    }
}

// gateway for army identity rules: constructors delegate name/size
// validation here, so tomorrow's rule change happens in one place
void Army::setArmy(const string &newName, int newSize) {
    if (countAlphabetic(newName) < MIN_ARMY_NAME_ALPHA || newSize < MIN_ARMY_SIZE) {
        name = DEFAULT_ARMY_NAME;
        size = DEFAULT_ARMY_SIZE;
        cout << '\n' << ERR_INVALID_ARMY << '\n';
    } else {
        name = newName;
        size = newSize;
    }
}

// end-of-life leg of the Rule of Three: announces itself for order
// verification, then hands the actual cleanup to the shared release point
Army::~Army() {
    cout << "Army " << name << " destructor in progress\n";
    releaseMemory();
}

// the one place memory is ever returned to the system: both normal
// destruction and bad_alloc rollback rely on this same routine, so the
// no-leak guarantee can never diverge between the two paths
void Army::releaseMemory() {
    if (creatures != nullptr) {
        int i = 0;
        while (i < size) {
            if (creatures[i] != nullptr) {
                delete creatures[i];
                creatures[i] = nullptr;
            }
            i++;
        }
        delete[] creatures;
        creatures = nullptr;
    }
    size = DEFAULT_ARMY_SIZE;
}

string Army::getName() const { return name; }

int Army::getSize() const { return size; }

void Army::print() const {
    cout << "\nArmy: " << name << " (size " << size << ")\n";
    if (creatures == nullptr || size == 0) {
        cout << "This army is empty.\n";
    } else {
        cout << left << setw(NAME_COL) << "Name" << setw(TYPE_COL) << "Type" << right
             << setw(NUM_COL) << "Strength" << setw(NUM_COL) << "Health" << '\n';
        int i = 0;
        while (i < size) {
            cout << creatures[i]->toString() << '\n';
            i++;
        }
    }
}

// recovery point for stream failures: keeps one bad entry from
// poisoning every read that follows it
void clearCin(const string &errorMessage) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << '\n' << errorMessage << '\n';
}

// the program's shield against non-numeric input: nothing past this
// function ever has to worry about a failed numeric read
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

// supports the army-name rule (3+ letters) without pulling in <cctype>
int countAlphabetic(const string &text) {
    int count = 0;
    int i = 0;
    while (i < static_cast<int>(text.length())) {
        char c = text[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            count++;
        }
        i++;
    }
    return count;
}

// single source of randomness for stats, bound to the configured range
int randomStat() { return (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT; }

// single source of randomness for types, bound to the configured type list
string randomType() { return CREATURE_TYPES[rand() % NUM_TYPES]; }

// the proving ground: exercises validation rollback, DMA provisioning,
// deep copying, and scope-exit destruction so every guarantee in the
// design is visible in one run of the program
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
Enter your choice: !

Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 3

Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: -1

Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: dfsa

Invalid menu choice


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 1

--- Creature validation check ---
Attempting Creature("Rex", "dragon", -5, 50):

Invalid creature data (negative strength or health); rolling back to defaults
Resulting creature: n/a            n/a                 40        40
Leaving creature scope:
n/a n/a destructor

--- Create Army ---
Enter army name (3+ letters): Elfie
Enter army size (minimum 5): I

Invalid input; please enter a whole number
Enter army size (minimum 5): A

Invalid input; please enter a whole number
Enter army size (minimum 5): 6

Creating army "Elfie" with 6 creatures...

Army: Elfie (size 6)
Name           Type          Strength    Health
Creature_1     dragon              96        86
Creature_2     troll               58       113
Creature_3     elf                 59        89
Creature_4     goblin             143       113
Creature_5     troll              101        52
Creature_6     elf                 57       151

Creating a deep copy of the army...

Army: Elfie (size 6)
Name           Type          Strength    Health
Creature_1     dragon              96        86
Creature_2     troll               58       113
Creature_3     elf                 59        89
Creature_4     goblin             143       113
Creature_5     troll              101        52
Creature_6     elf                 57       151

Leaving army scope; watch the destruction order (copy is destroyed first):
Army Elfie destructor in progress
Creature_1 dragon destructor
Creature_2 troll destructor
Creature_3 elf destructor
Creature_4 goblin destructor
Creature_5 troll destructor
Creature_6 elf destructor
Army Elfie destructor in progress
Creature_1 dragon destructor
Creature_2 troll destructor
Creature_3 elf destructor
Creature_4 goblin destructor
Creature_5 troll destructor
Creature_6 elf destructor


Army Builder Menu:
1. Create Army
2. Quit
Enter your choice: 2

Goodbye!
aidentsang@Aidens-MacBook-Pro src %
*/
