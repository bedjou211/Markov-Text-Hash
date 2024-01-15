
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include "hachage.h"
#include "markov.h"
#include "hachage.c"


markov_t creer_chaine(unsigned int graine){
    markov_t chaine = malloc(sizeof(struct markov_s));
    chaine->hash_table = creer_table(10);
    chaine->matrice = NULL;
    srand(graine);
    return chaine;
}



void detruire_chaine(markov_t chaine) {
    if (chaine == NULL) {
        return;
    }
    detruire_table(chaine->hash_table);
    detruire_etats(chaine->matrice);
    free(chaine);
}



void ajouter_etat(markov_t chaine, int etat_id) {
    if (chaine != NULL && etat_id != -1 ) {
        etat etat_existant = trouver_etat(chaine->matrice, etat_id);
        if (etat_existant == NULL) {
            etat nouvel_etat = creer_etat();
            if (nouvel_etat != NULL) {
            	
                nouvel_etat->id_etat = etat_id;
                nouvel_etat->suivant = chaine->matrice;
                chaine->matrice = nouvel_etat;
            } else {
                printf("Erreur d'allocation mémoire");
                return EXIT_FAILURE;
            }
        }
    } else {
       return;
    }
}


void augmenter_poids(markov_t chaine, int id_etat1, int id_etat2) {
    if (chaine != NULL && id_etat1 != -1) {
        etat etat1 = trouver_etat(chaine->matrice, id_etat1);
        if (etat1 != NULL) {
            transition etat2_transition = trouver_transition(etat1->liste_transitions, id_etat2);
            if (etat2_transition == NULL) {
                ajouter_etat(chaine, id_etat2);
                ajouter_transition(etat1, id_etat2);
            } else {
                etat2_transition->poids++;
            }
        }
    }
}

double proba_transition(markov_t chaine, int id_etat1, int id_etat2) {
    etat etat_courant = trouver_etat(chaine->matrice, id_etat1);
    if (etat_courant==NULL){
        return -1;
    }


    transition etat2 = trouver_transition(etat_courant->liste_transitions, id_etat2);
    if (etat2 == NULL) {
        return -1;
    }
    int poids_transition = etat2->poids;
    int poids_total = 0;
    transition trans = etat_courant->liste_transitions;

    while (trans != NULL) {
        poids_total += trans->poids;
        trans = trans->suivant;
    }

    if (poids_total == 0) {
        return 0.0;
    }

    return (double)poids_transition / (double)poids_total;
}


void tmatrice_proba(markov_t chaine) {
    etat etat_courant = chaine->matrice;
    while (etat_courant != NULL) {
        transition transition_courante = etat_courant->liste_transitions;
        double poids_total = 0.0;
        while (transition_courante != NULL) {
            poids_total += transition_courante->poids;
            transition_courante = transition_courante->suivant;
        }

        transition_courante = etat_courant->liste_transitions;
        while (transition_courante != NULL) {
            if (poids_total != 0) {
                double proba = transition_courante->poids / poids_total;
                transition_courante->poids = (double) proba;
            } else {
                transition_courante->poids = 0.0;
            }
            transition_courante = transition_courante->suivant;
        }
        etat_courant = etat_courant->suivant;
    }
}




int deplacement_aleatoire(markov_t chaine_markov, int id_etat) {
    etat etat_courant = trouver_etat(chaine_markov->matrice, id_etat);
    if (etat_courant == NULL || etat_courant->liste_transitions == NULL) {
        printf( "le motexiste pas dans la chaine");
        return -1;
    }
    int poids_total = 0;
    transition current_transition = etat_courant->liste_transitions;
    while (current_transition != NULL) {
        poids_total += current_transition->poids;
        current_transition = current_transition->suivant;
    }
    int poids_aleatoire = rand() % (poids_total + 1);
    current_transition = etat_courant->liste_transitions;
    int poids_cumule = 0;
    while (current_transition != NULL) {
        poids_cumule += current_transition->poids;
        if (poids_cumule >= poids_aleatoire) {
            return current_transition->cible;
        }
        current_transition = current_transition->suivant;
    }

    return -1;
}

/*****************************************************************/
/***************** Fonctions  auxiliare **************************/



etat creer_etat() {
    etat nouvel_etat = (etat) malloc(sizeof(struct etat_s));
    nouvel_etat->liste_transitions = NULL;
    nouvel_etat->suivant = NULL;
    return nouvel_etat;
}


etat trouver_etat(etats liste,int id_etat) {
    etat etat_courant = liste;
    while (etat_courant != NULL && etat_courant->id_etat != id_etat) {
        etat_courant = etat_courant->suivant;
    }
    return etat_courant;
}

void detruire_transitions(transitions trans) {
    transition tmp;
    while (trans != NULL) {
        tmp = trans;
        trans = trans->suivant;
        free(tmp);
    }
}

void detruire_etats(etats et) {
    etat tmp;
    while (et != NULL) {
        tmp = et;
        et = et->suivant;
        detruire_transitions(tmp->liste_transitions);
        free(tmp);
    }
}

