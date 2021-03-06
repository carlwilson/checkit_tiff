/* rule based checks if given TIFF is a specific baseline TIFF
 *
 * author: Andreas Romeyke, 2015
 * licensed under conditions of libtiff
 * (see http://libtiff.maptools.org/misc.html)
 *
 */

#include "check.h"
#include "check_helper.h"
/* #define DEBUG */

/* checks if TIF does not have a specified tag,
 * needed only for checks to ensure whitelist */
ret_t check_notag(ctiff_t * ctif, tag_t tag) {
  GET_EMPTY_RET(ret)
  tifp_check( ctif);
  ret_t r =  check_tag_quiet( ctif, tag);
  if (is_valid == r.returncode) {
    ret.returncode = tag_exist;
  } else if (tag_does_not_exist == r.returncode) {
    if (NULL != r.value_found) {
      free(r.value_found);
      r.value_found = NULL;
    }
    ret.returncode = is_valid;
  } else return r;
  return ret;
}
/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 smarttab expandtab :*/
