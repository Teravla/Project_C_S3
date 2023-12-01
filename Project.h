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

typedef struct {
    Cell* cell;
    int columnIndex;
    int height;
} SearchResult;

// Function declarations
int countCells(LevelList* list);
int CountCaracterInBelowLevels(LevelList* list, int columnToCount);
int numberCaracterInCell(LevelList* list, int column);
int countCharPreviousCell(LevelList* list, int column);
int nbCharPreviousCells(LevelList* list, int column);
Cell* createCell(int value, int levels);
int CountColumn(LevelList* list, int column);
LevelList* createLevelList(int maxLevels);
int SumCharLenght(int* array, int size);
void insertAtHead(LevelList* list, Cell* newCell);
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

#endif
