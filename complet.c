#include "markov.c"
//----------------------------------------- partie 3 -------------------------------------------
struct iterateur_s
{
    char* chemin;
    char* mot;
    long int position_courrante;
    int fin_de_phrase;
    int nb_caracters_speciaux;
};

typedef struct iterateur_s* iterateur_t;

int est_alphabet(char caractere){
    if ((caractere >= 'a' && caractere <= 'z') || (caractere >= 'A' && caractere <= 'Z')){
        return 1;
    }
    return -1;
}

iterateur_t init_iterateur(char* chemin){
    FILE* fichier = fopen(chemin,"r");
    if (fichier == NULL){
        return NULL;
    }
    fclose(fichier);

    iterateur_t iterateur = malloc(sizeof( struct iterateur_s));
    iterateur->mot = malloc(sizeof(char)*51);
    iterateur->chemin = chemin;
    iterateur->position_courrante = 0;
    iterateur->nb_caracters_speciaux = 0;
    iterateur->fin_de_phrase = -1;
    return iterateur;
}

char* mot_courrant(iterateur_t iterateur){
    FILE* fichier = fopen(iterateur->chemin,"r");
    if (fichier == NULL){
        return NULL;
    }
    
    fseek(fichier,iterateur->position_courrante,SEEK_SET);

    iterateur->nb_caracters_speciaux = 0;
    iterateur->fin_de_phrase = -1;
    int i = 0;
    char caractere = fgetc(fichier);

    while (caractere != EOF && caractere != ' '){
        if (est_alphabet(caractere) == 1){
            iterateur->mot[i] = tolower(caractere);
            i++;

        }else if(caractere != '.' || caractere == ' ') {
            iterateur->nb_caracters_speciaux++;
        }
        
        if (caractere == '.'){
            iterateur->fin_de_phrase = 1;
        }
        
        caractere = fgetc(fichier);
    }
    iterateur->mot[i]='\0';
    fclose(fichier);
    return iterateur->mot;
}

int has_next(iterateur_t itrateur){
    FILE* fichier = fopen(itrateur->chemin,"r");

    fseek(fichier,itrateur->position_courrante,SEEK_SET);

    char caractere = fgetc(fichier);
    if (caractere != EOF){
        fclose(fichier);
        return 1;
    }
    fclose(fichier);
    return -1;
}

void next_position(iterateur_t iterateur){
    iterateur->position_courrante+=(strlen(mot_courrant(iterateur))+(iterateur->nb_caracters_speciaux))+1;
}

void detruire(iterateur_t iterateur){
    free(iterateur->mot);
    free(iterateur);
}

struct dirent *dir;

markov_t creer_markov_fichier(char* chemin,unsigned int graine){
    DIR *reprtoir = opendir(chemin);
    markov_t chaine = creer_chaine(graine);
    iterateur_t it;
    
    if (reprtoir) {
        while ((dir = readdir(reprtoir)) != NULL) {
            char* chemin_complet = malloc(sizeof(char)*(strlen(chemin) + strlen(dir->d_name) + 2));
            strcpy(chemin_complet, chemin);
            strcat(chemin_complet, "/");
            strcat(chemin_complet, dir->d_name);
            it = init_iterateur(chemin_complet);
            char* mot_courran = mot_courrant(it);
            next_position(it);
            int id1=0;
            int id2=0;

            while (has_next(it) == 1)
            {   
                inserer_mot(mot_courran,chaine->hash_table);
                id1 = etat_mot(mot_courran,chaine->hash_table);
                ajouter_etat(chaine,id1);



                if (has_next(it)){
                    //printf (" deuxieme %s \n",mot_courrant(it));
                    inserer_mot(mot_courrant(it),chaine->hash_table);
                    id2 = etat_mot(mot_courrant(it),chaine->hash_table);
                    //printf(" %d \n",id2);
                    next_position(it);
                }
                augmenter_poids(chaine,id1,id2);
                 //printf("j'augmente le poids entre etat %d et etat %d \n",id1,id2);
            }
           detruire(it);
           free(chemin_complet);
        }
        closedir(reprtoir);
    }
    return chaine;
}





int main(int argc, char *argv[])
{
    printf("\n Compile \n");
    markov_t ch =creer_markov_fichier("src",(int) time(NULL));
    //markov_t ch= charger_matrice("matrice");
    //enregistrer_matrice(ch,"matrice");
    //afficher_chaine_markov(ch);


    printf(" la probabilité %f \n", proba_transition(ch,0,7));
    int etat_depart = rand()%ch->hash_table->nb_etats ;
    printf("etat e depart %d\n",etat_depart);
    char* mot = retrouver_mot(etat_depart,ch->hash_table);
    printf(" %s \n",mot);


    int etat_arrive = deplacement_aleatoire(ch,etat_depart);
    for (int i = 0 ; i < 100; i++){
    etat_arrive = deplacement_aleatoire(ch,etat_arrive);
    char* mot = retrouver_mot(etat_arrive,ch->hash_table);
    printf(" %s ",mot);
    }
    

    detruire_chaine(ch);
     
    return EXIT_SUCCESS;
}
