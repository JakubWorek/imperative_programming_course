#include <stdio.h>
#include <float.h>
#include <malloc.h>
#include <string.h>

// Zadanie 1

typedef struct _matrix {
    int rows;
    int cols;
    double** data;
} Matrix;

int create_matrix(Matrix* pmatrix, int rows, int cols) {
    if (rows < 1 || cols < 1 || pmatrix == NULL) {
        return 0;
    }

    pmatrix->rows = rows;
    pmatrix->cols = cols;

    pmatrix->data = (double**)malloc(rows * sizeof(double*));
    if (pmatrix->data == NULL) {
        return 0;
    }

    for (int i = 0; i < rows; i++) {
        pmatrix->data[i] = (double*)malloc(cols * sizeof(double));
        if (pmatrix->data[i] == NULL) {
            // W przypadku błędu przy alokacji pamięci, zwalniamy już zaalokowane wiersze
            for (int j = 0; j < i; j++) {
                free(pmatrix->data[j]);
            }
            free(pmatrix->data);
            return 0;
        }

        // Inicjalizacja wartości wiersza macierzy na 0
        for (int j = 0; j < cols; j++) {
            pmatrix->data[i][j] = 0.0;
        }
    }

    return 1;
}

int get(Matrix* pm, int row, int col, double* pvalue) {
    if (pm == NULL || pm->data == NULL || row < 0 || row >= pm->rows || col < 0 || col >= pm->cols) {
        return 0;
    }

    *pvalue = pm->data[row][col];
    return 1;
}

int set(Matrix* pm, int row, int col, double value) {
    if (pm == NULL || pm->data == NULL || row < 0 || row >= pm->rows || col < 0 || col >= pm->cols) {
        return 0;
    }

    pm->data[row][col] = value;
    return 1;
}

void create_identity_matrix(Matrix* pm, int size) {
    if (pm == NULL || size < 1) {
        return;
    }

    pm->rows = size;
    pm->cols = size;

    pm->data = (double**)malloc(size * sizeof(double*));
    if (pm->data == NULL) {
        return;
    }

    for (int i = 0; i < size; i++) {
        pm->data[i] = (double*)malloc(size * sizeof(double));
        if (pm->data[i] == NULL) {
            // W przypadku błędu przy alokacji pamięci, zwalniamy już zaalokowane wiersze
            for (int j = 0; j < i; j++) {
                free(pm->data[j]);
            }
            free(pm->data);
            return;
        }

        // Inicjalizacja wartości wiersza macierzy jednostkowej
        for (int j = 0; j < size; j++) {
            pm->data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

double dot_product_vector(const double* v1, const double* v2, int size) {
    double dotProduct = 0.0;

    for (int i = 0; i < size; i++) {
        dotProduct += v1[i] * v2[i];
    }

    return dotProduct;
}

void mult_vect(double* result, const Matrix* pm, const double* vect) {
    int rows = pm->rows;
    int cols = pm->cols;

    for (int i = 0; i < rows; i++) {
        const double* row = pm->data[i];
        result[i] = dot_product_vector(row, vect, cols);
    }
}

void max_element_in_rows(double* maxdata, const Matrix* pm) {
    int rows = pm->rows;
    int cols = pm->cols;

    for (int i = 0; i < rows; i++) {
        double maxElement = -DBL_MAX;

        for (int j = 0; j < cols; j++) {
            double element = pm->data[i][j];

            if (element > maxElement) {
                maxElement = element;
            }
        }

        maxdata[i] = maxElement;
    }
}

// Zadanie 2

typedef struct {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement* next;
} QueueElement;

typedef struct {
    QueueElement* front;
    QueueElement* rear;
} Queue;

void free_queue(Queue* pqueue) {
    QueueElement* current = pqueue->front;
    while (current != NULL) {
        QueueElement* next = current->next;
        free(current);
        current = next;
    }

    // Zerowanie wskaźników na początek i koniec kolejki
    pqueue->front = NULL;
    pqueue->rear = NULL;
}

void push_copy(Queue* pqueue, const Data* pdata) {
    // Alokacja pamięci dla nowego elementu kolejki
    QueueElement* newElement = (QueueElement*)malloc(sizeof(QueueElement));
    newElement->data.results = pdata->results;
    newElement->data.len = pdata->len;
    newElement->next = NULL;

    // Wstawienie nowego elementu na koniec kolejki
    if (pqueue->rear == NULL) {
        pqueue->front = newElement;
        pqueue->rear = newElement;
    } else {
        pqueue->rear->next = newElement;
        pqueue->rear = newElement;
    }
}

int get_total_count(const Queue* pqueue) {
    int totalCount = 0;

    QueueElement* current = pqueue->front;
    while (current != NULL) {
        totalCount += current->data.len;
        current = current->next;
    }

    return totalCount;
}

// Zadanie 3

typedef enum {
    CAR,
    BUS,
    TRUCK
} VehicleType;

typedef enum {
    ELECTRIC,
    HYBRID,
    COMBUSTION
} DriveType;

typedef struct {
    char* owner;
    int next_check[3];
    VehicleType vehicle_type;
    DriveType drive_type;
    union {
        struct {
            int max_passengers;
            float engine_power;
        } car;
        struct {
            int num_seats;
            int num_standing_places;
        } bus;
        struct {
            float engine_capacity;
            float axle_load;
        } truck;
    } specific_data;
} Vehicle;

void new_owner(Vehicle* pvehicle) {
    if (pvehicle->owner != NULL) {
        free(pvehicle->owner);
        pvehicle->owner = NULL;
    }

    char buffer[100];
    printf("Enter new owner's name: ");
    scanf("%99s", buffer);

    pvehicle->owner = strdup(buffer);
}

int delayed(Vehicle* pvehicle, int size, int base_date, char** pdelayed_owners) {
    int delay_count = 0;

    for (int i = 0; i < size; i++) {
        Vehicle* current_vehicle = &pvehicle[i];
        int vehicle_date = current_vehicle->next_check[2] * 10000 + current_vehicle->next_check[1] * 100 + current_vehicle->next_check[0];

        if (vehicle_date < base_date) {
            pdelayed_owners[delay_count] = current_vehicle->owner;
            delay_count++;
        }
    }

    if (delay_count == 0) {
        *pdelayed_owners = NULL;
    }

    return delay_count;
}

void print_vehicle(const Vehicle* pvehicle) {
    printf("Owner: %s\n", pvehicle->owner);
    printf("Next check date: %02d-%02d-%04d\n", pvehicle->next_check[0], pvehicle->next_check[1], pvehicle->next_check[2]);
    printf("Vehicle type: %d\n", pvehicle->vehicle_type);
    printf("Drive type: %d\n", pvehicle->drive_type);

    switch (pvehicle->vehicle_type) {
        case CAR:
            printf("Max passengers: %d\n", pvehicle->specific_data.car.max_passengers);
            printf("Engine power: %f\n", pvehicle->specific_data.car.engine_power);
            break;
        case BUS:
            printf("Number of seats: %d\n", pvehicle->specific_data.bus.num_seats);
            printf("Number of standing places: %d\n", pvehicle->specific_data.bus.num_standing_places);
            break;
        case TRUCK:
            printf("Engine capacity: %f\n", pvehicle->specific_data.truck.engine_capacity);
            printf("Axle load: %f\n", pvehicle->specific_data.truck.axle_load);
            break;
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
