#include "cpu/exec/helper.h"


#define DATA_BYTE 1

#define instr seta
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setae
#define flag (cpu.CF==0)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setb
#define flag (cpu.CF==1)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "setcc-template.h"
#undef instr
#undef flag

#define instr sete
#define flag (cpu.ZF==1)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setg
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setge
#define flag (cpu.SF==cpu.OF)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setl
#define flag (cpu.SF!=cpu.OF)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setle
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setne
#define flag (cpu.ZF==0)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setno
#define flag (cpu.OF==0)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setnp
#define flag (cpu.PF==0)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setns
#define flag (cpu.SF==0)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr seto
#define flag (cpu.OF==1)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr setp
#define flag (cpu.PF==1)
#include "setcc-template.h"
#undef instr
#undef flag

#define instr sets
#define flag (cpu.SF==1)
#include "setcc-template.h"
#undef instr
#undef flag

#undef DATA_BYTE
