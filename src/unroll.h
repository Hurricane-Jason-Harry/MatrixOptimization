/*
 * UNROLL(code, count):   code should be defined as the format of
 *                        #define CODE(c) .....
 *                        UNROLLL(code, count) expands to
 *                        code(0) code(1) ... code(count-1)
 *
 * UNROLL_STEP(code, count, step): code should be defined as the format of
 *                        #define CODE(c) .....
 *                        UNROLLL(code, count, step) expands to
 *                        code(0) code(step) ... code(count/step*step-step)
 *
 * UNROLL_STEP2(code, count1, count2): code should be defined as the format of
 *                        #define CODE(c1, c2) .....
 *                        UNROLLL(code, count1, count2) expands to
 *                        code(0, 0) code(0, 1) ... code(0, count2-1)\
 *                        code(1, 0) code(1, 1) ... code(1, count2-1)\
 *                        ...
 *                        code(count1-1,0) code(cout1-1,0) ... code(count1-1, count2-1)
 *
 * UNROLLZ(code, begin, count): code should be defined as the format of
 *                        #define CODE(c) .....
 *                        UNROLLL(code, count) expands to
 *                        code(begin) code(1) ... code(count-1)
 */



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
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

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

#define CODE_PROCESSZ(z, count, code) CODE_PROCESS(z, BOOST_PP_ADD(count, BOOST_PP_ARRAY_ELEM(1, code)), BOOST_PP_ARRAY_ELEM(0, code))

#define UNROLLZ(code, begin, count) BOOST_PP_REPEAT(count, CODE_PROCESSZ, (2, (code, begin)))

#endif
