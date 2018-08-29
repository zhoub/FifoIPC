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
    // Create the FIFO.
    //
    const char *kFifoPath = "/tmp/PipeIPC";

    std::cout << "RX: ABOUT TO CREATE TX " << kFifoPath << std::endl;
    if (mkfifo(kFifoPath, S_IRUSR | S_IWUSR))
    {
        std::cerr << "RX: BAD FIFO" << std::endl;

        return 1;
    }

    std::cout << "RX: GOT FIFO AT " << kFifoPath << std::endl;

    // Spawn the TX process.
    //
    std::cout << "RX: SPAWN TX" << std::endl;

    char Cmd[256];
    memset(Cmd, '\0', 256);
    sprintf(Cmd, "TX %s &", kFifoPath);

    if (system(Cmd) < 0)
    {
        std::cerr << "FAILED TO SPAWN TX" << std::endl;

        return 1;
    }

    // Read the data until error happens.
    //
    int ReadFd = open(kFifoPath, O_RDONLY);
    if (!ReadFd)
    {
        std::cerr << "FAILED TO OPEN PIPE AS FILE" << std::endl;

        return 1;
    }

    // Read loop.
    //
    char *Buffer = reinterpret_cast<char *>(malloc(1024 * 1024));
    while (read(ReadFd, Buffer, sizeof(int) * 2) == sizeof(int) * 2)
    {
        int Header[2] = {-1, 0};
        memcpy(Header, Buffer, sizeof(int) * 2);
        std::cout << "READ HEADER " << Header[0] << ' ' << Header[1] << std::endl;

        if (read(ReadFd, Buffer, Header[1]) == Header[1])
            std::cout << "\t" << "READ DATA " << Header[1] << std::endl;
    }
    free(Buffer);

    // Close our read fd.
    //
    close(ReadFd);

    return 0;
}
