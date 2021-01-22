#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>

extern "C" {

#include "vector300.h"
}

using namespace std;

int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

const char* words[15] = {"hello",   "these", "are",    "some",     "words",
                         "because", "I",     "needed", "pointers", "this",
                         "array",   "is",    "global", "cool",     "beans"};

// returns 0 upon failure, 1 for success
int do_compare_string(vector<char*> system, vector_t* student, int type) {
  if (student->array == nullptr || system.size() != student->length) {
    return 0;
  }

  for (size_t i = 0; i < student->length; i++) {
    char* str = *((char**)(((char*)student->array) + (i * type)));
    if (strcmp(system[i], str)) {
      return 0;
    }
  }

  return 1;
}
int do_compare_int(vector<int> system, vector_t* student, int type) {
  if (student->array == nullptr || system.size() != student->length) {
    return 0;
  }

  for (size_t i = 0; i < student->length; i++) {
    if (system[i] != *((int*)(((char*)student->array) + (i * type)))) {
      return 0;
    }
  }

  return 1;
}

void print_diff_int(vector_t* student, vector<int> system, string test) {
  printf("Failed test: %s.\n", test.c_str());

  if (student->array == nullptr) {
    return;
  }

  if (system.size() != student->length) {
    printf("sizes do not match. Expected size: %lu. Actual size: %lu\n",
           system.size(), student->length);
  }
  printf("Expected Result:\n");
  for (size_t i = 0; i < system.size(); i++) {
    printf("%d ", system[i]);
  }
  printf("\n");

  printf("Actual Result:\n");
  for (size_t i = 0; i < student->length; i++) {
    int ele = *((int*)(((char*)student->array) + (i * 4)));
    printf("%d ", ele);
  }
  printf("\n");
}
void print_diff_string(vector_t* student, vector<char*> system, string test) {
  printf("Failed test: %s.\n", test.c_str());

  if (student->array == nullptr) {
    return;
  }

  if ((size_t)system.size() != student->length) {
    printf("sizes do not match. Expected size: %lu. Actual size: %lu\n",
           system.size(), student->length);
  }
  printf("Expected Result:\n");
  for (size_t i = 0; i < system.size(); i++) {
    printf("%s ", system[i]);
  }
  printf("\n");

  printf("Actual Result:\n");
  for (size_t i = 0; i < student->length; i++) {
    char* ele = *((char**)(((char*)student->array) + (i * 8)));
    printf("%s ", ele);
  }
  printf("\n");
}

int test_initialize(vector_t* string_vect, vector_t* int_vect) {
  // total = 1
  initialize_vector(string_vect, 8);
  initialize_vector(int_vect, 4);
  // kind of hard to check this because its open ended, address sanitizers are
  // more effective for this, but this make sures they are correctly setting
  // element size
  if (string_vect->array != nullptr && string_vect->ele_size == 8 &&
      int_vect->ele_size == 4) {
    printf("Passed 1/1 initialize tests.\n");
    return 1;
  } else {
    printf("Failed test: Initialize.\n");
    return 0;
  }
}

int test_adding(vector_t* string_vect, vector<char*>& string_sol) {
  int total = 5;
  int points = 0;

  if (string_vect->array == nullptr) {
    printf("Failed test: Add Test.\n");
    return 0;
  }

  // test add_back to empty vector
  vector_add_back(string_vect, &words[0]);
  string_sol.push_back((char*)words[0]);
  points += do_compare_string(string_sol, string_vect, 8);

  // test regular adding to back
  vector_add_back(string_vect, &words[1]);
  string_sol.push_back((char*)words[1]);
  points += do_compare_string(string_sol, string_vect, 8);

  // test add to front of nonempty vector
  vector_add(string_vect, &words[2], 0);
  string_sol.insert(string_sol.begin(), (char*)words[2]);
  points += do_compare_string(string_sol, string_vect, 8);

  // test add to back of nonempty vector
  vector_add(string_vect, &words[3], 2);
  string_sol.insert(string_sol.begin() + 2, (char*)words[3]);
  points += do_compare_string(string_sol, string_vect, 8);

  // test add to the middle of a vector
  vector_add(string_vect, &words[4], 1);
  string_sol.insert(string_sol.begin() + 1, (char*)words[4]);

  vector_add(string_vect, &words[5], 2);
  string_sol.insert(string_sol.begin() + 2, (char*)words[5]);

  vector_add(string_vect, &words[6], 3);
  string_sol.insert(string_sol.begin() + 3, (char*)words[6]);

  vector_add(string_vect, &words[7], 2);
  string_sol.insert(string_sol.begin() + 2, (char*)words[7]);

  vector_add(string_vect, &words[8], 7);
  string_sol.insert(string_sol.begin() + 7, (char*)words[8]);

  vector_add(string_vect, &words[9], 6);
  string_sol.insert(string_sol.begin() + 6, (char*)words[9]);

  vector_add(string_vect, &words[10], 9);
  string_sol.insert(string_sol.begin() + 9, (char*)words[10]);
  points += do_compare_string(string_sol, string_vect, 8);

  // show differences
  if (points < total) {
    print_diff_string(string_vect, string_sol, "Add Test");
  }

  printf("Passed %d/%d add tests.\n", points, total);
  return points;
}

