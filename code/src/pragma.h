/**
 * @file pragma.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Pragma functions interface
 * @version 0.2
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gcc-plugin.h>
#include <c-family/c-pragma.h>

/**
 * @brief handle the pragma directive
 *
 * @param dummy Pointer to the cpp_reader instance provided by GCC. This object allows access to the current token stream in order to extract pragma name and arguments.
 */
void my_pragma_handler(cpp_reader *dummy);

/**
 * @brief check if all functions have been checked
 *
 * @param gcc_data Pointer to GCC internal data passed to the callback.
 * @param user_data Pointer to user-defined context data associated with the callback.
 */
void check_unused_function(void *gcc_data, void *user_data);
