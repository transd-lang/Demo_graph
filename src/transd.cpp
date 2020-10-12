/*
This is a distributed file for Transd programming language embedding
library.

Copyright (c) 2020 Albert Berger [alberger at gmail com].
All rights reserved.

Redistribution and use of this file in source and binary forms, 
without modification, are permitted. Any modifications to this file
are explicitly forbidden.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/*
	transd.cpp
	----------
	The minimized distributed source file for C++ binding embeddable
	library of Transd programming language.

	Transd Project website: https://github.com/transd-lang
*/

#ifdef __LINUX__
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#endif
#include <regex>
#include <climits>
#include <cerrno>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

#include "transd.hpp"

using namespace std;


namespace s4 {
uint32_t s5::s16;

void kexceptInit()
{
	s5::s233();
}

}
using namespace s4;
using s4::s5;


namespace s17 {

time_t s20( const std::wstring& f )
{
	int err;
#ifndef WIN32
	struct stat st;
	err = stat( U8( f ).c_str(), &st );
#else
	struct _stat st;
	err = _wstat( f.c_str(), &st );
#endif
	if( err )
		throw s5( L"cannot get mtime of: " + f, 
		(uint32_t)s5::s6::s12 );
	return st.st_mtime;
}

uint8_t s21( const std::wstring& f )
{
	int err;
#ifndef WIN32
	struct stat st;
	err = stat( U8( f ).c_str(), &st );
#else
	struct _stat st;
	err = _wstat( f.c_str(), &st );
#endif

	if( err )
		throw s5( L"file system object does not exist: " + f,
		(uint32_t)s5::s6::s12 );

	if( st.st_mode & S_IFREG )
		return 1;
	else if( st.st_mode & S_IFDIR )
		return 2;

	return 0;
}

bool s23( const wstring& s387 )
{
	try {
		s21( s387 );
	}
	catch( s5& ) {
		return false;
	}
	return true;
}

bool s22( const wstring& s387 )
{
	try {
		if( s21( s387 ) != 1 )
			throw s5( L"Cannot find data file: " + s387,
			(uint32_t)s5::s6::s12 );
	}
	catch( s5& e ) { // file don't exist
		if( e.which() != (uint32_t)s5::s6::s12 )
			throw;
		return false;
	}
	return true;
}

} // namespace s17

#ifdef __LINUX__

using namespace std;
using namespace s4;
using namespace s43;


namespace s17 {

wstring s24()
{
	char buf[1024];
	readlink( "/proc/self/exe", buf, 1023 );
	return wstring( conv.from_bytes( buf ) );
}

bool s18( const wstring& dir, const wstring& s388, wstring& res )
{
	struct dirent *entry;
	DIR *dp;
	wregex rg( s388 );
	bool ret = false;

	dp = opendir( conv.to_bytes( dir.c_str() ).c_str() );
	if( dp == NULL ) {
		wstring errmsg = conv.from_bytes( strerror( errno ) );
		throw s5( L"cannot open directory: " + dir + L";\nerror: " + errmsg,
				(uint32_t)s5::s6::s11 );
	}

	while( ( entry = readdir( dp ) ) ) {
		if( regex_match( conv.from_bytes( entry->d_name ), rg ) ) {
			res = dir + L"/" + conv.from_bytes( entry->d_name );
			ret = true;
			break;
		}
	}

	closedir( dp );
	return ret;
}

void s19( const wstring& dir, const wstring& s388, vector<wstring>& files, vector<wstring>& dirs )
{
	struct dirent *entry;
	DIR *dp;
	wregex rg( s388 );

	dp = opendir( conv.to_bytes( dir.c_str() ).c_str() );
	if( dp == NULL ) {
		wstring errmsg = conv.from_bytes( strerror( errno ) );
		throw s5( L"cannot open directory: " + dir + L";\nerror: " + errmsg,
				(uint32_t)s5::s6::s11 );
	}

	while( ( entry = readdir( dp ) ) ) {
		if( regex_match( conv.from_bytes( entry->d_name ), rg ) ) {
			wstring item = conv.from_bytes( entry->d_name );
			wstring s = dir + L"/" + item;
			uint8_t is = s21( s );
			if( is == 1 )
				files.push_back( item );
			else if( is == 2 )
				dirs.push_back( item );
		}
	}

	closedir( dp );
}


} // namespace s17

#endif // __LINUX__

#ifdef WIN32

using namespace std;

#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


namespace s17 {

wstring s24()
{
	wchar_t dest[1024];
	DWORD length = GetModuleFileNameW( NULL, dest, 1024 );
	PathRemoveFileSpecW( dest );
	return dest;
}

bool s18( const wstring& _dir, const wstring& s388, wstring& res )
{
	HANDLE hDir;
	WIN32_FIND_DATAW s389;
	wstring s390, s391;
	wregex rg( s388.c_str() );
	bool ret = false;
	wstring dir = _dir;
	if( dir.back() != L'/' && dir.back() != L'\\' )
		dir += L'\\';

	if( ( hDir = FindFirstFileW( ( dir + L"*" ).c_str(), &s389 ) ) == INVALID_HANDLE_VALUE )
		throw runtime_error( "no such directory" );

	while( FindNextFileW( hDir, &s389 ) ) {
		s390 = s389.cFileName;
		s391 = dir + s390;
		if( regex_match( s390, rg ) ) {
			res = s391;
			ret = true;
			break;
		}
	}
	return ret;
}

void s19( const wstring& _dir, const wstring& s388, vector<wstring>& files, vector<wstring>& dirs )
{
	HANDLE hDir;
	WIN32_FIND_DATAW s389;
	wstring s390, s391;
	wregex rg( s388.c_str() );
	wstring dir = _dir;
	if( dir.back() != L'/' && dir.back() != L'\\' )
		dir += L'\\';

	if( ( hDir = FindFirstFileW( ( dir + L"*" ).c_str(), &s389 ) ) == INVALID_HANDLE_VALUE )
		throw runtime_error( "no such directory" );

	while( FindNextFileW( hDir, &s389 ) ) {
		s390 = s389.cFileName;
		s391 = dir + s390;
		if( regex_match( s390, rg ) ) {
			uint8_t is = s21( s391 );
			if( is == 1 )
				files.push_back( s390 );
			else if( is == 2 )
				dirs.push_back( s390 );
		}
	}
}

} // namespace s17

#endif // WIN32

namespace s25 
{

wstring s26 = L"\n\r\t ";
string s27 = "\n\r\t ";
wstring s28 = L"";


void s32( const wstring& s, size_t s392, wchar_t left, wchar_t right,
	size_t& start, size_t& end )
{
	end = string::npos;
	start = s.find( left, s392 );
	if( start == string::npos || start == s.size() - 1 )
		return;
	int balance = 1;
	for( size_t n = start + 1; n < s.size(); ++n ) {
		if( s[n] == left ) {
			++balance;
			continue;
		}
		if( s[n] == right )
			--balance;
		if( balance == 0 ) {
			end = n;
			return;
		}
	}
}


void s33( const wstring& s, size_t s392, wchar_t c, size_t& start, size_t& end )
{
	end = string::npos;
	start = s.find( c, s392 );
	if( start == string::npos || start == s.size() - 1 )
		return;
	for( size_t n = start + 1; n < s.size(); ++n ) {
		if( s[n] == c && s[n - 1] != '\\' ) {
			end = n;
			return;
		}
	}
}

int s34( const wstring& sf, wchar_t s393, size_t& pos )
{
	size_t s394 = sf.find( s393, pos );
	if( s394 == wstring::npos )
		throw invalid_argument( "separator not found" );
	wstring s = s30( sf.substr( pos, s394 - pos ), L" " );
	int ret = stoi( s );
	pos = sf.find_first_not_of( s26 + s393, s394 );
	return ret;
}


bool s40( const wchar_t *str, long *val, wchar_t **end /*= NULL*/ )
{
	wchar_t *temp;
	bool ret = true;
	errno = 0;
	*val = std::wcstol( str, &temp, 0 );

	if( temp == str || ( !end && *temp != '\0' ) ||
		( ( *val == LONG_MIN || *val == LONG_MAX ) && errno == ERANGE ) )
		ret = false;

	if( end )
		*end = temp;
	return ret;
}

bool s41( const wchar_t *str, double *val, wchar_t **end /*= NULL*/ )
{
	wchar_t *temp;
	bool ret = true;
	errno = 0;
	*val = std::wcstod( str, &temp );

	if( temp == str || ( !end && *temp != '\0' ) ||
		( ( *val == HUGE_VAL || *val == HUGE_VALF || HUGE_VALL ) && errno == ERANGE ) )
		ret = false;

	if( end )
		*end = temp;
	return ret;
}

void s35( const wstring& s, const wstring& s393, vector<wstring>& res )
{
	size_t s400 = 0, enPos = 0;
	while( enPos != wstring::npos ) {
		enPos = s.find( s393, s400 );
		wstring seg = s30( s.substr( s400, enPos - s400 ), s26 );
		s400 = enPos + s393.size();
		res.push_back( seg );
	}
}

wstring s463( const wstring& sf, size_t& pos )
{
	size_t pl = sf.find( L'\n', pos );
	wstring line = s30( sf.substr( pos, pl - pos ), s26 );
	pos = pl == string::npos ? pl : pl + 1;
	return line;
}

void s36( const wstring& sf, const wstring& s393, 
		pair<wstring, wstring>& pr, size_t& pos )
{
	size_t pl = sf.find( L'\n', pos );
	wstring line = s30( sf.substr( pos, pl - pos ), s26 );
	pos = pl == string::npos ? pl : pl + 1;
	pl = line.find( s393 );
	if( pl == string::npos ) 
		throw s4::s5( L"invalid pair syntax: " + line );

	wstring nam = s30( line.substr( 0, pl ), s26 );
	wstring val = s30( line.substr( pl + s393.size() ), s26 );
	pr.first = nam;
	pr.second = val;
}

void s37( const wstring& sf, const wstring& s393, vector<pair<wstring, wstring>>& v )
{
	vector<wstring> s395;
	s35( s30( sf, s26 ), L"\n", s395 );
	for( size_t n = 0; n < s395.size(); ++n ) {
		size_t pl = s395[n].find( s393 );
		wstring nam = s30( s395[n].substr( 0, pl ), s26 );
		wstring val = s30( s395[n].substr( pl + s393.size() ), s26 );
		v.push_back( make_pair( nam, val ) );
	}
}

wstring s29( const std::wstring& l, const std::wstring& s, const std::wstring& r )
{
	size_t pl = s.find_first_not_of( l );
	if( pl == wstring::npos )
		return L"";
	wstring ret = s.substr( pl );
	return ret.substr( 0, ret.find_last_not_of( r ) + 1 );
}

wstring s30( const std::wstring& s, const std::wstring& c )
{
	size_t pl = s.find_first_not_of( c );
	if( pl == wstring::npos )
		return L"";
	wstring ret = s.substr( pl );
	return ret.substr( 0, ret.find_last_not_of( c ) + 1 );
}


wstring s38( const wstring& sf )
{
	if( sf[0] != L'{' )
		return sf;

	size_t s407, s408;
	s32( sf, 0, L'{', L'}', s407, s408 );
	wstring block = sf.substr( s407 + 1, s408 - s407 - 1 );
	vector<wstring> parts;
	s35( s30( block, L" " ), L"|", parts );
#ifdef WIN32	
	return parts[0] + sf.substr( s408 + 1 );
#else
	return parts[1] + sf.substr( s408 + 1 );
#endif	
}

bool s31( const std::wstring& s, const std::wstring& ss )
{
	if( s.substr( s.size() - ss.size() ) == ss )
		return true;
	return false;
}

void s39( wstring& dir )
{
#ifdef WIN32
	if( dir.back() != L'/' &&
		dir.back() != L'\\' )
		dir += L"\\";
#else
	if( dir.back() != L'/' )
		dir += L"/";
#endif
}

void s42( const std::wstring& msg )
{

}


void s32( const string& s, size_t s392, char left, char right,
	size_t& start, size_t& end )
{
	end = string::npos;
	start = s.find( left, s392 );
	if( start == string::npos || start == s.size() - 1 )
		return;
	int balance = 1;
	for( size_t n = start + 1; n < s.size(); ++n ) {
		if( s[n] == left ) {
			++balance;
			continue;
		}
		if( s[n] == right )
			--balance;
		if( balance == 0 ) {
			end = n;
			return;
		}
	}
}

int s34( const string& sf, char s393, size_t& pos )
{
	size_t s394 = sf.find( s393, pos );
	if( s394 == string::npos )
		throw invalid_argument( "separator not found" );
	string s = s30( sf.substr( pos, s394 - pos ), " " );
	int ret = stoi( s );
	pos = sf.find_first_not_of( s27 + s393, s394 );
	return ret;
}

void s35( const string& s, const string& s393, vector<string>& res )
{
	size_t s400 = 0, enPos = 0;
	while( enPos != string::npos ) {
		enPos = s.find( s393, s400 );
		string seg = s30( s.substr( s400, enPos - s400 ), s27 );
		s400 = enPos + s393.size();
		res.push_back( seg );
	}
}

void s37( const string& sf, vector<pair<string, string>>& v )
{
	vector<string> s395;
	s35( s30( sf, s27 ), "\n", s395 );
	for( size_t n = 0; n < s395.size(); ++n ) {
		size_t pl = s395[n].find( ':' );
		string nam = s30( s395[n].substr( 0, pl ), s27 );
		string val = s30( s395[n].substr( pl + 1 ), s27 );
		v.push_back( make_pair( nam, val ) );
	}
}

string s29( const std::string& l, const std::string& s, const std::string& r )
{
	size_t pl = s.find_first_not_of( l );
	if( pl == string::npos )
		return "";
	string ret = s.substr( pl );
	return ret.substr( 0, ret.find_last_not_of( r ) + 1 );
}

string s30( const std::string& s, const std::string& c )
{
	size_t pl = s.find_first_not_of( c );
	if( pl == string::npos )
		return "";
	string ret = s.substr( pl );
	return ret.substr( 0, ret.find_last_not_of( c ) + 1 );
}


string s38( const string& sf )
{
	if( sf[0] != '{' )
		return sf;

	size_t s407, s408;
	s32( sf, 0, '{', '}', s407, s408 );
	string block = sf.substr( s407 + 1, s408 - s407 - 1 );
	vector<string> parts;
	s35( s30( block, " " ), "|", parts );
#ifdef WIN32	
	return parts[0] + sf.substr( s408 + 1 );
#else
	return parts[1] + sf.substr( s408 + 1 );
#endif	
}

}//namespace s25


namespace s43 {
std::locale locloc;
std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
}

using namespace s25;
using namespace s43;
using namespace s1;


