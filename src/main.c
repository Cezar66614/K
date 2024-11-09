#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *code_read(size_t *code_len, FILE *code_file) {
  char *code = NULL; *code_len = 0;

  fseek(code_file, 0, SEEK_END);
  *code_len = ftell(code_file);
  fseek(code_file, 0, SEEK_SET);

  code = malloc(*code_len + 1);
  if (code == NULL) return NULL;
  fread(code, *code_len, 1, code_file);

  code[*code_len] = '\0';

  return code;
}

#define IS_CHAR_VISIBLE(a) (((a) >= 33) && ((a) <= 126))
#define IS_CHAR_LETTER_UPPER(a) (((a) >= 'A') && ((a) <= 'Z'))
#define IS_CHAR_LETTER_LOWER(a) (((a) >= 'a') && ((a) <= 'z'))
#define IS_CHAR_LETTER(a) (IS_CHAR_LETTER_UPPER(a) || IS_CHAR_LETTER_LOWER(a))
#define IS_CHAR_NUMBER(a) (((a) >= '0') && ((a) <= '9'))
#define IS_CHAR_SPECIAL(a) (IS_CHAR_VISIBLE(a) && !IS_CHAR_LETTER(a) && !IS_CHAR_NUMBER(a) && ( ((a) != '`') && ((a) != '~') && ((a) != '$') && ((a) != '%') && ((a) != '@')))

char *lexer_get_token(size_t *length, char *p_start, char *p_end) {
  char *word = NULL; *length = 0;
  char *word_start, *word_end;
  
  word_start = word_end = p_start;

  if (IS_CHAR_SPECIAL(*word_start)) {
    if (*word_start == '/') {
      if ((word_start + 1 <= p_end) && *(word_start + 1) == '/') { // Start of a comment
        while ((*word_end != '\n') && (word_end <= p_end)) { ++word_end; ++(*length); }
        return NULL;
      }
    } else if (*word_start == '"') {
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
    } else if (*word_start == '\'') {
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
    } else if (*word_start == '-') {
      if ((word_start + 1 <= p_end) && *(word_start + 1) == '>') { // Start of an indirect memory access
        ++word_end;
      }
      ++word_end;
    } else if (*word_start == '&') {
      if ((word_start + 1 <= p_end) && *(word_start + 1) == '&') { // Start of an and operation
        ++word_end;
      }
      ++word_end;
    } else if (*word_start == '|') {
      if ((word_start + 1 <= p_end) && *(word_start + 1) == '|') { // Start of an or operation
        ++word_end;
      }
      ++word_end;
    } else {
      ++word_end; // just isolate the single special character
    }
  } else {
    while (IS_CHAR_VISIBLE(*word_end) && (word_end <= p_end)) {
      if (IS_CHAR_SPECIAL(*word_end)) break;
      ++word_end;
    }
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

char **lexer(size_t *tokens_count, char *code, size_t code_len) {
  char **tokens = NULL; *tokens_count = 0;

  char *p_start = code, *p_end = code + code_len;
  char *word = NULL; size_t word_len;

  while (p_start <= p_end) {
    if (!IS_CHAR_VISIBLE(*p_start)) { ++p_start; continue; }

    word = lexer_get_token(&word_len, p_start, p_end);
    // printf("%ld: ", word_len);
    p_start += word_len;

    if (word != NULL) {
      //printf("%s\n", word);

      ++(*tokens_count);
      tokens = reallocarray(tokens, *tokens_count, sizeof(char *));
      if (tokens == NULL) return NULL;

      tokens[*tokens_count - 1] = word;

      word = NULL;
    }
  }

  return tokens;
}
void lexer_free(char **tokens, size_t tokens_count) {
  size_t i;
  for (i = 0; i < tokens_count; ++i) free(tokens[i]);
  free(tokens);
}

int main(int argc, char **argv) {
  if (argc == 1) return 1;

  printf("Working on: %s\n", argv[1]);

  char *src_code = NULL; size_t src_code_len = 0;

  FILE *src_file = fopen(argv[1], "rb");
  if (src_file == NULL) return 1;

  src_code = code_read(&src_code_len, src_file);

  fclose(src_file);

  if (src_code == NULL) return 1;

  // printf("%s\n%ld\n", src_code, src_code_len);

  char **src_tokens = NULL; size_t src_tokens_count = 0;

  src_tokens = lexer(&src_tokens_count, src_code, src_code_len);

  free(src_code);

  if (src_tokens == NULL) return 1;


  FILE *dst_file = fopen("out.c", "w");
  if (dst_file == NULL) return 1;

  size_t i;
  for (i = 0; i < src_tokens_count; ++i) {
    printf("%s\n", src_tokens[i]);
    fprintf(dst_file, "%s ", src_tokens[i]);
  }

  fclose(dst_file);

  lexer_free(src_tokens, src_tokens_count);
  src_tokens = NULL; src_tokens_count = 0;

  return 0;
}
