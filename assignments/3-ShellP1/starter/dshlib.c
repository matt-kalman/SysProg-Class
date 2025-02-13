#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || clist == NULL) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(clist, 0, sizeof(command_list_t));

    char *commands[CMD_MAX];
    int cmd_count = 0;

    while (*cmd_line == SPACE_CHAR) {
        cmd_line++;
    }

    char *token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL) {
        while (*token == SPACE_CHAR) {
            token++;
        }
        
        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR) {
            *end-- = '\0';
        }

        if (strlen(token) == 0 || cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        commands[cmd_count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_count;

    for (int i = 0; i < cmd_count; i++) {
        char *cmd = commands[i];
        token = strtok(cmd, " ");

        if (token == NULL || strlen(token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncpy(clist->commands[i].exe, token, EXE_MAX - 1);
        clist->commands[i].exe[EXE_MAX - 1] = '\0';

        char *args = clist->commands[i].args;
        args[0] = '\0';

        while ((token = strtok(NULL, " "))) {
            if (strlen(args) + strlen(token) + 1 >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcat(args, token);
            strcat(args, " ");
        }

        if (strlen(args) > 0) {
            args[strlen(args) - 1] = '\0';
        }
    }

    return OK;
}