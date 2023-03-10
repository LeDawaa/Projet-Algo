#include <stdio.h>
#include <assert.h>
#include <math.h>

////////////////////////////////////////// FORMULE E //////////////////////////////////////////

double sum_fact() {
    int fn = 1;
    double sum = 2;
    for (int i = 2; i < 15; i++) {
        fn *= i;
        sum += 1./fn;
    } return sum;
}

//////////////////////////////////////////// SUITE ////////////////////////////////////////////

double suite(int n, int print) {
    if (n == 0) return (sum_fact() - 1.);
    double s = (n * suite(n - 1, print)) - 1.;
    if (print) printf("%d : %f\n", n, s);
    return s;
}

//////////////////////////////////////////// POWER ////////////////////////////////////////////

//////////// POWER 1 ////////////

double pow_1(int x, int n) {
    return ((n == 0) ? 1. : (pow_1(x, n-1) * x));
}

double power_1(int x, int n) {
    return ((n < 0) ? 1./pow_1(x, abs(n)) : pow_1(x, n));
}

//////////// POWER 2 ////////////

double power_2(int x, int n) {
    double r = 1;
    int an = abs(n);
    for (int i = 0; i < an; i++) r *= x;
    return ((n < 0) ? 1./r : r);
}

//////////// POWER 3 ////////////

void pow_3(int x, int n, double* r) {
    if (n == 0) return;
    *r *= x;
    pow_3(x, n-1, r);
}

double power_3(int x, int n) {
    double r = 1;
    pow_3(x, n, &r);
    return r;
}

//////////// POWER 4 ////////////

double pow_4(int x, int n, double r) {
    if (n == 0) return r;
    return pow_4(x, n-1, r * x);
}

double power_4(int x, int n) {
    return pow_4(x, n, 1);
}

//////////// POWER 5 ////////////

double power_5(int x, int n) {
    if (n == 0) return 1;
    return power_5(x, n/2) * power_5(x, n/2) * ((n%2) ? x : 1);
}

//////////// POWER 6 ////////////

double power_6(int x, int n) {
    if (n == 0) return 1;
    double p = power_6(x, n/2);
    return p * p * ((n%2) ? x : 1);
}

//////////// POWER 7 ////////////

double power_7(int x, int n) {
    if (n == 0) return 1;
    return power_7(x*x, n/2) * ((n%2) ? x : 1);
}

//////////// POWER 8 ////////////

double pow_8(int x, int n, double r) {
    if (n == 0) return r;
    return pow_8(x*x, n/2, r * ((n%2) ? x : 1));
}

double power_8(int x, int n) {
    return pow_8(x, n, 1);
}

//////////// POWER 9 ////////////

void pow_9(double x, int n, double* r) {
    if (n == 0) return;
    if (n%2) *r *= x;
    pow_9(x*x, n/2, r);
}

double power_9(double x, int n) {
    double r = 1;
    pow_9(x, n, &r);
    return r;
}

//////////// POWER 10 ///////////

double power_10(double x, long long n) {
    double r = 1;
    while (n) {
        if (n%2) r *= x;
        n *= 0.5;
        x *= x;
    } return r;
}

////////////////////////////////////////// ACKERMANN //////////////////////////////////////////

int rec_A(int m, int n) {
    if (m == 0) return n+1;
    return ((n == 0) ? rec_A(m-1, 1) : rec_A(m-1, rec_A(m, n-1)));
}

int iter_A(int m, int n) {
    if (m == 0) return n+1;
    int r = 1;
    for (int i = 1; i <= n+1; i++) {
        r = iter_A(m-1, r);
    } return r;
}

//////////////////////////////////////// SUITE DE REELS ///////////////////////////////////////


//////////// ITERATIF ///////////

double iter_suite(int x) {
    double sum = 1;
    for (int i = 0; i < x; i++) {
        sum += (2/sum);
    } return sum;
}

//////////// RECURSIF ///////////

double rec_suite(int x) {
    if (x == 0) return 1;
    double sum = rec_suite(x-1);
    return sum + (2/sum);
}

/////////// RECURSIF SF /////////

double rec_sf_suite(int x, double sum) {
    return (x == 0) ? sum : rec_sf_suite(x-1, sum + (2/sum));
}

double sf_suite(int x) {
    return rec_sf_suite(x, 1);
}

/////////// RECURSIF SP /////////

void rec_sp_suite(int x, double* sum) {
    if (x == 0) return;
    *sum += (2/(*sum));
    rec_sp_suite(x-1, sum);
}

double sp_suite(int x) {
    double sum = 1;
    rec_sp_suite(x, &sum);
    return sum;
}

int main() {

    printf("e = %.16f\n\n", sum_fact());

    suite(50, 1);
    printf("\n");

    /*
    * L'utilisation de float réduit la precision et ne donne plus de resultat a partir de la 39e iteration.
    * Le passage au double permet d'obtenir tous les resultats jusqu'a 50, on double la precision (2x plus de nombres apres la virgule)
    * La suite tend vers +inf, une fois la valeur hors de l'intervale [0; 1] on assiste a une croissanse
    * tres rapide, croissance qui sera plus rapide encore en montant dans les index.
    */

    for (int i = 0; i < 12; i++) {
        long long p = power_10(10, i);
        printf("%.16f\n", power_10((1. + 1./p), p));
    } printf("\n");
    /*
    * De facon similaire a la premiere question, cette suite semble tendre vers 2.718282 selon mes test (donc possiblement tendre vers 3).
    */

    assert((round(power_1(10, 15)) == round(power_2(10, 15)))
        == (round(power_3(10, 15)) == round(power_4(10, 15)))
        == (round(power_5(10, 15)) == round(power_6(10, 15)))
        == (round(power_7(10, 15)) == round(power_8(10, 15)))
        == (round(power_9(10, 15)) == round(power_10(10, 15))));
    /*
    * Je ne rencontre pas le moindre probleme en executant ces fonctions, elles se terminent immediatemment.
    */

    for (int i = 0; i < 5; i++) assert((rec_A(i, 0)) == (iter_A(i, 0)));
    /*
    * Le programme tourne sans donner de resultat pour A(6, 0)
    * rec_A atteint la limite de recursion et ne donne pas de resultat pour A(5, 0) alors que iter_A fonctionne
    */

    assert((round(iter_suite(100) * 100000.0) == round(rec_suite(100) * 100000.0))
        == (round(sf_suite(100) * 100000.0) == round(sp_suite(100) * 100000.0)));
    printf("100e terme de la suite Xn = X(n-1) + 2/X(n-1) : %f", sp_suite(100));
    /*
    * On trouve bien le meme resultat avec les quatres fonctions.
    */
    return 0;
}
