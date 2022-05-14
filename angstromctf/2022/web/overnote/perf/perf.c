#include<stdio.h>
#include<stdlib.h>

//        with open(f"users/{username}", "w") as f:
 //           f.write(content[0] + "\n" + new_content)
 //
int main(int argc, char *argv[]) {
    FILE *fptr;

    fptr = fopen(argv[1], "w");
    if (fptr == NULL) {
        fprintf(stderr, "err");
        exit(1);
    }

    fprintf(fptr, "%s", argv[2]);
    fprintf(fptr, "\n");
    fprintf(fptr, "%s", argv[3]);

    fclose(fptr);
    return 0;
}
