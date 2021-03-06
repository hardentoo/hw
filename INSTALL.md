Building and installing Hedgewars
=================================

This file explains to you how to build/compile Hedgewars and how to install it.

See also: <https://hedgewars.org/kb/BuildingHedgewars>

Dependencies
------------

### Core dependencies

To compile and install Hedgewars, you need at least:

- A C++ compiler (e.g. GCC)
- CMake >= 2.6.0
- A make program (e.g. GNU Make)
- Free Pascal Compiler (FPC) >= 2.2.4
- Qt = 4.7.0
- SDL >= 2.0
- SDL\_net >= 2.0
- SDL\_mixer >= 2.0
- SDL\_image >= 2.0
- SDL\_ttf >= 2.0

### Optional dependencies

For some additional features, you can optionally install these dependencies:

- For Hedgewars:
    - PhysFS >= 2.0.0 (recommended)
    - Lua = 5.1.0 (recommended)
- For PNG screenshots:
    - libpng >= 1.2 (recommended)
- For video recording:
    - FFmpeg or Libav
- For the Hedgewars Server:
    - GHC >= 6.10
    - Various Haskell packages (see below)

PhysFS will be internally built if `-DPHYSFS_SYSTEM=OFF` is passed to `cmake`
(also allows to set `PHYSFS_LIBRARY` and `PHYSFS_INCLUDE_DIR` if needed).

Lua will be automatically built if not found.

### Hedgewars Server dependencies

The Hedgewars Server is an **optional** separate application.
It provides the online lobby and allows players to create rooms.
You will also be able to launch the server from the frontend
(network play → local network → start server).

**Most players do not need this!**

To compile it, you need:

- Glasgow Haskell Compiler (GHC) >= 6.10
- These Haskell packages:
    - `containers`
    - `vector`
    - `bytestring`
    - `network` >= 2.3
    - `random`
    - `time`
    - `mtl` >= 2
    - `sandi`
    - `hslogger`
    - `process`
    - `deepseq`
    - `utf8-string`
    - `SHA`
    - `entropy`
    - `zlib` >= 0.5.3 and < 0.6
    - `regex-tdfa`


Building
--------

### Summary

To build and install Hedgewars, obtain all dependencies, then run:

   $ cmake .
   $ make
   # make install

### Step 1: Configure

For a default install with all dependencis, use this command:

    $ cmake .

To build with a custom install directory, instead run:

    $ cmake -DCMAKE_INSTALL_PREFIX="<install_prefix>" .

(Replace `<install_prefix>` with the directoy in which you
want Hedgewars to be installed.)

Add the `-DNOSERVER=ON` switch if you do not want to build
the server.

#### CMake options

For more detailed build settings, change some CMake options.
Run `ccmake` for an interactive way to edit them.

Important CMake options:

- `CMAKE_INSTALL_PREFIX`: Installation directory
- `NOSERVER`: Set to `ON` to *not* build the server
- `NOVIDEOREC`: Set to `ON` to *not* build the video recorder
- `SYSTEM_PHYSFS`: Set to `OFF` to use Hedgewars-bundled PhysFS

### Step 2: Make

Run:

    $ make

This creates the following files:

- `bin/hedgewars`: Hedgewars
- `bin/hwengine`: Game engine, can be used to play demos and saved games
- `bin/hedgewars-server`: Hedgewars Server (optional)

### Step 3: Installation

To install Hedgewars to the install directory run:

    # make install

That's all! Enjoy!

Troubleshooting
---------------

### Qt is installed but it can't be found

If this happens, set the following CMake option:

    QT_QMAKE_EXECUTABLE="<path_to_qmake>"

(Replace `<path_to_qmake>` with the path to the `qmake` application.)

If this didn't work, make sure you have the correct Qt version
(see above).

### Hedgewars compiles successfully, but games instantly crash the map preview fails

This is likely to be a problem with PhysFS. Try to build Hedgewars
with the Hedgewars-bundled PhysFS by setting the CMake option
`SYSTEM_PHYSFS=OFF`, then try to run `make` again.

If the _bundled_ PhysFS fails, too, this is likely to be a bug in
Hedgewars, please report at <https://issues.hedgewars.org/>.

### Broken/missing Haskell dependencies

First, try to obtain the missing Haskell packages and make sure GHC
is up-to date, then try again. Read the error messages carefully
to figure out missing package names.

If everything fails and you don't need the server, set the CMake
option `NOSERVER=ON` so the server isn't built at all.

### Error messages related to libavcodec / libavformat

Update Libav or FFmpeg (whatever is present on your system) to
the latest version or install one of them if you haven't already.
Then try to build again.

If this still doesn't work and you give up, set the CMake option
`NOVIDEOREC=ON`, but then the video recording functionality will
not be available.

### Error messages related to Lua, “undefined reference to `lua_tonumber'”, and so on
If you get error messages like these:

* /home/username/hw/hedgewars//uScript.pas:226: undefined reference to `lua_tonumber'
* /home/username/hw/hedgewars/CMakeFiles/hwengine.dir/uScript.o: In function `LUATOVISUALGEARTYPEORD':

There might be something wrong with your Lua installation. Try to install Lua 5.1.
If this doesn't work, or you don't want to install Lua 5.1, try to build Hedgewars
with the bundled Lua version.

To build with the bundled Lua version, adding the CMake option `SYSTEM_LUA=OFF`, then
repeat the building process.

### Cleaning up

In case you want to start over and start with a clean build,
run `make clean`, then go back to step 2.

If things got seriously out of hand, you may want to reset
*everything* (even your configuration). If you use the
Mercural repository, you can run `hg purge --all`. Proceed with
step 1.

### Still can't build Hedgewars?

Visit us in the forums or IRC (see `README.md`) and ask for help.
