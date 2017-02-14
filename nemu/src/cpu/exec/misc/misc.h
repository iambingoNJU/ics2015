#ifndef __MISC_H__
#define __MISC_H__

make_helper(nop);
make_helper(hlt);
make_helper(int3);
make_helper(lea);
make_helper(ct_v);
make_helper(cld);
make_helper(std);
make_helper(lgidt_v);
make_helper(mov_cr2r);
make_helper(mov_r2cr);
make_helper(ljmp_l);
make_helper(mov_seg_w);
make_helper(int_n);
make_helper(iret);
make_helper(cli);
make_helper(sti);
make_helper(pusha_l);
make_helper(popa_l);
make_helper(in_i2a_b);
make_helper(in_i2a_v);
make_helper(in_d2a_b);
make_helper(in_d2a_v);
make_helper(out_a2i_b);
make_helper(out_a2i_v);
make_helper(out_a2d_b);
make_helper(out_a2d_v);

#endif
