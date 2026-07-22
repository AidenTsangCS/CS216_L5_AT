**NG 7/22/2026**
* **To provide you with thorough feedback, I read your code line by line, sometimes multiple times**. Jumping from file to file is too time-consuming. Unless noted otherwise, place your class definitions above the main() and all function definitions below main(), all in one file. While the industry standard involves multi-source file programs, we will use a single file format for these labs unless instructed otherwise. Submit a fully completed lab by th due date using the correct GitHub invite link to get a second chance
* **feedback.md is for instructor use only**: please DO NOT change the feedback.md; make a copy if needed; do not add anything; if you do not understand  the feedback, let me know asap (mymail.laccd.edu, grigorn@laccd.edu)
* if you do not understand feedback, let me know asap
* if any, items with (-X) - no deductions this time, serve as a warning; please ensure these errors are corrected, as repeating them in future assignments will result in X points being deducted
* in feedback, #N means line number, e.g., 
```text
* #25  global var(s); only constant may be global -10
meaning: see line #25 ; -10 deduction points for the mistake
```
* **class definition style & standards**:  first, member vars  one per line and do not forget to initialize them to state; although private is a default access specifier, still label private members  clearly for better readability; second- functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions, except for static function,  should be const; all functions with more than one statement should be normal (defined externally, outside of the class and below main()), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary;  to promote strong exception guarantee and to reduce redundancy, all setter functions, including c’tors, must call a setter function that sets all member variables
* **strong exception guarantee**: this guarantee acts as a transaction. If the operation successfully completes, all changes take effect; if it fails mid-execution, all partial changes are completely undone. If incoming values are invalid, do not change member variables; provided all member variables have been initialized to a valid state in the class definition and c’tors, the object will always be valid
* **there is no such thing as a partially valid record or set of related data** (let's say, a Creature record in an input file). If any required field (e.g., health, strength) in a record is invalid, then the entire record must be considered invalid. As a developer, your responsibility is to validate the input, not to "fix" it by replacing invalid values with defaults. Doing so changes the user's data and assumes you know what they intended to enter. In general, you should not guess what the user meant or silently modify invalid input. Instead, detect the invalid record and handle it according to the program's requirements (for example, by rejecting it or reporting an error). 
* in  CS 216, **classes must completely manage their own failures**; classes must catch and handle internal errors locally to maintain safe, fully encapsulated invariants. In other words, classes must maintain encapsulation by catching and handling internal exceptions locally. According to the C++ Core Guidelines, an object should achieve the no-fail or strong exception guarantee by rolling back state or swallowing expected failures internally so callers aren't burdened with cleanup.
* moving forward, if a lab submission does not comply with posted instructions, it will result in a loss of an attempt, further feedback will not be provided, and additional attempts will not be granted 
*  **do not split function parameters or assignment statements into multiple lines; it significantly reduces readability; moving forward, the submissions with function parameter lists and/or assignment statements split into multiple lines will be returned ungraded** 
* 338 and like: do not put closing } on the same line as else; else/else-if should be on their own lines;  hard to read -2
*  #81 : poor Id(s)  and/or inconsistent naming convention; ids should be self-documenting and as short as possible; use verbs for functions and nouns for variables; use camel-casing for variables (errorMessage) enum & const should be in upper case with words separated by “_”, e.g., MAX_TERMS, PRINT_RECORDS; pointer variables should start with p( if single)  or pp( if double pointer); flags isValid ( clearly what true would mean); if copying – e.g rhsQueue or scrQueue ( rhs- right-hand side, src – source); do not use IDs and default values that are same as or very similar to C++  keywords and functions; no need to call an array “array” or “arr”- it is obvious; vars should not be called value;  one char identifiers (e.g. j,k, i) are only suitable for loop counters;  -2
```text
 Creature **creatures
```
* redundant convoluted menu implementation; invalid menu choice should be handled by default case of the menu switch;  need only one loop and one switch; create a highly reusable function that takes an error message string as a parameter, clears the failed input, and outputs the error message to be called in the default case of the menu switch -5
```text
do {
	cout<<menu;
	cout>>sortChoice;

switch (sortChoice) {
            case SORT_NAME:
            case SORT_HEALTH:
                sortMenu(names, types, health, strength, numCreatures, sortChoice);
                break;
            case BACK:
			//message about going back to the previous menu
                break;
            default:
                clearCin("Invalid menu choice");
        }
}while (sortChoice!=QUIT) 
```
* 140 and like:  readability; each statement of a function/lopp/if body should be on its own line; only the opening { can be on the same line) -2
* #91: This function is public and can be called with any existing Army object outside of the class, e.g., by main(). What will happen with old data/list if it is called by an existing Army object? Can your class handle it?  To keep it simple, make it private so it cannot be called outside of the class and call it in the Army c’tor.  -5
* who wrote these comments? ; readability, comments; do not state the obvious, do not reiterate/duplicate the code,  AI and the textbooks reiterate/duplicate the code because they try to teach something, as a way of explanation; remove outdated code- it is disrespectful to leave outdated/commented out code; comments are supposed to help understand the code and not create unnecessary clutter; comments should be to the right of the statement whenever possible, not above => those who need to read them – will find them and read, those who do not will go to the next line; "Code never lies, comments sometimes do." - Ron Jeffries”; every C++ programmer knows how const, structs, function prototype looks like; what’s is the goal of excessive ____ and ****? what is the purpose of the comments as this(ese) one(s)? -5
```text
// central configuration: every tunable rule in the program lives here,
// so a range or policy change never requires hunting through the code
// the building block of the system: one creature, fully self-managing;
// Army allocates these on the heap and relies on this class keeping
// itself valid at all times
// the memory owner of the system: holds the heap-allocated creatures and
// carries the no-leak and strong-exception guarantees for the whole program
// the resource the Rule of Three exists to protect
```
*  **#54 and like: do not split function parameters or assignment statements into multiple lines; it significantly reduces readability; moving forward, the submissions with function parameter lists and/or assignment statements split into multiple lines will be returned ungraded** -5
* #58 and like: improper use of explicit; explicit is used for prevention of accidental or unexpected type conversion due to C++ default behavior of one-argument constrictors; in other words, explicit is used to prevent implicit conversion, which is default behavior of one-argument c’tors; https://en.cppreference.com/w/cpp/language/explicit  -2
```text
//e.g.  
Army (string fileName); //one argument c’tor in class Army
void  handleMainMenu( Army myArmy);  // prototype of  a function that belongs to in main()
handleMainMenu(“Natalia”); //  function call in main();Ok du to implicit conversion string to Army object
```
* #166, 260,244 and like: violates strong exception guarantee principle;  the Army and Creature classes must provide a strong exception guarantee: if the operation successfully completes, all changes take effect; if it fails, all partial changes, if any, must be completely undone; If incoming values are invalid, do not change member variables; if happens in a c’tor, provided all member variables have been initialized to a valid state in the class definition, the object will always be valid; e.g. if you tried to withdraw cash from and ATM machine and machine broke, should you balance bet reset to default $0.00? if incoming name is invalid  but size is valid, do not change the name;    -5
* 
* #203 and like: use setfill() to output the same char multiple times  -2
* #243-262 redundant code;  create a function that allocates and returns a double pointer to the allocated array of pointers and use it where appropriate -2
* 246: pointers are very powerful but might become DANGEROUS if not handled properly; ALWAYS initialize them at the time of definition/creation; if you do not know yet where they should be pointing to, initialize them to nullptr -5
* 244 sets size and name before  making sure that allocation is successful -2

```
myClass *pMyObj=nullptr;
int *pList[MAX]={nullptr}; //initializes all pointers to nullptr; no loop needed
int **pAnotherList= new int*[SIZE] {nullptr}; //initializes all pointers to nullptr; no loop needed
```
* 317 and like: use for loop here -2
***

