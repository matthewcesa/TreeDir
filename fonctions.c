#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "fonctions.h"

#define MAX_COMMAND_LENGTH 100

noeud courant = {1, "racine", NULL, &courant, NULL};

bool est_present(bool est_dossier, char nom[]) {
    liste_noeud *l = courant.fils;
    while (l != NULL) {
        if (l->no->est_dossier == est_dossier && strcmp(l->no->nom, nom) == 0) {
            //printf("Oui il est bien present");
            return true;
        }
        l = l->succ;
    }
    //printf("Non il n'est pas present");
    return false;
}

// Fonction qui retourne le nœud courant parent
noeud* get_pere(noeud *current_node) {
    return current_node->pere;
}

// Fonction qui retourne le nœud courant correspondant au chemin spécifié
noeud* get_node_by_path(char *path, noeud *current_node) {
    if (strlen(path) == 0 || strcmp(path, "/") == 0) {
        return current_node->racine;
    }
    if (path[0] == '/') {
        return get_node_by_path(&path[1], current_node->racine);
    }
    char *subpath = strtok(path, "/");
    while (subpath != NULL) {
        if (strcmp(subpath, "..") == 0) {
            current_node = current_node->pere;
        } else {
            struct liste_noeud *fils = current_node->fils;
            while (fils != NULL) {
                if (strcmp(fils->no->nom, subpath) == 0) {
                    current_node = fils->no;
                    break;
                }
                fils = fils->succ;
            }
            if (fils == NULL) {
                return NULL; // Le nœud spécifié n'a pas été trouvé
            }
        }
        subpath = strtok(NULL, "/");
    }
    return current_node;
}

// Fonction qui change le nœud courant de navigation
void change_current_node(noeud *nv) {
    courant = *nv;
}

// Fonction qui traite la commande "cd" passée en paramètre
void cd(char *commande) {
    printf("cd appele \n");
    if (strcmp(commande, "cd") == 0) {
        // Retourner à la racine
        change_current_node(courant.racine);
    } else if (strcmp(commande, "cd ..") == 0) {
        // Aller au nœud parent
        noeud *parent = get_pere(&courant);
        if (parent != NULL) {
            change_current_node(parent);
        }
    } else if (strncmp(commande, "cd ", 3) == 0) {
        // Aller au nœud spécifié par le chemin
        noeud *new_current_node = get_node_by_path(&commande[3], &courant);
        if (new_current_node != NULL) {
            change_current_node(new_current_node);
        } else {
            printf("Le chemin spécifié n'existe pas\n");
        }
    } else {
        printf("Commande non reconnue\n");
    }
}

//construire chemin
void build_path(char *path, noeud *current_node) {
    if (current_node->pere == NULL) {
        strcat(path, "/");
        strcat(path, current_node->nom);
        return;
    }
    build_path(path, current_node->pere);
    strcat(path, "/");
    strcat(path, current_node->nom);
}

//pwd
char* pwd(noeud* current_node) {
    char *path = malloc(1000 * sizeof(char)); // Allouer suffisamment de mémoire pour stocker le chemin absolu
    memset(path, '\0', sizeof(path)); // Initialiser la chaîne de caractères à vide
    build_path(path, current_node);
    return path;
}

void ls() {
    printf("Contenu du dossier courant :\n");

    liste_noeud* courant_fils = courant.fils;

    while (courant_fils != NULL) {
        if (courant_fils->no->est_dossier) {
            printf("%s [dossier]\n", courant_fils->no->nom);
        } else {
            printf("%s [fichier]\n", courant_fils->no->nom);
        }
        courant_fils = courant_fils->succ;
    }
}

