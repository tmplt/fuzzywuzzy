#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

#include "fuzzywuzzy.hpp"
// helpers

static void 
fuzzy(
	sqlite3_context *context,
	int argc,
	sqlite3_value **argv
){
	const char *s = (const char *)sqlite3_value_text(argv[0]);
	const char *t = (const char *)sqlite3_value_text(argv[1]);
	int distance = fuzz::ratio(s, t);

	sqlite3_result_int(context, distance);
}


static void 
fuzzy_partial(
	sqlite3_context *context,
	int argc,
	sqlite3_value **argv
){
	const char *s = (const char *)sqlite3_value_text(argv[0]);
	const char *t = (const char *)sqlite3_value_text(argv[1]);
	int distance = fuzz::partial_ratio(s, t);

	sqlite3_result_int(context, distance);
}


static void 
fuzzy_token_sort(
	sqlite3_context *context,
	int argc,
	sqlite3_value **argv
){
	const char *s = (const char *)sqlite3_value_text(argv[0]);
	const char *t = (const char *)sqlite3_value_text(argv[1]);
	int distance = fuzz::token_sort_ratio(s, t);

	sqlite3_result_int(context, distance);
}


static void 
fuzzy_token_set(
	sqlite3_context *context,
	int argc,
	sqlite3_value **argv
){
	const char *s = (const char *)sqlite3_value_text(argv[0]);
	const char *t = (const char *)sqlite3_value_text(argv[1]);
	int distance = fuzz::token_sort_ratio(s, t);

	sqlite3_result_int(context, distance);
}


/* SQLite invokes this routine once when it loads the extension.
 ** Create new functions, collating sequences, and virtual table
 ** modules here.  This is usually the only exported symbol in
 ** the shared library.
 */
extern "C" {
int sqlite3_extension_init(
	sqlite3 *db,
	char **pzErrMsg,
	const sqlite3_api_routines *pApi
){
	int rc = SQLITE_OK;
	SQLITE_EXTENSION_INIT2(pApi)
	sqlite3_create_function(db, "fuzzy", 2, SQLITE_ANY, 0, fuzzy, NULL, NULL);
	sqlite3_create_function(db, "fuzzy_partial", 2, SQLITE_ANY, 0, fuzzy_partial, NULL, NULL);
	sqlite3_create_function(db, "fuzzy_token_sort", 2, SQLITE_ANY, 0, fuzzy_token_sort, NULL, NULL);
	sqlite3_create_function(db, "fuzzy_token_set", 2, SQLITE_ANY, 0, fuzzy_token_set, NULL, NULL);

	return rc;
}
}
