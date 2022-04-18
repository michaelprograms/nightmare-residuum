nosave private object __User;
nosave private string *__Lines;
nosave private int __LineNum, __LinesCount;
nosave private int __ChunkSize = 25;

nomask private string prompt () {
    int chunkEnd = __LineNum + __ChunkSize;
    int percent;
    string prompt = "";

    if (chunkEnd > __LinesCount) {
        chunkEnd = __LinesCount;
        percent = 100;
    } else {
        percent = chunkEnd * 100 / __LinesCount;
    }

    prompt = sprintf("Lines %i to %i of %i - %i%% ", __LineNum + 1, chunkEnd, __LinesCount, percent);
    message("system", "----- %^BOLD%^" + prompt + "%^RESET%^ -----", __User);
}

nomask private void done () {
    __User->input_pop();
    destruct(this_object());
}

private void handle_page (mixed arg) {
    int i, l;

    if (arg == -1) {
        return destruct(this_object());
    }
    if (arg) {
        switch (arg[0]) {
            case 'q':
                return done();
            default:
                __LineNum += __ChunkSize;
                if (__LineNum > __LinesCount) {
                    return done();
                }
        }
    }

    l = __LinesCount;
    for (i = __LineNum; i < l && i < __LineNum + __ChunkSize; i ++) {
        message("system", __Lines[i] + "\n", __User);
    }
    if (__LinesCount >= __ChunkSize) { // && __LineNum + __ChunkSize < __LinesCount) {
        write("__LineNum: "+__LineNum+"\n");
        return; // to prompt
    }

    done();
}

void start (string *lines, object user) {

    // @TODO check param

    __Lines = lines;
    __LinesCount = sizeof(__Lines);
    __User = user;
    __User->input_push((: handle_page :), (: prompt :));
    if (catch (handle_page(0))) __User->input_pop();
}