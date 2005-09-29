/*
 $Id: dbmail.h 1787 2005-05-25 14:35:50Z paul $

 Copyright (C) 1999-2004 IC & S  dbmail@ic-s.nl

 This program is free software; you can redistribute it and/or 
 modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation; either 
 version 2 of the License, or (at your option) any later 
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/**
 * \file dbmail.h
 * header file for a general configuration
 */

#ifndef _DBMAIL_H
#define _DBMAIL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION "2.0"
#define PACKAGE "dbmail"
#endif

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <strings.h>
#include <fnmatch.h>
#include <gmime/gmime.h>
#include <glib.h>

#include "dbmailtypes.h"
#include "debug.h"
#include "dsn.h"
#include "misc.h"
#include "pipe.h"
#include "db.h"
#include "auth.h"
#include "imap4.h"
#include "memblock.h"
#include "mime.h"
#include "dbmsgbuf.h"
#include "rfcmsg.h"
#include "pidfile.h"
#include "sort.h"
#include "forward.h"
#include "dbmd5.h"
#include "md5.h"
#include "server.h"
#include "serverchild.h"
#include "pool.h"
#include "pop3.h"
#include "quota.h"

#include "dm_cidr.h"
#include "dm_imaputil.h"
#include "dm_search.h"

#ifdef SIEVE
#include "sortsieve.h"
#endif

#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

#include "dbmail-message.h"
#include "dbmail-user.h"
#include "dbmail-mailbox.h"
#include "dbmail-imapsession.h"



#define GETCONFIGVALUE(key, sect, var) \
	config_get_value(key, sect, var); \
	if (strlen(var) == 0) \
		trace(TRACE_DEBUG, "%s, %s: no value for "#key" in section "#sect, \
		    	__FILE__, __func__ ); \
	trace(TRACE_DEBUG, "%s, %s: key "#key" section "#sect" var "#var" value [%s]", \
			__FILE__, __func__, \
			var)
	/* No final ';' so macro can be called "like a function" */

#define CONFIG_ERROR_LEVEL TRACE_WARNING

#define COPYRIGHT "(c) 1999-2004 IC&S, The Netherlands"
/** default directory and extension for pidfiles */
#define DEFAULT_PID_DIR "/var/run/"
#define DEFAULT_PID_EXT ".pid"
/** default configuration file */
#define DEFAULT_CONFIG_FILE "/etc/dbmail.conf"

/** username of user that is owner of all mailboxes */
#define SHARED_MAILBOX_USERNAME "__public__"

/** prefix for #Users namespace */
#define NAMESPACE_USER "#Users"
/** prefix for public namespace */
#define NAMESPACE_PUBLIC "#Public"
/** seperator for namespaces and mailboxes and submailboxes */
#define MAILBOX_SEPARATOR "/"
/** username for owner of public folders */
#define PUBLIC_FOLDER_USER "__public__"

/** default table prefix */
#define DEFAULT_DBPFX "dbmail_"

#define MATCH(x,y) strcasecmp(x,y)==0

/**
 * \brief read configuration from filename
 * \param cfilename name of configuration file 
 * \return
 *     - -1 on error
 *     -  0 on success
 */
int config_read(const char *config_filename);

/**
 * free all memory taken up by config.
 */
void config_free(void);
	       
/**
 * \brief get configuration value for an item
 * \param name name of configuration item
 * \param service_name name of service
 * \param value value of configuration item name
 * \return 0
 * \attention value is set to a string beginning with a '\\0' 
 * if no configuration item with name is found in items.
 */
int config_get_value(const field_t name, const char *service_name,
                     /*@out@*/ field_t value);

/* some common used functions reading config options */
/**
 \brief get parameters for database connection
 \param db_params list of database parameters (db_param_t)
*/
void GetDBParams(db_param_t * db_params);
/** 
 \brief set the overall trace level, using the value in cfg_items
 \param service_name name of service to get trace level for.
 \attention trace level is set to TRACE_ERROR if no trace level value is found
 *                for service.
 */
void SetTraceLevel(const char *service_name);


#endif
