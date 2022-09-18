# transfer

## Install

```
git clone https://github.com/podpolkovnik/transfer.git
cd transfer
make
```
## Usage

### Run params
```
-s // run transfer as a server
-c // run transfer as a client
-m // message to send to the client
-t // frequency of sending messages to the client (in seconds)

```
### Examples
Start the server to send "Hello!" messages to the client once every 10 seconds
```
./transfer -s -m "Hello!" -t 10
```
Start the client to receive messages from the server
```
./transfer -c
```
