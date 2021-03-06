/* Fichier source contenant les fonctions relatives à la sauvegarde et au */
/* chargement de tableaux de points */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "points.h"
#include "erreur.h"

/* Vérifie l'extension d'un nom de fichier et sa cohérence */
int verif_nom_fic(char *nom_fic){
  /* Les extensions permises sont .dat .txt et .log */
  char ext[3][5] = {".dat", ".txt", ".log"};
  int i = 0, valide = 0;
  if(strlen(nom_fic) < 5){ /* La longueur minimale est 5 (ex : a.dat) */
    err_log("nom de fichier invalide dans la fonction verif_nom_fic");
    return 0;
  }

  /* On parcours le tableau d'extensions */
  while(i < 3 && !valide){
    if(!strcmp(nom_fic+(strlen(nom_fic)-4), ext[i])){
      valide = 1;  /* Si une extension correspond on affect 1 à valide */
    }
    i++;
  }
  if(valide){
    return 1; /* Si une extension correspond on retourne 1 */
  }
  return 0;   /* Sinon on retourne 0 */
}

/* Permet de charger des informations contenues dans un fichier dans un
   tableau de points et de retourner ce dernier */
TabPts *chargement_fichier(char *nom_fic){
  FILE *fic = NULL;
  TabPts *tab;
  double *coord_temp = NULL;
  int i, j, classe, nb_pts, dimension, nbclasse;

  /* On vérifie si le nom du fichier est valide */
  if(!verif_nom_fic(nom_fic)){
    err_log("Erreur, fonction de chargement, nom de fichier invalide");
    return NULL;
  }

  /*on essaye d'ouvrir le fichier dont le nom est en argument de la fonction*/
  fic = fopen(nom_fic, "r");
  if(fic == NULL){  /*si le fichier n'existe pas, on retourne une erreur*/
    err_log("fichier introuvable dans la fonction chargement_fichier");
    return NULL;
  }

  /*si la première ligne ne convient pas, on retourne une erreur*/
  if(fscanf(fic, "%d %d %d", &nb_pts, &dimension, &nbclasse) < 3){
    err_log("fichier mal formatté (premiere ligne)");
    return NULL;
  }

  /*on crée le tableau de points et on charge les elements*/
  tab = creer_tab_pts(dimension, nbclasse);

  /*on crée le tableau de points*/

  /*on alloue dynamiquement un tableau temporaire où seront chargées les
    coordonnées avant d'être insérées dans le point*/
  coord_temp = (double *)malloc(tab->dimension * sizeof(double));
  if(coord_temp == NULL){
    erreur("Erreur d'allocation dans la fonction chargement_fichier");
  }

  for(i = 0; i < nb_pts; i++){
    /*on essaye de saisir la classe du point, sinon on retourne une erreur*/
    if(fscanf(fic, "%d", &classe) != 1){
      err_log("fichier mal formatté (classe)");
      return NULL;
    }

    /*on crée le point dans le tableau*/
    ajouter_point(tab, creer_point(tab->dimension, classe));

    for(j = 0; j < tab->dimension; j++){
      /*on charge les coordonnées dans le tableau temporaire*/
      if(fscanf(fic, "%lf", &coord_temp[j]) != 1){
        err_log("fichier mal formatté (coordonnée)");
        return NULL;
      }
    }
    /*on charge les coordonnées dans le point à l'aide du tableau*/
    ajouter_coord(&tab->tab[i], tab->dimension, coord_temp);
  }

  /*on ferme le fichier*/
  fclose(fic);
  /*on libère le tableau temporaire*/
  free(coord_temp);
  return tab;
}

/* Permet de sauvegarder un tableau de points dans un fichier */
void sauvegarde_fichier(TabPts *tab, char *nom_fic){
  FILE *fic = NULL;
  int i, j;

  /* On vérifie si le nom du fichier est valide */
  if(!verif_nom_fic(nom_fic)){
    erreur("fonction de sauvegarde, nom de fichier invalide");
  }

  /*on vérifie que le fichier est bien créé*/
  if((fic = fopen(nom_fic, "w")) == NULL){
    erreur("Erreur, fonction de sauvegarde, échec d'ouverture de fichier'");
  }

  /*on imprime la premère ligne du fichier qui est un peu particulière*/
  fprintf(fic, "%d %d %d\n", tab->taille, tab->dimension, tab->nbclasse);

  /*on imprime les informations de chaque points*/
  for(i = 0; i < tab->taille; i++){
    fprintf(fic, "%d ", tab->tab[i].classe);
    for(j = 0; j < tab->dimension; j++){
      fprintf(fic, "%lf ", tab->tab[i].coord[j]);
    }
    fprintf(fic, "\n");
  }

  /*on ferme le fichier*/
  fclose(fic);
}
