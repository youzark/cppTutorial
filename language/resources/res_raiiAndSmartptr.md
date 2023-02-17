[resource Post](https://stackoverflow.com/questions/395123/raii-and-smart-pointers-in-c) 

## problem description
In practice with C++, what is RAII, what are smart pointers, how are these implemented in a program and what are the benefits of using RAII with smart pointers?

## Answer

### Tradition way , Java try syntax & C++ RAII in resource management

#### tradition ways:

we must make sure that we close the file once we've finished with it. 

```cpp
File file("/path/to/file");
// Do stuff with file
file.close();
```

DrawBacks:
- wherever we use File, we will have to called File::close() - if we forget to do this, we're holding onto the file longer than we need to.
- what if an exception is thrown before we close the file?

#### Java way
GC only deal with memory management , when we need to address other types of resource , try block is need in Java

With finally clause:
```java
try {
    File file = new File("/path/to/file");
    // Do stuff with file
} finally {
    file.close();
}
```

With try_with_resource
```java
try (File file = new File("/path/to/file")) {
   // Do stuff with file
}
```

#### RAII
```cpp
File file("/path/to/file");
// Do stuff with file
// No need to close it - destructor will do that for us
```

### When dose Smart pointer kick in

#### problem setup

Open a file as Logger file and set logger for certain objects

#### why not raii
```cpp
void setLog(const Foo & foo, const Bar & bar) {
    File file("/path/to/file", File::append);
    foo.setLogFile(file);
    bar.setLogFile(file);
}
```

resource allocated with RAII shares lifetime with function , there are times when we need
resources that lives longer.

#### why not raw Pointer?
```cpp
void setLog(const Foo & foo, const Bar & bar) {
    File* file = new File("/path/to/file", File::append);
    foo.setLogFile(file);
    bar.setLogFile(file);
}
```
Answer to this questions can be simple.

No one is managing both memory and file handle in this case. (Raw pointer)

#### smart Pointer trick
```cpp
void setLog(const Foo & foo, const Bar & bar) {
    shared_ptr<File> file = new File("/path/to/file", File::append);
    foo.setLogFile(file);
    bar.setLogFile(file);
}
```
When we need a resource that lives out the function, we need smart pointer to 
manage it!
