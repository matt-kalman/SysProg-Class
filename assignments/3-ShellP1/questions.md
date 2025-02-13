1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` is better for strings because it allows us to read full lines, including spaces. This makes it better for handling user input especially in loops.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  `malloc()` is a better choice to allocate memory for `cmd_buff` than a fixed size array because it allows for dynamic memory allocation. This makes better for handling various sizes of input. Having a fixed-size array would put a fixed limit on the length of the commands and possibly cause memory to be wasted. `malloc()` allows us to allocate the proper amount of memory and also free memory as needed.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: Trimming leading and trailing spaces is important because if we didn't, it may be improperly stored within the buffer. The shell may misinterpret the command if there are spaces and won't process it correctly. Commands that have arguments may also function differently if spaces are not trimmed properly. This also makes it easier for the user to input commands into the shell without having to worry about extra whitespace or not.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: We should implement output redirection `>`, output appending `>>`, and input redirection `<`. The challenge we may have implementing this is proper file handling.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection controls where inputs and outputs go and deal with file handling. Piping deals with connecting outputs of one command to the input of another. Redirection allows for data to be sent from one file to another while pipes connects multiple commands in real-time.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It allows for errors to be isolated so it can be handled separately from standard output. This ensures the errors are processed correctly and makes sure error messages don't mess with standard output data.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  We can detect return and exit codes. Non-zero codes usually indicate errors while 0 shows that the command has been processed correctly. We can provide a way to merge errors and outputs if necessary by using special redirection operators.