//pour l'instant les noms des nv noeuds ne peuvent pas avoir d'espace
void cree(char commande[]) {
    char copy[MAX_COMMAND_LENGTH];
    strcpy(copy, commande);

    char* action = strtok(commande, " ");
    char* nom = strtok(NULL, " ");

    printf("action %s\n", action);
    printf("nom %s\n", nom);

    if (action == NULL && nom == NULL) {
        printf("Erreur : commande invalide\n");
        return;
    }

    noeud* nouveau_noeud = (noeud*) malloc(sizeof(noeud));
    if (nouveau_noeud == NULL) {
        printf("Erreur : impossible de creer un nouveau noeud\n");
        return;
    }

    if (strcmp(action, "mkdir") == 0) {
        //printf("mkdir appele\n");
        nouveau_noeud->est_dossier = true;
    } else if (strcmp(action, "touch") == 0) {
        //printf("touch appele\n");
        nouveau_noeud->est_dossier = false;
    } else {
        printf("Erreur : commande invalide\n");
        free(nouveau_noeud);
        //cd(copy);
        return;
    }
    if(est_present(nouveau_noeud->est_dossier, nom)) {
        printf("Erreur : Le noeud a inserer est deja present\n");
        return;
    }
    strcpy(nouveau_noeud->nom, nom);
    nouveau_noeud->pere = &courant;
    nouveau_noeud->racine = courant.racine;
    nouveau_noeud->fils = NULL;

    liste_noeud* nouvel_element = (liste_noeud*) malloc(sizeof(liste_noeud));
    if (nouvel_element == NULL) {
        printf("Erreur : impossible de creer un nouvel élément de liste\n");
        free(nouveau_noeud);
        return;
    }

    nouvel_element->no = nouveau_noeud;
    nouvel_element->succ = courant.fils;
    courant.fils = nouvel_element;

    printf("%s cree avec succes\n", nouveau_noeud->est_dossier ? "Dossier" : "Fichier");
}


//PRINT

int count_fils(noeud *n) {
    int count = 0;
    liste_noeud *cur = n->fils;
    while (cur != NULL) {
        count++;
        cur = cur->succ;
    }
    return count;
}


int nbMots(char *chemin){
    int count = 0;
    char *token = strtok(chemin, "/");
    
    while (token != NULL) {
        count++;
        token = strtok(NULL,"/");
    }
    return count;
}

void rmAux(noeud* n){
    liste_noeud* tmp = n->fils;
    while(tmp!=NULL) {
        rmAux(tmp->no);
        tmp = tmp->succ;
    }
    free(n->fils);
    free(n);
}

//efface un noeud ou un dossier
void rm(char* chemin, noeud* courant) {
    if (strlen(chemin) == 0 || strcmp(chemin,"/") == 0){
        printf("Erreur : veuillez renseignez le(s) fichier(s)/dossier(s) que vous voulez supprimer");
        return;
    }
    // if(nbMots(chemin) <= 1) { // cad que si on a quun seul nom 
    //     if(strcmp(courant->fils->no->nom,strtok(chemin,"/")) == 0){
    //         //on regarde si le fichie/dossier du courant correspond au chemin
    //         if(count_fils(courant) == 1){
    //             //si c'est un fichier ou dossier vide
    //             free(courant->fils->no);
    //             // free(courant->fils);
    //         }else{
    //             rmAux(courant);
    //         }
    //     }
        //le cas chemin absolu
        char* cheminSecondaire = strtok(chemin,"/");
        while(cheminSecondaire != NULL){ 
        //on doit aller jusqu au dernier fichier ou dossier du chemin
            liste_noeud* successeur = courant->fils;
            while(successeur != NULL) {
                //parcours de la liste de noeuds cad les fils du courant
                if(strcmp(cheminSecondaire,successeur->no->nom) == 0) {
                    //verifie si le nom du chemin secondaire et le meme que le nom du noeud
                    courant = successeur->no;
                    break;
                }
                successeur = successeur->succ;
                if(successeur == NULL){ //si on arrive au dernier fils de courant et que le chemin ne correspond pas
                    printf("Erreur, le chemin est erroné.");
                    return;
                }
            }
            cheminSecondaire = strtok(NULL,"/");
        }
        noeud* tmp = courant;
        courant = courant->pere;
        liste_noeud* tempoFils = courant->fils;
        if(tempoFils->no == tmp && tempoFils->succ == NULL){
            //si le noeud suivant est nul et donc si le noeud voulu est le premier
            //on fait du cas par cas
            courant->fils = NULL;
        }
        else if(tempoFils->no == tmp){
            //le noeud a efface le premier et qu'il y a d'autre noeud apres
            courant->fils = courant->fils->succ;
        }else{
            while(tempoFils->succ->no != tmp) {
                tempoFils = tempoFils->succ;
            }
        tempoFils->succ = tempoFils->succ->succ;
        }
        rmAux(tmp);
    
}
noeud* get_noeud_chemin(noeud* a, char chemin[]) {
    printf("get_noeud_chemin de %s \n", chemin);
    if (chemin[0] != '/') {
        printf("Chemin invalide. Le chemin doit commencer par '/'.\n");
        return NULL;
    }
    
    char chemin_copie[100];
    strcpy(chemin_copie, chemin);
    
    char* segment = strtok(chemin_copie, "/");
    while (segment != NULL) {
        if (strcmp(segment, "..") == 0) {
            if (a->pere != NULL) {
                a = a->pere;
            } else {
                printf("Impossible de remonter plus haut que la racine.\n");
                return NULL;
            }
        } else {
            liste_noeud* fils_actuel = a->fils;
            while (fils_actuel != NULL) {
                if (strcmp(fils_actuel->no->nom, segment) == 0) {
                    a = fils_actuel->no;
                    break;
                }
                fils_actuel = fils_actuel->succ;
            }
            
            if (fils_actuel == NULL) {
                printf("Le chemin spécifié est invalide.\n");
                return NULL;
            }
        }
        
        segment = strtok(NULL, "/");
    }
    
    return a;
}

