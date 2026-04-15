#include <socket.h>
#include <driver/socket_err.h>

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
private nosave int __Port;
private nosave int __AllowStatic;
private nosave mixed *__URLPatterns;
private nosave string server_root;

/* -----  ----- */

/**
 * @return {int} The port this server is configured to bind on, or 0 if unset.
 */
int query_port () {
    return __Port;
}

/**
 * @param {int} p - The port to bind on when start() is called.
 */
void set_port (int p) {
    __Port = p;
}

/**
 * @return {mixed *} Array of ({ pattern, fn }) pairs registered via add_url_pattern().
 */
mixed *query_url_patterns () {
    return __URLPatterns;
}

/**
 * Register a URL pattern and the handler function to invoke when it matches.
 *
 * The fn argument uses dot-notation to locate the handler:
 *   "func"          - calls this_object()->func(response, args)
 *   "file.func"     - loads server_root/file and calls func(response, args)
 *   "dir.file.func" - loads server_root/dir/file and calls func(response, args)
 *
 * The pattern is a PCRE regex matched against the request URI. Captured groups
 * are passed to the handler as the args array.
 *
 * @param {string} pattern - PCRE regex matched against the request URI.
 * @param {string} fn - Dot-notation handler path.
 */
void add_url_pattern (string pattern, string fn) {
    __URLPatterns += ({ ({ pattern, fn }) });
}

/**
 * @return {int} 1 if static file serving is enabled, 0 otherwise.
 */
int query_allow_static_pages () {
    return __AllowStatic;
}

/**
 * Enable or disable serving static files from server_root/www/.
 * When enabled, unmatched requests are checked against the filesystem
 * before returning 404. URIs containing ".." are always rejected.
 *
 * @param {int} allow - 1 to enable, 0 to disable.
 */
void allow_static_pages (int allow) {
    __AllowStatic = !!allow;
}

/* -----  ----- */

/**
 * Parse a raw HTTP request string into a structured mapping.
 *
 * Returned mapping shape:
 *   "method"   : int    - HTTP method constant (GET, POST, etc.) or UNDEF_REQUEST (-1)
 *   "uri"      : string - Request URI
 *   "protocol" : string - Protocol string (ex: "HTTP/1.0")
 *   "header"   : mapping - Header field/value pairs; malformed lines are skipped
 *   "content"  : string - Request body (empty string if none)
 *
 * @param {string} req - Raw HTTP request including headers and optional body.
 * @return {mapping} Parsed request mapping.
 */
mapping parse_request (string req) {
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
        case "GET":
            request["method"] = GET;
            break;
        case "POST":
            request["method"] = POST;
            break;
        case "HEAD":
            request["method"] = HEAD;
            break;
        case "PUT":
            request["method"] = PUT;
            break;
        case "DELETE":
            request["method"] = DELETE;
            break;
        case "TRACE":
            request["method"] = TRACE;
            break;
        case "CONNECT":
            request["method"] = CONNECT;
            break;
        default:
            request["method"] = UNDEF_REQUEST;
            break;
        }
        request["uri"] = tmp[1];
        request["protocol"] = tmp[2];
    }

    // parse the headers
    headers = headers[1..];
    l = sizeof(headers);
    for (int i = 0; i < l; i++) {
        int pos;
        string field, value;

        pos = strsrch(headers[i], ":");
        if (!sizeof(headers[i]) || pos < 1) {
            continue;
        }
        field = headers[i][0..pos-1];
        value = headers[i][pos+2..];
        request["header"][field] = value;
    }

    return request;
}

/**
 * Format a response mapping into a raw HTTP/1.0 response string.
 *
 * Expected mapping shape:
 *   "code"       : string - Status line (ex: "200 OK", "404 Not Found")
 *   "connection" : string - Connection header value (ex: "close")
 *   "type"       : string - Content-Type value (ex: "text/html")
 *   "content"    : string - Response body
 *
 * @param {mapping} response - Response mapping to serialize.
 * @return {string} Raw HTTP response string ready to write to a socket.
 */
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

/**
 * Resolve a dot-notation handler path, call the handler, and populate the
 * response mapping. See add_url_pattern() for the dot-notation format.
 *
 * @param {mapping} response - Response mapping to populate.
 * @param {string} path - Dot-notation handler path from add_url_pattern().
 * @param {string *} args - PCRE capture groups extracted from the request URI.
 * @return {mapping} Populated response mapping.
 */
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
            response["code"] = "500 Internal Server Error";
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
            url_match = 1;
            break;
        }
    }

    if (__AllowStatic && !url_match) {
        string path = req["uri"];

        if (strsrch(path, "..") == -1) {
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
        error("http: Couldn't perform socket_accept.");
    }
    __Sockets += ({ socket });
}

/* ----- applies ----- */

void create () {
    __Sockets = ({ });
    __Port = 0;
    __AllowStatic = 0;
    __URLPatterns = ({ });

    server_root = file_name();
    server_root = server_root[0..strsrch(server_root, "/", -1)];
}

/**
 * Bind to the configured port and begin listening for connections.
 * set_port() must be called before start().
 */
void start () {
    int socket;

    if (!__Port) {
        error("http: No port configured.");
    }

    socket = socket_create(SOCKET_STREAM, "read_socket", "close_socket");
    if (socket <= 0) {
        error("Bad response socket_create: " + socket_error(socket));
    }
    if ((socket = socket_bind(socket, __Port)) < 0) {
        error("Bad response from socket_bind: " + socket_error(socket));
    }
    if ((socket = socket_listen(socket, "listen_socket")) < 0) {
        error("Bad response from socket_listen: " + socket_error(socket));
    }
}

/**
 * Close all open sockets. Called automatically when the object is destructed.
 */
void handle_remove () {
    foreach (int socket in __Sockets) {
        socket_close(socket);
    }
}