int test_realloc(vector_t* int_vect, vector<int>& int_sol) {
  int total = 3;
  int points = 0;

  if (int_vect->array == nullptr) {
    printf("Failed test: Realloc Test.\n");
    return 0;
  }

  // get initial capacity of the vector
  size_t size1 = malloc_usable_size(int_vect->array);

  // this doubles as a test to add to an empty vector
  vector_add(int_vect, &numbers[1], 0);
  int_sol.insert(int_sol.begin(), numbers[1]);
  points += do_compare_int(int_sol, int_vect, 4);

  // doubles as a test for add back after regular add (ensures that add updates
  // the length)
  vector_add_back(int_vect, &numbers[2]);
  int_sol.push_back(numbers[2]);
  points += do_compare_int(int_sol, int_vect, 4);

  vector_add(int_vect, &numbers[0], 0);
  int_sol.insert(int_sol.begin(), numbers[0]);
  vector_add(int_vect, &numbers[3], 3);
  int_sol.insert(int_sol.begin() + 3, numbers[3]);
  vector_add_back(int_vect, &numbers[4]);
  int_sol.push_back(numbers[4]);
  // check that length = 3, capacity = 4, and realloc happened
  size_t size2 = malloc_usable_size(int_vect->array);
  if (int_vect->length == 5 && int_vect->capacity == 8 && size1 < size2) {
    points++;
  }

  if (points < total) {
    print_diff_int(int_vect, int_sol, "Realloc Test");
  }

  printf("Passed %d/%d realloc tests.\n", points, total);
  return points;
}

int test_size(vector_t* string_vect, vector<char*>& string_sol) {
  // total = 1;
  int points = 0;

  if (string_vect->array == nullptr) {
    printf("Failed test: Size test.\n");
    return 0;
  }

  if (vector_size(string_vect) == string_sol.size()) {
    points++;
  } else {
    printf("Failed test: Size Test. Expected size = %lu. Actual size = %lu\n",
           string_sol.size(), vector_size(string_vect));
  }

  printf("Passed 1/1 size tests.\n");
  return points;
}

int test_delete(vector_t* string_vect, vector<char*>& string_sol) {
  int total = 6;
  int points = 0;

  if (string_vect->array == nullptr) {
    printf("Failed test: Delete Test.\n");
    return 0;
  }

  // test delete_back on nonempty vector
  vector_delete_back(string_vect);
  string_sol.pop_back();
  points += do_compare_string(string_sol, string_vect, 8);

  // test delete from last index
  vector_delete(string_vect, 9);
  string_sol.erase(string_sol.begin() + 9);
  points += do_compare_string(string_sol, string_vect, 8);

  // test delete from first index
  vector_delete(string_vect, 0);
  string_sol.erase(string_sol.begin());
  points += do_compare_string(string_sol, string_vect, 8);

  // test some deletes from the middle
  vector_delete(string_vect, 3);
  string_sol.erase(string_sol.begin() + 3);

  vector_delete(string_vect, 5);
  string_sol.erase(string_sol.begin() + 5);
  points += do_compare_string(string_sol, string_vect, 8);

  // try an add_back after delete
  vector_add_back(string_vect, &words[11]);
  string_sol.push_back((char*)words[11]);
  points += do_compare_string(string_sol, string_vect, 8);

  // try an add after delete
  vector_add(string_vect, &words[12], 3);
  string_sol.insert(string_sol.begin() + 3, (char*)words[12]);
  points += do_compare_string(string_sol, string_vect, 8);

  if (points < total) {
    print_diff_string(string_vect, string_sol, "Delete Test");
  }

  printf("Passed %d/%d delete tests.\n", points, total);
  return points;
}

