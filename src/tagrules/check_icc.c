/* rule based checks if given TIFF is a specific baseline TIFF
 * 
 * author: Andreas Romeyke, 2015/16
 * licensed under conditions of libtiff 
 * (see http://libtiff.maptools.org/misc.html)
 *
 */

#include "check.h"
#include "check_helper.h"
#include "validate_icc.h"
#include "ctype.h"
/** checks a ICC tag, see Annex B of http://www.color.org/specification/ICC1v43_2010-12.pdf
 */
ret_t check_icc(ctiff_t * ctif ) {
  ret_t ret;
  ret.value_found = malloc(VALUESTRLEN);
  if (NULL == ret.value_found) {
    ret.returncode=could_not_allocate_memory;
    return ret;
  }

  tifp_check( ctif);

  ifd_entry_t ifd_entry = TIFFGetRawIFDEntry(ctif, TIFFTAG_ICCPROFILE);
  uint32 icc_profile_size;
  uint32 count;
  char * icc_profile = NULL;
  switch (ifd_entry.datatype) { /* icc datatype should be undefined (val=7) */
      case TIFF_UNDEFINED: {
                       icc_profile_size = ifd_entry.count;
                       count = ifd_entry.count;
                     /*  offset */
                      if (ifd_entry.value_or_offset == is_offset) {
                        offset_t offset = read_offsetdata(ctif, ifd_entry.data32offset, count, ifd_entry.datatype);
                        icc_profile = (char *)offset.data32p;
                      } else {
                        ret.returncode = tagerror_encoded_as_value_excited_space;
                        return ret;
                      }
                       break;
                     }
    default: { /*  none */
               ret.value_found = strncpy(ret.value_found, TIFFTypeName(ifd_entry.datatype), VALUESTRLEN);
               ret.returncode = tagerror_unexpected_type_found;
               return ret;
               break;
             };
  }

  //printf("DEBUG: iccprofile_size=%i\n", icc_profile_size);
  // printf("DEBUG: iccprofile='%s'\n", icc_profile);
  /* DEBUG
  char * p = icc_profile;
  int i=0;
  for (i = 0; i< icc_profile_size; i++, p++) {
    if (0 == i % 8) printf("|");
    printf("%c(%0x) ", (isalnum(*p)?*p:' '),*p);
  }
  printf("\n");
  */
  char * errmessage = malloc(sizeof(char) * VALUESTRLEN);
  unsigned long errsize = VALUESTRLEN;
  icc_returncode_t icc_ret = parse_icc(icc_profile_size, icc_profile, errsize, errmessage);
  switch (icc_ret) { /*  map between returncodes icc profile and tag check */
    case icc_is_valid: ret.returncode = is_valid; break; 
    case icc_error_profileclass: ret.returncode = iccerror_profileclass; break; 
    case icc_error_colorspacedata: ret.returncode = iccerror_colorspacedata; break; 
    case icc_error_connectionspacedata: ret.returncode = iccerror_connectionspacedata; break; 
    case icc_error_primaryplatformsignature: ret.returncode = iccerror_primaryplatformsignature; break; 
    case icc_error_header_1v43_2010: ret.returncode = iccerror_header_1v43_2010; break; 
    case icc_error_header_v240_v430: ret.returncode = iccerror_header_v240_v430; break; 
    case icc_error_header_generic: ret.returncode = iccerror_header_generic; break; 
    case icc_error_preferredcmmtype: ret.returncode = iccerror_preferredcmmtype; break;
    case icc_error_committed_size_differs: ret.returncode = iccerror_committed_size_differs; break;
    case icc_should_not_occure:  ret.returncode = should_not_occure; break;
  }

  ret.value_found = strncpy(ret.value_found, errmessage, VALUESTRLEN);
  free (errmessage);
  return ret;
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 smarttab expandtab :*/
