#ifndef WX_DISPLAY_INTERFACE_H
#define WX_DISPLAY_INTERFACE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/colourdata.h>



#include "../gui/wxGuiInterface.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										CBratLookupTable	
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PlotData/BratLookupTable.h"


//void CVtkColor::Get(wxColour& color) const
//{
//  if (Ok() == false)
//  {
//    return;
//  }
//  uint8_t r;
//  uint8_t g;
//  uint8_t b;
//  uint8_t a;
//  Get(r, g, b, a);
//  color.Set(r,g,b);
//}
//wxColour CVtkColor::GetWXColor() const
//{
//  wxColour color;
//  Get(color);
//  return color;
//}
inline wxColour color_cast( const CVtkColor &c )
{
	wxColour color;
	if ( c.Ok() )
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		c.Get( r, g, b, a );
		color.Set( r, g, b );
	}
	return color;
}

//void CVtkColor::Set(const wxColour& color)
//{
//  Set(color.Red(), color.Green(), color.Blue());
//}
//CVtkColor::CVtkColor(const wxColour& color)
//{
//  Set(color);
//}
inline CVtkColor color_cast( const wxColour &color )
{
	CVtkColor c;
	c.Set(color.Red(), color.Green(), color.Blue());
	return c;
}


//void CVtkColor::Set(wxColourData& colorData, int32_t indexCustomColor)
//{
//  if (indexCustomColor < 0)
//  {
//    Set(colorData.GetColour());
//  }
//  else
//  {
//    Set(colorData.GetCustomColour(indexCustomColor));
//  }
//}
//const CVtkColor& CVtkColor::operator =(wxColourData& color)
//{
//  Set(color);
//  return *this;
//}
inline CVtkColor color_cast( const wxColourData &colorData, int32_t indexCustomColor = -1 )
{
	if ( indexCustomColor < 0 )
	{
		return color_cast(colorData.GetColour());
	}
	else
	{
		return color_cast(colorData.GetCustomColour( indexCustomColor ) );
	}
}




//
//	CPlotField					=> CBratObject
//	CPlot		=> CPlotBase	=> CBratObject
//	CWPlot		=> CPlotBase
//	CZFXYPlot	=> CPlotBase
//
bool LoadFromFile( CBratLookupTable &lut, const std::string& fileName );

void SaveToFile( CBratLookupTable &lut, const std::string& fileName );



#endif		//WX_DISPLAY_INTERFACE_H