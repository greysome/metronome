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
    for (int i = 0; i < n; i++)
	if (arr[i] == val)
	    return 1;
    return 0;
}

int max(int *arr, size_t n) {
    int max = INT_MIN;
    for (int i = 0; i < n; i++)
	if (arr[i] > max)
	    max = arr[i];
    return max;
}

int _gcd2(int x, int y) {
    if (y == 0) return x;
    else return _gcd2(y, x%y);
}

int _lcm2(int x, int y) {
    return x*y / _gcd2(x,y);
}

int lcm(int *arr, size_t n) {
    if (n == 1) return arr[0];

    int lcm = 1;
    for (int i = 0; i < n; i++)
	lcm = _lcm2(lcm, arr[i]);
    return lcm;
}

void metronome() {
    int nsubdiv = lcm(divs, ndiv);
    /* length of each cycle depends on this */
    int basediv = max(divs, ndiv);
    double cyclelen = 60000000/tempo*basediv;
    double delay = cyclelen/nsubdiv;

    /* calculate the number of subdivisions between each beep for each division */
    int subdivs[ndiv];
    for (int i = 0; i < ndiv; i++) {
	subdivs[i] = nsubdiv/divs[i];
    }

    /* print the borders */
    for (int i = 0; i < ndiv; i++) {
	printf("| ");
	for (int j = 0; j < nsubdiv; j++)
	    printf(" ");
	printf(" |\n");
    }
    /* navigate to top-left corner */
    printf("\033[2C");
    printf("\033[%dA", ndiv);
    printf("\033[s");

    while (1) {
	for (int i = 0; i < nsubdiv; i++) {
	    for (int j = 0; j < ndiv; j++) {
		if (i%subdivs[j] == 0) {
		    /* navigate to correct row */
		    printf("\033[u");
		    printf("\033[%dB", j);

		    /* print division number with padding */
		    for (int k = 0; k < i; k++)
			printf(" ");
		    printf("%d", i/subdivs[j]+1);

		    /* beep (\n registers the beep, \b ensures display is unaffected) */
		    printf("\a\n\b");
		}
	    }
	    usleep(delay);
	}

	/* clear all division numbers */
	printf("\033[u");
	for (int i = 0; i < ndiv; i++) {
	    for (int j = 0; j < nsubdiv; j++)
		printf(" ");
	    printf("\033[B");
	    printf("\033[%dD", nsubdiv);
	}
	printf("\033[u");
    }
}

int main(int argc, char **argv) {
    int c;
    int div;
    char *token;
    
    while ((c = getopt(argc, argv, "t:d:")) != -1) {
	switch (c) {
	case 't':
	    tempo = atoi(optarg);
	    break;
	case 'd':
	    while ((token = strtok(optarg, ",")) != NULL) {
		div = atoi(token);
		if (div > 0 && ndiv < MAX_DIVS && !exists(div, divs, ndiv))
		    divs[ndiv++] = div;
		optarg = NULL;
	    }
	    break;
	}
    }

    if (tempo == 0)
	tempo = DEFAULT_TEMPO;

    if (ndiv == 0) {
	ndiv = 1;
	divs[ndiv] = DEFAULT_DIV;
    }

    printf("tempo: %d\n", tempo);
    printf("divisions: ");
    for (int i = 0; i < ndiv; i++)
	printf("%d ", divs[i]);
    printf("\n");

    metronome();
}
