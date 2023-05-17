#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[col + row*cols];
}

void set(int cols, int row, int col, int *A, int value) {
    A[col +row*cols] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int valA = 0;
    int valB = 0;
    int valAB = 0;

    for( int y = 0; y < rowsA; y++ ){
        for( int x = 0; x < colsB; x++ ){
            set(colsB, y, x, AB, 0);
            for( int z = 0; z < colsA; z++ ){
                valA = get(colsA, y, z, A);
                valB = get(colsB, z, x, B);
                valAB = get(colsB, y, x, AB);
                set(colsB, y, x, AB, valA*valB + valAB );
            }
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    int val = 0;

    for( int y = 0; y < rows; y++){
        for( int x = 0; x < cols; x++){
            scanf("%d", &val);
            set(cols, y, x, t, val);
        }

    }
}

void print_mat(int rows, int cols, int *t) {
    for( int y = 0; y < rows; y++ ){
        for( int x = 0; x < cols; x++ ){
            printf("%d ", get(cols, y, x, t));
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    char znak;
    int i = 0;
    int j = 0;

    array[j] = malloc(BUF_SIZE);

    while( scanf("%c", &znak) != EOF ){
        if( znak != '\n' ){
            array[j][i] = znak;
            i++;
        }
        else{
            array[j][i] = '\0';
            i = 0;
            j++;
            array[j] = malloc(BUF_SIZE);
        }
    }

    return j;
}

void write_char_line(char *array[], int n) {
    int j = 0;
    for(int i = 0; i < BUF_SIZE; i++ ){
        if( *array[n] == '\0' )
            break;
        printf("%c", *array[n] );
        array[n]++;
        j++;
    }
    for(int i = 0; i < j; i++ ){
        array[n]--;
    }
}

void delete_lines(char *array[], int lines) {
    for( int i = 0; i < lines; i++ ){
        free(array[i]);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int liczba = 0;
    int i = 0;
    int i_2 = 0;
    int i_3 = 0;
    int c = 0;
    int j = 0;
    char buffer[BUF_SIZE];
    char string[BUF_SIZE];
    char *eptr;
    char *b = buffer;
    size_t bufsize = BUF_SIZE;
    ssize_t znaki;

    znaki = getline(&b, &bufsize, stdin);

    while((znaki = getline(&b, &bufsize, stdin)) != -1){
        i_2 = 0;
        i_3 = 0;

        while( i_2 < znaki ){
            if( buffer[i_2] != ' ' && buffer[i_2] != '\n'){
                string[i_3] = buffer[i_2];
                i_3++;
            }
            else{
                string[i_3] = '\0';
                liczba = strtod(string, &eptr);
                ptr_array[0][i] = liczba;
                i++;
                i_3 = 0;
            }
            i_2++;
        }

        c++;
        ptr_array[c] = &ptr_array[0][j];
        j = i;
    }

    ptr_array[0][i] = 0;
    c++;
    ptr_array[c] = &ptr_array[0][j];

    return c--;
}

void write_int_line_cont(int *ptr_array[], int n) {
    while( ptr_array[n] < ptr_array[n+1]){
        printf("%d ", *ptr_array[n]);
        ptr_array[n]++;
    }
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int line_count = 0;
    char buffer[BUF_SIZE];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        char *token = strtok(buffer, " ");
        int *values = malloc(sizeof(int));
        int value_count = 0;

        while (token != NULL) {
            int value = atoi(token);
            values = realloc(values, (value_count + 1) * sizeof(int));
            values[value_count] = value;
            value_count++;

            token = strtok(NULL, " ");
        }

        double sum = 0;
        for (int i = 0; i < value_count; i++) {
            sum += values[i];
        }

        double average = sum / value_count;

        lines_array[line_count].values = values;
        lines_array[line_count].len = value_count;
        lines_array[line_count].average = average;

        line_count++;
    }

    return line_count;
}

void write_int_line(line_type lines_array[], int n) {
    line_type line = lines_array[n];

    for (int i = 0; i < line.len; i++) {
        printf("%d ", line.values[i]);
    }

    printf("\n");

    printf("%.2f", line.average);
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type *line_a = (line_type *)a;
    line_type *line_b = (line_type *)b;

    if (line_a->average < line_b->average) {
        return -1;
    } else if (line_a->average > line_b->average) {
        return 1;
    } else {
        return 0;
    }
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; i++) {
        scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }

    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *triplet1 = (triplet *)t1;
    triplet *triplet2 = (triplet *)t2;

    if (triplet1->r < triplet2->r) {
        return -1;
    } else if (triplet1->r > triplet2->r) {
        return 1;
    } else {
        if (triplet1->c < triplet2->c) {
            return -1;
        } else if (triplet1->c > triplet2->c) {
            return 1;
        } else {
            return 0;
        }
    }
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);

    for (int i = 0; i < n_triplets; i++) {
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
    }

    int current_row = 0;
    R[0] = 0;

    for (int i = 1; i <= rows; i++) {
        int count = 0;

        while (current_row < n_triplets && triplet_array[current_row].r == i - 1) {
            current_row++;
            count++;
        }

        R[i] = R[i - 1] + count;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; i++) {
        y[i] = 0;

        for (int j = R[i]; j < R[i + 1]; j++) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n){
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            lines_counter = read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array, lines_counter);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}