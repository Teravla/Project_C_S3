#include <stdbool.h>

#ifndef AGENDA_H
#define AGENDA_H


// Définition de la structure du meeting
typedef struct Meeting {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    int level;
} Meeting;

// Définition de la structure du nœud de réunion
typedef struct MeetingNode {
    Meeting meeting;
    struct MeetingNode* next;
} MeetingNode;

// Définition de la structure de l'agenda
typedef struct Agenda {
    char name[100];
    char surname[100];

    int column; // Indice de la colonne dans laquelle se trouve la cellule

    MeetingNode* meetings; // Liste chaînée de réunions

    // Ajout du tableau next pour représenter les colonnes
    struct Agenda* next; 
} Agenda;

// Définition de la structure du niveau de l'agenda
typedef struct LevelAgenda {
    int maxLevels;
    Agenda* head;
} LevelAgenda;




//Creation de l'Agenda
LevelAgenda* createLevelAgenda(int maxLevels);
Agenda* createCellAgenda(const char* name, const char* surname, int column);
MeetingNode* createMeeting(int day, int month, int year, int hour, int minute, int second, int level);


//Gestion des contacts
LevelAgenda* addContact(LevelAgenda* myList);
LevelAgenda* deleteContact(LevelAgenda* myList);


//Gestion des Meetings
LevelAgenda* addMeeting(LevelAgenda* list);
LevelAgenda* deleteMeeting(LevelAgenda* list);



//Recherche
void searchContact(LevelAgenda* list);
void searchMeeting(LevelAgenda* list);


// Sorting 
void swap(Agenda* a, Agenda* b); 
LevelAgenda* sortAgenda(LevelAgenda* list);

void sortAgendaAlphabetically(LevelAgenda* list);

void sortAgendaMeeting(LevelAgenda* list);
bool hasMeeting(Agenda* agenda, int level);
void moveMeeting(Agenda* agenda, int fromLevel, int toLevel);
void insertMeeting(Agenda* agenda, MeetingNode* newMeeting);

//Affichage
int countCharName(LevelAgenda* list, Agenda* contact);
int countCharSurname(LevelAgenda* list, Agenda* contact);
void capitalizeString(char* str);
int countCellsAgenda(LevelAgenda* list);

void printAllLevelsAgenda(LevelAgenda* list, int numberOfCells);
void printLevelAgenda(LevelAgenda* list, int level, int numberOfCells);




//Other
int maxColumnsAgenda(LevelAgenda* list);
Agenda* obtenirCelluleAgenda(LevelAgenda* list, int column);


#endif