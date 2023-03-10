#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef struct bloc_image {
    int toutnoir ;
    struct bloc_image * fils[4] ;
} bloc_image ;
typedef bloc_image *image ;

image Construit_Blanc() { return NULL; }

void Free_Construit_Blanc(image* img) {
    image tmp = (*img);
    (*img) = Construit_Blanc();
    free(tmp);
}

image Construit_Noir() {
    image img = (image)malloc(sizeof(bloc_image));
    img -> toutnoir = 1;
    for (int i = 0; i < 4; i++) img -> fils[i] = NULL;
    return img;
}

image Construit_Composee(image img1, image img2, image img3, image img4) {
    image img = (image)malloc(sizeof(bloc_image));
    img -> toutnoir = 0;
    img -> fils[0] = img1; img -> fils[1] = img2;
    img -> fils[2] = img3; img -> fils[3] = img4;
    return img;
}

// =================================== Utilities ===================================

int aux_isEqual(char* input, int *shift, image img) {
    (*shift) += 1; char c = *(input + (*shift));
    if (c == 'B') { return (img == NULL); }
    else if (c == 'N') { return !(img == NULL) && (img -> toutnoir); }
    else if (c == '(') {
        return aux_isEqual(input, shift, img -> fils[0]) && aux_isEqual(input, shift, img -> fils[1]) &&
               aux_isEqual(input, shift, img -> fils[2]) && aux_isEqual(input, shift, img -> fils[3]);
    } else return aux_isEqual(input, shift, img);
}

int isEqual(char* input, image img) {
    int shift = -1;
    return aux_isEqual(input, &shift, img);
}

image CopyImg(image img) {
    if (img == NULL) return Construit_Blanc();
    else if (img -> toutnoir) return Construit_Noir();
    return Construit_Composee(CopyImg(img -> fils[0]), CopyImg(img -> fils[1]),
                              CopyImg(img -> fils[2]), CopyImg(img -> fils[3]));
}

// =================================================================================

void aux_affiche_normal(image img) {
    if (img == NULL) printf("B");
    else if (img -> toutnoir) printf("N");
    else {
        printf("(");
        for (int i = 0; i < 4; i++) aux_affiche_normal(img -> fils[i]);
        printf(")");
    }
}

void affiche_normal(image img) {
    aux_affiche_normal(img);
    printf("\n");
}

void aux_affiche_profondeur(image img, int depth) {
    if (img == NULL) printf("B%d", depth);
    else if (img -> toutnoir) printf("N%d", depth);
    else {
        printf("(");
        for (int i = 0; i < 4; i++) aux_affiche_profondeur(img -> fils[i], depth + 1);
        printf(")");
    }
}

void affiche_profondeur(image img) {
    aux_affiche_profondeur(img, 1);
    printf("\n");
}

image aux_Lecture(char* input, int *shift) {
    (*shift) += 1; char c = *(input + (*shift));
    if (c == 'B') { return Construit_Blanc(); }
    else if (c == 'N') { return Construit_Noir(); }
    else if (c == '(') {
        image img1 = aux_Lecture(input, shift); image img2 = aux_Lecture(input, shift);
        image img3 = aux_Lecture(input, shift); image img4 = aux_Lecture(input, shift);
        (*shift) += 1;
        if (*(input + (*shift)) != ')') { printf("Invalid Input"); exit(-1); }
        return Construit_Composee(img1, img2, img3, img4);
    } else { printf("Invalid Input"); exit(-1); }
}

image Lecture_String(char* input) {
    int shift = -1;
    image img = aux_Lecture(input, &shift);
    if (*(input + (shift + 1)) != '\000') { printf("Invalid Input"); exit(-1); }
    return img;
}

image Lecture() {
    char input[100];
    printf("Input : \n");
    scanf("%s", &input);
    return Lecture_String(input);
}

int EstNoire(image img) {
    if (img == NULL) return 0;
    else if (img -> toutnoir) return 1;
    else return EstNoire(img -> fils[0]) && EstNoire(img -> fils[1]) &&
                EstNoire(img -> fils[2]) && EstNoire(img -> fils[3]);
}

