/* vim: set et ts=4
 *
 * Copyright (C) 2015 Mirko Pasqualetti  All rights reserved.
 * https://github.com/udp/json-parser
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "json.h"
#include "json_manager.h"

/*
 * Test for json.c
 *
 * Compile with
 *         gcc -o test_json -I.. test_json.c ../json.c -lm
 *
 * USAGE: ./test_json <json_file>
 */

static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}

static void process_array(json_value* value, int depth);
// json_value* get_value_node(const char *pszKeyName, const json_value* pstjsonList);

static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;

        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                process_value(value->u.object.values[x].value, depth+1);
        }
}

static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth);
        }
}

void process_value(json_value* value, int depth)
{
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %ld\n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}

json_value* get_value_node(const char *pszKeyName, const json_value* pstjsonList)
{
    // Locals.
    int nLength = 0;
    int nIndex  = 0;
    json_value *pstJSONValue = NULL;


    if(NULL != pszKeyName && 0 < strlen(pszKeyName) && NULL != pstjsonList)
    {
        nLength = pstjsonList->u.object.length;

        for(nIndex = 0; nIndex < nLength; ++nIndex)
        {
            if(0 == strcmp(pstjsonList->u.object.values[nIndex].name, pszKeyName))
            {
                pstJSONValue = pstjsonList->u.object.values[nIndex].value;
                nIndex = nLength; // loop break;
            }
        }
    }

    return pstJSONValue;
}

float get_float_val(json_value* value)
{
    // Loclas.
    float fVal = 0.0f;

    if (NULL != value)
    {
        switch (value->type)
        {
            case json_integer:
                // Might cause data loss.
                fVal = (float)value->u.integer;
                    break;
            case json_double:
                // Might cause data loss.
                fVal = (float)value->u.dbl;
                    break;
            case json_string:
                fVal = strtof(value->u.string.ptr, NULL);
                    break;
            case json_boolean:
                    // Might cause data loss.
                    fVal = (float)value->u.boolean;
                    break;
            case json_none:
            default:
                fVal = 0.0f;
                    break;
        }
    }
    return fVal;
}

int get_int_val(json_value* value)
{
    // Loclas.
    int nVal = 0;

    if (NULL != value)
    {
        switch (value->type)
        {
            case json_integer:
                nVal = value->u.integer;
                    break;
            case json_double:
                    nVal = (int)value->u.dbl;
                    break;
            case json_string:
                nVal = strtol(value->u.string.ptr, NULL, 10);
                    break;
            case json_boolean:
                    nVal = (int)value->u.boolean;
                    break;
            case json_none:
            default:
                nVal = 0;
                    break;
        }
    }
    return nVal;
}

//static  json_value* get_node(const char* pszKeyName, const json_value* pstjsonList)
//{
//    // Locals.
//    int nLength = 0;
//    int nIndex  = 0;
//    json_value *pstJSONValue = NULL;
//
//    // TODO: null check and type check.
//
//    nLength = pstjsonList->u.object.length;
//
//    for(nIndex = 0; nIndex < nLength; ++nIndex)
//    {
//        printf("[%s]", pstjsonList->u.object.values[nIndex].name);
//        if(0 == strcmp(pstjsonList->u.object.values[nIndex].name, pszKeyName))
//        {
//            pstJSONValue = &(pstjsonList->u.object.values[nIndex].value);
//            nIndex = nLength; // loop break;
//        }
//    }
//
//    return pstJSONValue;
//}
