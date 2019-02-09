#ifndef WORM_H
#define WORM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>


typedef struct book_t {
	size_t id;
	size_t author_id;
	size_t publisher_id;
	size_t* b_author_edges;
	size_t* b_citation_edges;
	size_t* b_publisher_edges;
	size_t n_author_edges;
	size_t n_citation_edges;
	size_t n_publisher_edges;
} book_t;

typedef struct result_t {
	book_t** elements;
	size_t n_elements;
} result_t;

typedef struct shortest_t {
	int book_found;
	size_t* visited;
	size_t* origin;
	size_t foundIndex;
	size_t size;
} shortest_t;

void *find_thread_worker(void *arg);
void *author_thread_worker(void *arg);
void *reprinted_worker(void *arg);
void *shortest_worker(void *arg);
void *book_multiple_worker(void *arg);

result_t* find_book(book_t* nodes, size_t count, size_t book_id);
result_t* find_books_by_author(book_t* nodes, size_t count, size_t author_id);
result_t* find_books_reprinted(book_t* nodes, size_t count, size_t publisher_id);
result_t* find_books_k_distance(book_t* nodes, size_t count, size_t book_id, uint16_t k);
result_t* find_shortest_distance(book_t* nodes, size_t count, size_t b1_id, size_t b2_id);
result_t* find_shortest_edge_type(book_t* nodes, size_t count, size_t a1_id, size_t a2_id);
result_t* find_book_multiple(book_t* nodes, size_t count, size_t book_id);


shortest_t find_shortest_recursive(size_t* visited, size_t* origin, size_t count, size_t currentIndex, size_t size, size_t goal_id, book_t* nodes, char* visited_boolean);

size_t* recursive_k(book_t* book, book_t* nodes, size_t depth, size_t k, size_t checked_indexes[]);

#endif
