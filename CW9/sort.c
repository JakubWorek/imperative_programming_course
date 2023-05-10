#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 1

typedef struct {
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 i 8.2 funkcja bsearch2 i value

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

int cmp_food1(const void *f1, const void *f2){
    const Food *food1 = (Food *)f1;
    const Food *food2 = (Food *)f2;
    if(food1->valid_date.year > food2->valid_date.year){ return 1; }
    else if(food1->valid_date.year < food2->valid_date.year){ return -1; }
    else {
        if (food1->valid_date.month > food2->valid_date.month) { return 1; }
        else if (food1->valid_date.month < food2->valid_date.month) { return -1; }
        else {
            if (food1->valid_date.day > food2->valid_date.day) { return 1; }
            else if (food1->valid_date.day < food2->valid_date.day) { return -1; }
            else {
                int name_cmp = 0;
                name_cmp = strcmp(food1->art, food2->art);
                if(name_cmp == 0){
                    if(food1->price > food2->price){ return 1; }
                    else if(food1->price < food2->price){ return -1; }
                    else return 0;
                }
                else return name_cmp;
            }
        }
    }
}

int cmp_food(const void *f1, const void *f2){
    const Food *food1 = (Food *)f1;
    const Food *food2 = (Food *)f2;
    if(food1->price > food2->price){ return 1; }
    else if(food1->price < food2->price){ return -1; }
    else {
        if(food1->valid_date.year > food2->valid_date.year){ return 1; }
        else if(food1->valid_date.year < food2->valid_date.year){ return -1; }
        else {
            if (food1->valid_date.month > food2->valid_date.month) { return 1; }
            else if (food1->valid_date.month < food2->valid_date.month) { return -1; }
            else {
                if (food1->valid_date.day > food2->valid_date.day) { return 1; }
                else if (food1->valid_date.day < food2->valid_date.day) { return -1; }
                else {
                    int name_cmp = 0;
                    name_cmp = strcmp(food1->art, food2->art);
                    return name_cmp;
                }
            }
        }
    }
}

void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compar, char *result) {
    const char *base_ptr = base;
    size_t low = 0, high = nitems;
    while( low < high ){
        size_t mid = (low + high) /2;
        const char *mid_ptr = base_ptr + mid * size;
        int cmp = cmp_food(key, mid_ptr);
        if(cmp == 0){
            memcpy(result, "FOUND", 6);
            return (void *)mid_ptr;
        }
        else if(cmp == -1){
            high = mid;
        }
        else{
            low = mid+1;
        }
    }
    memcpy(result, "NOT FOUND", 10);
    return (void *)base_ptr+low*size;
}

