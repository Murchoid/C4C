
native "C":
    fn putchar(i32 c)->i32
    fn exit(i32 status)->i32
:

fn add(i32 x,i32 y)->i32:
    return x + y
:

i32 character = 0

pub fn main()->i32:
    i32 character = 65
    while character <= 120:
        putchar(character)
        character = character + 1
    :
   
    exit(200)
    putchar(10)

    exit(2)
    return 10
:
