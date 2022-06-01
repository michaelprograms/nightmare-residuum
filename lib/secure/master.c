#define SEFUN "/secure/sefun/sefun.c"

#include "/secure/include/ldmud/driver_hook.h"

inherit "/secure/daemon/master.c";

string _auto_include_hook (string base_file, string current_file, int sys_include) {
    // Do nothing for includes.
    if(current_file) return 0;

    if(base_file[0] != '/') base_file = "/" + base_file;

    return "#include \"/secure/include/global.h\"\n";
}

string _include_dirs_hook (string include_name, string current_file) {
    string name, part;
    int pos;
    if (include_name[0] != '/') name = "/" + include_name;
    else name = include_name;
    if (file_size(name) >= 0) return name;
    name = "/include" + name;
    if (file_size(name) >= 0) return name;
    name = "/secure" + name;
    if (file_size(name) >= 0) return name;
    return 0;
}

mixed _load_uids_fun (mixed object_name, object prev) {
    return 1;
}
mixed _clone_uids_fun (object blueprint, string new_name, object prev) {
    return 1;
}

mixed get_master_uid () {
    return 1;
}

mixed get_simul_efun () {
    mixed error;
    object ob;

    if (error = catch(ob = load_object(SEFUN))) {
        write("Failed to load SEFUN: " + error);
        return shutdown();
    }

    return program_name(ob);
}

void inaugurate_master (int arg) {
    if (!arg) {
        if (previous_object() && previous_object() != this_object()) {
            return;
        }
    }

    set_driver_hook(H_AUTO_INCLUDE, #'_auto_include_hook);
    set_driver_hook(H_INCLUDE_DIRS, #'_include_dirs_hook);
    set_driver_hook(H_LOAD_UIDS,
        unbound_lambda(({ 'object_name }), ({ #'_load_uids_fun, 'object_name, ({ #'previous_object }) }))
    );
    set_driver_hook(H_CLONE_UIDS,
        unbound_lambda(({ /* object */ 'blueprint, 'new_name}), ({ #'_clone_uids_fun, 'blueprint, 'new_name, ({#'previous_object}) }) )
    );
}

int prepare_destruct (object ob) {
    return 0;
}

int privilege_violation (string op, mixed who, mixed arg, mixed arg2) {
    if (who == this_object() || who == find_object(SEFUN)) return 1;

    if (program_name(who) == "/secure/daemon/test.c") return 1;

    debug_message("!!! privilege_violation: "+op+" "+SEFUN->identify(who)+" "+SEFUN->identify(arg)+" "+SEFUN->identify(arg2)+"\n");

    return 0;
}