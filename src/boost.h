#ifndef MATRIX_BOOST_H
#define MATRIX_BOOST_H

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/logical/not.hpp>

#define CODE_PROCESS(z, iter1, code) code(iter1)
#define UNROLL(code, count) BOOST_PP_REPEAT(count, CODE_PROCESS, code)


#define CODE_PROCESS_STEP(z, count, code) BOOST_PP_EXPR_IF(\
											BOOST_PP_NOT(BOOST_PP_MOD(count, BOOST_PP_ARRAY_ELEM(1, code))),\
											CODE_PROCESS(z, count, BOOST_PP_ARRAY_ELEM(0, code))) \

#define UNROLL_STEP(code, count, step) BOOST_PP_REPEAT(count, CODE_PROCESS_STEP, (2, (code, step)))


#define CODE_PROCESS2S(code, iter1, iter2) code(iter1, iter2)
#define CODE_PROCESS2(z, count, code) CODE_PROCESS2S(BOOST_PP_ARRAY_ELEM(0, code), \
								BOOST_PP_DIV(count, BOOST_PP_ARRAY_ELEM(1, code)),\
								 BOOST_PP_MOD(count, BOOST_PP_ARRAY_ELEM(1, code)))

#define UNROLL2(code, count1, count2)   BOOST_PP_REPEAT\
										 (BOOST_PP_MUL(count1, count2),\
										 CODE_PROCESS2, (2, (code, count1))) \

#endif
