#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_CODE_MAX 2048
#define SRC_CODE_MAX_LEN 32

FILE *src_file, *dst_file;
char *src_code; size_t src_code_len;

int main(int argc, char **argv) {
  if (argc == 1) return -1;

  printf("K: The C betterer!\n");

  src_file = fopen(argv[1], "rb");
  if (src_file == NULL) return -1;

  fseek(src_file, 0, SEEK_END);
  src_code_len = ftell(src_file);
  fseek(src_file, 0, SEEK_SET);

  src_code = malloc(src_code_len + 1);
  fread(src_code, src_code_len, 1, src_file);

  src_code[src_code_len] = '\0';

  fclose(src_file);

  printf("%s\n%ld\n", src_code, src_code_len);

  dst_file = fopen("output.c", "w");
  if (dst_file == NULL) return -1;

  fclose(dst_file);

  free(src_code);
  return 0;
}
