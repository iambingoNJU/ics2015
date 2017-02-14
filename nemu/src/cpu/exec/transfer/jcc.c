#include "cpu/exec/helper.h"


#define DATA_BYTE 1

#define instr ja
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jae
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jb
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jc
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag
*//*
#define instr jcxz
#define flag (cpu.gpr[R_ECX]._16==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jecxz
#define flag (cpu.gpr[R_ECX]._32==0)
#include "jcc-template.h"
#undef instr
#undef flag
*//*
#define instr je
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jz
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jg
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jge
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jl
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jle
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jna
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnae
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnb
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnbe
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnc
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jne
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jng
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnge
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnl
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnle
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jno
#define flag (cpu.OF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnp
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jns
#define flag (cpu.SF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jnz
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jo
#define flag (cpu.OF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jp
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jpe
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jpo
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr js
#define flag (cpu.SF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#undef DATA_BYTE


#define DATA_BYTE 2


#define instr ja
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jae
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jb
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jc
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr je
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jz
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jg
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jge
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jl
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jle
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jna
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnae
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnb
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnbe
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnc
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jne
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jng
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnge
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnl
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnle
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jno
#define flag (cpu.OF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnp
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jns
#define flag (cpu.SF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jnz
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jo
#define flag (cpu.OF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jp
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jpe
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jpo
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr js
#define flag (cpu.SF==1)
#include "jcc-template.h"
#undef instr
#undef flag


#undef DATA_BYTE


#define DATA_BYTE 4


#define instr ja
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jae
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jb
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jbe
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jc
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr je
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jz
#define flag (cpu.ZF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jg
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jge
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jl
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jle
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jna
#define flag ((cpu.CF==1)||(cpu.ZF==1))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnae
#define flag (cpu.CF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnb
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnbe
#define flag ((cpu.CF==0)&&(cpu.ZF==0))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnc
#define flag (cpu.CF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jne
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jng
#define flag ((cpu.ZF==1)||(cpu.SF!=cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnge
#define flag (cpu.SF!=cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnl
#define flag (cpu.SF==cpu.OF)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnle
#define flag ((cpu.ZF==0)&&(cpu.SF==cpu.OF))
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jno
#define flag (cpu.OF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jnp
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jns
#define flag (cpu.SF==0)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jnz
#define flag (cpu.ZF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr jo
#define flag (cpu.OF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jp
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag
/*
#define instr jpe
#define flag (cpu.PF==1)
#include "jcc-template.h"
#undef instr
#undef flag

#define instr jpo
#define flag (cpu.PF==0)
#include "jcc-template.h"
#undef instr
#undef flag
*/
#define instr js
#define flag (cpu.SF==1)
#include "jcc-template.h"
#undef instr
#undef flag


#undef DATA_BYTE

make_helper_v(ja_si)
make_helper_v(jae_si)
make_helper_v(jb_si)
make_helper_v(jbe_si)
//make_helper_v(jc_si)
//make_helper_v(je_si)
make_helper_v(jz_si)
make_helper_v(jg_si)
make_helper_v(jge_si)
make_helper_v(jl_si)
make_helper_v(jle_si)
//make_helper_v(jna_si)
//make_helper_v(jnae_si)
//make_helper_v(jnb_si)
//make_helper_v(jnbe_si)
//make_helper_v(jnc_si)
make_helper_v(jne_si)
//make_helper_v(jng_si)
//make_helper_v(jnge_si)
//make_helper_v(jnl_si)
//make_helper_v(jnle_si)
make_helper_v(jno_si)
make_helper_v(jnp_si)
make_helper_v(jns_si)
//make_helper_v(jnz_si)
make_helper_v(jo_si)
make_helper_v(jp_si)
//make_helper_v(jpe_si)
//make_helper_v(jpo_si)
make_helper_v(js_si)

