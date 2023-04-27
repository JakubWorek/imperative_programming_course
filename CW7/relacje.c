#include <stdio.h>
#define MAX_REL_SIZE 100

typedef struct {
	int first;
	int second;
} pair;

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    pair *p1 = (pair *)a;
    pair *p2 = (pair *)b;
    if (p1->first == p2->first)
        return p1->second - p2->second;

    return p1->first - p2->first;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    int result = 0;

    for( int i = 0; i < n; i++ ){
        result = cmp_pair(&tab[i], &new_pair);

        if( result != 0 ){
            if( tab[i].first == -1 && tab[i].second == -1 ){
                tab[i].first = new_pair.first;
                tab[i].second = new_pair.second;
                return i;
            }
        }
        else{
            return i;
        }
    }

    return n;
}

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int size){
    int x = 0;
    int flag = 0;

    for(int i=0; i<size; i++){
        flag = 0;
        x = relation[i].first;
        for(int j=0; j<size; j++){
            if(x == relation[j].first && x == relation[j].second){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size){
    if(is_reflexive(relation,size)==0){
        return 1;
    }
    return 0;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size){
    int x=0;
    int y=0;
    int flag = 0;

    for(int i=0; i<size; i++){
        flag = 0;
        x = relation[i].first;
        y = relation[i].second;

        for(int j=0; j<size; j++){
            if(y == relation[j].first && x == relation[j].second){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size){
    int x = 0;
    int y = 0;

    for( int i = 0; i < size; i++ ){
        x = relation[i].first;
        y = relation[i].second;

        for( int j = 0; j < size; j++ ){
            if( y == relation[j].first && x == relation[j].second){
                if( x != y ){
                    return 0;
                }
            }
        }
    }

    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int size){
    int x = 0;
    int y = 0;

    for( int i = 0; i < size; i++ ){
        x = relation[i].first;
        y = relation[i].second;

        for( int j = 0; j < size; j++ ){
            if( y == relation[j].first && x == relation[j].second){
                return 0;
            }
        }
    }

    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int size){
    int x = 0;
    int y = 0;
    int z = 0;
    int flag = 0;

    for( int i = 0; i < size; i++ ){
        x = relation[i].first;
        y = relation[i].second;

        for( int j = 0; j < size; j++ ){
            if( y == relation[j].first ){
                z = relation[j].second;
                flag = 0;

                for( int k = 0; k < size; k++ ){
                    if( x == relation[k].first && z == relation[k].second){
                        flag = 1;
                        break;
                    }
                }

                if( flag == 0 ){
                    return 0;
                }
            }
        }
    }

    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* relation, int size){
    if( is_reflexive(relation, size) == 1 && is_antisymmetric(relation, size) == 1 && is_transitive(relation, size) == 1 ){
        return 1;
    }
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* relation, int size){
    int x = 0;
    int y = 0;
    int flag = 0;
    int x_size = 0;
    int X[size];

    for(int i = 0; i < size; i++) X[i] = -1;

    // X - tablica wartości
    for(int i=0; i<size; i++){
        x = relation[i].first;
        y = relation[i].second;
        flag = 0;

        for(int j=0; j<x_size; j++){
            if(x == X[j]){
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            X[x_size] = x;
            x_size++;
        }

        flag = 0;

        for(int j=0; j<x_size; j++){
            if(y == X[j]){
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            X[x_size] = y;
            x_size++;
        }
    }

    // Sprawdzanie wartości z X z relacją
    for(int i = 0; i<x_size-1; i++){
        x = X[i];
        for(int j=i+1; j<x_size; j++){
            y = X[j];
            flag = 0;

            for(int k=0; k<size; k++){
                if( (x == relation[k].first && y == relation[k].second) || (y == relation[k].first && x == relation[k].second) ){
                    flag = 1;
                    break;
                }
            }

            if(flag == 0){
                return 0;
            }
        }
    }

    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair* relation, int size){
    if( is_partial_order(relation, size) == 1 && is_connected(relation, size) == 1){
        return 1;
    }
    return 0;
}

int get_domain(pair* relation, int size, int* domain){
    int n=0;
    int flag = 0;
    int x=0;
    int y=0;

    for(int i = 0; i < size; i++) domain[i] = -1;

    for(int i=0; i<size; i++){
        x=relation[i].first;
        y=relation[i].second;

        flag = 0;

        for(int j=0; j<n; j++){
            if(domain[j] == x){
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            domain[n]=x;
            for(int j=n; j>0; j--){
                if(x<domain[j-1]){
                    domain[j] = domain[j-1];
                    domain[j-1] = x;
                }
                else{ break; }
            }
            n++;
        }

        flag = 0;

        for(int j=0; j<n; j++){
            if(domain[j] == y){
                flag = 1; break;
            }
        }

        if(flag == 0){
            domain[n]=y;
            for(int j=n; j>0; j--){
                if(y<domain[j-1]){
                    domain[j] = domain[j-1];
                    domain[j-1] = y;
                }
                else{ break; }
            }
            n++;
        }
    }

    return n;
}

int find_max_elements(pair* relation, int size, int* max_elements){
    int domena[MAX_REL_SIZE];
    int d_size = get_domain(relation,size,domena);
    int m_size = 0;
    int flag = 0;

    for(int i = 0; i < size; i++) max_elements[i] = -1;

    for(int i=0; i<d_size; i++){
        for(int j=0; j<size; j++){
            flag = 0;
            if(relation[j].first == domena[i] && relation[j].second != domena[i]){
                flag = 1;
                break;
            }
        }
        if(flag == 0){ max_elements[m_size] = domena[i]; m_size++; }
    }

    return m_size;
}

int find_min_elements(pair* relation, int size, int* max_elements){
    int domena[MAX_REL_SIZE];
    int d_size = get_domain(relation,size,domena);
    int m_size = 0;
    int flag = 0;

    for(int i = 0; i < size; i++) max_elements[i] = -1;

    for(int i=0; i<d_size; i++){
        for(int j=0; j<size; j++){
            flag = 0;
            if(relation[j].second == domena[i] && relation[j].first != domena[i]){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            max_elements[m_size] = domena[i];
            m_size++;
        }
    }

    return m_size;
}
// Case 3:

int composition (pair* relation1, int size1, pair* relation2, int size2, pair* relation3){
    int size3 = 0;
    int res = 0;
    pair para;

    for(int i=0; i<MAX_REL_SIZE; i++){ relation3[i].first = -1; relation3[i].second = -1; }

    for(int i=0; i<size2; i++){
        for(int j=0; j<size1; j++){
            if(relation2[i].first == relation1[j].second){
                para.first = relation1[j].first;
                para.second = relation2[i].second;
                res = add_relation(relation3, size1, para);
                if(res > size3){
                    size3 = res;
                }
            }
        }
    }

    return size3+1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n = 0;
    int result = 0;
    int max_size = 0;
    pair para;

    scanf("%d",&n);

    for( int i = 0; i < n; i++ ){
        relation[i].first = -1;
        relation[i].second = -1;
    }

    for( int i = 0; i < n; i++ ){
        scanf("%d",&para.first);
        scanf("%d",&para.second);

        result = add_relation( relation, n, para);

        if( result > max_size ){
            max_size = result;
        }
    }

    return max_size+1;
}

void print_int_array(int *array, int n) {
    printf("%d\n", n);

    for (int i = 0; i < n; ++i){
        printf("%d ", array[i]);
    }

    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}