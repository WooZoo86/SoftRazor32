#pragma once

#include <stdint.h>
#include <Windows.h>
#include <tchar.h>

#include "vbstruct.h"

/*
Reference From Semi VB Decompiler
Thank: vbgamer45
http://www.vbforums.com/member.php?51546-vbgamer45
*/

/* pcode decode error code */
#define PCEC_DECODEOVER       0x00000001U       //解码完成
#define PCEC_NOTALIGNED       0x00000002U       //变参长度未2字节对齐
#define PCEC_OVERFLOW         0x80000004U       //指令长度超过最大值 (溢出)
#define PCEC_FIXEDERR         0x80000008U       //固定长度错误


/* pcode type0 br:ll */
#define PCT0_STD                      1
#define PCT0_IDX                      2
#define PCT0_CLEARMEMORY              3
#define PCT0_ONESTRING                4
#define PCT0_RETONEVAR                5
#define PCT0_ADDARGTOATACK            6
#define PCT0_NONE                     99

/* pcode type1 br:lh */
#define PCFLH_DBG                     0x0100          //?调试指令?

/* pcode flag br:h */
#define PCFH_ENDPROC                  0x00010000      //指令流程截断
#define PCFH_BRANCH                   0x00020000      //分支,跳转
#define PCFH_CONDBRAN                 0x00040000      //条件分支
#define PCFH_WARNING                  0x00080000      //警告
#define PCFH_INVALID                  0x00100000      //无效指令

/* pcode decode flag */
#define PCDF_INCREMENTCOUNT           0x00000001      //增加指令指针

/* pcode decode exception code */
#define PCDEC_INVALIDFORMATCHAR       1               //无效格式字符
#define PCDEC_REPEATVPLEN             2               //重复变参长度
#define PCDEC_VPTOOLONG               3               //变参太长

/* pcode decode exception result code */
#define PCDER_CONTINUE                -1              //继续
#define PCDER_FAILED                  0               //失败
#define PCDER_RESULT1                 1               //结果1
#define PCDER_RESULT2                 2               //结果2

/* pcode decode exception flag */
#define PCDEF_VALID_CONTINUE          0x00000001
#define PCDEF_VALID_FAILED            0x00000002
#define PCDEF_VALID_RESULTS           0x00000004

#define PCDEF_VALID_CONT_FAIL         (PCDEF_VALID_CONTINUE | PCDEF_VALID_FAILED)
#define PCDEF_VALID_ALL               (PCDEF_VALID_CONTINUE | PCDEF_VALID_FAILED | PCDEF_VALID_RESULTS)


#define PCDM_TESTBYTE(eaddr, in1b)    (*(uint8_t *)(eaddr) == (uint8_t)(in1b))
#define PCDM_TESTWORD(eaddr, in2b)    (*(uint16_t *)(eaddr) == (uint16_t)(in2b))
#define PCDM_CHKVPARAM(val)           (((val) & 0x80000000) == 0x80000000)  //判断变参
#define PCDM_CHKFLAG(val, flag)       (((val) & flag) == flag)

typedef int (WINAPI * func_qsym)(int qAddr, wchar_t * iText, size_t maxc);
typedef int (WINAPI * func_pceh)(int eType, int eFlag, struct _PCODE_DECODE_OBJECT * ppdo, PVOID pVar);

union un_dword
{
  WORD        wd[2];
  DWORD       dw;
};

typedef struct _VB_PCODE_INSTRUCTIONSET
{
  LPCCH                               text_inst;
  LPCWCH                              text_param;
  uint32_t                            inst_type;
  signed int                          inst_len;
} VB_PCODE_INSTRUCTIONSET, VBPCINSET, *PVBPCINSET;

typedef struct _PCODE_DECODE_OBJECT
{
  /* 输入缓冲指针 */
  PBYTE                       inp_opbuf;          //set in
  PBYTE                       pri_opmap;
  PDWORD                      pri_jmpad;
  uint32_t                    cnt_jmpad;
  /* 当前位置 */
  uint32_t                    inp_idx;            //in
  /* 缓冲大小 */
  uint32_t                    inp_count;          //set in
  uint32_t                    len_fixed;          //out 指令长度 + 定参 = 固定长度
  uint32_t                    len_total;          //out 固定长度 + 变参 = 总计长度
  uint32_t                    flag_out;           //out
  uint32_t                    flag_user;          //in
  uint32_t                    inst_type;          //out
  LONG                        org_cpol;           //原Const Pool
  LONG                        stk_cpol;
  PWCHAR                      sz_mnem;            //set out  Mnemonic
  uint32_t                    sl_mnem;            //set in
  PWCHAR                      sz_note;            //set out
  uint32_t                    sl_note;            //set in
  uint32_t                    mod_base;           //set in
  uint32_t                    mod_size;           //set in
  PVBPDI                      vb_pdi;             //set in
  func_qsym                   func_qs;            //query symbol function
  func_pceh                   func_eh;            //Exception Handler
  BOOL                        bl_init;
  uint32_t                    lastval;            //out
  uint32_t                    bk_idx;
  uint16_t                    pri_offset;
  BYTE                        ib_std;
  BYTE                        ib_lead;
} PCODE_DECODE_OBJECT, PDO, *PPDO;
