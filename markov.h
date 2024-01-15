#ifndef __MARKOV_H__
#define __MARKOV_H__

#include <stdlib.h>

struct transition_s {
    int cible;
    int poids;
    struct transition_s* suivant;
};

typedef struct transition_s * transition;
typedef struct transition_s * transitions;

struct etat_s {
    int id_etat;
    transitions liste_transitions;
    struct etat_s *suivant;
};

typedef struct etat_s * etat;
typedef struct etat_s * etats;

struct markov_s {
    hachage hash_table;
    etats matrice;
};

typedef struct markov_s * markov_t;
/***************** Fonctions **************************/

/**
 *Cette fonction renvoie une chaine de marcov intialisé avec une graine .
 *@param graine est une graine de generateur aleatoire .
 *@ensures hash_table->size=taille.
 *@ensures allouer l'èspace memoire pour l'element chaine de type marcov_t.
 *@ensures hash_table->tab est une table de hachage de taille size;
 *@ensures  chaine->matrice=NULL.
 *@ensures chaine->nb_etats = 0;
 *@return return une chaine  de marcov vide
*/
markov_t creer_chaine(unsigned int graine);

/**
 * Cette fonction prend en entrée une chaine de marcov et libère l'espace mémoire occupé par la chaine de type marcov_t;
 * @param chaine est la chaine de marcov .
 * @requires 'chaine' a été initialisé avec creer_creer_chaine.
 * @ensures le nombre d'appel a free depend de la taille (listes chainnées des transitions )+ taille(de la matrice des etats)+ appel a la fonction detruire table.
*/
void detruire_chaine(markov_t chaine);

/**
* Ajoute un nouvel état à la chaîne de Markov avecun id  spécifié
* si l'etat existe deja dans la table elle ne l'ajoute pas .
* @param etat_courant : un pointeur vers l'état auquel ajouter la transition.
* @param cible : l'identifiant de l'état cible de la transition.
* @ensures insertion de mot dans la table de hachage. 
* @ensures creation d'un nouvel etat avec la fonction creer etat .
* @ensures que le id_etat est recuperer a partir de la table de hachage .
* @ensures chaine->matrice = nouvel_etat.
*/
void ajouter_etat(markov_t chaine, int id_etat); 

/**
* Augmente le poids d'une transition entre deux mots dans la chaîne de Markov
* @param chaine_markov : un pointeur vers la chaîne de Markov
* @param id1 : l ididentifiant de mot de départ de la transition
* @param id2 : l ididentifiant de mot d'arrivée de la transition
* @requires
* @ensures si le mot de depart existe pas dans la table  elle l'ajoute  pas a la chaine de markov
* @ensures si le mot d'arrivé existe pas elle l'ajoute a la chaine de markov
* @ensures si le mot d'arrivé n'exite pas dans la liste des mots cible de l'etat1 elle l'ajoute avec la fonction ajouter_transition
* @ensures si le mot d'arrivé existe deja dans la liste des cibles de l'etat1 elle l'incremente sans poids.
*/
void augmenter_poids(markov_t chaine, int id1 , int id2 ); 
/**
* Calcule la probabilité de transition entre deux mots dans la chaîne de Markov
* @param chaine_markov : un pointeur vers la chaîne de Markov
* @param id1 : l ididentifiant de mot de départ de la transition
* @param id2 : l identifiant de  mot d'arrivé de la transition
* @return la probabilite de transition qui est le poids de la cible (id2) devisé par la somme des poids des cible de l'etat de id1.
* @return -1 si un des mot existe pas dans la chaine 
*/ 
double proba_transition(markov_t chaine, int id1, int id2);
/**
* retourne un etat aleatoire apartir d'un etat de depart dans une chaine de marcov 
* @param chaine_markov : un pointeur vers la chaîne de Markov
* @param id : l'identifiant de mot de depart 
* @reqires id est recuperer a partir de la table de hachage  
* @ensures si le mot existe pas elle retoune 0
* @ensures si le mot existe elle retourne un mot aleatoire existat dans ca liste de transition.
*/ 
 int deplacement_aleatoire(markov_t chaine, int id );
/**
transforme les poids des cible en probabilité [0,1]
* @param chaine : une chaine de markov creer avec creer_chaine.
* @ensures si la chaine de markov est NULL elle fait rien 
* @ensures  markov->matrice est NULL elle fait rien 
* @ensures que le poids de chaque cible de chaque etat est devisé par la somme des poids totale des cibles de chaque etat.
*/
void tmatrice_proba(markov_t chaine);



/***************** Fonctions auxiliaires **************************/


/**
* Détruit la liste des transitions
* @param trans : un pointeur vers la liste des transitions à détruire
* @ensures le nombre d'appel a free depend de la taille (listes chainnées des transitions )
*/
void detruire_transitions(transitions trans); 
/**
* Détruit la liste des états
* @param et : un pointeur vers la liste des états à détruire
* @ensures le nombre d'appel a free depend de la taille (listes chainnées des etats)+(listes chainnées des transitions )
*/
void detruire_etats(etats et);
/**
* Crée un nouvel état avec le mot spécifié
* @ensures l'allocation de la memoire pour un element etat.
* @ensures nouvel_etat->liste_transitions = NULL.
* @ensures nouvel_etat->suivant = NULL.
* @return : un pointeur vers une nouvelle instance de etat
*/
etat creer_etat(); 
/**
* Ajoute une nouvelle transition à l'état spécifié
* @param etat_courant : le mot pour lequel ajouter une nouvelle cible
* @param cible : l'identifiant de mot cible que on veut rajouter a la l'etat
* @ensures l'allocation de la memoire pour un element transition
* @ensures nouvelle_transition->cible = cible.
* @ensures nouvelle_transition->poids = 1.
* @ensures et->liste_transitions = nouvelle_transition.
*/
void ajouter_transition(etat et,  int cible); 
/**
* Trouve un état dans une liste d'états en fonction de l'identifiant spécifié
* @param liste_etats : un pointeur vers la liste des états à parcourir
* @param id_etat : l'identifiant de l'état à trouver
* @return : un pointeur vers l'état trouvé, ou NULL si elle ne trouve pas l etat.
*/
etat trouver_etat(etats liste, int id_etat); 
/**
* Trouve une transition dans une liste de transitions en fonction de l'identifiant spécifié
* @param liste_transitions : un pointeur vers la liste des transitions à parcourir
* @param id_cible : l'identifiant de la transition cible à trouver
* @return : un pointeur vers la transition trouvée, ou NULL si la transition n'est pas trouvée
*/
transition trouver_transition(transitions liste_transitions, int id_cible); 



#endif
