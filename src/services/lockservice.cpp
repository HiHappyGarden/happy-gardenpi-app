
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
#include <sys/file.h>

#include <cerrno>
#include <stdexcept>
using std::runtime_error;

#include "../config.h"

using hgardenpi::v1::LockService;

static const constexpr inline char LOCK_NAME[] = "/run/hgardenpi.pid";

bool LockService::lock()
{
    fd = open(HGARDENPI_FILE_LOCK_PATH, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        throw runtime_error("impossible create lock file");
    }
    pid_t pid = getpid();

    write(fd, (const void *)&pid, sizeof(pid));

    int rc = flock(fd, LOCK_EX | LOCK_NB);
    if (rc)
    {
        if (EWOULDBLOCK == errno)
            return true; // another instance is running
    }
    return false;
}

void LockService::release() const noexcept
{
    if (fd < 0)
        return;
    remove(HGARDENPI_FILE_LOCK_PATH);
}
