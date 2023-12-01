#ifndef PROJECT_H
#define PROJECT_H

// Structure declarations
typedef struct Cell {
    //Agenda
    char name[100];
    char surname[100];
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    //Part 1
    int value;
    
    int levels;
    int column;
    struct Cell** next;
} Cell;

typedef struct LevelList {
    int maxLevels;
    Cell* head;
} LevelList;

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
LevelList* addContact(LevelList* myList);
LevelList* deleteContact(LevelList* myList);
void searchContact(LevelList* list);
void searchMeeting(LevelList* list);
void searchMeetingByDate(LevelList* list);
void searchMeetingByHour(LevelList* list);

int countCharName(LevelList* list, int column);
int countCharSurname(LevelList* list, int column);
void capitalizeString(char* str);

void printLevelChar(LevelList* list, int level, int numberOfCells);
void printAllLevelsChar(LevelList* list, int numberOfCells);

//Main
LevelList* listPreset();
LevelList* listManual();
LevelList* listAlea();

void addCell(LevelList* myList);
void deleteCell(LevelList* myList);
void searchCell(LevelList* list);
void printOneLevel(LevelList* list);

#endif
