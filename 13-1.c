//
// Created by vislo on 24.05.2021.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

void move_next(char *path, int *len) {
    if (path[*len - 1] != 'z') {
        // a -> b, d -> e, zzc -> zzd, ...
        path[*len - 1]++;
    } else {
        // z -> za, zz -> zza, zzz -> zzza, ...
        strcat(path, "a");
        (*len)++;
    }
}

void do_recursive_symlink(char *current_file, int len, int *depth) {
    // copy old current_file
    char *next_file = malloc(sizeof(char) * (len + 1));
    strcpy(next_file, current_file);
    // figure out next file name
    move_next(next_file, &len);
    // try create symlink
    if (symlink(current_file, next_file) == -1) return;
    // if ok do it gain
    do_recursive_symlink(next_file, len, depth);
    // after done increase depth
    (*depth)++;
    // unlink so we do not create TOO MANY files (like even ls doesn't work)
    unlink(next_file);
    // free the memory
    free(next_file);
}

int main() {
    FILE *f;

    //try open a file
    f = fopen("a", "w");
    if (f == NULL) {
        printf("Failed to create file\n");
        exit(-1);
    }
    // put some info into the file
    if (fputs("a", f) == -1) {
        printf("Failed to create data in file\n");
        exit(-1);
    }
    // close the file since we do not need it opened anymore
    if (fclose(f) == -1) {
        printf("Failed to close the file\n");
        exit(-1);
    }

    int recursion_depth = 0;
    do_recursive_symlink("a", 1, &recursion_depth);
    printf("Max possible symlink recursion depth: %d\n", recursion_depth);

    return 0;
}