void cp(char commande[]) {
    char chemin1[100];
    char chemin2[100];
    sscanf(commande, "cp %s %s", chemin1, chemin2);

    noeud* source = NULL;
    noeud* destination = NULL;

    if (est_present(true, chemin1)) {
        // Le chemin1 correspond à un dossier fils du nœud courant
        source = get_noeud_chemin(&courant, chemin1);
    } else if (est_present(false, chemin1)) {
        // Le chemin1 correspond à un fichier fils du nœud courant
        source = get_noeud_chemin(&courant, chemin1);
    } else {
        // Le chemin1 est un chemin complet "/racine/dossier/noeud"
        source = get_noeud_chemin(&courant, chemin1);
    }

    if (est_present(true, chemin2)) {
        // Le chemin2 correspond à un dossier fils du nœud courant
        destination = get_noeud_chemin(&courant, chemin2);
    } else {
        // Le chemin2 est un chemin complet "/racine/dossier/noeud"
        destination = get_noeud_chemin(&courant, chemin2);
    }

    if (source == NULL) {
        printf("Le nœud source n'existe pas.\n");
        return;
    }

    if (destination == NULL) {
        printf("Le nœud destination n'existe pas.\n");
        return;
    }

    // Copie du nœud source
    noeud* copie = malloc(sizeof(noeud));
    copie->est_dossier = source->est_dossier;
    strcpy(copie->nom, source->nom);
    copie->pere = destination;
    copie->racine = destination->racine;
    copie->fils = NULL;

    // Ajouter la copie à la liste des fils du nœud destination
    liste_noeud* nouveau_fils = malloc(sizeof(liste_noeud));
    nouveau_fils->no = copie;
    nouveau_fils->succ = destination->fils;
    destination->fils = nouveau_fils;

    printf("La copie du noeud a ete creee avec succes.\n");
}
// //verifie si le chemin existe bien
// bool verifChemin(char* chem, noeud* nv) {
//     char* cheminSecondaire = strtok(chem,"/");
//     while(cheminSecondaire != NULL){ 
//     //on doit aller jusqu au dernier fichier ou dossier du chemin
//         liste_noeud* successeur = nv->fils;
//         while(successeur != NULL) {
//             //parcours de la liste de noeuds cad les fils du courant
//             if(strcmp(cheminSecondaire,successeur->no->nom) == 0) {
//                 //verifie si le nom du chemin secondaire et le meme que le nom du noeud
//                 nv = successeur->no;
//                 break;
//             }
//             successeur = successeur->succ;
//             if(successeur == NULL){ //si on arrive au dernier fils de courant et que le chemin ne correspond pas
//                 return false;
//             }
//         }
//         cheminSecondaire = strtok(NULL,"/");
//     }
//     return true;
// }

