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
    printf("discountrate is %.1f\n", discountrate);
    printf("taxrate addr is %p\n", &taxrate);
    printf("tmpPtr stored addr is %p\n", tmpPtr);
    strcpy(buyer,"network programming");
    strcpy(seller,buyer);
    printf("seller and buyer are ");
    if (strcmp(seller,buyer)) printf("different\n"); else printf("the same.\n");
    printf("buyer is %s\n", buyer);
    printf("seller is %s\n", seller);
    strcat(buyer,seller);
    printf("buyer length is %d\n", strlen(buyer));

	return EXIT_SUCCESS;
}
