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

#include "deviceservice.hpp"

#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <iostream>
#include <iosfwd>
#include <fstream>
#include <thread>
#include <iomanip>
using namespace std;

#include <hgardenpi-protocol/utilities/stringutils.hpp>
using namespace hgardenpi::protocol;

namespace hgardenpi
{
    inline namespace v1
    {

        const inline constexpr char *IFR_NAME = "wlan0";

        float getCPUTemperature()
        {
            string val;
            ifstream file("/sys/class/thermal/thermal_zone0/temp");
            file >> val;
            file.close();
            return stof(val) / 1'000.0f;
        }

        DeviceInfo::Ptr getDeviceInfo()
        {
            auto device = unique_ptr<DeviceInfo>(new (nothrow) DeviceInfo{"no info", "no info", "no info", "no info", cpu : thread::hardware_concurrency()});
            ifstream file("/proc/cpuinfo");
            string line;
            while (getline(file, line))
            {
                if (stringContain(line, "Hardware"))
                {
                    device->hardware = move(stringRight(line, ":", 2));
                }
                if (stringContain(line, "Revision"))
                {
                    device->revision = move(stringRight(line, ":", 2));
                }
                if (stringContain(line, "Serial"))
                {
                    device->serial = move(stringRight(line, ":", 2));
                }
                if (stringContain(line, "Model"))
                {
                    device->model = move(stringRight(line, ":", 2));
                }
            }
            file.close();
            return device;
        }

        string getWlan0IP()
        {
            int fd;
            struct ifreq ifr{};

            fd = socket(AF_INET, SOCK_DGRAM, 0);

            /* I want to get an IPv4 IP address */
            ifr.ifr_addr.sa_family = AF_INET;

            /* I want IP address attached to "eth0" */
            strncpy(ifr.ifr_name, IFR_NAME, IFNAMSIZ-1);

            ioctl(fd, SIOCGIFADDR, &ifr);

            close(fd);

            return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

        }

        string getWlan0MAC()
        {
            string ret;
            struct ifreq s{};
            int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

            strcpy(s.ifr_name, IFR_NAME);
            if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
                int i;
                stringstream ss;
                for (i = 0; i < 6; ++i)
                {
                    ss << setfill('0') << setw(2) << hex << (0xff & (unsigned int)s.ifr_addr.sa_data[i]);
                }

                ret = move(ss.str());
            }
            return ret.substr(0, ret.size() - 1);
        }

    }
}