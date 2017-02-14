#include "cpu/exec/helper.h"

#define DATA_BYTE 2

#define instr cmova
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovae
#define flag (cpu.CF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovb
#define flag (cpu.CF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmove
#define flag (cpu.ZF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovg
#define flag ((cpu.OF==cpu.SF)&&(cpu.ZF==0))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovge
#define flag (cpu.OF==cpu.SF)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovl
#define flag (cpu.OF!=cpu.SF)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovle
#define flag ((cpu.OF!=cpu.SF)||(cpu.ZF==1))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovne
#define flag (cpu.ZF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovno
#define flag (cpu.OF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovnp
#define flag (cpu.PF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovns
#define flag (cpu.SF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovo
#define flag (cpu.OF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovp
#define flag (cpu.PF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovs
#define flag (cpu.SF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#undef DATA_BYTE

#define DATA_BYTE 4

#define instr cmova
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovae
#define flag (cpu.CF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovb
#define flag (cpu.CF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmove
#define flag (cpu.ZF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovg
#define flag ((cpu.OF==cpu.SF)&&(cpu.ZF==0))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovge
#define flag (cpu.OF==cpu.SF)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovl
#define flag (cpu.OF!=cpu.SF)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovle
#define flag ((cpu.OF!=cpu.SF)||(cpu.ZF==1))
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovne
#define flag (cpu.ZF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovno
#define flag (cpu.OF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovnp
#define flag (cpu.PF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovns
#define flag (cpu.SF==0)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovo
#define flag (cpu.OF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovp
#define flag (cpu.PF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#define instr cmovs
#define flag (cpu.SF==1)
#include "cmovcc-template.h"
#undef instr
#undef flag

#undef DATA_BYTE

make_helper_v(cmova_rm2r)
make_helper_v(cmovae_rm2r)
make_helper_v(cmovb_rm2r)
make_helper_v(cmovbe_rm2r)
make_helper_v(cmove_rm2r)
make_helper_v(cmovg_rm2r)
make_helper_v(cmovge_rm2r)
make_helper_v(cmovl_rm2r)
make_helper_v(cmovle_rm2r)
make_helper_v(cmovne_rm2r)
make_helper_v(cmovno_rm2r)
make_helper_v(cmovnp_rm2r)
make_helper_v(cmovns_rm2r)
make_helper_v(cmovo_rm2r)
make_helper_v(cmovp_rm2r)
make_helper_v(cmovs_rm2r)

