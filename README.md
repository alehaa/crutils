# crutils

[![Travis](https://img.shields.io/travis/alehaa/crutils/master.svg?style=flat-square)](https://travis-ci.org/alehaa/crutils)
[![Codecov](https://img.shields.io/codecov/c/github/alehaa/crutils.svg?style=flat-square)](https://codecov.io/github/alehaa/crutils?branch=master)
[![](https://img.shields.io/github/issues-raw/alehaa/crutils.svg?style=flat-square)](https://github.com/alehaa/crutils/issues)
[![GPL license](http://img.shields.io/badge/license-LGPL-blue.svg?style=flat-square)](LICENSE)

Crutils provides a uniform way to get barcodes from different barcode readers.


# About

Some barcode readers emulate a HID (keyboard), some dump the output via serial and others use different protocols. To connect to any of these, you have to implement several APIs in your application and have to extend the code for new protocols.

Crutils provides a uniform API to read barcodes from different devices as simple as reading from `stdin`. Applications don't have to manage the communication with barcode readers anymore and on top, crutils is extendable by *drivers* for future protocols without recompiling your code.


## Configuration

The default configuration file in `etc/codereader.conf` in your install path loads no codereaders. To use barcode readers, you have to add their definitions in this file, or use an alternative configuration file by setting the `CODEREADER_CONFIG` environment variable. The [libconfig](http://www.hyperrealm.com/libconfig/libconfig_manual.html#Configuration-Files) syntax will be used.

Each device is defined as a named variable with an array of options. At least the `driver` field must be set to the name of the driver to use. A simple configuration may look like the following example:

```
test1 = {
  driver = "xinput2";
  match = ["OKE PS2-USB", "Barcode"];
};
```

A detailed description is included in the default configuration file.


## Drivers

Currently the following drivers are supported:

* **lxinput:** Parse Linux kernel input events.

  Config options:
  * `device`: Which device file to be used.

  **Note:** The user must have read *and* write permissions for the device file to grab the device. It is recommended to provide a symlink for your barcode reader via an udev rule and grant the user rights to access this device. You may add a group like `codereader` and put all your users into it:

      SUBSYSTEM=="input", ATTRS{idVendor}=="05fe", ATTRS{idProduct}=="1010", GROUP="codereader", MODE="660", SYMLINK+="input/barcode0"

  **Note:** At the moment numeric-only barcodes are supported only (see issue [#2](https://github.com/alehaa/crutils/issues/2)).

* **xinput:** Grab input devices of your X-session.

  Although most (cheap) barcode readers operate as a normal keyboard, they have one big caveat: If the cursor is not in the target input field, the scanned barcodes get lost and you have to rescan the items after setting the cursor to the right position. This driver will grab selected input devices from the current X-session to get their input, no matter where the cursor is.

  Config options:
  * `match`: An array of strings for devices to match. E.g. `match = ["Barcode"];` will match all input devices having `Barcode` in their name. *You can list all connected devices by `xinput`.*

  **Note:** This driver supports multiple devices. Just add the required match entries in the `match` option for all of your devices.


## Usage

Cruilts comes with a binary called `codereader`. You can use it in shell scripts to get barcodes. In normal mode it will print received barcodes in an endless loop.
```
~$ codereader | tee barcodes.txt
12345
```
You may add an optional argument with the number of barcodes to read:
```
~$ codereader 1
12345
~$
```


## Integration

For C/C++ applications *libcodereader* provides the codereader API. Simply create a new `FILE *` handle with `codereader_open()` to use it with functions like `fgets` to receive your barcodes as input from `stdin` - that's it.

```C
#include <stdio.h>
#include <codereader.h>

int
main() {
    FILE *ch = codereader_open();
    char buffer[32];
    while (fgets(buffer, 32, ch) != NULL)
        printf("%s", buffer);
}
```


## Adding a new driver

As mentioned above, crutils is extendable by `drivers` for any kind of devices. If your device is not supported yet, you may add a new driver in the `src/drivers` directory: Simply add a new subdirectory for your files and build it by calling the CMake function `codereader_add_driver(NAME SOURCES ...)`.

Each driver *must* support the following symbols:

* `int device_open(const config_setting_t *config, void **cookie)` to open the device. The device configuration will be passed and the driver can store a pointer for allocated memory in cookie, which will be passed to the following functions. A file descriptor must be returned.
* `int device_read(int fd, char *buffer, int size, void *cookie)` will be called, if the previously returned file descriptor is ready to read. Even if more than one barcode is available, only one should be written into `buffer`before this function returns. The parameters and return values are identical to the system's `read` function, except `cookie`, which is the pointer set by open and the special return value `0`, indicating that no barcode has been read, but no error occured (e.g. a new device has been found by the driver).
*  `int device_close(int fd, void *cookie)` to close the device. The parameters and return values are identical to the system's `close` function, except `cookie`, which is the pointer set by `device_open`.


## Contribute

Everyone is welcome to contribute. Simply fork this repository, make your changes *in an own branch* and create a pull-request for your changes. Please send only one change per pull-request.

You found a bug? Please [file an issue](https://github.com/alehaa/crutils/issues/new) and include any data to reproduce the bug.


## License

crutils is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

crutils is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. A Copy of the GPL can be found in the [LICENSE](LICENSE) file.

Copyright &copy; 2013-2017 Alexander Haase
