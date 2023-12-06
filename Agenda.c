#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "Agenda.h"

#define MAX_LEVELS 10




/* ------------------------------------------------------- */
/* -----------------------  AGENDA ----------------------- */
/* ------------------------------------------------------- */



//Generation 



// Fonction pour creer un agenda vide
LevelAgenda* createLevelAgenda(int maxLevels) {
    LevelAgenda* newList = (LevelAgenda*)malloc(sizeof(LevelAgenda));
    if (newList == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    newList->maxLevels = maxLevels;
    newList->head = NULL;

    printf("\n-- CreateLevelAgenda test1 passed : Agenda Init --\n");

    return newList;
}

// Fonction pour creer une cellule avec un meeting NULL
Agenda* createCellAgenda(const char* name, const char* surname, int column) {
    Agenda* newCell = (Agenda*)malloc(sizeof(Agenda));
    if (newCell == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour la cellule Agenda\n");
        return NULL;
    }

    strncpy(newCell->name, name, sizeof(newCell->name) - 1);
    strncpy(newCell->surname, surname, sizeof(newCell->surname) - 1);

    newCell->meetings = NULL;
    newCell->next = NULL;
    newCell->column = column;

    return newCell;
}

// Fonction pour creer un meeting avec les parametres donnes
MeetingNode* createMeeting(int day, int month, int year, int hour, int minute, int second, int level) {
    MeetingNode* newMeeting = (MeetingNode*)malloc(sizeof(MeetingNode));
    if (newMeeting == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour le noeud de reunion\n");
        return NULL;
    }

    newMeeting->meeting.day = day;
    newMeeting->meeting.month = month;
    newMeeting->meeting.year = year;
    newMeeting->meeting.hour = hour;
    newMeeting->meeting.minute = minute;
    newMeeting->meeting.second = second;
    newMeeting->meeting.level = level;

    newMeeting->next = NULL;

    return newMeeting;
}




// Gestion des Contacts


// Fonction pour ajouter un contact avec la possibilite d'ajouter des reunions
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

    int levelMeeting = 0;

    printf("Entrer le nom du contact sous la forme (nom_prenom) : ");
    scanf(" %99[^_]_%99s", name, surname);

    day = 11;
    month = 11;
    year = 1111;
    hour = 11;
    minute = 11;
    seconde = 11;
    levelMeeting = 0;

    // Creer une cellule avec un meeting et specifier la colonne (index)
    int column = maxColumnsAgenda(myList);
    Agenda* newCell = createCellAgenda(name, surname, column);
    MeetingNode* newMeeting = createMeeting(day, month, year, hour, minute, seconde, levelMeeting);

    // Ajouter le meeting a la liste des reunions de la cellule
    newMeeting->next = newCell->meetings;
    newCell->meetings = newMeeting;

    // Ajouter la cellule a la liste principale
    newCell->next = myList->head;
    myList->head = newCell;

    return myList;
}



LevelAgenda* deleteContact(LevelAgenda* myList) {
    char nameToDelete[100];
    char surnameToDelete[100];

    // Demander a l'utilisateur le nom a supprimer
    printf("Entrez le nom du contact a supprimer (sous la forme nom_prenom): ");
    scanf(" %99[^_]_%99s", nameToDelete, surnameToDelete);

    // Recherche du contact dans l'agenda
    Agenda** currentLevelPtr = &(myList->head); // Utilisation d'un double pointeur
    Agenda* previous = NULL;

    while (*currentLevelPtr != NULL) {
        Agenda* current = *currentLevelPtr;

        while (current != NULL) {
            // Comparer les noms et prenoms pour trouver le contact
            if (strcmp(current->name, nameToDelete) == 0 && strcmp(current->surname, surnameToDelete) == 0) {
                // Contact trouve, supprimer le contact de l'agenda

                if (previous != NULL) {
                    // Si ce n'est pas la premiere cellule, mettre a jour le lien next du precedent
                    previous->next = current->next;
                } else {
                    // Si c'est la premiere cellule, mettre a jour la tête de la liste
                    *currentLevelPtr = current->next;
                }

                free(current); // Liberer la memoire du contact supprime

                printf("Le contact \"%s %s\" a ete supprime de l'agenda.\n", nameToDelete, surnameToDelete);

                // Retourner la liste mise a jour
                return myList;
            }

            previous = current;
            current = current->next;
        }

        currentLevelPtr = &((*currentLevelPtr)->next);
    }

    // Si on arrive ici, le contact n'a pas ete trouve
    printf("Le contact \"%s %s\" n'a pas ete trouve dans l'agenda.\n", nameToDelete, surnameToDelete);
    return myList;
}