namespace s44
{
wstring numChars = L"-.0123456789";
wstring rightBound = L" ,]}\t\r\n";
wstring quotes = L"\"/`'";

void s81( const wstring& sf, size_t& pos, s3<s57> ob )
{
	size_t s407, s408;
	s32( sf, pos, L'{', L'}', s407, s408 );
	if( s407 == string::npos )
		return;
	wstring sob = sf.substr( s407, s408 - s407 + 1 );
	pos = s408 + 1;
	ob->s65( sob );
}

void s397( wstring& sf )
{
	wregex rgCmnt(
		LR"regex(\n\r?\/\/.*?\n\r?)regex"
	);
	wsmatch sm;
	wstring::const_iterator itst = sf.begin();
	wstring::const_iterator iten = sf.end();
	while( std::regex_search( itst, iten, sm, rgCmnt ) ) {
		sf.erase( sm.position(), sm.length() - 
			(sm[0].str()[sm.length()-1]=='\r' ? 2 : 1 ) );
		itst = sf.begin();
		iten = sf.end();
	}
}

void
s80( const wstring& s396, vector<s3<s57>>& res, const wstring& sheb )
{
	std::ifstream fs( TOFS( s396 ));
	if( !fs.good() )
		throw runtime_error( "Reading nqj file failed: " + U8( s396 ));
	string sfA( (std::istreambuf_iterator<char>( fs )), std::istreambuf_iterator<char>());
	wstring sf = conv.from_bytes( sfA );
	s397( sf );
	size_t pos = 0;
	if( sheb.size() ) {
		sf.find( sheb );
		if( pos == string::npos )
			throw s5( sheb + L" directive not found in the source file." );
		pos += sheb.size() + 1;
		if( sf.size() > 13 && sf[14] == '\r' )
			++pos;
	}
	size_t startPos = pos;
	wstring s172;
	bool s398 = false;
	while( ( pos = sf.find( L'{', pos ) ) != string::npos ) {
		size_t pl = sf.rfind( '}', pos );
		size_t subl = pos - pl - 1;
		if( string::npos == pl ) {
			pl = startPos; subl = pos - startPos;
		}
		else
			pl += 1;

		wstring s1 = s30( sf.substr( pl, /*pos - pl - 1*/subl ), s26 );
		if( s1.size() ) {
			if( s1[s1.size()-1] != L':' )
				throw s5( L"Malformed syntax: " + s1.substr( 0, 20 ) + L"...",
				(uint32_t)s5::s6::s9 );
			if( s1.size() == 1 )
				s398 = true;
			else
				s172 = s30( s1, s26 + L":" );
		}
		if( s172.empty() ) {
			size_t pl1 = sf.rfind( '\n', pl );
			if( pl1 < pl - 1 ) {
				wstring s1 = s30( sf.substr( pl1, pl ), s26 );
				if( s1.size() ) {
					if( ( !s398 && s1[s1.size() - 1] != L':' ) ||
						( s398 && s1[s1.size() - 1] == L':' ) ||
						(!s398 && s1.size() == 1 ) )
						throw s5( L"Malformed syntax: " + s1.substr( 0, 20 ) + L"...",
						(uint32_t)s5::s6::s9 );
					s172 = s30( s1, s26 + L":" );
				}
			}
		}
		if( quotes.find( s172[0] ) != string::npos ) {
			if( s172[0] != s172[s172.size()-1] )
				throw s5( L"Malformed block name: " + s172.substr( 0, 20 ) + L"...",
						(uint32_t)s5::s6::s9 );
		}
		else {
			if(	iswdigit( s172[0] ) )
				throw s5( L"wrong unquoted syntax: " + s172.substr( 0, 20 ) + L"...",
				(uint32_t)s5::s6::s9 );


		}
		s3<s57> ob = new s57;
		s81( sf, pos, ob );
		if( s172.size() )
			ob->s70( L"name", new s55( s172 ) );
		s172 = L"";
		s398 = false;
		res.push_back( ob );
	}
}

void s82( const wstring& s399, const vector<s1::s3<s57>>& objs )
{
	std::ofstream fs( TOFS( s399 ), ios::out | ios::trunc | ios::binary );
	if( !fs.good() )
		throw runtime_error( "Reading state file failed: " + U8( s399 ) );
	for( auto ob : objs ) {
		basic_stringstream<wchar_t> ss;
		ob->s66( &ss );
		string s = U8( ss.str() );
		s += "\n\n";
		fs.write( s.c_str(), s.size() );
	}
}


void s83( const wstring& sf, size_t& pos, wchar_t quote, wstring& res )
{
	size_t s407, s408;
	s33( sf, pos, quote, s407, s408 );
	if( s408 != string::npos ) {
		pos = s408 + 1;
		res = s30( sf.substr( s407+1, s408 - s407 - 1 ), s26 ); 
	}
	else
		pos = string::npos;
}

void s84( wostream* pd, const wstring& s )
{
	wchar_t q = L'\"';
	if( quotes.find( s[0] ) != string::npos )
		q = s[0];
	size_t start = 1;
	*pd << q;
	if( s[0] != q ) 
		start = 0;

	for( size_t n = start; n < s.size(); ++n ) {
		if( s[n] == q && s[n-1] != L'\\' && n != s.size() - 1 )
			*pd << L'\\';
		*pd << s[n];
	}

	if( s[s.size()-1] != q )
		*pd << q;
}

void s85( const wstring& sf, size_t& pos, wstring& res )
{
	if( !std::isgraph<wchar_t>( sf[pos], locloc ) ||
			sf[pos] == L':' || sf[pos] == L',' )
		throw s5( L"unrecognized unquoted syntax: " + sf.substr(pos, 20), 
				(uint32_t)s5::s6::s9 );
	size_t tokEnd = pos + 1;
	while( std::isgraph<wchar_t>( sf[tokEnd], locloc ) &&
			sf[tokEnd] != L':' && sf[tokEnd] != L',' )
		++tokEnd;
	res = s30( sf.substr( pos, tokEnd - pos ), s26 );
	pos = tokEnd;
}

void s86( const wstring& sf, size_t& pos, wstring& res )
{
	size_t s407, s408;
	s32( sf, pos, L'(', L')', s407, s408 );
	if( s408 == string::npos )
		throw s5( L"unmatched unquoted parentheses: " + sf.substr(pos, 20), 
			(uint32_t)s5::s6::s9 );
	if( s408 == sf.size() - 1 )
		throw s5( L"wrong unquoted parentheses syntax: " + sf.substr(pos, 20), 
			(uint32_t)s5::s6::s9 );
	size_t pl = sf.find_first_not_of( s26, s408+1 );
	if( pl == string::npos && sf[pl] != L',' && sf[pl] != L'}' )
		throw s5( L"wrong unquoted parentheses syntax: " + sf.substr(pos, 20), 
			(uint32_t)s5::s6::s9 );
	pos = s408 + 1;
}

void s87( const wstring& sf, size_t& pos, wstring& res )
{
	if( !std::isgraph<wchar_t>( sf[pos], locloc ) ||
		sf[pos] == L':' || sf[pos] == L',' )
		throw s5( L"unrecognized unquoted syntax: " + sf.substr( pos, 20 ),
		(uint32_t)s5::s6::s9 );
	size_t tokEnd = pos;
	if( sf[pos] == L'(' )
		s86( sf, tokEnd, res );
	else {
		tokEnd = pos + 1;

		while( std::isgraph<wchar_t>( sf[tokEnd], locloc ) &&
			sf[tokEnd] != L':' && sf[tokEnd] != L',' ) {
			if( sf[tokEnd] == L'(' ) {
				s86( sf, tokEnd, res );
				break;
			}
			++tokEnd;
		}
	}
	res = s30( sf.substr( pos, tokEnd - pos ), s26 );
	pos = tokEnd;
}

void s88( wostream* pd, const wstring& s )
{
	if( ( !std::isalpha<wchar_t>( s[0], locloc ) && s[0] != L'_' ) ||
			s[s.size()-1] == L'"' )
		throw s5( L"wrong unquoted syntax: " + s.substr(0, 20),
				(uint32_t)s5::s6::s9 );

	for( size_t n = 0; n < s.size(); ++n ) 
		if( !std::isgraph<wchar_t>( s[n], locloc ) )
			throw s5( L"wrong unquoted syntax: " + s.substr(0, 20),
				(uint32_t)s5::s6::s9 );

	*pd << s;
}

void s89( const wstring& sf, size_t& pos, vector<s3<s55>>& res )
{
	size_t s407, s408, s400 = 0;
	s32( sf, pos, L'[', L']', s407, s408 );
	pos = s408 + 1;
	wstring ar = s30( sf.substr( s407 + 1, s408 - s407 - 1 ), s26 );
	while( true ) {
		res.push_back( s3<s55>(new s55( ar, s400 ) ) );
		s400 = ar.find( /*' '*/',', s400 );
		if( s400 == string::npos )
			break;
		++s400;
	}
}

void s90( wostream* pd, const vector<s3<s55>>& v, bool strict=false )
{
	*pd << L'[';
	for( size_t n = 0; n < v.size(); ++n ) {
		v[n]->s66( pd, strict );
		if( n != v.size() - 1 )
			*pd << L',';
	}
	*pd << L']';
}

void s91( const wstring& sf, size_t& pos, wstring& res )
{
	size_t pl = sf.find_first_not_of( L", \n\r\t", pos );
	if( quotes.find( sf[pl] ) != string::npos ) 
		s83( sf, pos, sf[pl], res );
	else {

		if( !std::isgraph<wchar_t>( sf[pos], locloc ) ||
			sf[pos] == L':' || sf[pos] == L',' )
			throw s5( L"unrecognized name syntax: " + sf.substr( pos, 20 ) +
				L"...",	(uint32_t)s5::s6::s9 );
		size_t tokEnd = pos + 1;

		while( sf[tokEnd] != L':' && sf[tokEnd] != L',' ) {
			if( !std::isalnum<wchar_t>( sf[tokEnd], locloc ) &&
				!std::isspace<wchar_t>( sf[tokEnd], locloc ) && sf[tokEnd] != L'_' )
				throw s5( L"unrecognized name syntax: " + sf.substr( pos, 20 ) +
					L"...",	(uint32_t)s5::s6::s9 );
			++tokEnd;
		}
		res = s30( sf.substr( pos, tokEnd - pos ), s26 );
		pos = tokEnd;
	}
}

void s92( wostream* pd, const wstring& s, bool strict=false )
{
	bool space = false;
	for( size_t n = 0; n < s.size(); ++n )
		if( !std::isgraph<wchar_t>( s[n], locloc ) ) {
			space = true;
			break;
		}

	if( !space && !strict && quotes.find( s[0] ) == string::npos )
		s88( pd, s );
	else
		s84( pd, s );
}

s45 s93( const wstring& sf, size_t& pos, double& res )
{
	s45 ret = s54;
	size_t pl = sf.find_first_not_of( L"-0123456789.", pos );
	size_t pl1 = sf.find( L"." );
	if( pl1 < pl )
		ret = s50;
	if( pl == string::npos )
		res = stod( sf.substr( pos ) );
	else
		res = stod( sf.substr( pos, pl - pos ) );
	pos = pl;
	return ret;
}

bool s94( const wstring& sf, size_t& pos, bool& res )
{
	size_t pl = 0;
	if( sf.find( L"true", pos, 4 ) == pos )
		pl = 4;
	else if( sf.find( L"false", pos, 5 ) == pos )
		pl = 5;
	if( pl ) {
		if( rightBound.find( sf[pos+pl] ) != string::npos ) {
			pos += pl;
			res = (pl == 4);
			return true;
		}
	}
	return false;
}

bool s95( const wstring& sf, size_t& pos )
{
	if( sf.find( L"null", pos, 4 ) == pos ) {
		if( rightBound.find( sf[pos+4] ) != string::npos ) {
			pos += 4;
			return true;
		}
	}
	return false;
}

s55::s55( const wstring& sf, size_t& pos )
	: var( s46 )
{
	size_t s400 = sf.find_first_not_of( L" \n\r\t", pos );
	if( sf[s400] == L'{' ) {
		size_t s407, s408;
		s32( sf, s400, L'{', L'}', s407, s408 );
		pos = s408+1;
		obj.s65( s30( sf.substr( s407 + 1, s408 - s407 - 1 ), s26 ) );
		var = s47;
	}
	else {
		pos = s400;
		if ( quotes.find( sf[pos] ) != string::npos ) {
			s83( sf, pos, sf[pos], str );
			var = s53;
		}
		else if ( sf[pos] == L'[' ) {
			s89( sf, pos, s73 ); 
			var = s49;
		}
		else if ( numChars.find( sf[pos] ) != string::npos ) {
			var = s93( sf, pos, s74 );

		}
		else {
			if( s94( sf, pos, b ) )
				var = s51;
			else if( s95( sf, pos ) )
				var = s52;
			else {
				s87( sf, pos, str );
				var = s48;
			}
		}
	}
}

void s57::s65( const wstring& sf )
{

	s58.clear();
	size_t s400;
	size_t pl = sf.find_first_not_of( L" \n\r\t" );
	if( pl == string::npos )
		return;
	if( sf[pl] == L'{' )
		s400 = sf.find_first_not_of( L" \n\r\t", pl + 1 );
	else
		s400 = pl;
	while( true ) {
		wstring nam;
		s91( sf, s400, nam );
		size_t pl = sf.find_first_not_of( L" \n\r\t", s400 );
		if( pl == string::npos || sf[pl] != L':' )
			throw s5( L"unrecognized nqj syntax: " + sf.substr(s400, 50), 
					(uint32_t)s5::s6::s9 );
		s400 = pl + 1;

		s58.insert( make_pair( nam, new s55( sf, s400 ) ) );
		pl = sf.find_first_not_of( L" \n\r\t", s400 );
		if( pl != string::npos && rightBound.find( sf[pl] ) == string::npos ) 
			throw s5( L"wrong nqj syntax: missed separator or closing brace: " + sf.substr(s400, 50), 
					(uint32_t)s5::s6::s9 );
		if( pl != string::npos )
			pl = sf.find_first_not_of( L" \n\r\t", pl+1 );
		if( pl == string::npos )
			break;
		s400 = pl;
	}
}

bool s57::operator==( const s57& r )  const
{
	bool ret = true;
	for( auto it = s58.begin(); it != s58.end(); ++it ) {
		auto it1 = r.s58.find( it->first );
		if( it1 == r.s58.end() || !( *( it->second ) == *( it->second ) ) ) {
			ret = false;
			break;
		}
	}
	return ret;
}

void s57::s66( wostream* pd, bool strict ) const
{
	*pd << L"{\n";
	for( auto it = s58.begin(); it != s58.end(); ++it ) {
		if( it != s58.begin() )
			*pd << L",\n";
		s92( pd, it->first, strict );
		*pd << L" : ";
		it->second->s66( pd );
	}
	*pd << L"\n}";
}

void s55::s66( wostream* pd, bool strict ) const
{
	if( var == s47 )
		obj.s66( pd, strict );
	else if( var == s49 )
		s90( pd, s73, strict );
	else if( var == s48 )
		s92( pd, str, strict );
	else if( var == s53 )
		s92( pd, str, true );
	else if( var == s50 )
		*pd << to_wstring( s74 );
	else if( var == s51 )
		*pd << (b ? L"true" : L"false");
	else if( var == s52 )
		*pd << L"null" << endl;
}

bool s57::s67( const wstring& key ) const
{
	if( s58.find( key ) != s58.end() )
		return true;
	return false;
}

void s57::s68( const wstring& key, vector<s3<s55>>& res ) const
{
	auto i = s58.equal_range( key );
	if( i.first == i.second )
		return;
	for( auto it = i.first; it != i.second; ++it )
		res.push_back( it->second );
}

const s56 s57::s69( const wstring& key ) const
{
	if( s58.count( key ) > 1 )
		throw s5( L"multiple values: " + key, (uint32_t)s63 );

	auto ret = s58.find( key );
	return ret->second;
}

void s57::s70( const std::wstring& nam, s56 val, s59 ap )
{
	if( s58.find( nam ) != s58.end() ) {
		if( ap == s60 )
			throw s5( L"Field already exists: " + nam, (uint32_t)s5::s6::s10 );
		else if( ap == s62 )
			return;
		else if( ap == s61 )
			s58.erase( nam );
	}
	s58.insert( make_pair( nam, val ) );
}

void s57::s71( vector<pair<wstring, s3<s55>>>& res ) const
{
	for( auto it = s58.begin(); it != s58.end(); ++it ) 
		res.push_back( *it );
}

void s57::s72( wostream* pd, int ind ) const
{
	for( auto it = s58.begin(); it != s58.end(); ++it ) {
		*pd << FILL(ind) << it->first << " : " << endl;
		it->second->s72( pd, ind + 2 );
	}
}

void s55::s72( wostream* pd, int ind ) const
{
	if( var == s47 )
		obj.s72( pd, ind+2 );
	else if( var == s49 ) {
		for( size_t n = 0; n < s73.size(); ++n ) {
			s73[n]->s72( pd, ind );
			if( n < s73.size() - 1 )
				*pd << endl;
		}
	}
	else if( var == s48 )
		*pd << FILL(ind) << str << endl;
	else if( var == s50 )
		*pd << FILL(ind) << s74 << endl;
	else if( var == s51 )
		*pd << FILL(ind) << b << endl;
	else if( var == s52 )
		*pd << FILL(ind) << L"null" << endl;
}

bool operator==( const std::vector<s3<s55>>& l,
	const std::vector<s3<s55>>& r )
{
	if( l.size() == r.size() ) {
		for( size_t n = 0; n < l.size(); ++n )
			if( !(*( l[n] ) == *( r[n] ) ) )
				return false;
		return true;
	}
	return false;
}

} // namespace s44

using namespace s44;


