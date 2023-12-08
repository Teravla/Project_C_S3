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


// Fonction pour supprimer un meeting d'un contact
LevelAgenda* deleteMeeting(LevelAgenda* list) {
    char nameToDelete[100];
    char surnameToDelete[100];

    // Demander a l'utilisateur le nom et prenom du contact
    printf("Entrez le nom du contact (sous la forme nom_prenom) : ");
    scanf(" %99[^_]_%99s", nameToDelete, surnameToDelete);

    // Rechercher le contact dans l'agenda
    Agenda* currentLevel = list->head;

    while (currentLevel != NULL) {
        // Comparer les noms et prenoms pour trouver le contact
        if (strcmp(currentLevel->name, nameToDelete) == 0 && strcmp(currentLevel->surname, surnameToDelete) == 0) {
            // Afficher les meetings du contact
            printf("Liste des meetings pour %s %s :\n", nameToDelete, surnameToDelete);
            MeetingNode* currentMeeting = currentLevel->meetings;

            while (currentMeeting != NULL) {
                printf("%02d/%02d/%04d %02d:%02d:%02d\n", 
                        currentMeeting->meeting.day, currentMeeting->meeting.month, currentMeeting->meeting.year,
                        currentMeeting->meeting.hour, currentMeeting->meeting.minute, currentMeeting->meeting.second);
                currentMeeting = currentMeeting->next;
            }

            // Demander a l'utilisateur le meeting a supprimer
            int day, month, year, hour, minute, second;
            printf("Entrez le meeting a supprimer (jj-mm-aaaa hh:mm:ss) : ");
            int result = scanf("%d-%d-%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);

            if (result == 6) {
                // Rechercher le meeting dans la liste et le supprimer
                MeetingNode* previousMeeting = NULL;
                MeetingNode* currentMeeting = currentLevel->meetings;

                while (currentMeeting != NULL) {
                    if (currentMeeting->meeting.day == day && currentMeeting->meeting.month == month && currentMeeting->meeting.year == year &&
                        currentMeeting->meeting.hour == hour && currentMeeting->meeting.minute == minute && currentMeeting->meeting.second == second) {
                            printf("\n -- deleteMeeting : test1 passed -- ");
                            // Le meeting a ete trouve, le supprimer
                            if (previousMeeting != NULL) {
                                previousMeeting->next = currentMeeting->next;
                            } else {
                                currentLevel->meetings = currentMeeting->next;
                            }

                            free(currentMeeting); // Liberer la memoire du meeting supprime
                            printf("Meeting supprime avec succes.\n");

                            //Correction display 
                            sortAgendaMeeting(list);

                            return list;
                    }

                    previousMeeting = currentMeeting;
                    currentMeeting = currentMeeting->next;
                }

                printf("Meeting non trouve. Aucun meeting supprime.\n");
            } else {
                printf("Format de meeting incorrect. Aucun meeting supprime.\n");
            }
            return list; // Retourner la liste inchangee si le meeting n'a pas ete trouve
        }

        currentLevel = currentLevel->next;
    }

    // Si on arrive ici, le contact n'a pas ete trouve
    printf("Le contact \"%s %s\" n'a pas ete trouve dans l'agenda.\n", nameToDelete, surnameToDelete);


    return list;
}





// Recherche


void searchContactAutoComplete(LevelAgenda* list) {
    char partialName[100] = "";  // Initialisez la chaîne partielle
    char currentChar[2];  // Un tableau de deux caracteres pour la saisie utilisateur
    bool searchContinues = true;

    while (searchContinues) {
        printf("Entrez le caractere suivant du nom du contact (ou 0 pour terminer) : ");
        scanf("%1s", currentChar);

        // Verifiez si l'utilisateur a choisi de terminer la saisie
        if (currentChar[0] == '0') {
            searchContinues = false;
        } else {
            // Ajoutez le caractere actuel a la chaîne partielle
            partialName[strlen(partialName)] = currentChar[0];

            // Parcourez la liste et affichez les noms correspondants
            Agenda* current = list->head;
            while (current != NULL) {
                // Comparez les noms pour trouver les correspondances exactes
                if (strncmp(current->name, partialName, strlen(partialName)) == 0) {
                    printf("- %s %s\n", current->name, current->surname);
                }
                current = current->next;
            }

            // Verifiez s'il n'y a qu'une seule correspondance, auquel cas la recherche est terminee
            int matchesCount = 0;
            current = list->head;
            while (current != NULL) {
                if (strncmp(current->name, partialName, strlen(partialName)) == 0) {
                    matchesCount++;
                }
                current = current->next;
            }

            if (matchesCount == 1) {
                searchContinues = false;
                printf("Un seul contact correspondant. La recherche est terminee.\n");
            }
        }
    }
}

void searchContact(LevelAgenda* list) {
    clock_t start = clock();  // Enregistrez le temps de debut

    int choiceAutoComplete = 0;

    printf("\nVous avez choisi la recherche de contact.\n");
    printf("Voulez-vous utiliser l'autocompletion ? (1 : oui, 0 : non) ");
    scanf("%d", &choiceAutoComplete);

    if (choiceAutoComplete == 1) {
        searchContactAutoComplete(list);
    } else if (choiceAutoComplete == 0) {
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
                clock_t end = clock();  // Enregistrez le temps de fin
                double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;  // Calculez le temps ecoule
                printf("Temps de recherche : %.6f secondes.\n", elapsed_time);
                return;  // Retournez des que le contact est trouve, pas besoin de continuer la recherche
            }
            current = current->next;
        }

        // Si on arrive ici, le contact n'a pas ete trouve
        printf("Le contact \"%s %s\" n'a pas ete trouve dans l'agenda.\n", nameToSearch, surnameToSearch);

    } else {
        printf("Choix invalide. Veuillez reessayer.\n");
    }
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

    //printf("\n -- swap : test1 passed -- ");
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

