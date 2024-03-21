#include <cstdint>

class Ring_buffer {  
  #define CAPACITY 16

public: 
   Ring_buffer();
   uint8_t get();
   void put(uint8_t);
   bool empty();
   bool not_full();

private: 
  uint8_t wr_idx;
  uint8_t rd_idx;
  uint8_t size;
  uint8_t capacity{CAPACITY};
  uint8_t arr[CAPACITY];
};
