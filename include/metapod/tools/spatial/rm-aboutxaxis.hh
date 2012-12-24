// Copyright 2012,
//
// Olivier Stasse
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


#ifndef METAPOD_SPATIAL_ALGEBRA_ROTATION_MATRIX_ABOUTX_HH
# define METAPOD_SPATIAL_ALGEBRA_ROTATION_MATRIX_ABOUTX_HH

namespace metapod
{

  namespace Spatial
  {


    struct RotationMatrixAboutX
    {
      double m_c,m_s;

      RotationMatrixAboutX(): 
	m_c(0.0),m_s(0.0) 
      {}

      RotationMatrixAboutX(const Matrix3d &aR)
      {
	m_c=aR(1,1);m_s=aR(1,2);
      }

      RotationMatrixAboutX(double c, double s)
      {
	m_c=c;m_s=s;
      }

      /** \brief Random initialization */
      void randomInit()
      {
	boost::mt19937 rng;
	boost::uniform_real<> urd(-3.14, 3.14);

	FloatType theta_x=urd(rng);
	set(theta_x);
      }

      RotationMatrixAboutX transpose() const
      {
	return RotationMatrixAboutX(m_c,-m_s);
      }

      RotationMatrixAboutX operator*(FloatType a) const
      {
	return RotationMatrixAboutX(a*m_c,a*m_s);
      }

      RotationMatrixAboutX operator-() const
      {
	return RotationMatrixAboutX(-m_c,-m_s);
      }

      void set(FloatType theta)
      { m_c = cos(theta); m_s=sin(theta);}

      Matrix3d toMatrix()
      {
	Matrix3d r;
	r(0,0) = 1.0; r(0,1) = 0.0; r(0,2) = 0.0;
	r(1,0) = 0.0; r(1,1) = m_c;   r(1,2) = m_s;
	r(2,0) = 0.0; r(2,1) = -m_s;  r(2,2) = m_c;
	return r;
      }

      Matrix3d operator*(const Matrix3d &A) const
      {
	Matrix3d r;
	r.block<1,3>(0,0) = A.block<1,3>(0,0);

	for(unsigned int i=0;i<3;i++)
	  r(1,i) =  A(1,i) * m_c + A(2,i) * m_s;
	
	for(unsigned int i=0;i<3;i++)
	  r(2,i) = -A(1,i) * m_s + A(2,i) * m_c;
	
	return r;
      }

      RotationMatrix operator*(const RotationMatrix &aRM) const
      {
	Matrix3d r;
	const Matrix3d &lrm = aRM.m_rm;

	r.block<1,3>(0,0) = lrm.block<1,3>(0,0);

	for(unsigned int i=0;i<3;i++)
	  r(1,i) = lrm(1,i) * m_c + lrm(2,i) * m_s;
	
	for(unsigned int i=0;i<3;i++)
	  r(2,i) = -lrm(1,i) * m_s + lrm(2,i) * m_c;

	return RotationMatrix(r);
      }

      RotationMatrixAboutX operator*(const RotationMatrixAboutX &aRM) const
      {
	double lc,ls;
	lc = m_c * aRM.m_c - m_s * aRM.m_s;
	ls = m_c * aRM.m_s + m_s * aRM.m_c;
	// ls2 = -m_s * aRM.m_c - m_c * aRM.m_s;
	// lc2 = -m_s * aRM.m_s +  m_c *aRM.m_c;
	return RotationMatrixAboutX(lc,ls);
      }

      Matrix3d  rotGeneralMatrix(const Matrix3d &A) const
      {
	Matrix3d r;

	FloatType alpha_x = m_c*m_s*(A(1,2)+A(2,1)) +
	  m_s*m_s*(A(2,2) - A(1,1));

	FloatType beta_x = m_c*m_s*(A(2,2)- A(1,1)) -
	  m_s*m_s*(A(1,2) + A(2,1));

	r(0,0) = A(0,0); 
	r(0,1) = m_c*A(0,1) + m_s*A(0,2);
	r(0,2) = m_c*A(0,2) - m_s*A(0,1);

	r(1,0) = m_c*A(1,0) + m_s*A(2,0);
	r(1,1) = A(1,1) + alpha_x;
	r(1,2) = A(1,2) + beta_x;

	r(2,0) = m_c*A(2,0) - m_s*A(1,0);
	r(2,1) = A(2,1) + beta_x;
	r(2,2) = A(2,2) - alpha_x;
	return r;
      }

      /** \brief Compute the rotation for a symmetric matrix.
       */
      struct ltI rotSymmetricMatrix(const struct ltI &A)
      {
	struct ltI r;
	FloatType alpha_x = 2*m_c*m_s*A.m_ltI(4) +
	  m_s*m_s*(A.m_ltI(5) - A.m_ltI(2));
	FloatType beta_x  = m_c*m_s*(A.m_ltI(5) - A.m_ltI(2))+
	  (1-2*m_s*m_s)*A.m_ltI(4);

	r.m_ltI(0) = A.m_ltI(0);
	r.m_ltI(1) = m_c*A.m_ltI(1) + m_s*A.m_ltI(3); 
	r.m_ltI(2) = A.m_ltI(2) + alpha_x;
	r.m_ltI(3) = m_c*A.m_ltI(3) - m_s*A.m_ltI(1);
	r.m_ltI(4) = beta_x;
	r.m_ltI(5) = A.m_ltI(5) - alpha_x;
	return r;

      }      

      friend std::ostream & operator<<(std::ostream &os,
				       const struct RotationMatrixAboutX & aRMAX)
      {
	os << "1.0 0.0 0.0" <<  endl;
	os << "0.0 " <<  aRMAX.m_c << " " << aRMAX.m_s << endl;
	os << "0.0 " << -aRMAX.m_s << " " << aRMAX.m_c << endl;
	return os;
      }
      
      
      
    };
  
  } // end Spatial namespace
} // end metapod namespace

#endif // METAPOD_SPATIAL_ALGEBRA_ROTATION_MATRIX_ABOUTX_HH
