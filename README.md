# CandyKids

Candy factories generate candy to a bounded buffer which stores candy while kids eat candy from the bounded buffer.

Candykids program uses the producer-consumer solution to manage access to a bounded buffer storing candy. One group of threads will model candy factories which generate candy one at a time and insert the candy into the bounded buffer. Another group of threads will model kids which eat candy one a time from the bounded buffer.

## Install

Install the repository.

```shell
$ make
$ ./candykids
```

## Usage

Candykids accept three arguments:

```shell
./candykids <#factories> <#kids> <#seconds>

Example:
$ ./candykids 5 2 8
# Factories: Number of candy-factory threads to spawn.
# Kids: Number of kid threads to spawn.
# Seconds: Number of seconds to allow the factory thread to run for.
```

### Main Function

1. Extract arguments: Process the arguments passed on the command line.
2. Initialize modules: Do any module initialization.
3. Launch factory threads: Spawn the requested number of candy-factory threads.
4. Launch kid threads: Spawn the requested number of kid threads.
5. Wait for requested time: Call sleep().
6. Stop factory threads: Indicate to the factory threads that they are to finish, and then call join for each factory thread.
7. Wait until no more candy
8. Stop kid threads: For each kid thread, cancel the thread and then join the thread.
9. Print statistics
10. Cleanup any allocated memory

### Stats

```shell
$ ./candykids 8 3 7
Factory 0 ships candy & waits 3s
Factory 1 ships candy & waits 3s
Factory 3 ships candy & waits 3s
Factory 2 ships candy & waits 3s
Factory 4 ships candy & waits 3s
Factory 5 ships candy & waits 3s
Factory 6 ships candy & waits 3s
Factory 7 ships candy & waits 3s
Factory 7 ships candy & waits 2s
Factory 1 ships candy & waits 2s
Factory 4 ships candy & waits 2s
Factory 6 ships candy & waits 2s
Factory 3 ships candy & waits 2s
Factory 2 ships candy & waits 3s
Factory 0 ships candy & waits 2s
Factory 5 ships candy & waits 3s
Factory 0 ships candy & waits 1s
Factory 6 ships candy & waits 2s
Factory 1 ships candy & waits 1s
Factory 3 ships candy & waits 1s
Factory 7 ships candy & waits 1s
Factory 4 ships candy & waits 1s
Factory 1 ships candy & waits 3s
Factory 2 ships candy & waits 3s
Factory 0 ships candy & waits 3s
Factory 5 ships candy & waits 3s
Factory 4 ships candy & waits 3s
Factory 3 ships candy & waits 3s
Factory 7 ships candy & waits 3s
Candy-factory 6 done.
Candy-factory 0 done.
Candy-factory 5 done.
Candy-factory 2 done.
Candy-factory 1 done.
Candy-factory 7 done.
Candy-factory 4 done.
Candy-factory 3 done.
Statistics:
Factory#   #Made  #Eaten  Min Delay[ms]  Avg Delay[ms]  Max Delay[ms]
       0       4       4        0.38086      376.23212      752.57422
       1       4       4        0.36255        0.81342        1.09497
       2       3       3        0.33594      501.37191      752.58398
       3       4       4        0.34790      376.09711      752.59912
       4       4       4        0.27734      376.06390      752.61938
       5       3       3        0.23364      501.28548      752.56567
       6       3       3        0.21777      251.24194      752.60815
       7       4       4        0.13794      188.12555      750.63379
```





