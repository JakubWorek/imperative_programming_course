#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size){
    vector->element_size = element_size;
    vector->capacity = block_size*element_size;
    vector->data = malloc(vector->capacity);
    vector->size = 0;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity){
    if( vector->capacity < new_capacity ){
        vector->data = realloc(vector->data, vector->capacity*2);
        vector->capacity = vector->capacity*2;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size){
    if( vector->size < new_size){
        reserve(vector, new_size*(vector->element_size));
        memset(&vector->data[(vector->size)*vector->element_size], 0, (new_size-vector->size)*(vector->element_size));
    }

    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value){
    resize(vector, vector->size+1);

    for (int i = 0; i < vector->element_size; i++){
        ((char*)vector->data)[(vector->size-1)*vector->element_size+i] = ((char*)value)[i];
    }
}

// Remove all elements from the vector
void clear(Vector *vector){
    resize(vector, 0);
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value){
    resize(vector, vector->size+1);
    memmove(&vector->data[(index+1)*vector->element_size], &vector->data[(index)*vector->element_size], (vector->size-index-1)*vector->element_size);

    for (int i = 0; i < vector->element_size; i++){
        ((char*)vector->data)[index*vector->element_size+i] = ((char*)value)[i];
    }

}

// Erase element at position index
void erase(Vector *vector, size_t index){
    memmove(&vector->data[index*vector->element_size], &vector->data[(index+1)*vector->element_size], (vector->size-(index+1))*vector->element_size);
    resize(vector, vector->size-1);
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp){
    int i = 0;

    while( i < vector->size ){
        if( !cmp(value, &vector->data[i*vector->element_size]) ){
            erase(vector, i);
        }
        else{
            i += 1;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)){
    int i = 0;

    while( i < vector->size ){
        if (predicate(&vector->data[i*vector->element_size])){
            erase(vector, i);
        }
        else{
            i += 1;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector){
    vector->data = realloc(vector->data, vector->size*vector->element_size);
    vector->capacity = vector->size*vector->element_size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2){
    return *(const int*)v1 - *(const int*)v2;
}

// char comparator
int char_cmp(const void *v1, const void *v2){
    return *(const char*)v1 - *(const char*)v2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2){
    Person* per1 = (Person *)p1;
    Person* per2 = (Person *)p2;

    if( per1->age != per2->age ){
        return per2->age - per1->age;
    }
    else if( strcmp(per1->first_name, per2->first_name) ){
        return strcoll(per1->first_name, per2->first_name);
    }
    return strcoll(per1->last_name, per2->last_name);
}

// predicate: check if number is even
int is_even(void *value){
    return ( ( *(int*)value )+1 )%2;
}

// predicate: check if char is a vowel
int is_vowel(void *value){
    char c = *(char *)value;

    if( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y'){
        return 1;
    }

    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person){
    return ( (Person*)person )->age > 25;
}

// print integer value
void print_int(const void *v) {
    Vector* vector=(Vector *)v;
    printf("%ld\n", vector->capacity/vector->element_size);

    for( int i = 0; i < vector->size; i++ ) {
        printf("%d ", *(int *) &vector->data[i * sizeof(int)]);
    }

    printf("\n");
}

// print char value
void print_char(const void *v){
    Vector* vector=(Vector *)v;
    printf("%ld\n", vector->capacity/vector->element_size);

    for( int i = 0; i < vector->size; i++ ){
        printf("%c ", *(char*)&vector->data[i*sizeof(char)]);
    }

    printf("\n");
}

// print structure Person
void print_person(const void *v){
    Vector* vector=(Vector *)v;
    printf("%ld\n", vector->capacity/vector->element_size);

    for( int i = 0; i < vector->size; i++ )
    {
        Person* p = (Person*)&vector->data[i*sizeof(Person)];
        printf("%d %s %s\n", p->age, p->first_name, p->last_name);
    }

    printf("\n");
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    print(vector);
}

// read int value
void read_int(void* value){
    scanf("%d", (int*)value);
}

// read char value
void read_char(void* value){
    char c[2];
    scanf("%s", c);
    *(char*)value = c[0];
}

// read struct Person
void read_person(void* value){
    Person *person = (Person*)value;
    scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

static void* safe_malloc(size_t size){
    void* p = malloc(size);
    if(!p){
        exit(-1);
    }
    return p;
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = safe_malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
                print_vector(vector, print);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}


