
#ifndef IPA_NAT_DRV_H
#define IPA_NAT_DRV_H

#include "string.h"  /* memset */
#include "stdlib.h"  /* free, malloc */
#include "stdint.h"  /* uint32_t */

/**
 * struct ipa_nat_ipv4_rule - To hold ipv4 nat rule
 * @target_ip: destination ip address
 * @private_ip: private ip address
 * @target_port: destination port
 * @private_port: private port
 * @protocol: protocol of rule (tcp/udp)
 * @pdn_index: PDN index in the PDN config table
 */
typedef struct {
	uint32_t target_ip;
	uint32_t private_ip;
	uint16_t target_port;
	uint16_t private_port;
	uint16_t public_port;
	uint8_t  protocol;
	uint8_t  pdn_index;
} ipa_nat_ipv4_rule;

/**
* struct ipa_nat_pdn_entry - holds a PDN entry data
* @public_ip: PDN's public ip address
* @src_metadata: metadata to be used for source NAT metadata replacement
* @dst_metadata: metadata to be used for destination NAT metadata replacement
*/
typedef struct {
	uint32_t public_ip;
	uint32_t src_metadata;
	uint32_t dst_metadata;
} ipa_nat_pdn_entry;

/**
 * ipa_nat_add_ipv4_tbl() - create ipv4 nat table
 * @public_ip_addr: [in] public ipv4 address
 * @number_of_entries: [in]  number of nat entries
 * @table_handle: [out] Handle of new ipv4 nat table
 *
 * To create new ipv4 nat table
 *
 * Returns:	0  On Success, negative on failure
 */
int ipa_nat_add_ipv4_tbl(uint32_t public_ip_addr,
				uint16_t number_of_entries,
				uint32_t *table_handle);

/**
 * ipa_nat_del_ipv4_tbl() - delete ipv4 table
 * @table_handle: [in] Handle of ipv4 nat table
 *
 * To delete given ipv4 nat table
 *
 * Returns:	0  On Success, negative on failure
 */
int ipa_nat_del_ipv4_tbl(uint32_t table_handle);

/**
 * ipa_nat_add_ipv4_rule() - to insert new ipv4 rule
 * @table_handle: [in] handle of ipv4 nat table
 * @rule: [in]  Pointer to new rule
 * @rule_handle: [out] Return the handle to rule
 *
 * To insert new ipv4 nat rule into ipv4 nat table
 *
 * Returns:	0  On Success, negative on failure
 */
int ipa_nat_add_ipv4_rule(uint32_t table_handle,
				const ipa_nat_ipv4_rule * rule,
				uint32_t *rule_handle);

/**
 * ipa_nat_del_ipv4_rule() - to delete ipv4 nat rule
 * @table_handle: [in] handle of ipv4 nat table
 * @rule_handle: [in] ipv4 nat rule handle
 *
 * To insert new ipv4 nat rule into ipv4 nat table
 *
 * Returns:	0  On Success, negative on failure
 */
int ipa_nat_del_ipv4_rule(uint32_t table_handle,
				uint32_t rule_handle);


/**
 * ipa_nat_query_timestamp() - to query timestamp
 * @table_handle: [in] handle of ipv4 nat table
 * @rule_handle: [in] ipv4 nat rule handle
 * @time_stamp: [out] time stamp of rule
 *
 * To retrieve the timestamp that lastly the
 * nat rule was accessed
 *
 * Returns:	0  On Success, negative on failure
 */
int ipa_nat_query_timestamp(uint32_t  table_handle,
				uint32_t  rule_handle,
				uint32_t  *time_stamp);


/**
* ipa_nat_modify_pdn() - modify single PDN entry in the PDN config table
* @table_handle: [in] handle of ipv4 nat table
* @pdn_index : [in] the index of the entry to be modified
* @pdn_info : [in] values for the PDN entry to be changed
*
* Modify a PDN entry
*
* Returns:	0  On Success, negative on failure
*/
int ipa_nat_modify_pdn(uint32_t  tbl_hdl,
	uint8_t pdn_index,
	ipa_nat_pdn_entry *pdn_info);
#endif /* IPA_NAT_DRV_H */
