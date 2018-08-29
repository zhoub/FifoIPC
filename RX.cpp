#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

int main(int Argc, char *Argv[])
{
    int PipeFds[2] = {0, 0};
    if (pipe(PipeFds) == -1)
    {
        std::cerr << "BAD PIPE" << std::endl;

        return 1;
    }

    std::cout << "GOT 2 FD " << PipeFds[0] << ' ' << PipeFds[1] << std::endl;

    // Create the RX, and tive the file descriptor to it.
    //
    char ChildArgStr[256];
    memset(ChildArgStr, '\0', 256);
    sprintf(ChildArgStr, "%d", PipeFds[1]);

    std::cout << "ABOUT TO CREATE TX " << ChildArgStr << std::endl;

    if (execl("TX", ChildArgStr, NULL) == -1)
    {
        std::cerr << "FAILED TO CREATE TX PROCESS" << std::endl;

        return 1;
    }

    // Read the data until error happens.
    //
    FILE *PipeReadFile = fdopen(PipeFds[0], "r");
    if (!PipeReadFile)
    {
        std::cerr << "FAILED TO OPEN PIPE AS FILE" << std::endl;

        return 1;
    }

    std::cout << "ABOUT TO READ" << std::endl;

    char *Buffer = reinterpret_cast<char *>(malloc(1024 * 1024));
    while (!feof(PipeReadFile))
    {
        int Header[2] = {-1, 0};
        fread(Header, sizeof(Header), 1, PipeReadFile);
        std::cout << "READ HEADER " << Header[0] << ' ' << Header[1] << std::endl;

        if (fread(Buffer, Header[1], 1, PipeReadFile) == Header[1])
            std::cout << "\t" << "READ DATA " << Header[1] << std::endl;
    }
    free(Buffer);

    // Close our read fd.
    //
    fclose(PipeReadFile);

    close(PipeFds[0]);

    return 0;
}