void sortAgendaMeeting(LevelAgenda* list) {
    // Initialiser le niveau a 0
    int n = 0;

    // Parcourir la liste des agendas
    Agenda* current = list->head;

    while (current != NULL) {
        // Verifier si le niveau actuel de la cellule contient un meeting
        if (hasMeeting(current, n)) {
            n++;
        } else {
            // Deplacer le meeting du niveau n+1 au niveau n dans la même cellule
            moveMeeting(current, n + 1, n);
        }

        // Passer a la cellule suivante
        current = current->next;
    }
}

// Fonction pour verifier si le niveau de la cellule contient un meeting
bool hasMeeting(Agenda* agenda, int level) {
    MeetingNode* meeting = agenda->meetings;

    // Parcourir les meetings pour le niveau specifie
    while (meeting != NULL) {
        if (meeting->meeting.level == level) {
            return true;
        }
        meeting = meeting->next;
    }

    return false;
}

// Fonction pour deplacer un meeting d'un niveau a un autre dans la même cellule
void moveMeeting(Agenda* agenda, int fromLevel, int toLevel) {
    MeetingNode* currentMeeting = agenda->meetings;
    MeetingNode* previousMeeting = NULL;

    // Parcourir les meetings pour le niveau fromLevel
    while (currentMeeting != NULL) {
        if (currentMeeting->meeting.level == fromLevel) {
            // Supprimer le meeting du niveau fromLevel
            if (previousMeeting != NULL) {
                previousMeeting->next = currentMeeting->next;
            } else {
                agenda->meetings = currentMeeting->next;
            }

            // Mettre a jour le niveau du meeting
            currentMeeting->meeting.level = toLevel;

            // Inserer le meeting au niveau toLevel
            insertMeeting(agenda, currentMeeting);

            // Terminer la boucle apres le deplacement
            break;
        }

        previousMeeting = currentMeeting;
        currentMeeting = currentMeeting->next;
    }
}

// Fonction pour inserer un meeting dans la liste des meetings d'une cellule
void insertMeeting(Agenda* agenda, MeetingNode* newMeeting) {
    newMeeting->next = agenda->meetings;
    agenda->meetings = newMeeting;
}



