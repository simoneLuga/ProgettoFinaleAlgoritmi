/*
        Nome: Simone
     Cognome: Lugaresi
N. Matricola: 0000970392
      Gruppo: A
       Email: simone.lugaresi@studio.unibo.it
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Box {
    int n;
    double x;
    double y;
    double z;
} Box;

Box* read_from_file(FILE* f, Box* BoxVector , int  maxBox) {
    int i = 0;
    double x, y, z;
    
    while (3 == fscanf(f, "%lf %lf %lf", &x, &y, &z)) {
        BoxVector[i].n = i;
        BoxVector[i].x = x;
        BoxVector[i].y = y;
        BoxVector[i].z = z;
        i++;
    }
    if (i != maxBox) {
        fprintf(stderr, "WARNING: ho letto %d scatole, ma l'intestazione ne dichiara %d\n", i, maxBox);
        return NULL;
    }
    return BoxVector;
}

int compareX(const void * b1, const void * b2) {
    const Box *p1=b1, *p2=b2;
    if (p1->x > p2->x)
        return 1;
    else if (p1->x < p2->x)
        return -1;
    else
        return 0;
}

void method(Box *BoxVector, int maxBox ) {/*longest increasing subsequence*/
    int* top, i, j, max = 1 /*e' il numero massimo di scatole uno dentro all'altro*/ , previous;
    top = malloc(sizeof(int*) * maxBox);       /*vettore di int che memorizza il massimo LIS nella posizione i*/
                                                
    for (i = 0; i < maxBox; i++) top[i] = 1;   /*inizializzato ad 1*/

    for (i = 1; i < maxBox; i++) {
        for (j = 0; j < i; j++) {
            if ((BoxVector[i].y > BoxVector[j].y) && (BoxVector[i].z > BoxVector[j].z) && (top[i] < top[j] + 1)) { /*controllo solo Y e Z perche ho ordinato precedetemente la X*/
                top[i] = top[j] + 1;
                if (max < top[i]) {
                    max = top[i];
                }
            }
        }
    }
    
    printf("%d scatole\n", max);
    for (j = 0; j < max; j++) {
        for (i = 0; i < maxBox; i++) {
            if ((top[i] == max - j)) {
                if (j != 0) { /*nel primo giro stampa direttamente senza controllare il precedente*/
                    if ((BoxVector[previous].y > BoxVector[i].y) && (BoxVector[previous].z > BoxVector[i].z)) { /*controlla che il precedente sia strettamente maggiore dell'elemento che sta controllando*/
                        printf("scatola %3d: %lf-%lf-%lf \n", BoxVector[i].n, BoxVector[i].x, BoxVector[i].y, BoxVector[i].z);
                        previous = i;
                        break;
                    }
                }
                else
                {
                    printf("scatola %3d: %lf-%lf-%lf \n", BoxVector[i].n, BoxVector[i].x, BoxVector[i].y, BoxVector[i].z);
                    previous = i;
                    break;
                }  
            }      
        }
    }
    free(top);
}

int main(int argc, char* argv[])
{
    FILE* filein = stdin;
    int maxBox;
    Box* BoxVector;

    if (argc > 1) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return 1;
        }
    }

    if (1 != fscanf(filein, "%d", &maxBox)) {
        fprintf(stderr, "ERRORE durante la lettura del numero di scatole\n");
        return -1;
    };

    BoxVector = (Box*)malloc(sizeof(Box) * maxBox);             /*inizializza il vettore di BOX al numero esatto letto*/
    BoxVector=read_from_file(filein,BoxVector,maxBox);          /*Riempie BoxVector leggendo il file di input*/              

    qsort(BoxVector, maxBox, sizeof(Box), compareX);            /*ordina BoxVector per la variabile X*/
    method(BoxVector, maxBox);                                     /*chiama la longest increasing subsequence su BoxVector*/
    return 0;
}
