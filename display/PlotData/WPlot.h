/*
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

#ifndef __WPlot_H__
#define __WPlot_H__

#include "libbrathl/brathl.h"


namespace brathl{

	class CInternalFiles;
	class CInternalFilesZFXY;
}

#include "PlotBase.h"

//#include "BitSet32.h"


using namespace brathl;


//-------------------------------------------------------------
//------------------- CWPlot class --------------------
//-------------------------------------------------------------
/** 
  A CWPlot object management class

 \version 1.0
*/
class CWPlot : public CPlotBase
{
public:
	CWPlot( uint32_t groupNumber = 0 );
	virtual ~CWPlot();

    virtual CInternalFiles* GetInternalFiles( CBratObject* ob, bool withExcept = true ) override;

	static CInternalFilesZFXY* GetInternalFilesZFXY( CBratObject* ob, bool withExcept = true );

public:

	virtual const std::string& BaseTitle() const override
	{
		static const std::string s = "BRAT World Plot";
		return s;
	}

    virtual void GetInfo() override;

protected:
	void Init();
};


#endif
