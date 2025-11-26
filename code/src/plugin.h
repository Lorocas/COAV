/**
 * @file plugin.h
 * @author Lorenzo LUCAS -- ROBLOT (lorenzo.lucas--roblot@ensiie.eu), Alicia PLATH (alicia.plath@@ensiie.eu)
 * @brief Plugin interface
 * @version 0.2
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gcc-plugin.h>
#include <tree-pass.h>
#include <context.h>

/**
 * @brief Global variable required for plugin to execute
 *
 */
int plugin_is_GPL_compatible;

/**
 * @brief Main entry point for plugin
 *
 * @param plugin_info Structure containing the name and the arguments of the plugin
 * @param version Structure describing the GCC version
 * @return int return 0 if it successed
 */
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version);

/**
 * @brief Metadata describing the pass
 *
 */
const pass_data my_pass_data {
    GIMPLE_PASS, /**< Type */
    "my_pass", /**< Pass name */
    OPTGROUP_NONE, /**< optinfo_flags */
    TV_NONE, /**< tv_id */
    PROP_gimple_any, /**< Properties required */
    0, /**< Properties provided */
    0, /**< Properties destroyed */
    0, /**< todo_flags_start */
    0, /**< todo_flags_finish */
};

/**
 * @brief pass class herited from gimple_opt_pass
 *
 */
class my_pass : public gimple_opt_pass
{
public:
    /**
     * @brief Construct a new my_pass object
     *
     * @param ctxt GCC context which the pass is executed
     */
    my_pass(gcc::context *ctxt) : gimple_opt_pass (my_pass_data, ctxt) {}

    /**
     * @brief Clone the pass
     *
     * @return my_pass* New instance of the pass
     */
    my_pass* clone()
    {
        return new my_pass(g);
    }

    /**
     * @brief Determines whether the pass should run for the given function.
     *
     * @param fun Pointer to the function currently being analyzed.
     * @return bool return true if the pass should run, false otherwise.
     */
    bool gate(function *fun);

    /**
     * @brief Executes the logic of the pass on the given function.
     *
     * @param fun Pointer to the function currently being analyzed.
     * @return unsigned int Status code returned to GCC.
     */
    unsigned int execute(function *fun);
};