// Gestion des Meetings


LevelAgenda* addMeeting(LevelAgenda* list) {
    // DEMANDER A USER LE NOM DU CONTACT A QUI ONT DONNE LE MEETING
    char name[100] = "";
    char surname[100] = "";
    printf("Entrer le nom du contact sous la forme (nom_prenom) : ");
    scanf(" %99[^_]_%99s", name, surname);

    // RECHERCHER LA CELLULE CORRESPONDANT AU CONTACT
    Agenda* current = list->head;
    Agenda* contactCell = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->surname, surname) == 0) {
            contactCell = current;
            break;
        }
        current = current->next;
    }

    if (contactCell == NULL) {
        printf("Contact non trouve dans l'agenda.\n");
        return list;
    }

    // DEMANDER A USER LA DATE (jj-mm-yyyy) ET L'HEURE (hh:mm:ss) DU MEETING
    int day, month, year, hour, minute, second;
    printf("Entrer la date du meeting (jj-mm-yyyy) : ");
    scanf("%d-%d-%d", &day, &month, &year);

    printf("Entrer l'heure du meeting (hh:mm:ss) : ");
    scanf("%d:%d:%d", &hour, &minute, &second);

    // COMPTER LE NOMBRE DE MEETING ET AJOUTER LE MEETING A LA SUITE DES AUTRES
    int meetingCount = 0;
    MeetingNode* meetingNode = contactCell->meetings;
    while (meetingNode != NULL) {
        meetingCount++;
        meetingNode = meetingNode->next;
    }

    MeetingNode* newMeeting = createMeeting(day, month, year, hour, minute, second, meetingCount);
    
    newMeeting->next = contactCell->meetings;
    contactCell->meetings = newMeeting;

    return list;
}




// Recherche


