/*
 * LLVMRandom.h
 *
 *  Created on: Sep 23, 2014
 *      Author: andy
 */

#ifndef _RRLLVM_RANDOM_H_
#define _RRLLVM_RANDOM_H_

#include "tr1proxy/rr_random.h" // rr proxy to <random>
#include <stdint.h>

namespace rrllvm {

    class Random {
    public:
        /**
         * creates a new Random object and adds the distrib as global mappings
         * to the execution engine.
         */
        Random(class ModelGeneratorContext &ctx);

        /**
         * copy constructor, copy the distributions but reset the RNG to what
         * is specified by the config.
         */
        Random(const Random &other);

        /**
         * assignment operator, copies the fields from the
         * other object, but does not re-intialize them.
         */
        Random &operator=(const Random &rhs);

        /**
         * default ctor, this obviously does not add any mappings to a
         * ModelGeneratorContext. This would be used when a Random is created
         * by itself, to use the attached RNG engine.
         */
        Random();

        ~Random();

        /**
         * return a normalized random number between 0 and 1 using the
         * RNG engine.
         */
        double operator()();

        /**
        * Returns the maximum number of tries to find a value inside a truncated range.
        */
        int getMaxTries() const;

        /**
        * Sets the maximum number of tries to find a value inside a truncated range.
        */
        void setMaxTries(int maxTries);

#ifdef CXX11_RANDOM
        /**
        * used by random distributions
        */
       static constexpr unsigned long long min() { return 0; };

       /**
        * used by random distrbutions
        */
       static constexpr unsigned long long max() { return 1; };
#else

        /**
         * min random number. MSVC looks at this, but gcc stdlib assumes normalized dist.
         */
        double min() { return 0.0; };

        /**
         * max random number. MSVC looks at this, but gcc stdlib assumes normalized dist.
         */
        double max() { return 1.0; };
#endif

        /**
         * Try to hide the RNG, so we can use different RNGs in the future.
         * set the seed used by the random number generator. This will by definition
         * reset the RNG.
         */
        void setRandomSeed(int64_t);

        /**
         * Try to hide the RNG so we can used different RNGs in the future.
         * get the seed used by the RNG.
         */
        int64_t getRandomSeed();

        /**
         * RNG engine.
         */
        cxx11_ns::mt19937 engine;

    private:
        // seed that was used to seed the engine.
        int64_t randomSeed;
        // internal distribution used to generate reals between 0.0 and 1.0

#ifdef CXX11_RANDOM
        cxx11_ns::uniform_real_distribution<double> normalized_uniform_dist;
#else
        cxx11_ns::uniform_real<double> normalized_uniform_dist;
#endif

        /**
        * The maximum number of tries to find a value inside a truncated range.
        */
        int mMaxTries;

    };

    // function signatures for distrib

    using fn_d1 = double (*)(Random*, double);
    using fn_d2 = double (*)(Random*, double, double);
    using fn_d3 = double (*)(Random*, double, double, double);
    using fn_d4 = double (*)(Random*, double, double, double, double);

    using distrib_uniform_FnTy           = fn_d2;
    using distrib_normal_FnTy            = fn_d2;
    using distrib_normal_four_FnTy       = fn_d4;
    using distrib_bernoulli_FnTy         = fn_d1;
    using distrib_binomial_FnTy          = fn_d2;
    using distrib_binomial_four_FnTy     = fn_d4;
    using distrib_cauchy_FnTy            = fn_d2;
    using distrib_cauchy_one_FnTy        = fn_d1;
    using distrib_cauchy_four_FnTy       = fn_d4;
    using distrib_chisquare_FnTy         = fn_d1;
    using distrib_chisquare_three_FnTy   = fn_d3;
    using distrib_exponential_FnTy       = fn_d1;
    using distrib_exponential_three_FnTy = fn_d3;
    using distrib_gamma_FnTy             = fn_d2;
    using distrib_gamma_four_FnTy        = fn_d4;
    using distrib_laplace_FnTy           = fn_d2;
    using distrib_laplace_one_FnTy       = fn_d1;
    using distrib_laplace_four_FnTy      = fn_d4;
    using distrib_lognormal_FnTy         = fn_d2;
    using distrib_lognormal_four_FnTy    = fn_d4;
    using distrib_poisson_FnTy           = fn_d1;
    using distrib_poisson_three_FnTy     = fn_d3;
    using distrib_rayleigh_FnTy          = fn_d1;
    using distrib_rayleigh_three_FnTy    = fn_d3;

    double distrib_uniform(Random *random, double _min, double _max);
    double distrib_normal(Random *random, double mu, double sigma);
    double distrib_normal_four(Random *random, double mu, double sigma, double _min, double _max);
    double distrib_bernoulli(Random *random, double prob);
    double distrib_binomial(Random *random, double nTrials, double probabilityOfSuccess);
    double distrib_binomial_four(Random *random, double nTrials, double probabilityOfSuccess, double _min, double _max);
    double distrib_cauchy(Random *random, double location, double scale);
    double distrib_cauchy_one(Random *random, double scale);
    double distrib_cauchy_four(Random *random, double location, double scale, double _min, double _max);
    double distrib_chisquare(Random *random, double degreesOfFreedom);
    double distrib_chisquare_three(Random *random, double degreesOfFreedom, double _min, double _max);
    double distrib_exponential(Random *random, double lambda);
    double distrib_exponential_three(Random *random, double lambda, double _min, double _max);
    double distrib_gamma(Random *random, double shape, double scale);
    double distrib_gamma_four(Random *random, double shape, double scale, double _min, double _max);
    double distrib_laplace(Random *random, double location, double scale);
    double distrib_laplace_one(Random *random, double scale);
    double distrib_laplace_four(Random *random, double location, double scale, double _min, double _max);
    double distrib_lognormal(Random *random, double mu, double sigma);
    double distrib_lognormal_four(Random *random, double mu, double sigma, double _min, double _max);
    double distrib_poisson(Random *random, double lambda);
    double distrib_poisson_three(Random *random, double lambda, double _min, double _max);
    double distrib_rayleigh(Random *random, double scale);
    double distrib_rayleigh_three(Random *random, double scale, double _min, double _max);


} /* namespace rrllvm */

#endif /* _RRLLVM_RANDOM_H_ */