namespace transd {
namespace s96 {

DEFCONST s97 = L"typeMembers";
DEFCONST s98 = L"typeMember";
DEFCONST s99 = L"typeMethods";
DEFCONST s100 = L"typeMethod";
DEFCONST s101 = L"members";
DEFCONST s102 = L"member";
DEFCONST s103 = L"methods";
DEFCONST s104 = L"method";
DEFCONST s105 = L"init";
DEFCONST s106 = L"initCall";
DEFCONST s107 = L"thisPtr";
DEFCONST s108 = L"virtual";
DEFCONST s109 = L"override";


DEFCONST s110 = L"import";
DEFCONST s111 = L"input";
DEFCONST s112 = L"output";
DEFCONST s113 = L"export";
DEFCONST s114 = L"opaque";


DEFCONST s459 = L"public";
DEFCONST s460 = L"private";
DEFCONST s461 = L"secure";


DEFCONST s115 = L"class";
DEFCONST s116 = L"module";


DEFCONST s117 = L"AssemblyDescription";
DEFCONST s118 = L"ProgramText";
DEFCONST s119 = L"Export";

DEFCONST s120 = L"_start";
DEFCONST s121 = L"asm.td";


DEFCONST s126 = L"source";
DEFCONST s122 = L"objectType";
DEFCONST s123 = L"moduleType";
DEFCONST s124 = L"name";
DEFCONST s125 = L"entryPoint";
DEFCONST s127 = L"perm";
DEFCONST s129 = L"extends";

DEFCONST s454 = L"#lang transd";

} // namespace s96
std::wstring s135[] = { L"value", L"function", L"object", L"name" };

const wstring s136 = L"if";
const wstring s137 = L"set";
const wstring s138 = L"sum";
const wstring s139 = L"with";
const wstring s140 = L"mul";
const wstring s141 = L"div";
const wstring s142 = L"dif";
const wstring s143 = L"eq";
const wstring s144 = L"less";
const wstring s145 = L"greater";
const wstring s146 = L"leq";
const wstring s147 = L"geq";
const wstring s148 = L"neq";
const wstring s149 = L"and";
const wstring s150 = L"or";
const wstring s151 = L"dcmp";
const wstring s152 = L"dcmpr";
const wstring s153 = L"mod";
const wstring s154 = L"sqrt";
const wstring s155 = L"abs";
const wstring s156 = L"pow";
const wstring s157 = L"textout";
const wstring s158 = L"lambda";
const wstring s159 = L"proc";
const wstring s160 = L"while";
const wstring s161 = L"eval";


s163::s163( const std::wstring& s )
{
	size_t pos = 0;
	if( s[pos] == L'(' )
		s176( s );
	else if( iswalpha( s[pos] ) || s[pos] == L'_' ) {
		if( s.find( L'(' ) != string::npos )
			s178( s );
		else
			s179( s );
	}
	else if( s[pos] == L'"' || iswdigit( s[pos] ) || s[pos] == L'\'' ||
					 s[pos] == L'-'	) {
		wstring sf;

			sf = s30( s, s26 );

		s178( sf );
	}
	else
		throw s5( L"Unknown string content: " + s );
}

s163::s163( const std::wstring& s298, const s44::s55& s190 )
	: s172( s298 )
{
	if( s190.Var() == s48 ) {
		wstring s = s190.s79();
		size_t pos = 0;
		if( s[pos] == L'(' )
			s176( s );
		else if( iswalpha( s[pos] ) )
			s178( s );
		else
			throw s5( L"Unknown string content: " + s );
	}
	else if( s190.Var() == s53 ) {
		wstring s = L"\"" + 
			s190.s79() + 
			L"\"";
		s178( s );
	}
	else if( s190.Var() == s47 )
		s177( s190.s76() );
	else if( s190.Var() == s50 )
		s178( to_wstring( s190.s78() ) );
	else if( s190.Var() == s54 )
		s178( to_wstring( s190.s34() ) );
	else if( s190.Var() == s49 ) {
		basic_stringstream<wchar_t> ss;
		s190.s66( &ss );
		s178( ss.str() );
	}
	else
		throw s5( L"Unhandled nqj variant: " + to_wstring( s190.Var() ) );
}

s163::s163( const s44::s57& s47 )
{
	s177( s47 );
}

void s163::s176( const std::wstring& sf )
{
	size_t pos = 0;
	vector<wstring> tokens;
	s182( sf, pos, tokens );
	if( tokens.empty() )
		return;
	s173 = s132;
	size_t s400 = 0;
	s174 = s181( tokens[0], s400 );

	for( size_t n = 1; n < tokens.size(); n++ )
		s175.insert( make_pair( to_wstring( n - 1 ), new s163( tokens[n] ) ) );
}

void s163::s178( const std::wstring& sf )
{
	s173 = s131;
	s174 = sf;
}

void s163::s179( const std::wstring& sf )
{
	s173 = s134;
	s174 = sf;
}

void s163::s177( const s44::s57& s47 )
{
	s172 = s196( s47, s96::s124 );
	s173 = s133;
	vector<pair<wstring, s56>> s58;
	s47.s71( s58 );
	for( size_t n = 0; n < s58.size(); ++n ) {
		if( s58[n].first != s96::s124 )
			s180( s58[n].first, *s58[n].second );
	}
}

void s401( wstring& s )
{
	wregex rgBr(
		LR"regex(\s(\s+))regex"
	);
	wsmatch sm;
	wstring::const_iterator itst = s.begin();
	wstring::const_iterator iten = s.end();
	while( std::regex_search( itst, iten, sm, rgBr ) ) {
		s.erase( sm.position( 1 ), sm.length( 1 ) );
		itst = s.begin();
		iten = s.end();
	}
}

void s163::s183( s44::s57& obj )
{
	wstring s508 = s25::s30( s196( obj, s96::s124/*,
																to_wstring( ++namecnt )*/ ), s26 );
	wstring s402 = s508;
	s401( s402 );
	if( s402.empty() )
		throw s5( L"Nameless blocks are forbidden." );

	if( s402 != s508 )
		obj.s70( s96::s124, s55( s402 ), s57::s61 );

	if( s402 == s172 ) { // partial definition
		vector<pair<wstring, s56>> s58;
		obj.s71( s58 );
		for( size_t n = 0; n < s58.size(); ++n )
			s180( s58[n].first, *s58[n].second );
	}
	else {

		s175.insert( make_pair( s402, new s163( obj ) ) );

	}
}

void s163::s180( const std::wstring& s172, const s55& val )
{
	s175.insert( make_pair( s172, new s163( s172, val ) ) );
}

void s163::s184( s162* s194, size_t s195 /*= 0*/ )
{
	s169::const_iterator it = s175.begin();
	for( ; it != s175.end(); ++it ) {
		if( it->second->s191() == s133 ) {
			if( s195 == 0 ) {
				it->second->s184( s194, 1 );
			}
			else {

			}
		}
	}
}

const s163& s163::s186( const std::wstring& s172 ) const
{
	size_t cnt = s175.count( s172 );
	if( cnt > 1 )
		throw s5( L"multiple values: " + s172, (uint32_t)s63 );
	if( cnt == 1 ) {
		auto ret = s175.find( s172 );
		return *ret->second;
	}
	throw s5( L"node not found: " + s172, (uint32_t)s5::s6::s12 );
	s169::const_iterator it = s175.begin();
	for( ; it != s175.end(); ++it ) {
		try {
			const s163& ret = it->second->s186( s172 );
			return ret;
		}
		catch( s5& e ) {
			if( e.which() == (uint32_t)s5::s6::s12 )
				continue;
			throw e;
		}
	}

	throw s5( L"node not found: " + s172, (uint32_t)s5::s6::s12 );
}

void s163::s182( const wstring& sf, size_t& pos, vector<wstring>& res )
{
	size_t s407, s408;
	s32( sf, pos, L'(', L')', s407, s408 );
	if( s407 != pos || s408 == string::npos )
		throw s5( L"Parentheses don't match near \"" + sf.substr( 0, 40 ) + L"...\"" );
	pos = s408;

	size_t s400 = sf.find_first_not_of( s26, s407 + 1 );
	if( sf[s400] == L')' )
		return;
	size_t s403 = s400;
	wstring s404;
	while( s403 < s408 ) {
		s400 = sf.find_first_not_of( s26 + L")", s403 );
		if( s400 >= s408 )
			break;

		size_t s405, s406;

		if( sf[s400] == L'(' ) {
			s32( sf, s400, L'(', L')', s405, s406 );
			if( s406 == s408 )
				throw s5( L"Parentheses don't match near \'" + sf.substr( s400, 40 ) + L"...\'" );
			s404 = sf.substr( s400, s406 - s400 + 1 );
			s403 = s406 + 1;
		}
		else if( sf[s400] == L'"' ) {
			s33( sf, s400, L'"',
				s405, s406 );
			if( s406 == string::npos )
				throw s5( L"Quotes don't match near \'" + sf.substr( s400, 40 ) + L"...\'" );
			s404 = sf.substr( s400, s406 - s400 + 1 );
			s403 = s406 + 1;
		}
		else if( iswdigit( sf[s400] ) ) {
			s406 = sf.find_first_of( s26 + L")", s400 );
			s404 = sf.substr( s400, s406 - s400 );
			s403 = s406;
		}
		else if( iswalpha( sf[s400] ) || sf[s400] == L'_' ) {
			s406 = sf.find_first_of( s26 + L")", s400 );
			size_t pl = sf.find( L'(', s400 );
			if( pl < s406 ) {
				s32( sf, pl, L'(', L')', s405, s406 );
				if( s406 == s408 )
					throw s5( L"Parentheses don't match near \'" + sf.substr( s400, 40 ) + L"...\'" );
				s404 = sf.substr( s400, s406 - s400 + 1 );
				s403 = s406 + 1;
			}
			else {
				s404 = sf.substr( s400, s406 - s400 );
				s403 = s406;
			}
		}
		else {
			s406 = sf.find_first_of( s26 + L")", s400 );
			s404 = sf.substr( s400, s406 - s400 );
			s403 = s406;
		}

		res.push_back( s404 );
	}
}

wstring s163::s181( const std::wstring& sf, size_t &pos )
{
	wstring ret;
	if( sf[pos] == L'+' && sf[pos + 1] != L'=' )
		ret = s138;
	else if( sf[pos] == L'-' && sf[pos + 1] != L'=' )
		ret = s142;
	else if( sf[pos] == L'*' && sf[pos + 1] != L'=' )
		ret = s140;
	else if( sf[pos] == L'/' && sf[pos + 1] != L'=' )
		ret = s141;
	else if( sf[pos] == L'^' && sf[pos + 1] != L'=' )
		ret = s156;
	else if( sf[pos] == L'=' && sf[pos + 1] != L'=' )
		ret = s137;
	else if( sf[pos] == L'<' && sf[pos + 1] != L'=' )
		ret = s144;
	else if( sf[pos] == L'>' && sf[pos + 1] != L'=' )
		ret = s145;
	else if( sf[pos] == L'λ' )
		ret = s158;

	if( ret.size() ) {
		if( sf.size() > pos + 1 && sf.find_first_of( s26 + L")", pos ) != pos + 1 )
			throw s5( L"1: unknown syntax: " + sf.substr(pos, 40),
			(uint32_t)s5::s6::s9 );
		pos += 1;
		return ret;
	}

	if( sf.substr( pos, 2 ) == L"+=" )
		ret = L"sumeq";
	else if( sf.substr( pos, 2 ) == L"-=" )
		ret = L"difeq";
	else if( sf.substr( pos, 2 ) == L"*=" )
		ret = L"muleq";
	else if( sf.substr( pos, 2 ) == L"/=" )
		ret = L"diveq";
	else if( sf.substr( pos, 2 ) == L"^=" )
		ret = L"poweq";
	else if( sf.substr( pos, 2 ) == L"<=" )
		ret = L"leq";
	else if( sf.substr( pos, 2 ) == L">=" )
		ret = L"geq";
	else if( sf.substr( pos, 2 ) == L"==" )
		ret = L"eq";
	else if( sf.substr( pos, 2 ) == L"!=" )
		ret = L"neq";

	if( ret.size() ) {
		if( sf.size() > pos + 2 && sf.find_first_of( s26 + L")", pos ) != pos + 2 )
			throw s5( L"2: unknown syntax: " + sf.substr( pos, 40 ), 
				(uint32_t)s5::s6::s9 );
		pos += 2;
		return ret;
	}

	size_t s403 = sf.find_first_of( s26 + L")", pos );
	ret = sf.substr( pos, s403 - pos );
	pos = s403;
	return ret;
}

std::wstring s163::s192() const
{
	return (s171.size() ? s171 + L"::" : L"") + s172;
}

wstring s163::s189( const s44::s57& obj,
	const std::wstring& def /*= std::wstring( L"" )*/ )
{
	if( obj.s67( s96::s123 ) )
		return obj.s69( s96::s123 )->s79();
	return def;
}

std::wstring s163::s196( const s44::s57& obj,
	const std::wstring& s197,
	const std::wstring& def /*= std::wstring( L"" )*/ )
{
	if( obj.s67( s197 ) )
		return obj.s69( s197 )->s79();
	return def;
}


void s163::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << s172 << L" : " << s135[s173] << endl;
	buf << s25::fill_n_ch( L' ', ind ) << L" " << s174 << endl;

	map<wstring, s163*>::const_iterator it = s175.begin();
	for( ; it != s175.end(); ++it ) {
		buf << s25::fill_n_ch( L' ', ind + INDSTEP ) << it->first << endl;
		it->second->s198( pd, ind + INDSTEP * 2 );
	}

}
std::map<const s212*, bool> s165::s214;

s165::~s165()
{

}

s211* s165::s218()
{	throw std::logic_error( "1: not implemented" );}

const s211* s165::s225() const
{	throw std::logic_error( "2: not implemented" );}

s211* s165::s219( s211** s168, size_t s220 )
{	throw std::logic_error( "3: not implemented" ); }

s165* s165::s226() const
{	throw std::logic_error( "4: not implemented" ); }

void s165::s223( const wstring& ) { }

inline bool
s165::isTypeOf( const s211* classObj ) const
{
	return ( classObj->s217() == s217() );
}

s165::operator bool() const { throw std::logic_error( "5: not implemented" ); }

s165::operator int() const { throw std::logic_error( "6: not implemented" ); }

s165::operator double() const { throw std::logic_error( "7: not implemented" ); }

wstring s165::to_wstring() const { throw std::logic_error( "8: not implemented" ); }

bool s165::s224() const
{
	if( s214.find( this ) != s214.end() )
		return true;
	s214[this] = true;
	return false;
}

s165* s165::s227( s163& s372, s162* s194, s166* obj, const wstring& s228 )
{
	s212* p = nullptr;
	if( s372.s191() == s131 ) {


		p = s194->s381( s372.s185(), s228 );
		((s211*)p)->s241( s372.s193() );
	}
	else if( s372.s191() == s132 )
		p = s326::s357( s372, s194, obj, s228 );
	else if( s372.s191() == s134 )
		p = new s230( s372.s185(), s194 );
	else //if( s372.s191() == s133 )

		throw s5( L"unexpected node type" );

	return p;
}

s212* s509( s229& m, const wstring& key )
{
	if( m.count( key ) > 1 )
		throw s5( L"multiple values: " + key, (uint32_t)s5::s6::s10 );
	
	auto ret = m.find( key );
	if( ret == m.end() )
		throw s5( L"no such key: " + key, (uint32_t)s5::s6::s12 );
	return ret->second;
}

void s517( const std::wstring& s, std::wstring& modName, std::wstring& s359 )
{
	size_t pl = s.find( L"::" );
	if( pl == string::npos || s.find( L"::", pl + 1 ) != string::npos )
		throw s5( L"unknown format of qualified name: " + s, (uint32_t)s5::s6::s9 );
	modName = s.substr( 0, pl );
	s359 = s.substr( pl + 2 );
}
vector<wstring> s258::s261 = { s96::s113, s96::s111,
																					s96::s114, s96::s112 };

s1::s3<s262> s288 = new s262();

void s213::s184()
{
	for( auto type : m )
		type.second->s234();
}

void s213::s253( const s210& st )
{

}

void s213::s254( s211* p )
{
	wstring s409 = /*L"::" +*/ p->s238();
	if( m.find( s409 ) != m.end() )
		throw s5( (uint32_t)s5::s6::s10 );
	v.push_back( s409 );
	p->s237( v.size() );
	m[s409] = p;
}

void s213::s255( s211* p, const wstring& s256 )
{
	wstring s409 = s256 + L"::" + p->s238();
	if( m.find( s409 ) != m.end() )
		throw s5( (uint32_t)s5::s6::s10 );
	v.push_back( s409 );
	p->s237( v.size() );
	m[s409] = p;
}

s211* s213::s257( s162* s194, s163& s372, const wstring& s256 )
{
	wstring val = s372.s185();
	if( val[0] == L'[' ) {

		return &nullobject;
	}
	size_t pl = val.find( L"(" );
	wstring s409 = /*L"::" +*/ s307( s194 ).s238();
	if( pl != string::npos ) {
		s409 = val.substr( 0, pl );

	}
	return ( (s166*)m[s409] )->s232( s372, s256 );

	for( int i = (int)v.size() - 1; i >= 0; --i ) {
		wstring s = v[i];
		if( s[0] != L':' ) {
			if( s.find( s256 + L"::" ) != 0 )
				continue;
		}

		s211* ret = m[v[i]]->s232( val, s256 );
		if( ret )
			return ret;
	}

	throw s5( L"No parser for string: " + val,
		(uint32_t)s5::s6::s9 );
}

s211* s213::s257( s162* s194, const std::wstring& val, const wstring& s256 )
{
	if( val[0] == L'[' ) {
		return m[s309( s194, L"" ).s238()]->s232( val, s256 );

		return &nullobject;
	}
	wstring s409 = L"";
	wstring quotes = L"\"/`'";
	if( quotes.find( val[0] ) != string::npos )
		s409 = s307( s194 ).s238();
	else if( isdigit( val[0] ) || val[0] == L'-' )

		return s293::s303( s194, val, s256 );
	else {
		size_t pl = val.find( L"(" );
		if( pl != 0 && pl != string::npos ) {
			s409 = val.substr( 0, pl );

		}
	}
	auto it = m.find( s409 );
	if( it == m.end() )
		throw s5( L"Type does not exist: " + val );
	return it->second->s232( val, s256 );
}

s211* s213::s257( s162* s194, size_t type, const wstring& s256 )
{
	if( type > v.size() )
		throw s5( L"no such type: " + to_wstring( type ) );
	wstring s = v[type - 1];

	return (s211*)m[s]->s239();
}

s211* s213::get( const wstring& nam, bool thr/*= true*/ ) const
{
	auto ret = m.find( nam );
	if( ret == m.end() ) {
		if( thr )
			throw s5( L"symbol not found: " + nam );
		else
			return NULL;
	}
	return ret->second;
}

bool s213::s67( const std::wstring& nam ) const
{
	if( m.find( nam ) == m.end() )
		return false;
	return true;
}

void s213::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"Types :" << endl;

	s252::const_iterator it = m.begin();
	for( ; it != m.end(); ++it ) {
		buf << s25::fill_n_ch( L' ', ind + 5 ) << it->first << endl;
		it->second->s198( pd, ind + 10 );
	}

}

s210::s210( const s210& right )
	: s410( right.s410 ), s271( right.s271 ), s462( right.s462 )
{
}

void s210::s223( const wstring& s222, bool s273/*= true*/ )
{
	for( auto s411 : s410 ) {
		if( s411.second->s439() != s204 )
			s411.second->s223( s222 );
	}
	if( s273 ) {
		for( auto s411 : s410 ) {
			if( s411.second->s439() == s204 )
				s411.second->s223( s222 );
		}
	}
}

void s210::s274()
{
	for( auto it = s410.begin(); it != s410.end(); ++it )
		delete it->second;
	s410.clear();
}

void s210::copy( s210& t, s166* s431, s166* s354, s59 ap ) const
{
	t.s271 = s271;
	for( s270::const_iterator it = s410.begin(); it != s410.end(); ++it ) {
		if( it->second->s439() == s204 )
			t.s275( it->first, new s326( *(s326*)it->second,
				s431, s354, vector<s212*>() ), ap );
		else
			t.s275( it->first, (s211*)it->second->s226(), ap );
	}
}

void s210::s282( s210& t ) const
{
	for( s270::const_iterator it = s410.begin(); it != s410.end(); ++it ) {
		if( it->second->s439() == s204 )
			t.s275( it->first, it->second );
		else
			t.s275( it->first, (s211*)it->second->s226() );
	}
}

