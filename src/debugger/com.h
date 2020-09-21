/*
   +----------------------------------------------------------------------+
   | Xdebug                                                               |
   +----------------------------------------------------------------------+
   | Copyright (c) 2002-2020 Derick Rethans                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 1.01 of the Xdebug license,   |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | https://xdebug.org/license.php                                       |
   | If you did not receive a copy of the Xdebug license and are unable   |
   | to obtain it through the world-wide-web, please send a note to       |
   | derick@xdebug.org so we can mail you a copy immediately.             |
   +----------------------------------------------------------------------+
 */

#ifndef __HAVE_XDEBUG_COM_H__
#define __HAVE_XDEBUG_COM_H__

#include "handlers.h"

#if WIN32|WINNT
# define SOCK_ERR INVALID_SOCKET
# define SOCK_CONN_ERR SOCKET_ERROR
# define SOCK_RECV_ERR SOCKET_ERROR
#else
# define SOCK_ERR -1
# define SOCK_CONN_ERR -1
# define SOCK_RECV_ERR -1
#endif
#define SOCK_TIMEOUT_ERR -2
#define SOCK_ACCESS_ERR -3

size_t xdebug_read_data(xdebug_con *context, char *buffer, size_t buffer_size);
size_t xdebug_send_data(xdebug_con *context, char *data, size_t length);

void xdebug_close_socket(int socket);

/* Remote connection activation and house keeping */
int xdebug_is_debug_connection_active(void);
void xdebug_stop_debugger(void);
void xdebug_restart_debugger(void);
void xdebug_mark_debug_connection_active(void);
void xdebug_mark_debug_connection_not_active(void);
void xdebug_mark_debug_connection_pending(void);
void xdebug_debug_init_if_requested_at_startup(void);
void xdebug_debug_init_if_requested_on_error(void);
void xdebug_debug_init_if_requested_on_xdebug_break(void);

#endif
