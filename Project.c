#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "Project.h"

#define CHAR_CELL 18



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
        maxLevels = newCellLevels; // Mise a jour du nombre maximal de niveaux
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


void addCell(LevelList* myList) {

    printf("\nVous avez choisi d'ajouter une nouvelle cellule.\n");

    int choiceLevel;
    int choiceValue;

    // Creer un tableau avec toutes les valeurs de la liste
    int maxCols = maxColumns(myList);
    int* allValues = (int*)malloc(maxCols * sizeof(int));

    if (allValues == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour le tableau des valeurs\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    Cell* current = myList->head->next[0];

    while (current != NULL) {
        allValues[index++] = current->value;
        current = current->next[0];
    }


    // Saisir le niveau de la nouvelle cellule
    printf("On rappelle le niveau max de la liste : --> %d <--\n", myList->maxLevels);
    do {
        printf("Niveau de la nouvelle cellule : ");
        scanf("%d", &choiceLevel);
    } while (choiceLevel <= 0 || choiceLevel > myList->maxLevels);

    printf("On rappelle les valeurs de la liste qui ne peuvent pas etre prises : ");
    for (int i = 0; i < index; i++) {
        printf("%d ", allValues[i]);
    }
    printf("\n");

    // Saisir la valeur de la nouvelle cellule
    do {
        printf("Valeur de la nouvelle cellule : ");
        scanf("%d", &choiceValue);

        // Verifier si la valeur est deja presente dans la liste
        for (int i = 0; i < index; i++) {
            if (allValues[i] == choiceValue) {
                printf("Cette valeur est deja presente dans la liste. Veuillez en choisir une autre.\n");
                choiceValue = -1;  // Reinitialiser la valeur pour forcer la boucle
                break;
            }
        }
    } while (choiceValue <= 0);

    // Creer une nouvelle cellule
    Cell* newCell = createCell(choiceValue, choiceLevel);

    // Inserer la nouvelle cellule a la tête de la liste
    insertAtHead(myList, newCell);

    // // Reinserer toutes les cellules dans la liste
    // for (int i = 0; i < index; i++) {
    //     Cell* existingCell = createCell(allValues[i], choiceLevel);
    //     insertAtHead(myList, existingCell);
    // }

    free(allValues);

    printf("Nouvelle cellule ajoutee avec succes.\n");
    printf("Nouvelle affichage de la liste : \n");

    int numberOfCells = countCells(myList);
    assignColumnIndices(myList, numberOfCells);
    printAllLevels(myList, numberOfCells);
}


// Fonction pour supprimer une cellule de la liste (a adapter selon votre implementation)
void deleteCell(LevelList* myList) {
    printf("\nVous avez choisi de supprimer une cellule.\n");

    int valueToDelete;

    // Creer un tableau avec toutes les valeurs de la liste
    int maxCols = maxColumns(myList);
    int* allValues = (int*)malloc(maxCols * sizeof(int));

    if (allValues == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour le tableau des valeurs\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    Cell* current = myList->head->next[0];

    while (current != NULL) {
        allValues[index++] = current->value;
        current = current->next[0];
    }

    printf("Pour supprimer une cellule, vous devez donner sa valeur : \n");

    // Afficher les valeurs de la liste
    printf("Les valeurs de la liste sont : ");
    for (int i = 0; i < index; i++) {
        printf("%d ", allValues[i]);
    }
    printf("\n");

    // Saisir la valeur de la cellule a supprimer
    do {
        printf("Valeur de la cellule a supprimer : ");
        scanf("%d", &valueToDelete);

        // Verifier si la valeur est dans le tableau des valeurs
        int found = 0;
        for (int i = 0; i < index; i++) {
            if (allValues[i] == valueToDelete) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Cette valeur n'est pas presente dans la liste. Veuillez reessayer.\n");
            valueToDelete = -1;  // Reinitialiser la valeur pour forcer la boucle
        }
    } while (valueToDelete <= 0);

    // Recherche de la cellule a supprimer a tous les niveaux
    current = myList->head;
    Cell* prev = NULL;

    while (current != NULL) {
        if (current->value == valueToDelete) {
            // Mettre a jour les pointeurs pour contourner la cellule a supprimer
            if (prev != NULL) {
                for (int level = 0; level < current->levels; level++) {
                    prev->next[level] = current->next[level];
                }
            } else {
                for (int level = 0; level < current->levels; level++) {
                    myList->head->next[level] = current->next[level];
                }
            }

            free(current);  // Liberer la memoire de la cellule a supprimer
            printf("Cellule supprimee avec succes.\n");
            break;  // Sortir de la boucle des que la cellule est trouvee et supprimee
        }

        prev = current;
        current = current->next[0];
    }

    if (current == NULL) {
        printf("Cellule non trouvee dans la liste.\n");
    }
    

    printf("Nouvelle affichage de la liste : \n");

    int numberOfCells = countCells(myList) - 1;
    printf("Nombre de cellules dans la liste : %d\n", numberOfCells);
    assignColumnIndices(myList, numberOfCells);
    printAllLevels(myList, numberOfCells);

    free(allValues);
}

/* ------------------------------------------------------- */
/* ---------------- AFFICHAGE DU TABLEAU ----------------- */
/* ------------------------------------------------------- */


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
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    } // Soit numberOfCells valeurs differentes
    int count = 0;

    // Recherche de valeurs distinctes et affectation des indices
    for (int i = 0; i < list->maxLevels; i++) {
        Cell* current = list->head->next[i];
        while (current != NULL) {
            int value = current->value;
            int found = 0;

            // Verification si la valeur existe deja dans distinctValues
            for (int j = 0; j < count; j++) {
                if (distinctValues[j] == value) {
                    found = 1;
                    break;
                }
            }
            
            // Si la valeur n'existe pas, elle est ajoutee a distinctValues
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



int numberCaracterInCell(LevelList* list, int column) {
    if (list->head == NULL) {
        // Liste vide
        return 0;
    }

    Cell* current = list->head->next[0]; // Pointe vers la premiere cellule du niveau 0

    // Parcours les cellules du niveau 0 jusqu'a la colonne specifiee
    while (current != NULL && current->column != column) {
        current = current->next[0]; // Passe a la cellule suivante du niveau 0
    }

    if (current != NULL) {
        int value = current->value;

        // Calcule le nombre de caracteres dans la valeur de la cellule
        int count = 0;
        do {
            value /= 10;
            count++;
        } while (value != 0);

        return count;
    } else {
        // La colonne specifiee n'existe pas dans le niveau 0
        return 0;
    }
}

void printArray(int* array, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]");
}

int SumCharLenght(int* array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    return sum;
}

int tabcharlength(LevelList* list, int column) {
    Cell* current = list->head->next[0];

    // Nombre total de colonnes dans la liste
    int totalColumns = 0;
    while (current != NULL) {
        totalColumns++;
        current = current->next[0];
    }

    // Creation du tableau pour stocker les longueurs des caracteres
    int* charLengths = (int*)malloc(totalColumns * sizeof(int));
    if (charLengths == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    current = list->head->next[0];
    int index = 0;

    while (current != NULL && current->column <= column) {
        int value = current->value;

        // Calcul du nombre de caracteres dans la valeur de la cellule
        int numDigits = 0;
        while (value > 0) {
            value /= 10;
            numDigits++;
        }

        charLengths[index] = numDigits;
        index++;

        current = current->next[0];
    }

    // Affichage du tableau
    //printArray(charLengths, index);
    int sum = SumCharLenght(charLengths, index);

    free(charLengths);


    return sum;
}

Cell* obtenirCellule(LevelList* list, int column) {
    Cell* current = list->head->next[0];

    while (current != NULL && current->column != column) {
        current = current->next[0];
    }

    return current;
}


void printLevel(LevelList* list, int level, int numberOfCells) {
    printf("[ list head_%d | @ - ] --", level);

    Cell* current = list->head->next[level];
    int char_cell = 18;
    
    int maxCols = maxColumns(list);
    int* distinctValues = malloc(numberOfCells * sizeof(int));
    if (distinctValues == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    if (current == NULL) {
        // Affichage du nombre total de caracteres pour chaque colonne
        for (int k = 0; k < maxCols; k++) {
            int totalChars = (char_cell + (numberCaracterInCell(list, k) +1));

            // Affichage des tirets
            for (int i = 0; i < totalChars; i++) {
                printf("-");
            }
        }
        printf("-> NULL\n");
        return;
    }



    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = -1;
    

    while (current != NULL) {

        int nbCharLenght = tabcharlength(list, current->column);

        for (int i = lastPrinted + 1; i < current->column; i++) {
            
            int nbCharPreviousCell = numberCaracterInCell(list, i);

            for (int j = 0; j < char_cell + nbCharPreviousCell +1 ; j++) {
                printf("-");
            }
            printedColumns[i] = 1;
        }

        printf("> [ %d (%d) | @ - ]", current->value, current->column);

        if (current->next[level] != NULL && current->next[level]->column != current->column) {
            printf(" --");
        } else {
            printf(" ");
            int remainingColumns = maxCols - (current->column +1);
            
            for (int k = 0; k < remainingColumns; k++) {
                // Obtenez la cellule correspondant a la colonne (fin de la liste - k)
                Cell* celluleCourante = obtenirCellule(list, maxCols - k - 1);
                
                int nbCharCurrentCell = numberCaracterInCell(list, celluleCourante->column);
                int totalChars = char_cell + nbCharCurrentCell+1;
                
                // Affichage des tirets
                for (int i = 0; i < totalChars; i++) {
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

void printAllLevels(LevelList* list, int numberOfCells) {
    for (int i = 0; i < list->maxLevels; i++) {
        printLevel(list, i, numberOfCells);
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


void printOneLevel(LevelList* list) {
    printf("\n\nVous avez choisi d'afficher un seul niveau\n\n");

    // Afficher le nombre de niveaux dans la liste
    printf("Nombre de niveaux dans la liste : %d\n", list->maxLevels);

    int levelToPrint;

    // Demander a l'utilisateur le niveau a afficher
    do {
        printf("Veuillez saisir le niveau a afficher (entre 1 et %d) : ", list->maxLevels);
        scanf("%d", &levelToPrint);
    } while (levelToPrint < 1 || levelToPrint > list->maxLevels);

    // Compter le nombre de cellules dans la liste
    int numberOfCells = countCells(list);

    // Afficher le niveau
    printf("Affichage du niveau %d :\n", levelToPrint);

    printLevel(list, levelToPrint, numberOfCells);

    printf("\nNombre total de cellules dans le niveau %d : %d\n", levelToPrint, numberOfCells);
}




/* ------------------------------------------------------- */
/* ---------------  FONCTIONS DE RECHERCHE --------------- */
/* ------------------------------------------------------- */


void searchCell(LevelList* list) {
    printf("\nVous avez choisi de rechercher une cellule dans la liste.\n");

    int searchChoice;
    int searchValue;
    int found = 0;
    Cell* current = list->head->next[0];
    int position = 0;

    printf("Vous avez deux modes de recherche :\n");
    printf("0. Rechercher la cellule a partir de sa valeur\n");
    printf("1. Rechercher la cellule a partir de son indice de colonne\n");

    do {
        printf("Votre choix : ");
        scanf("%d", &searchChoice);
    } while (searchChoice != 0 && searchChoice != 1);

    switch (searchChoice)
    {
    case 0:
        printf("Veuillez renseigner la valeur de la cellule que vous souhaitez rechercher : ");
        scanf("%d", &searchValue);

        // Rechercher dans la liste la cellule a partir de sa valeur
        

        while (current != NULL) {
            if (current->value == searchValue) {
                found = 1;
                break;
            }
            current = current->next[0];
            position++;
        }

        if (found) {
            printf("La cellule avec la valeur %d a ete trouvee.\n", searchValue);
            printf("Position dans la liste : %d\n", position+1); //'+1' car user compte a partir de 1
            printf("Hauteur de la cellule : %d\n", current->levels);
        } else {
            printf("La cellule avec la valeur %d n'a pas ete trouvee dans la liste.\n", searchValue);
        }

        break;

    case 1:

        do {
            printf("Veuillez renseigner l'indice de la position de la cellule que vous souhaitez rechercher : ");
            scanf("%d", &searchValue);
        } while (searchValue < 0);
        searchValue--; //'-1' car user compte a partir de 1

        // Afficher le contenu et la hauteur de la cellule a l'indice searchValue
        

        while (current != NULL && position < searchValue) {
            current = current->next[0];
            position++;
        }

        if (current != NULL) {
            printf("Contenu de la cellule a l'indice %d : %d\n", searchValue, current->value);
            printf("Hauteur de la cellule : %d\n", current->levels);
        } else {
            printf("Indice invalide. La liste ne contient pas de cellule a l'indice %d+1.\n", searchValue);
        }

        break;


    default:
        printf("Error code 'searchCell-01', contact admin.\n");
        break;
    }
}


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
                result.height = i + 1;  // Hauteur est indexee a partir de 1
                return result;
            } else if (current->value < value) {
                break;  // Passer au niveau inferieur si la valeur actuelle est inferieure a la recherche
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
/* -----------------------  AGENDA ----------------------- */
/* ------------------------------------------------------- */

//Generation 

Agenda* createCellAgenda(const char* name, const char* surname, int levels) {
    Agenda* newCell = (Agenda*)malloc(sizeof(Agenda));
    
    if (newCell == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la cellule Agenda\n");
        exit(EXIT_FAILURE);
    }

    strncpy(newCell->name, name, sizeof(newCell->name) - 1);
    newCell->name[sizeof(newCell->name) - 1] = '\0';  // Assurer la null-termination

    strncpy(newCell->surname, surname, sizeof(newCell->surname) - 1);
    newCell->surname[sizeof(newCell->surname) - 1] = '\0';  // Assurer la null-termination

    newCell->level = 0;
    newCell->column = 0;

    newCell->meetings = NULL; // Initialiser la liste chaînée de réunions à NULL

    newCell->next = (Agenda**)malloc(levels * sizeof(Agenda*));

    if (newCell->next == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les niveaux de la cellule Agenda\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < levels; i++) {
        newCell->next[i] = NULL;
    }

    return newCell;
}


//Fonctionalites

LevelAgenda* addContact(LevelAgenda* myList) {
    printf("Vous avez choisi d'ajouter un contact.\n");

    int choiceCount;
    int choiceReu;

    do {
        printf("Combien de contacts voulez-vous ajouter?\n");
        scanf("%d", &choiceCount);
    } while (choiceCount <= 0);

    for (int i = 0; i < choiceCount; i++) {
        Agenda* newCell = createCellAgenda("", "", 1);

        printf("Entrer le nom du contact sous la forme (nom_prenom) : ");
        scanf(" %99[^_]_%99s", newCell->name, newCell->surname);

        // À ce stade, newCell->name et newCell->surname contiennent le nom et le prenom separes
        // Inserer la nouvelle cellule dans la liste
        insertAtHeadChar(myList, newCell);

        // Afficher le contenu de la cellule
        printf("Nom: %s\n", newCell->name);
        printf("Prenom: %s\n", newCell->surname);

        do {
            printf("Voulez-vous ajouter une reunion ? (1/0) : ");
            scanf("%d", &choiceReu);
        } while (choiceReu != 1 && choiceReu != 0);

        if (choiceReu == 1) {
            Meeting* newMeeting = (Meeting*)malloc(sizeof(Meeting));

            printf("Entrer la date de la reunion sous la forme (jour-mois-annee) : ");
            scanf("%d-%d-%d", &newMeeting->day, &newMeeting->month, &newMeeting->year);

            printf("Entrer l'heure de la reunion sous la forme (heure:minute:seconde) : ");
            scanf("%d:%d:%d", &newMeeting->hour, &newMeeting->minute, &newMeeting->second);

            // Ajouter la réunion à la liste chaînée
            insertMeeting(&newCell->meetings, newMeeting);
        } else {
            // Aucune réunion
            newCell->meetings = NULL;
        }
    }

    return myList;
}



LevelAgenda* deleteContact(LevelAgenda* myList) {
    printf("Vous avez choisi de supprimer un contact.\n");

    char nameToDelete[100];
    int choiceCount;

    do {
        printf("Combien de contacts voulez-vous supprimer?\n");
        scanf("%d", &choiceCount);
    } while (choiceCount <= 0);

    for (int i = 0; i < choiceCount; i++) {

        // Boucle do-while pour la saisie sécurisée
        do {
            printf("Veuillez choisir le nom du contact : ");
            if (scanf("%99s", nameToDelete) != 1) {
                printf("Erreur de saisie. Veuillez réessayer.\n");
                // Effacer le tampon d'entrée en cas d'erreur
                while (getchar() != '\n');
                continue;
            }

            // Sortir de la boucle si la saisie est correcte
            break;

        } while (1);

        // Recherche du contact dans la liste
        Agenda* current = myList->head;
        Agenda* prev = NULL;

        while (current != NULL) {
            // Comparer le nom du contact avec celui saisi
            if (strcmp(current->name, nameToDelete) == 0) {
                // La cellule correspondante a été trouvée, supprimer la cellule
                if (prev != NULL) {
                    prev->next[0] = current->next[0];
                } else {
                    myList->head = current->next[0];
                }

                // Libérer la mémoire de la cellule à supprimer
                free(current->meetings);
                free(current);

                printf("Contact supprimé avec succès.\n");
                break;
            }

            prev = current;
            current = current->next[0];
        }

        // Afficher un message si le contact n'a pas été trouvé
        if (current == NULL) {
            printf("Contact non trouvé dans la liste.\n");
        }
    }

    return myList;
}



//Affichage

int countCharName(LevelAgenda* list, int column) {
    // Initialisation du compteur
    int count = 0;

    // Parcourir la liste pour trouver la cellule à la colonne spécifiée
    Agenda* current = list->head;
    while (current != NULL) {
        if (current->column == column) {
            // Trouver la cellule à la colonne spécifiée, compter les caractères du nom
            count = strlen(current->name);
            break;
        }
        current = current->next[0];
    }

    return count;
}


int countCharSurname(LevelAgenda* list, int column) {
    // Initialisation du compteur
    int count = 0;

    // Parcourir la liste pour trouver la cellule à la colonne spécifiée
    Agenda* current = list->head;
    while (current != NULL) {
        if (current->column == column) {
            // Trouver la cellule à la colonne spécifiée, compter les caractères du prénom
            count = strlen(current->surname);
            break;
        }
        current = current->next[0];
    }

    return count;
}


void capitalizeString(char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = toupper(str[i]);
    }
}

void searchContact(LevelAgenda* list) {
    char searchName[100];

    printf("Vous avez choisi de rechercher un contact.\n");

    // Boucle do-while pour la saisie sécurisée
    do {
        printf("Veuillez choisir le nom ou le prenom du contact : ");
        if (scanf("%99s", searchName) != 1) {
            printf("Erreur de saisie. Veuillez réessayer.\n");
            // Effacer le tampon d'entrée en cas d'erreur
            while (getchar() != '\n');
            continue;
        }

        // Sortir de la boucle si la saisie est correcte
        break;
    } while (1);

    // Recherche du contact dans la liste
    Agenda* current = list->head;

    while (current != NULL) {
        // Comparer le nom ou le prenom du contact avec la saisie
        if (strcmp(current->name, searchName) == 0 || strcmp(current->surname, searchName) == 0) {
            // Afficher les informations du contact
            printf("Contact trouvé :\n");
            printf("Nom : %s\n", current->name);
            printf("Prenom : %s\n", current->surname);
            printf("Nombre de réunions (niveau) : %d\n", current->level);

            // Afficher les heures de réunion
            Node* meeting = current->meetings;
            while (meeting != NULL) {
                printf("Réunion : %02d-%02d-%04d %02dh%02d:%02d\n", meeting->meeting.day, meeting->meeting.month, meeting->meeting.year, meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
                meeting = meeting->next;
            }

            return;  // Sortir de la fonction après avoir trouvé le contact
        }

        current = current->next[0];
    }

    // Afficher un message si le contact n'a pas été trouvé
    printf("Contact non trouvé dans la liste.\n");
}




void searchMeeting(LevelAgenda* list) {
    int searchChoice;

    printf("Vous avez choisi de rechercher une reunion.\n");

    // Demander a l'utilisateur de choisir entre la recherche par date ou par heure
    do {
        printf("Choisissez le type de recherche :\n");
        printf("1. Par date\n");
        printf("2. Par heure\n");
        printf("Votre choix : ");
        scanf("%d", &searchChoice);
    } while (searchChoice < 1 || searchChoice > 2);

    switch (searchChoice) {
        case 1:
            // Recherche par date
            searchMeetingByDate(list);
            break;
        case 2:
            // Recherche par heure
            searchMeetingByHour(list);
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void searchMeetingByDate(LevelAgenda* list) {
    int searchDay, searchMonth, searchYear;

    printf("Veuillez entrer la date de la reunion (jj mm aaaa) : ");
    scanf("%d %d %d", &searchDay, &searchMonth, &searchYear);

    // Parcourir la liste et afficher les reunions a la date donnee
    Agenda* current = list->head;

    while (current != NULL) {
        Node* meetingNode = current->meetings;

        while (meetingNode != NULL) {
            if (meetingNode->meeting.day == searchDay &&
                meetingNode->meeting.month == searchMonth &&
                meetingNode->meeting.year == searchYear) {
                printf("Reunion trouvee :\n");
                printf("Nom : %s\n", current->name);
                printf("Prenom : %s\n", current->surname);
                printf("Date : %02d-%02d-%04d\n", meetingNode->meeting.day, meetingNode->meeting.month, meetingNode->meeting.year);
                printf("Heure : %02dh%02d:%02d\n", meetingNode->meeting.hour, meetingNode->meeting.minute, meetingNode->meeting.second);
                printf("\n");
            }
            meetingNode = meetingNode->next;
        }

        current = current->next[0];
    }
}

void searchMeetingByHour(LevelAgenda* list) {
    int searchHour, searchMinute;

    printf("Veuillez entrer l'heure de la reunion (hh mm) : ");
    scanf("%d %d", &searchHour, &searchMinute);

    // Parcourir la liste et afficher les reunions a l'heure donnee
    Agenda* current = list->head;

    while (current != NULL) {
        Node* meetingNode = current->meetings;

        while (meetingNode != NULL) {
            if (meetingNode->meeting.hour == searchHour &&
                meetingNode->meeting.minute == searchMinute) {
                printf("Reunion trouvee :\n");
                printf("Nom : %s\n", current->name);
                printf("Prenom : %s\n", current->surname);
                printf("Date : %02d-%02d-%04d\n", meetingNode->meeting.day, meetingNode->meeting.month, meetingNode->meeting.year);
                printf("Heure : %02dh%02d:%02d\n", meetingNode->meeting.hour, meetingNode->meeting.minute, meetingNode->meeting.second);
                printf("\n");
            }
            meetingNode = meetingNode->next;
        }

        current = current->next[0];
    }
}

LevelAgenda* addMeeting(LevelAgenda* list) {
    printf("Vous avez choisi d'ajouter une reunion.\n");

    // Creer une nouvelle cellule pour la reunion
    Agenda* newMeeting = createCellAgenda("", "", 1);

    // Demander a l'utilisateur de choisir le nom et le prenom de la personne pour la reunion
    printf("Entrer le nom du participant sous la forme (nom_prenom) : ");
    scanf("%99[^_]_%99s", newMeeting->name, newMeeting->surname);

    // Demander a l'utilisateur de choisir la date de la reunion
    printf("Entrer la date de la reunion (jj mm aaaa) : ");
    scanf("%d %d %d", &newMeeting->meetings->meeting.day, &newMeeting->meetings->meeting.month, &newMeeting->meetings->meeting.year);

    // Demander a l'utilisateur de choisir l'heure de la reunion
    printf("Entrer l'heure de la reunion (hh mm) : ");
    scanf("%d %d", &newMeeting->meetings->meeting.hour, &newMeeting->meetings->meeting.minute);

    // Verifier si la reunion n'existe pas deja pour eviter les doublons
    if (meetingExists(list, newMeeting)) {
        printf("Cette reunion existe deja.\n");
        free(newMeeting);  // Liberer la memoire de la nouvelle cellule
        return list;       // Retourner la liste inchangee
    }

    // Ajouter la nouvelle reunion comme un nouveau level dans la cellule correspondante
    insertAtHeadAgenda(list, newMeeting);

    printf("Reunion ajoutee avec succes.\n");

    return list;
}



// Fonction pour vérifier si une réunion existe déjà
int meetingExists(LevelAgenda* list, Agenda* meeting) {
    Agenda* current = list->head;

    while (current != NULL) {
        // Assurez-vous de comparer avec les champs de la structure Meeting dans Node
        if (strcmp(current->name, meeting->name) == 0 &&
            strcmp(current->surname, meeting->surname) == 0 &&
            current->meetings->meeting.day == meeting->meetings->meeting.day &&
            current->meetings->meeting.month == meeting->meetings->meeting.month &&
            current->meetings->meeting.year == meeting->meetings->meeting.year &&
            current->meetings->meeting.hour == meeting->meetings->meeting.hour &&
            current->meetings->meeting.minute == meeting->meetings->meeting.minute) {
            return 1;  // La réunion existe déjà
        }
        current = current->next[0];
    }

    return 0;  // La réunion n'existe pas
}




//Affichage 

void printLevelAgenda(LevelAgenda* list, int level, int numberOfCells) {
    printf("[ list head_%d | @ - ] --", level);

    Agenda* current = list->head->next[level];
    int char_cell = 42;

    int maxCols = maxColumnsAgenda(list);
    int* distinctValues = malloc(numberOfCells * sizeof(int));
    if (distinctValues == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    if (current == NULL) {
        // Affichage du nombre total de caracteres pour chaque colonne
        for (int k = 0; k < maxCols; k++) {
            int totalChars = (char_cell + (countCharNameAgenda(list, k) + countCharSurnameAgenda(list, k)));

            // Affichage des tirets
            for (int i = 0; i < totalChars; i++) {
                printf("-");
            }
        }
        printf("-> NULL\n");
        return;
    }

    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = -1;

    while (current != NULL) {
        int nbCharLenght = tabcharlengthAgenda(list, current->column);

        for (int i = lastPrinted + 1; i < current->column; i++) {
            int nbCharPreviousCell = countCharNameAgenda(list, i) + countCharSurnameAgenda(list, i);

            for (int j = 0; j < char_cell + nbCharPreviousCell; j++) {
                printf("-");
            }
            printedColumns[i] = 1;
        }
        capitalizeString(current->name);
        printf("> [ %s %s ~ %02d-%02d-%04d %02dh%02d:%02d (%d) | @ - ]", 
        current->name, current->surname, 
        current->meetings->meeting.day, current->meetings->meeting.month, current->meetings->meeting.year,
        current->meetings->meeting.hour, current->meetings->meeting.minute, current->meetings->meeting.second,
        current->column);



        if (current->next[level] != NULL && current->next[level]->column != current->column) {
            printf(" --");
        } else {
            printf(" ");
            int remainingColumns = maxCols - (current->column + 1);

            for (int k = 0; k < remainingColumns; k++) {
                // Obtenez la cellule correspondant a la colonne (fin de la liste - k)
                Agenda* celluleCourante = obtenirCelluleAgenda(list, maxCols - k - 1);

                int nbCharCurrentCell = countCharNameAgenda(list, celluleCourante->column) + countCharSurnameAgenda(list, celluleCourante->column);
                int totalChars = char_cell + nbCharCurrentCell;

                // Affichage des tirets
                for (int i = 0; i < totalChars; i++) {
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


void printAllLevelsChar(LevelList* list, int numberOfCells) {
    for (int i = 0; i < list->maxLevels; i++) {
        printLevelChar(list, i, numberOfCells);
    }
}

void insertAtHeadChar(LevelList* list, Cell* newCell) {
    int maxLevels = list->maxLevels;
    int newCellLevels = newCell->levels;

    if (newCellLevels > maxLevels) {
        LevelList* extendedList = createLevelList(newCellLevels);

        for (int i = 0; i < maxLevels; i++) {
            Cell* current = list->head->next[i];
            while (current != NULL) {
                Cell* next = current->next[i];
                insertAtHeadChar(extendedList, current);
                current = next;
            }
        }

        free(list->head);
        free(list);
        *list = *extendedList;
        maxLevels = newCellLevels; // Mise a jour du nombre maximal de niveaux
    }

    // for (int i = 0; i < newCellLevels; i++) {
    //     Cell* current = list->head;
    //     while (current->next[i] != NULL && current->next[i]->value < newCell->value) {
    //         current = current->next[i];
    //     }

    //     if (current->next[i] != NULL && current->next[i]->value == newCell->value) {
    //         printf("Duplicate value detected. Cannot insert.\n");
    //         return;
    //     }

    //     newCell->next[i] = current->next[i];
    //     current->next[i] = newCell;
    // }
}


/* ------------------------------------------------------- */
/* ------------------------  MAIN ------------------------ */
/* ------------------------------------------------------- */



int main() {
    int mainOption;
    int option;
    int optionFonctionalityChoice;
    int optionFonctionality;


    printf("\n\n\nBIENVENUE SUR VOTRE AGENDA\n\n\n");


    printf("Pour passer au mode developpeur(Partie 1), tapper 1, sinon taper sur n'importe quel autre nombre : ");
    scanf("%d", &mainOption);

    if (mainOption == 1){
        LevelList* myList = NULL;

        printf("Bienvenue sur le programme de recherche d'une valeur dans une liste.\n");
        printf("Vous pouvez choisir entre trois options:\n");

        printf("1. Rechercher une valeur dans une liste a partir de cellules predefinies\n");
        printf("2. Rechercher une valeur dans une liste a partir de cellules que vous choississez\n");
        printf("3. Rechercher une valeur dans une liste a partir de n cellules aleatoires. Vous choississez 'n'.\n");

        do {
            printf("Voulez-vous utiliser le schema par defaut (0), le schema manuel (1), l'utilisation d'une liste de n valeurs (2) ou quitter le programme (3) ? (0, 1, 2 ou 3) : ");
            scanf("%d", &option);
        } while (option < 0 || option > 3);

        printf("\n\n");

        switch (option) {
            case 0:
                myList = listPreset();
                break;
            case 1:
                myList = listManual();
                break;
            case 2:
                myList = listAlea();
                break;
            case 3:
                printf("Fin du programme.\n");
                return 0;
            default:
                printf("Error code 'main-01', contact admin.\n");
                return 1;
        }

        do {
            printf("\nLa liste precedente est a present affichee. Maintenant, vous pouvez utiliser les quelques fonctionnalites suivantes :\n");
            printf("Vous pouvez choisir entre cinq options:\n");

            printf("0. Ajouter un element\n");
            printf("1. Supprimer un element\n");
            printf("2. Rechercher un element\n");
            printf("3. Afficher un seul niveau\n");

            do {
                printf("Voulez-vous utiliser ces fonctionnalites? (1 pour Oui, 0 pour Non) : ");
                scanf("%d", &optionFonctionalityChoice);
            } while (optionFonctionalityChoice < 0 || optionFonctionalityChoice > 1);

            if (optionFonctionalityChoice == 1) {
                printf("Choississez la fonctionnalite (0, 1, 2 ou 3) : \n");

                do {
                    printf("Votre choix : ");
                    scanf("%d", &optionFonctionality);
                } while (optionFonctionality < 0 || optionFonctionality > 3);

                switch (optionFonctionality) {
                    case 0:
                        addCell(myList);
                        break;
                    case 1:
                        deleteCell(myList);
                        break;
                    case 2:
                        searchCell(myList);
                        break;
                    case 3:
                        printOneLevel(myList);
                        break;
                    default:
                        printf("Error code 'main-02-02', contact admin.\n");
                        break;
                }

            } else if (optionFonctionalityChoice == 0) {
                printf("Retour au menu principal.\n");

            } else {
                printf("Error code 'main-02-01', contact admin.\n");
                return 1;
            }

        } while (optionFonctionalityChoice == 1);

    } else {

        int choiceAgenda;

        LevelAgenda* myList = createLevelAgenda(5);

        char fileName[100];
        char name[100];
        char surname[100] = "Valentin";

        do {
            printf("\n\nBonjour %s!\n\n", surname);

            printf("Bienvenue dans votre menu.\nVous pouvez : \n\n");

            printf("1. Ajouter un ou plusieurs contacts.\n");
            printf("2. Supprimer un ou plusieurs contacts.\n");
            printf("3. Rechercher un contact.\n");
            printf("4. Rechercher une reunion.\n");
            printf("5. Assigner une nouvelle reunion a un contact.\n");
            printf("6. Afficher l'agenda.\n");
            
            do {
                printf("Que voulez-vous faire (0 pour quitter): ");
                scanf("%d", &choiceAgenda);
            } while (choiceAgenda < 0 || choiceAgenda > 6);

            switch (choiceAgenda) {
                case 0:
                    printf("Au revoir %s!\n", surname);
                    return 0;
                    break;
                case 1:
                    myList = addContact(myList);
                    printf("Ajout effectue avec succes.\n");
                    printf("Affichage de votre agenda : \n");

                    int numberOfCells = countCells(myList); // À définir : appeler une fonction pour obtenir le nombre de cellules
                    assignColumnIndices(myList, numberOfCells);
                    printAllLevelsChar(myList, numberOfCells);
                    break;
                case 2:
                    myList = deleteContact(myList);

                    int numberOfCell = countCells(myList); // À définir : appeler une fonction pour obtenir le nombre de cellules
                    assignColumnIndices(myList, numberOfCell);
                    printAllLevelsChar(myList, numberOfCell);
                    break;
                case 3:
                    searchContact(myList);
                    break;
                case 4:
                    searchMeeting(myList);
                    break;
                case 5:
                    addMeeting(myList);

                    int numberOfCellss = countCells(myList); // À définir : appeler une fonction pour obtenir le nombre de cellules
                    assignColumnIndices(myList, numberOfCellss);
                    printAllLevelsChar(myList, numberOfCellss);
                    break;
                case 6:
                    printf("Affichage de votre agenda : \n");

                    int numberOfCellsss = countCells(myList); // À définir : appeler une fonction pour obtenir le nombre de cellules
                    assignColumnIndices(myList, numberOfCellsss);
                    printAllLevelsChar(myList, numberOfCellsss);
                    break;
                default:
                    break;
            }

        } while (choiceAgenda != 0);

    }
    return 0;
}


LevelList* listPreset() {
    // Utiliser le schema par defaut
    LevelList* myList = createLevelList(5);

    Cell* cell1 = createCell(1, 3);
    Cell* cell2 = createCell(12, 4);
    Cell* cell3 = createCell(3, 3);
    Cell* cell4 = createCell(500, 3);
    Cell* cell5 = createCell(400, 3);

    insertAtHead(myList, cell1);
    insertAtHead(myList, cell2);
    insertAtHead(myList, cell3);
    insertAtHead(myList, cell4);
    insertAtHead(myList, cell5);

    int numberOfCells = countCells(myList);
    assignColumnIndices(myList, numberOfCells);
    printAllLevels(myList, numberOfCells);
    printf("\n");

    return myList;  // Liberer la memoire allouee a la fin de la fonction
}

LevelList* listManual() {
    // Saisir les valeurs depuis l'utilisateur
    int numLevels;
    int numCells;

    printf("Combien de niveaux souhaitez-vous creer ?");

    do {
        scanf("%d", &numLevels);
    } while (numLevels <= 0);


    LevelList* myList = createLevelList(numLevels);

    do {
        printf("Combien de cellules souhaitez-vous creer ? ");
        scanf("%d", &numCells);
    } while (numCells <= 0);



    for (int i = 0; i < numCells; i++) {
        int value, levels;

        do {
            printf("Valeur et niveau de la cellule %d (separes par un espace) : ", i + 1);
            scanf("%d %d", &value, &levels);
        } while (levels <= 0 || levels > numLevels);

        Cell* newCell = createCell(value, levels);
        insertAtHead(myList, newCell);
    }

    int numberOfCells = countCells(myList);
    assignColumnIndices(myList, numberOfCells);
    printAllLevels(myList, numCells);

    int searchValue = 10;  // Modifier la valeur a rechercher
    compareSearchTimes(myList, searchValue);

    return myList; 
}

LevelList* listAlea() {

    int numLevels;
    int numRandomValues;

    do {
        printf("Combien de niveaux souhaitez-vous creer ? ");
        scanf("%d", &numLevels);
    } while (numLevels <= 0);
    
    LevelList* randomList = createRandomLevelList(numLevels);

    
    do {
        printf("Combien de valeurs aleatoires souhaitez-vous ajouter? ");
        scanf("%d", &numRandomValues);
    } while (numRandomValues <= 0);
    

    fillTableWithRandomValues(randomList, numRandomValues);

    int numberOfCells = countCells(randomList);
    assignColumnIndices(randomList, numberOfCells);
    printAllLevels(randomList, numRandomValues);

    int searchValue = 10;  // Modifier la valeur a rechercher
    compareSearchTimes(randomList, searchValue);

    return randomList; 
}
