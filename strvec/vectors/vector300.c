#include "vector300.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Initializes all of the fields of a vector struct
 *Arguments:
 * - v: a pointer to a vector_t
 * - type: a size_t indicating the size of the type of this vector's elements
 *Return value: none
 */
void initialize_vector(vector_t* v, size_t type) {
  // TODO: implement
  v->array = NULL;  // you'll need to change this
}

/*Frees this vector
 *Arguments:
 * - v: a pointer to the vector which needs to be freed
 *Return value: none
 */
void destroy_vector(vector_t* v) {
  // TODO: implement
}

/*Gets the size of a given vector
 *Arguments:
 * - v: the vector whose size is desired
 *Return value: an integer containing the size of the vector
 */
size_t vector_size(vector_t* v) {
  // TODO: implement
  return (size_t)0;
}

/*Gets the element at a desired position within a vector
 *Arguments:
 * - v: a pointer to a vector_t
 * - index: the index of the desired element in v (with 0 indexing)
 *Return value: a void pointer to the element at index (to be casted
 *appropriately by the caller)
 */
void* vector_get(vector_t* v, int index) {
  // TODO: implement
  return NULL;
}

/*Adds an element to the back of a vector, doubling the capacity of the vector
 *if needed Arguments:
 * - v: a pointer to a vector_t
 * - ele: a pointer to the element to be copied into v
 * Return value: none
 */
void vector_add_back(vector_t* v, void* ele) {
  // TODO: implement
}

/*Removes the last element in a vector
 *Arguments:
 * - v: a pointer to a vector_t
 *Return value: none
 */
void vector_delete_back(vector_t* v) {
  // TODO: implement
}

/*Adds an element to a specified index in a vector, double its capacity if
 *needed Arguments:
 * - v: a pointer to a vector_t
 * - ele: a pointer to the element to be copied into v
 * - index: the desired index for ele in the vector, v (using 0 indexing)
 *Return value: none
 */
void vector_add(vector_t* v, void* ele, int index) {
  // TODO: implement
}

/*Deletes an element from the specified position in a vector
 *Arguments:
 * - v: a pointer to a vector_t
 * - index: the index of the element to be deleted from v (using 0 indexing)
 *Return value: none
 */
void vector_delete(vector_t* v, int index) {
  // TODO: implement
}
