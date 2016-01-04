#include "stdafx.h"

#include "libbrathl/Product.h"
#include "new-gui/Common/+UtilsIO.h"
#include "new-gui/Common/QtUtilsIO.h"
#include "new-gui/brat/Workspaces/Workspace.h"

#include "ApplicationSettings.h"



bool CApplicationSettings::CheckVersion()
{	
	// lambdas

	//	- remove spaces from group names

	auto clean_groups = []( const std::string &s ) -> std::string
	{
		const size_t len = s.length();
		std::string new_s;
		for ( size_t i = 0; i < len; ++i )
		{
			new_s += s[ i ];
			if ( s[ i ] == '[' )
			{
				++i;
				for ( size_t j = i + 1; j < len; ++j )
				{
					if ( s[ j ] == ']' )
					{
						std::string group = s.substr( i, j - i + 1 );	//group name and the closing ']'
						new_s += replace( group, "\\ ", "" );
						i = j;
						break;
					}
				}
			}
		}
		return new_s;
	};

	//	- remove spaces from key names
	//	- put "" around predefined constant values containing commas 

	auto clean_keys = []( const std::string &s ) -> std::string
	{
		const size_t len = s.length();
		std::string new_s;
		for ( size_t i = 0; i < len; ++i )
		{
			new_s += s[ i ];
			if ( s[i] == '\n' )
			{
				if ( s[ i + 1 ] == '[' )	//we are at the beginning of a section (group) not of a key
					continue;
				++i;
				for ( size_t j = i + 1; j < len; ++j )
				{
					if ( s[ j ] == '=' )
					{
						std::string key = s.substr( i, j - i + 1 );	//key name and the closing '='
						new_s += replace( key, "\\ ", "" );
						i = j;
						break;
					}
				}
			}
		}
		return replace( replace( new_s, 
			VALUE_OPERATION_TypeZFXY,		"\"" + VALUE_OPERATION_TypeZFXY + "\"" ), 
			VALUE_DISPLAY_eTypeZFLatLon,	"\"" + VALUE_DISPLAY_eTypeZFLatLon + "\"" );
	};

    auto write_version_signature = [this]() -> bool
    {
        return WriteValues( GROUP_SETTINGS_VERSION,
        {
            { KEY_SETTINGS_VERSION, VersionValue() }
        }
        );
    };


	// -- function body --

	{
		CSection section( mSettings, GROUP_SETTINGS_VERSION );
		if ( mSettings.childKeys().empty() )
			mVersion = UnknownVersionValue();
		else
			mVersion = ReadValue( section, KEY_SETTINGS_VERSION );
	}

	if ( mVersion == VersionValue() )							//"our" format, no format check/update necessary
		return true;


	std::string new_path = q2a( mSettings.fileName() );			//the existing path, to save new contents

	if ( !IsFile( new_path ) )
        return write_version_signature();


	std::string s;
	if ( !Read2String( s, new_path ) )
		return false;

	std::string new_s = clean_keys( clean_groups( s ) );

	if ( s == new_s )
		return true;

	std::string old_path = CreateUniqueFileName( new_path );		//new path, to save old contents

	if ( old_path.empty() || !DuplicateFile( new_path, old_path ) )	//save "old" file
		return false;

	if ( !Write2File( new_s, new_path ) )						   	//save "new" file
		return false;

	Sync();

	return true;
}



void CApplicationSettings::Sync()
{
	mSettings.sync();
}


bool CApplicationSettings::SaveConfigSelectionCriteria()
{
	CMapProduct& mapProductInstance = CMapProduct::GetInstance();
	for ( CObMap::iterator it = mapProductInstance.begin(); it != mapProductInstance.end(); it++ )
	{
		CProduct* product = dynamic_cast<CProduct*>( it->second );
		if ( product == nullptr || !product->HasCriteriaInfo() )
			continue;

		std::string configPath = BuildComposedKey( { GROUP_SEL_CRITERIA, product->GetLabel() } );		
		{
			CSection section( mSettings, configPath );
			if ( product->IsSetLatLonCriteria() )
				WriteValue( section, ENTRY_LAT_LON, product->GetLatLonCriteria()->GetAsText() );

			if ( product->IsSetDatetimeCriteria() )
				WriteValue( section, ENTRY_DATETIME, product->GetDatetimeCriteria()->GetAsText() );

			if ( product->IsSetCycleCriteria() )
				WriteValue( section, ENTRY_CYCLE, product->GetCycleCriteria()->GetAsText() );

			if ( product->IsSetPassIntCriteria() )
				WriteValue( section, ENTRY_PASS_NUMBER, product->GetPassIntCriteria()->GetAsText() );

			if ( product->IsSetPassStringCriteria() )
				WriteValue( section, ENTRY_PASS_STRING, product->GetPassStringCriteria()->GetAsText() );
		}

		Sync();
	}

	return mSettings.status() == QSettings::NoError;
}



