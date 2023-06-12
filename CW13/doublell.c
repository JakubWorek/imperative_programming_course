#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *element = safe_malloc(sizeof(Node));

    element->data = data;
    element->array_size = array_size;
    element->prev = NULL;
    element->next = NULL;

    if(list->tail){
        list->tail->next = element;
        element->prev = list->tail;
    }

    list->tail = element;

    if(!list->head)
        list->head = list->tail;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
    Node *element = itr->node_ptr;

    while( element->array_size < n ){
        n -= element->array_size;
        element = element->next;
    }

    itr->node_ptr = element;
    itr->position = n;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator itr = begin(list->head);

    skip_forward(&itr, n);

    return itr.node_ptr->data[itr.position-1];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
    Node *element = itr->node_ptr;

    while( element->array_size < n ){
        n -= element->array_size;
        element = element->prev;
    }

    itr->node_ptr = element;
    itr->position = n;
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator itr = end(list->tail);

    skip_backward(&itr, n);

    return itr.node_ptr->data[itr.node_ptr->array_size-itr.position];
}

void remove_node(Node *node_ptr) {
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator itr = begin(list->head);

    skip_forward(&itr, n);

    if( itr.node_ptr->array_size == 1 ){
        if( !itr.node_ptr->prev && !itr.node_ptr->next ){
            list->head = NULL;
            list->tail = NULL;

        }
        else if( !itr.node_ptr->prev ){
            list->head = itr.node_ptr->next;
            list->head->prev = NULL;
        }
        else if( !itr.node_ptr->next ){
            list->tail = itr.node_ptr->prev;
            list->head->next = NULL;
        }
        else{
            itr.node_ptr->prev->next = itr.node_ptr->next;
            itr.node_ptr->next->prev = itr.node_ptr->prev;
            //list->size -= 1;
        }

        free(itr.node_ptr->data);
        free(itr.node_ptr);
    }
    else{
        int *tab = malloc((itr.node_ptr->array_size-1)*sizeof(int));

        memcpy(tab, itr.node_ptr->data, (itr.position-1)*sizeof(int));
        memcpy(&(tab[itr.position-1]), &(itr.node_ptr->data[itr.position]), (itr.node_ptr->array_size-1-(itr.position-1))*sizeof(int));

        free(itr.node_ptr->data);

        itr.node_ptr->data = tab;
        itr.node_ptr->array_size -= 1;
    }
}

// return the number of digits of number n
size_t digits(int n) {
    size_t count = 0;
    if (n == 0)
        return 1;
    else if (n < 0)
        n = -n;

    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
    size_t length = digits(value);

    Node *curr = list->head;

    while( curr ){
        if(curr->data) {
            if (digits(curr->data[0]) == length) {
                break;
            }
        }
        curr = curr->next;
    }

    if (curr && digits(curr->data[0]) == length) {
        size_t index = 0;
        while (index < curr->array_size && value > curr->data[index]) {
            index++;
        }

        curr->data = safe_realloc(curr->data, (curr->array_size + 1) * sizeof(int));

        for (size_t i = curr->array_size; i > index; i--) {
            curr->data[i] = curr->data[i - 1];
        }

        curr->data[index] = value;
        curr->array_size++;
    } else {
        int *data = safe_malloc(sizeof(int));
        data[0] = value;

        Node *what = create_node(data, 1, NULL, NULL);
        Node *where = list->head;
        while(where){
            if(where->data) {
                if (digits(where->data[0]) >= length) {
                    break;
                }
            }
            where = where->next;
        }

        if(where){
            what->prev = where->prev;
            where->prev->next = what;
            what->next = where;
            where->prev = what;
        }
        else{
            list->tail->next = what;
            what->prev = list->tail;
            list->tail = what;
        }
    }
}


// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next->next; node != NULL; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
            //sortList(&list);
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}