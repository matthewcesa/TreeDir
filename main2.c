#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "fonctions.h"

#define MAX_COMMAND_LENGTH 100

char* removeChars(const char* str, int numChars) {
    int len = strlen(str);

    if (numChars >= len) {
        // Si le nombre de caractères à enlever est supérieur ou égal à la longueur de la chaîne,
        // on renvoie une chaîne vide.
        char* emptyStr = malloc(sizeof(char));
        emptyStr[0] = '\0';
        return emptyStr;
    } else {
        // Alloue de la mémoire pour la nouvelle chaîne de caractères
        char* modifiedStr = malloc(sizeof(char) * (len - numChars + 1));

        // Copie les caractères restants dans la nouvelle chaîne de caractères
        strcpy(modifiedStr, str + numChars);

        return modifiedStr;
    }
}
//pour appeler les commandes et renvoyer le boolean determinant si on quitte ou pas
int commandes(char commande[]) {
    char copy[MAX_COMMAND_LENGTH];
    strcpy(copy, commande);
    char *token = strtok(commande, " "); // On sépare la commande de ses arguments

    if (token == NULL) { // Si la commande est vide
        return 0;
    }
    if (strcmp(token, "ls") == 0) { // Si la commande est "ls"
        ls();
        return 0;
    } else if (strcmp(token, "mkdir") == 0) { // Si la commande est "mkdir"
        cree(copy);
        return 0;
    } else if (strcmp(token, "touch") == 0) { // Si la commande est "touch"
        cree(copy);
        return 0;
    } else if (strcmp(token, "cd") == 0) { // Si la commande est "cd"
        cd(copy);
        return 0;
    } else if (strcmp(token, "print") == 0) { // Si la commande est "print"
        print();
        return 0;
    }else if (strcmp(token, "rm") == 0) { // si la commande est "rm"
        char modifiedStr[strlen(copy)];
        strcpy(modifiedStr,copy+3);
        rm(modifiedStr,&courant);
        return 0;
    } else if (strcmp(token, "quit") == 0) { // Si la commande est "quit"
        return 1;
    } else { // Si la commande n'est pas reconnue
        printf("Commande non reconnue.\n");
    }
    return 2;
}


int main() {

    char commande[MAX_COMMAND_LENGTH];
    int stay = 1;

    while(true){
        printf("\n");
        printf("<%s>",pwd(&courant));

        fgets(commande, MAX_COMMAND_LENGTH, stdin);
        commande[strcspn(commande, "\n")] = '\0';
        //printf("%s\n",commande);
        stay = commandes(commande);
        if(stay==1){
            break;
        }
    }
    return 0;
}