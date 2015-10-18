#pragma once

#include <boost/preprocessor/cat.hpp>

// Remove the outer parentheses from anything
#define GLK_PP_REMOVE_PARENS_(...)\
	__VA_ARGS__

#define GLK_PP_REMOVE_PARENS(...)\
	GLK_PP_REMOVE_PARENS_ __VA_ARGS__

// Double the parentheses of a Boost.PP sequence
// I.e. (a, b)(c, d) becomes ((a, b))((c, d))
#define GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_0(...)\
     ((__VA_ARGS__)) GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_1

#define	GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_1(...)\
     ((__VA_ARGS__)) GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_0

#define GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_0_END
#define GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_1_END

#define GLK_PP_SEQ_DOUBLE_PARENS(seq)\
	BOOST_PP_CAT(GLK_PP_DETAIL_SEQ_DOUBLE_PARENS_0 seq, _END)
