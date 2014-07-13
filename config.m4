dnl $Id$
dnl config.m4 for extension smd5

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(smd5, for smd5 support,
dnl Make sure that the comment is aligned:
dnl [  --with-smd5             Include smd5 support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(smd5, whether to enable smd5 support,
 Make sure that the comment is aligned:
 [  --enable-smd5           Enable smd5 support])

if test "$PHP_SMD5" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-smd5 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/smd5.h"  # you most likely want to change this
  dnl if test -r $PHP_SMD5/$SEARCH_FOR; then # path given as parameter
  dnl   SMD5_DIR=$PHP_SMD5
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for smd5 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SMD5_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SMD5_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the smd5 distribution])
  dnl fi

  dnl # --with-smd5 -> add include path
  dnl PHP_ADD_INCLUDE($SMD5_DIR/include)

  dnl # --with-smd5 -> check for lib and symbol presence
  dnl LIBNAME=smd5 # you may want to change this
  dnl LIBSYMBOL=smd5 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SMD5_DIR/lib, SMD5_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SMD5LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong smd5 lib version or lib not found])
  dnl ],[
  dnl   -L$SMD5_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SMD5_SHARED_LIBADD)

  PHP_NEW_EXTENSION(smd5, smd5.c, $ext_shared)
fi
