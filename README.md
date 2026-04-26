# aerozen
A communication architecture based on Zenoh, built for mass production.

## Dependencies

### Required (core library)

- Protobuf (`protoc`, `libprotobuf`)
- ZeroMQ (`libzmq`)
- CppZMQ
- UUID

### Optional

- zenoh (`zenohc`, `zenohcxx`) for Zenoh transport backend
- IFADDRS for network interface discovery
- AerozenUtils (v4, `cli` component) for `log` / `parameters` components
- SQLite3 (>= 3.7.13) for `log` component
- Python3 Interpreter / Development for Python bindings

### Test dependency

- GTest

## Documentation

- [C++ code style (简体中文)](docs/code-style.md) — formatting and naming conventions for `aerozen/`
- [Docs index](docs/README.md)
