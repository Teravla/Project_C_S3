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


//Recherche
int meetingExists(LevelAgenda* list, Agenda* meeting);

void searchContact(LevelAgenda* list);
void searchMeeting(LevelAgenda* list);
void searchMeetingByDate(LevelAgenda* list);
void searchMeetingByHour(LevelAgenda* list);


// Sorting 
void swap(Agenda* a, Agenda* b);
LevelAgenda* sortAgenda(LevelAgenda* list);

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