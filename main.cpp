#include <iostream>
#include <vector>
#include <conio.h>
#include <sstream>
#include <fstream>

using namespace std;

struct PhoneBook {
    short id;
    string firstName, lastName, phoneNo, email, address;
};

void importContactsFromFile(vector<PhoneBook> &contacts, unsigned short &lastContactID, const char &DELIMITER);
PhoneBook splitLineOfText(string stringToSplit, const char &DELIMITER);
void addContact(vector<PhoneBook> &contacts, unsigned short &lastContactID);
void updateDBFile(vector<PhoneBook> &contacts, unsigned short &lastContactID);
void findContacts(vector<PhoneBook> &contacts, unsigned short &lastContactID, bool &searchModeSwitch);
void displayContacts(vector<PhoneBook> &contacts);
void deleteContact(vector<PhoneBook> &contacts, unsigned short &lastContactID);
void editContact(vector<PhoneBook> &contacts, unsigned short &lastContactID);

int main(){
    system("chcp 1250");

    vector<PhoneBook> contacts;
    char choice = '0';
    unsigned short lastContactID = 0;
    const char DELIMITER = '|';
    bool searchModeSwitch = false, FirstNameSearch = true, LastNameSearch = false;

    importContactsFromFile(contacts, lastContactID, DELIMITER);

    while (1) {
        system("cls");
        cout << "KSIAZKA ADRESOWA" << '\n';
        cout << "1. Dodaj adresata" << '\n';
        cout << "2. Wyszukaj po imieniu" << '\n';
        cout << "3. Wyszukaj po nazwisku" << '\n';
        cout << "4. Wyswietl wszystkich adresatow" << '\n';
        cout << "5. Usun adresata" << '\n';
        cout << "6. Edytuj adresata" << '\n';
        cout << "9. Zakoncz program" << '\n';
        cout << "Twoj wybor: ";
        choice = getch();

        switch (choice) {
            case '1':   addContact(contacts, lastContactID);
                        break;
            case '2':   searchModeSwitch = FirstNameSearch;
                        findContacts(contacts, lastContactID, searchModeSwitch);
                        break;
            case '3':   searchModeSwitch = LastNameSearch;
                        findContacts(contacts, lastContactID, searchModeSwitch);
                        break;
            case '4':   displayContacts(contacts);
                        system("pause");
                        break;
            case '5':   deleteContact(contacts, lastContactID);
                        break;
            case '6':   editContact(contacts, lastContactID);
                        break;
            case '9':   exit(0);
                        break;
            default:    cout << '\n' << "Niepoprawny wybor. Spróbuj ponownie." << '\n';
                        system("pause");
        }
    }

    return 0;
}

void importContactsFromFile(vector<PhoneBook> &contacts, unsigned short &lastContactID, const char &DELIMITER) {
    fstream dbFile;
    dbFile.open("contacts.txt", ios::in);

    if (!dbFile.good()) {
        system("cls");
        cout << '\n' << "Brak pliku z baza kontaktow." << '\n' << '\n';
        system("pause");
    }

    //Lines in text file count from 1, NOT from 0!
    unsigned short lineOfTextCount = 1;
    string lineOfText;

    while (getline(dbFile, lineOfText)) {
        contacts.emplace_back(splitLineOfText(lineOfText, DELIMITER));
        lineOfTextCount++;
    }

    dbFile.close();
    lastContactID = lineOfTextCount - 1;
}

PhoneBook splitLineOfText(string lineOfText, const char& DELIMITER) {
    stringstream ss(lineOfText);
    string singleValueFromLineOfText;
    vector<string> splittedStrings;
    PhoneBook person;

    while (getline(ss, singleValueFromLineOfText, DELIMITER)) {
       splittedStrings.emplace_back(singleValueFromLineOfText);
    }

    person.id = stoi(splittedStrings.at(0));
    person.firstName = splittedStrings.at(1);
    person.lastName = splittedStrings.at(2);
    person.phoneNo = splittedStrings.at(3);
    person.email = splittedStrings.at(4);
    person.address = splittedStrings.at(5);

    return person;
}

void addContact(vector<PhoneBook> &contacts, unsigned short &lastContactID) {
    PhoneBook person;

    system("cls");
    person.id = lastContactID + 1;
    lastContactID++;
    cout << "Podaj imie: ";
    cin >> person.firstName;
    cout << "Podaj nazwisko: ";
    cin >> person.lastName;
    cout << "Podaj nr telefonu: ";
    cin >> person.phoneNo;
    cout << "Podaj adres email: ";
    cin >> person.email;
    cout << "Podaj pelny adres (ulica [nr], kod pocztowy, miasto): ";
    cin.sync();
    getline(cin, person.address);

    contacts.emplace_back(person);
    updateDBFile(contacts, lastContactID);
}


