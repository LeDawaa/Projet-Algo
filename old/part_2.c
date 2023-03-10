#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

typedef enum { FALSE, TRUE } bool;

//////////////////////////////////// DEFINITION TYPE LISTE ////////////////////////////////////

typedef struct Bloc {
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste;

/////////////////////////////////////// PREDECLARATIONS ///////////////////////////////////////

/* initialise une Liste vide */
void initVide(Liste* L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);

/* longueur en recursif et en iteratif */
int longueur_rec (Liste l);
int longueur_iter (Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec (Liste *L);
void VireDernier_iter (Liste *L);

/////////////////////////////////////// BRIQUES DE BASE ///////////////////////////////////////

void initVide(Liste *L) {
    *L = NULL;
}

bool estVide(Liste l) {
    return l == NULL;
}

int premier(Liste l) {
    return l -> nombre; 
}

int cptListe = 0;

Liste ajoute(int x, Liste l) {
    Liste tmp = (Liste)malloc(sizeof(Bloc));
    tmp -> nombre = x;
    tmp -> suivant = l;
    cptListe++;
    return tmp;
}

void empile(int x, Liste *L) {
      *L = ajoute(x, *L); 
}

Liste suite(Liste l) {
    return l -> suivant;
}


void depile(Liste *L) {
    Liste tmp = *L;
    *L = suite(*L);
    free(tmp);
}

////////////////////////////// AFFICHE, AVEC LES BRIQUES DE BASE //////////////////////////////

void affiche_rec(Liste l) {
    if(estVide(l)) {
        printf("\n");
        return;
    }
    printf("%d ", premier(l));
    affiche_rec(suite(l));
}


void affiche_iter(Liste l) {
    Liste L2 = l;
    while(!estVide(L2)) {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    } printf("\n");
}

////////////////////////////// LONGEUR, SANS LES BRIQUES DE BASE //////////////////////////////

int longueur_rec (Liste l) {
    if (l == NULL) return 0;
    return (1 + longueur_rec(l -> suivant));
}


int longueur_iter (Liste l) {
    Liste P = l;
    int cpt = 0;
    while (P != NULL) {
        P = P -> suivant;
        cpt++;
    } return cpt;
}

////////////////////// VireDernier, SANS LES BRIQUES DE BASE, NI LE "->" //////////////////////

void VD (Liste *L) {    // *L non NULL ie liste non vide
    if (((**L).suivant) == NULL) depile(L);    // moralement : depile(& (*L));
    else VD (&((**L).suivant));
}

void VireDernier_rec (Liste *L) {
    if ((*L) != NULL) VD(L);   // moralement : VD(& (*L));
}

void VireDernier_iter (Liste *L) {
    if ((*L) == NULL) return;
    while (((**L).suivant) != NULL) L = &((**L).suivant);
    free(*L);
    *L = NULL;
}

////////////////////////////////////// LIBERE LA MEMOIRE //////////////////////////////////////

void VideListe(Liste *L) {
    if(estVide(*L)) return;
    depile(L);
    VideListe(L);
}
//////////////////////////////////////////// PRINT ////////////////////////////////////////////

void poup (Liste l) {
    printf("Double Affichage \n");
    affiche_rec(l);
    affiche_iter(l);

    printf("Longueur en double %d %d \n\n",
        longueur_rec(l), 
        longueur_iter(l)
    );
}

////////////////////////////////////////// UTILITIES //////////////////////////////////////////

void BulkEmpile(Liste *l, int n[], int length) {
    for (int i = 0; i < length; i++) empile(n[i], l);
}

Liste nth(Liste l, int n) {
    if (n == 0) return l;
    return nth(l -> suivant, n-1);
}

bool isEqual(Liste l1, Liste l2) {
    if (estVide(l1) && estVide(l2)) return TRUE;
    else if (estVide(l1) || estVide(l2) || (premier(l1) != premier(l2))) return FALSE;
    return isEqual(l1 -> suivant, l2 -> suivant);
}

//////////////////////////////// ZeroEnPositionUnOuDeuxOuTrois ////////////////////////////////

bool ZeroEnPositionUnOuDeuxOuTrois(Liste l) {
    for (int i = 0; i < 3; i++) {
        if (estVide(l)) return 0;
        else if ((l -> nombre) == 0) return 1;
        l = l -> suivant;
    } return 0;
}

///////////////////////////////////////// Pluscourte //////////////////////////////////////////

bool Pluscourte(Liste l1, Liste l2) {
    if (estVide(l2)) return 0;
    else if (estVide(l1)) return 1;
    return Pluscourte(l1 -> suivant, l2 -> suivant);
}

/////////////////////////////////// NombreDe0AvantPositionK ///////////////////////////////////

//////////// RECURSIF ///////////

int rec_NombreDe0AvantPositionK(Liste l, int k) {
    if (estVide(l) || (k == 0)) return 0;
    return ((l -> nombre) == 0) + rec_NombreDe0AvantPositionK(l -> suivant, k-1);
}

//////////// ITERATIF ///////////

int iter_NombreDe0AvantPositionK(Liste l, int k) {
    int acc = 0;
    for (int i = 0; i < k; i++) {
        if (estVide(l)) break;
        if ((l -> nombre) == 0) acc++;
        l = l -> suivant;
    } return acc;
}

/////////// RECURSIF SF /////////

int rec_sf_NombreDe0AvantPositionK(Liste l, int k, int acc) {
    if (estVide(l) || (k == 0)) return acc;
    return rec_sf_NombreDe0AvantPositionK(l -> suivant, k-1, acc + ((l -> nombre) == 0));
}

int sf_NombreDe0AvantPositionK(Liste l, int k) {
    return rec_sf_NombreDe0AvantPositionK(l, k, 0);
}

/////////// RECURSIF SP /////////

void rec_sp_NombreDe0AvantPositionK(Liste l, int k, int* acc) {
    if (estVide(l) || (k == 0)) return;
    *acc += ((l -> nombre) == 0);
    rec_sp_NombreDe0AvantPositionK(l -> suivant, k-1, acc);
}

int sp_NombreDe0AvantPositionK(Liste l, int k) {
    int acc = 0;
    rec_sp_NombreDe0AvantPositionK(l, k, &acc);
    return acc;
}

//////////////////////////////// NombreDe0ApresRetroPositionK /////////////////////////////////

void rec_NombreDe0ApresRetroPositionK(Liste l, int* k, int* acc) {
    if (!estVide(l -> suivant)) rec_NombreDe0ApresRetroPositionK(l -> suivant, k, acc);
    if (*k != 0) {
        *acc += ((l -> nombre) == 0);
        *k -= 1;
    }
}

int NombreDe0ApresRetroPositionK(Liste l, int k) {
    int acc = 0;
    rec_NombreDe0ApresRetroPositionK(l, &k, &acc);
    return acc;
}

////////////////////////////////////////// FctBegaye //////////////////////////////////////////

/////////// RECURSIF ////////////

Liste rec_FctBegaye(Liste l) {
    if (estVide(l)) return NULL;
    return (((l -> nombre) > 0) ? ajoute(l -> nombre, ajoute(l -> nombre, rec_FctBegaye(l -> suivant)))
                                : rec_FctBegaye(l -> suivant));
}

/////////// RECURSIF SF /////////

Liste rec_sf_FctBegaye(Liste l) {
    if (estVide(l)) return NULL;
    return (((l -> nombre) > 0) ? ajoute(l -> nombre, ajoute(l -> nombre, rec_sf_FctBegaye(l -> suivant)))
                                : rec_sf_FctBegaye(l -> suivant));
}

Liste sf_FctBegaye(Liste l) {
    return rec_sf_FctBegaye(l);
}

/////////// ITERATIF ////////////

// Liste iter_FctBegaye(Liste l) {
//     Liste p;
//     Liste temp;
//     int value = 0;
//     initVide(&p);
//     initVide(&temp);
//     while (!estVide(l)) {
//         value = l -> nombre;
//         if (value > 0) temp = ajoute(value, ajoute(value, temp));
//         l = l -> suivant;
//     } while (!estVide(temp)) {
//         p = ajoute(premier(temp), p);
//         temp = temp -> suivant;
//     } return p;
// }

Liste iter_FctBegaye(Liste l) {
    Liste p = NULL;
    Liste temp = NULL;
    int i = 1;
    while (!estVide(l)) {
        if (l -> nombre > 0) {
            if (p != NULL) temp = nth(temp, i);
            Liste l1 = (Liste)malloc(sizeof(Bloc));
            Liste l2 = (Liste)malloc(sizeof(Bloc));
            l1 -> nombre = l2 -> nombre = l -> nombre;
            l1 -> suivant = l2;
            l2 -> suivant = NULL;
            if (p == NULL) temp = p = l1;
            else {
                temp -> suivant = l1;
                i = 2;
            }
        } l = l -> suivant;
    } return p;
}

///////////////////////////////////////// ProcBegaye //////////////////////////////////////////

void rec_ProcBegaye(Liste* l) {
    if (estVide(*l)) return;
    int value = (*l) -> nombre;
    depile(l);
    rec_ProcBegaye(l);
    if (value > 0) { empile(value, l); empile(value, l); }
}

void ProcBegaye(Liste *l) {
    rec_ProcBegaye(l);
}

//////////////////////////////////////// Permutations /////////////////////////////////////////

typedef struct BL {
    Liste list;
    struct BL *suivant;
} BL;

typedef BL *Listes;

int cptListes = 0;

Listes ajoutes(Liste x, Listes l) {
    Listes tmp = (Listes)malloc(sizeof(BL));
    tmp -> list = x;
    tmp -> suivant = l;
    cptListes++;
    return tmp;
}

void affiche_iter_2(Listes l) {
    Listes L2 = l;
    while(L2 != NULL) {
        affiche_iter(L2 -> list);
        L2 = L2 -> suivant;
    } printf("\n");
}

/////////// NON-OPTI ////////////

Listes Concat (Listes l1, Listes l2) {
    if (l1 == NULL) return l2;
    return ajoutes(l1 -> list, Concat(l1 -> suivant, l2));
}

Listes AETTL (int x, Listes l) {
    if (l == NULL) return NULL;
    return ajoutes(ajoute(x, l -> list), AETTL(x, l -> suivant));
}

Listes ATP (int x, Liste l) {
    if (l == NULL) return ajoutes(ajoute(x, NULL), NULL);
    return ajoutes(ajoute(x, l), AETTL(l -> nombre, ATP(x, l -> suivant)));
}

Listes ATLTP (int x, Listes l) {
    if (l == NULL) return NULL;
    return Concat(ATP(x, l -> list), ATLTP(x, l -> suivant));
}

Listes Permutations (int n) {
    if (n == 0) return NULL;
    return ATLTP(n, Permutations(n-1));
}

///////////// OPTI //////////////

int fcpt = 0;

void freel(Listes L) {
    Listes temp;
    while ((L) != NULL) {
        temp = L;
        L = L -> suivant;
        free(temp);
    } fcpt++;
}

Listes Concat_v2 (Listes l1, Listes l2) {
    if (l1 == NULL) return l2;
    Listes l = ajoutes(l1 -> list, Concat_v2(l1 -> suivant, l2));
    return l;
}

Listes AETTL_v2 (int x, Listes l) {
    if (l == NULL) return NULL;
    Listes temp1 = AETTL_v2(x, l -> suivant);
    Listes final = ajoutes(ajoute(x, l -> list), temp1);
    return final;
}

Listes ATP_v2 (int x, Liste l) {
    if (l == NULL) return ajoutes(ajoute(x, NULL), NULL);
    Listes temp1 = ATP_v2(x, l -> suivant);
    Listes temp2 = AETTL_v2(l -> nombre, temp1);
    Listes templ = ajoutes(ajoute(x, l), temp2);
    freel(temp1);           // <------- WORKING
    // freel(temp2);           // <------- SEGFAULT
    return templ;
}

Listes ATLTP_v2 (int x, Listes l) {
    if (l == NULL) return NULL;
    Listes temp1 = ATP_v2(x, l -> list);
    Listes temp2 = ATLTP_v2(x, l -> suivant);
    Listes final = Concat_v2(temp1, temp2);
    freel(temp1);           // <------- WORKING
    // freel(temp2);           // <------- SEGFAULT
    return final;
}

Listes Permutations_v2 (int n) {
    if (n == 0) return ajoutes(NULL, NULL);
    Listes temp1 = Permutations_v2(n-1);
    Listes final = ATLTP_v2(n, temp1);
    freel(temp1);           // <------- WORKING
    return final;
}

//////// TOTALEMENT OPTI ////////

void swap(Liste x, Liste y) { 
    int temp = x -> nombre;
    x -> nombre = y -> nombre;
    y -> nombre = temp;
}

Liste copyList(Liste head) {
    if (head == NULL) return NULL;
    Liste newList = (Liste)malloc(sizeof(BL));
    cptListe++;
    newList -> nombre = head -> nombre;
    newList -> suivant = copyList(head -> suivant);
    return newList;
}

void rec_permute(Listes* l, Liste a, int start, int end)  { 
    if (start == end) *l = ajoutes(copyList(a), *l);
    else { 
        for (int i = start; i <= end; i++) { 
            swap(nth(a, start), nth(a, i));
            rec_permute(l, a, start+1, end);
            swap(nth(a, start), nth(a, i)); //backtrack 
        }
    }
}

Listes permute(int n) {
    Listes L = NULL;
    Liste a = NULL;
    for (int i = 1; i <= n; i++) empile (i, &a);
    rec_permute(&L, a, 0, n-1);
    return L;
}

////////////////////////////////////////////////////////////////////

typedef struct BLP {
    int nombre;
    struct BLP *suivant;
    struct BLP **pred;
} BLP;

typedef BLP *BisListe;

BisListe empile_bis(int x, BisListe *l) {
    BisListe tmp = (BisListe)malloc(sizeof(BLP));
    tmp -> nombre = x;
    tmp -> suivant = *l;
    tmp -> pred = NULL;
    if ((*l) != NULL) (*l) -> pred = &(tmp -> suivant);
    (*l) = tmp;
}

void BulkEmpile_bis(BisListe *l, int n[], int length) {
    for (int i = 0; i < length; i++) empile_bis(n[i], l);
}

void affiche_bis(BisListe l) {
    BisListe L2 = l;
    while(L2 != NULL) {
        printf("%d ", L2 -> nombre);
        L2 = L2 -> suivant;
    } printf("\n");
}

BisListe nth_bis(BisListe l, int n) {
    if (n == 0) return l;
    return nth_bis(l -> suivant, n-1);
}

void retire(BisListe *l, int n) {
    BisListe toFree;
    if (n == 0) {
        toFree = (*l);
        (*l) = (*l) -> suivant;
        (*l) -> pred = NULL;
    } else {
        BisListe target = nth_bis((*l), n-1);
        toFree = target -> suivant;
        if ((target -> suivant) -> suivant != NULL) (((target -> suivant) -> suivant) -> pred) = ((target -> suivant) -> pred);
        (target -> suivant) = ((target -> suivant) -> suivant);
    } free(toFree);
}

int main() {

    printf("\n------------------------------------------------------------------");
    printf("\n|                             Liste 1                            |");
    printf("\n------------------------------------------------------------------\n\n");
    
    Liste l = NULL;
    BulkEmpile(&l, (int[]){0, 1, 0, 0, 0, 9, 6, 5, 0, 2}, 10);

    poup(l);

    printf("%s parmis les 3 premiers elements\n\n", (ZeroEnPositionUnOuDeuxOuTrois(l) ? "Presence de 0" : "Pas de 0"));

    assert((rec_NombreDe0AvantPositionK(l, 6) == iter_NombreDe0AvantPositionK(l, 6))
        ==  (sf_NombreDe0AvantPositionK(l, 6) == sp_NombreDe0AvantPositionK(l, 6)));

    printf("%d 0 parmis les %d premiers elements\n", sf_NombreDe0AvantPositionK(l, 6), 6);

    printf("\n------------------------------------------------------------------");
    printf("\n|                             Liste 2                            |");
    printf("\n------------------------------------------------------------------\n\n");

    Liste l2 = NULL;
    BulkEmpile(&l2, (int[]){2, 0, 5, 6, 9, 0, 0, 0, 1, 0}, 10);

    poup(l2);

    printf("%d 0 parmis les %d derniers elements\n", NombreDe0ApresRetroPositionK(l2, 6), 6);

    printf("\n------------------------------------------------------------------");
    printf("\n|                             Liste 3                            |");
    printf("\n------------------------------------------------------------------\n\n");

    Liste l3 = NULL;
    BulkEmpile(&l3, (int[]){8, 8, -2, 6, 0, 1, 2}, 7);

    Liste l3_begaye = copyList(l3);
    ProcBegaye(&l3_begaye);

    Liste begaye = NULL;
    BulkEmpile(&begaye, (int[]){8, 8, 8, 8, 6, 6, 1, 1, 2, 2}, 10);

    assert((isEqual(rec_FctBegaye(l3), begaye))
        == (isEqual(iter_FctBegaye(l3), begaye))
        == (isEqual(sf_FctBegaye(l3), begaye))
        == (isEqual(l3_begaye, begaye)));

    poup(l3);

    printf("------------------------------------------------------------------");
    printf("\n|                          Begaye de L3                          |");
    printf("\n------------------------------------------------------------------\n\n");

    poup(sf_FctBegaye(l3));

    // free des Liste car elles ne seront plus utilisees

    VideListe(&l);
    VideListe(&l2);
    VideListe(&l3);
    VideListe(&l3_begaye);

    printf("------------------------------------------------------------------");
    printf("\n|                          Permutations                          |");
    printf("\n------------------------------------------------------------------\n\n");
    
    int test_rep = 1;

    clock_t begin1 = clock();
    for (int i = 0; i < test_rep; i++) Permutations_v2(9);
    double cpt_1 = cptListes;
    double cpt_3 = cptListe;
    clock_t end1 = clock();
    double elapsed1 = (double)(end1 - begin1);
    printf("Permutation basique     : %.2f s | %.f malloc de Liste | %.f malloc de Listes | %d free.\n", (elapsed1 / (CLOCKS_PER_SEC * test_rep)), cpt_3, cpt_1, fcpt);

    cptListes = 0;
    cptListe = 0;

    clock_t begin2 = clock();
    for (int i = 0; i < test_rep; i++) permute(9);
    double cpt_2 = cptListes;
    double cpt_4 = cptListe;
    clock_t end2 = clock();
    double elapsed2 = (double)(end2 - begin2);
    printf("Permutation optimisee   : %.2f s | %.f malloc de Liste | %.f malloc de Listes.\n", (elapsed2 / (CLOCKS_PER_SEC * test_rep)), cpt_4, cpt_2);

    printf("\nVersion optimisee %.2f%% plus rapide et %.2f%% moins de memoire utilisee.\n\n", (((elapsed1 - elapsed2) / elapsed1) * 100), (((cpt_1 - cpt_2) / cpt_1) * 100));

    printf("------------------------------------------------------------------");
    printf("\n|                           Listes Bis                           |");
    printf("\n------------------------------------------------------------------\n\n");

    BisListe bl = NULL; 
    BulkEmpile_bis(&bl, (int[]){5, 4, 3, 2, 1}, 5);

    affiche_bis(bl);
    retire(&bl, 3); // retire l'élément à l'index 3
    affiche_bis(bl);

    return 0;
}