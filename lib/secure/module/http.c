#include <socket.h>
#include <driver/socket_err.h>

inherit "/std/json.c";

// https://github.dev/NFA/LPC-httpd

#define UNDEF_REQUEST   -1
#define GET             1
#define POST            2
#define HEAD            3
#define PUT             4
#define DELETE          5
#define TRACE           6
#define CONNECT         7

/* -----  ----- */

nosave private int *__Sockets = ({ });
private nosave int port;
private nosave int __AllowStatic;
private nosave mixed *__URLPatterns;
private nosave string server_root;

/* -----  ----- */

void set_port (int p) {
    port = p;
}
void add_url_pattern (string pattern, string fn) {
    __URLPatterns += ({ ({ pattern, fn }) });
}
void allow_static_pages (int allow) {
    __AllowStatic = !!allow;
}

/* -----  ----- */

protected mapping parse_request (string req) {
    string *headers;
    string *tmp;
    mapping request = ([
        "method": 0,
        "uri": "",
        "protocol": "",
        "header": ([ ]),
        "content": "",
    ]);
    int l;

    tmp = explode(req, "\r\n\r\n"); // 2 linebreaks marks divider

    if (sizeof(tmp) > 1) { // content was ""
        request["content"] = tmp[1];
    }

    // parse request line
    headers = explode(tmp[0], "\r\n");

    tmp = explode(headers[0], " "); // METHOD URI PROTOCOL
    if (sizeof(tmp) == 3) {
        switch (tmp[0]) {
            case "GET":     request["method"] = GET; break;
            case "POST":    request["method"] = POST; break;
            case "HEAD":    request["method"] = HEAD; break;
            case "PUT":     request["method"] = PUT; break;
            case "DELETE":  request["method"] = DELETE; break;
            case "TRACE":   request["method"] = TRACE; break;
            case "CONNECT": request["method"] = CONNECT; break;
            default:        request["method"] = UNDEF_REQUEST; break;
        }
        request["uri"] = tmp[1];
        request["protocol"] = tmp[2];
    }

    // parse the headers
    headers = headers[1..];
    l = sizeof(headers);
    for (int i = 0; i < l; i ++) {
        int pos = 0;
        string field, value;

        pos = strsrch(headers[i], ":");
        field = headers[i][0..pos-1];
        value = headers[i][pos+2..];

        request["header"][field] = value;
    }

    return request;
}

string format_response (mapping response) {
    string msg = "HTTP/1.0 ";
    msg += response["code"];
    msg += "\r\n";
    msg += "Connection: " + response["connection"] + "\r\n";
    msg += "Server: lpc-http 0.1\r\n";
    msg += "Content-Type: " + response["type"] + "\r\n";
    msg += "\r\n";
    msg += response["content"];
    return msg;
}

/* -----  ----- */

mapping handle_response (mapping response, string path, string *args) {
    string *tmp = explode(path, ".");
    object ob;

    switch (sizeof(tmp)) {
        case 1: // add_url_pattern(path, "func")
            response["content"] = json_encode(call_other(this_object(), path, response, args));
            response["type"] = "text/json";
            response["code"] = "200 OK";
            response["connection"] = "close";
            break;
        case 2: // add_url_pattern(path, "file.func")
            ob = load_object(server_root + tmp[0]);
            response["content"] = json_encode(call_other(ob, tmp[1], response, args));
            response["type"] = "text/json";
            response["code"] = "200 OK";
            response["connection"] = "close";
            break;
        case 3: // add_url_pattern(path, "dir.file.func")
            ob = load_object(server_root + tmp[0] + "/" + tmp[1]);
            response["content"] = json_encode(call_other(ob, tmp[2], response, args));
            response["type"] = "text/json";
            response["code"] = "200 OK";
            response["connection"] = "close";
            break;
        default:
            response["content"] = "Couldn't resolve path.\n";
            response["type"] = "text/json";
            response["code"] = "200 OK";
            response["connection"] = "close";
            break;
    }

    return response;
}

void read_socket (int fd, string msg) {
    mapping req;
    mapping res = ([ ]);
    int url_match = 0;
    string result;
    int t = time_ns();

    req = parse_request(msg);

    D_LOG->log("http", ctime() + " request " + identify(req));

    foreach (mixed *arr in __URLPatterns) {
        string pattern = arr[0];
        string func = arr[1];

        if (pcre_match(req["uri"], pattern)) {
            string *args = pcre_extract(req["uri"], pattern);
            res = handle_response(res, func, args);
            debug_message("res = "+sprintf("%O", res));
            url_match = 1;
            break;
        }
    }

    if (__AllowStatic && !url_match) {
        string path = req["uri"];
        path = server_root + "www/" + path[1..];

        if (file_size(path) > 0) {
            res = ([
                "content": read_file(path),
                "code": "200 OK",
                "type": "text/html",
                "connection": "close",
            ]);
            url_match = 1;
        }
    }

    if (!url_match) {
        debug_message("Found no matches. Sending default HTTP response.\n\n");
        res = ([
            "content": "404 Not Found",
            "code": "404 Not Found",
            "type": "text/html",
            "connection": "close"
        ]);
    }

    result = format_response(res);
    t = time_ns() - t;
    D_LOG->log("http", ctime() + " response (" + sprintf("%.1f", (t / 1000000.0)) + " ms) " + identify(res));

    socket_write(fd, result);
    socket_close(fd);
}
void write_socket (int fd) {

}

void close_socket (int socket) {
    __Sockets -= ({ socket });
}

void listen_socket (int fd) {
    int socket = socket_accept(fd, "read_socket", "write_socket");

    if (socket < 0) {
        error("http: Couldn't perform socket_accept.\n");
    }

    __Sockets += ({ socket });
}

/* ----- applies ----- */

void create () {
    __Sockets = ({ });
    port = 0;
    __AllowStatic = 0;
    __URLPatterns = ({ });

    server_root = file_name(this_object());
    server_root = server_root[0..strsrch(server_root, "/", -1)];
}

void start () {
    int socket;

    if (!port) {
        return error("http: No port configured.\n");
    }

    socket = socket_create(SOCKET_STREAM, "read_socket", "close_socket");
    if (socket <= 0) {
        error("Bad response socket_create: " + socket_error(socket));
    }
    if ((socket = socket_bind(socket, port)) < 0) {
        error("Bad response from socket_bind: " + socket_error(socket));
    }
    if ((socket = socket_listen(socket, "listen_socket")) < 0) {
        error("Bad response from socket_listen: " + socket_error(socket));
    }
}

void handle_remove () {
    foreach (int socket in __Sockets) {
        socket_close(socket);
    }
}