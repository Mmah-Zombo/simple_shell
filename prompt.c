#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  char *line = NULL;
  size_t size = 0;
  int status;

  while (1)
  {
    // Display prompt and read command line
    printf("$ ");
    int chars_read = getline(&line, &size, stdin);

    // Check for end of file condition (Ctrl+D)
    if (chars_read == -1)
    {
      printf("\n");
      break;
    }

    // Remove newline character from command line
    line[chars_read - 1] = '\0';

    // Fork a child process to execute the command
    pid_t pid = fork();

    if (pid == 0)
    {
      // Child process
      execlp(line, line, NULL);

      // If execlp returns, it was unsuccessful
      printf("sh: command not found: %s\n", line);
      exit(1);
    }
    else if (pid > 0)
    {
      // Parent process
      waitpid(pid, &status, 0);
    }
    else
    {
      // Error
      perror("fork");
      exit(1);
    }
  }

  free(line);

  return 0;
}

