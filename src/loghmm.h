


#ifndef LogHMM_H
#define LogHMM_H

#include "utility.h"
#include "densities.h"
#include <cmath>
#include <R.h> // R_CheckUserInterrupt()
#include <vector> // storing density functions
#include <time.h> // time(), difftime()

// #if defined TARGET_OS_MAC || defined __APPLE__
// #include <libiomp/omp.h> // parallelization options on mac
// #elif defined __linux__ || defined _WIN32 || defined _WIN64
// #include <omp.h> // parallelization options
// #endif

class LogHMM  {

	public:
		// Constructor and Destructor
		LogHMM(int T, int N);
		~LogHMM();

		// Member variables
		std::vector<Density*> densityFunctions; ///< density functions for each state

		// Methods
		void initialize_transition_probs(double* initial_A, bool use_initial_params);
		void initialize_proba(double* initial_proba, bool use_initial_params);
		void EM(int* maxiter, int* maxtime, double* eps);
		void calc_weights(double* weights);

		// Getters and Setters
		void get_posteriors(double** post);
		double get_posterior(int iN, int t);
		double get_proba(int i);
		double get_A(int i, int j);
		double get_logP();
		void set_cutoff(int cutoff);

	private:
		// Member variables
		int T; ///< length of observed sequence
		int N; ///< number of states
		int cutoff; ///< a cutoff for observations
		double* sumgamma; ///< vector[N] of sum of posteriors (gamma values)
		double** sumxi; ///< matrix[N x N] of xi values
		double** gamma; ///< matrix[N x T] of posteriors
		double logP; ///< loglikelihood
		double dlogP; ///< difference in loglikelihood from one iteration to the next
		// Scaling approach
			double** A; ///< matrix [N x N] of transition probabilities
			double* proba; ///< initial probabilities (length N)
		// Log approach
			double** logA; ///< matrix [N x N] of transition probabilities
			double* logproba; ///< initial probabilities (length N)
			double** logalpha; ///< matrix [T x N] of forward probabilities
			double** logbeta; ///<  matrix [T x N] of backward probabilities
			double** logdensities; ///< matrix [N x T] of density values
		// Miscellany
			time_t EMStartTime_sec; ///< start time of the EM in sec
			int EMTime_real; ///< elapsed time from start of the 0th iteration
			int sumdiff_state_last; ///< sum of the difference in the state 1 assignments from one iteration to the next
			double sumdiff_posterior; ///< sum of the difference in posterior (gamma) values from one iteration to the next
	// 		bool use_tdens; ///< switch for using the tdensities in the calculations

		// Methods
		void forward(); ///< calculate forward variables (alpha)
		void backward(); ///< calculate backward variables (beta)
		void calc_sumgamma();
		void calc_sumxi();
		void calc_loglikelihood();
		void calc_densities();
		void print_uni_iteration(int iteration);
		void print_uni_params();
};

#endif
