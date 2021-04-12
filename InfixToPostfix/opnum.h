#ifndef OPNUM_H 
#define OPNUM_H

#define register

#define SYM_NULL   0x0
#define SYM_NAME   0x1
#define SYM_INTEG  0x2
#define SYM_FLOAT  0x3
#define END_LINE   0x11


/* operators Bit 2 is on */
#define SYM_ADD   0x4 
#define SYM_SUB   0x5 
#define SYM_MUL   0x6
#define SYM_DIV   0x7

/* operand Bit 3 is on */
#define SYM_OPEN   0x8
#define SYM_CLOSE  0x9

#define SYM_INVAL  0x10

extern void set_input(int argc, char ** argv);
extern char * get_opnum(int *val);
#endif
