#include <evhtp.h>
#include <evhtp/log.h>

typedef struct http_server_request {
  evhtp_request_t* evhtp_request;
} http_server_request;

typedef struct http_server_response {
  uint16_t code;
  evhtp_request_t* evhtp_request;
} http_server_response;

typedef void (*http_server_handler)(http_server_request* req,
                                    http_server_response* res);

typedef struct http_server {
  uint16_t port;
  struct event_base* evbase;
  struct evhtp* htp;
} http_server;

http_server* http_server_new(uint16_t port) {
  http_server* s = malloc(sizeof(http_server));
  if (s == NULL) {
    return NULL;
  }

  struct event_base* evbase = event_base_new();
  if (evbase == NULL) {
    free(s);

    return NULL;
  }

  struct evhtp* htp = evhtp_new(evbase, NULL);
  if (htp == NULL) {
    event_base_free(evbase);
    free(s);

    return NULL;
  }

  s->port = port;
  s->evbase = evbase;
  s->htp = htp;

  return s;
}

void http_server_free(http_server* s) {
  event_base_free(s->evbase);
  evhtp_free(s->htp);
  free(s);
}

int http_server_serve(http_server* s) {
  evhtp_enable_flag(s->htp, EVHTP_FLAG_ENABLE_ALL);

  int ret = evhtp_bind_socket(s->htp, "0.0.0.0", s->port, 128);
  if (ret < 0) {
    return ret;
  }

  return event_base_loop(s->evbase, 0);
}

int http_server_stop(http_server* s) {
  int ret = event_base_loopbreak(s->evbase);

  evhtp_unbind_socket(s->htp);

  return ret;
}

static void process_request(evhtp_request_t* r, void* a) {
  http_server_request req = {.evhtp_request = r};
  http_server_response res = {.evhtp_request = r};
  http_server_handler handler = (http_server_handler)a;

  handler(&req, &res);

  evhtp_send_reply(r, res.code);
}

int http_server_register(http_server* s, const char* path,
                         http_server_handler handler) {
  evhtp_set_cb(s->htp, path, process_request, (void *)handler);

  return 0;
}
