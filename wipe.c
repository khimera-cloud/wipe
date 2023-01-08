/*

khime.cloud wipe program
This program is used to "wipe" (overwrite with zeroes) the free space on a filesystem.
It is useful when shrinking/cloning a disk.
Usage: wipe <path>
If path is omitted, ./ is used

*/
#define _GNU_SOURCE //needed by asprintf() before including stdio.h
#include <stdio.h> //printf()
#include <stdlib.h> //exit()
#include <unistd.h> //unlink()
#include <time.h> //time()
#include <signal.h> //signal()
#include <sys/stat.h> //stat()
#include <string.h> //strlen()

#include "wipe.h"

//Display a human readable size
char *humanSize(unsigned long bytes) {
 char *ret;
 int i = 0;
 const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

 while (bytes>=1024) {
  bytes/=1024;
  i++;
 }
 asprintf(&ret, "%lu%s", bytes, units[i]);
 return ret;
}

int main(int argc, char *argv[]) {
 srand(time(NULL));
 char *fn;
 if (argc<2) asprintf(&fn, "./.wipe.zeroes.%i", rand());
 else {
  if (argv[1][strlen(argv[1])-1]=='/') argv[1][strlen(argv[1])-1]='\0';
  asprintf(&fn, "%s/.wipe.zeroes.%i", argv[1], rand());
 }

 printf("Running khimera.cloud wipe with file %s\n", fn);

 struct stat statbuf;
 if (stat(fn, &statbuf)==0) ex_err("File already exists?\n");

 char *zeroes = malloc(BUFSIZE);
 if (zeroes==NULL) ex_err("Can't allocate memory!\n");
 FILE *f = fopen(fn, "wb");
 if (f==NULL) ex_err("Cound not open file!\n");

 void cleanup(int sig) {
  printf("\33[2K\r");
  fflush(stdout);
  free(zeroes);
  printf("Closing and deleting file\n");
  fclose(f);
  unlink(fn);
  free(fn);
  exit(EXIT_SUCCESS);
 }
 signal(SIGINT, cleanup);

 for (int i=0; i<BUFSIZE; i++) zeroes[i]='\0';
 unsigned long written = 0;

 while(1) {
  if (fwrite(zeroes, BUFSIZE, 1, f)!=1) break;
  written+=BUFSIZE;
  char *tmp = humanSize(written);
  printf("\33[2K\rWriting zeroes: %lu bytes (%s)", written, tmp);
  free(tmp);
  fflush(stdout);
 }

 cleanup(0);
}
