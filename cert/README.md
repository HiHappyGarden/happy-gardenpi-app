# Build certs
Follow this steps for generate a cert

## For server side
* sudo -i
* openssl genrsa -des3 -out /etc/mosquitto/certs/ca.key 2048
* openssl req -new -x509 -days 1826 -key /etc/mosquitto/certs/ca.key -out ca.crt
* openssl genrsa -out /etc/mosquitto/certs/server.key 2048
* openssl req -new -out /etc/mosquitto/certs/server.csr -key server.key
* openssl x509 -req -in /etc/mosquitto/certs/server.csr -CA /etc/mosquitto/certs/ca.crt -CAkey /etc/mosquitto/certs/ca.key -CAcreateserial -out /etc/mosquitto/certs/server.crt -days 3600
* edit /etc/mosquitto/conf.d/default.conf
```
#SSL
listener 8883

cafile /etc/mosquitto/certs/ca.crt
keyfile /etc/mosquitto/certs/server.key
certfile /etc/mosquitto/certs/server.crt
```
* systemctl restart mosquitto

## For client side
* openssl genrsa -out /etc/mosquitto/certs/client.key 2048
* openssl req -new -out /etc/mosquitto/certs/client.csr -key /etc/mosquitto/certs/client.key
* openssl x509 -req -in /etc/mosquitto/certs/client.csr -CA /etc/mosquitto/certs/ca.crt -CAkey /etc/mosquitto/certs/ca.key -CAcreateserial -out /etc/mosquitto/certs/client.crt -days 3600

## Create cert to import

* create file cert/ca-crt.hpp
```C++
#pragma once

#include <stdint.h>

namespace hhg
{
inline namespace v1
{

namespace
{
constexpr const char CA_CRT[] = R"(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)";
};

}
}
```
* create file client-crt.hpp
```C++
#pragma once

#include <stdint.h>

namespace hhg
{
inline namespace v1
{

namespace
{
constexpr const char CLIENT_CRT[] = R"(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)";
};

}
}
```
* create file client-key.hpp
```C++
#pragma once

#include <stdint.h>

namespace hhg
{
inline namespace v1
{

namespace
{
constexpr const char CLIENT_KEY[] = R"(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)";
};

}
}
```