1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation uses waitpid() to ensure that all piped commands finish before accepting new user input. If we forgot to call waitpid() on child processes, it would lead to memory leaks since the exit statuses would be uncollected.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because it can cause processes to run indefinitely when expecting EOF. This can lead to memory leaks and closing them makes sure that all resources are cleaned up.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is implemented as a built-in command because it works with the working directory of where the shell is. If it were executed externally using execvp(), it would change its own working directory but not the directory of the shell.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

You can modify the implementation to allow an arbitrary number of piped commands by allocating and reallocating memory as needed. The trade off of this is that you would need to frequently reallocate memory and this can lead to memory management issues.