// Fonction pour trier les cellules de l'agenda par ordre alphabetique des noms
void sortAgendaAlphabetically(LevelAgenda* list) {
    if (list == NULL || list->head == NULL || list->head->next == NULL) {
        // Pas besoin de trier s'il y a 0 ou 1 cellule dans l'agenda
        return;
    }

    int swapped;
    Agenda* current;
    Agenda* last = NULL;

    do {
        swapped = 0;
        current = list->head;

        while (current->next != last) {
            // Comparer les noms des cellules et echanger si necessaire
            if (strcmp(current->name, current->next->name) > 0) {
                swap(current, current->next);
                swapped = 1;
            }

            current = current->next;
        }

        last = current;
    } while (swapped);
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
    printf("[ list head_%d | @ - ] --", level);

    Agenda* current = list->head;


    // Ajout d'une condition pour gerer le cas où la liste est vide
    if (current == NULL) {
        printf("-> NULL\n");
        return;
    }

    int char_cell = 36;
    int maxCols = list->maxLevels;


    while (current != NULL) {

        // Recherche de la reunion correspondant au niveau actuel
        MeetingNode* meeting = current->meetings;
        while (meeting != NULL && meeting->meeting.level != level) {
            meeting = meeting->next;
        }

        if (meeting != NULL) {
            printf("> [ %s %s ~ %02d-%02d-%04d %02dh%02d:%02d | @ - ] ", 
                current->name, current->surname, 
                meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
        } else {
            int numberOfCaracteres = char_cell + countCharName(list, current) + countCharSurname(list, current);
            for (int i = 0; i < numberOfCaracteres; i++) {
                printf("-");
            }
        }
        if (current->next == NULL) {
            printf("--> NULL\n");
        } else if (current->next != NULL && current->next->column != current->column) {
            printf("--");
        } 

        current = current->next;
    }

}

void printAllLevelsAgenda(LevelAgenda* list, int numberOfCells) {
    printf("\n");
    // Tri de l'agenda avant l'impression
    LevelAgenda* sortList = sortAgenda(list);
    sortAgendaAlphabetically(sortList);


    for (int i = 0; i < sortList->maxLevels; i++) {
        printLevelAgenda(sortList, i, numberOfCells);
    }
}


// Export 

void exportAgenda(LevelAgenda* list) {
    char fileName[100];  // Vous pouvez ajuster la taille selon vos besoins

    // Demander le nom du fichier a l'utilisateur
    printf("Entrez le nom du fichier pour exporter l'agenda : ");
    scanf("%s", fileName);

    FILE* file = fopen(fileName, "w");

    if (file == NULL) {
        // Si le fichier n'existe pas, tentez de le creer
        file = fopen(fileName, "w");
        if (file == NULL) {
            printf("Erreur lors de la creation du fichier.\n");
            return;
        }
    }

    Agenda* current = list->head;
    while (current != NULL) {
        fprintf(file, "Contact: %s %s\n", current->name, current->surname);

        MeetingNode* meeting = current->meetings;

        // Stocker les reunions dans un tableau pour les afficher dans l'ordre inverse du niveau
        int meetingCount = 0;
        while (meeting != NULL) {
            meetingCount++;
            meeting = meeting->next;
        }

        MeetingNode** meetingArray = malloc(meetingCount * sizeof(MeetingNode*));
        if (meetingArray == NULL) {
            printf("Erreur lors de l'allocation memoire pour le tableau de reunions.\n");
            fclose(file);
            return;
        }

        // Remplir le tableau avec les reunions
        meeting = current->meetings;
        for (int i = 0; i < meetingCount; i++) {
            meetingArray[i] = meeting;
            meeting = meeting->next;
        }

        // Afficher les reunions dans l'ordre inverse du niveau
        for (int i = meetingCount - 1; i >= 0; i--) {
            fprintf(file, "Meeting level %d: %02d/%02d/%04d %02d:%02d:%02d\n",
                    meetingArray[i]->meeting.level,
                    meetingArray[i]->meeting.day, meetingArray[i]->meeting.month, meetingArray[i]->meeting.year,
                    meetingArray[i]->meeting.hour, meetingArray[i]->meeting.minute, meetingArray[i]->meeting.second);
        }

        free(meetingArray);  // Liberer la memoire allouee pour le tableau

        fprintf(file, "\n");  // Ajouter une ligne vide entre chaque contact
        current = current->next;
    }

    fclose(file);
    printf("Agenda exporte avec succes vers le fichier %s.\n", fileName);
}


// Fonction pour importer un agenda a partir d'un fichier
LevelAgenda* importAgenda(const char* fileName) {
    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    LevelAgenda* agendaList = (LevelAgenda*)malloc(sizeof(LevelAgenda));
    if (agendaList == NULL) {
        printf("Erreur lors de l'allocation memoire pour la liste d'agenda.\n");
        fclose(file);
        return NULL;
    }

    agendaList->maxLevels = 5;
    agendaList->head = NULL;

    Agenda* currentAgenda = NULL;
    MeetingNode* currentMeeting = NULL;

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Contact:", 8) == 0) {
            if (currentAgenda != NULL) {
                currentAgenda->next = agendaList->head;
                agendaList->head = currentAgenda;
                //printf("\n -- importAgenda : test1 passed --\n");
            }

            currentAgenda = (Agenda*)malloc(sizeof(Agenda));
            if (currentAgenda == NULL) {
                printf("Erreur lors de l'allocation memoire pour l'agenda.\n");
                freeAgendaList(agendaList);
                fclose(file);
                return NULL;
            }

            currentAgenda->meetings = NULL;
            sscanf(line, "Contact: %s %s", currentAgenda->name, currentAgenda->surname);
            currentAgenda->next = NULL;
            currentMeeting = NULL;
        } else if (strncmp(line, "Meeting level", 13) == 0) {
            if (currentAgenda == NULL) {
                printf("Erreur : Meeting trouve sans Contact correspondant.\n");
                freeAgendaList(agendaList);
                fclose(file);
                return NULL;
            }

            MeetingNode* newMeeting = (MeetingNode*)malloc(sizeof(MeetingNode));
            if (newMeeting == NULL) {
                printf("Erreur lors de l'allocation memoire pour la reunion.\n");
                freeAgendaList(agendaList);
                fclose(file);
                return NULL;
            }

            sscanf(line, "Meeting level %d: %02d/%02d/%04d %02d:%02d:%02d",
                &newMeeting->meeting.level,
                &newMeeting->meeting.day, &newMeeting->meeting.month, &newMeeting->meeting.year,
                &newMeeting->meeting.hour, &newMeeting->meeting.minute, &newMeeting->meeting.second);

            newMeeting->next = NULL;

            if (currentMeeting == NULL) {
                currentAgenda->meetings = newMeeting;
                currentMeeting = newMeeting;
                //printf("\n -- importAgenda : test2 passed --\n");
            } else {
                currentMeeting->next = newMeeting;
                currentMeeting = newMeeting;
                //printf("\n -- importAgenda : test3 passed --\n");
            }
        }
    }

    if (currentAgenda != NULL) {
        currentAgenda->next = agendaList->head;
        agendaList->head = currentAgenda;
    }

    fclose(file);
    printf("Agenda importe avec succes depuis le fichier %s.\n", fileName);

    Agenda* current = agendaList->head;
    while (current != NULL) {
        printf("Contact: %s %s\n", current->name, current->surname);
        
        MeetingNode* meeting = current->meetings;
        while (meeting != NULL) {
            printf("    Meeting level %d: %02d/%02d/%04d %02d:%02d:%02d\n",
                    meeting->meeting.level,
                    meeting->meeting.day, meeting->meeting.month, meeting->meeting.year,
                    meeting->meeting.hour, meeting->meeting.minute, meeting->meeting.second);
            meeting = meeting->next;
        }

        current = current->next;
    }

    return agendaList;
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


// Fonction pour liberer la memoire de la liste d'agenda
void freeAgendaList(LevelAgenda* agendaList) {
    if (agendaList == NULL) {
        return;
    }

    Agenda* currentAgenda = agendaList->head;
    while (currentAgenda != NULL) {
        MeetingNode* currentMeeting = currentAgenda->meetings;
        while (currentMeeting != NULL) {
            MeetingNode* nextMeeting = currentMeeting->next;
            free(currentMeeting);
            currentMeeting = nextMeeting;
        }

        Agenda* nextAgenda = currentAgenda->next;
        free(currentAgenda);
        currentAgenda = nextAgenda;
    }

    free(agendaList);
}
