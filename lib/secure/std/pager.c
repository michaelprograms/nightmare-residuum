#include <config.h>

/** @type {STD_USER} */
nosave private object __User;
nosave private string *__Lines;
nosave private int __LineNum, __LinesCount;
nosave private int __ChunkSize = 40;

object query_user () {
    return __User;
}

nomask private void done (int focus) {
    __User->input_pop();
    if (focus) {
        __User->input_focus();
    }
    destruct();
}

nomask private string prompt () {
    int chunkEnd = __LineNum + __ChunkSize;
    int percent;
    string prompt = "";

    if (chunkEnd >= __LinesCount) {
        chunkEnd = __LinesCount;
        percent = 100;
        done(1);
        return "";
    } else {
        int width = to_int(query_account_setting("width")) || DEFAULT_WIDTH;
        percent = chunkEnd * 100 / __LinesCount;
        prompt = sprintf("Lines %s to %s of %s (%i%%) --- press <enter> or <q>", format_integer(__LineNum + 1), format_integer(chunkEnd), format_integer(__LinesCount), percent);
        prompt = sprintf("%|*s", width, prompt);
        return "%^B_WHITE%^BLACK%^" + prompt + "%^RESET%^";
    }
}

private void handle_page (mixed arg) {
    int i, l;

    if (arg == -1) {
        return destruct();
    }
    if (arg) {
        switch (arg[0]) {
            case 'q':
                done(0);
                return; // to prompt
            default:
                __LineNum += __ChunkSize;
        }
    }

    l = __LinesCount;
    for (i = __LineNum; i < l && i < __LineNum + __ChunkSize; i ++) {
        message("system", __Lines[i] + "\n", __User);
    }
    if (__LinesCount >= __ChunkSize) {
        return; // to prompt
    }

    done(0);
}

/**
 * Start pager mode for the user.
 *
 * @param lines the string array of text to page through
 * @param {STD_USER} user
 */
void start (string *lines, object user) {
    if (user) {
        __ChunkSize = to_int(user->query_setting("lines"));
        __Lines = lines;
        __LinesCount = sizeof(__Lines);
        __User = user;
        __User->input_push((: handle_page :), (: prompt :));
        if (catch (handle_page(0))) {
            __User->input_pop();
            __User->input_focus();
        }
    }
}