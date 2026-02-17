/*
enum OpenFlag:
    O_RDONLY    = 0      // open for reading only
    O_WRONLY    = 1      // open for writing only
    O_RDWR      = 2      // open for reading and writing
    O_CREAT     = 64     // create file if it does not exist
    O_EXCL      = 128    // error if O_CREAT and the file exists
    O_NOCTTY    = 256    // do not assign controlling terminal
    O_TRUNC     = 512    // truncate file to zero length
    O_APPEND    = 1024   // append on each write
    O_NONBLOCK  = 2048   // non-blocking mode
    O_DSYNC     = 4096   // synchronous I/O data integrity
    O_SYNC      = 1052672 // synchronous I/O file integrity
    O_RSYNC     = 1052672 // synchronous reads
    O_DIRECTORY = 65536   // fail if not a directory
    O_NOFOLLOW  = 131072  // do not follow symbolic links
    O_CLOEXEC   = 524288  // set close-on-exec
:



native "C":
    fn open(char *name,i32 flags,i32 mode)->i32
    fn creat(char *name,i32 mode)->i32
    fn read(i32 fd,char *buf,u64 buflen)->i64
    fn write(i32 fd,char *buf,u64 buflen)->i64
    fn close(i32 fd)->i32
    fn puts(char *str)->i32
    fn malloc(u64 size)->void *
    fn free(void *ptr)->void
:


union Data:
    i32 fd
    i64 ret
:

fn main()->i32:
    i32 fd = open("test.rs",OpenFlag.O_RDONLY,0)
    char *buf = malloc(1000)
    read(fd,buf,1000)
    //puts(buf)
    free(buf)

    switch fd:
        case 0:
            puts("0")
        :
        case 1:
            puts("1")
        :
        case 2:
            puts("2")
        :
        case 3:
            puts("3")
        :
        default:
            puts("default")
        :
    :

    return 0
:

*/

/*

struct Color:
    i32 red
    i32 green
    i32 blue
:

impl Color:
    fn huh()->i32:
        self->red = 0
        return 0
    :

    fn newh(i32 red,i32 green,i32 blue)->void:
        self->red = red
        self->green = green
        self->blue = blue
    :

    fn get_red()->i32:
        return self->red
    :
:


struct Pixel:
    Color color
:

impl Pixel:
    fn get_red()->i32:
        self->color.red
    :
:

fn main()->i32:
    i32 age = 90
    i32 *ptr1 = &age
    i32 **ptr2 = &ptr1

    Color color = Color:
        .red = 90,
        .green = 89,
        .blue = 78,
    :

    //color.huh()

    Pixel pixel = Pixel:
        .color = color,
    :

    //return pixel.color.get_red()
:


*/


pub fn main()->i32:
    i32 a = 90
    i32 *b = &a
    return b@write(78) + 7 + b@write(50)
:
