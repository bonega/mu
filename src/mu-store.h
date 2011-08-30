/*
** Copyright (C) 2008-2010 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**
*/

#ifndef __MU_STORE_H__
#define __MU_STORE_H__

#include <glib.h>
#include <inttypes.h>
#include <mu-msg.h>
#include <mu-util.h> /* for MuError, MuError */

G_BEGIN_DECLS

struct _MuStore;
typedef struct _MuStore MuStore;


/**
 * create a new writable Xapian store, a place to store documents
 *
 * @param path the path to the database
 * @param ccachepath path where to cache the contacts information, or NULL
 * @param err to receive error info or NULL. err->code is MuError value
 *
 * @return a new MuStore object with ref count == 1, or NULL in case
 * of error; free with mu_store_unref
 */
MuStore*  mu_store_new_writable  (const char *xpath, const char *ccachepath,
				  GError **err)
                 G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;


/**
 * create a new read-only Xapian store, for querying documents
 *
 * @param path the path to the database
 * @param err to receive error info or NULL. err->code is MuError value
 *
 * @return a new MuStore object with ref count == 1, or NULL in case
 * of error; free with mu_store_unref
 */
MuStore* mu_store_new_read_only (const char* xpath, GError **err)
                   G_GNUC_MALLOC G_GNUC_WARN_UNUSED_RESULT;



/**
 * increase the reference count for this store with 1
 *
 * @param store a valid store object
 *
 * @return the same store with increased ref count, or NULL in case of
 * error
 */
MuStore* mu_store_ref (MuStore *store);

/**
 * decrease the reference count for this store with 1
 *
 * @param store a valid store object
 *
 * @return NULL
 */
MuStore* mu_store_unref (MuStore *store);


/**
 * we need this when using Xapian::WritableDatabase* from C
 */
typedef gpointer XapianWritableDatabase;



/**
 * get the underlying writable database object for this store; not
 * that this pointer becomes in valid after mu_store_destroy
 *
 * @param store a valid store
 *
 * @return a Xapian::WritableDatabase (you'll need to cast in C++), or
 * NULL in case of error.
 */
XapianWritableDatabase* mu_store_get_writable_database (MuStore *store);


/**
 * we need this when using Xapian::WritableDatabase* from C
 */
typedef gpointer XapianDatabase;


/**
 * get the underlying read-only database object for this store; not that this
 * pointer becomes in valid after mu_store_destroy
 *
 * @param store a valid store
 *
 * @return a Xapian::Database (you'll need to cast in C++), or
 * NULL in case of error.
 */
XapianDatabase* mu_store_get_read_only_database (MuStore *store);



/**
 * set the Xapian batch size for this store. Normally, there's no need
 * to use this function as the default is good enough; however, if you
 * use mu in a very memory-constrained environment, you can set the
 * batchsize to e.g. 1000 at the cost of significant slow-down.
 *
 * @param store a valid store object
 * @param batchsize the new batch size; or 0 to reset to
 * the default batch size
 */
void  mu_store_set_batch_size (MuStore *store, guint batchsize);





/**
 * get the numbers of documents in the database
 *
 * @param index a valid MuStore instance
 *
 * @return the number of documents in the database; 0 in case of error
 * or an empty database
 */
unsigned mu_store_count (MuStore *store);

/**
 * get a version string for the database; it's a const string, which
 * is valid as long MuStore exists and mu_store_version is not called
 * again.
 *
 * @param store a valid MuStore
 *
 * @return the version string or NULL in case of error
 */
const char* mu_store_version (MuStore *store);


/**
 * try to flush/commit all outstanding work
 *
 * @param store a valid xapian store
 */
void mu_store_flush (MuStore *store);

/**
 * store an email message in the XapianStore
 *
 * @param store a valid store
 * @param msg a valid message
 * @param replace whether or not we should try to replace the same
 * message if it already exists; that is usually desirable, but when
 * we're sure already that the document does not exist (e.g, in case
 * of a initial fill or rebuild of the database), we can set 'replace'
 * to FALSE for a couple% performance gain
 *
 * @return TRUE if it succeeded, FALSE otherwise
 */
gboolean mu_store_store_msg   (MuStore *store, MuMsg *msg, gboolean replace);


/**
 * store an email message in the XapianStore; similar to
 * mu_store_store, but instead takes a path as parameter instead of a
 * MuMsg*
 *
 * @param store a valid store
 * @param path full filesystem path to a valid message
 *
 * @return TRUE if it succeeded, FALSE otherwise
 */
gboolean mu_store_store_path (MuStore *store, const char *path);

/**
 * remove a message from the database
 *
 * @param store a valid store
 * @param msgpath path of the message (note, this is only used to
 * *identify* the message; a common use of this function is to remove
 * a message from the database, for which there is no message anymore
 * in the filesystem.
 *
 * @return TRUE if it succeeded, FALSE otherwise
 */
gboolean mu_store_remove_path (MuStore *store, const char* msgpath);


/**
 * does a certain message exist in the database already?
 *
 * @param store a store
 * @param path the message path
 *
 * @return TRUE if the message exists, FALSE otherwise
 */
gboolean mu_store_contains_message (MuStore *store,  const char* path);

/**
 * store a timestamp for a directory
 *
 * @param store a valid store
 * @param msgpath path to a maildir
 * @param stamp a timestamp
 *
 * @return TRUE if setting the timestamp succeeded, FALSE otherwise
 */
gboolean mu_store_set_timestamp (MuStore *store, const char* msgpath,
				 time_t stamp);

/**
 * get the timestamp for a directory
 *
 * @param store a valid store
 * @param msgpath path to a maildir
 *
 * @return the timestamp, or 0 in case of error
 */
time_t mu_store_get_timestamp (MuStore *store,
			       const char* msgpath);


/**
 * check whether this store is read-only
 *
 * @param store a store
 *
 * @return TRUE if the store is read-only, FALSE otherwise (and in
 * case of error)
 */
gboolean mu_store_is_read_only (MuStore *store);


/**
 * call a function for each document in the database
 *
 * @param self a valid store
 * @param func a callback function to to call for each document
 * @param user_data a user pointer passed to the callback function
 *
 * @return MU_OK if all went well, MU_STOP if the foreach was interrupted,
 * MU_ERROR in case of error
 */
typedef MuError (*MuStoreForeachFunc) (const char* path,
					      void *user_data);
MuError  mu_store_foreach (MuStore *self, MuStoreForeachFunc func,
			   void *user_data);

/**
 * set metadata for this MuStore
 *
 * @param store a store
 * @param key metadata key
 * @param val metadata value
 *
 * @return TRUE if succeeded, FALSE otherwise
 */
gboolean mu_store_set_metadata (MuStore *store, const char *key, const char *val);

/**
 * get metadata for this MuStore
 *
 * @param store a store
 * @param key the metadata key
 *
 * @return the value of the metadata (gfree when done with it), or
 * NULL in case of error
 */
char* mu_store_get_metadata (MuStore *store, const char *key)
	G_GNUC_WARN_UNUSED_RESULT;


/**
" * get the version of the xapian database (ie., the version of the
 * 'schema' we are using). If this version != MU_STORE_SCHEMA_VERSION,
 * it's means we need to a full reindex.
 *
 * @param xpath path to the xapian database
 *
 * @return the version of the database as a newly allocated string
 * (free with g_free); if there is no version yet, it will return NULL
 */
gchar* mu_store_database_version (const gchar *xpath) G_GNUC_WARN_UNUSED_RESULT;


/**
 * check whether the database needs to be upgraded, e.g., when it was
 * created with a different version of mu
 *
 * @param store a MuStore instance
 *
 * @return TRUE if the database needs upgrading, FALSE otherwise
 */
gboolean mu_store_needs_upgrade (MuStore *store);



/**
 * clear the database, ie., remove all of the contents. This is a
 * destructive operation, but the database can be restored be doing a
 * full scan of the maildirs. Also, clear the contacts cache file
 *
 * @param store a MuStore object
 *
 * @return TRUE if the clearing succeeded, FALSE otherwise.
 */
gboolean mu_store_clear (MuStore *store);


/**
 * check if the database is locked for writing
 *
 * @param xpath path to a xapian database
 *
 * @return TRUE if it is locked, FALSE otherwise (or in case of error)
 */
gboolean mu_store_database_is_locked (const gchar *xpath);

G_END_DECLS

#endif /*__MU_STORE_H__*/
