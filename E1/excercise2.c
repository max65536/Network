#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	double taxrate=7.3, discountrate;
    char buyer[100], seller[100];
    double* tmpPtr;
    tmpPtr = &taxrate;
    printf("the value of tempPtr is %.1f\n", *tmpPtr);
    discountrate = *tmpPtr;
    printf("taxrate addr is %p\n", &taxrate);
    printf("tmpPtr addr is %p\n", tmpPtr);
    strcpy(buyer,"network programming");
    strcpy(seller,buyer);
    printf("buyer is %s\n", buyer);
    printf("seller is %s\n", seller);
    strcat(buyer,seller);
    printf("buyer length is %d\n", strlen(buyer));

	return EXIT_SUCCESS;
}
