/****************************************************************************
 Copyright (c) 2019 Alair Dias Junior

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*****************************************************************************/
 
#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#ifndef CBMC_CHECKING
#include <stdexcept>
#endif
#include <cstddef>

template <class T, bool OVERWRITE>
class CircularBuffer {
    private:
        T* _buffer;
        size_t _capacity;
        size_t _start;
        size_t _peek_start;
        size_t _peeked;
        size_t _end;
        size_t _size;
        bool _full;
        bool _peek_empty;
    
        void incEnd() {
            if (++_end == _capacity) _end = 0;
        }
        
        void incStart() {
            if (_start == _peek_start && !_peek_empty)
                incPeekStart();
            
            if (++_start == _capacity) _start = 0;
            
        }
    
        void incPeekStart() {
            if (++_peek_start == _capacity) _peek_start = 0;
        }
    
    
        bool peekedData() {
            return (_peek_start != _start) || (isFull() && _peek_empty);
        }
    public:
        explicit CircularBuffer(size_t capacity) {
#ifndef CBMC_CHECKING
            if (capacity < 1) throw std::length_error("Invalid Capacity");
#endif
            _buffer = new T[capacity];
            
            _capacity = capacity;
            clear();
        }
        
        ~CircularBuffer() { if (_buffer) delete[] _buffer; }
        
        bool isFull() {
            return _full;
        }
        
        bool isEmpty() {
            return _start == _end && !_full;
        }
        
        bool canPeek() {
            return !isEmpty() && !_peek_empty;
        }
               
        void push(T item) {
            
            if (!OVERWRITE && isFull()) return;
            
            _buffer[_end] = item;
            incEnd();
            if (isFull()) {
                incStart();
            } else {
                ++_size;
                if (_end == _start) _full = true;
            }
            
            _peek_empty = false;
        }
    
        T pop() {
#ifndef CBMC_CHECKING
            if (isEmpty()) throw std::out_of_range("Reading from an Empty Buffer");
#else
            T dummy;
            if (isEmpty()) return dummy;
#endif
            if (_full) _full = false;
            
            T item = _buffer[_start];
            incStart();
            
            if (_start == _end) _peek_empty = true;
            
            --_size;
            return item;
        }
        
        
        T peek() {
#ifndef CBMC_CHECKING
            if (!canPeek()) throw std::out_of_range("Cannot peek this buffer");
#else
            T dummy;
            if (isEmpty()) return dummy;
#endif
            
            T item = _buffer[_peek_start];
            incPeekStart();
            if (_end == _peek_start) _peek_empty = true;
            
            _peeked++;

            return item;
        }
        
        void resetPeekPointer() {
            _peek_start = _start;
            _peek_empty = isEmpty();
            _peeked = 0;
        }       
        
        void removePeeked() {
            if (peekedData()) {
                _start = _peek_start;
                _full = false;
                _size -= _peeked;
                _peeked = 0;
            }
            
        }
        
        
        size_t getCapacity() {
            return _capacity;
        }
        
        size_t getSize() {
            return _size;
        }
        
        void clear() {
            _start = 0;
            _end = 0;
            _full = false;
            _peek_empty = true;
            _peek_start = 0;
            _peeked = 0;
            _size = 0;
        }
};


#endif // __CIRCULAR_BUFFER_H__
