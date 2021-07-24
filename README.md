# Happy GardenPI app

Source code of Happy GardenPI, simple project for the construction of an automated watering can managed by a Pasrperry PI Zero

## Setup

For build the project you must do this:

### 1. Install packages needed

On Ubuntu

```
sudo apt install gdb-multiarch
```

On Rasperry PI W

```
sudo apt install libmosquittopp-dev libcurl4-openssl-dev
```

### 2. Install WiringPi

```
sudo apt update
sudo apt install cmake git
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi/
./build
```

check if the configuration is ready

```
gpio -v
```

In my case return the follow informations:

```
gpio version: 2.60
Copyright (c) 2012-2018 Gordon Henderson
This is free software with ABSOLUTELY NO WARRANTY.
For details type: gpio -warranty

Raspberry Pi Details:
  Type: Pi Zero-W, Revision: 01, Memory: 512MB, Maker: Sony
  * Device tree is enabled.
  *--> Raspberry Pi Zero W Rev 1.1
  * This Raspberry Pi supports user-level GPIO access.
```

### 3 Debug whith Valgrind
```
valgrind --leak-check=full --show-leak-kinds=all  --track-origins=yes  --verbose  --log-file=valgrind-out.txt ./hgardenpi 
```