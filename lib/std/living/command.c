varargs int handle_command (string command, int debug) {
    string *split, action, input;
    string cmdPath;

    mixed result, resultGo;

    if (!environment() || !command) {
        return 0;
    }
    if (this_object()->query_immobile()) {
        message("status", "You are unable to take any actions right now.", this_object());
        return 1;
    }

    if (strsrch(command, "enter") == 0) {
        command = "go " + command;
    } else if (strsrch(command, "out") == 0) {
        command = "go " + command;
    }

    split = explode(command, " ") - ({ "" });
    action = split[0];
    input = sizeof(split) > 1 ? command[(strlen(action)+1)..] : 0;
    if (cmdPath = D_COMMAND->query_command(action)) {
        mixed *parse = parse_command_flags(input);
        catch(call_other(cmdPath + "/" + action, "command", parse[0], parse[1]));
        return 1;
    }

    result = parse_sentence(command, debug);
    if (stringp(result)) {
        write(result + "\n");
        return 1;
    }

    /*
    ** If the result is 0, the parser didn't know the verb so we keep looking.
    ** If a 1 was returned, then nothing more needs to be done.
    ** If the result is -1 or -2, the parser figured something was wrong.
    **
    */
    switch (result) {
    case 0: // Parser found no verb, fall through
        break;
    case 1: // Parser called verb
        return 1;
    case -1:
        write("Nonsense?!\n");
        return 1;
    case -2:
        write("You aren't able to do that.\n");
        return 1;
    default:
        write("Unexpected result.\n");
        return 1;
    }
    if (environment() && environment()->query_exit(command)) {
        resultGo = parse_sentence("go " + command);
        if (resultGo == 1) {
            return 1;
        }
        if (!result) {
            result = resultGo;
        }
    }
    if (stringp(result) && (result[0..12] != "You can't go " && result[0..11] != "There is no ")) {
        write(result + "\n");
        return 1;
    }

    return 0;
}