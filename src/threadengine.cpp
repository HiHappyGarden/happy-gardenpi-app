// MIT License
//
// Copyright (c) 2021. Happy GardenPI
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
//

//
// Created by Antonio Salsi on 29/07/21.
//

#include "threadengine.hpp"

#include <unistd.h>
#include <sys/syscall.h>

#include "utilities/stringutils.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief exec command
         * @param cmd command to throw
         * @return result
         */
        static vector<string> exec(string &&cmd);

#pragma region staticVariables
        static constexpr const inline auto tick = static_cast<size_t>(Time::TICK);
#pragma endregion staticVariables

#pragma region ThreadPool

        ThreadPool::ThreadPool(size_t threads)
                : stop(false),
                  threads(threads)
        {
            for (size_t i = 0; i < threads; ++i)
                workers.emplace_back(
                        [this]
                        {
                            for (;;)
                            {
                                function<void()> task;

                                {
                                    unique_lock<mutex> lock(this->queueMutex);
                                    this->condition.wait(lock,
                                                         [this]
                                                         { return this->stop || !this->tasks.empty(); });
                                    if (this->stop && this->tasks.empty())
                                        return;
                                    task = move(this->tasks.front());
                                    this->tasks.pop();
                                }

                                task();
                            }
                        }
                );
        }

        ThreadPool::~ThreadPool() noexcept
        {
            {
                unique_lock<mutex> lock(queueMutex);
                stop = true;
            }
            condition.notify_all();
            for (thread &worker: workers)
                worker.join();
        }

        ThreadPool *threadPool = nullptr;
#pragma endregion ThreadPool

#pragma region variables
        sigset_t sigset;
        atomic_bool shutdownRequest(false);
        condition_variable cv;
        long pidMain;


        //exit signal handler
        function<int()> threadSignalHandler = []
        {

            int signum = 0;

            // wait until a signal is delivered:
            sigwait(&sigset, &signum);
            shutdownRequest.store(true);

            cout << "signum:" << to_string(signum) << endl;

            for (auto &&pid : threadGetChildPid(syscall(__NR_gettid)))
            {
                exec("kill -9 " + to_string(pid));
            }

            if (threadPool)
            {
                cout << "delete threadPool" << endl;
                delete threadPool;
                threadPool = nullptr;
            }
            // notify all waiting workers to check their predicate:
            cv.notify_all();
            return signum;
        };
#pragma endregion variables

#pragma region functions

        static vector<string> exec(string &&cmd) {
            array<char, 128> buffer{};
            vector<string> result{};
            unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
            if (!pipe) {
                throw runtime_error("popen() failed!");
            }
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                string data = buffer.data();
                result.emplace_back(move(data.substr(0, data.size() - 1)));
            }
            return result;
        }

        void threadSleep(Time &&millis) noexcept //keep not inline
        {
            this_thread::sleep_for(chrono::milliseconds(static_cast<size_t>(millis)));
        }

        void threadSleep(const Time &millis) noexcept //keep not inline
        {
            this_thread::sleep_for(chrono::milliseconds(static_cast<size_t>(millis)));
        }

        vector<long> threadGetChildPid(long exclude) noexcept
        {

            vector<long> ret{};
            for (auto &&row : exec("ps -eLf | grep hgardenpi")) {

                auto &&rowSplitted = split(row, ' ');
                decltype(rowSplitted) rowWithNotEmplyData{};

                copy_if (rowSplitted.begin(), rowSplitted.end(), back_inserter(rowWithNotEmplyData), [](auto str){return !str.empty();} );

                long parent = atol(rowWithNotEmplyData[0].c_str());
                long child = atol(rowWithNotEmplyData[3].c_str());

                if (exclude > -1 && exclude == child) {
                    continue;
                }

                if (parent != pidMain && child > parent && pidMain != child)
                {
                    ret.push_back(child);
                }
            }

            return ret;
        }

#pragma endregion functions
    }
}