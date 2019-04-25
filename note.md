# Note

## Basic structure
### Define basic cost function
```cpp
// Simple concave maximize problem
struct concave_costFunctor
{
    // R = 10 - X_1^2 - X_2^2
    bool operator()(const double *const x, double *residual) const
    {
        residual[0] = 10.0 - x[0] * x[0] - x[1] * x[1];
        return true;
    }
};
```
* inputs and outputs are pointer
* return outputs as argument

### Set cost function's functor
``` cpp
    Problem problem;

    CostFunction *cost_functor = new NumericDiffCostFunction<concave_costFunctor, CENTRAL, 1, 2>(new concave_costFunctor);
    problem.AddResidualBlock(cost_functor, NULL, x);
```
* Several types of cost function
* Usually,
```
(cost fucntion type)<(cost function functor), (option), (number of residual), (dimention of parameter)>
```
* Set cost function and parameter that will be optimized with 'AddResidualBlock' method

### Set optimizing option
```cpp
Solver::Options options;
options.minimizer_progress_to_stdout = true;
```
* not only mathematical option but also other options like printout

### Optimizing
```
Solver::Summary summary;
Solve(options, &problem, &summary);

std::cout << summary.BriefReport() << "\n";
```
* Run Solve function with options, problem with cost function
* save result summery to 'summery'
