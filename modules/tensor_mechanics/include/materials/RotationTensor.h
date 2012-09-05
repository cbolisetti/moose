/**
 * RotationTensor creates a RealTensorValue rotation tensor according to three Euler Angles
 * Original class authors: M.R. Tonks
 */

#ifndef ROTATIONTENSOR_H
#define ROTATIONTENSOR_H

// Any requisite includes here
#include "tensor_value.h"
#include "vector_value.h"

class RotationTensor : public RealTensorValue
{
public:

 /**
  * Default constructor; fills according to Euler angles
  */
  RotationTensor(const RealVectorValue & Euler_angles);

protected:
  
private:
  
};


#endif //ROTATIONTENSOR_H
