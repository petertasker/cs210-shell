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

    // Debugging: Print the captured output
    printf("Debug: Shell output = '%s'\n", buffer);

    // Check if the prompt appears at least once in the output
    if (strstr(buffer, "$") != NULL) {
        printf("Test passed: Shell correctly handles empty input.\n");
    } else {
        printf("Test failed: Shell did not reprint the prompt after empty input.\n");
    }

    close(pipe_out[0]); // Close the read end of the output pipe
}

int main() {
    int pipe_in[2], pipe_out[2];
    pid_t shell_pid;

    // Start the shell process
    shell_pid = start_shell(pipe_in, pipe_out);

    // Test empty input handling
    test_empty_input(pipe_in, pipe_out);

    // Wait for the shell process to finish
    waitpid(shell_pid, NULL, 0);

    return 0;
}
