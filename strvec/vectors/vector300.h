#include <stddef.h>

typedef struct vector300 {
  void* array;
  size_t ele_size;
  size_t capacity;
  size_t length;
} vector_t;

void initialize_vector(vector_t* v, size_t type);

void destroy_vector(vector_t* v);

void* vector_get(vector_t* v, int index);

size_t vector_size(vector_t* v);

void vector_add_back(vector_t* v, void* ele);

void vector_delete_back(vector_t* v);

void vector_add(vector_t* v, void* ele, int index);

void vector_delete(vector_t* v, int index);
