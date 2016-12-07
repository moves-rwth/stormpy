/*
 * types.h
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#ifndef PYTHON_CORE_TYPES_H_
#define PYTHON_CORE_TYPES_H_

#include <carl/numbers/numbers.h>
#include <carl/core/Variable.h>
#include <carl/core/Monomial.h>
#include <carl/core/Term.h>
#include <carl/core/MultivariatePolynomial.h>
#include <carl/core/FactorizedPolynomial.h>
#include <carl/core/RationalFunction.h>
#include <carl/interval/Interval.h>

#ifdef PYCARL_USE_CLN
typedef cln::cl_RA Rational;
#else
//typedef double Rational;
typedef mpq_class Rational;
#endif
typedef carl::Monomial::Arg Monomial;
typedef carl::Term<Rational> Term;
typedef carl::MultivariatePolynomial<Rational> Polynomial;
typedef carl::FactorizedPolynomial<Polynomial> FactorizedPolynomial;
typedef carl::RationalFunction<Polynomial, true> RationalFunction;
typedef carl::RationalFunction<FactorizedPolynomial, true> FactorizedRationalFunction;
typedef carl::PolynomialFactorizationPair<Polynomial> FactorizationPair;

using Interval = carl::Interval<Rational>;

#endif /* PYTHON_CORE_TYPES_H_ */
