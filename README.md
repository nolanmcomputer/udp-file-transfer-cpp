# UDP File Transfer in C++

This project contains two C++ programs:

- `sender.cc` — reads the contents of `file.txt` and sends them over the network
- `receiver.cc` — listens for incoming data and writes the received contents into `file.txt`

The intended use is to run `receiver.cc` on one computer and `sender.cc` on another computer on the same network or across reachable hosts.

## Overview

This is a simple UDP-based file transfer example written in C++. The sender reads data from a local file and transmits it in chunks. The receiver binds to a UDP port, waits for incoming packets, and writes the received bytes to an output file.

### Important note

This implementation uses **UDP**, which is connectionless and does **not** guarantee:

- delivery
- packet order
- duplicate protection
- retransmission on loss

## Files

- `sender.cc`
- `receiver.cc`
- `file.txt` — the input file on the sender side, and the output file on the receiver side

## Features

- Sends file contents in fixed-size chunks
- Uses standard Berkeley sockets
- Supports command-line destination host and destination port for the sender
- Receiver supports selecting IPv4 or IPv6 mode with a runtime flag (IPv6 currently not functioning)

## Requirements

- Linux or Unix-like environment
- `g++`
- Standard socket libraries available through the system headers

## Compilation

Compile each program separately.

### Compile the receiver

```bash
g++ receiver.cc -o receiver
```

### Compile the sender

```bash
g++ sender.cc -o sender
```

## Running the programs

### 1. Start the receiver

Run the receiver first on the destination machine.

```bash
./receiver -4 (or -6 for IPv6 mode)
```

### Receiver behavior

- creates a UDP socket
- binds to port `8081`
- waits for incoming packets
- writes received data to `file.txt`

### 2. Run the sender

Run the sender on the source machine and provide:

1. destination IP address or hostname
2. destination port number

#### Example

```bash
./sender 192.168.1.50 8081
```

### Sender behavior

- expects exactly two command-line arguments
- opens `file.txt` in binary mode
- reads the file in chunks of 512 bytes
- sends each chunk to the destination host and port

## Input and output files

### Sender side

The sender reads from:

```text
file.txt
```

This file must exist in the same directory where `sender` is run.

### Receiver side

The receiver writes to:

```text
file.txt
```

## Troubleshooting

### Receiver says file cannot be opened

Make sure the receiver has write permission in the current directory.

### Sender says file cannot be opened

Make sure `file.txt` exists in the sender's current working directory.

### No data is received

Check the following:

- the receiver is running first
- the sender is using the correct IP address
- the sender is using port `8081` unless receiver code is changed
- firewall rules are not blocking UDP traffic

## Exit behavior

The programs return nonzero values on some error conditions such as:

- incorrect arguments
- socket creation failure
- bind failure
- file open failure
- address resolution failure
