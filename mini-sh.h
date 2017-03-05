#ifndef MINI_SHELL
#define  MINI_SHELL

#define MSH_RL_BUFSIZE 1024
#define MSH_TOK_BUFSIZE 64
#define MSH_TOK_DELIM " \t\r\n\a"

int msh_cd (char **args);
int msh_help (char **);
int msh_exit (char **);

// Number of builtions command(3 at the current moment)
int msh_num_builtins (void);

// Main shell loop
void msh_loop (void);

// Read line from stdin
char *msh_read_line (void);

// Split line into null-terminated strings
char **msh_split_line (char*);

// Execute non built-in shell command
int msh_execute (char **args);

#endif // MINI_SHELL
