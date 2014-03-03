//El Midaoui Mohamed Ayoub
//Bouhjira Youssef

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct s//sructure de sommet
{
    int etiquette;//l'etiquette du sommet
    struct s *suiv;//pointeur vers la valeur suivante
}sommet;

typedef struct arc//sructure d'arc
{
    int s_init;//sommet initial de l'arc
    int s_term;//sommet terminal de l'arc
    int poids;//le poids de l'arc
    struct arc *suiv;//pointeur vers l'arc suivant
}arc;

typedef struct G//structure de graphe
{
    sommet s[MAX];//tableau de sommets
    int L;//longueur du graphe
}G;

//fontion de creation d'une liste chainee a partir d'une variable
sommet *liste_creer(int val)
{
    sommet *l = malloc(sizeof(sommet));
    
    if(!l)
    {
        printf("Erreur d'allocation de mémoire\n");
        exit(1);
    }
    
    l->etiquette = val;
    l->suiv = NULL;
    return l;
}

//fonction qui ajoute une valeur a la fin d'une liste d'arcs
arc *ajout_arc(arc *cible, int a, int b, int c)
{
    arc *a_ajouter = malloc(sizeof(arc));
    a_ajouter->s_init = a;
    a_ajouter->s_term = b;
    a_ajouter->poids = c;
    a_ajouter->suiv = NULL;
    
    if (!cible) return a_ajouter;
    
    arc *tmp = cible;
    while (tmp->suiv)
    {
        tmp = tmp->suiv;
    }
    
    tmp->suiv = a_ajouter;
    return cible;
}

//fonction qui ajoute une valeur a la fin d'une liste chainee
void liste_ajout_fin(sommet **liste, int val)
{
    sommet *courant = *liste;
    sommet *nouveau = &(*liste_creer(val));
    if(!(*liste)) *liste = nouveau;
    else
    {
        while(courant->suiv)  courant = courant->suiv;
        courant->suiv = nouveau;
    }
}

//fonction qui indique si une variable figure dans une liste ou non
int liste_recherche(int val, sommet *liste)
{
    if(!liste) return -1;
    sommet *courant = liste;
    int pos;
    for(pos = 0; courant; ++pos, courant = courant->suiv)
        if(courant->etiquette== val)
            return pos;
    return -1;
}

//fonction qui indique s'il existe une chaine entre 2 elems dans un graphe
int chaine_recherche(int a,int b, G graphe)
{
    int i=0;
    
    if(a == b) return 1;//si collision on arrete
    
    //sinon
    while((i < graphe.L) && (graphe.s[i].etiquette != a))
        i++;//on parcourt le graphe a la recheche de a
    
    sommet *courant = graphe.s[i].suiv;
    while(courant)
    {
        //parcourt de la liste des adjoints de a, a la recherche de b
        if(courant->etiquette == b) return 1;
        courant = courant->suiv;
    }
    
    courant = graphe.s[i].suiv;
    while(courant)
    {
        int j=0;
        while((j < graphe.L) && (graphe.s[j].etiquette != courant->etiquette))
            j++;//passer a l'element suivant
        
        if(liste_recherche(b, graphe.s[j].suiv) == 0)//si pas de resultat
        {
            //refaire la recherche
            if(chaine_recherche(courant->etiquette, b, graphe) == 1) return 1;
        }
        courant = courant->suiv;
    }
    
    return 0;
}


//fonction qui retourne une variable booleenne, selon la connexite du graphe
int connexite(G graphe)
{
    int i=0, j;
    
    //verifier que pour tout couple de sommets, il existe une chaine
    for(i=0; i < graphe.L; i++)
    {
        for(j=0; j < graphe.L; j++)
        {
            if(chaine_recherche(graphe.s[i].etiquette,
                                graphe.s[j].etiquette,
                                graphe) == 0) return 0;
        }
    }
    
    return 1;
}

//fonction qui affiche le contenu d'une liste chainee
void liste_afficher(sommet *liste)
{
    sommet *courant = liste;
    printf("[");
    while (courant)
    {
        printf("%d", courant->etiquette);
        if (courant->suiv) printf("\t");
        courant = courant->suiv;
    }
    printf("]");
}

//fonction qui calcule le nombre de cellule dans une liste chainee
int longueur_liste(sommet *liste)
{
    int i=0;
    sommet *tmp = liste;
    while(tmp)
    {
        tmp = tmp->suiv;
        i++;
    }
    return i;
}

//fonction qui determine l'intersection entre deux listes chainee//
sommet *liste_intersection(sommet *l1, sommet *l2)
{
    if(!l1 && !l2) return NULL;
    sommet *intersect = NULL;
    
    sommet *courant;
    
    if(longueur_liste(l1) > longueur_liste(l2))
    {
        for(courant = l1; courant; courant = courant->suiv )
        {
            if(liste_recherche(courant->etiquette, l2) >= 0)
                liste_ajout_fin(&intersect, courant->etiquette);
        }
    }
    
    else
    {
        for(courant = l2; courant; courant = courant->suiv )
        {
            if(liste_recherche(courant->etiquette, l1) >= 0)
                liste_ajout_fin(&intersect, courant->etiquette);
        }
    }
    return intersect;
}

