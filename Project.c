#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Project.h"

#define CHAR_CELL 21



/* ------------------------------------------------------- */
/* ----------------- CREATION DU TABLEAU ----------------- */
/* ------------------------------------------------------- */



Cell* createCell(int value, int levels) {
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    newCell->value = value;
    newCell->levels = levels;
    newCell->next = (Cell**)malloc(levels * sizeof(Cell*));
    for (int i = 0; i < levels; i++) {
        newCell->next[i] = NULL;
    }
    return newCell;
}

LevelList* createLevelList(int maxLevels) {
    LevelList* newList = (LevelList*)malloc(sizeof(LevelList));
    newList->maxLevels = maxLevels;
    newList->head = createCell(0, maxLevels);
    return newList;
}

void insertAtHead(LevelList* list, Cell* newCell) {
    int maxLevels = list->maxLevels;
    int newCellLevels = newCell->levels;

    if (newCellLevels > maxLevels) {
        LevelList* extendedList = createLevelList(newCellLevels);

        for (int i = 0; i < maxLevels; i++) {
            Cell* current = list->head->next[i];
            while (current != NULL) {
                Cell* next = current->next[i];
                insertAtHead(extendedList, current);
                current = next;
            }
        }

        free(list->head);
        free(list);
        *list = *extendedList;
        maxLevels = newCellLevels; // Mise à jour du nombre maximal de niveaux
    }

    for (int i = 0; i < newCellLevels; i++) {
        Cell* current = list->head;
        while (current->next[i] != NULL && current->next[i]->value < newCell->value) {
            current = current->next[i];
        }

        if (current->next[i] != NULL && current->next[i]->value == newCell->value) {
            printf("Duplicate value detected. Cannot insert.\n");
            return;
        }

        newCell->next[i] = current->next[i];
        current->next[i] = newCell;
    }
}

int maxColumns(LevelList* list) {
    int maxCols = 0;
    for (int i = 0; i < list->maxLevels; i++) {
        Cell* current = list->head->next[i];
        int cols = 0;
        while (current != NULL) {
            cols++;
            current = current->next[i];
        }
        if (cols > maxCols) {
            maxCols = cols;
        }
    }
    return maxCols;
}

void assignColumnIndices(LevelList* list) {
    int distinctValues[100] = {0}; // Supposons un maximum de 100 valeurs distinctes
    int count = 0;

    // Recherche de valeurs distinctes et affectation des indices
    for (int i = 0; i < list->maxLevels; i++) {
        Cell* current = list->head->next[i];
        while (current != NULL) {
            int value = current->value;
            int found = 0;

            // Vérification si la valeur existe déjà dans distinctValues
            for (int j = 0; j < count; j++) {
                if (distinctValues[j] == value) {
                    found = 1;
                    break;
                }
            }
            
            // Si la valeur n'existe pas, elle est ajoutée à distinctValues
            if (!found) {
                distinctValues[count] = value;
                count++;
            }

            current = current->next[i];
        }
    }

    // Tri des valeurs distinctes
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (distinctValues[i] > distinctValues[j]) {
                int temp = distinctValues[i];
                distinctValues[i] = distinctValues[j];
                distinctValues[j] = temp;
            }
        }
    }

    // Attribution des indices aux valeurs distinctes
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < list->maxLevels; j++) {
            Cell* current = list->head->next[j];
            while (current != NULL) {
                if (current->value == distinctValues[i]) {
                    current->column = i;
                }
                current = current->next[j];
            }
        }
    }
}

void printLevel(LevelList* list, int level) {
    printf("[ list head_%d | @ - ] --", level);

    Cell* current = list->head->next[level];
    int maxCols = maxColumns(list);

    if (current == NULL) {
        for (int i = 0; i < CHAR_CELL * maxCols +1; i++) {
            printf("-");
        }
        printf("> NULL\n");
        return;
    }

    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = -1;

    while (current != NULL) {
        for (int i = lastPrinted + 1; i < current->column; i++) {
            for (int j = 0; j < CHAR_CELL; j++) {
                printf("-");
            }
            printedColumns[i] = 1;
        }

        printf("> [ %d (%d) | @ - ]", current->value, current->column);

        if (current->next[level] != NULL && current->next[level]->column != current->column) {
            printf(" --");
        } else {
            printf(" ");
            int remainingColumns = maxCols - (current->column + 1);
            for (int i = 0; i < remainingColumns; i++) {
                for (int j = 0; j < CHAR_CELL; j++) {
                    printf("-");
                }
            }
            printf("---> NULL\n");
            break;
        }

        printedColumns[current->column] = 1;
        lastPrinted = current->column;
        current = current->next[level];
    }

    free(printedColumns);
}

void printAllLevels(LevelList* list) {
    for (int i = 0; i < list->maxLevels; i++) {
        printLevel(list, i);
    }
}



/* ------------------------------------------------------- */
/* ---------------  FONCTIONS DE RECHERCHE --------------- */
/* ------------------------------------------------------- */



// Fonction pour rechercher une valeur dans la liste en utilisant tous les niveaux
Cell* searchAllLevels(LevelList* list, int value) {
    for (int i = list->maxLevels - 1; i >= 0; i--) {
        Cell* current = list->head->next[i];
        while (current != NULL) {
            if (current->value == value) {
                return current;
            } else if (current->value < value) {
                break;  // Passer au niveau inférieur si la valeur actuelle est inférieure à la recherche
            }
            current = current->next[i];
        }
    }
    return NULL;  // La valeur n'a pas été trouvée dans la liste
}

// Fonction pour comparer les temps de recherche entre le niveau 0 et tous les niveaux
void compareSearchTimes(LevelList* list, int value) {
    clock_t start, end;

    // Recherche en utilisant uniquement le niveau 0
    start = clock();
    Cell* resultLevel0 = list->head->next[0];
    while (resultLevel0 != NULL && resultLevel0->value != value) {
        resultLevel0 = resultLevel0->next[0];
    }
    end = clock();
    double timeLevel0 = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Recherche en utilisant tous les niveaux
    start = clock();
    Cell* resultAllLevels = searchAllLevels(list, value);
    end = clock();
    double timeAllLevels = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Affichage des résultats
    if (resultLevel0 != NULL) {
        printf("Level 0 Search: Found value %d\n", resultLevel0->value);
    } else {
        printf("Level 0 Search: Value not found\n");
    }
    printf("Time using Level 0: %f seconds\n", timeLevel0);

    if (resultAllLevels != NULL) {
        printf("All Levels Search: Found value %d\n", resultAllLevels->value);
    } else {
        printf("All Levels Search: Value not found\n");
    }
    printf("Time using All Levels: %f seconds\n", timeAllLevels);
}



/* ------------------------------------------------------- */
/* ------------------------  MAIN ------------------------ */
/* ------------------------------------------------------- */



int main() {
    

    /* PARTIE I */

    LevelList* myList = createLevelList(5);

    Cell* cell1 = createCell(10, 3);
    Cell* cell2 = createCell(20, 3);
    Cell* cell3 = createCell(30, 2);
    Cell* cell4 = createCell(15, 4);

    insertAtHead(myList, cell1);
    insertAtHead(myList, cell2);
    insertAtHead(myList, cell3);
    insertAtHead(myList, cell4);

    assignColumnIndices(myList);


    printAllLevels(myList);

    int searchValue = 10;  // Modifier la valeur à rechercher
    compareSearchTimes(myList, searchValue);

    free(myList);  // Libérer la mémoire allouée à la fin de la fonction


    return 0;  // Retourne 0 pour indiquer une exécution sans erreur
}