void ajouter_transition(etat et,int cible) {
    transition nouvelle_transition = (transition) malloc(sizeof(struct transition_s));
    nouvelle_transition->cible = cible;
    nouvelle_transition->poids = 1;
    nouvelle_transition->suivant = et->liste_transitions;
    et->liste_transitions = nouvelle_transition;
}



transition trouver_transition(transitions liste,    int cible) {
    transition trans = liste;
    while (trans != NULL && trans->cible != cible) {
        trans = trans->suivant;
    }
    return trans;
}



void afficher_chaine_markov(markov_t chaine_markov) {
    etat etat_courant;
    transition transition_courante;

    if (chaine_markov == NULL) {
        printf("La chaîne de Markov est vide.\n");
        return;
    }

    printf("Chaîne de Markov :\n");

    etat_courant = chaine_markov->matrice;
    while (etat_courant != NULL) {
        printf("Etat %u:\n", etat_courant->id_etat);

        transition_courante = etat_courant->liste_transitions;
        while (transition_courante != NULL) {
            printf(" -> %u = %u\n", transition_courante->cible, transition_courante->poids);
            transition_courante = transition_courante->suivant;
        }

        etat_courant = etat_courant->suivant;
    }

    etat_courant = chaine_markov->matrice;
    while (etat_courant != NULL) {
        printf("Etat %u:\n", etat_courant->id_etat);

        transition_courante = etat_courant->liste_transitions;
        while (transition_courante != NULL) {
            printf(" -> %u = %f\n", transition_courante->cible,proba_transition(chaine_markov,etat_courant->id_etat,transition_courante->cible));
            transition_courante = transition_courante->suivant;
        }

        etat_courant = etat_courant->suivant;
    }
    
}


/*
int main(int argc, char *argv[])
{
    printf("\n Compile \n");
    markov_t chaine = creer_chaine((int) time(NULL));


for (int i = 0 ; i < 1000; i++){
    ajouter_etat(chaine,i);
    augmenter_poids(chaine,i,i+1);
}
for (int i = 0 ; i < 40000; i++){
    ajouter_etat(chaine,i);
    augmenter_poids(chaine,i,i+1);
}
for (int i = 0 ; i < 1000; i++){
    ajouter_etat(chaine,i);
    augmenter_poids(chaine,i,i+1);
}

for (int i = 0 ; i < 100; i++){
    ajouter_etat(chaine,i);
    augmenter_poids(chaine,i,i+5);
}
    augmenter_poids(chaine,10,1);
    afficher_chaine_markov(chaine);
    
    printf(" la probabilite entre  yani et louheb est de %f \n",proba_transition(chaine,0,1));


    int etat_depart = 0 ;
    int etat_arrive = deplacement_aleatoire(chaine,etat_depart);
    printf("%d ----> %d \n",etat_depart,etat_arrive);
    etat_arrive = deplacement_aleatoire(chaine,etat_arrive);
    for (int i = 0 ; i < 10; i++){
    printf("%d ----> %d \n",etat_depart,etat_arrive);
    etat_arrive = deplacement_aleatoire(chaine,etat_arrive);
    }

    
    
    
    detruire_chaine(chaine);
     
    return EXIT_SUCCESS;
}
*/
/*******amelioration *******************************************************************/


void enregistrer_matrice(markov_t chaine, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    etat current_etat = chaine->matrice;
    while (current_etat != NULL) {
        fwrite(&(current_etat->id_etat), sizeof(int), 1, file);
        transition current_transition = current_etat->liste_transitions;
        while (current_transition != NULL) {
            fwrite(&(current_transition->cible), sizeof(int), 1, file);
            fwrite(&(current_transition->poids), sizeof(int), 1, file);
            current_transition = current_transition->suivant;
        }
        int end_marker = -1; // marquer la fin des transtion 
        fwrite(&end_marker, sizeof(int), 1, file);

        current_etat = current_etat->suivant;
    }
    int end_marker = -1; // marquer la fin des etats 
    fwrite(&end_marker, sizeof(int), 1, file);

    fclose(file);
}



markov_t charger_matrice(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier");
        return NULL;
    }

    markov_t chaine = (markov_t)malloc(sizeof(struct markov_s));
    chaine->matrice = NULL;
    chaine->hash_table = NULL; // Initialisez la hash_table si nécessaire

    int id_etat;
    while (fread(&id_etat, sizeof(int), 1, file) == 1 && id_etat != -1) {
        etat new_etat = (etat)malloc(sizeof(struct etat_s));
        new_etat->id_etat = id_etat;
        new_etat->liste_transitions = NULL;
        new_etat->suivant = chaine->matrice;
        chaine->matrice = new_etat;

        int cible, poids;
        while (fread(&cible, sizeof(int), 1, file) == 1 && cible != -1) {
            fread(&poids, sizeof(int), 1, file);
            transition new_transition = (transition)malloc(sizeof(struct transition_s));
            new_transition->cible = cible;
            new_transition->poids = poids;
            new_transition->suivant = new_etat->liste_transitions;
            new_etat->liste_transitions = new_transition;
        }
    }

    fclose(file);
    return chaine;
}

