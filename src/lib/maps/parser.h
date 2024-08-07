/*
   +----------------------------------------------------------------------+
   | Xdebug                                                               |
   +----------------------------------------------------------------------+
   | Copyright (c) 2002-2024 Derick Rethans                               |
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

#ifndef __XDEBUG_MAPS_PARSER_H__
#define __XDEBUG_MAPS_PARSER_H__

/* Parser error codes */
#define PATH_MAPS_OK             0x0000
#define PATH_MAPS_CANT_OPEN_FILE 0x0001
#define PATH_MAPS_NO_RULES       0x0002
#define PATH_MAPS_EMPTY_LINE     0x0003
#define PATH_MAPS_NO_NEWLINE     0x0004
#define PATH_MAPS_GARBAGE        0x0005

bool xdebug_path_maps_parse_file(xdebug_path_maps *maps, const char *filename, int *error_code, char **error_message);

#endif