/*Fichier : hachage.h
  Auteur : Bedjou Celina 22112538 */
#ifndef __HACHAGE_H__
#define __HACHAGE_H__


/*************************Définition de la structure **************************/


struct element_t {
    char* mot;   
    int etat;
    struct  element_t* suivant;
    
};

typedef struct  element_t * element;
typedef struct  element_t * Liste;

struct hachage_t
{
    struct  element_t ** tab;
    unsigned int size;
    int nb_etats;
};
typedef struct hachage_t * hachage;

#define p 3


/******************************* Fonctions ***********************************/


/**
 *Cette fonction renvoie une table de hachage initialisé à la taille "taille" .
 *@param taille est la taille de la nouvelle table .
 *@ensures hash_table->size=taille.
 *@ensures allouer l'èspace memoire pour l'element hash_table->tab de type element_t.
 *@ensures hash_table->size = taille;
 *@ensures hash_table->nb_etats = 0
 *@ensures pour i allant de 0 a taille-1 hash_table->tab[i] =NULL.
 *@requires taille > 0
 *@return  Un pointeur vers une nouvelle table de hachage vide.
*/
hachage creer_table (unsigned int taille) ;

/**
 * Cette fonction prend en entrée une table de hachage et libère l'espace mémoire occupé par la table de taille size .
 * @param hash_table est la table de hashage.(pointeur vers une table de hachage).
 * @requires 'hash_table' a été initialisé avec creer_table.
 * @ensures le nombre d'appel a free depend de la taille (listes chainnées )+ taille(tableau tab).
 * @ensures Libère la mémoire allouée pour la table de hachage et tous ses éléments.
*/
void detruire_table(hachage hash_table);
/**
 * Fonction de hachage efficace pour les mots définis sur {a, . . . , z}.
 * Cette fonction prend en parametre un mot et la taille de la table de hachage .
 * @param mot est le mot qu'on doit hacher.(pointeur vers une chaine de caractere ).
 * @param taille est la taille de la table de hachage .
 * @ensures resultat>=0 && resultat<taille.
 * @requires taille >0.
 * @return Une valeur de hachage calculée à partir du mot donné.
*/
int hash(char* mot,unsigned int taille);

/**
 * Cette fonction prend un mot(cle) et  l'ajoute dans la table.
 * On suppose que la table de hachage est crée avec la fontion creer_table()
 * on ajoute le mot a l'index = hash(mot) : si l'index est deja occupée(on verifie si le mot qui occupe l'index == le mot qu'on veut ajoute donc on fait rien ) ,sinon alors on l'ajoute a la fin 
 * @param mot est le mot qu'on doit ajouter.(pointeur vers une chaine de caractere ).
 * @param hash_table est la table de hachage.(pointeur vers une table de hachage).
 * @requires  mot!=NULL 
 * @requires hash_table!=NULL
 * @ensures mot sera dans hash_table->tab[hash(mot)] 
 * @ensures mot-> etat = hash_table->nb_etats.
 * @ensures  hash_table->nbetats++.
 */
void inserer_mot(char* mot,hachage hash_table);
/**
 * Cette fonction prend en argument un mot et une table de hachage puis return la position du mot en verifiant :
 * Si le mot existe dans table de hachage alors on return la position (le hash du mot)
 * Sinon on return -1 c'est a dire le mot n'existe pas dans la table 
 * @param mot est le mot qu'on cherche .(pointeur vers une chaine de caractere ).
 * @param hash_table est la table de hachage ou on cherche la position du mot "mot".(pointeur vers une table de hachage).
 * @requires  mot!=NULL .
 * @ensures resultat >= 0 && resultat < hash_table->taille si mot existe dans hash_tab .
 * @return -1 si le mot n'existe pas ,sinon  la position du mot dans la table .
*/
int position_mot(char* mot,hachage hash_table);
/**
 * Cette fonction retourne l'etat d'un mot stocké dans la table de hachage 
 * OU elle associe chaque mot à un identifiant unique
 * Si le mot n'existe pas la fonction retourne -1
 * @param mot le mot qu'on cherche son etat (pointeur vers une chaine de caractere ).
 * @param hash_table la table de hachage (pointeur vers une table de hachage).
 * @ensures resulta >= 0 si mot est dans hash_table 
 * @return -1 si le mot n'existe pas dans la table, sinon sont état
*/
int etat_mot(char* mot,hachage hash_table);
/**
 * Fonction pour retrouver un mot à partir de son identifiant unique
 * la fonction renvoie le mot la cle "etat" si il existe .
 * @param etat est L'identifiant unique du mot.
 * @param hash_table est la table de hachage ou on veut trouver le mot de la valeur "etat".(pointeur vers une table de hachage).
 * @requires etat>=0.
 * @ensures resultat!=NULL si un mot ayant etat existe dans hash_table
 * @return NULL si l'etat n'existe pas dans la table , sinon renvoie le mot correspond a l'etat
*/
char* retrouver_mot(int etat,hachage hash_table);
/**
 *Cette fonction affiche une table de hachage.
 * @param hash_table est la table de hachage .(pointeur vers une table de hachage).
*/
void afficheTableHash(hachage hash_table);


/***************************** Fonctions Auxiliaire **************************/

/**
 * Recherche un mot dans une liste chaînée.
 * @param L La liste chaînée à parcourir.
 * @param x Le mot à rechercher.
 * @requires liste != NULL && mot != NULL , La liste chaînée et le mot ne doivent pas être NULL.
 * @ensures La fonction retourne 1 si le mot est trouvé, 0 sinon.
 * @return 1 si le mot est trouvé ,0 sinon.
*/
int rechercher_mot(Liste L, char* x);
#endif
