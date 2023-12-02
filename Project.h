#ifndef PROJECT_H
#define PROJECT_H

// Structure declarations
typedef struct Cell {
    int value;
    int levels;
    int column;
    struct Cell** next;
} Cell;
typedef struct LevelList {
    int maxLevels;
    Cell* head;
} LevelList;

//Partie III
typedef struct Meeting {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} Meeting;

typedef struct Node {
    Meeting meeting;
    struct Node* next;
} Node;

typedef struct Agenda {
    char name[100];
    char surname[100];
    Node* meetings; // Liste chaînée de réunions
    
    int level;
    int column;

    struct Agenda** next;
} Agenda;

typedef struct LevelAgenda {
    int maxLevels;
    Agenda* head;
} LevelAgenda;


typedef struct {
    Cell* cell;
    int columnIndex;
    int height;
} SearchResult;

// Function declarations

//List and cell creation
Cell* createCell(int value, int levels);
LevelList* createLevelList(int maxLevels);
void insertAtHead(LevelList* list, Cell* newCell);
void addCell(LevelList* myList);

//Display list
int countCells(LevelList* list);
int numberCaracterInCell(LevelList* list, int column);
void printArray(int* array, int size);
int SumCharLenght(int* array, int size);
int tabcharlength(LevelList* list, int column);
Cell* obtenirCellule(LevelList* list, int column);
int maxColumns(LevelList* list);
void assignColumnIndices(LevelList* list, int numberOfCells);
void printLevel(LevelList* list, int level, int numberOfCells);
void printAllLevels(LevelList* list, int numberOfCells);

// Generation aleatoire
LevelList* createRandomLevelList(int numLevels);
void fillTableWithRandomValues(LevelList* list, int n);


// Recherche 
SearchResult searchAllLevels(LevelList* list, int value); // Correction ici
void compareSearchTimes(LevelList* list, int value);

//Agenda
void insertMeeting(Node** meetings, Meeting* newMeeting);
LevelAgenda* addContact(LevelAgenda* myList);
LevelAgenda* deleteContact(LevelAgenda* myList);
void searchContact(LevelAgenda* list);
void searchMeeting(LevelAgenda* list);
void searchMeetingByDate(LevelAgenda* list);
void searchMeetingByHour(LevelAgenda* list);
LevelAgenda* addMeeting(LevelAgenda* list);
int meetingExists(LevelAgenda* list, Agenda* meeting);

int countCharName(LevelAgenda* list, int column);
int countCharSurname(LevelAgenda* list, int column);
void capitalizeString(char* str);
void insertAtHeadAgenda(LevelAgenda* list, Agenda* newAgenda);
LevelAgenda* createLevelAgenda(int maxLevels);
int countCellsAgenda(LevelAgenda* list);
void assignColumnIndicesAgenda(LevelAgenda* list, int numberOfCells);

int maxColumnsAgenda(LevelAgenda* list);
Agenda* obtenirCelluleAgenda(LevelAgenda* list, int column);
void printAllLevelsAgenda(LevelAgenda* list, int numberOfCells);
void printLevelAgenda(LevelAgenda* list, int level, int numberOfCells);

//Main
LevelList* listPreset();
LevelList* listManual();
LevelList* listAlea();

void addCell(LevelList* myList);
void deleteCell(LevelList* myList);
void searchCell(LevelList* list);
void printOneLevel(LevelList* list);

#endif
