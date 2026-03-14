# MIPS-EMU
Command-line MIPS emulator that executes an assembly or a binary file with optional register values logging

# Compiling The Emulator
This project is written in C++17 and is compiled using the gnu-g++ compiler.

Install gnu-g++
```bash
sudo apt install g++
```
Install make
```bash
sudo apt install make
```
then project can be compiled using
```bash
make
```

# Compiling an Example MIPS Code
Install mips-gnu-gcc, mips-gnu-g++
```bash
sudo apt-get install gcc-mips-linux-gnu g++-mips-linux-gnu
```
Install vim
```bash
sudo apt install vim
```
Create example's C file
```bash
vim test.c
```
Example code (Paste in test.c)
```c
int sum(int n1, int n2);

int main(int argc, char* argv[])
{
        volatile int a = 15;
        volatile int b = 3;
        volatile int c = sum(a, b);
        return 0;
}

int sum(int n1, int n2)
{
        return n1 + n2;
}
```
Cross-compile for MIPS
Create a non-linked, object file from "test.c"
```bash
mips-linux-gnu-gcc -c test.c -o test.o
```
Link this object file
```bash
mips-linux-gnu-gcc -nostdlib test.o -o test.elf
```
Dump the resultant object file into human readable format for reviewing
```bash
mips-linux-gnu-objdump -d test.elf
```

# Running MIPS ".elf" File On The Emulator
```bash
./main.exe test.elf log.txt -v
```