void findContacts(vector<PhoneBook> &contacts, unsigned short &lastContactID, bool &searchModeSwitch) {
    bool contactFound = false;
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();
    string newValue = "", oldValue = "";

    system("cls");

    //search by firstName
    if (searchModeSwitch == true) {
        cout << "Podaj imie do wyszukania: ";
    }
    //search by lastName
    else {
        cout << "Podaj nazwisko do wyszukania: ";
    }

    cin >> newValue;

    for (itr; itr != lastContactPosition; ++itr) {
        oldValue = (searchModeSwitch) ? itr->firstName : itr->lastName;

        if (oldValue == newValue) {
            cout << itr->id << " | " << itr->firstName << " | " << itr->lastName << " | "
                 << itr->phoneNo << " | " << itr->email <<  " | " << itr->address << " | " << '\n';
            contactFound = true;
        }
    }

    if (!contactFound) {
        cout << '\n' << "Zaden kontakt nie spelnia kryterium wyszukiwania." << '\n' << '\n';
        system("pause");
    } else {
        cout << '\n';
        system("pause");
    }
}

void displayContacts(vector<PhoneBook> &contacts) {
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();

    system("cls");

    for (itr; itr != lastContactPosition; ++itr ) {
        cout << itr->id << " | " << itr->firstName << " | " << itr->lastName << " | "
             << itr->phoneNo << " | " << itr->email <<  " | " << itr->address << " | " << '\n';
    }
    cout << '\n';
}


void updateDBFile(vector<PhoneBook> &contacts, unsigned short &lastContactID) {
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();
    fstream dbFile;
    dbFile.open("contacts.txt", ios::out);

    for (itr; itr != lastContactPosition; ++itr) {
        dbFile << itr->id << '|' << itr->firstName << '|' << itr->lastName << '|'
               << itr->phoneNo << '|' << itr->email <<  '|' << itr->address << '|' << '\n';
        lastContactID = itr->id;
    }

    dbFile.close();
}

void deleteContact(vector<PhoneBook> &contacts, unsigned short &lastContactID) {
    short choice;
    char choiceConfirmed;
    bool contactFound = false;
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();

    displayContacts(contacts);

    cout << '\n' << "Podaj ID kontaktu, ktory chcesz usunac: ";
    cin >> choice;

    for (itr; itr != lastContactPosition; ++itr) {
        if (itr->id == choice) {
            contactFound = true;
            cout << "Czy na pewno chcesz usunac ten kontakt? Potwierdz: [t/n]?";
            choiceConfirmed = getch();

            if (choiceConfirmed == 't') {
                itr = contacts.erase(itr);
                cout << '\n' << "Kontakt zostal pomyslnie usuniety." << '\n';
                system("pause");
                break;
            }
        }
    }

    if (contactFound) {
        updateDBFile(contacts, lastContactID);
    } else {
        cout << "Brak kontaktu o podanym ID." << '\n';
        system("pause");
    }
}

void editContact(vector<PhoneBook> &contacts, unsigned short &lastContactID) {
    char choice = ' ';
    short contactIDToEdit = 0;
    bool IDSearchSuccessful = false;
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();

    displayContacts(contacts);

    cout << "Podaj ID kontaktu, ktory chcesz edytowac: ";
    cin >> contactIDToEdit;

    system("cls");
    cout << "Ktora informacje chcesz zmienic?:" << '\n';
    cout << "1 - imie" << '\n';
    cout << "2 - nazwisko" << '\n';
    cout << "3 - numer telefonu" << '\n';
    cout << "4 - email" << '\n';
    cout << "5 - adres" << '\n';
    cout << "6 - powrot do menu" << "\n\n";
    choice = getch();
    cout << "Twoj wybor: " << choice << '\n';

    for (itr; itr != lastContactPosition; ++itr) {
        if (itr->id == contactIDToEdit) {
            IDSearchSuccessful = true;
            switch (choice) {
                case '1':   cout << "Podaj nowe imie: ";
                            cin >> itr->firstName;

                            break;
                case '2':   cout << "Podaj nowe nazwisko: ";
                            cin >> itr->lastName;
                            break;
                case '3':   cout << "Podaj nowy numer telefonu: ";
                            cin >> itr->phoneNo;
                            break;
                case '4':   cout << "Podaj nowy email: ";
                            cin >> itr->email;
                            break;
                case '5':   cout << "Podaj nowe adres: ";
                            cin >> itr->address;
                            break;
                case '6':   break;
                default:    cout << '\n' << "Niepoprawny wybor. Spróbuj ponownie." << '\n';
                            system("pause");
            }
        }
    }

    if (!IDSearchSuccessful) {
        cout << '\n' << "Brak kontaktu o podanym ID." << '\n';
        system("pause");
    }

    updateDBFile(contacts, lastContactID);
}
