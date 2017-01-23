/* rule based checks if given TIFF is a specific baseline TIFF
 * 
 * author: Andreas Romeyke, 2015
 * licensed under conditions of libtiff 
 * (see file LICENSE)
 *
 */

#include "check.h"
#include "check_helper.h"
#include <string.h>
/*
#define DEBUG
*/

ret_t check_tag_has_some_of_these_values(ctiff_t * ctif, tag_t tag, int count, unsigned int * values) {
  //printf("check if tag %u (%s) has some of these %i-values", tag, TIFFTagName(tif, tag), count);
  tifp_check( ctif);
  char msg[EXPECTSTRLEN];
  char expected[EXPECTSTRLEN]="";
  snprintf(msg, sizeof(msg), "has some of these %i-values: ", count);
  unsigned int * p = values;
  for (int i=0; i< count; i++) {
    if( i>= 1) secstrcat (expected, ", ", EXPECTSTRLEN);
    secstrcat (expected, int2str(*p), EXPECTSTRLEN);
    p++;
  }
  secstrcat (msg, expected, EXPECTSTRLEN);
  tif_rules_tag(tag, strdup(msg));
  TIFFDataType datatype =  TIFFGetRawTagType( ctif, tag );
  switch (datatype) {
    case TIFF_LONG: { 
                      p = values;
                      ret_t tmp_res;
                      for (int i=0; i< count; i++) {
#ifdef DEBUG
                        printf("### value = %u", *p);
#endif
                        tmp_res = check_tag_has_u32value(ctif, tag, *p);
                        if (tmp_res.returncode == 0) return tmp_res;
                        p++;
                      }
                      uint32 * valp = NULL;
                      uint32 val;
                      TIFFGetFieldLONG(ctif, tag, &valp);
                      val = *valp;
                      return tif_fails_tag( tag, strdup(expected), int2str(val));
                      break;
                    }
    case TIFF_SHORT: {
                       p = values;
                       ret_t tmp_res;
                       for (int i=0; i< count; i++) {
#ifdef DEBUG
                         printf("### value = %u", *p);
#endif
                         tmp_res = check_tag_has_u16value(ctif, tag, *p);
                         if (tmp_res.returncode == 0) return tmp_res;
                         p++;
                       }
                       uint16 * valp = NULL;
                       uint16 val;
                       TIFFGetFieldSHORT(ctif, tag, &valp);
                       val = *valp;
                       return tif_fails_tag( tag, strdup(expected), int2str(val));
                       break;
                     }
    case TIFF_RATIONAL: {
                          p = values;
                          ret_t tmp_res;
                          for (int i=0; i< count; i++) {
#ifdef DEBUG
                            printf("### value = %u", *p);
#endif
                            tmp_res = check_tag_has_fvalue(ctif, tag, *p);
                            if (tmp_res.returncode == 0) return tmp_res;
                            p++;
                          }
                          float * valp = NULL;
                          float val;
                          TIFFGetFieldRATIONAL(ctif, tag, &valp);
                          val = * valp;
                          return tif_fails_tag( tag, strdup(expected), float2str(val));
                          //tif_fails("tag %u (%s) does not have some of expected values (but have:%f)\n", tag, TIFFTagName(tif, tag), val);
                          break;
                        }
    default: /*  none */
                        {
                        // tif_fails("tag %u (%s) should have values of type long, short or float, but was:%i\n", tag, TIFFTagName(tif, tag), datatype);
                        char array[VALUESTRLEN];
                        snprintf(array, sizeof(array), "but was:%i", datatype);
                        return tif_fails_tag( tag, "of type long, short or float", array);
                        }
  }
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 smarttab expandtab :*/
