nosave private object __Shell;

object query_shell () {
    return __Shell;
}

protected void shell_start () {
    if (!__Shell) {
        __Shell = clone_object("/secure/shell/shell.c");
        __Shell->shell_start();
    }
}

protected void shell_stop () {
    if (__Shell) {
        __Shell->handle_remove();
    }
}