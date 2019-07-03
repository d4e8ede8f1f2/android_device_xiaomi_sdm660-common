
#include "stdint.h"  /* uint32_t */
#include "stdio.h"
#include <netinet/in.h> /* for proto definitions */

#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

/*============ Preconditions to run NAT Test cases =========*/
#define IPA_NAT_TEST_PRE_COND_TE  20

#define CHECK_ERR1(x, tbl_hdl) \
  if(ipa_nat_validate_ipv4_table(tbl_hdl)) { \
    if(sep) {\
       ipa_nat_del_ipv4_tbl(tbl_hdl); \
     }\
    return -1;\
  }\
  if(x) { \
    IPAERR("%d\n", ret); \
    if(sep) {\
      ipa_nat_del_ipv4_tbl(tbl_hdl); \
     }\
     return -1; \
  }

#define CHECK_ERR(x) if(x) { \
    IPAERR("%d\n", ret); \
    return -1;\
 }

#if 0
#define CHECK_ERR(x) if(x) { \
    IPAERR("%d\n", ret); \
    if(sep) {\
      ipa_nat_del_ipv4_tbl(tbl_hdl); \
    }\
    return -1;\
 }
#endif

#define IPADBG(fmt, args...) printf(" %s:%d " fmt, __FUNCTION__, __LINE__, ## args)
#define IPAERR(fmt, args...) printf(" %s:%d " fmt, __FUNCTION__, __LINE__, ## args)

#define NAT_DUMP
int ipa_nat_validate_ipv4_table(u32);

int ipa_nat_test000(int, u32, u8);
int ipa_nat_test001(int, u32, u8);
int ipa_nat_test002(int, u32, u8);
int ipa_nat_test003(int, u32, u8);
int ipa_nat_test004(int, u32, u8);
int ipa_nat_test005(int, u32, u8);
int ipa_nat_test006(int, u32, u8);
int ipa_nat_test007(int, u32, u8);
int ipa_nat_test008(int, u32, u8);
int ipa_nat_test009(int, u32, u8);
int ipa_nat_test010(int, u32, u8);
int ipa_nat_test011(int, u32, u8);
int ipa_nat_test012(int, u32, u8);
int ipa_nat_test013(int, u32, u8);
int ipa_nat_test014(int, u32, u8);
int ipa_nat_test015(int, u32, u8);
int ipa_nat_test016(int, u32, u8);
int ipa_nat_test017(int, u32, u8);
int ipa_nat_test018(int, u32, u8);
int ipa_nat_test019(int, u32, u8);
int ipa_nat_test020(int, u32, u8);
int ipa_nat_test021(int, int);
int ipa_nat_test022(int, u32, u8);
