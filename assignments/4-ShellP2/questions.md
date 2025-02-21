1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Using `fork` before running `execvp` allows us to create a new child process while allowing the parent process to run separately. If we just used `execvp`, it would overwrite the parent process and prevent it from properly running the command.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If `fork()` fails, it returns `ERR_EXEC_CMD` and returns an error that prints `error: execvp()`. `ERR_EXEC_CMD` returns `-6` which is a non-zero status, indicating failure.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  `execvp()` looks through the directories listed in the `PATH` variable and attempts to locate the executable. If no match is found, it returns `ERR_EXEC_CMD`, indicating an error.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling `wait()` ensures that the parent process waits for the child process to finish before continuing. If we didn't call it, the child process would end and the exit status would, leading to resource exhaustion.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  `WEXITSTATUS()` takes the value returned from `wait()`. This allows the parent process to determine if the child exited successfully.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It treats quoted arguments as a single argument rather than multiple. This is important because many commands can contain spaces, such as directories and also the `echo` command.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The parsing logic is different compared to the previous one because we parse the command directly from the shell instead of making a separate command list. We also created parsing logic for quoted commands.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in Linux all for different processes to communicate with each other. This differs from IPC because it is more lightweight and allows for asynchronous communication.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  `SIGKILL` forces a process to terminate immediately, but it does not execute any cleanup. This cannot be ignored. `SIGTERM` requests that a process terminates and allows the process to clean up before exiting. `SIGINT` is when the user interrupts a process, such as when they hit `CTRL+C`. This also allows for the process to clean up.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  `SIGSTOP` pauses a process until it is resumed. Just like `SIGKILL`, this cannot be ignored and does not allow for any cleanup.
