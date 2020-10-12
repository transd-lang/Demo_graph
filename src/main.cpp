/*
Copyright (c) 2020 Albert Berger

This file is licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may view the text of the License in the 'LICENSE' file
shipped with the distribution of Demo_graph.

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif
#include <iostream>

#include "transd.hpp"

//TODO: specify your working directory where the Transd files are located and where the output file
// will be placed
std::string workingDir = "Y:\\your\\working\\directory\\"


using namespace std;


#ifdef _WIN32
int wmain( int argc, wchar_t* argv[] )
#else
int main( int argc, char* argv[] )
#endif
{
#ifdef WIN32
	_setmode( _fileno( stdout ), _O_U16TEXT );
	_setmode( _fileno( stdin ), _O_U16TEXT );
#endif

	try {
		transd::HPROG prog = transd::createAssembly();

		transd::loadProgram( prog, U16(workingDir) );
		wstring sy = L"height", sx = L"width", sv = L"v";
		int width = *(int*)transd::getExportVariable( prog, sx );
		int height = *(int*)transd::getExportVariable( prog, sy );
		int sz = width *height * 3;

		transd::run( prog );
		vector<transd::TDType*>& rv = *( vector<transd::TDType*>* )transd::getExportVariable( prog, sv );
		size_t sz_ = rv.size();
		vector<uint8_t> bytes( sz_, 0 );
		for( size_t n = 0; n < sz_; ++n )
			bytes[n] = ( int )*(transd::TDDouble*)rv[n];


		string outFileName = workingDir + "graph.ppm";
		FILE* f = fopen( outFileName.c_str(), "wb" );
		char buf[40];
		sprintf( buf, "P6 %d %d 255\n", width, height );
		fwrite( buf, strlen( buf ), 1, f );
		unsigned char* p = (uint8_t*)&bytes[0];
		for( int i = 0; i <= sz - width * 3; i += width * 3 ) {
			fwrite( p + i, width * 3, 1, f );
		}
		fclose( f );

		wcout << L"OK." << endl;
	}
	catch( transd::TDException& e ) {
		wcout << L"TDException has been raised in main(): \n\n" << e.Msg() << endl;
	}
	catch( std::exception& e ) {
		wcout << L"std::exception has been raised in main(): \n\n" << U16( e.what() ) << endl;
	}

	return 0;
}
