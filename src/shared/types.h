#ifndef TYPES_H
#define TYPES_H

typedef enum {
    KEY_CONTROL = 0,
    KEY_SHIFT,
    KEY_ALT,

    KEY_LEFT,
    KEY_RIGHT,  
    KEY_UP,
    KEY_DOWN,

    KEY_ENTER,
    KEY_SPACE,  
    KEY_BACKSPACE,
    KEY_DELETE,     
    KEY_TAB,    
    KEY_ESCAPE,
    KEY_PAGE_UP,       
    KEY_PAGE_DOWN,
    KEY_END,            
    KEY_HOME
} action_key_t;

typedef enum {
   TRUTHY,
   FALSY,
   EQUAL,
   NOT_EQUAL,
} assertion_t;

typedef enum {
    ONE,
    MANY
} cardinality_t;

typedef enum {
    LET,
    CONST,
    VAR
} variable_scope_t;

typedef enum {
    NAN_VALUE,
    UNDEFINED_VALUE,
    NULL_VALUE
} null_literal_t;
#endif