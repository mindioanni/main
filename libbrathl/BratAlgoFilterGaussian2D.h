/*
* 
*
* This file is part of BRAT 
*
* BRAT is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* BRAT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#if !defined(_BratAlgoFilterGaussian2D_h_)
#define _BratAlgoFilterGaussian2D_h_

#include <string>

#include "common/tools/brathl_error.h"
#include "brathl.h" 

#include "Tools.h" 
#include "BratAlgoFilterGaussian.h" 


using namespace brathl;

namespace brathl
{
//-------------------------------------------------------------
//------------------- CBratAlgoFilterGaussian2D class --------------------
//-------------------------------------------------------------


/** \addtogroup Algorithm Algorithms classes
  @{ */

/** 
  Algorithm base class. 
*/

class CBratAlgoFilterGaussian2D : public CBratAlgoFilterGaussian 
{
public:
  /** Default contructor */
  CBratAlgoFilterGaussian2D();
  /** Copy contructor */
  CBratAlgoFilterGaussian2D(const CBratAlgoFilterGaussian2D	&copy);

  /** Destructor */
  virtual ~CBratAlgoFilterGaussian2D();

  virtual std::string GetName() const override { return "BratAlgoFilterGaussianGrid"; }

  virtual std::string GetDescription() const override { return "Gaussian filter for two-dimensional data (e.g. gridded data) as input data source. "
                                           "Gaussian filter is windowed filter of linear class, by its nature is weighted mean. "
                                           "Gaussian filter is a type of data-smoothing filter that uses a Gaussian function (normal distribution) for calculating the transformation to apply to each data. "
                                           "The output value unit depends on the variable (data) filtered" ; }

  virtual double Run(CVectorBratAlgorithmParam& args) override;


    /** Overloads operator '=' */
  CBratAlgoFilterGaussian2D& operator=(const CBratAlgoFilterGaussian2D &copy);

  virtual uint32_t GetDataWindowSize() override { return m_dataWindowLength * m_dataWindowLength; }

  /** Dump function */
  virtual void Dump(std::ostream& fOut = std::cerr) override;
 

protected:

  void Init();
  void Set(const CBratAlgoFilterGaussian2D &copy);

  virtual void OpenProductFile() override;
  void CheckProduct();
  virtual void CheckVarExpression(uint32_t index) override;

  virtual void SetBeginOfFile() override;
  virtual void SetEndOfFile() override;
  
  double ComputeSingle();
  double ComputeMean();

  virtual double ComputeGaussian(CExpressionValue& exprValue);



protected:


};


/** @} */
}


#endif // !defined(_BratAlgoFilterGaussian2D_h_)
