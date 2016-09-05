#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define TEST_PIPENAME "\\\\?\\pipe\\uv-test"
/* Die with fatal error. */
#define FATAL(msg)                                        \
    do {                                                    \
        fprintf(stderr,                                       \
                "Fatal error in %s on line %d: %s\n",         \
                __FILE__,                                     \
                __LINE__,                                     \
                msg);                                         \
        fflush(stderr);                                       \
        abort();                                              \
    } while (0)

/* Have our own assert, so we are sure it does not get optimized away in
 * a release build.
 */
#define ASSERT(expr)                                      \
    do {                                                     \
        if (!(expr)) {                                          \
            fprintf(stderr,                                       \
                    "Assertion failed in %s on line %d: %s\n",    \
                    __FILE__,                                     \
                    __LINE__,                                     \
                    #expr);                                       \
            abort();                                              \
        }                                                       \
    } while (0)

typedef enum {
    TCP = 0,
    UDP,
    PIPE
} stream_type;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static uv_loop_t* loop;
static int server_closed;
static uv_handle_t* server;


static void echo_alloc(uv_handle_t* handle,
                       size_t suggested_size,
                       uv_buf_t* buf)
{
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

static void on_close(uv_handle_t* peer)
{
    free(peer);
}

static void on_server_close(uv_handle_t* handle)
{
    ASSERT(handle == server);
}



static void after_shutdown(uv_shutdown_t* req, int status)
{
    uv_close((uv_handle_t*) req->handle, on_close);
    free(req);
}

static void after_write(uv_write_t* req, int status)
{
    write_req_t* wr;
    /* Free the read/write buffer and the request */
    wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);

    if (status == 0)
        return;

    fprintf(stderr,
            "uv_write error: %s - %s\n",
            uv_err_name(status),
            uv_strerror(status));
}



static void after_read(uv_stream_t* handle,
                       ssize_t nread,
                       const uv_buf_t* buf)
{
    int i;
    write_req_t *wr;
    uv_shutdown_t* sreq;

    if (nread < 0) {
        /* Error or EOF */
        ASSERT(nread == UV_EOF);
        free(buf->base);
        sreq = (uv_shutdown_t*)malloc(sizeof * sreq);
        ASSERT(0 == uv_shutdown(sreq, handle, after_shutdown));
        return;
    }

    if (nread == 0) {
        /* Everything OK, but nothing read. */
        free(buf->base);
        return;
    }

    /*
     * Scan for the letter Q which signals that we should quit the server.
     * If we get QS it means close the stream.
     */
    if (!server_closed) {
        for (i = 0; i < nread; i++) {
            if (buf->base[i] == 'Q') {
                if (i + 1 < nread && buf->base[i + 1] == 'S') {
                    free(buf->base);
                    uv_close((uv_handle_t*)handle, on_close);
                    return;
                } else {
                    uv_close(server, on_server_close);
                    server_closed = 1;
                }
            }
        }
    }

    wr = (write_req_t*) malloc(sizeof * wr);
    ASSERT(wr != NULL);
    wr->buf = uv_buf_init(buf->base, nread);

    if (uv_write(&wr->req, handle, &wr->buf, 1, after_write)) {
        FATAL("uv_write failed");
    }
}




static void on_connection(uv_stream_t* server, int status)
{
    uv_stream_t* stream;
    int r;

    if (status != 0) {
        fprintf(stderr, "Connect error %s\n", uv_err_name(status));
    }

    ASSERT(status == 0);
    stream = (uv_stream_t*)malloc(sizeof(uv_pipe_t));
    ASSERT(stream != NULL);
    r = uv_pipe_init(loop, (uv_pipe_t*)stream, 0);
    ASSERT(r == 0);
    /* associate server with stream */
    stream->data = server;
    r = uv_accept(server, stream);
    ASSERT(r == 0);
    r = uv_read_start(stream, echo_alloc, after_read);
    ASSERT(r == 0);
}


int main()
{
    loop = uv_default_loop();
    int r;
    uv_pipe_t pipeServer;
    server = (uv_handle_t*)&pipeServer;
    stream_type serverType;
    serverType = PIPE;
    r = uv_pipe_init(loop, &pipeServer, 0);

    if (r) {
        fprintf(stderr, "uv_pipe_init: %s\n", uv_strerror(r));
        return 1;
    }

    r = uv_pipe_bind(&pipeServer, TEST_PIPENAME);

    if (r) {
        fprintf(stderr, "uv_pipe_bind: %s\n", uv_strerror(r));
        return 1;
    }

    r = uv_listen((uv_stream_t*)&pipeServer, SOMAXCONN, on_connection);

    if (r) {
        fprintf(stderr, "uv_pipe_listen: %s\n", uv_strerror(r));
        return 1;
    }

    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}