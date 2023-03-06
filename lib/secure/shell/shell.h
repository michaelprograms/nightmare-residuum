#ifndef __SHELL_H__
#define __SHELL_H__

/* --- shell --- */
void handle_remove ();
mixed *parse_command_flags (string rawInput);
void execute_command (string input);
protected void shell_input (mixed input);
protected void shell_init ();
protected mixed query_prompt ();
void shell_start ();
int clean_up (int inherited);

/* --- alias --- */
nomask string *query_alias_names ();
// nomask class ShellAlias query_alias (string alias);
varargs void add_alias (string name, string template, string *defaults, int xverb);
nomask void remove_alias (string name);
mixed expand_alias (string input);

/* --- variable --- */
mixed query_variable (string v);
varargs mixed set_variable (string v, mixed value, int set_if_undefined);
void unset_variable (string v);
void set_variable_hook (string variable, function fn);

/*  --- */

#endif /* __SHELL_H__ */
