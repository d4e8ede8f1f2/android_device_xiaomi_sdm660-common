
#ifndef _LOC_MISC_UTILS_H_
#define _LOC_MISC_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
FUNCTION loc_split_string

DESCRIPTION:
    This function is used to split a delimiter separated string into
    sub-strings. This function does not allocate new memory to store the split
    strings. Instead, it places '\0' in places of delimiters and assings the
    starting address of the substring within the raw string as the string address
    The input raw_string no longer remains to be a collection of sub-strings
    after this function is executed.
    Please make a copy of the input string before calling this function if
    necessary

PARAMETERS:
    char *raw_string: is the original string with delimiter separated substrings
    char **split_strings_ptr: is the arraw of pointers which will hold the addresses
                              of individual substrings
    int max_num_substrings: is the maximum number of substrings that are expected
                            by the caller. The array of pointers in the above parameter
                            is usually this long
    char delimiter: is the delimiter that separates the substrings. Examples: ' ', ';'

DEPENDENCIES
    N/A

RETURN VALUE
    int Number of split strings

SIDE EFFECTS
    The input raw_string no longer remains a delimiter separated single string.

EXAMPLE
    delimiter = ' ' //space
    raw_string = "hello new user" //delimiter is space ' '
    addresses  =  0123456789abcd
    split_strings_ptr[0] = &raw_string[0]; //split_strings_ptr[0] contains "hello"
    split_strings_ptr[1] = &raw_string[6]; //split_strings_ptr[1] contains "new"
    split_strings_ptr[2] = &raw_string[a]; //split_strings_ptr[2] contains "user"

===========================================================================*/
int loc_util_split_string(char *raw_string, char **split_strings_ptr, int max_num_substrings,
                     char delimiter);

/*===========================================================================
FUNCTION trim_space

DESCRIPTION
   Removes leading and trailing spaces of the string

DEPENDENCIES
   N/A

RETURN VALUE
   None

SIDE EFFECTS
   N/A
===========================================================================*/
void loc_util_trim_space(char *org_string);

/*===========================================================================
FUNCTION dlGetSymFromLib

DESCRIPTION
   Handy function to get a pointer to a symbol from a library.

   If libHandle is not null, it will be used as the handle to the library. In
   that case libName wll not be used;
   libHandle is an in / out parameter.
   If libHandle is null, libName will be used to dlopen.
   Either libHandle or libName must not be nullptr.
   symName must not be null.

DEPENDENCIES
   N/A

RETURN VALUE
   pointer to symName. Could be nullptr if
       Parameters are incorrect; or
       libName can not be opened; or
       symName can not be found.

SIDE EFFECTS
   N/A
===========================================================================*/
void* dlGetSymFromLib(void*& libHandle, const char* libName, const char* symName);

#ifdef __cplusplus
}
#endif

#endif //_LOC_MISC_UTILS_H_
