#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "Agenda.h"

#define MAX_LEVELS 10




/* ------------------------------------------------------- */
/* -----------------------  AGENDA ----------------------- */
/* ------------------------------------------------------- */



//Generation 



// Fonction pour créer un agenda vide
LevelAgenda* createLevelAgenda(int maxLevels) {
    LevelAgenda* newList = (LevelAgenda*)malloc(sizeof(LevelAgenda));
    if (newList == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    newList->maxLevels = maxLevels;
    newList->head = NULL;

    printf("\n-- CreateLevelAgenda test1 passed : Agenda Init --\n");

    return newList;
}

// Fonction pour créer une cellule avec un meeting NULL
Agenda* createCellAgenda(const char* name, const char* surname, int column) {
    Agenda* newCell = (Agenda*)malloc(sizeof(Agenda));
    if (newCell == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la cellule Agenda\n");
        return NULL;
    }

    strncpy(newCell->name, name, sizeof(newCell->name) - 1);
    strncpy(newCell->surname, surname, sizeof(newCell->surname) - 1);

    newCell->meetings = NULL;
    newCell->next = NULL;
    newCell->column = column;

    return newCell;
}

// Fonction pour créer un meeting avec les paramètres donnés
MeetingNode* createMeeting(int day, int month, int year, int hour, int minute, int second) {
    MeetingNode* newMeeting = (MeetingNode*)malloc(sizeof(MeetingNode));
    if (newMeeting == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le noeud de réunion\n");
        return NULL;
    }

    newMeeting->meeting.day = day;
    newMeeting->meeting.month = month;
    newMeeting->meeting.year = year;
    newMeeting->meeting.hour = hour;
    newMeeting->meeting.minute = minute;
    newMeeting->meeting.second = second;

    newMeeting->next = NULL;

    return newMeeting;
}




// Gestion des Contacts


// Fonction pour ajouter un contact avec la possibilité d'ajouter des réunions
LevelAgenda* addContact(LevelAgenda* myList) {
    printf("Vous avez choisi d'ajouter un contact.\n");

    char name[100] = "";
    char surname[100] = "";
    int day = 0;
    int month = 0;
    int year = 0;
    int hour = 0;
    int minute = 0;
    int seconde = 0;

    printf("Entrer le nom du contact sous la forme (nom_prenom) : ");
    scanf(" %99[^_]_%99s", name, surname);

    day = 11;
    month = 11;
    year = 1111;
    hour = 11;
    minute = 11;
    seconde = 11;

    // Créer une cellule avec un meeting et spécifier la colonne (index)
    int column = maxColumnsAgenda(myList);
    Agenda* newCell = createCellAgenda(name, surname, column);
    MeetingNode* newMeeting = createMeeting(day, month, year, hour, minute, seconde);

    // Ajouter le meeting à la liste des réunions de la cellule
    newMeeting->next = newCell->meetings;
    newCell->meetings = newMeeting;

    // Ajouter la cellule à la liste principale
    newCell->next = myList->head;
    myList->head = newCell;

    return myList;
}



LevelAgenda* deleteContact(LevelAgenda* myList) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");

    return myList;
}


// Gestion des Meetings


LevelAgenda* addMeeting(LevelAgenda* list) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}


// Recherche


int meetingExists(LevelAgenda* list, Agenda* meeting) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

void searchContact(LevelAgenda* list) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

void searchMeeting(LevelAgenda* list) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

void searchMeetingByDate(LevelAgenda* list) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

void searchMeetingByHour(LevelAgenda* list) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}


//Affichage


int countCharName(LevelAgenda* list, int column) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

int countCharSurname(LevelAgenda* list, int column) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}

void capitalizeString(char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = toupper(str[i]);
    }
}

int countCellsAgenda(LevelAgenda* list) {
    int count = 0;

    // Parcourir la liste des agendas
    Agenda* current = list->head;
    while (current != NULL) {
        count++;

        // Passer à la prochaine cellule
        current = current->next;
    }

    return count;
}





void assignColumnIndicesAgenda(LevelAgenda* list, int numberOfCells) {
    printf("\n---\nCette fonctionnalite n'est pas encore disponible.\n");
}


void printLevelAgenda(LevelAgenda* list, int level, int numberOfCells) {
    printf("[ list head_%d | @ - ] __", level);

    Agenda* current = list->head;

    // Ajout d'une condition pour gérer le cas où la liste est vide
    if (current == NULL) {
        printf("-> NULL\n");
        return;
    }

    int char_cell = 1;
    int maxCols = list->maxLevels;

    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = 0;

    

    while (current != NULL) {
        // Ajout de tirets pour les colonnes non imprimées
        for (int i = lastPrinted + 1; i < current->column; i++) {
            printf("La colonne est : %d", current->column);
            int nbCharPreviousCell = 3;

            for (int j = 0; j < char_cell + nbCharPreviousCell; j++) {
                printf("-");
            }
            printedColumns[i] = 1;
        }

        capitalizeString(current->name);
        printf("> [ %s %s ~ %02d-%02d-%04d %02dh%02d:%02d (%d | %d) | @ - ]", 
            current->name, current->surname, 
            current->meetings->meeting.day, current->meetings->meeting.month, current->meetings->meeting.year,
            current->meetings->meeting.hour, current->meetings->meeting.minute, current->meetings->meeting.second, current->column, level);

        // Utilisation de current->next au lieu de current->next->column
        if (current->next == NULL) {
            printf(" ^^^^^>\n");
        }
        else if (current->next != NULL && current->next->column != current->column) {
            printf(" ^^");
        } else {
            printf(" ");
            int remainingColumns = maxCols - (current->column + 1);

            // Modification de la condition de boucle
            if (remainingColumns > 0) {
                for (int k = 0; k < remainingColumns; k++) {
                    // Obtenez la cellule correspondant à la colonne (fin de la liste - k)
                    Agenda* celluleCourante = obtenirCelluleAgenda(list, maxCols - k - 1);

                    int nbCharCurrentCell = 3;
                    int totalChars = char_cell + nbCharCurrentCell;

                    // Affichage des tirets
                    for (int i = 0; i < totalChars; i++) {
                        printf(".");
                    }
                }
                printf("___> NULL\n");
            } else {
                printf("___> NULL\n");
            }
            break;
        }



        printedColumns[current->column] = 1;
        lastPrinted = current->column;
        current = current->next;
    }

    free(printedColumns);
}




void printAllLevelsAgenda(LevelAgenda* list, int numberOfCells) {
    printf("\n");
    for (int i = 0; i < list->maxLevels; i++) {
        printLevelAgenda(list, i, numberOfCells);
    }
}



// Other


// Fonction pour obtenir le nombre maximal de colonnes dans la structure Agenda
int maxColumnsAgenda(LevelAgenda* list) {
    int maxCols = 0;
    Agenda* current = list->head;

    while (current != NULL) {
        if (current->column > maxCols) {
            maxCols = current->column;
        }
        current = current->next;
    }

    return maxCols + 1; // Ajoute 1 car les colonnes sont indexées à partir de 0
}

// Fonction pour obtenir la cellule correspondant à la colonne
Agenda* obtenirCelluleAgenda(LevelAgenda* list, int column) {
    Agenda* current = list->head;

    while (current != NULL) {
        if (current->column == column) {
            return current;
        }
        current = current->next;
    }

    return NULL; // Retourne NULL si la colonne n'est pas trouvée
}
