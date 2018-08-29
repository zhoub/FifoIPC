#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <memory>

int main(int Argc, char *Argv[])
{
    std::cout << "RUNNING TX " << std::endl;
    for (int i = 0; i < Argc; ++ i)
    {
        std::cout << "\t" << Argv[i] << std::endl;
    }

    int WriteFd = atoi(Argv[0]);
    std::cout << "USING PIPE " << WriteFd << std::endl;

    FILE *PipeWriteFile = fdopen(WriteFd, "w");
    if (!PipeWriteFile)
    {
        std::cerr << "FAILED TO OPEN PIPE TO WRITE" << std::endl;

        return 1;
    }

    char *Buffer = new char[1024 * 1024];
    for (int i = 0; i < 32; ++ i)
    {
        memcpy(Buffer, &i, sizeof(int));

        // Generate random sized data.
        //
        int Length = static_cast<float>(random()) / static_cast<float>(RAND_MAX) * 1024 * 1024;

        std::cout << "WRITING " << i << ' ' << Length << std::endl;

        memcpy(Buffer + sizeof(int), &Length, sizeof(int));

        memset(Buffer + sizeof(int) * 2, i * 2, Length);

        // Send data to the RX.
        //
        fwrite(Buffer, sizeof(int) * 2 + Length, 1, PipeWriteFile);
        fflush(PipeWriteFile);
    }
    delete [] Buffer;

    fclose(PipeWriteFile);

    close(WriteFd);

    return 0;
}