int test_get(vector_t* int_vect, vector<int>& int_sol) {
  int test1 = 0;
  int test2 = 0;
  int test3 = 0;
  int total = 3;

  // test get of first element:
  int* first_ele = (int*)vector_get(int_vect, 0);
  if (first_ele == nullptr) {
    printf("Failed test: Get Test.\n");
    return 0;
  }
  if (int_sol.at(0) == *first_ele) {
    test1 = 1;
  } else {
    printf("Failed test: Get Test.\n");
    int ele = *(int*)vector_get(int_vect, 0);
    printf("Expected at index 0: %d. Actual at index 0: %d\n", int_sol.at(0),
           ele);
  }

  // test get of last element
  if (int_sol.at(4) == *(int*)vector_get(int_vect, 4)) {
    test2 = 1;
  } else {
    printf("Failed test: Get Test.\n");
    int ele = *(int*)vector_get(int_vect, 4);
    printf("Expected at index 4: %d. Actual at index 4: %d\n", int_sol.at(4),
           ele);
  }

  // test getting something in the middle
  if (int_sol.at(2) == *(int*)vector_get(int_vect, 2)) {
    test3 = 1;
  } else {
    printf("Failed test: Get Test.\n");
    int ele = *(int*)vector_get(int_vect, 2);
    printf("Expected at index 2: %d. Actual at index 2: %d\n", int_sol.at(2),
           ele);
  }

  int points = test1 + test2 + test3;
  printf("Passed %d/%d get tests.\n", points, total);
  return points;
}

int test_insert_delete() {
  // total = 1
  // start with empty vectors
  vector_t int_vect;
  initialize_vector(&int_vect, 4);
  vector<int> int_sol;

  if (int_vect.array == nullptr) {
    printf("Failed test: Insertion and Deletion test.\n");
    return 0;
  }

  // continuously push/pop
  vector_add_back(&int_vect, &numbers[0]);
  int_sol.push_back(numbers[0]);
  vector_delete_back(&int_vect);
  int_sol.pop_back();

  vector_add_back(&int_vect, &numbers[0]);
  int_sol.push_back(numbers[0]);
  vector_delete_back(&int_vect);
  int_sol.pop_back();

  vector_add_back(&int_vect, &numbers[0]);
  int_sol.push_back(numbers[0]);
  vector_delete_back(&int_vect);
  int_sol.pop_back();

  vector_add_back(&int_vect, &numbers[0]);
  int_sol.push_back(numbers[0]);

  int points = do_compare_int(int_sol, &int_vect, 4);

  if (!points) {
    print_diff_int(&int_vect, int_sol, "Insertion and Deletion Test");
  }

  destroy_vector(&int_vect);

  printf("Passed %d/1 insert and deletion tests.\n", points);
  return points;
}

int main() {
  vector_t string_vect;
  vector_t int_vect;
  vector<int> int_sol;
  vector<char*> string_sol;

  int points = 0;
  printf("\n");

  points += test_initialize(&string_vect, &int_vect);
  points += test_adding(&string_vect, string_sol);
  points += test_size(&string_vect, string_sol);
  points += test_realloc(&int_vect, int_sol);
  points += test_delete(&string_vect, string_sol);
  points += test_get(&int_vect, int_sol);
  // I don't really know if this test is worth while
  points += test_insert_delete();

  // this will need to be checked with an address sanitizer outside of this
  // points system or just by visually looking at the code and making sure they
  // call free
  destroy_vector(&string_vect);
  destroy_vector(&int_vect);

  printf("\n=====================\nTotal Score: %d out of 20\n", points);
  return 0;
}
