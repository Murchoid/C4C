
pub fn main()->i32:
    i32 age = 30

    if age < 18:
        return 1
    :
    elif age < 23:
        return 2
    :
    elif age <= 25:
        return 3
    :
    else:
        i32 counter = 0
        while 1:
            if counter < 100:
                counter = counter + 1
                continue
            :

            return counter
        :
    :
:
