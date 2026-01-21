nasm -felf64 test.asm -o test.o
gcc test.o -o test
./test

echo $?
