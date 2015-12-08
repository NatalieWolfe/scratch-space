
#include <cstdio>
#include <iostream>
#include <unistd.h>

int main(void){
    int pipe_fd[2];
    ::pipe(pipe_fd);

    const char write_buf[] = "Hello, World!";
    char read_buf[sizeof(write_buf) + 1] = { 0 };

    int written_bytes = ::write(pipe_fd[1], write_buf, sizeof(write_buf));
    ::fsync(pipe_fd[1]);

    int read_bytes = ::read(pipe_fd[0], read_buf, sizeof(write_buf));

    ::close(pipe_fd[1]);
    ::close(pipe_fd[0]);

    std::cout << written_bytes << " bytes written of " << sizeof(write_buf) << std::endl;
    std::cout << read_bytes << " bytes read for " << (const char*)read_buf << std::endl;

    return 0;
}
