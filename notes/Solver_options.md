# Optimize(Solver) option
* Not all options
* Full option details are here : http://ceres-solver.org/nnls_solving.html#solver-options

## set options
* set options with Solver::Options
* example 
```cpp
Solver::Options options;
options.minimizer_progress_to_stdout = true;
```

### int Solver::Options::max_num_iterations
* Default: 50
* Maximum number of iterations for which the solver should run.

### double Solver::Options::max_solver_time_in_seconds
* Default: 1e6 
* Maximum amount of time for which the solver should run.

### int Solver::Options::num_threads
* Default: 1
* Number of threads used by Ceres to evaluate the Jacobian.

### PreconditionerType Solver::Options::preconditioner_type
* Default: JACOBI
* available options : IDENTITY, JACOBI, SCHUR_JACOBI, CLUSTER_JACOBI and CLUSTER_TRIDIAGONAL

### LoggingType Solver::Options::logging_type
* Default: PER_MINIMIZER_ITERATION

### bool Solver::Options::minimizer_progress_to_stdout
* Default: false
* print out progress to STDOUT
* __cost__ is the value of the objective function.
* __cost_change__ is the change in the value of the objective function if the step computed in this iteration is accepted.
* __|gradient|__ is the max norm of the gradient.
* __|step|__ is the change in the parameter vector.
* __tr_ratio__ is the ratio of the actual change in the objective function value to the change in the value of the trust region model.
* __tr_radius__ is the size of the trust region radius.
* __ls_iter__ is the number of linear solver iterations used to compute the trust region step. For direct/factorization based solvers it is always 1, for iterative solvers like ITERATIVE_SCHUR it is the number of iterations of the Conjugate Gradients algorithm.
* __iter_time__ is the time take by the current iteration.
* __total_time__ is the total time taken by the minimizer.

### bool Solver::Options::check_gradients
* Default: false
* Check all Jacobians computed by each residual block with finite differences. 
* expensive
* computing the derivative by normal means, then also computing it using finite differences. The results are compared

### double Solver::Options::gradient_check_relative_precision
* Default: 1e08
* Precision to check for in the gradient checker.
* If the relative difference between an element in a Jacobian exceeds this number, then the Jacobian for that cost term is dumped.

### bool Solver::Options::update_state_every_iteration
* Default: false
* the user’s parameter blocks are updated at the end of every Minimizer iteration
* otherwise they are updated when the Minimizer terminates.

### vector<IterationCallback> Solver::Options::callbacks
* Callbacks that are executed at the end of each iteration
* Should set __Solver::Options::update_state_every_iteration__

### bool Solver::Options::evaluation_callback
* Default: NULL
* If non-NULL, gets notified when Ceres is about to evaluate the residuals and/or Jacobians.

## Callback prototype
* For more detail about callbacks,
* http://ceres-solver.org/nnls_solving.html#evaluationcallback
* http://ceres-solver.org/nnls_solving.html#iterationcallback

### class IterationSummary
```cpp
class LoggingCallback : public IterationCallback {
 public:
  explicit LoggingCallback(bool log_to_stdout)
      : log_to_stdout_(log_to_stdout) {}

  ~LoggingCallback() {}

  CallbackReturnType operator()(const IterationSummary& summary) {
    const char* kReportRowFormat =
        "% 4d: f:% 8e d:% 3.2e g:% 3.2e h:% 3.2e "
        "rho:% 3.2e mu:% 3.2e eta:% 3.2e li:% 3d";
    string output = StringPrintf(kReportRowFormat,
                                 summary.iteration,
                                 summary.cost,
                                 summary.cost_change,
                                 summary.gradient_max_norm,
                                 summary.step_norm,
                                 summary.relative_decrease,
                                 summary.trust_region_radius,
                                 summary.eta,
                                 summary.linear_solver_iterations);
    if (log_to_stdout_) {
      cout << output << endl;
    } else {
      VLOG(1) << output;
    }
    return SOLVER_CONTINUE;
  }

 private:
  const bool log_to_stdout_;
};
```

### class EvaluationCallback
```cpp
class EvaluationCallback {
 public:
  virtual ~EvaluationCallback() {}
  virtual void PrepareForEvaluation()(bool evaluate_jacobians
                                      bool new_evaluation_point) = 0;
};
```
