# Bookworm
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
  
  ``` struct result_t* find_book(struct book_node_t* nodes, size_t count, size_t id, uint16_t tcount); ```
  - Find all books by author
  
  ```struct result_t* find_all_books_by_author(struct book_node_t* nodes, size_t count, size_t author, uint16_t tcount);```

  - Find all books that have been reprinted under another publisher
  
  ```struct result_t* find_all_books_reprinted(struct book_node_t* nodes, size_t count, size_t publisher_id, uint16_t tcount);```

  - Find all books of k degree of separation of a book
  
  ```struct result_t* find_all_books_k_distance_from(struct book_node_t* nodes, size_t count, size_t book_id, uint16_t k, uint16_t tcount);```

  - Find the shortest distance between two books
  
  ```struct result_t* find_shortest_distance_between_books(struct book_node_t* nodes, size_t count, size_t a_id, size_t b_id, uint16_t tcount);```

  - Determine the shortest path between any book between author A and B using only a certain edge type
  
  ```struct result_t* find_shortest_edge_type(struct book_node_t* nodes, size_t count, size_t a_id, size_t b_id, uint16_t tcount);```

