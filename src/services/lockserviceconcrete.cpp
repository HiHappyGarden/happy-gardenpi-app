// MIT License
//
// Copyright (c) 2021 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "lockserviceconcrete.hpp"

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

#include "../config.h"

using hgardenpi::v1::LockServiceConcrete;

bool LockServiceConcrete::lock() noexcept
{
    ifstream lockFileCheck(configInfo->fileLock);

#if HGARDENPI_TEST > 0
        return false;
#else
    //check if exist pid file
    if (!lockFileCheck.good())
    {
        ofstream lockFile(configInfo->fileLock);
        lockFile << std::to_string(getpid()) << endl;
        lockFile.close();
        lockFileCheck.close();
        return true;
    }
#endif


    string line;
    while (getline(lockFileCheck, line))
    {

        // convert string to pid
        stringstream ss;
        ss << line;
        ss >> LockService::pidInExecution;
    }

    lockFileCheck.close();
    return true;
}

void LockServiceConcrete::release() const noexcept
{
    ifstream lockFileCheck(configInfo->fileLock);
    if (lockFileCheck.good())
    {
        if (pidInExecution == 0)
            remove(configInfo->fileLock.c_str());
    }
    lockFileCheck.close();
}
