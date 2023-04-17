#include <driver/socket_err.h>
#include <socket.h>

#define IPC_PORT (driver_port() + 1)
#define IPC_INTERVAL 30

inherit M_CLEAN;

private void setup_ipc ();
private void monitor_ipc ();
private void authenticate_ipc (int socket);
private void read_socket (int s, mixed message, string addr);
private void listen_socket (int fd);
private void close_socket (int fd);

nosave private int *__Clients = ({});
nosave private int __Socket;

// -----------------------------------------------------------------------------

void create () {
    set_no_clean(1);
    setup_ipc();
    call_out((: monitor_ipc :), IPC_INTERVAL);
}

void handle_remove () {
    foreach (int client in __Clients) {
        socket_close(client);
    }
}

// -----------------------------------------------------------------------------

private void setup_ipc () {
    int socket;

    __Socket = socket_create(SOCKET_STREAM, "read_socket", "close_socket");
    if (__Socket < 0) {
        error("Bad response socket_create: " + socket_error(__Socket));
    }
    if ((socket = socket_bind(__Socket, IPC_PORT)) < 0) {
        error("Bad response from socket_bind: " + socket_error(socket));
    }
    if ((socket = socket_listen(__Socket, "listen_socket")) < 0) {
        error("Bad response from socket_listen: " + socket_error(socket));
    }
}

private void monitor_ipc () {
    if (!socket_status(__Socket)) {
        setup_ipc();
    }
    call_out((: monitor_ipc :), IPC_INTERVAL);
}

private void authenticate_ipc (int socket) {
    string addr = socket_address(socket);
    if (addr != "127.0.0.1" && addr != "0.0.0.0") {
        socket_close(socket);
        error("Bad socket request from " + addr);
    }
}

void send_to_sockets (string message) {
    foreach (int client in __Clients) {
        int socket = socket_write(client, message);
        if (socket < 0) {
            error("Bad response from socket_write: " + socket_error(socket));
        }
    }
}

// -----------------------------------------------------------------------------

private void read_socket (int fd, mixed message) {
    if (!stringp(message)) {
        socket_close(fd);
        error("Bad message from read_socket");
    }

    authenticate_ipc(fd);

    debug_message("D_IPC->read_socket received " + message);
    /*
    // Examples:
    // CHAT:chan:pos:nick:msg
    // GEN:name:/save/gen/Name/file.txt

    if (message[0..4] == "CHAT:") {
        // string chan, pos, nick, msg;
        // if (sscanf(data, "CHAT:%s:%s:%s:%s", chan, pos, nick, msg) != 4) {
        //     debug("Malformed chat msg:" + socket_address(fd) + ":" + data);
        //     return;
        // }
        // CHAT_D->ipc_chat(chan, nick, msg);
    } else if (message[0..3] == "GEN:") {
        // do something
    }
    */
}
void write_socket(int fd) {

}

private void listen_socket (int fd) {
    int socket = socket_accept(fd, "read_socket", "write_socket");

    authenticate_ipc(fd);

    if (socket < 0) {
        error("Bad response to socket_accept: " + socket_error(socket));
    }
    __Clients += ({ socket });
}

private void close_socket (int fd) {
    __Clients -= ({ fd });
}