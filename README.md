# crutils

[![Travis](https://img.shields.io/travis/alehaa/crutils/master.svg?style=flat-square)](https://travis-ci.org/alehaa/crutils) [![Codecov](https://img.shields.io/codecov/c/github/alehaa/crutils.svg?style=flat-square)](https://codecov.io/github/alehaa/crutils?branch=master)  [![](https://img.shields.io/github/issues-raw/alehaa/crutils.svg?style=flat-square)](https://github.com/alehaa/crutils/issues)
[![GPL license](http://img.shields.io/badge/license-LGPL-blue.svg?style=flat-square)](http://www.gnu.org/licenses/)

Some barcode readers emulate a HID (keyboard), others communicate via RS232, ... To get a uniform way to connect to these without huge code overheader, crutils should help to provide a simple API for barcode reader communications.


## Configuration

Before usage you must provide a valid configuration file for crutils. Otherwise libcodereader will not find your barcode reader. Providing a configuration file is very simple: just add a line with device file and required driver separated by space to ``codereader.conf`` in your configuration directory:
```
/dev/input/event10 lxinput
```

It is recommended to provide a symlink for your barcode reader via a udev rule. Depending on the device it may be required to grant the user rights to access this device. You may add a group like ``codereader`` and put all your users into it:
```
SUBSYSTEM=="input", ATTRS{idVendor}=="05fe", ATTRS{idProduct}=="1010", GROUP="codereader", MODE="660", SYMLINK+="input/barcode0"
```
Don't forget to change your configuration file:
```
/dev/input/barcode0 lxinput
```


## Usage

cruilts comes with a binary called ``codereader``. You can use it in shell scripts to get barcodes. In normal mode it will print received barcodes in an endless loop.
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

For C applications libcodereader provides the codereader API. Simply create a new ``FILE *`` handle with ``codereader_open()`` to use it with functions like ``fgets`` to receive your barcodes like you do via ``stdin`` - that's it.

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


## Drivers

Currently the following drivers are supported:

|name|description|
|-------|------------|
|lxinput|Parse Linux kernel event files.|


## Contribute

Anyone is welcome to contribute. Simply fork this repository, make your changes **in an own branch** and create a pull-request for your change. Please do only one change per pull-request.

You found a bug? Please fill out an issue and include any data to reproduce the bug.

#### Contributors

[Alexander Haase](https://github.com/alehaa)


## License

crutils is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

crutils is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**. A Copy of the GPL can be found in the [LICENSE](LICENSE) file.

Copyright &copy; 2013-2017 Alexander Haase
