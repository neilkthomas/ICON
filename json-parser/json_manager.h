#ifndef _JSON_MANAGER_H_INCLUDED_
#define _JSON_MANAGER_H_INCLUDED_

#include "json.h"

/**
 * Description : Function shall check the json object and
 *               return the value node based on the provided key.
 * Args        :
 *              pszKeyName    - Key name to be searched.
 *              pstjsonObject - json object node '{}'.
 * Return      : Return NULL if search failed or invalid input is provided.
 *               Othervise return the value node.
 */
json_value* get_value_node(const char *pszKeyName, const json_value* pstjsonObject);

/**
 * Description : Function shall check the type of the value node and
 *               convert the value in to float value.
 * Args        :
 *              pstJSONValNode - json value node.
 * Return      : Return float value.
 */
float get_float_val(json_value* pstJSONValNode);

/**
 * Description : Function shall check the type of the value node and
 *               convert the value in to int format.
 * Args        :
 *               pstJSONValNode - json value node.
 * Return      : Return int value.
 */
int get_int_val(json_value* value);

/**
 * Description : Test method for debug purpose.
 * Args        :
 *               value  - json node.
 *               nDepth - This is to align the json debug print.
 *               Pass '0' if root node is passed for debug.
 * Return      : None.
 */
void process_value(json_value* pstJSON, int nDepth);

#endif
