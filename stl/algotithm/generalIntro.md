# 105 STL Algorithms in Less then One Hour(Each...)

for_each is just a small part of STL.

## Permutations

### HEAPS(Heapify)

A data structure thats trees whos root is always larger(smaller) than it's children.

1. Can be represented with a vector

```cpp
std::make_heap(begin(numbers), end(numbers));
```

```cpp
numbers.push_back(aNumber);
std::push_heap(begin(numbers),end(numbers));    // Heapify
```

```cpp
std::pop_heap(begin(numbers),end(numbers));     // swap the root with the tail
numbers.pop_back();
```

### Sorting

1. sort
2. partial_sort
3. nth_element
4. sort_heap
5. inplace_merge


### Partitioning

> what's partition  
to put all the node that satisfy a predication at the front of the sequence.

> Border:(Partition Point)  
the node after the last point that satisfy the criteria.
![partitionBoarder](/home/youzark/projects/cppTutorial/stl/algotithm/img/partitionBoarder.png)

### Other Permutations

> rotate
put element at the end to the begin

> shuffle

> next_permutation & prev_permutation  

![allPermutation](/home/youzark/projects/cppTutorial/stl/algotithm/img/allPermutation.png)

> reverse

### stable_* is_* is_*_until

> stable_*

stable_sort, stable_partition

> is_*

is_sorted, is_partitioned, is_heap

> is_*_until

is_sorted_until, is_partitioned_until, is_heap_until


## Queries

### Numeric Algorithms

> count

> accumulate/(transform_)reduce

sum up all the element , transform_ allows to perform a given function first.

> partial_sum, (transform_)inclusive_scan, (transform_)exclusive_scan

> inner_product

> adjacent_difference

> Sample

![numericAlgorithm](/home/youzark/projects/cppTutorial/stl/algotithm/img/numericAlgorithm.png)

### Querying a property

> all_of

1. is all elements satisfy

> any_of

1. exist at least one element satisfy

> none_of

1. all not satisfy


### Querying a property on 2 ranges

![queryingTwo](/home/youzark/projects/cppTutorial/stl/algotithm/img/queryingTwo.png)


### Search a value

> Not sorted 
1. find()
2. adjacent_find()

> Sorted
1. equal_range
2. lower_bound(for insert)
3. upper_bound(for insert)
4. binary_search -> bool

### Search a range
1. search ( find subrange in a range )
2. find_end ( find subrange in a range starting from the end )
3. find_first_of ( iter to the frist elem in large range the equal one the the elem in the litte range )

### Searching for a relative value
> max_element -> iter

> min_element -> iter

> minmax_element -> pair<iter,iter>

## Algorithms on Set

> Set :
Any sorted collection , std::set is , sorted std::vector is also a set.

> std::set_difference:
return elem in A but not in B in linear time
```cpp
std::set_difference(begin(A),end(A),begin(B),begin(B),std::back_inserter(results))
```

> std::set_intersection

> std::set_union

> std::set_symmetric_difference

> std::includes

> std::merge


## Movers

> std::copy(first, last, out)

> std::move(first, last, out)

> std::swap_ranges(first, last, out)

> std::copy_backward

> std::move_backward

## Value Modifiers

> std::fill(first, last, value)

> std::generate(first, last, function)

> std::iota(first, last, startValue) 

*first = startValue , *(first + i) = startValue + i

> std::replace(first, last, oldValue ,newValue)


## Structure Changers

> std::remove(begin(collection), end(collection), 99) -> iter

change value in the collection rather than change the size and really remove something  
size the STL algorithms are only allowed to visit containers' iterator

collection.erase(std::remove(being(collection),end(collection),99),end(collection));  
May be wrap it up

> std::unique(being,end)  
remove adjacent duplicate  
and we have to erase them like in std::remove()


### *_copy

> remove_copy

> unique_copy

> reverse_copy

> rotate_copy

> replace_copy

> partition_copy

> partial_sort_copy


### *_if

> find_if

> find_if_not

> count_if

> remove_if

> remove_copy_if

> replace_if

> replace_copy_if

> copy_if


## Transform

> std::transform(begin(collection), end(collection), std::back_inserter(results), f)

> std::transform(begin(A),end(A),begin(B),end(B),f(a,b))

## For_each
std::for_each(begin(),end(),f); // doesn't care f's return value

for_each is designed to produce Side_effects


## Raw Memory

fill ,copy, move : all three of them use operator=

uninitialized_fill(first,end,value)  
uninitialized_move(first,end,value)  
uninitialized_copy(first,end,value)  

std::destory(first,end)

uninitialized_default_constructor()
uninitialized_value_constructor()


## _N

copy_n
fill_n
generate_n
search_n
for_each_n
uninitialized_move_n
...
destory_n



## 
replace for-loop with algorithms

