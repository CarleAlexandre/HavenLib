#ifndef HAVEN_FILE_HPP
# define HAVEN_FILE_HPP

char *readFile(const char *filepath);
void writeFile(const char *filepath, const char *data, size_t n);

#endif
