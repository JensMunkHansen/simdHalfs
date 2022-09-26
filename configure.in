AC_INIT(src/main.cpp)
AM_INIT_AUTOMAKE( sse-halfs, 1.0 )
AM_CONFIG_HEADER(include/config.h)
AC_PROG_CXX
AC_STDC_HEADERS

AC_CHECK_FUNCS(printf)
# MMX
AC_CHECK_HEADERS([mmintrin.h])
# SSE
AC_CHECK_HEADERS([xmmintrin.h])
# SSE2
AC_CHECK_HEADERS([emmintrin.h])
# SSE3
AC_CHECK_HEADERS([pmmintrin.h])
# SSSE3
AC_CHECK_HEADERS([tmmintrin.h])
# SSE4.1
AC_CHECK_HEADERS([smmintrin.h])
# SSE4.2
AC_CHECK_HEADERS([nmmintrin.h])

CONFDATO=`date "+%Y-%m-%d %H:%M:%S"`
AC_DEFINE_UNQUOTED(CONFDATO,"$CONFDATO",[Date for running ./configure])

AC_MSG_CHECKING(if we should optimise for MMX)
AC_ARG_ENABLE(mmx, [  --enable-mmx            Use MMX optimisations [default no]],,enable_mmx=no)
if test x$enable_mmx = xyes; then
    AC_MSG_RESULT(yes)
    CXXFLAGS="$CXXFLAGS -mmmx"
else
    AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(if we should optimise for 3DNOW)
AC_ARG_ENABLE(3dnow, [  --enable-3dnow          Use 3DNOW optimisations [default no]],,enable_3dnow=no)
if test x$enable_3dnow = xyes; then
    AC_MSG_RESULT(yes)
    CXXFLAGS="$CXXFLAGS -m3dnow"
else
    AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(if we should optimise for SSE2)
AC_ARG_ENABLE(sse2, [  --enable-sse2           Use SSE2 optimisations [default no]],,enable_sse2=no)
if test x$enable_sse2 = xyes; then
    AC_MSG_RESULT(yes)
    CXXFLAGS="$CXXFLAGS -msse2"
else
    AC_MSG_RESULT(no)
fi

AC_OUTPUT(Makefile src/Makefile)
