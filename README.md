# Bookworm
A University of Sydney COMP2129 C Assignment.

A program that performs queries on a graph of books.
The books themselves are represented as an array of book_node_t and contain categorised edges linking to other books. 
The assignment had a leaderboard which timed our assignents, and ranked us against our peers. 

Book Structure:
```
struct book_node_t {
  uint32_t id;
  uint32_t publisher_id;
  uint32_t author_id;
  uint32_t n_publisher_size;
  uint32_t n_author_size;
  uint32_t n_cited_books;
  uint32_t* related_publisher_books;
  uint32_t* same_author_books;
  uint32_t* cited_by;
};
```

Queries return result in structure:
```
struct result_t {
  struct book_node_t** elements;
  size_t n_elements;
};
```

Queries:
  - Find a book
  - Find all books by author
  - Find all books that have been reprinted under another publisher
  - Find all books of k degree of separation of a book
  - Find the shortest distance between two books
  - Determine the shortest path between any book between author A and B using only a certain edge type
