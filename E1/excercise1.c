/*
 ============================================================================
 Name        : test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
struct route{
	int routeID;
	char descrp[25];
};
typedef struct route Route;
struct route longroutes[10];
struct route* routePtr;




int main(void) {
	Route route1;
	printf("please input routeID\n");
    scanf("%d", &route1.routeID);
	printf("please input descrp\n");
    scanf("%s", &route1.descrp);
    printf("routeID is %d\n",route1.routeID);
    printf("descrp is %s\n",route1.descrp);
    longroutes[3] = route1;
    routePtr = longroutes;
    printf("longroutes[3] is %s\n", (routePtr+3)->descrp);




	return EXIT_SUCCESS;
}