int EstBlanche(image img) {
    if (img == NULL) return 1;
    else if (img -> toutnoir) return 0;
    else return EstBlanche(img -> fils[0]) && EstBlanche(img -> fils[1]) &&
                EstBlanche(img -> fils[2]) && EstBlanche(img -> fils[3]);
}

image Diagonale(int p) {
    if (p <= 0) return Construit_Noir();
    return (p == 1) ? Construit_Composee(Construit_Noir(), Construit_Blanc(), Construit_Blanc(), Construit_Noir())
                    : Construit_Composee(Diagonale(p - 1), Construit_Blanc(), Construit_Blanc(), Diagonale(p - 1));
}

image QuartDeTour(image img) {
    if (img == NULL) return Construit_Blanc();
    else if (img -> toutnoir) return Construit_Noir();
    return Construit_Composee(QuartDeTour(img -> fils[2]), QuartDeTour(img -> fils[0]),
                              QuartDeTour(img -> fils[3]), QuartDeTour(img -> fils[1]));
}

void Negatif(image* img) {
    if ((*img) == NULL) (*img) = Construit_Noir();
    else if ((*img) -> toutnoir) Free_Construit_Blanc(img);
    else for (int i = 0; i < 4; i++) Negatif(&((*img) -> fils[i]));
}

void SimplifieProfP(image* img, int p) {
    if (p == 0) {
        if (EstBlanche(*img)) Free_Construit_Blanc(img);
        else if (EstNoire(*img)) (*img) = Construit_Noir();
    } else if (!((*img) == NULL) && !((*img) -> toutnoir)) {
        for (int i = 0; i < 4; i++) SimplifieProfP(&((*img) -> fils[i]), p - 1);
    }
}

int Incluse(image img1, image img2) {
    int EN1 = EstNoire(img1); int EB1 = (EN1) ? 0 : EstBlanche(img1);
    int EN2 = EstNoire(img2); int EB2 = (EN2) ? 0 : EstBlanche(img2);
    if ((EN1 && !EN2) || (!EB1 && EB2)) return 0;
    else if ((EN1 && EN2) || (!EN1 && EN2) || (EB1 && EB2) || (EB1 && !EB2)) return 1;
    else return Incluse(img1 -> fils[0], img2 -> fils[0]) && Incluse(img1 -> fils[1], img2 -> fils[1]) &&
                Incluse(img1 -> fils[2], img2 -> fils[2]) && Incluse(img1 -> fils[3], img2 -> fils[3]);
}

int max (int a, int b) { return (b > a) ? b : a; }

int depth(image img) {
    if ((img == NULL) || (img -> toutnoir)) return 0;
    return 1 + max(max(depth(img -> fils[0]), depth(img -> fils[1])),
                   max(depth(img -> fils[2]), depth(img -> fils[3])));
}

int HautMaxBlanc(image img) {
    if ((img == NULL) || (img -> toutnoir)) return 0;
    else if (EstNoire(img)) return -1;
    else if (EstBlanche(img)) return depth(img);
    return max(max(HautMaxBlanc(img -> fils[0]), HautMaxBlanc(img -> fils[1])),
               max(HautMaxBlanc(img -> fils[2]), HautMaxBlanc(img -> fils[3])));
}

void BlanchitProfP(image* img, int x, int y, int p) {
    if ((*img) == NULL) return;
    else if ((*img) -> toutnoir) {
        image tmp = (*img);
        (*img) = Lecture_String("(NNNN)");
        free(tmp);
    } if (p == 0) { Free_Construit_Blanc(img); return; }

    int middle = (int)(pow(2, p) + 1e-9) / 2;
    int index = ((x < middle) ? (y < middle ? 2 : 0) : (y < middle ? 3 : 1));
    BlanchitProfP(&((*img) -> fils[index]), x % middle, y % middle, p - 1);
}