bool s210::s272( const std::wstring& nam, s59 ap /*= s60*/ )
{
	if( s410.find( nam ) != s410.end() ) {
		wstring rep = s96::s98 + L" name already exists: " + nam;
		if( ap == s60 || ap == s269 ) {

			if( ap == s60 )
				throw s5( (uint32_t)s5::s6::s10 );
			return false;
		}
		else if( ap == s62 ) {

			return false;
		}
	}
	return true;
}

void s210::s275( const std::wstring& nam, s211* s411, s59 ap/*= s60*/ )
{
	if( (ap == s61) || s272( nam, ap ) )
		s410[nam] = s411;
}

void s210::s275( const std::wstring& nam, s56 val, s59 ap/*= s60*/ )
{

	throw s5( (uint32_t)s5::s6::s10 );
}

void s210::s276( const std::wstring& nam )
{
	auto it = s410.find( nam );
	if( it == s410.end() )
		return;
	s410.erase( it );
}

void s210::s277( const s44::s57& block, s59 ap/*= s60*/ )
{
	vector<pair<wstring, s56>> s58;
	block.s71( s58 );
	for( size_t n = 0; n < s58.size(); ++n )
		s275( s58[n].first, s58[n].second );
}

bool s210::s67( const std::wstring& nam ) const
{
	if( s410.find( nam ) == s410.end() )
		return false;
	return true;
}

s211* s210::get( const wstring& nam, bool thr/*= true*/ ) const
{
	auto ret = s410.find( nam );
	if( ret == s410.end() ) {
		if( thr )
			throw s5( L"symbol not found: " + nam );
		else
			return NULL;
	}
	return ret->second;
}

void s210::s280( std::vector<std::wstring>& res ) const
{
	for( auto s411 : s410 )
		res.push_back( s411.first );
}

void s210::s278( vector<wstring>& res ) const
{
	for( auto s411 : s410 ) {
		if( s411.second->s439() == s204 )
			res.push_back( s411.first );
	}
}

void s210::s279( vector<wstring>& res ) const
{
	for( auto s411 : s410 ) {
		if( s411.second->s439() == s202 ||
			s411.second->s439() == s209 || s411.second->s439() == s208 )
			res.push_back( s411.first );
	}
}

void s210::s281( const s210& t,
	vector<wstring>& res ) const
{
	for( auto s411 : s410 ) {
		if( t.s67( s411.first ) )
			res.push_back( s411.first );
	}
}

void s210::s183( const s210& t, s59 ap /*= s60*/ )
{
	if( ap != s61 ) 
		for( auto s411 : t.s410 )
			s272( s411.first, ap );

	for( auto s411 : t.s410 )
		s275( s411.first, s411.second, ap );
}

void s210::s283( s248 pv, s163* block )
{
	if( !s271 )
		s271 = new s262();
	s271->s264( pv, block );
}

void s210::s268( const wstring& s228, vector<wstring>& res )
{
	for( s270::iterator it = s410.begin(); it != s410.end(); ++it ) {
		s248 pv = s249;
		if( s271 )
			pv = s271->s267( it->first, s228 );
		if( pv == s249 ) {
			if( s462 == s456 )
				pv = s250;
			else if( s462 == s458 )
				pv = s251;
			else {
				if( it->second->s439() == s204 )
					pv = s250;
				else
					pv = s251;
			}
		}
		if( pv == s250 )
			res.push_back( it->first );
	}
}

bool s210::s285( const std::wstring& s172,
																		const std::wstring& s228 ) const
{
	if( s271 ) {
		s248 pv = s271->s267( s172, s228 );
		if( pv == s250 )
			return true;
		else if( pv == s249 ) {
			if( s462 == s456 )
				pv = s250;
			else if( s462 == s458 )
				pv = s251;
			else {
				auto it = s410.find( s172 );
				if( it == s410.end() )
					throw s5( L"1: Symbol does not exist: " + s172 );
				size_t cat = it->second->s439();
				if( cat == s204 || cat == s207 || cat == s209 )
					return true;
			}
		}
		return false;
	}
	return true;
}

void s210::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"Symbols :" << endl;

	s270::const_iterator it = s410.begin();
	for( ; it != s410.end(); ++it ) {
		buf << s25::fill_n_ch( L' ', ind + INDSTEP ) << it->first << endl;
		it->second->s198( pd, ind + INDSTEP * 2 );
	}

}

s286::s286()
{

}

void s286::s223( s162* s194, const wstring& s420 )
{
	for( size_t n = 0; n < s476.size(); ++n ) {
		wstring nam = s476[n];
		vector<wstring> res;
		s194->s384( nam, s420, res );
		for( size_t m = 0; m < res.size(); ++m ) {
			s410.insert( make_pair( res[m], (s211*)s194->s338( 
																			s476[n],	s420 ) ) );
		}
	}
}

void s286::s484( const wstring& nam, s162* s194, const wstring& s420 )
{
	s476.push_back( nam );
	vector<wstring> res;
	s194->s384( nam, s420, res );
	for( size_t n = 0; n < res.size(); ++n )
		s410.insert( make_pair( res[n], (s211*)s194->s338( nam,	s420 ) ) );
}

void s286::s287( const std::wstring& s )
{
	if( s[0] != L'"' || s[s.size() - 1] != L'"' )
		throw s5( L"Unrecognized import syntax: " + s );
	s476.push_back( s.substr(1, s.size() - 2 ) );
}

bool s286::s242( const std::wstring& s411, s230* ref )
{
	s270::iterator it = s410.find( s411 );
	if( it != s410.end() )
		return it->second->s242( s411, ref, L"" );
	return false;
}

void s286::copy( s286& t ) const
{
	for( s270::const_iterator it = s410.begin(); it != s410.end(); ++it )
		t.s410[it->first] = it->second;
}

s258::s258( s248 pv_, const vector<wstring>& s259 )
	: pv( pv_ ), patterns( s259 )
{
}

s258::s258( const wstring& ag_, const wstring& sf )
{
}

s248 s258::s260( const std::wstring& s222 ) const
{
	/* Stub code. TODO with regexes. */
	for( size_t n = 0; n < patterns.size(); ++n )
		if( patterns[n] == s222 || patterns[n] == L"_" )
			return pv;
	if( pv == s251 )
		return s250;
	return s251;
}

void s262::s264( s248 pv, s163* s372 )
{
	s163::s169& s412 = s372->s187();
	s163::s169::iterator it = s412.begin();
	for( ; it != s412.end(); ++it ) {
		wstring l = it->second->s185().substr( 1, it->second->s185().size() - 2 );
		vector<wstring> res;
		wstring s393( VECELSEP, 1 );
		s25::s35( l, s393, res );
		acls[it->first] = new s258( pv, res );
	}
}

void s262::s265( const std::wstring& s, s258& pl )
{

}

void s262::s266( const std::wstring& s, const std::wstring& s388 )
{

}


s248 s262::s267( const std::wstring& mem, const std::wstring& s222 )
{
	s263::iterator it = acls.find( mem );
	if( it == acls.end() )
		return s249;

	return it->second->s260( s222 );
}
size_t s308::type;

const wstring s308::s409 = L"Null";
s293::s299 s308::s413;
size_t s304::type;

const wstring s304::s409 = L"Int";
s293::s299 s304::s413;
size_t s306::type;

const wstring s306::s409 = L"Double";
s293::s299 s306::s413;
size_t s307::type;

const wstring s307::s409 = L"String";
s293::s299 s307::s413;
size_t s309::type;
const wstring s309::s409 = L"Vector";
s293::s299 s309::s413;

s308 nullobject( NULL );

template <class charT, charT s393>
class punct_facet : public std::numpunct<charT>
{
protected:
	charT do_decimal_point() const { return s393; }
};

std::locale exprloc;

s211::~s211()
{
}


void s211::s233( s162* s194 )
{
	setlocale( LC_NUMERIC, "C" );

	s194->s254( new s308( s194 ) );
	s194->s254( new s307( s194 ) );
	s194->s254( new s304( s194, 0 ) );
	s194->s254( new s306( s194, 0 ) );
	s194->s254( new s309( s194, L"" ) );

	nullobject = s308( s194 );
}

s211* s211::s218()
{
	return this; 
}

size_t s293::s235( const std::wstring& s236 ) const
{
	auto it = s300().find( s236 );
	if( it == s300().end() )
		throw s5( L"No such operation: " + s238() + L"::" + s236,
		(uint32_t)s5::s6::s12 );
	size_t ret = it->second->s297;
	if( !ret ) // e.g. s309::get
		ret = s217();
	return ret;
}

const s211* s293::s225() const
{
	return (s211*)s194->s386( s238(), s199 );
}

s294 s293::s301( const std::wstring& s236 ) const
{
	s299::const_iterator it = s300().find( s236 );
	if( it == s300().end() )
		throw s5( L"No such operation: " + s238() + L"::" + s236,
		(uint32_t)s5::s6::s12 );
	s296* sp = it->second;
	return sp->s414;
}

s211* s293::s219( s211** s168, size_t s220 )
{
	return this;
	this->s295( s168, s220 );
	return 0;
}

s211* s293::s303( s162* s194, const std::wstring& val, const std::wstring& s228 )
{
	if( val.find_first_not_of( L"-0123456789" ) == std::string::npos ||
		val.find( L"Int(" ) == 0 ) {
		size_t pos = 0;
		return s304( s194, 0 ).s232( val, s228 );
	}
	else 
		return s306( s194, 0.0 ).s232( val, s228 );
}

s304::s304( s162* s194, int i )
	: s293( s194 ), val( i )
{
	s216 = s304::s305();
}

s304::s304( const s304& r )
	: s293( r.s194 ), val( r.val )
{
	s216 = s304::s305();
}

s304::s304( s162* s194, const std::wstring& s, size_t& pos )
	: s293( s194 )
{
	const wchar_t* pst = NULL;
	wchar_t* pend = NULL;
	long res = 0;
	if( !s.empty() ) {
		pst = s.c_str();
		if( !s40( pst, &res, &pend ) )
			throw s5( L"1: unknown syntax: " + s.substr(pos, 40),
						(uint32_t)s5::s6::s9 );
	}
	val = (int)res;
	pos = pend ? ( pend - pst ) : string::npos;
	s216 = s304::s305();
}

void s304::s234()
{
	s413[L"abs"] = new s296( L"abs", &s304::abs, s304::s305() );
	s413[L"set"] = new s296( L"set", &s304::set, s308::s305() );
	s413[L"sum"] = new s296( L"sum", &s304::sum, s304::s305() );
	s413[L"dif"] = new s296( L"dif", &s304::dif, s304::s305() );
	s413[L"mul"] = new s296( L"mul", &s304::mul, s304::s305() );
	s413[L"div"] = new s296( L"div", &s304::div, s304::s305() );
	s413[L"pow"] = new s296( L"pow", &s304::pow, s304::s305() );
	s413[L"sumeq"] = new s296( L"sumeq", &s304::sumeq, s304::s305() );
	s413[L"difeq"] = new s296( L"difeq", &s304::difeq, s304::s305() );
	s413[L"muleq"] = new s296( L"muleq", &s304::muleq, s304::s305() );
	s413[L"diveq"] = new s296( L"diveq", &s304::diveq, s304::s305() );
	s413[L"poweq"] = new s296( L"poweq", &s304::poweq, s304::s305() );
	s413[L"eq"] = new s296( L"eq", &s304::eq, s304::s305() );
	s413[L"less"] = new s296( L"less", &s304::less, s304::s305() );
	s413[L"greater"] = new s296( L"greater", &s304::greater, s304::s305() );
	s413[L"leq"] = new s296( L"leq", &s304::leq, s304::s305() );
	s413[L"geq"] = new s296( L"geq", &s304::geq, s304::s305() );
	s413[L"neq"] = new s296( L"neq", &s304::neq, s304::s305() );
	s413[L"mod"] = new s296( L"mod", &s304::mod, s304::s305() );
	s413[L"sqrt"] = new s296( L"sqrt", &s304::sqrt, s306::s305() );
	s413[L"nroot"] = new s296( L"nroot", &s304::nroot, s306::s305() );
	s413[L"toDouble"] = new s296( L"toDouble", &s304::toDouble, s306::s305() );
}

s211* s304::s239() const
{
	return new s304( s194, 0 );
}

size_t s304::s221() const
{
	return s304::s305();
}


void s304::s237( size_t type_ )
{
	type = type_;
	s216 = type_;
}

void s304::s223( const std::wstring& s228 )
{
}

s211* s304::s226() const
{
	return new s304( *this );
}

void s304::s240( s212* p ) const
{
	if( p->s221() == s304::s305() )
		*( (s304*)p )->Ptr() = val;
	else if( p->s221() == s306::s305() )
		*( (s306*)p )->Ptr() = (double)val;
	else
		throw s5( L"No conversion to type " + std::to_wstring( p->s221() ) );
}

#define OPRES ((s304*)(s212*)*s168)
#define FLAGRES ((s304*)(s212*)*s168)
#define DR ((s212*)*(s168 + 1))
#define PARN(n)((s212*)*(s168 + n))

inline void s304::abs( s211** s168, size_t s220 )
{
	int t = (int)*DR;
	if( t >= 0 )
		OPRES->val = t;
	else
		OPRES->val = -t;
}

inline void s304::set( s211** s168, size_t s220 )
{

	( (s304*)DR)->val = (int)*PARN(2);
}

inline void s304::sum( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) + (int)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val += (int)*PARN( n );
}

inline void s304::dif( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) - (int)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val -= (int)*PARN( n );
}

inline void s304::mul( s211** s168, size_t s220 )
{
	OPRES->val = ((int)*DR) * (int)*PARN(2);
	for( size_t n = 3; n < s220; ++n )
		OPRES->val *= (int)*PARN( n );
}

inline void s304::div( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) / (int)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val /= (int)*PARN( n );
}

inline void s304::pow( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) * ( (int)*DR ); ;
	for( int n = 2; n < (int)*PARN( 2 ); ++n )
		OPRES->val *= ( (int)*DR );
}

inline void s304::sumeq( s211** s168, size_t s220 )
{
	( (s304*)DR )->val += (int)*PARN(2);
	for( size_t n = 3; n < s220; ++n )
		( (s304*)DR )->val += (int)*PARN(n);
	OPRES->val = ( (int)*DR );
}

inline void s304::difeq( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) - (int)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val -= (int)*PARN( n );
}

inline void s304::muleq( s211** s168, size_t s220 )
{
	OPRES->val = ((int)*DR) * (int)*PARN(2);
	for( size_t n = 3; n < s220; ++n )
		OPRES->val *= (int)*PARN( n );
}

inline void s304::diveq( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) / (int)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val /= (int)*PARN( n );
}

inline void s304::poweq( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) * ( (int)*DR ); ;
	for( int n = 2; n < (int)*PARN( 2 ); ++n )
		OPRES->val *= ( (int)*DR );
}

