#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
void print_dragon();

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);

    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    return OK;
}

int build_cmd_list(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (cmd_line == NULL || cmd_buff == NULL) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    if (alloc_cmd_buff(cmd_buff) == ERR_MEMORY) {
        return ERR_MEMORY;
    };

    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

    char *buff = cmd_buff->_cmd_buffer;
    int argc = 0;
    bool quoted = false;
    char quote_char = '\0';

    while (*buff) {
        while (isspace(*buff)){
            buff++;
        }

        if (*buff == '\0') {
            break;
        }

        if (*buff == '"' || *buff == '\'') {
            quoted = true;
            quote_char = *buff;
            buff++;
        }

        cmd_buff->argv[argc] = buff;

        while (*buff && (quoted || !isspace(*buff))) {
            if (quoted && *buff == quote_char) {
                *buff = '\0';
                quoted = false;
                break;
            }
            buff++;
        }

        if (!quoted && *buff) {
            *buff = '\0';
            buff++;
        }

        argc++;
        if (argc >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
    }

    cmd_buff->argc = argc;
    cmd_buff->argv[argc] = NULL;

    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    }

    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds command = match_command(cmd->argv[0]);

    switch (command) {
        case BI_CMD_EXIT:
            exit(OK_EXIT);
        case BI_CMD_CD:
            if (cmd->argc > 1) {
                if (chdir(cmd->argv[1]) != 0) {
                    fprintf(stderr, "cd: %s: No such file or directory\n", cmd->argv[1]);
                }
            }
            return BI_EXECUTED;
        case BI_CMD_DRAGON:
            print_dragon();
            return BI_CMD_DRAGON;
        default:
            return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        fprintf(stderr, "error: execvp\n");
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return ERR_EXEC_CMD;
    } else {
        fprintf(stderr, "error: fork\n");
        return ERR_EXEC_CMD;
    }
}

int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
    cmd_buff_t cmd;

    if (cmd_buff == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strlen(cmd_buff) == 0) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        }

        if(alloc_cmd_buff(&cmd) == ERR_MEMORY) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(cmd_buff);
            return EXIT_FAILURE;
        }

        rc = build_cmd_list(cmd_buff, &cmd);
        if (rc == OK) {
            if (exec_built_in_cmd(&cmd) == BI_NOT_BI) {
                exec_cmd(&cmd);
            }
        }
        free_cmd_buff(&cmd);
    }

    free(cmd_buff);

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}
