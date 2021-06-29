//
//  cppcommons
//
//  Created by Antonio Salsi on 21/03/2020.
//  Copyright © 2020 Antonio Salsi. All rights reserved.
//

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
