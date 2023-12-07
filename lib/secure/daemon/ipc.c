#include <driver/socket_err.h>
#include <socket.h>

#define IPC_PORT 6668
#define IPC_INTERVAL 30

inherit M_CLEAN;

nosave private int *__Sockets = ({ });
nosave private int __Socket;

/* ----- helpers ----- */

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
    // 0.0.0.0 and 127.0.0.1 are the only allowed connections
    if (!regexp(addr, "0\\.0\\.0\\.0|127\\.0\\.0\\.1 [0-9]+")) {
        socket_close(socket);
        error("Bad socket request from " + addr);
    }
}

void send (string message) {
    foreach (int socket in __Sockets) {
        int s = socket_write(socket, message);
        if (s < 0) {
            error("Bad response from socket_write: " + socket_error(s));
        }
    }
}

/* ----- callbacks ----- */

private void read_socket (int fd, mixed message) {
    if (!stringp(message)) {
        socket_close(fd);
        error("Bad message from read_socket");
    }

    authenticate_ipc(fd);

    /*
    // Examples:
    // CHAT:channel:name:message
    */
    if (message[0..4] == "CHAT:") {
        string chan, nick, msg;
        if (sscanf(message, "CHAT:%s:%s:%s", chan, nick, msg) != 3) {
            error("Malformed IPC chat message from " + socket_address(fd) + ":" + message);
            return;
        }
        D_CHANNEL->send_ipc(chan, nick, msg);
    }

}
void write_socket (int fd) {

}

private void listen_socket (int fd) {
    int socket = socket_accept(fd, "read_socket", "write_socket");

    authenticate_ipc(fd);

    if (socket < 0) {
        error("Bad response to socket_accept: " + socket_error(socket));
    }
    __Sockets += ({ socket });
}

private void close_socket (int socket) {
    __Sockets -= ({ socket });
}

/* ----- lifecycle ----- */

void create () {
    set_no_clean(1);
    setup_ipc();
    call_out((: monitor_ipc :), IPC_INTERVAL);
}

void handle_remove () {
    foreach (int client in __Sockets) {
        socket_close(client);
    }
}