#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>  

using namespace std;

struct Guest {
    string lastName;
    string firstName;
    string middleName;
    int roomNumber;
};

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    vector<Guest> guests;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;

    void saveHelper(TrieNode* node, ofstream& outFile, string currentPrefix) {
        if (node->isEndOfWord) {
            for (const auto& guest : node->guests) {
                outFile << currentPrefix << " " << guest.firstName << " " << guest.middleName << " " << guest.roomNumber << "\n";
            }
        }

        for (const auto& child : node->children) {
            saveHelper(child.second, outFile, currentPrefix + child.first);
        }
    }

    void loadHelper(const string& line) {
        istringstream iss(line);
        Guest guest;
        iss >> guest.lastName >> guest.firstName >> guest.middleName >> guest.roomNumber;
        addGuest(guest);
    }

public:
    Trie() : root(new TrieNode()) {}

    void addGuest(const Guest& guest) {
        TrieNode* currentNode = root;
        for (char ch : guest.lastName) {
            if (!currentNode->children[ch]) {
                currentNode->children[ch] = new TrieNode();
            }
            currentNode = currentNode->children[ch];
        }
        currentNode->isEndOfWord = true;
        currentNode->guests.push_back(guest);
    }

    bool findGuest(const string& lastName, vector<Guest>& result) {
        TrieNode* currentNode = root;
        for (char ch : lastName) {
            if (!currentNode->children[ch]) {
                return false;
            }
            currentNode = currentNode->children[ch];
        }

        if (currentNode->isEndOfWord) {
            result = currentNode->guests;
            return true;
        }
        return false;
    }

    void deleteGuest(const string& lastName, const string& firstName, const string& middleName) {
        TrieNode* currentNode = root;
        for (char ch : lastName) {
            if (!currentNode->children[ch]) return;
            currentNode = currentNode->children[ch];
        }
        if (currentNode->isEndOfWord) {
            auto& guests = currentNode->guests;
            guests.erase(remove_if(guests.begin(), guests.end(), [&](const Guest& g) {
                return g.firstName == firstName && g.middleName == middleName;
                }), guests.end());

            if (guests.empty()) {
                currentNode->isEndOfWord = false;
            }
        }
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            saveHelper(root, outFile, "");
            outFile.close();
        }
        else {
            cerr << "Error opening file for writing.\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                loadHelper(line);
            }
            inFile.close();
        }
        else {
            cerr << "Error opening file for reading.\n";
        }
    }
};

void displayMenu() { 
    cout << "1. Add Guest\n";
    cout << "2. Delete Guest\n";
    cout << "3. Edit Guest\n";
    cout << "4. Find Guest by Last Name\n";
    cout << "5. Save Data to File\n";
    cout << "6. Load Data from File\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

Guest inputGuestInfo() {
    Guest guest;
    cout << "Enter last name: ";
    cin >> guest.lastName;
    cout << "Enter first name: ";
    cin >> guest.firstName;
    cout << "Enter middle name: ";
    cin >> guest.middleName;
    cout << "Enter room number: ";
    while (!(cin >> guest.roomNumber)) {
        cin.clear();  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
        cout << "Invalid input. Please enter a valid room number: ";
    }
    return guest;
}

int main() {
    Trie trie;
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            Guest guest = inputGuestInfo();
            trie.addGuest(guest);
            break;
        }
        case 2: {
            string lastName, firstName, middleName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter middle name: ";
            cin >> middleName;
            trie.deleteGuest(lastName, firstName, middleName);
            break;
        }
        case 3: {
            string lastName, firstName, middleName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter middle name: ";
            cin >> middleName;
            trie.deleteGuest(lastName, firstName, middleName);
            Guest newGuest = inputGuestInfo();
            trie.addGuest(newGuest);
            break;
        }
        case 4: {
            string lastName;
            cout << "Enter last name: ";
            cin >> lastName;
            vector<Guest> result;
            if (trie.findGuest(lastName, result)) {
                for (const auto& guest : result) {
                    cout << "Found: " << guest.lastName << " " << guest.firstName << " " << guest.middleName << " Room: " << guest.roomNumber << "\n";
                }
            }
            else {
                cout << "No guest found with last name " << lastName << "\n";
            }
            break;
        }
        case 5: {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            trie.saveToFile(filename);
            break;
        }
        case 6: {
            string filename;
            cout << "Enter filename to load: ";
            cin >> filename;
            trie.loadFromFile(filename);
            break;
        }
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}
