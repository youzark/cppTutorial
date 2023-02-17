[source](https://www.acodersjourney.com/6-tips-supercharge-cpp-11-vector-performance/) 

#1 Avoid unnecessary reallocate and copy cycles by reserving the size of vector ahead of time.

#2 Use shrink_to_fit() to release memory consumed by the vector – clear() or erase() does not release memory.

#3 When filling up or copying from one vector into another vector, prefer assignment over insert() or push_back().

#4 While iterating through elements in a std::vector, avoid the std::vector::at() function.
