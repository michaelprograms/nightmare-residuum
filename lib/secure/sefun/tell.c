// @TODO object find_user (string);

varargs void tell (mixed target, string msg) {
    if (arrayp(target)) {
        foreach (object t in target) {
            t->receive_message("type", msg);
        }
    } else {
        target->receive_message("type", msg);
    }
}

// varargs void tell_from_inside (mixed ob, string msg, int msg_type, mixed exclude) {
//     ob->receive_inside_msg(msg, exclude, msg_type | INSIDE_MSG);
// }

// varargs void tell_from_outside (mixed ob, string msg, int msg_type, mixed exclude) {
//     ob->receive_outside_msg(msg, exclude, msg_type | OUTSIDE_MSG);
// }

// varargs void tell_env (mixed ob, string msg, int msg_type, mixed exclude) {
//     object env;
//     if (env = environment(ob)) {
//         env->receive_inside_msg(msg, exclude, msg_type | INSIDE_MSG);
//     }
// }

// @TODO
// varargs void tell_user (string username, string msg, int msg_type) {
//     object who;
//     if (who = find_user(username)) {
//         tell(who, msg, msg_type);
//     }
// }

