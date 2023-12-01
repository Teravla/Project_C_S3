LevelList* addContact(LevelList* myList) {
    Cell* newCell = createCell(0, 1);

    printf("Entrer le nom du contact sous la forme (nom_prenom) : ");
    scanf("%99s", newCell->value);

    // À ce stade, newCell->name et newCell->surname contiennent le nom et le prenom separes
    // Inserer la nouvelle cellule dans la liste
    insertAtHead(myList, newCell);

    AFFICHER LE CONTENU DE LA CELLULE
}