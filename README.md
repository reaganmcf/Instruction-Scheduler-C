The directory "benchmarks" contains the iloc benchmark programs that you need to use for your report. The subdirectory "additional-tests" contain tests that are not public but will be used for testing the functionality of your scheduler in addition to the provided benchmark programs.

The directory "C" contains a basic infrastructure for reading in and printing out ILOC instructions. You are not required to use this code.


### Todo

- [x] Register dependencies
- [x] Memory dependencies
- [ ] Register Anti Dependencies (NEED LOAD AND STORE)
- [ ] Memory Anti Dependencies (NEED ALL)
- [ ] Heap algorithm for active / ready set
- [ ] Longest latency weighted path heuristic scheduler
- [ ] Highest latency instruction heuristic scheduler
- [ ] My own heuristic (unknown)

## Development

```console
$ nodemon -e .c -x "make clean && make && ./schedule -a < ./benchmarks/simple.iloc"
```
