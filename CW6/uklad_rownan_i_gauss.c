#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
    for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
    for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
    printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
    for(size_t i=0; i<m; ++i) {
        for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
    }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
    for(size_t i=0; i<m; ++i) {
        for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
        printf("\n");
    }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    double mnoznik = 0;
    double wyznacznik = 1;
    double pom = 0;
    double swap1 = 0;
    int swap2 = 0;
    int max_ind = 0;
    double max_wart = 0;
    int flag = 1;
    int indeksy[n];
    int znak = 1;

    /*
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    */

    // robimy tablice indeksow do przepisywania w triangulacji
    for(int i=0; i<n; i++){
        x[i] = b[i];
        if(b[i] != 0){
            flag = 0;
        }
        indeksy[i] = i;
    }

    if(flag == 1){
        return 0;
    }

    // robimy triangulacje
    for(int i=0; i<n-1; i++){
        max_ind = indeksy[i];
        max_wart = A[indeksy[i]][i];

        for(int j=i; j<n; j++){
            if(fabs( A[ indeksy[j] ][ i ] ) > fabs(max_wart)){
                max_ind=j;
                max_wart=A[ indeksy[j] ][i];
            }
        }

        // zamieniamy wiersze w tablicach
        if(max_ind != indeksy[i]){
            swap2=indeksy[i];
            indeksy[i]=indeksy[max_ind];
            indeksy[max_ind]=swap2;
            znak *= (-1);

            swap1 = x[i];
            x[i] = x[max_ind];
            x[max_ind] = swap1;
        }

        // no i gauss
        for(int j=i+1; j<n; j++){
            if( fabs( A[ indeksy[i] ][ i ] ) < eps ){
                return 0;
            }
            mnoznik = -A[ indeksy[j] ][i]/A[ indeksy[i] ][i];
            for(int k=i; k<=n; k++){
                A[ indeksy[j] ][k] += mnoznik*A[ indeksy[i] ][k];
            }
            x[j] += mnoznik*x[i];
        }

        /*
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                printf("%f ", A[indeksy[i]][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");
        */
    }

    /*
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%f ", A[indeksy[i]][j]);
        }
        printf("\n");
    }

    for(int i=0; i<n; i++){
        printf("%f ", x[indeksy[i]]);
    }
    */

    // obliczamy wyznacznik
    for(int i=0; i<n; i++){
        wyznacznik *= A[ indeksy[i] ][i];
    }
    wyznacznik *= znak;

    for( int i=n-1; i>=0; i--){
        pom = A[ indeksy[i] ][i];
        for(int j=n-1; j>=i+1; j--){
            x[i] -= A[ indeksy[i] ][j]*x[j];
        }
        if( fabs( A[ indeksy[i] ][ i ] ) < eps ){
            return 0;
        }
        x[i] = x[i]/pom;
    }

    return wyznacznik;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    int indeksy[n];
    double mnoznik = 0;
    double wynik = 1;
    int znak = 1;
    int swap1 = 0;
    double swap2 = 0;
    int max_ind = 0;
    double max_wart = 0;

    // robimy tablice, zeby pamietac indeksy wierszy
    for( int i = 0; i < n; i++ ){
        B[i][i] = 1;
        indeksy[i] = i;
    }

    for( int i = 0; i < n-1; i++ ){
        max_ind = indeksy[i];
        max_wart = A[ indeksy[i] ][i];

        for( int j = i; j < n; j++){
            if(fabs(A[ indeksy[j] ][i]) > fabs(max_wart) ){
                max_ind = j;
                max_wart = A[ indeksy[j] ][i];
            }
        }

        if( max_ind != indeksy[i] ){
            for( int j = 0; j < n; j++ ){
                swap2 = B[i][j];
                B[i][j] = B[max_ind][j];
                B[max_ind][j] = swap2;
            }

            swap1 = indeksy[i];
            indeksy[i] = indeksy[max_ind];
            indeksy[max_ind] = swap1;
            znak *= (-1);
        }


        for( int j = i+1; j < n; j++ ){
            if( fabs(A[ indeksy[i] ][i]) < eps ){
                for( int i = 0; i < n; i++ ){
                    for( int j = 0; j < n; j++) {
                        B[i][j] = 0;
                    }
                }
                return 0;
            }

            mnoznik = -A[ indeksy[j] ][i] / A[ indeksy[i] ][i];

            for( int k = i; k <= n; k++ ){
                A[ indeksy[j] ][k] += mnoznik * A[ indeksy[i] ][k];
            }

            for( int k = 0; k < n; k++ ){
                B[j][k] += mnoznik * B[i][k];
            }

        }

    }

    for( int i = 0; i < n; i++ ){
        wynik *= A[ indeksy[i] ][i];
    }

    wynik *= znak;

    for( int i = n-1; i > 0; i-- ){
        for( int j = i-1; j > -1; j--){
            mnoznik = -A[ indeksy[j] ][i] / A[ indeksy[i] ][i];
            A[ indeksy[j] ][i] += mnoznik * A[ indeksy[i] ][i];
            for( int k = 0; k < n; k++)
                B[j][k] += mnoznik * B[i][k];
        }
    }

    for( int i = 0; i < n; i++ ){
        mnoznik = 1/A[ indeksy[i] ][i];
        A[ indeksy[i] ][i] *= mnoznik;
        for( int j = 0; j < n; j++) {
            B[i][j] *= mnoznik;
        }
    }


    for( int i = 0; i < n; i++ ){
        for( int j = 0; j < n; j++){
            if(fabs(B[i][j]) < eps) {
                B[i][j] = 0;
            }
        }
    }

    return wynik;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m,n,p,q;
    if(TEST) printf("Wpisz nr zadania ");
    scanf ("%d", &to_do);
    switch (to_do) {
        case 1:
            if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
            scanf("%d", &n);
            if(TEST) printf("Wpisz macierz A (wierszami): ");
            read_mat(A,n,n);
            if(TEST) printf("Wpisz wektor b: ");
            read_vec(b,n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n",det);
            if(det) print_vec(x,n);
            break;
        case 2:
            if(TEST) printf("Wpisz rozmiar macierzy n = ");
            scanf("%d",&n);
            if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
            read_mat(A,n,n);
            det = matrix_inv(A,B,n,eps);
            printf("%.4f\n",det);
            if(det) print_mat(B,n,n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