void searchContact(LevelAgenda* list) {
    char nameToSearch[100];
    char surnameToSearch[100];

    // Demander a l'utilisateur le nom du contact
    printf("Entrez le nom du contact (sous la forme nom_prenom) : ");
    scanf(" %99[^_]_%99s", nameToSearch, surnameToSearch);

    // Parcourir la liste et afficher les reunions correspondantes
    Agenda* current = list->head;
    while (current != NULL) {
        // Comparer les noms et prenoms pour trouver le contact
        if (strcmp(current->name, nameToSearch) == 0 && strcmp(current->surname, surnameToSearch) == 0) {
            printf("Reunions pour %s %s :\n", current->name, current->surname);
            MeetingNode* meeting = current->meetings;
            while (meeting != NULL) {
                printf("    %02d/%02d/%04d %02d:%02d:%02d\n",
                       meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                       meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
                meeting = meeting->next;
            }
            return; // Retourner des que le contact est trouve, pas besoin de continuer la recherche
        }
        current = current->next;
    }

    // Si on arrive ici, le contact n'a pas ete trouve
    printf("Le contact \"%s %s\" n'a pas ete trouve dans l'agenda.\n", nameToSearch, surnameToSearch);
}

void searchMeeting(LevelAgenda* list) {
    int choice;
    printf("Voulez-vous rechercher par la date (1) ou l'heure (2) du meeting ? ");
    scanf("%d", &choice);

    // Consommer le caractere de nouvelle ligne restant dans le flux d'entree
    while (getchar() != '\n');

    bool meetingFound = false;  // Variable pour verifier si au moins une reunion a ete trouvee

    do {
        if (choice == 1) {
            int day, month, year;

            // Utiliser fgets pour lire la ligne
            char dateInput[20];
            printf("Entrez la date du meeting (jj-mm-aaaa) : ");
            fgets(dateInput, sizeof(dateInput), stdin);

            // Utiliser sscanf pour extraire les valeurs
            if (sscanf(dateInput, "%d-%d-%d", &day, &month, &year) == 3) {
                // Parcourir la liste et afficher les reunions correspondantes
                Agenda* current = list->head;
                while (current != NULL) {
                    MeetingNode* meeting = current->meetings;
                    while (meeting != NULL) {
                        if (meeting->meeting.day == day && meeting->meeting.month == month && meeting->meeting.year == year) {
                            printf("Meeting pour %s %s : %02d/%02d/%04d %02d:%02d:%02d\n",
                                   current->name, current->surname,
                                   meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                                   meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
                            meetingFound = true;
                        }
                        meeting = meeting->next;
                    }
                    current = current->next;
                }
            } else {
                printf("Format de date incorrect. Veuillez reessayer.\n");
            }
        } else if (choice == 2) {
            int hour, minute, second;

            // Utiliser fgets pour lire la ligne
            char timeInput[20];
            printf("Entrez l'heure du meeting (hh:mm:ss) : ");
            fgets(timeInput, sizeof(timeInput), stdin);

            // Utiliser sscanf pour extraire les valeurs
            if (sscanf(timeInput, "%d:%d:%d", &hour, &minute, &second) == 3) {
                // Parcourir la liste et afficher les reunions correspondantes
                Agenda* current = list->head;
                while (current != NULL) {
                    MeetingNode* meeting = current->meetings;
                    while (meeting != NULL) {
                        if (meeting->meeting.hour == hour && meeting->meeting.minute == minute && meeting->meeting.second == second) {
                            printf("Meeting pour %s %s : %02d/%02d/%04d %02d:%02d:%02d\n",
                                   current->name, current->surname,
                                   meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                                   meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
                            meetingFound = true;
                        }
                        meeting = meeting->next;
                    }
                    current = current->next;
                }
            } else {
                printf("Format d'heure incorrect. Veuillez reessayer.\n");
            }
        } else {
            printf("Choix invalide. Veuillez reessayer.\n");
        }

        if (!meetingFound) {
            printf("Aucun meeting correspondant a la recherche.\n");
        }

        // Si aucun meeting n'a ete trouve, demander a l'utilisateur de reessayer
    } while (!meetingFound);
}


// Sorting 
void swap(Agenda* a, Agenda* b) {
    Agenda temp = *a;
    *a = *b;
    *b = temp;

    // Mise a jour des liens next
    Agenda* tempNext = a->next;
    a->next = b->next;
    b->next = tempNext;

    printf("\n -- swap : test1 passed -- ");
}


LevelAgenda* sortAgenda(LevelAgenda* list) {
    // Verifier si la liste est vide ou a un seul element
    if (list == NULL || list->head == NULL || list->head->next == NULL) {
        //printf("\n -- sortAgenda : test0 passed --\n");
        return list; // Pas besoin de trier
    }

    //printf("\n -- sortAgenda : test1 passed --");

    Agenda* current = list->head;

    while (current != NULL) {
        Agenda* next = current->next;

        while (next != NULL) {
            //printf("current : %s %s ~ %d\n", current->name, current->surname, current->column);
            //printf("next : %s %s ~ %d\n", next->name, next->surname, next->column);
            if (current->column > next->column) {
                swap(current, next);
                //printf("\n -- sortAgenda : test2 passed --\n\n");
            }
            
            // Toujours incrementer next même si aucune permutation n'est effectuee
            next = next->next;
            
            if (next == NULL) {
                //printf("\n -- sortAgenda : WHILE : next Null --\n");
                break;
            }
        }
        //printf("\n -- sortAgenda : test4 passed --\n\n");

        current = current->next;
        //printf("\n -- sortAgenda : test5 passed --\n\n");
    }

    //printf("\n -- sortAgenda : test6 passed --\n\n");

    // // AFFICHER LES NOMS DE L'AGENDA AVEC LEUR INDICE DE COLONNE 
    // current = list->head;
    // while (current != NULL) {
    //     printf("Display after sort: %s %s ~ %d\n", current->name, current->surname, current->column);
    //     current = current->next;
    // }

    return list;
}









//Affichage

int countCharName(LevelAgenda* list, Agenda* contact) {
    // Verifier si la liste ou le contact est NULL
    if (list == NULL || contact == NULL) {
        fprintf(stderr, "Liste ou contact NULL.\n");
        return -1; // Valeur d'erreur
    }

    // Compter les caracteres dans le champ 'name'
    int count = 0;
    for (int i = 0; contact->name[i] != '\0'; i++) {
        count++;
    }

    return count;
}

int countCharSurname(LevelAgenda* list, Agenda* contact) {
    // Verifier si la liste ou le contact est NULL
    if (list == NULL || contact == NULL) {
        fprintf(stderr, "Liste ou contact NULL.\n");
        return -1; // Valeur d'erreur
    }

    // Compter les caracteres dans le champ 'surname'
    int count = 0;
    for (int i = 0; contact->surname[i] != '\0'; i++) {
        count++;
    }

    return count;
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

        // Passer a la prochaine cellule
        current = current->next;
    }

    return count;
}


void printLevelAgenda(LevelAgenda* list, int level, int numberOfCells) {
    printf("[ list head_%d | @ - ] __", level);

    Agenda* current = list->head;

    // Ajout d'une condition pour gerer le cas où la liste est vide
    if (current == NULL) {
        printf("-> NULL\n");
        return;
    }

    int char_cell = 44;
    int maxCols = list->maxLevels;

    int* printedColumns = (int*)calloc(maxCols, sizeof(int));
    int lastPrinted = 0;

    while (current != NULL) {

        //capitalizeString(current->name);

        // Recherche de la reunion correspondant au niveau actuel
        MeetingNode* meeting = current->meetings;
        while (meeting != NULL && meeting->meeting.level != level) {
            meeting = meeting->next;
        }

        if (meeting != NULL) {
            printf("> [ %s %s ~ %02d-%02d-%04d %02dh%02d:%02d (%d | %d) | @ - ] ", 
                current->name, current->surname, 
                meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second, current->column, level);
        } else {
            int numberOfCaracteres = char_cell + countCharName(list, current) + countCharSurname(list, current);
            for (int i = 0; i < numberOfCaracteres; i++) {
                printf("-");
            }
        }

        // Utilisation de current->next au lieu de current->next->column
        if (current->next == NULL) {
            printf("__> NULL\n");
        } else if (current->next != NULL && current->next->column != current->column) {
            printf("--");
        } else {
            printf(" ");
            int remainingColumns = maxCols - (current->column + 1);

            // Modification de la condition de boucle
            if (remainingColumns > 0) {
                for (int k = 0; k < remainingColumns; k++) {
                    // Obtenez la cellule correspondant a la colonne (fin de la liste - k)
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

    // Tri de l'agenda avant l'impression
    LevelAgenda* sortedList = sortAgenda(list);

    for (int i = 0; i < list->maxLevels; i++) {
        printLevelAgenda(sortedList, i, numberOfCells);
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

    return maxCols + 1; // Ajoute 1 car les colonnes sont indexees a partir de 0
}

// Fonction pour obtenir la cellule correspondant a la colonne
Agenda* obtenirCelluleAgenda(LevelAgenda* list, int column) {
    Agenda* current = list->head;

    while (current != NULL) {
        if (current->column == column) {
            return current;
        }
        current = current->next;
    }

    return NULL; // Retourne NULL si la colonne n'est pas trouvee
}
