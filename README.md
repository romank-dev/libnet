# libnet
This repository is a basic sockets library, containing utilities and wrapper classes of Linux sockets.
## Building
* **libnet** is built using the [workspace](https://github.com/romank-dev/workspace) build system. 
** For building with a different system, see [this link](https://github.com/romank-dev/workspace?tab=readme-ov-file#deep-dive--building-projects-without-workspace).
* The manifest [URL](https://github.com/romank-dev/libnet/blob/main/manifest.txt) may be used by the Workspace **init.sh** script to pull this project and it's dependencies.

## Overview
**libnet** allows fast development of netowrking applications that require working with sockets.
Currently supported types of sockets/protocols:
* IPv4
  * TCP
  * UDP
* UNIX
  * Stream
  * Datagram

The currently implemented sockets are **blocking** with support for timeout configuration.
