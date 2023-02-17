# Constant


# Literal
## Storage
[source post](https://stackoverflow.com/questions/43454489/storage-of-literal-constants-in-c#:~:text=Although%20the%20C%20and%20C%2B%2B,section%20and%20a%20data%20section.) 

Although the C and C++ standards don't dictate where the literals are stored, common practice stores them in one of two places: in the code (see @NeilButterworth answer), or in a "constants" segment.

Common executable files have a code section and a data section. The data segment may be split up into read-only, uninitialized read/write and initialized read-write. Often, the literals are placed into the read-only section of the executable.

In summary, the placement of literals is implementation dependent. The C and C++ standards state that writing to the location of literals is undefined behavior. Preferred practice with character literals is to declare the variable as const so compiler can generate warnings or errors when a write to a literal occurs.
