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
Route longroutes[10];
Route route1;
Route* routePtr;




int main(void) {
	printf("please input routeID\n");
    scanf("%d", &route1.routeID);
	printf("please input descrption\n");
    scanf("%s", &route1.descrp);
    longroutes[3] = route1;
    routePtr = longroutes;
    printf("routeID of longroutes[3] is %d\n", (routePtr+3)->routeID);
    printf("descrp of longroutes[3] is %s\n", (routePtr+3)->descrp);




	return EXIT_SUCCESS;
}
