#ifndef FONCTIONS_H
#define FONCTIONS_H


struct noeud{
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};

struct liste_noeud{
    struct noeud *no;
    struct liste_noeud *succ;
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

extern noeud courant;


extern void ls();
extern void cd(char commande[]);
extern char* pwd(noeud* current_node);
extern void cree(char commande[]);
extern void rm(char chemin[], noeud* courant);
// extern int nbMots(char chemin[]);
extern void cp(char commande[]);
extern void mv(char commande[]);
extern void print();

#endif // FONCTIONS_H
