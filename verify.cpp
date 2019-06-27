#include <cassert>

void assume (bool e) { while (!e) ;  } 
int nondet_int () { int x; return x; } // generate non-deterministic int

#include "circular_buffer.h"

void pushOneMore(int *values, size_t size, size_t capacity) {
    
    // Create the circular buffer
    CircularBuffer<int, true> cb(capacity);
    
    // Populates the buffer with values
    for (size_t i = 0; i < size; ++i) {
        cb.push(values[i]);
    }
    
    // get the size before inserting
    size_t sbefore = cb.getSize();
    cb.push(nondet_int());
    // get the size after inserting
    size_t safter  = cb.getSize();
    
    // Assertion: if we populate with less elements than capacity
    //            the size after must be the size before plus one
    if (capacity > size) assert(sbefore + 1 == safter);
    
    // Assertion: if we populate with #capacity elements or more
    //            the size after must be equal to the size before
    else assert(sbefore == safter);
}

void popOneMore(int *values, size_t size, size_t capacity) {
    
    // Create the circular buffer
    CircularBuffer<int, true> cb(capacity);
    
    // Populates the buffer with values
    for (size_t i = 0; i < size; ++i) {
        cb.push(values[i]);
    }
    
    // get the size before removing
    size_t sbefore = cb.getSize();
    int val = cb.pop();
    // get the size after inserting
    size_t safter  = cb.getSize();
    
    // Assertion: if the number of elements populated is more than 0    
    if (size > 0) {
        // the size after must be the size before minus one
        assert(sbefore - 1 == safter);
        
        // if we did not exceed capacity
        if (capacity >= size) {
            // the popped element must be equal to the first one pushed
            assert(val == values[0]);
        } else {
            // the popped element is the element that stayed at the
            // begining of the circular buffer after all inserts
            assert(val == values[size - capacity]);
        }
    } else if (size == 0) {
        assert(sbefore == 0 && safter == 0);
    }
}


void testAll(size_t size, size_t capacity) {
    
    assume(capacity > 0);
    
    int values[size];
    
    for(size_t i = 0; i < size; ++i) 
        values[i] = nondet_int();
    
    pushOneMore(values, size, capacity);
    popOneMore (values, size, capacity);
}
