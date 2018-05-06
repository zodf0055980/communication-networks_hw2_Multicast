# communication-networks_hw2_Multicast

# how to use
## multicast
change the server.c and client.c #define localip.
if you want to change the multicastip , change the server.c and client.c #define multicastip.
```
$ gcc -o server server.c
$ gcc -o client client.c
$ ./client
$ ./server pass_filename
```
## multicast
change the client_thread.c #define localip.
```
$ gcc -o server server.c -lpthread
$ gcc -o client client.c
$ ./server pass_filename
$ ./client
```