void aux_Chute(image* img1, image* img2) {

    if (!((*img1) == NULL) && ((*img2) == NULL)) {
        (*img2) = (image)malloc(sizeof(bloc_image));
        (*img2) = (*img1);
        (*img1) = Construit_Blanc();
    } else if (!((*img1) == NULL) && ((*img1) -> toutnoir) && (!((*img2) == NULL))) {
        image temp = *img1;
        (*img1) = (*img2);
        (*img2) = temp;
    }

    if (!((*img1) == NULL) && !((*img1) -> toutnoir)) {
        aux_Chute(&((*img1) -> fils[0]), &((*img1) -> fils[2]));
        aux_Chute(&((*img1) -> fils[1]), &((*img1) -> fils[3]));
    }

    if (!((*img2) == NULL) && !((*img2) -> toutnoir)) {
        aux_Chute(&((*img2) -> fils[0]), &((*img2) -> fils[2]));
        aux_Chute(&((*img2) -> fils[1]), &((*img2) -> fils[3]));
    }
    
    if (!((*img1) == NULL) && !((*img1) -> toutnoir) &&
        !((*img2) == NULL) && !((*img2) -> toutnoir)) {
        aux_Chute(&((*img1) -> fils[2]), &((*img2) -> fils[0]));
        aux_Chute(&((*img1) -> fils[3]), &((*img2) -> fils[1]));
        aux_Chute(&((*img1) -> fils[0]), &((*img1) -> fils[2]));
        aux_Chute(&((*img1) -> fils[1]), &((*img1) -> fils[3]));
        aux_Chute(&((*img2) -> fils[0]), &((*img2) -> fils[2]));
        aux_Chute(&((*img2) -> fils[1]), &((*img2) -> fils[3]));
        aux_Chute(&((*img1) -> fils[2]), &((*img2) -> fils[0]));
        aux_Chute(&((*img1) -> fils[3]), &((*img2) -> fils[1]));
    }
}

void Chute (image* img) {
    aux_Chute(&((*img) -> fils[0]), &((*img) -> fils[2]));
    aux_Chute(&((*img) -> fils[1]), &((*img) -> fils[3]));
}

int main() {
    image i1 = Lecture_String("(BBBB)"); image i2 = Lecture_String("(NNNN)"); image i3 = Lecture_String("(NBNB)");
    assert((EstBlanche(i1) == EstNoire(i2)) == (!EstBlanche(i3) == !EstNoire(i3)));

    assert(isEqual("((BNBN)(NBNN)NB))", QuartDeTour(Lecture_String("((BNNN)B(NNBB)N)"))));

    image i4 = Lecture_String("((BNNN)B(NNBB)N)"); Negatif(&i4);
    assert(isEqual("((NBBB)N(BBNN)B)", i4));

    image i5 = Lecture_String("(N(NB(NN(NNNN)N)B)(NBN(NBN(BBBB)))(BB(BBBB)B))"); SimplifieProfP(&i5, 2);
    assert(isEqual("(N(NBNB)(NBN(NBN(BBBB)))(BBBB))", i5));

    assert(!Incluse(Lecture_String("(((BBBB)NBN)BN((BBNN)BB(NBBN)))"), Lecture_String("((BNNN)(BBNB)(NNNN)(NBN(NNNB)))"))
        ==  Incluse(Lecture_String("(((BBBB)NBN)BN((BBNN)BB(NBBN)))"), Lecture_String("((BNNN)(BBNB)(NNNN)(NBN(NNNN)))")));

    assert(HautMaxBlanc(Lecture_String("((BBBB)N(BNBN)(NBN(NB(BB(BBBB)(BBB(BBBB)))N)))")) == 3);

    image i6 = Lecture_String("(NBNB)"); BlanchitProfP(&i6, 1, 3, 2);
    image i7 = Lecture_String("((N(NBNN)NN)BNB)"); BlanchitProfP(&i7, 1, 3, 2);
    assert(isEqual("((NBNN)BNB)", i6) == isEqual("((NBNN)BNB)", i7));

    image i8 = Lecture_String("(N((NBNN)NB(BNBN))(N(NBBN)(BNNN)(NBNB))(NN(NBBB)(BNBN)))"); Chute(&i8);
    assert(isEqual("(((BNNN)(BBNB)N(NBNN))(B(BNBN)(BBNB)(BNBN))N((NBNN)NNN))", i8));

    image i9 = Lecture();
    printf("QuartDeTour : "); affiche_normal(QuartDeTour(i9));
    image i10 = CopyImg(i9); Negatif(&i10);
    printf("Negatif : "); affiche_normal(i10);
    image i11 = CopyImg(i9); Chute(&i11);
    printf("Chute : "); affiche_normal(i11);
}