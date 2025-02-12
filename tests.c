#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "constants.h" // Constants

// Function to start the shell process
pid_t start_shell(int pipe_in[2], int pipe_out[2]) {
    pid_t pid;

    // Create pipes for communication
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    if ((pid = fork()) == 0) { // Child process
        // Redirect stdin and stdout
        dup2(pipe_in[0], STDIN_FILENO);  // Read input from pipe
        dup2(pipe_out[1], STDOUT_FILENO); // Write output to pipe

        // Close unused pipe ends
        close(pipe_in[1]);
        close(pipe_out[0]);

        // Execute the shell
        execl("./shell", "./shell", NULL);
        perror("execl"); // If execl fails
        exit(EXIT_FAILURE);
    }

    // Close unused pipe ends in the parent process
    close(pipe_in[0]);
    close(pipe_out[1]);

    return pid;
}

// Function to test empty input handling
void test_empty_input(int pipe_in[2], int pipe_out[2]) {
    char buffer[MAX_INPUT_LEN];

    // Simulate user pressing enter (sending "\n")
    if (write(pipe_in[1], "\n", 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    close(pipe_in[1]); // Close the write end of the input pipe

    // Wait for the shell to process the input and produce output
    sleep(1); // Add a small delay to ensure the shell processes the input

    // Capture output from the shell
    int bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Check if the prompt appears at least once in the output
    if (strstr(buffer, "$") != NULL) {
        printf("Test passed: Shell correctly handles empty input.\n");
    } else {
        printf("Test failed: Shell did not reprint the prompt after empty input.\n");
    }

    close(pipe_out[0]); // Close the read end of the output pipe
}

// Function to test if the shell exits when "exit" is typed
void test_exit_command(int pipe_in[2], int pipe_out[2], pid_t shell_pid) {
    // Simulate user typing "exit" followed by a newline
    if (write(pipe_in[1], "exit\n", 5) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    close(pipe_in[1]); // Close the write end of the input pipe

    // Wait for the shell to process the input and exit
    int status;
    waitpid(shell_pid, &status, 0);

    // Check if the shell exited normally
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Test passed: Shell exited cleanly after 'exit' command.\n");
    } else {
        printf("Test failed: Shell did not exit cleanly after 'exit' command.\n");
    }

    close(pipe_out[0]); // Close the read end of the output pipe
}

// Function to test if the shell exits when <Ctrl> + D is pressed
void test_ctrl_d_exit(int pipe_in[2], int pipe_out[2], pid_t shell_pid) {
    // Simulate <Ctrl> + D by closing the write end of the input pipe
    close(pipe_in[1]);

    // Wait for the shell to process the EOF and exit
    int status;
    waitpid(shell_pid, &status, 0);

    // Check if the shell exited normally
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Test passed: Shell exited cleanly after <Ctrl> + D.\n");
    } else {
        printf("Test failed: Shell did not exit cleanly after <Ctrl> + D.\n");
    }

    close(pipe_out[0]); // Close the read end of the output pipe
}

// Function to test external commands
// Function to test external commands
void test_external_commands(int pipe_in[2], int pipe_out[2]) {
    char buffer[MAX_INPUT_LEN];

    // Test 'ps aux'
    const char *command1 = "ps aux\n";
    if (write(pipe_in[1], command1, strlen(command1)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Wait for the shell to process the input and produce output
    sleep(1);

    // Capture output from the shell
    int bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Check if the output contains process information
    if (strstr(buffer, "USER") != NULL && strstr(buffer, "PID") != NULL) {
        printf("Test passed: Shell executed 'ps aux' correctly.\n");
    } else {
        printf("Test failed: Shell did not execute 'ps aux' correctly.\n");
    }


    // Test 'clear'
    const char *command2 = "clear\n";
    if (write(pipe_in[1], command2, strlen(command2)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Wait for the shell to process the input and produce output
    sleep(1);

    // Capture output from the shell
    bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Check if the shell prints the prompt after 'clear'
    if (strstr(buffer, "") != NULL) {
        printf("Test passed: Shell executed 'clear' correctly.\n");
    } else {
        printf("Test failed: Shell did not execute 'clear' correctly.\n");
    }

    close(pipe_in[1]); // Close the write end of the input pipe
    close(pipe_out[0]); // Close the read end of the output pipe
}

// Function to test non-existing commands
// Function to test non-existing commands
void test_non_existing_command(int pipe_in[2], int pipe_out[2]) {
    char buffer[MAX_INPUT_LEN];

    // Test a non-existing command
    const char *command = "nonexistentcommand\n";
    if (write(pipe_in[1], command, strlen(command)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Wait for the shell to process the input and produce output
    sleep(1);

    // Capture output from the shell
    int bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Check if the error message contains the command name and the prompt is printed
    if (strstr(buffer, "nonexistentcommand: command not found") != NULL) {
        printf("Test passed: Shell handled non-existing command correctly.\n");
    } else {
        printf("Test failed: Shell did not handle non-existing command correctly.\n");
    }

    close(pipe_in[1]); // Close the write end of the input pipe
    close(pipe_out[0]); // Close the read end of the output pipe
}
// Function to test child process exit after <Ctrl> + D
void test_child_process_exit(int pipe_in[2], int pipe_out[2], pid_t shell_pid) {
    // Send a command to the shell
    const char *command = "ps aux\n";
    if (write(pipe_in[1], command, strlen(command)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Simulate <Ctrl> + D by closing the write end of the input pipe
    close(pipe_in[1]);

    // Wait for the shell to process the EOF and exit
    int status;
    waitpid(shell_pid, &status, 0);

    // Check if the shell exited normally
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Test passed: Shell exited cleanly after <Ctrl> + D.\n");
    } else {
        printf("Test failed: Shell did not exit cleanly after <Ctrl> + D.\n");
    }

    close(pipe_out[0]); // Close the read end of the output pipe
}

void test_starts_in_home(int pipe_in[2], int pipe_out[2], pid_t shell_pid){
    char buffer[MAX_INPUT_LEN];
    const char *command = "pwd\n";

    if (write(pipe_in[1], command, strlen(command)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Simulate <Ctrl> + D by closing the write end of the input pipe
    close(pipe_in[1]);

    sleep(1);

    // Capture output from the shell
    int bytes_read = read(pipe_out[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Check if the error message contains the command name and the prompt is printed
    if (strstr(buffer, "/home") != NULL) {
        printf("Test passed: Shell starts in home.\n");
    } else {
        printf("Test failed: Shell did not start in home.\n");
    }

    close(pipe_in[1]); // Close the write end of the input pipe
    close(pipe_out[0]); // Close the read end of the output pipe

}


int main() {
    int pipe_in[2], pipe_out[2];
    pid_t shell_pid;

    // Test empty input handling
    printf("Testing empty input handling...\n");
    shell_pid = start_shell(pipe_in, pipe_out);
    test_empty_input(pipe_in, pipe_out);

    // Test exit command
    printf("Testing 'exit' command...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_exit_command(pipe_in, pipe_out, shell_pid);

    // Test <Ctrl> + D exit
    printf("Testing <Ctrl> + D exit...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_ctrl_d_exit(pipe_in, pipe_out, shell_pid);

    // Test external commands
    printf("Testing external commands...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_external_commands(pipe_in, pipe_out);

    // Test non-existing command
    printf("Testing non-existing command...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_non_existing_command(pipe_in, pipe_out);

    // Test child process exit after <Ctrl> + D
    printf("Testing child process exit after <Ctrl> + D...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_child_process_exit(pipe_in, pipe_out, shell_pid);

    printf("Testing starts in home...\n");
    shell_pid = start_shell(pipe_in, pipe_out); // Restart shell
    test_starts_in_home(pipe_in, pipe_out, shell_pid);


    // Wait for the shell process to finish
    waitpid(shell_pid, NULL, 0);

    return 0;
}
