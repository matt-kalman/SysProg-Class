#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int count_words(char *, int, int);
void reverse_string(char *, int, int);
void word_print(char *, int, int);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int buff_len = 0;
    int user_str_len = 0;
    int is_whitespace = 0;

    while (*user_str != '\0'){
        if (*user_str != ' '){
            *buff++ = *user_str;
            is_whitespace = 0;
        } else if (!is_whitespace){
            *buff++ = ' ';
            is_whitespace = 1;
        }
        buff_len++;
        user_str_len++;
        user_str++;

        if (user_str_len > len){
            printf("Supplied string is too long for buffer.\n");
            return -1; //Handles exception if user supplied string is too large
        }
    }

    while (buff_len < len){
        *buff++ = '.';
        buff_len++;
    }

    return user_str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++){
        if (*buff != ' ' && *buff != '.'){
            if (!in_word){
                word_count++;
                in_word = 1;
            }
        } else{
            in_word = 0;
        }
        buff++;
    }

    return word_count;
}

void reverse_string(char *buff, int len, int str_len){
    printf("Reversed String: ");
    for (int i = str_len - 1; i >= 0; i--){
        putchar(buff[i]);
    }
    printf("\n");
}

void word_print(char *buff, int len, int str_len){
    int word_num = 1;
    int char_count = 0;

    printf("Word Print\n");
    printf("----------\n");

    for (int i = 0; i < str_len + 1; i++){
        if (*buff == ' ' || *buff == '.'){
            printf(" (%d)\n", char_count);
            char_count = 0;
            word_num++;
        } else{
            if (char_count == 0){
                printf("%d. ", word_num);
            }
            putchar(*buff);
            char_count++;
        }
        *buff++;
    }
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe because of short-circuit evaluation. argc is the number of args, so if this is true, then argv[1] will exist.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      argv[0] is the filename, therefore it takes the filename and outputs it to stdout as shown in the usage function above.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("malloc has failed to allocate buffer.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, BUFFER_SZ, user_str_len);
            break;
        case 'w':
            word_print(buff, BUFFER_SZ, user_str_len);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          It is good practice to provide both the pointer and the length because
//          it provides you with flexibility and can also reduce the risk of buffer overflows.