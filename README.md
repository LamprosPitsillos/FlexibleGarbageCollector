# GarbageColectorCpp

## How to build :

```sh
make -j$(nproc)
```

## How to build for tests :

```sh
make test -j$(nproc)
cd Test
run_tests.sh build
run_tests.sh run
```

# FEATURES LIST
+ Mark Sweep Tracing
+ Reference counting
    + Instantly destruct
        + BFS
        + RDFS 
        + IDFS
    + Lately destruct
        + BFS
        + RDFS 
        + IDFS
    + Write pointer wrapper
+ Memory and time monitor (LOGGER)
    + Exports to csv
+ Benchmarks
    + Manual Memory Manager ( new, delete )
    + Test DataStructures
        + Array
        + List
        + Tree
        + TODO: graph

# TODO LIST
+ Add concurrency
    + Implement tricolor algorithm
    + Use Open MP
+ Provide collected primitive types
+ More coming soon...

## `gc_config.json` Syntax

+ `collection_type`
    + `Counting` 
    Reference counting implementation
        + `destruction`
            + `Late`
            + `Instant`
        + `graph_traversal`
            + `RDFS`
            + `IDFS`
            + `BFS`
    + `Tracing`
    Mark n Sweep implementation
        + `graph_traversal`
            + `RDFS`
            + `IDFS`
            + `BFS`

## Bugs:
+ When returning a GCPtr<> object from a funtion and disregarding it , the last value is always in the stack 
    making the GC think it's alive
    ```cpp
        make_garbage_tree(1);
        make_garbage_tree(10);
        make_garbage_tree(2);
        // 2^2 -1 objects thought to be alive
        GCPtr<Foo> x;
        // No it's not on the stack so the GC collects it...
    ```
+ While logging ,around 600 lines, we get a data corruption,probably due to the stream flashing while the thread still writes to it
