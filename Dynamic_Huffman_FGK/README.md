# Dynamic Huffman FGK Algorithm Implementation

## Introduction

Based on Jeferey Scott Vitter's paper (Vitter, 1987) "Design and Analysis of Dynamic Huffman Codes" and UNSW COMP9319 lecture ppt, I finish both the compression and decompression codes for the FGK algorithm. 

This algorithm is a one-pass huffman coding, meaning it only needs to read the input file for one time only. The static huffman coding requires reading the file for two times, one for generate the occ table and the tree, the other one to code the file. However, the FGK algorithm can generate the tree and codes 'one the fly'.

The main idea is to use an NYT node, abbreviated for "Not Yet Transmitted". Each time when the file inputs a new char, the NYT node breaks down to two nodes, the left child being the new NYT node and the right child being the node for the char, with occ = 1. Every time an existed char is input again, before increasing the occ, the algorithm first looks for the nodes with the same occ, and exchange the node with the node with maximum label number. The label number starts from 256, and decreases in a top-down, right-left order. Details for the algorithm can be found in Vitter's paper. 

The FGK version is a bit simpler than the Vitter's version in terms of the swap and update procedure. Vitter's version is more complex, since its aim is to achieve a more balanced binary tree. As a result, it adds a step called 'slide and increment'. 

## Code Structure

The code has the following structure:

```
FGK.c
    FGK_functions.h FGK_functions.c
        tree.c tree.h
```

Usage of the code:

```
// please put test files in the same folder level as the FGK programme
// otherwise the filename will have some errors. 

Usage: ./FGK <-compress|-decompress> <file>
```

Steps to run the file: (Assume we have a file called test1) 

```
make
./FGK -compress test1           // generate a file with .FGK suffix
./FGK -decompress test1.FGK     // generate a file with .FGK suffix removed and add deFGK_ prefix
diff deFGK_test1 test1          // compare the decompressed file and the original file
```

## Pesudo EOF

One issue with any compression method is how to deal with the end EOF sign. Since the compressed file may not have exact number of bytes, maybe just 3 bits left or 7 bits left, and we need to pad it to 8 bits in order to print out. I used the following structure to organize the body of the compressed file. Please see the example:

| First byte | Body of file|
|------------|----------------|
| 0000 0011 | compressed file|

The first byte of the compressed file indicates number of bits padded at the end. Since 0000 0011 in binary represents 3 in decimal, so 3 bits are padded at the end. And the following example shows 4 bits padded at the end. 

| First byte | Body of file|
|------------|----------------|
| 0000 0100 | compressed file|

And during decompression, the program reads the first byte to notice how many bits are padded at the end. Then the main paragraph starts from the second byte.

## Reference

Vitter, J., 1987. Design and analysis of dynamic Huffman codes. Journal of the ACM (JACM), 34(4), pp.825-845.