int nbCharPreviousCell = numberCaracterInCell(list, k);
                printf(" %d", nbCharPreviousCell);
                int totalChars = (char_cell + nbCharPreviousCell +1);
                //printf("%d", numberCaracterInCell(list, k));

                // Affichage des tirets
                for (int i = 0; i < totalChars; i++) {
                    printf("-");
                }