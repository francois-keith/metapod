// Copyright 2012,
//
// Olivier STASSE
//
// LAAS, CNRS
//
// This file is part of metapod.
// metapod is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// metapod is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Lesser Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with metapod.  If not, see <http://www.gnu.org/licenses/>.(2);


#ifndef METAPOD_SPATIAL_ALGEBRA_CONSTRAINT_MOTION_ANYAXIS_HH
# define METAPOD_SPATIAL_ALGEBRA_CONSTRAINT_MOTION_ANYAXIS_HH

namespace metapod
{

  namespace Spatial
  {
    // Class of motion constraint with a rotation around a general axis.
    template<class FloatType>
    class ConstraintMotionAnyAxis
    {
      EIGEN_METAPOD_TYPEDEFS;
      public:
        // Constructors
      ConstraintMotionAnyAxis(FloatType axisx,
                              FloatType axisy,
                              FloatType axisz): m_S(Vector6d::Zero())
      { m_S(0) = axisx; m_S(1) = axisy; m_S(2) = axisz;}

      Vector6d operator*(double d) const;

      private:
        Vector6d m_S;

      public:
      const Vector6d & S() const {return m_S;}
      Vector6dt transpose() const {return m_S.transpose();}
    };

    template <class FloatType>
    inline class Vector6dTpl<FloatType>::Type 
    ConstraintMotionAnyAxis<FloatType>::operator*
    (double x) const
    {
      Vector6d tmp = Vector6d::Zero();
      tmp.segment<3>(0) = x*m_S.segment<3>(0);
      return tmp;
    }

    template <class FloatType>
    inline class Vector6dTpl<FloatType>::Type operator*(const InertiaTpl<FloatType> & m,
                                                        const ConstraintMotionAnyAxis<FloatType> & a)
    {
      EIGEN_METAPOD_TYPEDEFS;
      Vector6d r;
      const Vector6d &altI = m.I().m_ltI;
      r[0] = altI(0)*a.S()[0] + altI(1)*a.S()[1] + altI(3)*a.S()[2];
      r[1] = altI(1)*a.S()[0] + altI(2)*a.S()[1] + altI(4)*a.S()[2];
      r[2] = altI(3)*a.S()[0] + altI(4)*a.S()[1] + altI(5)*a.S()[2];

      Matrix3d msh = -skew<FloatType>(m.h());
      for(unsigned int i=0; i<3; ++i)
        r[i+3] = msh(i,0)*a.S()[0]+
                 msh(i,1)*a.S()[1]+
                 msh(i,2)*a.S()[2];
      return r;
    }
  }
}

#endif
