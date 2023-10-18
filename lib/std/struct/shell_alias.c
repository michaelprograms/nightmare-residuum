struct ShellAlias {
    string template;
    string *defaults;
    int num_args;
    int global_alias_status;
}