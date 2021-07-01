//
// Created by antoniosalsi on 01/07/21.
//

#pragma once
#include <string>
#include <memory>

namespace hgardenpi {

    inline namespace v1 {

        using std::string;
        using std::unique_ptr;

        struct DeviceInfo {

            typedef unique_ptr<DeviceInfo> Ptr;

            string hardhare;
            string revision;
            string serial;
            string model;
        };

    }
}