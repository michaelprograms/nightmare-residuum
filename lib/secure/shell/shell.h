#ifndef __SHELL_H__
#define __SHELL_H__

/* --- shell --- */
void execute_command (string input);
void handle_remove ();
protected void shell_input (mixed input);
protected void shell_init ();
protected mixed query_prompt ();
void shell_start ();

/* --- alias --- */
nomask string *query_alias_names ();
// nomask class ShellAlias query_alias (string alias);
varargs void add_alias (string name, string template, string *defaults, int xverb);
// remove_alias
mixed expand_alias (string input);
varargs protected void cmd_alias (mixed argv, string *implode_info);

/*  --- */

#endif /* __SHELL_H__ */