bool CApplicationSettings::LoadConfigSelectionCriteria()
{
	QStringList keys = mSettings.childGroups();
	if ( keys.empty() )
		return true;		qDebug() << keys;

	try {
		CMapProduct& mapProductInstance = CMapProduct::GetInstance();
		mapProductInstance.AddCriteriaToProducts();

		for ( CObMap::iterator it = mapProductInstance.begin(); it != mapProductInstance.end(); it++ )
		{
			CProduct* product = dynamic_cast<CProduct*>( it->second );

			if ( product == nullptr || !product->HasCriteriaInfo() )
				continue;

            std::string configPath = BuildComposedKey( { GROUP_SEL_CRITERIA, product->GetLabel() } );
			{
				CSection section( mSettings, configPath );
				QStringList keys = mSettings.childKeys();
				if ( keys.empty() )
					continue;			qDebug() << keys;

				std::string val;
				if ( product->HasLatLonCriteria() )
				{
					val = ReadValue( section, ENTRY_LAT_LON );
					if ( !val.empty() )
						product->GetLatLonCriteria()->Set( val );
				}
				if ( product->HasDatetimeCriteria() )
				{
					val = ReadValue( section, ENTRY_DATETIME );
					if ( !val.empty() )
						product->GetDatetimeCriteria()->SetFromText( val );
				}
				if ( product->HasCycleCriteria() )
				{
					val = ReadValue( section, ENTRY_CYCLE );
					if ( !val.empty() )
						product->GetCycleCriteria()->SetFromText( val );
				}
				if ( product->HasPassIntCriteria() )
				{
					val = ReadValue( section, ENTRY_PASS_NUMBER );
					if ( !val.empty() )
						product->GetPassIntCriteria()->SetFromText( val );
				}
				if ( product->HasPassStringCriteria() )
				{
					val = ReadValue( section, ENTRY_PASS_STRING );
					if ( !val.empty() )
						product->GetPassStringCriteria()->Set( val );
				}
			}
		}
	}
	catch ( const CException &e ) 
	{
		std::string msg( "An error occured while loading criteria selection configuration (CBratGuiApp::LoadConfigSelectionCriteria)\nNative exception: " );
		msg += e.what();
		throw CException( msg, e.error() );
	}

	return mSettings.status() == QSettings::NoError;
}

bool CApplicationSettings::SaveConfig( const CWorkspace *wks )
{
	m_lastWksPath = wks != nullptr ? wks->GetPath() : "";

	return
	WriteValues( GROUP_COMMON, 
	{ 
		{ ENTRY_USER_MANUAL, m_userManual },
		{ ENTRY_USER_MANUAL_VIEWER, m_userManualViewer },
		{ ENTRY_LAST_DATA_PATH, m_lastDataPath },
		{ ENTRY_LAST_PAGE_REACHED, m_lastPageReached },
	} 
	)
	&&
	WriteValues( GROUP_WKS, 
	{ 
		{ ENTRY_LAST, m_lastWksPath },
	} 
	)
	&&
	WriteValues( GROUP_COLORTABLE, 
	{ 
		{ ENTRY_LAST, m_lastColorTable },
	} 
	)
	&&
	SaveConfigSelectionCriteria();
}

bool CApplicationSettings::LoadConfig()
{
	return
	( mSettings.status() == QSettings::NoError )	//just in case check; LoadConfig is typically called only once and before any other settings use
	&&
	ReadValues( GROUP_COMMON, 
	{ 
		{ ENTRY_USER_MANUAL, m_userManual },
		{ ENTRY_USER_MANUAL_VIEWER, m_userManualViewer },
		{ ENTRY_LAST_DATA_PATH, m_lastDataPath },
		{ ENTRY_LAST_PAGE_REACHED, m_lastPageReached },
	} 
	)
	&&
	ReadValues( GROUP_WKS, 
	{ 
		{ ENTRY_LAST, m_lastWksPath },
	} 
	)
	&&
	ReadValues( GROUP_COLORTABLE, 
	{ 
		{ ENTRY_LAST, m_lastColorTable },
	} 
	)
	&&
	LoadConfigSelectionCriteria();
}



//////////////////////////////////////////////////////////////////////////////////////////
//	TODO: delete: only for brat v3 support to v4 development
//////////////////////////////////////////////////////////////////////////////////////////

template< class CONTAINER >
bool CApplicationSettings::SaveRecentFilesGeneric( const CONTAINER &paths )
{
	auto brathlFmtEntryRecentWksMacro = []( size_t index ) -> std::string
	{
		return KEY_WKS_RECENT + n2s<std::string>( index );
	};

	//

	{
		CSection section( mSettings, GROUP_WKS_RECENT );
		const size_t size = paths.size();
		for ( size_t i = 0; i < size; ++i )
			WriteValue( section, brathlFmtEntryRecentWksMacro( i ), paths[ (typename CONTAINER::size_type) i ] );
	}

	return mSettings.status() == QSettings::NoError;
}


bool CApplicationSettings::SaveRecentFiles( const std::vector<std::string> &paths )
{
	return SaveRecentFilesGeneric( paths );
}
bool CApplicationSettings::SaveRecentFiles( const QStringList &paths )
{
	return SaveRecentFilesGeneric( paths );
}


template< class CONTAINER >
bool CApplicationSettings::LoadRecentFilesGeneric( CONTAINER &paths )
{
	long maxEntries = 0;

	{
		CSection section( mSettings, GROUP_WKS_RECENT );
		auto const &keys = mSettings.childKeys();
        maxEntries = keys.size();
		for ( auto const &key : keys )
		{
			auto value = ReadValue< typename CONTAINER::value_type >( section, q2t< std::string >( key ) );
			paths.push_back( value );
		};
	}

	assert__( paths.size() == maxEntries );			Q_UNUSED( maxEntries );		//for release builds

	return mSettings.status() == QSettings::NoError;
}

bool CApplicationSettings::LoadRecentFiles( std::vector<std::string> &paths )
{
	return LoadRecentFilesGeneric( paths );
}
bool CApplicationSettings::LoadRecentFiles( QStringList &paths )
{
	return LoadRecentFilesGeneric( paths );
}