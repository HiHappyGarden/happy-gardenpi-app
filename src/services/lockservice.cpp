
/*
MIT License

Copyright (c) 2021 Happy GardenPI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "lockservice.hpp"

#include <unistd.h>

#include <filesystem>
using namespace std;

#include "../config.h"

using hgardenpi::v1::LockService;

bool LockService::lock() noexcept
{

    ifstream lockFileCheck(HGARDENPI_FILE_LOCK_PATH);
    if ( lockFileCheck && !lockFileCheck.good())
    {
        lockFile.open (HGARDENPI_FILE_LOCK_PATH);
        lockFile << std::to_string(getpid());
        lockFile.flush();
        lockFileCheck.close();
        return false;
    }

    // get length of file:
    lockFileCheck.seekg (0, ifstream::end);
    streamoff length = lockFileCheck.tellg();
    lockFileCheck.seekg (0, ifstream::beg);

    char * buffer = new(nothrow) char [length];

    // read data as a block:
    lockFileCheck.read (buffer, static_cast<int>(length));

    // convert string to pid_t
    stringstream ss;
    ss << buffer;
    ss >> pidInExecution;

    delete[] buffer;

    lockFileCheck.close();
    return true;
}

void LockService::release() noexcept
{
    if (lockFile && lockFile.is_open())
    {
        lockFile.close();
    }
    remove(HGARDENPI_FILE_LOCK_PATH);
}
