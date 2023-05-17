#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CBUFF_SIZE  52

int conflicts = 0;

int cardsA[CBUFF_SIZE];
int outA=0, lenA=0;

int cardsB[CBUFF_SIZE];
int outB=0, lenB=0;


double cardsA_push(int card){
    if(lenA == CBUFF_SIZE){
        return INFINITY;
    }
    cardsA[(outA + lenA) % CBUFF_SIZE] = card;
    lenA++;
    return 1.0;
}

double cardsB_push(int card){
    if(lenB == CBUFF_SIZE){
        return INFINITY;
    }
    cardsB[(outB + lenB) % CBUFF_SIZE] = card;
    lenB++;
    return 1.0;
}

int cardsA_pop(){
    if(lenA == 0){
        return -1;
    }
    int card = cardsA[outA];
    outA = (outA +1) % CBUFF_SIZE;
    lenA--;
    return card;
}

int cardsB_pop(){
    if(lenB == 0){
        return -1;
    }
    int card = cardsB[outB];
    outB = (outB +1) % CBUFF_SIZE;
    lenB--;
    return card;
}

void deck_print(int deck[], int out, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", deck[(out + i) % CBUFF_SIZE]);
    }
    printf("\n");
}


int rand_from_interval(int a, int b){
    return a + rand() % (b - a + 1);
}

void rand_permutation(int n, int tab[]) {
    if( n >= 0){
        for(int i=0; i<n; i++){
            tab[i]=i;
        }
        for(int i=0; i<n-1; i++){
            int k = rand_from_interval(i, n-1);
            int tmp = tab[i];
            tab[i] = tab[k];
            tab[k]=tmp;
        }
    }
}


void give_cards(int tab[]){
    for(int i=0; i<26; i++){
        cardsA_push(tab[i]);
    }
    for(int i=26; i<52; i++){
        cardsB_push(tab[i]);
    }
}


int game(int gamemode, int max_conf){
    int cardA;
    int cardB;
    while(conflicts<=max_conf && lenA>0 && lenB>0){
        cardA = cardsA[outA];
        cardB = cardsB[outB];

        //deck_print(cardsA,outA,lenA);
        //deck_print(cardsB,outB,lenB);
        //printf("%d %d\n", cardA, cardB);
        //printf("%d %d\n", cardA/4, cardB/4);
        //printf("%d\n", conflicts);


        if((cardA / 4) > (cardB / 4)){
            //A wygrywa konflikt
            //printf("A wins\n");
            cardsA_push(cardsA_pop());
            cardsA_push(cardsB_pop());
            conflicts++;
        }
        else if((cardA / 4) < (cardB / 4)){
            //B wygrywa konflikt
            //printf("B wins\n");
            cardsB_push(cardsB_pop());
            cardsB_push(cardsA_pop());
            conflicts++;
        }
        else{
            if(gamemode==1) {
                //printf("Draw\n");
                cardsA_push(cardsA_pop());
                cardsB_push(cardsB_pop());
                conflicts++;
            }
            else{
                //printf("War\n");
                int cnt=1;
                int outpA=(outA +2) % CBUFF_SIZE;
                int outpB=(outB +2) % CBUFF_SIZE;

                if(lenA==2 || lenB==2){
                    return 1;
                }

                int lengA=lenA-2;
                int lengB=lenB-2;
                int flag=0;

                conflicts++;

                while(conflicts<=max_conf && lengA>0 && lengB>0){
                    cardA = cardsA[outpA];
                    cardB = cardsB[outpB];
                    //printf("%d %d\n", cardA, cardB);
                    //printf("%d %d\n", cardA/4, cardB/4);
                    if((cardA / 4) > (cardB / 4)){
                        //printf("A wins\n");
                        cnt+=2;
                        conflicts++;
                        flag=1;
                        break;
                    }
                    else if((cardA / 4) < (cardB / 4)){
                        //printf("B wins\n");
                        cnt+=2;
                        conflicts++;
                        flag=2;
                        break;
                    }
                    else{
                        //printf("Continue \n");
                        cnt+=2;
                        outpA=(outpA +2) % CBUFF_SIZE;
                        outpB=(outpB +2) % CBUFF_SIZE;
                        lengA=lenA-2;
                        lengB=lenB-2;
                        if(lengA==2 || lengB==2){
                            return 1;
                        }
                    }
                    flag=0;
                }

                if(flag==1){
                    for(int i=0; i<cnt; i++){
                        cardsA_push(cardsA_pop());
                    }
                    for(int i=0; i<cnt; i++){
                        cardsA_push(cardsB_pop());
                    }
                }
                if(flag==2){
                    for(int i=0; i<cnt; i++){
                        cardsB_push(cardsB_pop());
                    }
                    for(int i=0; i<cnt; i++){
                        cardsB_push(cardsA_pop());
                    }
                }
            }
        }
    }

    if(conflicts > max_conf){
        return 0;
    }

    if(lenB==0){
        return 2;
    }

    if(lenA==0){
        return 3;
    }

    return -2; //error
}


int main() {
    int seed;
    int gamemode;
    int max_conf;

    scanf("%d %d %d", &seed, &gamemode, &max_conf);

    if(gamemode != 0 && gamemode != 1){
        return -1;
    }

    if(max_conf < 0){
        return -1;
    }

    srand(seed);
    int tab[CBUFF_SIZE];
    rand_permutation(52,tab);
    give_cards(tab);
    int wynik=-1;
    wynik=game(gamemode,max_conf);

    if(wynik==0){
        printf("0 ");
        printf("%d ", lenA);
        printf("%d\n", lenB);
    }
    else if(wynik==1){
        printf("1 ");
        printf("%d ", lenA);
        printf("%d\n", lenB);
    }
    else if(wynik==2){
        printf("2 ");
        printf("%d\n", conflicts);
    }
    else if(wynik==3){
        printf("3\n");
        deck_print(cardsB,outB,lenB);
    }

    return 0;
}