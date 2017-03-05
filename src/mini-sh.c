#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "mini-sh.h"

// ARRAY OF pointers to FUNCTIONS whose PARAM is (char **) and RETURN int
int (*builtin_func[]) (char **) = {
  &msh_cd,
  &msh_help,
  &msh_exit
};

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int msh_num_builtins(void) {
  return sizeof(builtin_str) / sizeof(char *);
};

int msh_cd (char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "msh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("msh");
    }
  }
  
  return 1;
}

int msh_help (char **args) {
  printf("\n");
  printf(" Mini shell implementation\n");
  printf(" Type program name and arguments, and hit enter.\n");
  printf(" The following are built in:\n");
  
  for (int i = 0; i < msh_num_builtins(); ++i) {
    printf("  %s\n", builtin_str[i]);
  }
  
  printf("\n");
  
  return 1;
}

int msh_exit (char **args) {
  return 0;
}

int msh_launch (char **args) {
  pid_t pid;
  int status;
  
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("msh");
    }
    
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("msh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  
  return 1;
}

int msh_execute (char **args) {
  int i;
  
  if (args[0] == NULL) {
    // An empty command was entered
    return 1;
  }
  
  for (i = 0; i < msh_num_builtins(); ++i) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  
  return msh_launch(args);
}

void msh_loop (void) {
  char *line;
  char **args;
  int status;
  char cwd[1024];
  
  do {
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
      perror("msh: getcwd() error");
      exit(EXIT_FAILURE);
    }
    printf("%s> ", cwd);
    line = msh_read_line();
    args = msh_split_line(line);
    status = msh_execute(args);
    
    free(line);
    free(args);
  } while (status);
}

char *msh_read_line (void) {
  int bufsize = MSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  
  if (buffer == NULL) {
    fprintf(stderr, "msh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  
  while (1) {
    c = getchar();
    
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    
    position++;
    
    if (position >= bufsize) {
      bufsize += MSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "msh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

/**
 * Split a line into tokens
 * @param  line
 * @return Null-terminated array of tokens
 */
char **msh_split_line (char *line) {
  int bufsize = MSH_TOK_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;
  char **tokens_backup;
  
  if (!tokens) {
    fprintf(stderr, "msh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  
  token = strtok(line, MSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;
    
    if (position >= bufsize) {
      bufsize += MSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      
      if (tokens == NULL) {
        free(tokens_backup);
        fprintf(stderr, "msh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    
    token = strtok(NULL, MSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  
  return tokens;
}
