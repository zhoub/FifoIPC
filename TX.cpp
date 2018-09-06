#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

int main(int Argc, char *Argv[])
{
    std::cout << "TX: RUNNING" << std::endl;

    const char *FifoPath = Argv[1];
    std::cout << "TX: USING FIFO " << FifoPath << std::endl;

    int WriteFd = open(FifoPath, O_WRONLY | O_SYNC);
    if (WriteFd == -1)
    {
        std::cerr << "FAILED TO OPEN PIPE TO WRITE" << std::endl;

        return 1;
    }

    std::cout << "TX: ABOUT TO WRITE" << std::endl;

    char *Buffer = new char[1024 * 1024];
    for (int i = 0; i < 32; ++ i)
    {
        // Set index.
        //
        memcpy(Buffer, &i, sizeof(int));

        // Generate random sized data.
        //
        int Length = static_cast<float>(random()) / static_cast<float>(RAND_MAX) * 1024 * 1024;
        memcpy(Buffer + sizeof(int), &Length, sizeof(int));

        memset(Buffer + sizeof(int) * 2, i * 2, Length);

        // Send data to the RX.
        //
        std::cout << "TX: WRITING " << i << ' ' << Length << std::endl;

        write(WriteFd, Buffer, sizeof(int) * 2 + Length);
    }
    delete [] Buffer;

    close(WriteFd);

    return 0;
}

