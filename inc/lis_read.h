#ifndef LIS_READ_H
#define LIS_READ_H

ssize_t read_file(const char *, void **);

ssize_t fread_file(int, void **);

char * get_file_value(const char *);

#endif /* LIS_READ_H */