void print_art(Food *p, size_t n, char *art) {
    for(int i=0; i<n; i++){
        if(strcmp(p[i].art, art)==0){
            printf("%.2f %.2f %d %d %d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

Food* add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new) {
    char result[10];
    Food *found = bsearch2(new, tab, *np, sizeof(Food), compar, result);
    if( strcmp(result, "FOUND")==0){
        found->amount += new->amount;
        return found;
    }
    else{
        if(*np >= tab_size){ return NULL; }
        int index = found - tab;
        memmove(found+1, found, (*np-index)*sizeof(Food));
        memcpy(found, new, sizeof(Food));
        (*np)++;
        return found;
    }
}

int read_stream(Food *tab, size_t size, int no, FILE *stream) {
    int i = 0;
    int ile = 0;

    for(i; i<no; i++){
        Food new_food;
        fscanf(stream, "%s %f %f %d %d %d", &new_food.art, &new_food.price, &new_food.amount, &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
        add_record(tab, size, &ile, cmp_food, &new_food);
    }

    return ile;
}

int read_stream0(Food *tab, size_t size, int no, FILE *stream) {
    int i = 0;
    int ile = 0;

    for(i; i<no; i++){
        Food new_food;
        fscanf(stream, "%s %f %f %d %d %d", &new_food.art, &new_food.price, &new_food.amount, &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
        add_record(tab, size, &ile, cmp_food, &new_food);
    }

    return ile;
}


float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
    qsort(food_tab, n, sizeof(Food), cmp_food1);

    struct tm cdate = {0};
    cdate.tm_year = curr_date.year - 1900;
    cdate.tm_mon = curr_date.month - 1;
    cdate.tm_mday = curr_date.day;
    time_t ctime = mktime(&cdate);

    float val = 0.0;

    for(int i=0; i<n; i++){
        struct tm fdate = {0};
        fdate.tm_year = food_tab[i].valid_date.year - 1900;
        fdate.tm_mon = food_tab[i].valid_date.month - 1;
        fdate.tm_mday = food_tab[i].valid_date.day;
        time_t ftime = mktime(&fdate);

        if(ctime+((anticip)*24*60*60) == ftime){
            val+=((food_tab[i].amount)*(food_tab[i].price));
        }
    }

    return val;
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL pretendent:1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
    int noc;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = { 28, 10, 2011 }; // Data zmiany kolejności sukcesji

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
    int nop=0;
    for(int i=0; i<size; i++){
        idx_tab[i].par_name = pers_tab[i].name;
    }
    for(int i=0; i<size; i++){
        idx_tab[i].index = 0;
    }
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(pers_tab[j].parent == idx_tab[i].par_name){
                idx_tab[i].index = j;
                break;
            }
        }
    }
    for(int i=0; i<size; i++){
        int start = idx_tab[i].index;
        int ile = 0;
        for(int j=start; j<size; j++){
            if(pers_tab[j].parent == pers_tab[i].name){
                ile+=1;
            }
            else{
                break;
            }
        }
        idx_tab[i].noc = ile;
    }
    for(int i=0; i<size; i++){
        if(idx_tab[i].noc >0){nop+=1;}
    }
    return nop;
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
    for(int i=0; i<size; i++){
        if(idx_tab[i].noc >0){
            Person *shifted_arr = malloc((idx_tab[i].noc)*sizeof(Person));
            memmove(shifted_arr, &person_tab[idx_tab[i].index], (idx_tab[i].noc)*sizeof(Person));
            int ile = idx_tab[i].index - i - 1;
            int gdzie = idx_tab[i].index - ile + idx_tab[i].noc;
            memmove(&person_tab[gdzie],&person_tab[i+1],ile*sizeof(Person));
            memmove(&person_tab[i+1], shifted_arr, idx_tab[i].noc*sizeof(Person));
            free(shifted_arr);
            no_parents = fill_indices_tab(idx_tab, person_tab, size);
        }
    }
}

int cleaning(Person *person_tab,int n) {
    for(int i=0; i<n; i++){
        if(person_tab[i].bits.pretendent == no){
            memmove(&person_tab[i],&person_tab[i+1],(n-(i+1))*sizeof(Person));
            n-=1;
            i-=1;
        }
    }
    return n;
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for(int i=1; i<=n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
}

int cmp_date(const void* da, const void* db){
    Date *d1 = (Date *)da;
    Date *d2 = (Date *)db;

    if(d1->year < d2->year){return -1;}
    else if(d1->year > d2->year){return 1;}
    else{
        if(d1->month < d2->month){return -1;}
        else if(d1->month > d2->month){return 1;}
        else{
            if(d1->day < d2->day){return -1;}
            else if(d1->day > d2->day){return 1;}
            else{return 0;}
        }
    }
}

int cmp_persons(const void *f1, const void *f2){
    const Person *p1 = (Person *)f1;
    const Person *p2 = (Person *)f2;

    if(p1->parent == NULL){return -1;}
    else if(p2->parent == NULL){ return 1;}

    int parrent = strcmp(p1->parent, p2->parent);
    if(parrent<0){return -1;}
    else if(parrent>0){return 1;}
    else{
        int born_date = cmp_date(&p1->born, &p2->born);
        if(cmp_date(&p1->born, &primo_date) < 0 &&  cmp_date(&p2->born, &primo_date) < 0 ){
            if(p1->bits.sex == M && p2->bits.sex == F){return -1;}
            else if(p1->bits.sex == F && p2->bits.sex == M){return 1;}
        }
        if(born_date<0){return -1;}
        else if(born_date>0){return 1;}
        else{
            if(p1->bits.sex > p2->bits.sex){return 1;}
            else if(p1->bits.sex < p2->bits.sex){return -1;}
            else{return 0;}
        }
    }
}

int create_list(Person *person_tab, int n) {
    qsort(person_tab, n, sizeof(Person), cmp_persons);
    Parent id_tab[33];
    int no_par=0;
    no_par = fill_indices_tab(id_tab, person_tab, n);
    persons_shiftings(person_tab, n, id_tab, no_par);
    print_persons(person_tab, n);
    int no_per = 0;
    no_per = cleaning(person_tab,n);
    return no_per;
}

////////////////////////////////////////////////////////////////

int main(void)
{
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33]={
            {"Charles III",M,no,14,11,1948,"Elizabeth II"},
            {"Anne",F,yes,15,8,1950,"Elizabeth II"},
            {"Andrew",M,yes,19,2,1960,"Elizabeth II"},
            {"Edward",M,yes,10,3,1964,"Elizabeth II"},
            {"David",M,yes,3,11,1961,"Margaret"},
            {"Sarah",F,yes,1,5,1964,"Margaret"},
            {"William",M,yes,21,6,1982,"Charles III"},
            {"Henry",M,yes,15,9,1984,"Charles III"},
            {"Peter",M,yes,15,11,1977,"Anne"},
            {"Zara",F,yes,15,5,1981,"Anne"},
            {"Beatrice",F,yes,8,8,1988,"Andrew"},
            {"Eugenie",F,yes,23,3,1990,"Andrew"},
            {"James",M,yes,17,12,2007,"Edward"},
            {"Louise",F,yes,8,11,2003,"Edward"},
            {"Charles",M,yes,1,7,1999,"David"},
            {"Margarita",F,yes,14,5,2002,"David"},
            {"Samuel",M,yes,28,7,1996,"Sarah"},
            {"Arthur",M,yes,6,5,2019,"Sarah"},
            {"Georg",M,yes,22,7,2013,"William"},
            {"George VI",M,no,14,12,1895,NULL},
            {"Charlotte",F,yes,22,5,2015,"William"},
            {"Louis",M,yes,23,4,2018,"William"},
            {"Archie",M,yes,6,5,2019,"Henry"},
            {"Lilibet",F,yes,4,6,2021,"Henry"},
            {"Savannah",F,yes,29,12,2010,"Peter"},
            {"Isla",F,yes,29,3,2012,"Peter"},
            {"Mia",F,yes,17,1,2014,"Zara"},
            {"Lena",F,yes,18,6,2018,"Zara"},
            {"Elizabeth II",F,no,21,4,1925,"George VI"},
            {"Margaret",F,no,21,8,1930,"George VI"},
            {"Lucas",M,yes,21,3,2021,"Zara"},
            {"Sienna",F,yes,18,9,2021,"Beatrice"},
            {"August",M,yes,9,2,2021,"Eugenie"}
    };

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE *file;
    if(TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d",&no); getchar();
            if(TEST) file = stdin;
            else {
                scanf("%s",buff);
                file = fopen(buff,"r");
                if(file==NULL) { printf("Error 1\n"); break; }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
            n = read_stream(food_tab,FOOD_MAX,no,file);
            if (TEST) printf("Wpisz nazwe artykulu: ");
            scanf("%s",buff);
            print_art(food_tab,n,buff);
            break;
        case 2: // proste sortowanie struktur
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d",&no); getchar();
            if(TEST) file = stdin;
            else {
                scanf("%s",buff);
                file = fopen(buff,"r");
                if(file==NULL) { printf("Error 1\n"); break; }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
            n = read_stream0(food_tab,FOOD_MAX,no,file);
            Date curr_date;
            int anticip;
            if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
            scanf("%d %d %d",&curr_date.day,&curr_date.month,&curr_date.year);
            if (TEST) printf("Wpisz roznice dni: ");
            scanf("%d",&anticip);
            printf("%.2f\n",value(food_tab,n,curr_date,anticip));
            break;
        case 3: // sukcesja
            if(TEST==1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
            scanf("%d",&no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            no_persons = create_list(person_tab,no_persons);
            if(TEST) print_persons(person_tab,no_persons);
            else print_person(person_tab+no-1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}