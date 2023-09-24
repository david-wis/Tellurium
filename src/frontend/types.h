#ifndef TYPES_H
#define TYPES_H

typedef enum {
    CONTROL,
    SHIFT,
    ALT,

    LEFT,
    RIGHT,  
    UP,
    DOWN,

    ENTER,
    SPACE,  
    BACKSPACE,
    DELETE,     
    TAB,    
    ESCAPE,
    PAGE_UP,       
    PAGE_DOWN,
    END,            
    HOME
} actionkey_t;

typedef enum {
    FALSE,
    TRUE
} bool_t;

#endif