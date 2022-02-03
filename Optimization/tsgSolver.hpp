/*
 * Copyright (c) 2022, Miroslav Stoyanov & Weiwei Kong
 *
 * This file is part of
 * Toolkit for Adaptive Stochastic Modeling And Non-Intrusive ApproximatioN: TASMANIAN
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * UT-BATTELLE, LLC AND THE UNITED STATES GOVERNMENT MAKE NO REPRESENTATIONS AND DISCLAIM ALL WARRANTIES, BOTH EXPRESSED AND
 * IMPLIED. THERE ARE NO EXPRESS OR IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF
 * THE SOFTWARE WILL NOT INFRINGE ANY PATENT, COPYRIGHT, TRADEMARK, OR OTHER PROPRIETARY RIGHTS, OR THAT THE SOFTWARE WILL
 * ACCOMPLISH THE INTENDED RESULTS OR THAT THE SOFTWARE OR ITS USE WILL NOT RESULT IN INJURY OR DAMAGE. THE USER ASSUMES
 * RESPONSIBILITY FOR ALL LIABILITIES, PENALTIES, FINES, CLAIMS, CAUSES OF ACTION, AND COSTS AND EXPENSES, CAUSED BY, RESULTING
 * FROM OR ARISING OUT OF, IN WHOLE OR IN PART THE USE, STORAGE OR DISPOSAL OF THE SOFTWARE.
 */

#ifndef __TASMANIAN_SOLVER_HPP
#define __TASMANIAN_SOLVER_HPP

#include "tsgOptimizationEnumerates.hpp"

namespace TasOptimization {

// A function that evaluates a single point.
using ObjectiveFunction = std::function<double(const std::vector<double> &x)>;

class Solver {
  public:
    Solver();
    Solver(ObjectiveFunction input_fn, std::vector<double> &input_x);
    ~Solver();

    std::vector<double> getX() {return x;}
    std::vector<double> getLowerBounds() {return lower;}
    std::vector<double> getUpperBounds() {return upper;}
    ObjectiveFunction getObjectiveFunction() {return f;}
    SolverStatus getStatus() {return status;}
    size_t getNumDimensions() {return num_dimensions;}
    int getIterationCount() {return num_iterations;}
    int getRuntime() {return runtime;}
    double getObjectiveValue() {return f(x);}

    void checkIterationCount() {if (num_iterations >= max_num_iterations) {status = iteration_limit; return;}}
    void checkRuntime() {if (runtime >= max_runtime) {status = time_limit; return;}}
    void checkInfeasibility() {if (!is_feasible()) {status = infeasible; return;};}
    void checkOptimality() {if (is_optimal()) {status = optimal; return;};}
    void checkAll() {checkIterationCount(); checkRuntime(); checkInfeasibility(); checkOptimality();}

    void setX(std::vector<double> &input_x);
    void setLowerBounds(std::vector<double> &input_lower);
    void setUpperBounds(std::vector<double> &input_upper);
    void setObjectiveFunction(ObjectiveFunction input_f) {f = input_f;}
    void setMaxIterationCount(int limit) {max_num_iterations = limit;}
    void setMaxRuntime(double limit) {max_runtime = limit;}

    void addIterations(int k) {num_iterations += k; checkIterationCount();}
    void addRuntime(double t) {runtime += t; checkRuntime();}

    // Main algorithm that minimizes f using the current candidate point x.
    virtual bool is_optimal() = 0;
    virtual bool is_feasible() = 0;
    virtual void optimize() = 0;

  private:
    ObjectiveFunction f;
    std::vector<double> x, lower, upper;
    size_t num_dimensions;
    int num_iterations, max_num_iterations;
    double runtime, max_runtime;
    SolverStatus status;
};

}

#endif