inline void s304::eq( s211** s168, size_t s220 )
{
	if( ((int)*DR) == (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::less( s211** s168, size_t s220 )
{
	if( ( (int)*DR ) < (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::greater( s211** s168, size_t s220 )
{
	if( ( (int)*DR ) > (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::leq( s211** s168, size_t s220 )
{
	if( ( (int)*DR) <= (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::geq( s211** s168, size_t s220 )
{
	if( ( (int)*DR ) >= (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::neq( s211** s168, size_t s220 )
{
	if( ( (int)*DR ) != (int)*PARN( 2 ) )
		FLAGRES->val = 1;
	else
		FLAGRES->val = 0;
}

inline void s304::mod( s211** s168, size_t s220 )
{
	OPRES->val = ( (int)*DR ) % (int)*PARN( 2 );
}

inline void s304::sqrt( s211** s168, size_t s220 )
{
	*((s306*)s168[0])->Ptr() = ::sqrt( (double)*DR );
}

inline void s304::nroot( s211** s168, size_t s220 )
{
	*((s306*)s168[0])->Ptr() = ::pow( (int)*DR, (double)*PARN( 2 ) );
}

inline void s304::toDouble( s211** s168, size_t s220 )
{
	*( (s306*)s168[0] )->Ptr() = (double)*DR;
}

s211* s304::s232( const wstring& s, const wstring& s228 )
{
	if( s.find_first_not_of( L"-0123456789" ) == std::string::npos ) {
		size_t pos = 0;
		return new s304( s194, s, pos );
	}
	else if( s.find( L"Int(" ) == 0 ) {
		size_t pos = 0;
		return new s304( s194, s.substr( 4, s.size() - 5 ), pos );
	}

	return 0;
}

s211* s306::s232( const wstring& s, const wstring& s228 )
{
	if( s.find_first_not_of( L"-0123456789." ) == std::string::npos &&
		s.find( L"." ) != std::string::npos ) {
		size_t pos = 0;
		return new s306( s194, s, pos );
	}
	else if( s.find( L"Double(" ) == 0 ) {
		size_t pos = 0;
		return new s306( s194, s.substr( 7, s.size() - 8 ), pos );
	}

	return 0;
}

void s304::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"Integer : " << val << endl;

}

s306::s306( s162* s194, double d ) 
	: s293( s194 ), val( d ) 
{ 
	s216 = s306::s305();
}

s306::s306( const s306& r )
	: s293( r.s194 ), val( r.val )
{
	s216 = s306::s305();
}

s306::s306( s162* s194, const std::wstring& s, size_t& pos )
	: s293( s194 )
{
	const wchar_t* pst = NULL;
	wchar_t* pend = NULL;
	double res = 0.0;
	if( !s.empty() ) {
		pst = s.c_str();
		if( !s41( pst, &res, &pend ) )
			throw s5( L"2: unknown syntax: " + s.substr(pos, 40),
						(uint32_t)s5::s6::s9 );
	}
	val = res;
	pos = pend ? pend - pst : string::npos;
	s216 = s306::s305();
}

s211* s306::s239() const
{
	return new s306( s194, 0.0 );
}

size_t s306::s221() const
{
	return s306::s305();
}

void s306::s237( size_t type_ )
{
	type = type_;
	s216 = type_;
}

void s306::s223( const std::wstring& s228 )
{
}

void s306::s234()
{
	s413[L"abs"] = new s296( L"abs", &s306::abs, s306::s305() );
	s413[L"set"] = new s296( L"set", &s306::set, s308::s305() );
	s413[L"sum"] = new s296( L"sum", &s306::sum, s306::s305() );
	s413[L"dif"] = new s296( L"dif", &s306::dif, s306::s305() );
	s413[L"mul"] = new s296( L"mul", &s306::mul, s306::s305() );
	s413[L"div"] = new s296( L"div", &s306::div, s306::s305() );
	s413[L"pow"] = new s296( L"pow", &s306::pow, s306::s305() );
	s413[L"sumeq"] = new s296( L"sumeq", &s306::sumeq, s306::s305() );
	s413[L"difeq"] = new s296( L"difeq", &s306::difeq, s306::s305() );
	s413[L"muleq"] = new s296( L"muleq", &s306::muleq, s306::s305() );
	s413[L"diveq"] = new s296( L"diveq", &s306::diveq, s306::s305() );
	s413[L"poweq"] = new s296( L"poweq", &s306::poweq, s306::s305() );
	s413[L"sqrt"] = new s296( L"sqrt", &s306::sqrt, s306::s305() );
	s413[L"nroot"] = new s296( L"nroot", &s306::nroot, s306::s305() );
	s413[L"eq"] = new s296( L"eq", &s306::eq, s304::s305() );
	s413[L"less"] = new s296( L"less", &s306::less, s304::s305() );
	s413[L"greater"] = new s296( L"greater", &s306::greater, s304::s305() );
	s413[L"leq"] = new s296( L"leq", &s306::leq, s304::s305() );
	s413[L"geq"] = new s296( L"geq", &s306::geq, s304::s305() );
	s413[L"neq"] = new s296( L"neq", &s306::neq, s304::s305() );
	s413[L"dcmp"] = new s296( L"dcmp", &s306::dcmp, s304::s305() );
	s413[L"dcmpr"] = new s296( L"dcmpr", &s306::dcmpr, s304::s305() );
	s413[L"toInt"] = new s296( L"toInt", &s306::toInt, s304::s305() );
}

s211* s306::s226() const
{
	return new s306( *this );
}

void s306::s240( s212* p ) const
{
	if( p->s221() == s304::s305() )
		*( (s304*)p )->Ptr() = (int)val;
	else if( p->s221() == s306::s305() )
		*( (s306*)p )->Ptr() = val;
	else
		throw s5( L"No conversion to type " + std::to_wstring( p->s221() ) );
}

std::wstring s306::to_wstring() const 
{
	wstring s = std::to_wstring( val );
	size_t pl = s.find_last_not_of( L'0' );
	if( s[pl] != L'.' && s[pl] != L',' )
		++pl;
	return s.substr(0, pl);
}

#undef OPRES
#define OPRES ((s306*)(s212*)*s168)

inline void s306::set( s211** s168, size_t s220 )
{

	( (s306*)DR )->val = (double)*PARN( 2 );
}

void s306::abs( s211** s168, size_t s220 )
{
	double t = (double)*DR;
	if( t >= 0 )
		OPRES->val = t;
	else
		OPRES->val = -t;
}

inline void s306::mul( s211** s168, size_t s220 )
{
	OPRES->val = ( (double)*DR ) * (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val *= (double)*PARN( n );
}

inline void s306::sum( s211** s168, size_t s220 )
{

	OPRES->val = ( (double)*DR ) + (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val += (double)*PARN( n );
}

inline void s306::div( s211** s168, size_t s220 )
{
	OPRES->val = ( (double)*DR ) / (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val /= (double)*PARN( n );
}

inline void s306::dif( s211** s168, size_t s220 )
{
	OPRES->val = ( (double)*DR ) - (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		OPRES->val -= (double)*PARN( n );
}

inline void s306::sumeq( s211** s168, size_t s220 )
{
	( (s306*)DR )->val += (double)*PARN(2);
	for( size_t n = 3; n < s220; ++n )
		( (s306*)DR )->val += (double)*PARN(n);
	OPRES->val = ( (double)*DR );
}

inline void s306::difeq( s211** s168, size_t s220 )
{
	( (s306*)DR )->val -= (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		( (s306*)DR )->val -= (double)*PARN( n );
	OPRES->val = ( (double)*DR );
}

inline void s306::muleq( s211** s168, size_t s220 )
{
	( (s306*)DR )->val *= (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		( (s306*)DR )->val *= (double)*PARN( n );
	OPRES->val = ( (double)*DR );
}

inline void s306::diveq( s211** s168, size_t s220 )
{
	( (s306*)DR )->val /= (double)*PARN( 2 );
	for( size_t n = 3; n < s220; ++n )
		( (s306*)DR )->val /= (double)*PARN( n );
	OPRES->val = ( (double)*DR );
}

inline void s306::poweq( s211** s168, size_t s220 )
{
	( (s306*)DR )->val = ::pow( (double)*DR, (double)*PARN( 2 ) );
	for( size_t n = 3; n < s220; ++n )
		( (s306*)DR )->val = ::pow( (double)*DR, (double)*PARN( n ) );
	OPRES->val = ( (double)*DR );
}

inline void s306::eq( s211** s168, size_t s220 )
{
	double d = (double)*DR;
	if( std::abs( d - (double)*PARN(2) ) < 0.000001  ) // FIX:
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::less( s211** s168, size_t s220 )
{
	if( ((double)*DR) < (double)*PARN( 2 ) )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::greater( s211** s168, size_t s220 )
{
	if( ( (double)*DR ) > (double)*PARN( 2 ) )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::leq( s211** s168, size_t s220 )
{
	if( ( (double)*DR ) <= (double)*PARN( 2 ) )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::geq( s211** s168, size_t s220 )
{
	if( ( (double)*DR ) >= (double)*PARN( 2 ) )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::neq( s211** s168, size_t s220 )
{
	if( ( (double)*DR ) != (double)*PARN( 2 ) )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s306::sqrt( s211** s168, size_t s220 )
{
	OPRES->val = ::sqrt((double)*DR);
}

inline void s306::nroot( s211** s168, size_t s220 )
{
	*( (s306*)s168[0] )->Ptr() = ::pow( (double)*DR, (double)*PARN( 2 ) );
}

inline void s306::dcmp( s211** s168, size_t s220 )
{
	double dl = (double)*DR;
	double dr = (double)*PARN( 2 );
	double diff = (double)*PARN( 3 );
	if( dl > dr ) {
		if( dl - dr <= diff )
			*FLAGRES->Ptr() = 0;
		else
			*FLAGRES->Ptr() = 1;
	}
	else {
		if( dr - dl <= diff )
			*FLAGRES->Ptr() = 0;
		else
			*FLAGRES->Ptr() = -1;
	}
}

inline void s306::dcmpr( s211** s168, size_t s220 )
{
	double dl = (double)*DR;
	double dr = (double)*PARN( 2 );
	double diff = (double)*PARN( 3 );
	if( dl > dr ) {
		if( dl / dr - 1 <= diff )
			*FLAGRES->Ptr() = 0;
		else
			*FLAGRES->Ptr() = 1;
	}
	else {
		if( dr / dl - 1 <= diff )
			*FLAGRES->Ptr() = 0;
		else
			*FLAGRES->Ptr() = -1;
	}
}

inline void s306::pow( s211** s168, size_t s220 )
{
	OPRES->val = std::pow( ( (double)*DR ), (double)*PARN( 2 ) );
}

inline void s306::toInt( s211** s168, size_t s220 )
{
	*( (s304*)s168[0] )->Ptr() = (int)*DR;
}

void s306::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"Double : " << val << endl;

}

s307::s307( s162* s194, const std::wstring& s )
	: s293( s194 ), val( s )
{
	s216 = s307::s305();
}


s307::s307( const s307& r )
	: s293( r.s194 ), val( r.val )
{
	s216 = s307::s305();
}

s307::s307( s162* s194, const std::wstring& s, size_t& pos )
	:s293( s194 )
{
	val = s;
	pos = string::npos;
	s216 = s307::s305();
}

s211* s307::s239() const
{
	return new s307( s194, L"" );
}

size_t s307::s221() const
{
	return s307::s305();
}


void s307::s234()
{
	s413[L"set"] = new s296( L"set", &s307::set, s308::s305() );
	s413[L"sum"] = new s296( L"sum", &s307::sum, s307::s305() );
	s413[L"eq"] = new s296( L"eq", &s307::eq, s304::s305() );
	s413[L"less"] = new s296( L"less", &s307::less, s304::s305() );
	s413[L"greater"] = new s296( L"greater", &s307::greater, s304::s305() );
	s413[L"leq"] = new s296( L"leq", &s307::leq, s304::s305() );
	s413[L"geq"] = new s296( L"geq", &s307::geq, s304::s305() );
	s413[L"neq"] = new s296( L"neq", &s307::neq, s304::s305() );
}

void s307::s237( size_t type_ )
{
	type = type_;
	s216 = type_;
}

void s307::s223( const wstring& s228 )
{
}

wstring s415( wchar_t c )
{
	wstring ret;
	if( c == L'n' )
		ret = L"\n";
	else if( c == L't' )
		ret = L"    ";
	else
		throw s5( L"Unknown escape sequence: \\" + c );
	return ret;
}

s211* s307::s232( const wstring& s, const wstring& s228 )
{
	wstring val;
	size_t pl;
	if( s.find( L"String(" ) == 0 )
		val = s.substr( 7, s.size() - 8 );
	else if( s.size() == 4 && s[0] == L'\'' && s[1] == L'\\' && s[3] == L'\'' )
		val = s415( s[2] );
	else if( ( pl = s.find( L"\\" ) ) != string::npos ) {
		val = s;
		while( pl != string::npos ) {
			if( pl == val.size() - 1 )
				break;
			wstring code = s415( val[pl + 1] );
			val = val.replace( pl, 2, code );
			pl = val.find( L"\\" );			
		}
	}

	if( val.empty() )
		val = s;

	return new s307( s194, val );
}

s211* s307::s226() const
{
	return new s307( *this );
}

void s307::s240( s212* p ) const
{
	if( p->s221() == s307::s305() )
		*( (s307*)p )->Ptr() = val;
	else if( p->s221() == s304::s305() )
		*( (s304*)p )->Ptr() = std::stoi( val );
	else if( p->s221() == s306::s305() )
		*( (s306*)p )->Ptr() = std::stof( val );
	else
		throw s5( L"No conversion to type " + std::to_wstring( p->s221() ) );
}

#undef OPRES
#define OPRES ((s307*)(s212*)*s168)


inline void s307::set( s211** s168, size_t s220 )
{

	( (s307*)DR )->val = ( (s307*)PARN( 2 ))->val;
}

inline void s307::sum( s211** s168, size_t s220 )
{
	OPRES->val = DR->to_wstring() + PARN( 2 )->to_wstring();
	for( size_t n = 3; n < s220; ++n )
		OPRES->val += PARN( n )->to_wstring();
}

inline void s307::eq( s211** s168, size_t s220 )
{
	if( DR->to_wstring() == PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s307::less( s211** s168, size_t s220 )
{
	if( DR->to_wstring() < PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s307::greater( s211** s168, size_t s220 )
{
	if( DR->to_wstring() > PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s307::leq( s211** s168, size_t s220 )
{
	if( DR->to_wstring() <= PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s307::geq( s211** s168, size_t s220 )
{
	if( DR->to_wstring() >= PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

inline void s307::neq( s211** s168, size_t s220 )
{
	if( DR->to_wstring() != PARN( 2 )->to_wstring() )
		*FLAGRES->Ptr() = 1;
	else
		*FLAGRES->Ptr() = 0;
}

void s307::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"String : " << val << endl;

}

size_t s308::s221() const
{
	return s308::s305();
}

void s308::s237( size_t type_ )
{
	type = type_;
	s216 = type_;
}

void s308::s223( const wstring& s228 )
{
}

s211* s308::s239() const
{
	return new s308( s194 );
}

s211* s308::s232( const wstring& s, const wstring& s228 )
{
	if( s.find( L"Null" ) == 0 ) {
		size_t pos = 0;
		return &nullobject;
	}

	return NULL;
}

void s308::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"NullValue : -" << endl;

}

s309::s309( s162* s194, const wstring& sf )
	: s293( s194 ), s474( 0 )
{
	if( sf.size() )
		val.push_back( new s290( s194, sf, L"" ) );
	s216 = s309::s305();
}

s309::s309( const s309& right )
	: s293( right.s194 ), s474( 0 )
{
	val = right.val;
	s216 = right.s216;
}

s211* s309::s232( const wstring& s, const wstring& s228 )
{
	wstring sv;
	if( s[0] == L'[' && s[s.size() - 1] == L']' )
		sv = s25::s30( s, L"[]" );
	else if( s.find( s409 ) == 0 && s[s.size() - 1] == L')' ) {
		sv = s25::s30( s.substr( s409.size() ), s26 );
		if( sv[0] != L'(' )
			throw s5( L"wrong vector constructor syntax" );
		sv = s25::s30( sv.substr( 1, sv.size() - 2 ), L" " );
		wstring tp, s74=L"";
		if( isdigit( sv[sv.size() - 1] ) ) {
			s74 = sv.substr( sv.rfind( L" " ) + 1 );
			tp = sv.substr( 0, sv.size() - s74.size() );
		}
		else {
			s74 = L" 0";
			tp = sv;
		}

		sv = L"]" + tp + s74;
	}
	else
		return 0;
	return new s309( s194, sv );
}

void s309::s223( const wstring& s228 )
{
	s290* pst = (s290*)val[0];
	val.clear();
	const wstring& sf = pst->Str();
	if( sf[0] == L']' ) {
		wstring s416 =  sf.substr( sf.rfind( L" " ) );
		wstring s417 = sf.substr( 1, sf.size() - s416.size() - 1 );
		size_t s74 = std::stoi( s416 );
		for( size_t n = 0; n < s74; ++n )
			val.push_back( s194->s381( s417, s228 ) );
		if( s74 == 0 )
			s474 = s194->s381( s417, s228 )->s217();
		else
			s474 = val[0]->s217();
	}
	else {
		size_t s400 = 0;
		size_t s418, s419;
		while( s400 < sf.size() - 1 ) {
			size_t pl = sf.find_first_not_of( s26, s400 );
			if( pl == string::npos )
				break;
			if( s400 != 0 ) {
				if( sf[pl] != VECELSEP )
					throw s5( L"wrong vector syntax: missed separator" );
				++pl;
			}
			pl = sf.find_first_not_of( s26, pl );
			if( pl == string::npos || sf[pl] == VECELSEP )
				throw s5( L"wrong vector syntax: empty element" );

			if( wstring( L"\'\"`" ).find( sf[pl] ) != string::npos ) {
				s25::s33( sf, pl, sf[pl], s418, s419 );
				if( s419 == string::npos )
					throw s5( L"wrong vector syntax: mismatched quotes" );
			}
			else {
				s418 = pl;
				s419 = sf.find_first_of( s26 + VECELSEP, pl );
			}
			s400 = s419;
			wstring sel = sf.substr( s418, s419 - s418 );
			val.push_back( s194->s381( sel, s228 ) );
			if( !s474 )
				s474 = val[0]->s217();
			else if( s474 != val.back()->s217() )
				throw s5( L"different types in vector" );
		}
	}
	delete pst;
	s216 = s309::s305();
	s413[L"get"]->s297 = s474;
	s222 = s228;
}

void s309::s234()
{
	s413[L"set"] = new s296( L"set", &s309::set, s308::s305() );
	s413[L"get"] = new s296( L"get", &s309::get, 0 );
	s413[L"add"] = new s296( L"add", &s309::add, s308::s305() );
	s413[L"size"] = new s296( L"size", &s309::size, s304::s305() );
	s413[L"resize"] = new s296( L"resize", &s309::resize, s308::s305() );
	s413[L"eq"] = new s296( L"eq", &s309::eq, s304::s305() );
}

s211* s309::s239() const
{
	return new s309( s194, L"" );
}

size_t s309::s221() const
{
	return s309::s305();
}

void s309::s237( size_t type_ )
{
	type = type_;
	s216 = type_;
}

size_t s309::s217() const
{
	if( val.empty() || val[0]->s217() == s205 )
		throw s5( L"vector is not initialized" );

		return val[0]->s217();
	return s216;
}

inline bool
s309::isTypeOf( const s211* classObj ) const
{

	return (( (s309*)classObj )->s474 == s474);
}

s211* s309::s226() const
{
	return new s309( *this );
}

void s309::s240( s212* p ) const
{
}

#undef OPRES
#define OPRES ((s309*)(s212*)*s168)


inline void s309::set( s211** s168, size_t s220 )
{

	s168[3]->s240( ( (s309*)DR )->val[(int)*s168[2]]);
}

inline void s309::get( s211** s168, size_t s220 )
{
	*s168 = (s211*)( (s309*)DR )->val[(int)*s168[2]];
}

inline void s309::add( s211** s168, size_t s220 )
{
	if( ( (s309*)DR )->s312() != s168[2]->s217() )
		throw s5( L"unmatched element type" );
	( (s309*)DR )->val.push_back( s168[2]->s226() );
}

inline void s309::size( s211** s168, size_t s220 )
{
	*((s304*)*s168)->Ptr() = (int)( (s309*)DR )->val.size();
}

inline void s309::resize( s211** s168, size_t s220 )
{
	int size = (int)*s168[2];
	( (s309*)DR )->val.resize( size );
	for( int i = 0; i < size; ++i )
		( (s309*)DR )->val[i] = ( (s309*)DR )->s194->s381(
		( (s309*)DR )->s474, ( (s309*)DR )->s222 );
}

inline void s309::eq( s211** s168, size_t s220 )
{
	size_t size = ( (s309*)DR )->val.size( ); 
	s309* right = (s309*)s168[2];
	if( right->val.size() != size ) {
		*( (s304*)*s168 )->Ptr() = 0;
		return;
	}
	for( size_t n = 0; n < size; ++n ) {
		if( right->val[n] != ( (s309*)DR )->val[n] ) {
			*( (s304*)*s168 )->Ptr() = 0;
			return;
		}
	}
	*( (s304*)*s168 )->Ptr() = 1;
}

wstring s309::to_wstring() const 
{ 
	wstring ret = L"[";
	for( size_t n = 0; n < val.size(); ++n )
		ret += val[n]->to_wstring() + ( n == val.size() - 1 ? L"" : L", " );
	ret += L"]";
	return ret; 
}

void s309::s198( std::wostream* pd, int ind /*=0*/ ) const
{
	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"Vector : " << endl;
	for( size_t n = 0; n < val.size(); ++n )
		val[n]->s198( pd, ind + INDSTEP );
}


s230::s230( const s230& right )
	: s212( right.s194, s203 ), s231( right.s231 ), s314( right.s314 ), s316( 0 )

{

}

void s230::s223( const s166* s354, const wstring& s420 )
{
	if( !s354->s242( s231, this, s420 ) )
		throw s5( L"Symbol is not resolved: " + s231 );
}

s211* s230::s317( s166* ob )
{
	return 0;
}

s211* s230::s320( const s166* ob ) const
{
	if( s315.empty() )

		return s313;

	s166* ret = (s166*)((s166*)s313)->s337( *(size_t*)&s315[0] );
	for( size_t n = 1; n < s316; ++n )

		ret = (s166*)((s166*)ret->s218())->s337( *( (size_t*)&s315[0] + n ) );
	return ret->s218();
}

s211* s230::s321( s166* ob )
{
	if( s315.empty() )

		return (s211*)s313;

		ob = (s166*)s313;
	s166* ret = (s166*)ob->s337( *(size_t*)&s315[0] );
	for( size_t n = 1; n < s316; ++n )
		ret = (s166*)ret->s337( *( (size_t*)&s315[0] + n ) );
	return ret;
}

s211* s230::s317( s211** s318 )
{
	return 0;
}

s211* s230::s219( s211** s168, size_t s220 )
{
	return s320( NULL )->s219( s168, s220 );
}

s211* s230::s218()
{
	return s320( NULL )->s218();
}

bool s230::isTypeOf( const s211* classObj ) const
{
	return s320( NULL )->isTypeOf( classObj );
}

s165* s230::s226() const
{
	return new s230( *this );
}

void s230::s319( size_t off, const s211* s475 )
{
	s315.push_back( off );
	s316 = s315.size();
	s314 = s475;
}

void s230::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"DataRef : " << s231 << endl;

}

s290::s290( const s290& right )
	: s165( right.s194, s205 ), str( right.str )
{

}

s165* s290::s226() const
{
	return new s290( *this );
}

void s290::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << L"DataStub : " << str << endl;

}
const wstring s199 = L"";

s210 s423;

s166::s166( s162* s194, size_t type_, s200 pc/* = s201*/ )
	: s211( s194, pc ), s409( s199 ), //s332( s423 ),
	type( type_ ), s329( NULL ), ns( NULL )
{
	s327 = this;
}

s166::s166( const s166& s475 )
	: s211( s475.s194, s475.s215 ), s409( s475.s172 ),
	 s327( &s475 ), s329( s475.s329 ), //s332( s423 ),
	s422( s475.s422 ), ns( s475.ns )
{
	type = s475.type;
	s216 = s475.s216;
	if( s215 == s207 )
		s215 = s209;
	if( s475.s510 )
		s475.s510->s487( this );

	s475.s328.copy( s328, this, NULL, s210::s61 );
	for( size_t n = 0; n < s422.size(); ++n )
		s330.push_back( s328.get( s422[n] ) );
}

s166::s166( const wstring& s298, s162* s194, s200 pc )
	: s211( s194, pc ), s172( s298 ), s409( s199 ),
		s329( NULL ), ns( NULL )//, s332( *new s210() )
{
	s327 = this;
}

s166::s166( s163& s372, s162* s194, s200 pc, s166* ns_, s455 am, 
	s478& cl, s479& cnl, s480& al )
	: s211( s194, pc ), s172( s372.s192() ), s409( s372.s193() ),
		s329( NULL ), ns( ns_ ),  s328( am ), s510( NULL ) //,s332( *new s210() )
{
	if( s172.find( L" " ) != string::npos )
		throw s5( L"Malformed class name: " + s172 );
	s327 = this;

	if( pc == s207 ) {
		wstring anc = L"";
		try {
			const s163& ancNam = s372.s186( s96::s129 );
			anc = ancNam.s185();
		}
		catch( s5& e ) {
			if( e.which() != (uint32_t)s5::s6::s12 )
				throw e;
		}
		cnl.push_back( make_pair( s372.s193(), anc ) );

		if( cl.find( s372.s193() ) != cl.end() )
			throw s5( L"class already exists: " + s372.s193() );

		cl.insert(make_pair(s372.s193(), this));
	}
	if( al.find( s372.s193() ) != al.end() )
		throw s5( L"object already exists: " + s372.s193() );
	al.insert(make_pair(s372.s193(), &s372));

	s241( L"proto" );
	s163::s169& s412 = s372.s187();
	s163::s169::iterator it = s412.begin();
	vector<pair<wstring,wstring>> vswap;
	for( ; it != s412.end(); ++it ) {
		if( it->second->s191() == s131 ) {
			if( it->first == s96::s129 ) {
				s483 = it->second->s185();

			}
		}
		else if( it->second->s191() == s133 ) {

			wstring s402 = it->first;
			s248 pv = s251;
			am = s457;
			
			size_t seppl = s402.find_first_of( s25::s26 );
			if( seppl != string::npos ) {
				vector<wstring> vres;
				s25::s35( it->first, wstring( 1, s402[seppl] ), vres );
				if( vres.size() > 3 )
					throw s5( L"malformed block name: " + it->first );
				if( vres[0] == s96::s115 || vres.size() == 3 ) {
					pc = s207;
					if( vres.size() == 3 ) {
						if( vres[0] == s96::s459 )
							am = s456;
						else if( vres[0] == s96::s461 )
							am = s458;
						else if( vres[0] != s96::s460 )
							throw s5( L"invalid access mode name: " + it->first );
						s402 = vres[2];
					}
					else
						s402 = vres[1];

					if( s412.find( s402 ) != s412.end() )
						throw s5( L"duplicated object name: " + it->first );
					it->second->s188( s402 );
	
					vswap.push_back( make_pair( it->first, s402 ) );
				}
				else if( vres[0] == L"access" && ( vres[1] == L"allow" || vres[1] == L"deny" )) {
					if( vres[1] == L"allow" )
						pv = s250;
					pc = s208;
				}
				else
					throw s5( L"unknown object type: " + vres[0] );
			}
			if( pc == s208 ) {

				s328.s283( pv, it->second );
			}
			else {

				s166* ob = new s166( *it->second, s194, s207, this, am, cl, cnl, al );
				s328.s275( ob->s193(), ob );
			}
		}
	}
	for( size_t n = 0; n < vswap.size(); ++n ) {
		auto it = s412.find( vswap[n].first );
		s412.insert(make_pair(vswap[n].second, it->second ));
		s412.erase( it );
	}
}

s211* s166::s232( s163& s372, const wstring& s228 )
{

	return s424( s372, s228 );
}

s211* s166::s232( const wstring& val, const wstring& s256 )
{
	if( s215 == s207 )
		if( !ns->s339( s172, s256 ) )
			throw s5( L"Class '" + s172 + L"' is not accessible from '" 
				+ s256 + L"'." );
	s163 q( L"", val );
	return s232( q, s256);
}

void s166::s237( size_t type_ )
{
	type = type_;
}

void s166::s486( const s210& s168 )
{

}

bool
s166::isTypeOf( const s211* classObj ) const
{
	if( classObj->s217() == s217() )
		return true;
	if( s510 )
		return s510->isTypeOf( classObj );
	return false;
}

void s166::s65( s163& s372 )
{
	s163::s169& s412 = s372.s187();
	s163::s169::iterator it = s412.begin();
	for( ; it != s412.end(); ++it ) {
		s163* node = it->second;
		if( node->s191() == s132 ) {
			s326* func = s326::s356( *node, s194, this,
				node->s193(), s172 );
			s328.s275( func->s193(), func );
			s330.push_back( func );
			s422.push_back( func->s193() );
		}
		else if( node->s191() == s131 ) {
			wstring nam = node->s193();
			if( nam == s96::s110 )
				s425( *node );
			else if( nam == s96::s129 )
				s485( *node );
			else {
				s211* dv = s194->s381( node->s185(), s172 );
				dv->s241( node->s193() );
				try {
					s328.s275( node->s193(), dv );
				}
				catch( s5& e ) {
					if( e.which() == (uint32_t)s5::s6::s10 ) {
						e.addInfo( L"Symbol already defined: " + s172 + L"::" + node->s193() );
					}
					throw e;
				}
				s330.push_back( dv );
				s422.push_back( node->s193() );
			}
		}
		else if( node->s191() == s133 ) {

			if( 0 && it->first.find( L"access" ) != 0 ) {
				s166* ob = (s166*)s328.get( it->first );
				ob->s65( *it->second );
			}
		}
		else if( node->s191() == s134 ) {

		}
	}
}

void s166::s223( const wstring& s222 )
{
	if( s483.size() ) {
		s510 = (s166*)s194->s386( s483, s222 );
		s510->s329->copy( *s329 );
	}

	if( s329 )
		s329->s223( s194, s172 );
	else
		s329 = s327->s329;

	s328.s223( s222, (s215 == s207));

	if( s328.s67( s96::s106 ) ) {
		vector<s212*> s168;

		s168.push_back( this );
		s326* p = (s326*) s328.get( s96::s106 );
		p->s223( s222 );
		p->s219( (s211**)&s168[0], 1 );
	}
}

void s166::s487( s166* desc ) const
{
	if( s510 )
		s510->s487( desc );

}

void s166::s425( s163& s372 )
{
	if( !s329 )
		s329 = new s286();
	s329->s287( s372.s185() );

	return;

}

void s166::s426( const wstring& s228,	vector<wstring>& res )
{
	s328.s268( s228, res );

}

void s166::s484( const wstring& nam, const wstring& s420 )
{
	if( !s329 )
		s329 = new s286();
	s329->s484( nam, s194, s420 );
}

void s166::s485( s163& s372 )
{

	s510 = (s166*)s194->s386( s483, s172 );
}

bool s166::s242( const wstring& sym_, 
																s230* ref, const wstring& s228 ) const
{
	bool ret = false;
	wstring s411 = sym_;
	size_t pl = s411.find( L"." );
	if( pl != string::npos ) {
		s411 = sym_.substr( pl + 1 );
		wstring s448 = sym_.substr( 0, pl );
		s166* ob = (s166*)s338( s448, s228, false );
		if( ob ) {

			ret = ob->s242( s411, ref, s228 );
		}
		else {
			ret = s329->s242( s411, ref );

		}
	}
	else {
		s166* ob = (s166*)s338( s411, s228, false );
		if( ob ) {
			if( !ref->s323() )
				ref->s324( ( s211* )this );

			ref->s319( s335( s411 ), ob );
			ret = true;
		}
		else {
			if( s329 )
				ret = s329->s242( s411, ref );

		}
	}

	return ret;
}

s211* s166::s336( const std::wstring& s172, const std::wstring& s228 )
{
	return s328.get( s172 );
}

size_t s166::s335( const std::wstring& memname ) const
{
	for( size_t n = 0; n < s330.size(); ++n ) {
		if( s422[n] == memname )
			return n;
	}
	throw s5( L"3: Symbol does not exist: " + memname );
}

s165* s166::s337( size_t off )
{
	return s330[off];
}

s165* 
s166::s338( const wstring& s411, const wstring& s228, 
											bool thr/*= true*/ ) const
{
	size_t pl = s411.find( L"::" );
	if( pl != string::npos ) {
		wstring smod = s411.substr( 0, pl );
		wstring sym1 = s411.substr( pl + 2 );
		s166* mod = (s166*)s328.get( smod, true );
		return mod->s338( sym1, s228, thr );
	}
	wstring sym1 = s411;
	if( s411.size() > 2 && ( s411.find( L"()" ) == s411.size() - 2 ) )
		sym1 = s411.substr( 0, s411.size() - 2 );
	s165* ret = s328.get( sym1, false );
	if( !ret && thr )
		throw s5( L"4: Symbol does not exist: " + s411, 
			(uint32_t)s5::s6::s12 );
	if( ret && !s339( sym1, s228 ) ) {
		if( thr )
			throw s5( L"4: Symbol not found: " + s411,
				(uint32_t)s5::s6::s12 );
		ret = NULL;
	}
	return ret;
}

bool s166::s339( const wstring& s411, const wstring& s222 ) const
{
	return s328.s285( s411, s222 );
}

size_t s166::s235( const wstring& s236 ) const
{
	s211* s427 = s328.get( s236 );
	size_t ret = s427->s221();
	if( ret == 0 ) {
		s427->s223( s193() );
		ret = s427->s221();
	}
	return ret;
}

s211* s166::s424( s163& s372, const std::wstring& s228 )
{
	vector<wstring> s318;
	vector<s165*> s168;
	s166* ret = new s166( *this );
	size_t pos = s372.s185().find( L"(" );
	size_t pos_ = pos;
	s163::s182( s372.s185(), pos, s318 );
	if( s318.size() ) {
		s326* tdInit = (s326*)s328.get( s96::s105, false );
		if( !tdInit )
			throw s5( L"Arguments passed to a class without init function." );
		else {
			wstring str = s372.s185();
			if( str[pos_ + 1] != L' ' )
				str.insert( pos_ + 1, L" " );
			str = str.substr( pos_ ).insert( 1, s96::s105 );
			s163 s467( s96::s106, s55( str ) );
			s428* cs = (s428*)s165::s227( s467, s194, ret, s228 );

			s326* td = s326::s358( 
				cs, s194, ret, ret, s172 );


			ret->s328.s275( s96::s106, td );
		}
	}

	return ret;
}

s211* s166::s239() const
{
	return new s166( *this );
}

size_t s166::s221() const
{
	return type;
}

std::wstring s166::s192() const
{
	return /*s171 + L"::" +*/ s172;
}

s211* s166::s219( s211** s168, size_t s220 )
{
	return s333->s219( s168, s220 );
}

s326* s166::s301( const std::wstring& s298, const std::wstring& s228,
	bool thr /*= true*/ ) const
{

	s230 ref( s298, s194 );
	if( !s242( s298, &ref, s228 ) )
		return false;
	return (s326*)ref.s321( NULL );
}

s165* s166::s226() const
{
	return new s166( *this );
}

void s166::s240( s212* p ) const
{

	throw s5( L"" );
	vector<wstring> fields;
	s328.s279( fields );

	for( size_t n = 0; n < fields.size(); n++ )
		((s211*)s328.get( fields[n] ))->s240( p );
}

wstring s166::to_wstring() const
{
	return L"";
}

void s166::s198( std::wostream* pd, int ind /*=0*/ ) const
{

	std::wostream& buf = *pd;
	buf << endl << s25::fill_n_ch( L' ', ind ) << s172 << L" : " << L"Object" << endl;
	s328.s198( pd, ind + INDSTEP );

}
Transd* s326::s356( s163& s372, s162* s194, s166* obj,
	const wstring& s172, const wstring& s420 )
{
	s163::s169& s412 = s372.s187();
	size_t s429 = s412.size();
	vector<s212*> s168;
	for( size_t n = 0; n < s429; ++n ) {
		s163::s169::iterator it = s412.find( std::to_wstring( n ) );
		s168.push_back( s165::s227( *it->second, s194, obj, s420 ) );
	}
	Transd* ret = NULL;
	size_t seppl = s172.find_first_of( s25::s26 );
	if( seppl != string::npos ) {
		vector<wstring> vres;
		s25::s35( s172, wstring( 1, s172[seppl] ), vres );
		if( vres.size() > 2 )
			throw s5( L"malformed function name: " + s172 );
		if( vres[0] == s96::s108 ) {
			ret = new s501( s194, obj, NULL, s168 );
			ret->s172 = vres[1];
		}
		else if( vres[0] == s96::s109 ) {
			ret = new s326( s194, obj, NULL, s168 );
			s166* pAnc = (s166*)s194->s386( obj->s482(), s420 );
			( (s501*)pAnc->s301( vres[1], s420 ) )->s502( obj->s221(),
																		ret );
			ret->s172 = vres[1];
			ret->s499( s492 );
		}
		else
			throw s5( L"malformed function name: " + s172 );
	}
	else {
		ret = new s326( s194, obj, NULL, s168 );
		ret->s172 = s172;
	}

	return ret;
}

s428* s326::s357( s163& s372, s162* s194, s166* obj, const wstring& s420 )
{
	s163::s169& s412 = s372.s187();
	size_t s429 = s412.size();
	vector<s212*> s168;
	for( size_t n = 0; n < s429; ++n ) {
		s163::s169::iterator it = s412.find( std::to_wstring( n ) );
		s168.push_back( s165::s227( *it->second, s194, obj, s420 ) );
	}

	wstring s359 = s372.s185();
	if( s359.find( L"::" ) != string::npos ) {
		wstring s430 = s359.substr( 0, s359.rfind( L"::" ) );
		s359 = s359.substr( s430.size() + 2 );
		s163* p = new s163( s430 );
		s168.insert( s168.begin(), s165::s227( *p, s194, obj, s420 ) );
	}

	return new s428( s359, s168 );
}

s501::s501( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l )
	: s326( s194, s431, s354, l )
{
	virtType = s491;
}

void s501::s502( size_t typ, s326* pOvr )
{
	types.push_back( typ );
	ptrs.push_back( pOvr );
}

s211*
s501::s219( s211** s318, size_t s429 )
{
	if( s348 == s347 && s318[0]->s221() != type ) {
		size_t type = s318[0]->s221();
		size_t* pt = (size_t*)&types[0];
		for( size_t n = 0; n < types.size(); ++n ) {
			if( pt[n] == type )
				return ptrs[n]->s219( s318, s429 );
		}
		throw s5( L"dispatching failed" );
	}
	else {
		return s326::s219( s318, s429 );
	}
}


void s428::s223( const wstring& s228 )
{
	for( size_t n = 0; n < s168.size(); ++n ) {
		s168[n]->s223( s228 );
	}
}

s165* s428::s226() const
{
	return new s428( *this );
}

Transd* s326::s358( s428* cs, s162* s194, s166* s431,
	s166* s432, const wstring& s420 )
{
	wstring s359 = cs->s193();
	if( s359 == s139 )
		return s194->s379( cs->s193(), cs->s360(), s431, s432 );

	vector<s212*>& s434 = cs->s360();
	vector<s212*> s168;
	const s211* s433 = NULL;
	wstring s435 = s431->s192();
	s326* ret = NULL;
	s165* s437 = NULL;
	s165* s436 = NULL;

	for( size_t n = 0; n < s434.size(); ++n ) {
		s437 = s434[n];
		s200 pc = s437->s439();
		if( pc == s203 ) {
			s436 = s437;
			( (s230*)s436 )->s223( s432, s420 );
		}
		else {
			if( pc == s205 ) {
				s436 = s194->s381( ( (s290*)s437 )->Str(),
					( (s290*)s437 )->s473() );

			}
			else if( pc == s206 )
				s436 = s358( (s428*)s437, s194, s431, s432, s420 );
			else
				s436 = s437;

			s436->s223( s431->s193() );
		}
		s168.push_back( s436 );
	}

	s326* s442 = s194->s379( s359, s168, s431, s432 );
	if( s442 )
		return s442;

	s326* s440 = NULL;
	s326* s441 = NULL;
	s294 s340 = NULL;

	if( s168.empty() ) {
		s440 = s431->s301( s359, s435, false );
		if( !s440 )
			throw s5( L"Procedure is not resolved: " + s359 );
	}
	else {
		s436 = s168[0];
		if( s436->s439() == s203 ) {
			if( ( (s230*)s437 )->ValName() == s96::s107 )
				s436 = s431;
			s433 = (s211*)( (s230*)s436 )->s320( NULL );
		}
		else
			s433 = s436->s218();

		s200 s438 = s433->s439();

		if( s438 == s203 ) {
			s230* ref = (s230*)s433;
			s433 = ref->s225();
			s438 = s433->s439();
		}

		try {
			if( s438 == s209 || s438 == s208 )
				s441 = ( (s166*)s433 )->s301( s359, s435, false );
			else if( s438 == s202 )
				s340 = ( (s293*)s433 )->s301( s359 );
		}
		catch( s5& e ) {
			if( e.which() != (uint32_t)s5::s6::s12 )
				throw;
		}

		if( !s441 && !s340 )
			s440 = s431->s301( s359, s435, false );


		if( !( /*s442 ||*/ s340 || s440 || s441 ) )
			throw s5( L"Method is not resolved" );
	}

	if( s340 ) {

		ret = new s326( s194, s432, s168, s340, s433->s235( s359 ) );
		ret->s348 = s344;

	}
	else if( s440 ) {

		ret = new s326( *(s326*)s440, s431, s432, s168 );
		ret->s348 = s345;

	}
	else if( s441 ) {
		s326* td = (s326*)s441->s226();
		s168.erase( s168.begin() );
		ret = new s326( *td, (s166*)s433, s432, s168 );
		delete td;
		if( s441->s500() == s491 )
			ret->s348 = s347;
		else
			ret->s348 = s346;

	}

	ret->s172 = s359;
	return ret;
}

s326::s326( s162* s194, s166* s443, s166* s444, const vector<s212*>& l )
	: s166( s194, 0, s204 ), s340( NULL ), s431( s443 ), s354( s444 ), s449( NULL ),
		virtType( s490 )
{
	size_t n = 0;
	for( ; n < l.size(); n += 2 ) {
		if( l[n]->s439() != s203 )
			break;
		s230* nam = (s230*)l[n];
		s211* ex = (s211*)l[n + 1];
		if( ex->s439() != s202 && ex->s439() != s209 && 
				ex->s439() != s208 && ex->s439() != s205 )
			throw s5( L"incorrect syntax for lambda parameter " + nam->ValName() +
				L"; should be a type constructor.", (uint32_t)s5::s6::s9 );
		s351.s275( nam->ValName(), ex );
		s328.s275( nam->ValName(), ex );
		s494.push_back( (s211*)ex );
		s330.push_back( (s211*)ex );
		s422.push_back( nam->ValName() );
	}

	s353.insert( s353.end(), l.begin() + n, l.end() );
}

s326::s326( s162* s194, s166* s444, const std::vector<s212*>& l,
	s294 s445, size_t s446 )
	: s166( s194, 0, s204 ), s340( s445 ), s431( NULL ), s354( s444 ), s449( NULL ), virtType( s490 )
{
	s216 = s446;
	s168 = l;
}

s326::s326( const s326& right, s166* s443, s166* s444,
				const vector<s212*>& s434 )
	: s166( right.s194, right.s216, right.s215 ), 

		s172( right.s172 ), s351( right.s351 ),
		s431( s443 ), s354( s444 ), s449( NULL ), virtType( s490 )
{

	s340 = right.s340;
	s348 = right.s348;
	s330 = right.s330;
	s422 = right.s422;
	s328 = right.s328;
	s494 = right.s494;

	if( s434.size() ) {
		s168.assign( s434.begin(), s434.end() );

		for( size_t n = 0; n < s168.size(); ++n )
			s351.s275( s422[n], (s211*)s168[n]/*->s218()*/, s210::s61 );
	}
	else
		s168.assign( right.s168.begin(), right.s168.end() );

	s353 = right.s353;
}

s326::~s326()
{
	for( size_t n = 0; n < s168.size(); ++n ) {
		delete s168[n];
	}
}

s428::s428( const std::wstring& s396, const std::vector<s212*>& l )
	: s165( NULL, s206 ), s359( s396 )
{
	s168.assign( l.begin(), l.end() );
}

s428::s428( const s428& right )
	: s165( right.s194, s206 ), s359( right.s359 )
{
	for( size_t n = 0; n < right.s168.size(); ++n ) {
		s168.push_back( right.s168[n]->s226() );
	}
}

void s326::s495(const std::wstring& s228)
{
	for( size_t n = 0; n < s494.size(); ++n ) {
		s165* fpar = s494[n];
		s200 pc = fpar->s439();
		if( pc == s205 ) {

		}
		else

			s494[n]->s223( s228 );

	}
}

void s326::s496(const std::wstring& s228)
{
	for( size_t n = 0; n < s168.size(); ++n ) {
		s165* s436 = s168[n];
		if( s436->s439() == s205 ) {
			s211* s437 = s194->s381( ( (s290*)s168[n] )->Str(),
					( (s290*)s168[n] )->s473() );
			s168[n] = s437;
		}
		else if( s436->s439() == s206 ) {
			s428* cs = new s428( *(s428*)s436 );
			s436 = s358( cs, s194, s431, this, s192() );
			s436->s223( s228 );
			s168[n] = s436;
		}
		if( s168[n]->s439() != s203 ) {
			if( !s340 && s348 != s343 ) {
				s330[n] = s168[n]->s218();
				s351.s275( s422[n], (s211*)s330[n], s210::s61 );
			}
			s352.push_back( s168[n]->s218() );
		}
		else {
			if( !s340 && s348 != s343 ) {
				s330[n] = s168[n];
				s351.s275( s422[n], (s211*)( (s230*)s168[n] )->Root(), s210::s61 );
			}
			s352.push_back( NULL );
		}
		if( n < s330.size() && !( (s211*)s330[n] )->isTypeOf( (s211*)s494[n] ) )
			throw s5( L"wrong argument's type" );
	}
}

void s326::s223( const wstring& s228 )
{

	if( s431 && ( s431->s439() == s208 || s431->s439() == s207 ) ) {
		s495( s228 );
		if( s431->s439() == s207 )
			return;
	}

		s498( s228 );

	if( !s340 ) {
		for( size_t n = 0; n < s353.size(); ++n ) {
			s428* cs = new s428( *(s428*)s353[n] );
			s353[n] = s358( cs, s194, s431, this, s431->s193() );
		}
		for( auto it : s353 ) {
			if( ((s326*)it)->s348 != s346 && 
				( (s326*)it )->s348 != s347 )
			it->s223( s228 );
		}
		if( !s216 && s353.size() )
			s216 = s353.back()->s217();
		else if( s172 == s136 )
			s216 = s352[1]->s217();
	}
	else {
		s165* s436 = s168[0];
		s200 s438 = s436->s439();
		if( s438 == s203 ) {
			s230* ref = (s230*)s436;
			s211* pobj = (s211*)ref->s225();
			s438 = pobj->s439();
			s216 = ( (s293*)pobj )->s235( s172 );
		}
	}
	if( s216 ) {
		s449 = s194->s381( s217(), s228 );
		s352.insert( s352.begin(), s449 );

		if( s168.size() ) {
			s350 = (s212**)&s168[0];
			s464 = s168.size();
			s349 = &s352[0];
			s220 = s352.size();
		}
		else {
			s350 = NULL;
			s349 = NULL;
			s464 = s220 = 0;
		}
	}
}

void s326::s497( const vector<s212*>& l )
{

	s168.clear();

	s168.assign( l.begin(), l.end() );
}

void s326::s498( const wstring& s228 )
{
	for( size_t n = 0; n < s168.size(); ++n ) {
		s165* s436 = s168[n];
		if( s436->s439() == s205 ) {
			s211* s437 = s194->s381( ( (s290*)s168[n] )->Str(),
				( (s290*)s168[n] )->s473() );
			s168[n] = s437;
		}
		else if( s436->s439() == s206 ) {
			s428* cs = new s428( *(s428*)s436 );
			s436 = s358( cs, s194, s431, this, s192() );
			s436->s223( s228 );
			s168[n] = s436;
		}
		if( s168[n]->s439() != s203 ) {
			if( !s340 && s348 != s343 ) {
				s330[n] = s168[n]->s218();
				s351.s275( s422[n], (s211*)s330[n], s210::s61 );
			}
			s352.push_back( s168[n]->s218() );
		}
		else {
			if( !s340 && s348 != s343 ) {
				s330[n] = s168[n];
				s351.s275( s422[n], (s211*)( (s230*)s168[n] )->s320( NULL ), s210::s61 );
			}
			s352.push_back( NULL );
		}
		if( n < s330.size() && !( (s211*)s330[n] )->isTypeOf( (s211*)s494[n] ) )
			throw s5( L"wrong argument's type" );
	}
}

s211*
s326::s219( s211** s318, size_t s429 )
{
	s211* ret = NULL;

	size_t shift = 0;
	if( s348 == s346 || s348 == s344 || s348 == s343 )
		shift = 1;

	for( size_t n = 0; n < s464; ++n ) {
		if( *( s350 + n ) ) {
			s200 pc = ( ( s212* )*( s350 + n ) )->s439();
			if( pc == s203 )
				*( s349 + n + shift ) =
				(s211*)( ( s230* )*( s350 + n ) )->s320( (s166*)s318 + n );
			else if( pc == s204 )
				*( s349 + n + shift ) =
				(s211*)( ( s326* )*( s350 + n ) )->s219( s318, s429 );
		}
	}

	if( s340 ) {
		s340( s349, s220 );
		ret = s352[0];
	}
	else {
		for( size_t n = 0; n < s353.size(); ++n )
			ret = s353[n]->s219( s349, s220 );
	}
	return ret;
}

bool
s326::s242( const wstring& s298, s230* ref,
	const wstring& s228 ) const
{
	bool ret = false;
	wstring s172 = s298;
	size_t pl = s172.find( L"." );
	if( pl == string::npos ) { //a

		s230* ob = (s230*)s351.get( s172, false );
		if( !ob ) {

			if( s354 )
				ret = s354->s242( s172, ref, s228 );
			if( !ret ) {
				ret = s431->s242( s172, ref, s228 );
				if( !ret )
					ret = s194->s242( s172, ref, s228 );
			}
		}
		else {
			size_t off = s335( s172 );
			ref->s319( off, s168[off]->s218()->s225() );
			ref->s324( (s211*)this /*(s212**)&s168[off]*/ );

			ret = true;
		}
	}
	else {// a.y
		s172 = s298.substr( pl + 1 );
		wstring s448 = s298.substr( 0, pl );
		s165* ob = s351.get( s448, false );
		if( !ob ) {
			ret = s431->s242( s298, ref, s228 );
			if( !ret && s354 )
				ret = s354->s242( s172, ref, s228 );
		}
		else {
			size_t off = s335( s448 );
			ref->s319( off, NULL );
			ret = ob->s225()->s242( s172, ref, s228 );
			ref->s324( (s211*)this );
		}
	}

	return ret;
}

s165* 
s326::s337( size_t off )
{
	return s168[off];
}

s165* s326::s226() const
{
	s326* ret = new s326( *this );
	return ret;
}

s362::s362( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s139 )
{
	s172 = s139;

	size_t n = 0;
	for( ; n < l.size(); n += 2 ) {
		if( l[n]->s439() != s203 )
			break;
		s230* nam = (s230*)l[n];
		s211* typ = (s211*)l[n + 1];

		s351.s275( nam->ValName(), typ );
		s168.push_back( typ );
		s330.push_back( typ );
		s422.push_back( nam->ValName() );
	}

	s353.insert( s353.end(), l.begin() + n, l.end() );
	type = l.back()->s221();
}

void
s362::s223( const wstring& s228 )
{
	s165* s437 = NULL;
	s165* s436 = NULL;

	for( size_t n = 0; n < s168.size(); ++n ) {
		s437 = s168[n];
		if( s437->s439() == s205 ) {
			s436 = s194->s381( ( (s290*)s437 )->Str(),
				( (s290*)s437 )->s473() );
		}
		else if( s437->s439() == s206 ) {
			s428* cs = new s428( *(s428*)s437 );
			s436 = s358( (s428*)s437, s194, s431, this, s192() );
		}
		else
			s436 = s437;
		s436->s223( s228 );
		s168[n] = s436;
		s352.push_back( s168[n]->s218() );
	}
	for( size_t n = 0; n < s353.size(); ++n ) {
		s428* cs = new s428( *(s428*)s353[n] );
		s353[n] = s358( cs, s194, s431, this, s172 );
	}
	for( auto it : s353 )
		it->s223( s228 );

	s216 = s353.back()->s217();
}

s211*
s362::s219( s211** s318, size_t s429 )
{
	s211* ret = NULL;
	for( size_t n = 0; n < s168.size(); ++n ) {
		s168[n]->s219( NULL, 0 );
	}

	for( size_t n = 0; n < s353.size(); ++n )
		ret = s353[n]->s219( s318, s429 );
	return ret;
}

s165* 
s362::s447( const std::wstring& s298, s230* ref,
	const std::wstring& s228 ) const
{
	throw;
	s165* ret = NULL;
	wstring s172 = s298;
	size_t pl = s172.find( L"." );
	if( pl == string::npos ) { //a

		s230* ob = (s230*)s351.get( s172, false );
		if( !ob )
			s431->s242( s172, ref, s228 );
		else {
			size_t off = s335( s172 );
			ref->s319( off, s168[off]->s218()->s225() );
			ref->s324( ( s211* )this );

		}
	}
	else {// a.y
		s172 = s298.substr( pl + 1 );
		wstring s448 = s298.substr( 0, pl );
		s165* ob = s351.get( s448, false );
		if( !ob ) {
			s431->s242( s298, ref, s228 ); // throws if not successful

		}
		else {
			size_t off = s335( s448 );
			ref->s319( off, NULL );
			ob->s225()->s242( s172, ref, s228 );
			ref->s324( ( s211* )this );
		}
	}

	return ret;
}

s364::s364( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s149 )
{
	s168.assign( l.begin(), l.end() );
}

s211* s364::s219( s211** s168, size_t s220 )
{
	return NULL;
}

s365::s365( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s149 )
{
	s168.assign( l.begin(), l.end() );
	s216 = s304::s305();
}


s211* s365::s219( s211**, size_t )
{
	*( (s304*)s449 )->Ptr() = 1;
	for( size_t n = 0; n < s168.size(); ++n ) {
		if( !( bool )*( (s212*)( s168[n]->s219( 0, 0 ) ) ) ) {
			*( (s304*)s449 )->Ptr() = 0;
			return s449;
		}
	}

	return s449;
}

s366::s366( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s136 )
{
	s168.assign( l.begin(), l.end() );
}

void s366::s223( const wstring& s228 )
{
	s326::s223( s228 );
	s216 = s168[1]->s217();

}

s211* s366::s219( s211**, size_t )
{
	size_t npairs = s464 / 2;
	for( size_t n = 0; n < npairs; ++n ) {
		if( ( bool )*( (s212*)( s168[n]->s219( 0, 0 ) ) ) )
			return s168[2*n+1]->s219( 0, 0 );
	}
	if( s464 % 2 )
		return s168[s464-1]->s219( 0, 0 );
	return &nullobject;
}

s368::s368( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s160 )
{
	s172 = s160;
	s168.push_back( l[0] );
	s168.push_back( l[1] );

	s353.insert( s353.end(), l.begin() + 2, l.end() );

}

s368::s368( s162* s194 )
	: s326( s194, NULL, NULL, s343, s160 )
{
	s172 = s160;
}

void
s368::s223( const wstring& s228 )
{
	s165* s437 = NULL;
	s165* s436 = NULL;

	for( size_t n = 0; n < s168.size(); ++n ) {

		if( !n )
			condition = (s326*)s168[n];
		else
			step = (s326*)s168[n];
		s352.push_back( s168[n]->s218() );
	}
	/*for( size_t n = 0; n < s353.size(); ++n ) {
		s428* cs = new s428( *(s428*)s353[n] );
		s353[n] = s358( cs, s194, s431, this, s172 );
	}
	for( auto it : s353 )
		it->s223( this, s228 );*/

	s216 = s353.back()->s217();
}

s211*
s368::s219( s211** s318, size_t s429 )
{
	s211* ret = NULL;
	while( 1 ) {
		if( (bool)*condition->s219( 0, 0 ) == false )
			return ret; // s449;

		for( size_t n = 0; n < s353.size(); ++n )
			ret = s353[n]->s219( s318, s429 );

		step->s219( 0, 0 );
	}
	return ret;
}

s367::s367( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s157 )
{
	if( l.empty() )
		throw s5( L"Wrong numbers of arguments" );
	s168.assign( l.begin(), l.end() );
	s216 = s308::s305();
}

s367::s367( s162* s194 )
	: s326( s194, NULL, NULL, s343, s157 )
{
	s216 = s308::s305();
}

void s367::s223( const wstring& s228 )
{
	s326::s223( s228 );
	return;
	s165* s437 = NULL;
	s165* s436 = NULL;

	for( size_t n = 0; n < s168.size(); ++n ) {
		s437 = s168[n];
		if( s437->s439() == s205 ) {
			s436 = s194->s381( ( (s290*)s437 )->Str(),
				( (s290*)s437 )->s473() );
		}
		else if( s437->s439() == s206 )
			s436 = s358( (s428*)s437, s194, s431, this, s192() );
		else
			s436 = s437;
		s436->s223( s228 );
		s168[n] = s436;
		s352.push_back( s168[n]->s218() );
	}
	for( size_t n = 0; n < s353.size(); ++n ) {
		s428* cs = new s428( *(s428*)s353[n] );
		s353[n] = s358( cs, s194, s431, this, s172 );
	}
}

s211* s367::s219( s211** s434, size_t s220 )
{
	for( size_t n = 0; n < s168.size(); ++n ) {
		s211* res = s168[n]->s219( s434, s220 );
		wcout << res->to_wstring();
	}


	return &nullobject;
}

s211* s367::s226()const
{
	s211* ret = new s367( *this );
	return ret;
}

s369::s369( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l )
	: s326( s194, s443, s444, s343, s161 )
{
	if( l.size() != 1 )
		throw s5( s161 + L"(): wrong numbers of arguments" );
	s168.assign( l.begin(), l.end() );
	s216 = s308::s305();
}

s369::s369( s162* s194 )
	: s326( s194, NULL, NULL, s343, s161 )
{
	s216 = s308::s305();
}


s211* s369::s219( s211** s434, size_t s220 )
{
	s211* res = s168[0]->s219( s434, s220 );


	return &nullobject;
}

s211* s369::s226() const
{
	s211* ret = new s369( *this );
	return ret;
}

s211* s366::s226() const
{
	s211* ret = new s366( *this );
	return ret;
}

s211* s364::s226() const
{
	s211* ret = new s364( *this );
	return ret;
}

s211* s365::s226()const
{
	s211* ret = new s365( *this );
	return ret;
}

s211* s362::s226()const
{
	s211* ret = new s362( *this );
	return ret;
}

s211* s368::s226()const
{
	s211* ret = new s368( *this );
	return ret;
}
s162::s162()
	: s166( this, 0, s201 ), s372( s133, L"", L"global" ), s373( s96::s120 )
{	
}

s162::~s162()
{

}

void s162::s274()
{

}

void s162::s223()
{
	s451.s184();
}

void s377( const std::wstring& asmName, vector<wstring>& res ) 
{
	vector<s3<s57>> vobj;
	s80( asmName, vobj, s96::s454 );
	res.push_back( L"" );
	for( size_t n = 0; n < vobj.size(); n++ ) {
		wstring s402 = s163::s196( *vobj[n], s96::s124 );

		if( s402 == s96::s117 ) {
			const vector<s56>& s73 = (*vobj[n]).s69( s96::s126 )->s77();
			for( size_t n = 0; n < s73.size(); n++ )
				res.push_back( s73[n]->s79() );
			if( ( *vobj[n] ).s67( s96::s125 ) )
				res[0] = ( *vobj[n] ).s69( s96::s125 )->s79();
		}
	}
	if( res.size() == 1 )
		res.push_back( asmName );
}

void s378( s162* s194, s163& s372, s210& syms,
											s478& cl, s479& cnl, s480& al )
{
	s163::s169& s412 = s372.s187();
	s163::s169::iterator it = s412.begin();
	s455 am = s457;
	for( ; it != s412.end(); ++it ) {
		s200 pc = s208;
		if( it->first.find( L" " ) != string::npos ) {
			vector<wstring> vres;
			s25::s35( it->first, L" ", vres );
			if( vres.size() > 3 )
				throw s5( L"malformed block name: " + it->first );
			wstring s402 = vres[1];
			size_t idx = vres.size() - 2;
			if( vres[idx] == s96::s115 )
				pc = s207;
			else if( vres[idx] == s96::s116 )
				pc = s208;
			else
				throw s5( L"unknown object type: " + vres[0] );
			if( vres.size() == 3 ) {
				if( vres[0] == s96::s459 )
					am = s456;
				else if( vres[0] == s96::s461 )
					am = s458;
				else if( vres[0] != s96::s460 )
					throw s5( L"invalid access mode name: " + it->first );
				s402 = vres[2];
			}
			it->second->s188( s402 );
		}


		s166* ob = new s166( *it->second, s194, pc, s194, am, cl, cnl, al );

		syms.s275( ob->s193(), ob );
	}
}

void s162::s504( const wstring& fname_ )
{
	vector<wstring> srcs;
	s163 _ast;
	wstring s396 = fname_;
	wstring s519;
	int isf = s17::s21( fname_ );
	if( isf == 2 ) {
		s25::s39( s396 );
		s396 += s96::s121;
		s519 = fname_;
	}
	else if( isf == 1 )
		s519 = s396.substr( 0, s396.find_last_of( L"\\/" ) );
	else
		throw s5( L"cannot find file or directory: " + fname_,
		(uint32_t)s5::s6::s12 );

	s25::s39( s519 );

	s377( s396, srcs );
	vector<s3<s57>> vobj;
	for( size_t n = 1; n < srcs.size(); n++ ) {
		vobj.clear();
		wstring s518 = srcs[n];
		if( s518[0] != L'/' && s518[1] != L':' )
			s518 = s519 + s518;

		s80( s518, vobj, s96::s454 );
		for( size_t m = 0; m < vobj.size(); m++ )
			if( s163::s189( *vobj[m], s96::s118 ) == s96::s118 )
				_ast.s183( *vobj[m] );
	}
	s503( _ast );
	s373 = srcs[0];
}

void s162::s503( s163& _ast )
{
	s210 syms;

	s163::s169& s412 = _ast.s187();
	s163::s169::iterator it = s412.begin();
	s480 m1;
	s479 s512;
	s478 classList;
	vector<wstring> s513;


	s378( this, _ast, syms, classList, s512, m1 );

	size_t s511 = s512.size();
	while( s513.size() < s511 ) {
		size_t s514 = s512.size();
		for( size_t n = 0; n < s512.size(); ++n ) {
			if( s512[n].second.empty() ||
				std::find( s513.begin(), s513.end(), s512[n].second )
				!= s513.end() ) {
				s513.push_back( s512[n].first );
				s512.erase( s512.begin() + n-- );
				continue;
			}
		}
		if( s514 == s512.size() )
			throw s5( L"unresolved parent class: " + s512[0].second );
	}

	for( size_t n = 0; n < s513.size(); ++n )
		if( s451.s67( s513[n] ) )
			throw s5( L"class name already exists: " + s513[n] );

	for( size_t n = 0; n < s513.size(); ++n )
		s451.s254( classList[s513[n]] );

	s328.s183( syms );


	for( size_t n = 0; n < s513.size(); ++n ) {
		s166* ob = (s166*)s451.get( s513[n], false );
		if( ob )
			ob->s65( *m1[s513[n]] );
	}

	for( it = m1.begin(); it != m1.end(); ++it ) {
		s166* ob = (s166*)syms.get( it->first, false );
		if( ob && ob->s439() == s208 )
			ob->s65( *it->second );
	}


	syms.s223( s193(), false );
}

void s162::s506( s57& obj )
{
	s163 _ast;
	_ast.s183( obj );

	s503( _ast );
}


s211* s162::run()
{
	if( s373.empty() )
		throw s5( L"unknown entry point ", (uint32_t)s5::s6::s9 );

	vector<wstring> l;
	return callFunc( s373, l );
}

s211* s162::callFunc( const wstring& s, vector<wstring>& l )
{
	wstring  s516, s515;
	s517( s, s515, s516 );
	s166* mod = (s166*)s328.get( s515 );
	s326* s374 = (s326*)mod->s338( s516, L"::" );
	vector<s212*> s168;
	for( size_t n = 0; n < l.size(); n++ ) {
		s163 s467( l[n] );
		s428* cs = s326::s357( s467, this, mod, mod->s192() );
		s168.push_back( cs );
	}
	s374->s497( s168 );
	s212* pr = s374->s218();
	if( !pr )
		s374->s223( L"::" );
	else
		s374->s498( L"::" );

	return s374->s219();
}

s326* s162::getProc( const std::wstring& s )
{
	wstring  s516, s515;
	s517( s, s515, s516 );
	s166* mod = (s166*)s328.get( s515 );
	s326* s374 = (s326*)mod->s338( s516, L"::" );
	s212* pr = s374->s218();
	if( !pr )
		s374->s223( mod->s192() );
	return s374;
}

void s162::s375( const std::wstring& fname_ )
{
	wstring s396 = fname_;
	wstring s519;
	int isf = s17::s21( fname_ );
	if( isf == 2 ) {
		s25::s39( s396 );
		s396 += s96::s121;
		s519 = fname_;
	}
	else if( isf == 1 )
		s519 = s396.substr( 0, s396.find_last_of( L"\\/" ) );
	else
		throw s5( L"cannot find file or directory: " + fname_, 
			(uint32_t)s5::s6::s12 );

	s25::s39( s519 );

	vector<s3<s57>> vobj;
	s80( s396, vobj, s96::s454 );
	for( size_t n = 0; n < vobj.size(); n++ ) {
		wstring s402 = s163::s196( *vobj[n], s96::s124 );

		if( s402 == s96::s117 ) {
			s376( *vobj[n] );
			if( s371.empty() )
				s371.push_back( s396 );
		}
	}
	if( s371.empty() )
		s371.push_back( s396 );
	for( size_t n = 0; n < s371.size(); n++ ) {
		vobj.clear();
		wstring s518 = s371[n];
		if( s518[0] != L'/' && s518[1] != L':' )
			s518 = s519 + s518;
		s80( s518, vobj, s96::s454 );
		for( size_t m = 0; m < vobj.size(); m++ )
			if( s163::s189( *vobj[m], s96::s118 ) == s96::s118 )
				s372.s183( *vobj[m] );
	}
}

void s162::s376( const s44::s57& obj )
{
	const vector<s56>& s73 = obj.s69( s96::s126 )->s77();
	for( size_t n = 0; n < s73.size(); n++ )
		s371.push_back( s73[n]->s79() );
	if( obj.s67( s96::s125 ) )
		s373 = obj.s69( s96::s125 )->s79();
	return;
	vector<s56> vvals;
	obj.s68( s96::s126, vvals );
	for( size_t n = 0; n < vvals.size(); n++ )
		s371.push_back( vvals[n]->s79() );
}

void s162::s254( s211* p )
{
	s451.s254( p );
}

void s162::s384( const wstring& nam, const wstring& s228,
	vector<wstring>& res )
{
	size_t pl = nam.rfind( L"::" );
	s166* mod = NULL;
	bool bCl = false;
	if( nam.find( L"()" ) == nam.size() - 2 )
		bCl = true;
	
	if( pl != string::npos ) {

		wstring s452 = nam.substr( 0, pl );
		wstring s411 = nam.substr( pl + 2 );
		if( bCl ) {
			s411 = s411.substr( 0, s411.size() - 2 );

		}

			mod = (s166*)s328.get( s452 );

		if( !mod->s339( s411, s228 ) )
			throw s5( L"Symbol '" + nam + L"' cannot be imported by '" +
				s228 + L"'");

		res.push_back( s411 );
	}
	else {

		if( bCl ) {
			wstring s411 = nam.substr( 0, nam.size() - 2 );
			s451.get( s411 );
			res.push_back( s411 );
		}
		else {
			mod = (s166*)s328.get( nam );
			mod->s426( s228, res );
		}
	}
}


s165* s162::s380( const wstring& val, const wstring& s228 )
{

	return new s290( this, val, s228 );
}

s211* s162::s381( size_t type, const wstring& s228 )
{
	return s451.s257( this, type, s228 );
}

s211* s162::s381( const std::wstring& val, const wstring& s228 )
{
	return s451.s257( this, val, s228 );
}

s211* s162::s381( const s290& ds )
{
	return s451.s257( this, ds.Str(), ds.s473() );
}

s211* s162::s381( s163& s372, const wstring& s228 )
{
	return s451.s257( this, s372, s228 );
}

s326* s162::s379( const std::wstring& s453, 
														std::vector<s212*>& s434, s166* s431, s166* s432 )
{
	s326* ret = NULL;

	if( s453 == s136 )
		ret = new s366( this, s431, s432, s434 );
	else if( s453 == s139 )
		ret = new s362( this, s431, s432, s434 );
	else if( s453 == s149 )
		ret = new s365( this, s431, s432, s434 );
	else if( s453 == s160 )
		ret = new s368( this, s431, s432, s434 );
	else if( s453 == s150 )
		ret = new s364( this, s431, s432, s434 );
	else if( s453 == s157 )
		ret = new s367( this, s431, s432, s434 );
	else if( s453 == s161 )
		ret = new s369( this, s431, s432, s434 );
	else if( s453 == s158 )

		throw s5( L"unexpected lambda" );

	return ret;
}

s165*
s162::s386( const wstring& nam, const wstring& s222, bool thr /*= true*/ )
{
	size_t pl = nam.rfind( L"::" );
	s165* ret = NULL;
	if( pl == string::npos ) {
		ret = s451.get( nam, thr );
		if( !ret ) {
			s166* exp = ( (s166*)s451.get( s96::s119, false ) );
			if( exp )
				ret = exp->s338( nam, s222, thr );
		}
		return ret;
	}

	wstring s452 = nam.substr( 0, pl );
	wstring s411 = nam.substr( pl + 2 );
	return ( (s166*)s451.get( s452 ) )->s338( s411, s222 );	
}

void* s162::getExportVariable( const std::wstring& s172 )
{
	s293* ret;
	s166* exp = ( (s166*)s328.get( s96::s119, false ) );
	if( exp )
		ret = (s293*)exp->s338( s172, L"", true );
	else
		throw s5( L"no such export variable: " + s172 );
	void * pv = ret->addr();

	return pv;
}
map<int, s162*> handles;

HPROG createAssembly()
{
	static int cnt = 0;
	s162* p = new s162();
	handles[++cnt] = p;
	s211::s233( p );
	p->s223();

	return cnt;
}

void loadProgram( HPROG handle, const wstring& s396 )
{
	s162* p = handles[handle];
	p->s504( s396 );
}

TDType* getProc( HPROG handle, const std::wstring& s359 )
{
	s162* p = handles[handle];
	return (TDType*)p->getProc( s359 );
}

void s506( HPROG handle, s44::s57& obj )
{
	s162* p = handles[handle];
	p->s506( obj );
}

s211* run( HPROG handle )
{
	return handles[handle]->run();
}

void* callFunc( HPROG handle, const std::wstring& sf, vector<wstring>& l )
{
	s162* p = handles[handle];
	return p->callFunc( sf, l );
}

void evaluateExpression( const std::wstring& sf )
{

}

void* getExportVariable( HPROG handle, const std::wstring& s172 )
{
	s162* p = handles[handle];
	return p->getExportVariable( s172 );
}

void* execute( TDType* p )
{
	return ( (s293*)p->s219() )->addr();
}

void importSymbol( HPROG handle, const std::wstring& modName, const std::wstring& symName,
										const std::wstring& s228 )
{
	s162* p = handles[handle];
	s166* ob = (s166*) p->s338( modName, s228 );
	ob->s484( symName, s228 );
}

void importFile( HPROG handle, const std::wstring& modName, const std::wstring& s390,
									const std::wstring& s228 )
{
	vector<s1::s3<s44::s57>> vobj;
	s44::s80( s390, vobj, s96::s454 );
	
	s162* p = handles[handle];
	p->s504( s390 );
	s166* ob = (s166*)p->s338( modName, s228 );
	for( size_t n = 0; n < vobj.size(); n++ ) {
		wstring s402 = s163::s196( *vobj[n], s96::s124 );
		ob->s484( s402, s228 );
	}
}

HPROG initShell( const std::wstring& modName, const std::wstring& modText )
{
	s1::s3<s44::s57> ob = new s44::s57();
	size_t pos = 0;
	s81( modText, pos, ob );
	ob->s70( L"name", new s44::s55( modName ) );

	HPROG prog = createAssembly();
	s506( prog, *ob );
	return prog;
}



} // namespace transd
