#include "new-gui/brat/stdafx.h"

#include "QtInterface.h"

#include "Common/QtUtils.h"

#include "new-gui/brat/DataModels/Workspaces/Dataset.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										CBratLookupTable	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										CBratLookupTable - Load
//////////////////////////////////////////////////////////////////////////////////////////////////////////


bool LoadFromFile( CBratLookupTable &lut, const std::string& fileName )
{
	Q_UNUSED(lut);
	Q_UNUSED(fileName);

	SimpleWarnBox("Development message: LoadFromFile( CBratLookupTable &lut, const std::string& fileName ) called");
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										CBratLookupTable - Store
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void SaveToFile( CBratLookupTable &lut, const std::string& fileName )
{
	Q_UNUSED(lut);
	Q_UNUSED(fileName);

	SimpleWarnBox("Development message: SaveToFile( CBratLookupTable &lut, const std::string& fileName ) called");
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											File Names
//////////////////////////////////////////////////////////////////////////////////////////////////////////


std::string GetFileName( const std::string &path )
{
	return GetBaseFilenameFromPath( path );
}

void SetFileExtension( std::string &path, const std::string &extension )
{
	std::string dir = GetDirectoryFromPath( path );
	std::string name = GetFilenameFromPath( path );
	path = path + "/" + name + "." + extension;
}

std::string NormalizedPath( const std::string &path, const std::string &dir )
{
	std::string d = NormalizedPath( dir );
	std::string f = NormalizedPath( path );
	if ( QDir::isAbsolutePath( t2q( f ) ) )
		return f;
	else
		return dir + "/" + path;
}
