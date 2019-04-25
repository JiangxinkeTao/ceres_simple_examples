# Cost function types

## Fixed sized parameter Cost function
* Size of parameter, number of residuals should be fixed at create cost function functor
### numeric_diff_cost_function
* Compute Jacobian with Numeric differentiation
* Detail :http://en.wikipedia.org/wiki/Numerical_differentiation.
* prototype
```cpp
   class MyScalarCostFunctor {
     explicit MyScalarCostFunctor(double k): k_(k) {}

     bool operator()(const double* const x,
                     const double* const y,
                     double* residuals) const {
       residuals[0] = k_ - x[0] * y[0] - x[1] * y[1];
       return true;
     }

    private:
     double k_;
   };
```
* cost function creation
```cpp
   CostFunction* cost_function
       = new NumericDiffCostFunction<MyScalarCostFunctor, CENTRAL, 1, 2, 2>(
           new MyScalarCostFunctor(1.0));                    ^     ^  ^  ^
                                                             |     |  |  |
                                 Finite Differencing Scheme -+     |  |  |
                                 Dimension of residual ------------+  |  |
                                 Dimension of x ----------------------+  |
                                 Dimension of y -------------------------+
```
### autodiff_cost_function
* Compute Jacobian with Automatic differentiation
* Need template declaration at define cost functor
* Detail : http://en.wikipedia.org/wiki/Automatic_differentiation
* prototype
```cpp
   class MyScalarCostFunctor {
     MyScalarCostFunctor(double k): k_(k) {}

     template <typename T>
     bool operator()(const T* const x , const T* const y, T* e) const {
       e[0] = T(k_) - x[0] * y[0] + x[1] * y[1];
       return true;
     }

    private:
     double k_;
   };
```
* cost function creation
```cpp
   CostFunction* cost_function
       = new AutoDiffCostFunction<MyScalarCostFunctor, 1, 2, 2>(
            new MyScalarCostFunctor(1.0));             ^  ^  ^
                                                       |  |  |
                            Dimension of residual -----+  |  |
                            Dimension of x ---------------+  |
                            Dimension of y ------------------+
```
### conditioned_cost_function
* Sometimes, need to apply more weight to specific residuals
* Use this functor when need more detail than other cost functor
* cost function creation
```cpp
   // my_cost_function produces N residuals
   CostFunction* my_cost_function = ...
   CHECK_EQ(N, my_cost_function->num_residuals());
   vector<CostFunction*> conditioners;

   // Make N 1x1 cost functions (1 parameter, 1 residual)
   CostFunction* f_1 = ...
   conditioners.push_back(f_1);
   ...
   CostFunction* f_N = ...
   conditioners.push_back(f_N);
   ConditionedCostFunction* ccf =
     new ConditionedCostFunction(my_cost_function, conditioners);

   // Now ccf's residual i (i=0..N-1) will be passed though the i'th conditioner.

   ccf_residual[i] = f_i(my_cost_function_residual[i])

  // and the Jacobian will be affected appropriately.
```
## Variable sized(dynamic) parameter Cost function
* These can change parameter numbers and size later
* To set paramter number and size, need more setup functions

### dynamic_autodiff_cost_function
* prototype
```cpp
   struct MyCostFunctor {
     template<typename T>
     bool operator()(T const* const* parameters, T* residuals) const {
       // Use parameters[i] to access the i'th parameter block.
     }
   };
```
* Creation and setup parameters
```cpp
   DynamicAutoDiffCostFunction<MyCostFunctor, 3> cost_function(
       new MyCostFunctor());
   cost_function.AddParameterBlock(5);
   cost_function.AddParameterBlock(10);
   cost_function.SetNumResiduals(21);
```

### dynamic_numeric_diff_cost_function
* prototype
```cpp
   struct MyCostFunctor {
     bool operator()(double const* const* parameters, double* residuals) const {
       // Use parameters[i] to access the i'th parameter block.
     }
   }
```
* Creation and setup parameters
```cpp
   DynamicAutoDiffCostFunction<MyCostFunctor, CENTRAL> cost_function(
       new MyCostFunctor());
   cost_function.AddParameterBlock(5);
   cost_function.AddParameterBlock(10);
   cost_function.SetNumResiduals(21);
```
