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

typedef enum {
   TRUTHY,
   FALSY 
} assertion_t;

typedef enum {
    EQUAL,
    NOT_EQUAL,
} cmp_assertion_t;

typedef enum {
    ONE,
    MANY
} cardinality_t;

typedef enum {
    LET,
    CONST,
    VAR
} variable_scope_t;

#endif