// noeud* creerNoeud(noeud* n, noeud* pere){
//     noeud* nv = (noeud*) malloc(sizeof(noeud));
//     nv->est_dossier=n->est_dossier;
//     char* nom = malloc(sizeof(char)*MAX_COMMAND_LENGTH);
//     nom = strcpy(nv->nom,n->nom);
//     nv->pere = pere;
//     nv->racine = n->racine;
//     nv->fils = n->fils;

//     return nv;
// }

// liste_noeud* creerListe(liste_noeud* l) {
//     liste_noeud* liste = (liste_noeud*) malloc(sizeof(liste_noeud));
//     liste->no = l->no;
//     liste->succ = l->succ;

//     return liste;
// }
// void enregistre_tous_les_fils(noeud* courant){ //copie tous les fils de courant mais pas courant
//     if(courant->fils == NULL){
//         return;
//     }else{
//         noeud* tmp = courant;
//         while(tmp->fils != NULL){

//         }
//     }
// }
// //déplace le noeud 
// void mv(char* chem1, char* chem2, noeud* nv) {
//     if (nv == nv->racine){
//         if(verifChemin(chem1,nv->racine) && verifChemin(chem2,nv->racine)){

//         }else{
//             printf("Le(s) chemins sont erronés, veuillez réessayer avec un/des chemin(s) valable(s)")
//         }
//     }else{
//         if(verifChemin(chem1,nv) && verifChemin(chem2,nv)){

//         }else{
//             printf("Le(s) chemins sont erronés, veuillez réessayer avec un/des chemin(s) valable(s)")
//         }
//     }
// }

// void mv(char commande[]) {
//     cp(commande);
//     char chemin1[100];
//     char chemin2[100];
//     sscanf(commande, "cp %s %s", chemin1, chemin2);
//     char* cheminSecondaire = strtok(chemin1,"/");
//     while(cheminSecondaire != NULL){ 
//     //on doit aller jusqu au dernier fichier ou dossier du chemin
//         liste_noeud* successeur = courant->fils;
//         while(successeur != NULL) {
//             //parcours de la liste de noeuds cad les fils du courant
//             if(strcmp(cheminSecondaire,successeur->no->nom) == 0) {
//                 //verifie si le nom du chemin secondaire et le meme que le nom du noeud
//                 nv = successeur->no;
//                 break;
//             }
//             successeur = successeur->succ;
//             if(successeur == NULL){ //si on arrive au dernier fils de courant et que le chemin ne correspond pas
//                 return false;
//             }
//         }
//         cheminSecondaire = strtok(NULL,"/");
//     }
//     rm(chemin1, cheminSecondaire);
// }

void print_rec(noeud *n, int prof) {
    int i;
    for (i = 0; i < prof; i++) {
        printf("  ");
    }
    printf("Noeud %s (%s), pere: %s, %d fils: ",
           n->nom, n->est_dossier ? "D" : "F",
           n->pere == NULL ? "/" : n->pere->nom,
           count_fils(n));
    liste_noeud *cur = n->fils;
    while (cur != NULL) {
        printf("%s", cur->no->nom);
        if (cur->no->est_dossier) {
            printf(" (D)");
        } else {
            printf(" (F)");
        }
        if (cur->succ != NULL) {
            printf(", ");
        }
        cur = cur->succ;
    }
    printf("\n");
    cur = n->fils;
    while (cur != NULL) {
        print_rec(cur->no, prof+1);
        cur = cur->succ;
    }
}

void print() {
    printf("Noeud %s (%s), %d fils: ", courant.nom, courant.est_dossier ? "D" : "F",count_fils(&courant));
    liste_noeud *cur = courant.fils;
    while (cur != NULL) {
        printf("%s", cur->no->nom);
        if (cur->no->est_dossier) {
            printf(" (D)");
        } else {
            printf(" (F)");
        }
        if (cur->succ != NULL) {
            printf(", ");
        }
        cur = cur->succ;
    }
    printf("\n");
    cur = courant.fils;
    while (cur != NULL) {
        print_rec(cur->no, 1);
        cur = cur->succ;
    }
}
