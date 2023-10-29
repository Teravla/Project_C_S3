#ifndef PROJECT_H
#define PROJECT_H

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

Cell* createCell(int value, int levels);
LevelList* createLevelList(int maxLevels);
void insertAtHead(LevelList* list, Cell* newCell);
int maxColumns(LevelList* list);
void assignColumnIndices(LevelList* list);
void printLevel(LevelList* list, int level);
void printLevelSimplified(LevelList* list, int level);
void printAllLevels(LevelList* list);

#endif
