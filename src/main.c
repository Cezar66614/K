#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE *src_file, *dst_file;
char *src_code; size_t src_code_len;

bool is_char_visible(char a) { return ((a >= 33) && (a <= 126)); }
bool is_char_number(char a) { return ((a >= '0') && (a <= '9')); }

char *get_word(size_t *length, char *p_start, char *p_end) {
  char *word_start, *word_end;
  char *word = NULL; *length = 0;

  word_start = word_end = p_start;

  if (*word_start == '/') {
    if ((word_start + 1 <= p_end) && *(word_start + 1) == '/') { // Start of a comment
      while ((*word_end != '\n') && (word_end <= p_end)) { ++word_end; ++(*length); }
      return NULL;
    }
  }

  switch (*word_start) {
  case '"': {// Start of a const char array
    ++word_end;

    bool escape = false; size_t escape_count = 0;
    while (word_end <= p_end) {
      if (*word_end == '\\') { ++escape_count; escape = escape_count % 2; }
      else {
        if ((*word_end == '"') && !escape) { ++word_end; break; }

        escape = false;
        escape_count = 0;
      }

      ++word_end;
    }

    break;
  }
  case '\'': {// Start of a const char
    ++word_end;

    bool escape = false; size_t escape_count = 0;
    while (word_end <= p_end) {
      if (*word_end == '\\') { ++escape_count; escape = escape_count % 2; }
      else {
        if ((*word_end == '\'') && !escape) { ++word_end; break; }

        escape = false;
        escape_count = 0;
      }

      ++word_end;
    }

    break;
  }

  case '-':
    if ((word_start + 1 <= p_end) && *(word_start + 1) == '>') { // Start of an indirect memory access
      ++word_end;
    }
    ++word_end;
    break;
  
  case '&':
    if ((word_start + 1 <= p_end) && *(word_start + 1) == '&') { // Start of an and operation
      ++word_end;
    }
    ++word_end;
    break;
  
  case '|':
    if ((word_start + 1 <= p_end) && *(word_start + 1) == '|') { // Start of an or operation
      ++word_end;
    }
    ++word_end;
    break;

  case '#':

  case '<':
  case '>':

  case '!':
  case '^':

  case '?':
  case ':':

  case '=':
  case '+':
  case '*':
  case '/':

  case '.':

  case '[':
  case ']':

  case '(':
  case ')':
  case '{':
  case '}':

  case ',':
  case ';':
    ++word_end;
    break;

  default:
    bool end = false;
    while (!end && is_char_visible(*word_end) && (word_end <= p_end)) {
      switch (*word_end) {
      case '"':
      case '\'':

      case '-':
      case '&':
      case '|':

      case '#':

      case '<':
      case '>':

      case '!':
      case '^':

      case '?':
      case ':':

      case '=':
      case '+':
      case '*':
      case '/':

      case '.':

      case '[':
      case ']':

      case '(':
      case ')':
      case '{':
      case '}':

      case ',':
      case ';':

        end = true;
        break;
      
      default:
        ++word_end;
        break;
      }
    }

    break;
  }

  word = malloc(sizeof(char) * (word_end - word_start + 1));
  if (word == NULL) { printf("ERROR Word\n"); return NULL; }

  while (word_start + *length < word_end) {
    word[*length] = *(word_start + *length);
    ++(*length);
  }

  word[*length] = '\0';

  return word;
}

int main(int argc, char **argv) {
  if (argc == 1) return -1;

  printf("Working on: %s\n", argv[1]);

  src_file = fopen(argv[1], "rb");
  if (src_file == NULL) return -1;

  fseek(src_file, 0, SEEK_END);
  src_code_len = ftell(src_file);
  fseek(src_file, 0, SEEK_SET);

  src_code = malloc(src_code_len + 1);
  fread(src_code, src_code_len, 1, src_file);

  src_code[src_code_len] = '\0';

  fclose(src_file);

  // printf("%s\n%ld\n", src_code, src_code_len);

  char *p_start = src_code, *p_end = src_code + src_code_len;
  char *word = NULL; size_t word_len;

  while (p_start <= p_end) {
    if (!is_char_visible(*p_start)) { ++p_start; continue; }

    word = get_word(&word_len, p_start, p_end);
    printf("%ld: ", word_len);
    p_start += word_len;

    if (word != NULL) {
      printf("%s\n", word);

      free(word); word = NULL;
    }
  }

  free(src_code);
  return 0;
}
