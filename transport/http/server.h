#pragma once

#include <stdint.h>

/**
 * http_server_request that is passed to the http_server_handler.
 */
typedef struct http_server_request http_server_request;

/**
 * http_server_response that is passed to the http_server_handler.
 */
typedef struct http_server_response {
  uint16_t code; /**< code to return (e.g 200) */
} http_server_response;

/**
 * http_server that serves requests.
 */
typedef struct http_server http_server;

/**
 * http_server_handler to handle the path request.
 */
typedef void (*http_server_handler)(http_server_request* req,
                                    http_server_response* res);

/**
 * http_server_new creates a new http_server.
 */
extern http_server* http_server_new(uint16_t port);

/**
 * http_server_free releases the http_server.
 */
extern void http_server_free(http_server* s);

/**
 * http_server_serve the registered handlers in http_server.
 */
extern int http_server_serve(http_server* s);

/**
 * http_server_stop the http_server.
 */
extern int http_server_stop(http_server* s);

/**
 * http_server_register a handler to the path for the http_server.
 */
extern int http_server_register(http_server* s, const char* path,
                                http_server_handler handler);
