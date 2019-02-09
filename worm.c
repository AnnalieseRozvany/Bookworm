#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include "worm.h"
#include <time.h>

//size_t g_nthreads = 7; // number of threads

// Returns result set containing book with given id.
result_t* find_book(book_t* nodes, size_t count, size_t book_id) {
  // clock_t start = clock();
  // printf("Starting find_book: %ld\n", start);
  // Global variables for thread to access easily

  for (int i = 0; i < count; i++) {
		if (nodes[i].id == book_id) {
      result_t* result = (result_t*)malloc(1*sizeof(result_t));
      result->elements = (book_t**)malloc(1*sizeof(book_t)); // initialise lements
      result->elements[0] = &nodes[i]; // set elements to the found book
      result->n_elements = 1;
      // clock_t found = clock();
      // printf("book_found: %ld\n", found);
			return result;
		}
	}

  result_t* result = (result_t*)malloc(1*sizeof(result_t));

  result->elements = NULL; // initialise lements
  result->n_elements = 0;

  // clock_t found = clock();
  // printf("book_found: %ld\n", found);

	return result;
}

// Returns result set containing books by given author.
result_t* find_books_by_author(book_t* nodes, size_t count, size_t author_id) {

  // clock_t start = clock();
  // printf("Starting author_book: %ld\n", start);

  for (int i = 0; i < count; i++) {
		if (nodes[i].author_id == author_id) { // book has been found
      result_t* value = (result_t*)malloc(1*sizeof(result_t));
      size_t size = nodes[i].n_author_edges + 1;
      value->n_elements = size;
      value->elements = (book_t**)malloc(size*sizeof(book_t)); // initialise elements
      value->elements[0] = &nodes[i];
      for (int j = 1; j < size; j++) {
        value->elements[j] = &nodes[nodes[i].b_author_edges[j-1]];
      }
      // clock_t found = clock();
      // printf("book_found: %ld\n", found);
      return value;
		}
	}

  result_t* value = (result_t*)malloc(1*sizeof(result_t));
  value->elements = (book_t**)malloc(1*sizeof(book_t)); // initialise elements
  value->n_elements = 0;

  // clock_t found = clock();
  // printf("book_not_found: %ld\n", found);

	return value;
}

// Returns result set containing books that have been reprinted by a different publisher.
result_t* find_books_reprinted(book_t* nodes, size_t count, size_t publisher_id) {

  size_t indexCount = 0;
  size_t indexes[count];

  for (int i = 0; i < count; i++) {
    if (nodes[i].publisher_id == publisher_id) {
      for (int j = 0; j < nodes[i].n_author_edges; j++) {
        if (nodes[nodes[i].b_author_edges[j]].id == nodes[i].id && nodes[nodes[i].b_author_edges[j]].publisher_id != publisher_id) {
          indexes[indexCount] = nodes[i].b_author_edges[j];
          indexCount++;
        }
      }
    }
  }

  result_t* value = (result_t*)malloc(1*sizeof(result_t));
  value->elements = (book_t**)malloc(indexCount*sizeof(book_t)); // initialise lements

  for (int i = 0; i<indexCount; i++) {
    value->elements[i] = &nodes[indexes[i]];
  }
  value->n_elements = indexCount;
	return value;
}


