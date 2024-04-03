#ifndef BL_IO_H
#define BL_IO_H

int read_entire_file(char* file_name, char** buffer);

#ifdef BL_IO_IMPLEMENTATION
int read_entire_file(char* file_name, char** buffer) {
  FILE *file = fopen(file_name, "r");
  int   file_size;

  if (!file) return 0;

  // Find file size and malloc a buffer
  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  *buffer = malloc(file_size+1);

  // Read the file into the buffer
  size_t len = fread(*buffer, sizeof(char), file_size, file);
  if (ferror(file)) {
    fprintf(stderr, "Could not read source file '%s'\n", file_name);
    return 0;
  }
  fclose(file);
  return file_size;
}
#endif

#endif
