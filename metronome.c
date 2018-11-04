#define _POSIX_C_SOURCE 200112L

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define DEFAULT_TEMPO 120
#define DEFAULT_DIV 4
#define MAX_DIVS 10

int tempo = 0;
int divs[MAX_DIVS];
size_t ndiv;

int exists(int val, int *arr, size_t n) {
    for (size_t i = 0; i < n; i++)
	if (arr[i] == val)
	    return 1;
    return 0;
}

int max(int *arr, size_t n) {
    int max = INT_MIN;
    for (size_t i = 0; i < n; i++)
	if (arr[i] > max)
	    max = arr[i];
    return max;
}

int gcd2(int x, int y) {
    if (y == 0) return x;
    else return gcd2(y, x%y);
}

int lcm2(int x, int y) {
    return x*y / gcd2(x,y);
}

int lcm(int *arr, size_t n) {
    if (n == 1) return arr[0];

    int lcm = 1;
    for (size_t i = 0; i < n; i++)
	lcm = lcm2(lcm, arr[i]);
    return lcm;
}

void metronome() {
    size_t nsubdiv = lcm(divs, ndiv);
    /* length of each cycle depends on this */
    int basediv = max(divs, ndiv);
    double cyclelen = (double) 60*1000000000/tempo*basediv;
    struct timespec delay = {.tv_sec = 0,
			     .tv_nsec = cyclelen/nsubdiv};

    /* calculate the number of subdivisions between each beep for each division */
    int subdivs[ndiv];
    for (size_t i = 0; i < ndiv; i++) {
	subdivs[i] = nsubdiv/divs[i];
    }

    /* print the borders */
    for (size_t i = 0; i < ndiv; i++) {
	printf("| ");
	for (size_t j = 0; j < nsubdiv; j++)
	    printf(" ");
	printf(" |\n");
    }
    /* navigate to top-left corner */
    printf("\033[2C");
    printf("\033[%ldA", ndiv);
    printf("\033[s");

    while (1) {
	for (size_t i = 0; i < nsubdiv; i++) {
	    for (size_t j = 0; j < ndiv; j++) {
		if (i%subdivs[j] == 0) {
		    /* navigate to correct row */
		    printf("\033[u");
		    printf("\033[%ldB", j);

		    /* print division number with padding */
		    for (size_t k = 0; k < i; k++)
			printf(" ");
		    printf("%ld", i/subdivs[j]+1);

		    /* beep (\n registers the beep, \b ensures display is unaffected) */
		    printf("\a\n\b");
		}
	    }

	    nanosleep(&delay, NULL);
	}

	/* clear all division numbers */
	printf("\033[u");
	for (size_t i = 0; i < ndiv; i++) {
	    for (size_t j = 0; j < nsubdiv; j++)
		printf(" ");
	    printf("\033[B");
	    printf("\033[%ldD", nsubdiv);
	}
	printf("\033[u");
    }
}

int main(int argc, char **argv) {
    int c;
    int div;
    char *rawdiv;
    char *token;
    
    while ((c = getopt(argc, argv, "t:d:")) != -1) {
	switch (c) {
	case 't':
	    tempo = atoi(optarg);
	    break;
	case 'd':
	    rawdiv = optarg;
	    while ((token = strtok(rawdiv, ",")) != NULL) {
		div = atoi(token);
		if (div > 0 && ndiv < MAX_DIVS && !exists(div, divs, ndiv))
		    divs[ndiv++] = div;
	        rawdiv = NULL;
	    }
	    break;
	}
    }

    if (tempo == 0)
	tempo = DEFAULT_TEMPO;

    if (ndiv == 0) {
	ndiv = 1;
	divs[0] = DEFAULT_DIV;
    }

    printf("tempo: %d\n", tempo);
    printf("divisions: ");
    for (size_t i = 0; i < ndiv; i++)
	printf("%d ", divs[i]);
    printf("\n");

    metronome();
}