// Returns result set containing books that are k distance from given book.
result_t* find_books_k_distance(book_t* nodes, size_t count, size_t book_id, uint16_t k) {

  // 	clock_t start = clock();
	// printf("Starting k_dis: %ld\n", start);

  if (k > count/4) {
    result_t* value = (result_t*)malloc(1*sizeof(result_t));

    size_t index;

    int book_found = 0;

    for (int i = 0; i < count; i++) {
  		if (nodes[i].id == book_id) {
        book_found = 1;
        index = i;
  			break;
  		}
  	}

    if (book_found == 0) {
      value->elements = (book_t**)malloc(1*sizeof(book_t));
      value->n_elements = 0;
      return value;
    }

    // clock_t found = clock();
    // printf("Book found: %ld\n", found);

    size_t visited[count];
    size_t depth[count];
    char* visited_boolean = (char*)malloc(count*sizeof(char));
    value->elements = (book_t**)malloc(count*sizeof(book_t));

    size_t currentIndex = 0;
    visited[currentIndex] = index;
    depth[currentIndex] = 0;
    visited_boolean[currentIndex] = '1';
    value->elements[0] = &nodes[index];
    size_t size = 1;

  	// clock_t algo = clock();
  	// printf("Starting algorithm: %ld\n", algo);

    while (depth[currentIndex] < k) {
      if (currentIndex == size) {
        break;
      }
      book_t* book = &nodes[visited[currentIndex]];
      int add = 1;
      if (book->n_citation_edges != 0) {
        for (int i = 0; i < book->n_citation_edges; i++) {
          add = 1;
          if (visited_boolean[book->b_citation_edges[i]] == '1') {
            add = 0;
          }
          if (add == 1) {
            visited[size] = book->b_citation_edges[i];
            value->elements[size] = &nodes[book->b_citation_edges[i]];
            depth[size] = depth[currentIndex] + 1;
            visited_boolean[book->b_citation_edges[i]] = '1';
            size++;
          }
        }
      }
      currentIndex++;
    }

  	// clock_t fin = clock();
  	// printf("Finished algo: %ld\n", fin);

    value->n_elements = size;


    free(visited_boolean);


  	// clock_t finreal = clock();
  	// printf("Finished: %ld\n", finreal);

  	return value;
  } else {
    size_t index;

    int book_found = 0;

    for (int i = 0; i < count; i++) {
      if (nodes[i].id == book_id) {
        book_found = 1;
        index = i;
        break;
      }
    }

    if (book_found == 0) {
      result_t* value = (result_t*)malloc(1*sizeof(result_t));
      value->elements = (book_t**)malloc(1*sizeof(book_t));
      value->n_elements = 0;
      return value;
    }

    // clock_t found = clock();
    // printf("Book found: %ld\n", found);


    size_t visited[count];
    size_t depth[count];
    char* visited_boolean = (char*)malloc(count*sizeof(char));


    size_t currentIndex = 0;
    visited[currentIndex] = index;
    depth[currentIndex] = 0;
    visited_boolean[currentIndex] = '1';
    size_t size = 1;

    // clock_t algo = clock();
    // printf("Starting algorithm: %ld\n", algo);

    while (depth[currentIndex] < k) {
      if (currentIndex == size) {
        break;
      }
      book_t* book = &nodes[visited[currentIndex]];
      int add = 1;
      if (book->n_citation_edges != 0) {
        for (int i = 0; i < book->n_citation_edges; i++) {
          add = 1;
          if (visited_boolean[book->b_citation_edges[i]] == '1') {
            add = 0;
          }
          if (add == 1) {
            visited[size] = book->b_citation_edges[i];
            depth[size] = depth[currentIndex] + 1;
            visited_boolean[book->b_citation_edges[i]] = '1';
            size++;
          }
        }
      }
      currentIndex++;
    }

    // clock_t fin = clock();
    // printf("Finished algo: %ld\n", fin);

    result_t* value = (result_t*)malloc(1*sizeof(result_t));
    value->elements = (book_t**)malloc(size*sizeof(book_t));
    value->n_elements = size;

    for (int i = 0; i < size; i++) {
      value->elements[i] = &nodes[visited[i]];
    }

    free(visited_boolean);


    // clock_t finreal = clock();
    // printf("Finished: %ld\n", finreal);

    return value;
  }


}