//fonction qui unifie deux listes chainee en une nouvelle liste chainee
sommet *liste_union(sommet *l1, sommet *l2)
{
    sommet *l_union = NULL;
    sommet *tmp1 = l1;
    sommet *tmp2 = l2;
    
    //on suppose les elements des 2 listes sont distincts(Kruskal verifie)
    while(tmp1)
    {
        liste_ajout_fin(&l_union, tmp1->etiquette);
        tmp1 = tmp1->suiv;
    }
    
    while(tmp2)
    {
        liste_ajout_fin(&l_union, tmp2->etiquette);
        tmp2 = tmp2->suiv;
    }
    
    return l_union;
}

//Fonction qui affiche les sommets d'un graphe et leurs adjoints
void affichrer_graphe(G graphe)
{
    int i;
    sommet *tmp;
    for(i=0; i<graphe.L; i++)
    {
        if(i) printf("\n");
        printf("Le sommet %d est adjoint a :", graphe.s[i].etiquette);
        tmp = graphe.s[i].suiv;
        while(tmp)
        {
            printf(" %d",tmp->etiquette);
            if(tmp->suiv) printf(",");
            else printf(".");
            tmp = tmp->suiv;
        }
    }
}

//Fonction qui affiche les arcs d'un graphe qlc
void affichage_arcs(arc arcs)
{
    arc *tmp = &arcs;
    while(tmp)
    {
        printf("(%d,%d) ", tmp->s_init, tmp->s_term);
        tmp = tmp->suiv;
    }
        
}

//Fonction qui copie une liste de sommets dans une autre
sommet *sommet_copie(sommet *cible, sommet *a_copier)
{
    sommet *crt = a_copier;
    while(crt) liste_ajout_fin(&cible, crt->etiquette);
    return cible;
}

//Fonction principale
int main()
{
    char reponse;
    int i, adjoint;
    G graphe;
    printf("Donner le nombre de sommets du graphe :");
    scanf("%d",&graphe.L);
    
    
    //Remplissage des sommets
    for(i=0; i <= (graphe.L - 1); i++)
    {
        printf("Donner le sommet %d :", i+1);
        scanf("%d", &graphe.s[i].etiquette);
        graphe.s[i].suiv = NULL;
    }
    
    //Remplissage du graphe
    for(i=0; i <= (graphe.L - 1); i++)
    {
        printf("Le sommet %d a-t-il un adjoint ?", graphe.s[i].etiquette);
        getchar();
        scanf("%c",&reponse);
        getchar();
        while(reponse == 'O')
        {
            sommet *tmp = malloc(sizeof(sommet));
            printf("Donner l'adjoint :");
            scanf("%d", &adjoint);
            tmp->etiquette = adjoint;
            tmp->suiv = graphe.s[i].suiv;
            graphe.s[i].suiv = tmp;
            //liste_ajout_fin(&graphe->s[i]), adjoint);
            printf("Le sommet %d a-t-il un autre adjoint ?", graphe.s[i].etiquette);
            getchar();
            scanf("%c",&reponse);
        }
    }
    
    //Affichage du graphe
    printf("\nAffichage du graphe :\n");
    affichrer_graphe(graphe);
    
    //Connexite du graphe
    printf("\n\nConnexite du graphe :\n");
    if(connexite(graphe)==1) printf("Le graphe est connexe !!");
    else printf("Le graphe n'est pas connexe !!");
    
    //Remplissage des arcs
    int s_init, s_term, s_poids, j;
    char r = 'O';
    printf("\n\nRemplissage des arcs (Ordre Croissant):\n");
    arc *U = NULL;
    //arc *courant = &U;
    do{
        printf("Donner un arc (SI,ST,Poid) :");
        getchar();
        scanf("(%d,%d,%d)", &s_init, &s_term, &s_poids);
        U = ajout_arc(U, s_init, s_term, s_poids);
        printf("Voulez-vous ajouter un autre arc :");
        getchar();
        scanf("%c", &r);
    }while(r == 'O');
    
    //Affichage du graphe pricipal
    affichage_arcs(*U);
    
    //Construction du graphe minimal
    arc *T = NULL;
    
    //Initialisation des composantes connexes (dans un tableau)
    sommet *cc[100] ;
    for(j=0; j<100; j++)
        cc[j] = liste_creer(graphe.s[j].etiquette);
    
    
    //Algorithme de Kruscal
    arc *tmp = U;
    while(tmp)//tant que il y a des arcs non exploites
    {
        if(!liste_intersection(cc[tmp->s_init], cc[tmp->s_term]))
        {
            //ajout de l'arc dans le graphe minimal
            T = ajout_arc(T, tmp->s_init, tmp->s_term, tmp->poids);
            
            sommet *copie = NULL;
            copie = sommet_copie(copie, cc[tmp->s_init]);
            
            //fucionner les deux composantes C(i)=C(j)=C(i)UC(j)
            cc[tmp->s_init] = liste_union(cc[tmp->s_init], cc[tmp->s_term]);
            cc[tmp->s_term] = liste_union(copie, cc[tmp->s_term]);
        }
        tmp = tmp->suiv;
    }
    
    //Affichage du chemin minimal
    printf("\nAffichage du chemin minimal :\n");
    affichage_arcs(*T);
}