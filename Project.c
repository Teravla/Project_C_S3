#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Project.h"

#define CHAR_CELL 20



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

void assignColumnIndices(LevelList* list, int numberOfCells) {
    int* distinctValues = malloc(numberOfCells * sizeof(int));
    if (distinctValues == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    } // Soit numberOfCells valeurs differentes
    int count = 0;

    // Recherche de valeurs distinctes et affectation des indices
    for (int i = 0; i < list->maxLevels; i++) {
        Cell* current = list->head->next[i];
        while (current != NULL) {
            int value = current->value;
            int found = 0;

            // Verification si la valeur existe dejà dans distinctValues
            for (int j = 0; j < count; j++) {
                if (distinctValues[j] == value) {
                    found = 1;
                    break;
                }
            }
            
            // Si la valeur n'existe pas, elle est ajoutee à distinctValues
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


int CountCaracterInBelowLevels(LevelList* list, int level) {
    int count = 0;

    // Vérifie chaque niveau en dessous de level
    for (int i = 0; i < level; i++) {
        Cell* current = list->head->next[i];

        // Parcours les cellules au niveau i
        while (current != NULL) {
            
            int value = current->value;

            // Calcule le nombre de caractères dans la valeur de la cellule
            int numDigits = 0;
            while (value%10 >= 1) {
                value /= 10;
                numDigits++;
            }

            // Incrémente count en fonction du nombre de caractères
            count = (numDigits + 1);

            current = current->next[i];
        }
    }
    
    return count;
}



void printLevel(LevelList* list, int level) {
    printf("[ list head_%d | @ - ] --", level);

    Cell* current = list->head->next[level];
    int maxCols = maxColumns(list);

    int numberCaracterInBelowLevels = CountCaracterInBelowLevels(list, level);

    if (current == NULL) {
        //printf("%d", numberCaracterInBelowLevels);
        for (int i = 0; i < CHAR_CELL * maxCols + numberCaracterInBelowLevels; i++) {
            printf("-");
        }
        printf("> NULL\n");
        return;
    }

    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = -1;
    
    int char_cell = CHAR_CELL + numberCaracterInBelowLevels;

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
                printf("%d", numberCaracterInBelowLevels);
                for (int j = 0; j < char_cell; j++) {
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

int countCells(LevelList* list) {
    int count = 0;
    Cell* currentCell = list->head;

    while (currentCell != NULL) {
        count++;
        currentCell = currentCell->next[0];
    }

    return count;
}



/* ------------------------------------------------------- */
/* ---------------  FONCTIONS DE RECHERCHE --------------- */
/* ------------------------------------------------------- */


// Fonction pour rechercher une valeur dans la liste en utilisant tous les niveaux
SearchResult searchAllLevels(LevelList* list, int value) {
    SearchResult result = {NULL, -1, -1};  // Initialisation des valeurs par defaut

    for (int i = list->maxLevels - 1; i >= 0; i--) {
        Cell* current = list->head->next[i];
        while (current != NULL) {
            if (current->value == value) {
                // Stocker les resultats et la hauteur
                result.cell = current;
                result.columnIndex = current->column;
                result.height = i + 1;  // Hauteur est indexee à partir de 1
                return result;
            } else if (current->value < value) {
                break;  // Passer au niveau inferieur si la valeur actuelle est inferieure à la recherche
            }
            current = current->next[i];
        }
    }

    return result;  // La valeur n'a pas ete trouvee dans la liste
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
    SearchResult resultAllLevels = searchAllLevels(list, value);
    end = clock();
    double timeAllLevels = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Affichage des resultats
    if (resultLevel0 != NULL) {
        printf("Level 0 Search: Found value %d at column index %d\n", resultLevel0->value, resultLevel0->column);
    } else {
        printf("Level 0 Search: Value not found\n");
    }
    printf("Time using Level 0: %f seconds\n", timeLevel0);

    if (resultAllLevels.cell != NULL) {
        printf("All Levels Search: Found value %d at column index %d, height %d\n", resultAllLevels.cell->value, resultAllLevels.columnIndex, resultAllLevels.height);
    } else {
        printf("All Levels Search: Value not found\n");
    }
    printf("Time using All Levels: %f seconds\n", timeAllLevels);
}


/* ------------------------------------------------------- */
/* ----------------GENERATION ALEATOIRE ------------------ */
/* ------------------------------------------------------- */



LevelList* createRandomLevelList(int numLevels) {
    LevelList* newList = createLevelList(numLevels);
    return newList;
}

// Fonction pour remplir le tableau avec n valeurs aleatoires
void fillTableWithRandomValues(LevelList* list, int n) {
    printf("Remplissage du tableau avec %d valeurs aleatoires :\n", n);

    srand(time(NULL));  // Initialisation de la graine pour la generation aleatoire

    // Remplissage du tableau avec n valeurs aleatoires
    for (int i = 0; i < n; i++) {
        int value = rand() % n;  // Valeur aleatoire entre 0 et 99
        int levels = rand() % list->maxLevels + 1;  // Niveau aleatoire entre 1 et maxLevels

        Cell* newCell = createCell(value, levels);
        insertAtHead(list, newCell);
    }
}



/* ------------------------------------------------------- */
/* ------------------------  MAIN ------------------------ */
/* ------------------------------------------------------- */



int main() {


    int option;

    printf("Bienvenue sur le programme de recherche d'une valeur dans une liste.\n");
    printf("Vous pouvez choisir entre trois options:\n");

    printf("1. Rechercher une valeur dans une liste a partir de cellules predefinies\n");
    printf("2. Rechercher une valeur dans une liste a partir de cellules que vous choississez\n");
    printf("3. Rechercher une valeur dans une liste a partir de n cellules aleatoires. Vous choississez 'n'.\n");

    printf("Voulez-vous utiliser le schema par defaut (0), le schema manuel (1) ou l'utilisation d'une liste de n valeurs (2) ? (0, 1 ou 2) : ");
    //scanf("%d", &option);
    option = 0; // /!\ TEST VALUE /!\ // 
    printf("\n\n");
    

    if (option == 0) {
        // Utiliser le schema par defaut
        LevelList* myList = createLevelList(5);

        Cell* cell1 = createCell(1, 3);
        Cell* cell2 = createCell(2, 3);
        Cell* cell3 = createCell(30, 2);
        Cell* cell4 = createCell(5, 4);
        Cell* cell5 = createCell(4, 3);

        insertAtHead(myList, cell1);
        insertAtHead(myList, cell2);
        insertAtHead(myList, cell3);
        insertAtHead(myList, cell4);
        insertAtHead(myList, cell5);

        int numberOfCells = countCells(myList);
        assignColumnIndices(myList, numberOfCells);
        printAllLevels(myList);
        printf("\n");

        int searchValue = 10;  // Modifier la valeur à rechercher
        compareSearchTimes(myList, searchValue);

        free(myList);  // Liberer la memoire allouee à la fin de la fonction
    } else if (option == 1) {
        // Saisir les valeurs depuis l'utilisateur
        int numLevels;

        printf("Combien de niveaux souhaitez-vous creer ?");
        scanf("%d", &numLevels);


        if (numLevels <= 0) {
            printf("Nombre de niveaux invalide.\n");
            return 1;
        }

        LevelList* myList = createLevelList(numLevels);

        int numCells;

        printf("Combien de cellules souhaitez-vous creer ? ");
        scanf("%d", &numCells);

        if (numCells <= 0) {
            printf("Nombre de cellules invalide.\n");
            free(myList);
            return 1;
        }

        for (int i = 0; i < numCells; i++) {
            int value, levels;

            printf("Valeur et niveau de la cellule %d (separes par un espace) : ", i + 1);
            scanf("%d %d", &value, &levels);

            if (levels <= 0 || levels > numLevels) {
                printf("Niveau invalide pour la cellule %d.\n", i + 1);
                free(myList);
                return 1;
            }

            Cell* newCell = createCell(value, levels);
            insertAtHead(myList, newCell);
        }

        int numberOfCells = countCells(myList);
        assignColumnIndices(myList, numberOfCells);
        printAllLevels(myList);

        int searchValue = 10;  // Modifier la valeur à rechercher
        compareSearchTimes(myList, searchValue);

        free(myList);
    } else if (option == 2) {
        int numLevels;

        printf("Combien de niveaux souhaitez-vous creer ? ");
        scanf("%d", &numLevels);

        if (numLevels <= 0) {
            printf("Nombre de niveaux invalide.\n");
            return 1;
        }

        LevelList* randomList = createRandomLevelList(numLevels);

        int numRandomValues;
        printf("Combien de valeurs aleatoires souhaitez-vous ajouter ? ");
        scanf("%d", &numRandomValues);

        if (numRandomValues <= 0) {
            printf("Nombre de valeurs aleatoires invalide.\n");
            free(randomList);
            return 1;
        }

        fillTableWithRandomValues(randomList, numRandomValues);

        int numberOfCells = countCells(randomList);
        assignColumnIndices(randomList, numberOfCells);
        printAllLevels(randomList);

        int searchValue = 10;  // Modifier la valeur à rechercher
        compareSearchTimes(randomList, searchValue);

        free(randomList);
    } else {
        printf("Option invalide.\n");
        return 1;
    }

    return 0;
}