// Returns result set containing books in shortest path between book 1 and 2.
result_t* find_shortest_distance(book_t* nodes, size_t count, size_t b1_id, size_t b2_id) {

// clock_t start = clock();
// printf("Starting shortest_dis: %ld\n", start);

  size_t index;

  int book_found = 0;

  for (int i = 0; i < count; i++) {
		if (nodes[i].id == b1_id) {
      book_found = 1; // set book_found to true
      index = i;
			break;
		}
	}

  if (book_found == 0) {
    result_t* value = (result_t*)malloc(1*sizeof(result_t));
    value->elements = (book_t**)malloc(1*sizeof(book_t));
    value->n_elements = 0;
    return value;
  }

// clock_t found = clock();
// printf("book found: %ld\n", found);

  if (b1_id == b2_id) {
    int ncount = 0;
    int indexes[2];
    for (int i = 0; i < count; i++) {
      if (nodes[i].id == b1_id) {
        indexes[ncount] = i;
        ncount++;
        if (ncount == 2) {
          break;
        }
      }
    }
    if (ncount == 1) {
      result_t* value = (result_t*)malloc(1*sizeof(result_t));
      value->elements = (book_t**)malloc(1*sizeof(book_t));
      value->elements[0] = &nodes[indexes[0]];
      value->n_elements = 1;
      return value;
    } else if (ncount > 2) {
      result_t* value = (result_t*)malloc(1*sizeof(result_t));
      value->elements = (book_t**)malloc(1*sizeof(book_t));
      value->elements[0] = &nodes[indexes[0]];
      value->elements[1] = &nodes[indexes[1]];
      value->n_elements = 2;
      return value;
    }
  }



  // clock_t checks = clock();
  // printf("Checks finished: %ld\n", checks);

  size_t visited[count];
  size_t origin[count];
  char* visited_boolean = (char*)malloc(count*sizeof(char));

  size_t currentIndex = 0;
  visited[currentIndex] = index;
  origin[currentIndex] = -1;
  visited_boolean[currentIndex] = '1';
  book_found = 0;
  size_t foundIndex = -1;
  size_t size = 1;

  // clock_t mal = clock();
  // printf("Stuff malloced: %ld\n", mal);

  while(1) {
    if (currentIndex == size) {
      book_found = 0;
      break;
    }
    book_t* book = &nodes[visited[currentIndex]];
    int add = 1;
    if (book->n_citation_edges != 0) {
      for (int i = 0; i < book->n_citation_edges; i++) {
        if (nodes[book->b_citation_edges[i]].id == b2_id) {
          visited[size] = book->b_citation_edges[i];
          origin[size] = currentIndex;
          book_found = 1,
          foundIndex = size;
          size++;
          break;
        }
        add = 1;
        if (visited_boolean[book->b_citation_edges[i]] == '1') {
          add = 0;
        }
        if (add == 1) {
          visited[size] = book->b_citation_edges[i];
          origin[size] = currentIndex;
          visited_boolean[book->b_citation_edges[i]] = '1';
          size++;
        }
      }
    }

    if (book_found == 1) {
      break;
    }

    if (book->n_author_edges != 0) {
      for (int i = 0; i < book->n_author_edges; i++) {
        if (nodes[book->b_author_edges[i]].id == b2_id) {
          visited[size] = book->b_author_edges[i];
          origin[size] = currentIndex;
          book_found = 1;
          foundIndex = size;
          size ++;
          break;
        }
        add = 1;
        if (visited_boolean[book->b_author_edges[i]] == '1') {
          add = 0;
        }
        if (add == 1) {
          visited_boolean[book->b_author_edges[i]] = '1';
          visited[size] = book->b_author_edges[i];
          origin[size] = currentIndex;
          size++;
        }
      }
    }

    if (book_found == 1) {
      break;
    }

    if (book->n_publisher_edges != 0) {
      for (int i = 0; i < book->n_publisher_edges; i++) {
        if (nodes[book->b_publisher_edges[i]].id == b2_id) {
          visited[size] = book->b_publisher_edges[i];
          origin[size] = currentIndex;
          book_found = 1;
          foundIndex = size;
          size ++;
          break;
        }
        add = 1;
        if (visited_boolean[book->b_publisher_edges[i]] == '1') {
          add = 0;
        }
        if (add == 1) {
          visited_boolean[book->b_publisher_edges[i]] = '1';
          visited[size] = book->b_publisher_edges[i];
          origin[size] = currentIndex;
          size++;
        }
      }
    }

    if (book_found == 1) {
      break;
    }

    currentIndex++;

  }

  // clock_t algo = clock();
  // printf("Algo finished: %ld\n", algo);

  result_t* value = (result_t*)malloc(1*sizeof(result_t));

  if (book_found == 0) {
    value->elements = (book_t**)malloc(1*sizeof(book_t));
    value->n_elements = 0;
    free(visited_boolean);
    return value;
  }

  currentIndex = foundIndex;
  value->n_elements = 0;
  //
  // clock_t build = clock();
  // printf("Start building: %ld\n", build);

  while (currentIndex != origin[0]) {
    currentIndex = origin[currentIndex];
    value->n_elements++;
  }

  value->elements = (book_t**)malloc(value->n_elements*sizeof(book_t));
  currentIndex = foundIndex;
  int i = 0;
  while (currentIndex != origin[0]) {
    value->elements[i] = &nodes[visited[currentIndex]];
    i++;
    currentIndex = origin[currentIndex];
  }

  free(visited_boolean);

  // clock_t fin = clock();
  // printf("Finished: %ld\n", fin);

  return value;

}
