//El Midaoui Med Ayoub
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
arc *ajout_arc(arc *cible, arc *a_ajouter)
{
    if (!cible) return a_ajouter;
    
    arc *tmp = cible;
    while (tmp)
    {
        tmp = tmp->suiv;
    }
    
    tmp->s_init = a_ajouter->s_init;
    tmp->s_term = a_ajouter->s_term;
    tmp->poids = a_ajouter->poids;
    tmp->suiv = NULL;
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
        if(chaine_recherche(courant->etiquette, b, graphe) == 1) return 1;
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

void affichrer_graphe(G graphe)
{
    int i;
    sommet *tmp;
    for(i=0; i<graphe.L; i++)
    {
        if(i) printf("\n");
        printf("Le sommet %d est adjoint a :",graphe.s[i].etiquette);
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

void affichage_arcs(arc *arcs)
{
    arc *tmp = arcs;
    while(tmp)
    {
        printf("(%d,%d) ", tmp->s_init, tmp->s_term);
        tmp = tmp->suiv;
    }
        
}
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
    printf("\n\nConnexite du graphe :\n\n");
    if(connexite(graphe)==1) printf("Le graphe est connexe !!");
    else printf("Le graphe n'est pas connexe !!");
    
    //Remplissage des arcs
    char r = 'O';
    printf("\nRemplissage des arcs (Ordre Croissant):\n");
    arc *U = malloc(sizeof(arc));
    do{
        printf("Donner un arc (SI,ST,Poid) :");
        getchar();
        scanf("(%d,%d,%d)", &U->s_init, &U->s_term, &U->poids);
        getchar();
        printf("Voulez-vous ajouter un autre arc :");
        scanf("%c", &r);
    }while(r == 'O');
    U->suiv = NULL;
    //Construction de l'arbre minimal
    arc *T = NULL;
    
    //
    //Initialisation des composantes connexes (dans un tableau)
    sommet cc[100];
    for(i=0; i<100; i++)
        cc[i] = *liste_creer(graphe.s[i].etiquette);
    
    //Algorithme de Kruscal
    arc *tmp = U;
    while(tmp)
    {
        if(liste_intersection(&cc[tmp->s_init], &cc[tmp->s_term]))
        {
            T = ajout_arc(T, tmp);
            cc[tmp->s_init] = *liste_union(&cc[tmp->s_init], &cc[tmp->s_term]);
            cc[tmp->s_term] = *liste_union(&cc[tmp->s_init], &cc[tmp->s_term]);
        }
    }
    
    //Affichage de l'arbre minimal
    affichage_arcs(T);
}