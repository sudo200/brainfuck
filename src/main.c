#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "v0.0.1"

#define EQUALS(x, y) (strcmp(x, y) == 0)

__attribute__((noreturn)) static void version(void) {
  fputs(VERSION, stderr);
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}

__attribute__((noreturn)) static void help(void) {
  fputs(
    "bfi [-v | --version] [-h | --help] [-c <inline code>] [-s arraysize] file ...\n"
    "\n"
    "sudo200 and contributors\n"
  , stderr);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  FILE *stream = NULL;
  size_t array_size = 30000;

  for(int i = 1; i < argc; i++) {
    if(EQUALS(argv[i], "-c") && stream == NULL) {
      int ii = ++i;
      stream = fmemopen(argv[ii], strlen(argv[ii]), "r");
    }
    else if(EQUALS(argv[i], "-s"))
      array_size = strtoul(argv[++i], NULL, 0);
    else if(EQUALS(argv[i], "-v") || EQUALS(argv[i], "--version"))
      version();
    else if(EQUALS(argv[i], "-h") || EQUALS(argv[i], "--help"))
      help();
    else
      stream = fopen(argv[i], "r");
  }

  if(stream == NULL)
    help();

  unsigned char array[array_size], *ptr = array;
  memset(array, '\0', sizeof(*array) * array_size);

  size_t stack[0xFFFF], stack_top;

  while(!feof(stream)) {
    switch(fgetc(stream)) {
      case '>': {
        ptr++;
      }
      break;

      case '<': {
        ptr--;
      }
      break;

      case '+': {
        ++*ptr;
      }
      break;

      case '-': {
        --*ptr;
      }
      break;

      case ',': {
        *ptr = getchar();
      }
      break;

      case '.': {
        putchar(*ptr);
      }
      break;

      case '[': { // Safe current position
        stack[stack_top++] = ftell(stream);
      }
      break;

      case ']': { // Get most nested position
        if(stack_top == 0UL)
          break;

        if(*ptr != 0)
          fseek(stream, stack[stack_top - 1], SEEK_SET);
        else
         stack_top--;
      }
      break;

      default: continue;
    }
  }

  fclose(stream);
  return EXIT_SUCCESS;
}

