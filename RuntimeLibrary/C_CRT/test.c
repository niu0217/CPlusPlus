/* ************************************************************************
> File Name:     test.c
> Author:        niu0217
> Created Time:  Mon 19 Feb 2024 02:58:02 PM CST
> Description:
 ************************************************************************/

#include "minicrt.h"

int main(int argc, char* argv[])
{
    int i;
    FILE* fp;
    char** v = malloc(argc * sizeof(char*));
    for(i = 0; i < argc; i++) {
        v[i] = malloc(strlen(argv[i] + 1));
        strcpy(v[i], argv[i]);
    }

    fp = fopen("test.txt", "w");
    for(i = 0; i < argc; i++) {
        int len = strlen(v[i]);
        fwrite(&len, 1, sizeof(int), fp);
        fwrite(v[i], 1, len, fp);
    }
    fclose(fp);

    fp = fopen("test.txt", "r");
    for(i = 0; i < argc; i++) {
        int len;
        char* buf;
        fread(&len, 1, sizeof(int), fp);
        buf = malloc(len + 1);
        fread(buf, 1, len, fp);
        buf[len] = '\0';
        printf("%d %d\n", len, buf);
        free(buf);
        fread(v[i]);
    }
    fclose(fp);
}
