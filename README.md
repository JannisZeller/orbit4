# orbit4

A little C++ based and python plotted program which is dedicated to simulate some (non relativistic) orbits with some relatively basic numeric approaches. It is called orbit4 because of ancient local versions before I decided to set up a repo.

It is straightforward to setup different systems with changes to the `main.cpp` file and build from source. The results show (at least as far as I can tell) that the simulation works. However, the simulation of the Earth-Moon-Sun systems needs really small step sizes to yield stable results and still the moon-orbit seems "wiggly". This is surprising especially when I compare it with my more recent fun project ([stellarflow](https://github.com/JannisZeller/stellarflow)) where I use a fully vectorized implementation for simulating equivalent orbits using `tensorflow`. In the latter implementation comparably huge step sizes yielded good results.


### Note for Compilation
There is still (2022-15-12) something going on with the vector standard and compilation within VSCode (see e. g. [here](https://stackoverflow.com/q/72737601) and [here](https://stackoverflow.com/q/697738779)). With VSCodes standard C++-Setup the default build resulted in the following situation:
- The build worked, and the execution of the produced .exe-file via the VSCode-Debugger inside the VSCode terminal worked as well, i. e. produced output in the terminal, but no `<fstream>` worked.
- The execution of the produced .exe-file from another shell (instance) resulted in no error, but also no output or anything, just a quit nothing.
- Trying to run the produced .exe-file via double-click resulted in a pop-up with the cryptic error message 
    ```text
    _ZSt28__throw_bad_array_new_length could no be located in the dynamic link library
    ```
    again no output or anything of course.

As suggested in both threads linked above I managed to fix the problem by adding `"-static-libstdc++"` to the compiler arguments. This helped me on my machine running msys2 with mingw64 with g++ 12.2.0 and gdb 12.1.