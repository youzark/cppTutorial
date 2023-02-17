# Const And Constexpr
[source vid](https://www.youtube.com/watch?v=tA6LbPyYdco) 

## Constness

1. Flavours of constness
    1. const
    2. const_cast
    3. const_expr
    4. consteval
    5. constinit
    6. is_constant_evaluated

2. differences:
    1. Function execution
    2. Variable initialization


## const

> *const* is a quality attribute of you program

### const objects

1. must be initialized
2. cannot be modified
3. cannot be victims of data races
4. can only invoke const memeber functions
5. const member functions cannot change the object

### physical & logical constness

[source post](https://stackoverflow.com/questions/3830367/difference-between-logical-and-physical-const-ness) 

