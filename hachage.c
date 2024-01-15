/*Fichier : hachage.c
  Auteur : bedjou celina 22112538
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"hachage.h"
   


hachage creer_table(unsigned int taille){
    if (taille == 0){
        printf("Taille invalide \n");
        return NULL;
    }
    hachage hash_table = malloc(sizeof(struct hachage_t ));
    if(hash_table == NULL){
        printf("erreur allocation mémoire \n");
        return NULL;
    }
    hash_table->tab = calloc(taille ,sizeof(struct element_t*)); 
    if(hash_table->tab == NULL){
        printf("erreur allocation mémoire \n");
        return NULL;
    }
    hash_table->nb_etats=0;
    hash_table->size=taille;
    for (int i=0; i<taille; i++) {
        hash_table->tab[i] = NULL;
    }
    return hash_table;
}

void detruire_table(hachage hash_table){
    if(hash_table == NULL){
        return;
    }
    else{
        if(hash_table->tab!=NULL){
            for (int i=0; i<hash_table->size; i++) {
                element tmp1 =  hash_table->tab[i];
                while(tmp1!=NULL){
                    element tmp2=tmp1->suivant;
                    
                    free(tmp1->mot);
                    free(tmp1);
                    tmp1=tmp2;
                   
                }   
            }
            free(hash_table->tab);
        }
        free(hash_table);
    }
}

int hash(char* mot,unsigned int taille){
   int somme = 0;
    for(int i=0;i<strlen(mot);i++){
        somme = somme +  mot[i]*(int)pow(p,i);
    }
    return somme % taille;

}

void inserer_mot(char* mot,hachage hash_table){
    if(hash_table !=NULL && mot!=NULL &&  mot!=""){
        int index = hash(mot,hash_table->size);
        if(hash_table->tab[index] == NULL){   
            hash_table->tab[index] = (struct element_t*) malloc(sizeof(struct element_t));
            if(hash_table->tab[index] !=NULL){
                
                hash_table->tab[index]->mot  = (char*) malloc(strlen(mot) +1);
                strcpy(hash_table->tab[index]->mot , mot);
                hash_table->tab[index]->etat=hash_table->nb_etats;
                hash_table->tab[index]->suivant=NULL;
                hash_table->nb_etats++;
            }
            else{
                printf("Erreur allocation mémoire\n");
                return EXIT_FAILURE;
            }
        }
        else{
            if(position_mot(mot,hash_table) == -1){
                Liste tmp = hash_table->tab[index] ; 
                while(tmp->suivant!=NULL){
                    tmp = tmp->suivant;
                }
                tmp->suivant =(struct element_t*) malloc(sizeof(struct element_t));
                tmp->suivant->mot = (char*) malloc(strlen(mot) +1 );
                strcpy(tmp->suivant->mot,mot);
                tmp->suivant->etat = hash_table->nb_etats;
                tmp->suivant->suivant = NULL;
                hash_table->nb_etats++;   
            }
            else{
               
            }   
        }   
    }
}


int position_mot(char* mot,hachage hash_table){
    int index = hash(mot,hash_table->size);
    if(hash_table !=NULL && hash_table->tab!=NULL && mot!=NULL){
        if(rechercher_mot(hash_table->tab[index],mot)==1){
            return index;
        }
        
    }
    return -1;
}
int etat_mot(char* mot, hachage hash_table) {
    if (hash_table != NULL && hash_table->tab != NULL && mot != NULL && mot != "") {
        int index = hash(mot, hash_table->size);
        if (index < hash_table->size && index >= 0) {
            element current = hash_table->tab[index];
            while (current != NULL) {
                if (strcmp(current->mot, mot) == 0) {
                    return current->etat;
                }
                current = current->suivant;
            }
        }
    }
    return -1;
}

/*fonction auxiliaire*/
int rechercher_mot(Liste L, char* x){
    while(L!=NULL){
        if(strcmp(L->mot, x)==0){
            return 1; 
        }
        L=L->suivant;
    }
    return 0;
}


char* retrouver_mot(int etat,hachage hash_table){
    if(hash_table !=NULL && hash_table->tab!=NULL){
        for (int i=0; i<hash_table->size; i++) {
            Liste tmp = hash_table->tab[i] ;
            while(tmp!=NULL){
                if(tmp->etat == etat){
                    return tmp->mot;
                }
                tmp=tmp->suivant;
            }
        }
    }
    return  NULL;
}


void afficheListe(Liste L){
    if (L == NULL) printf("[X]\n");
    while(L != NULL){
        printf("[%s|%d]",L->mot,L->etat);
        L = L->suivant;
        if (L == NULL){
            printf("--> [X]\n");
        }
        else{
            printf("-->");
        }
    }
}

void afficheTableHash(hachage hash_table){
    if(hash_table == NULL){
        printf("HASH TAbLE NULL\n");
        return;
    }
    else{
        if(hash_table->tab ==NULL){
            printf("TABLEAU NULL");
            return;
        }
        for (int i=0; i<hash_table->size; i++) {
            printf("tab[%d] => ",i);
            afficheListe(hash_table->tab[i]);
        }
    }
}
