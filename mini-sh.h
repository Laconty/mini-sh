#ifndef MINI_SHELL
#define  MINI_SHELL

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int lsh_cd(char **args);
int lsh_help(char **);
int lsh_exit(char **);

int lsh_num_builtins(void);

void lsh_loop (void);

char *lsh_read_line(void);

char **lsh_split_line(char*);

#endif // MINI_SHELL
