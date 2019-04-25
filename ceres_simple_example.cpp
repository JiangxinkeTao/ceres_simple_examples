#include "ceres/ceres.h"

#include <iostream>
#include <vector>
#include <algorithm>

using ceres::CENTRAL;
using ceres::CostFunction;
using ceres::NumericDiffCostFunction;
using ceres::AutoDiffCostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;

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

void test_concave_cost_function()
{
    // initial value
    double x[2] = {0.5, 0.5};
    double initial_x[2];
    std::copy(x, x+2, initial_x);

    // build problem
    Problem problem;

    CostFunction *cost_functor = new NumericDiffCostFunction<concave_costFunctor, CENTRAL, 1, 2>(new concave_costFunctor);
    problem.AddResidualBlock(cost_functor, NULL, x);

    Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x[0] << ", " << initial_x[1]
              << " -> " << x[0] << ", " << x[1] << "\n";
}

// concave maximize problem with parameter a, b
int num_param = 5;
const double a[] = {11, 55, 22, 44, 66};
const double b[] = {22, 33, 77, 55, 66};

struct parameter_costFunctor
{
    parameter_costFunctor(double a, double b)
    : a_(a), b_(b)
    {}

    // R = 10 - a*X_1^2 - b*X_2^2
    template <typename T> bool operator()(const T *const x, T *residual) const
    {
        residual[0] = 10.0 - a_*x[0] * x[0] - b_*x[1] * x[1];
        return true;
    }

    private:
    const double a_;
    const double b_;
};

void test_parameter_costFunctor()
{
    // initial value
    double x[2] = {1.0, 1.0};
    double initial_x[2];
    std::copy(x, x+2, initial_x);

    // build problem
    Problem problem;

    for(int i = 0; i < num_param; i++)
    {
        CostFunction *cost_functor = new AutoDiffCostFunction<parameter_costFunctor, 1, 2>(new parameter_costFunctor(a[i], b[i]));
        problem.AddResidualBlock(cost_functor, NULL, x);
    }

    Solver::Options options;
    options.max_num_iterations = 25;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary convex_summary;
    Solve(options, &problem, &convex_summary);

    std::cout << convex_summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x[0] << ", " << initial_x[1]
              << " -> " << x[0] << ", " << x[1] << "\n";
}

int main(int argc, char **argv)
{
    test_concave_cost_function();
    test_parameter_costFunctor();
    return 0;
}
