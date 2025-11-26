/**
 * @file pragma.cpp
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@ensiie.eu)
 * @brief Pragma functions implementation
 * @version 0.4
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "pragma.h"
#include <tree.h>
#include <gimple.h>
#include <gimple-iterator.h>
#include <diagnostic.h>

/**
 * @brief List of functions that need to be analyzed.
 *
 */
vec<const char*> list_func;


/**
 * @brief List of functions that have been analized.
 *
 */
vec<const char*> used_func;


/**
 * @brief true if a pragma statement exists, false otherwise.
 *
 */
bool pragma_execute = false;


void my_pragma_handler(cpp_reader *dummy)
{

    location_t loc;
    enum cpp_ttype token;
    tree x;
    bool close_paren_needed_p = false;

    if (cfun)
    {
      error ("%<#pragma GCC option%> is not allowed inside functions");
      return;
    }

    token = pragma_lex (&x, &loc);
    // si les paramètres commencent par une parenthèse ouvrante, il y a besoin d'une parenthèse fermante
    if (token == CPP_OPEN_PAREN)
    {
      close_paren_needed_p = true;
      token = pragma_lex(&x, &loc);
    }

    if (token == CPP_NAME)
    {
        // tant que le curseur n'est pas à la fin des paramètres
        do
        {
            // si la fonction n'est pas déjà dans la liste
            if (!list_func.contains(IDENTIFIER_POINTER(x))) list_func.safe_push(IDENTIFIER_POINTER(x));
            else warning_at(loc, 0, "The function name: %s appears more than 1 time", IDENTIFIER_POINTER(x));

            token = pragma_lex(&x);

            // tant qu'on n'a pas atteint de virgule on récupère le premier argument
            while (token == CPP_COMMA) token = pragma_lex(&x);

        }
        while (token == CPP_NAME);

        // s'il y avait des parenthèses ouvrantes, il faut des parenthèses fermantes
        if (close_paren_needed_p)
        {
            if (token == CPP_CLOSE_PAREN) token = pragma_lex (&x);
            else error ("%<#pragma GCC target (string [,string]...)%> does not have a final %<)%>");
        }


        if (token != CPP_EOF)
        {
            error ("%<#pragma GCC target%> string is badly formed");
            return;
        }

        pragma_execute = true;
    }

}


void check_unused_function(void *gcc_data, void *user_data)
{
    for (unsigned int i=0; i<list_func.length(); i++)
    {
        if (!used_func.contains(list_func[i])) warning(0, "function '%s' defined in pragma instruction but not used", list_func[i]);
    }
}
