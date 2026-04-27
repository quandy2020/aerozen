\page introduction Introduction

Next Tutorial: \ref installation

## What is Aerozen?

Aerozen is an open source communication library that allows sharing
data between clients. In our context, a client is called a node. Nodes might
be running within the same process on the same machine or on separate machines
located around the world. Aerozen platform (Linux), so all the low level details, such as data alignment or
endianness, are hidden for you.

Aerozen uses
[Google Protocol buffers](https://developers.google.com/protocol-buffers/?hl=en)
as the data type for communicating between nodes. Users can define their own
messages using the Protobuf utils, and then exchange them between the nodes.
Aerozen discovers, serializes and delivers messages to the
destinations using a combination of custom code and a middleware backend.
Currently, two middleware backends are available:
[ZeroMQ](http://zeromq.org/) and [Zenoh](https://zenoh.io/).

## How do I specify my middleware backend?

Aerozen support an environment variable `AEROZEN_IMPLEMENTATION`
that allows the user to select the middleware backend to use.

`zeromq` is the default middleware backend and the recommended option for
production code.

Please, review the \ref envvars section to learn about the environment variables
that can be used with each middleware backend.

## What programming language can I use with Aerozen?

C++ is the native implementation and the only language that has all available
library features. Python implementation is a wrapper around C++ methods using
`pybind11`. It does not support all features like C++, but contains the main
features such as publication, subscription and service request.
