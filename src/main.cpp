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

#include <iostream>

#include "engine.hpp"
#include "utilities/threadutils.hpp"

int main(int argc, char *argv[])
{

    using namespace std;

    setlocale(LC_ALL, "");
    bindtextdomain(HGARDENPI_NAME, "./locale");
    textdomain(HGARDENPI_NAME);

    try
    {
        //initialize Happy GardenPI
        hgardenpi::initialize();

        //start loop
        hgardenpi::start();
        
        //set signal behavior on SIGINT SIGTERM
        sigemptyset(&hgardenpi::sigset);
        sigaddset(&hgardenpi::sigset, SIGINT);
        //            sigaddset(&sigset, SIGQUIT);
        //            sigaddset(&sigset, SIGKILL);
        sigaddset(&hgardenpi::sigset, SIGTERM);
        pthread_sigmask(SIG_BLOCK, &hgardenpi::sigset, nullptr);

        //program not exit until interrupt trig SIGINT or SIGTERM
        auto &&signalHandler = async(launch::async, hgardenpi::threadSignalHandler);

        auto &&signal = signalHandler.get();

        cout << HGARDENPI_NAME << _(" shuting down, signal:") << to_string(signal) << endl;
    }
    catch (const std::exception &e)
    {
        cerr << "error: " << e.what() << std::endl;

        hgardenpi::Engine::getInstance()
                ->getFactory()
                ->getSystem()
                ->getLogService()
                ->write(LOG_ERR, e.what());

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}