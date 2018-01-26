# C-Bowl
## Bayan Berri, Brian Lim, & Gabi Newman -- pd10

### Description

C-Bowl is a multiplayer quiz bowl style trivia game. This game allows groups of 2-4 players, each on their own machine, to connect to a single server. They then race to answer trivia questions. Whoever submits the correct answer to a question first wins points. After ten questions, the game ends and whoever has the most points wins.

### Launch Instructions

```
$ git clone git@github.com:bberri1205/final_sys.git
Cloning into 'final_sys'...
remote: Counting objects: 338, done.
remote: Compressing objects: 100% (115/115), done.
remote: Total 338 (delta 118), reused 108 (delta 58), pack-reused 165
Receiving objects: 100% (338/338), 71.52 KiB | 0 bytes/s, done.
Resolving deltas: 100% (209/209), done.
Checking connectivity... done.

$ make
gcc -g -c start_screen.c select_client.c
gcc -g -c networking.c
gcc -g -o client select_client.o networking.o
gcc -g -c start_screen.c select_server.c
gcc -g -o server select_server.o networking.o\
```
After running make open at least two other terminals. (3 total)

On terminal[0] run the server:

```
$ ./server n (where n is the number of clients you want, if you don't include n we assume 2)
[server] socket created
[server] socket bound
[server] socket in listen state
```

on all other terminals run clients and submit your name:

```
$ ./client
[SERVER BROADCAST] type your name

enter data: 4
hello 4

[SERVER BROADCAST] game will start when all players join
```
#### After signing in

After all clients (n) have connected to the server and signed in with their names they are prompted with science questions and whoever answers the most correctly first wins the game. they are all multiple choice questions and the answer you input has to be 1, 2, 3, or 4. Failure to do so means you don't get the points! Each player gets one try per question to avoid cheating. players get 109 points for answering a question correctly.

### Running on Multiple Machines

Follow the above instructions to clone and make.
Then, run the server one machine. Again, pass the number of desired clients as an argument. The default is 2.

```
$ ./server 2
[server] socket created
[server] socket bound
[server] socket in listen state
```
Now it is time to run the clients on at least two other machines. When you run the client, you must pass the server's IP address as an argument in order to connect. Then, enter your name as before.

```
$ ./client SERVER_IP
[SERVER BROADCAST] type your name

enter data: bayan
hello bayan
```
Play the game as previously specified.

### Documentation 
Most of our documentation can be found in our textfile titled fxns.txt


