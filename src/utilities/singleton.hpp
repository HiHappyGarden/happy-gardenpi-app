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

#pragma once

#include <memory>
#include <mutex>

namespace hgardenpi
{

    inline namespace v1
    {

        using std::forward;
        using std::once_flag;
        using std::unique_ptr;

        template <typename T>
        class Singleton
        {
        protected:
            Singleton<T>() noexcept = default;
            virtual ~Singleton<T>() noexcept = default;

            static unique_ptr<T> instance;
            static once_flag once;

        public:
            Singleton<T>(const Singleton<T> &) = delete;
            Singleton<T> &operator=(const Singleton<T> &) = delete;
            Singleton<T>(Singleton<T> &&) = delete;
            Singleton<T> &operator=(Singleton<T> &&) = delete;

            static T *getInstance() noexcept
            {
                call_once(once, []()
                          { instance.reset(new T); });
                return instance.get();
            }

            template <typename... Args>
            static T *getInstance(Args &&...args) noexcept
            {
                call_once(once, [&]()
                          { instance.reset(new T(forward<Args>(args)...)); });
                return instance.get();
            }
        };

        template <typename T>
        unique_ptr<T> Singleton<T>::instance;
        template <typename T>
        once_flag Singleton<T>::once;

    }
}
