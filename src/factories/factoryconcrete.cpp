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

#include "factoryconcrete.hpp"

#include <stdexcept>
using namespace std;

#include "systemconcrete.hpp"
#include "deviceconcrete.hpp"

namespace hgardenpi
{

    inline namespace v1
    {
        FactoryConcrete::FactoryConcrete() : system(new (nothrow) SystemConcrete), device(new (nothrow) DeviceConcrete)
        {
            if (!system)
            {
                throw runtime_error(_("no memory for SystemConcrete"));
            }
            if (!device)
            {
                throw runtime_error(_("no memory for DeviceConcrete"));
            }
        }

        inline FactoryConcrete::~FactoryConcrete() noexcept
        {
            if (system)
            {
                delete system;
            }
            if (device)
            {
                delete device;
            }
        }
    }
}