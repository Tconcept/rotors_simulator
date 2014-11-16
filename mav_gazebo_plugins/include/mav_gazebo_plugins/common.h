/*
 * Copyright (C) 2014 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright (C) 2014 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright (C) 2014 Pascal Gohl, ASL, ETH Zurich, Switzerland
 * Copyright (C) 2014 Sammy Omari, ASL, ETH Zurich, Switzerland
 * Copyright (C) 2014 Markus Achtelik, ASL, ETH Zurich, Switzerland
 *
 * This software is released to the Contestants of the european 
 * robotics challenges (EuRoC) for the use in stage 1. (Re)-distribution, whether 
 * in parts or entirely, is NOT PERMITTED. 
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <gazebo/gazebo.hh>

namespace gazebo {

/**
 * \brief Obtains a parameter from sdf.
 * \param[in] sdf Pointer to the sdf object.
 * \param[in] name Name of the parameter.
 * \param[out] param Param Variable to write the parameter to.
 * \param[in] default_value Default value, if the parameter not available.
 * \param[in] verbose If true, gzerror if the parameter is not available.
 */
template<class T>
bool getSdfParam(sdf::ElementPtr sdf, const std::string& name, T& param, const T& default_value, const bool& verbose =
                     false) {
  if (sdf->HasElement(name)) {
    param = sdf->GetElement(name)->Get<T>();
    return true;
  }
  else {
    param = default_value;
    if (verbose)
      gzerr << "[mav_gazebo_plugins] Please specify a value for parameter \"" << name << "\".\n";
  }
  return false;
}

}




template <typename T> 
class FirstOrderFilter {
/*
This class can be used to apply a first order filter on a signal.
It allows different acceleration and deceleration time constants.

Short reveiw of discrete time implementation of firest order system:
Laplace:
    U(s)/X(s) = 1/(tau*s + 1)
continous time system: 
    dx(t) = (-1/tau)*x(t) + (1/tau)*u(t)
discretized system (ZoH): 
    x(k+1) = exp(samplingTime*(-1/tau))*x(k) + (1 - exp(samplingTime*(-1/tau))) * u(k)
*/

  public:
    FirstOrderFilter(double timeConstantUp, double timeConstantDown, T initialState):
      timeConstantUp_(timeConstantUp),
      timeConstantDown_(timeConstantDown),
      previousState_(initialState) {}

    T updateFilter(T inputState, double samplingTime) {
      /* 
      This method will apply a first order filter on the inputState.
      */
      T outputState;
      if(inputState > previousState_){
        //Accelerate
        double alphaUp = exp(- samplingTime / timeConstantUp_);
        // x(k+1) = Ad*x(k) + Bd*u(k) 
        outputState = alphaUp * previousState_ + (1 - alphaUp) * inputState;

      }else{
        //Decelerate
        double alphaDown = exp(- samplingTime / timeConstantDown_);
        outputState = alphaDown * previousState_ + (1 - alphaDown) * inputState;
      }
      previousState_ = outputState;
      return outputState;

    }
    ~FirstOrderFilter() {}

  protected:
    double timeConstantUp_;
    double timeConstantDown_;
    T previousState_;
};

#endif /* COMMON_H_ */




