# BWA177
This is my CS177 bioinformatics course project.

### preliminary
I used the library called libdivsufsort to support Larrson's suffix sort algorithm. The library is built by Yuta Mori. To run my code, ypu should first install the HSDS library with the following codes:

```
$ git clone https://github.com/y-256/libdivsufsort.git
$ cd libdivsufsort
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE="Release" \
-DCMAKE_INSTALL_PREFIX="/usr/local" ..
$ make
$ sudo make install
$ ldconfig
```

I used the library called HSDS to support fully indexable dictionary. The library is built by Hideaki Ohno. To run my code, ypu should first install the HSDS library with the following codes:

```
$ git clone https://github.com/hideo55/cpp-HSDS.git
$ cd cpp-HSDS
$ git submodule init
$ git submodule update
$ cmake .
$ make && make install
$ ldconfig
```

### build BWA177
```
$ cd BWA177
$ mkdir && cd build
$ cmake ..
$ make
```

### how to use
##### build index
```
./BWA177 index 1721  < ../sequence/80000.fa
```
```index``` means we are in index mode
```1721``` is the name we give, this name should be the same as the name we used in the align phase
```../sequence/80000.fa``` the path to the input reference file


##### do align
```
$ ./BWA177 align 1721  < ../sequence/demo_80000.fq 80000
```
```align``` means we are in align mode
```1721``` should be the same as the name you give to the reference at the index phase, we need this di find the pre-calculated information on disk
```../sequence/demo_80000.fq``` is the path to the short read input
```80000``` is the name of the reference file, we need this because we need to print the final result from this file
