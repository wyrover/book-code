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



union handles {
    uv_handle_t handle;
    uv_stream_t stream;
    uv_pipe_t pipe;
    uv_tcp_t tcp;
    uv_tty_t tty;
};

struct echo_ctx {
    uv_pipe_t listen;
    uv_pipe_t channel;
    uv_write_t write_req;
    uv_write_t write_req2;
    uv_handle_type expected_type;
    union handles recv;
    union handles recv2;
};

static struct echo_ctx ctx2;


static void alloc_cb(uv_handle_t* handle,
                     size_t suggested_size,
                     uv_buf_t* buf)
{
    /* we're not actually reading anything so a small buffer is okay */
    static char slab[8];
    buf->base = slab;
    buf->len = sizeof(slab);
}

static void read_cb(uv_stream_t* handle,
                    ssize_t nread,
                    const uv_buf_t* rdbuf)
{
    uv_buf_t wrbuf;
    uv_pipe_t* pipe;
    uv_handle_type pending;
    int r;
    union handles* recv;
    uv_write_t* write_req;

    if (nread == UV__EOF || nread == UV__ECONNABORTED) {
        return;
    }

    pipe = (uv_pipe_t*) handle;

    do {
        if (++read_cb_count == 2) {
            recv = &ctx2.recv;
            write_req = &ctx2.write_req;
        } else {
            recv = &ctx2.recv2;
            write_req = &ctx2.write_req2;
        }

        ASSERT(pipe == &ctx2.channel);
        ASSERT(nread >= 0);
        ASSERT(uv_pipe_pending_count(pipe) > 0);
        pending = uv_pipe_pending_type(pipe);
        ASSERT(pending == UV_NAMED_PIPE || pending == UV_TCP);

        if (pending == UV_NAMED_PIPE)
            r = uv_pipe_init(ctx2.channel.loop, &recv->pipe, 0);
        else if (pending == UV_TCP)
            r = uv_tcp_init(ctx2.channel.loop, &recv->tcp);
        else
            abort();

        ASSERT(r == 0);
        r = uv_accept(handle, &recv->stream);
        ASSERT(r == 0);
        wrbuf = uv_buf_init(".", 1);
        r = uv_write2(write_req,
                      (uv_stream_t*)&ctx2.channel,
                      &wrbuf,
                      1,
                      &recv->stream,
                      write2_cb);
        ASSERT(r == 0);
    } while (uv_pipe_pending_count(pipe) > 0);
}

static void send_recv_start()
{
    int r;
    ASSERT(1 == uv_is_readable((uv_stream_t*)&ctx2.channel));
    ASSERT(1 == uv_is_writable((uv_stream_t*)&ctx2.channel));
    ASSERT(0 == uv_is_closing((uv_handle_t*)&ctx2.channel));
    r = uv_read_start((uv_stream_t*)&ctx2.channel, alloc_cb, read_cb);
    ASSERT(r == 0);
}



int main()
{
    uv_loop_t *loop = uv_default_loop();
    int r;
    memset(&ctx2, 0, sizeof(ctx2));
    r = uv_pipe_init(loop, &ctx2.listen, 0);
    ASSERT(r == 0);
    r = uv_pipe_init(loop, &ctx2.channel, 1);
    ASSERT(r == 0);
    r = uv_pipe_open(&ctx2.channel, 0);
    ASSERT(r == 0);
    send_recv_start();
    r = uv_run(loop, UV_RUN_DEFAULT);
    ASSERT(r == 0);
    uv_loop_close(loop);
    free(loop);
    return 0;
}