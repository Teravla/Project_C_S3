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
Cell* createCell(int value, int levels);
LevelList* createLevelList(int maxLevels);
void insertAtHead(LevelList* list, Cell* newCell);
int maxColumns(LevelList* list);
void assignColumnIndices(LevelList* list);
void printLevel(LevelList* list, int level);
void printAllLevels(LevelList* list);
SearchResult searchAllLevels(LevelList* list, int value); // Correction ici
void compareSearchTimes(LevelList* list, int value);

#endif
