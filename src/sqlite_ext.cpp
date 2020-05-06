#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "fuzzywuzzy.hpp"
// helpers
int minimum(int a,int b,int c);

/*
  function to determine damerau-levenshtein distance
  damerau_levenshtein(src,dts,max_distance) => bool
*/
static void 
fuzzy(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
    
  const char *s    = (const char *)sqlite3_value_text(argv[0]);
  const char *t    = (const char *)sqlite3_value_text(argv[1]);
  int distance = fuzz::ratio(s, t);
  
  sqlite3_result_int(context, distance);
}


inline int minimum(int a, int b, int c) {
  int min = a;
  if (b < min) min=b;
  if (c < min) min=c;
  return min;
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
  
  return rc;
}
}
