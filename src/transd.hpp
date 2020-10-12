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
	transd.hpp
	----------
	The minimized distributed header file for C++ binding embeddable
	library of Transd programming language.

	Transd Project Website: https://github.com/transd-lang
*/
#include <vector>
#include <map>
#include <list>
#include <codecvt>
#include <set>



#if _WIN32 || _WIN64
#if defined(_WIN64)
#define OS_64
#else
#define OS_32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define OS_64
#else
#define OS_32
#endif
#endif

#ifndef WIN32
#define _W64
#define __int64 int64_t
#endif

namespace s1 
{
#ifdef OS_64
typedef std::int64_t				INT_PTR, *PINT_PTR;
typedef std::uint64_t				UINT_PTR, *PUINT_PTR;
typedef long								LONG_PTR, *PLONG_PTR;
typedef unsigned long				ULONG_PTR, *PULONG_PTR;
typedef long long						LONGLONG;
typedef unsigned long long	ULONGLONG;
typedef ULONG_PTR		    		DWORD_PTR, *PDWORD_PTR;
#else
typedef _W64 int						INT_PTR, *PINT_PTR;
typedef _W64 unsigned int		UINT_PTR, *PUINT_PTR;
typedef _W64 long						LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long	ULONG_PTR, *PULONG_PTR;
typedef __int64							LONGLONG;

typedef ULONG_PTR		    		DWORD_PTR, *PDWORD_PTR;
#endif

/*
*	Checking convertibility
*/

template<class T, class U>
class Conversion
{
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test( U );
	static Big Test( ... );
	static T MakeT();
public:
	enum
	{
		exists =
		sizeof( Test( MakeT() ) ) == sizeof( Small )
	};
	enum { sameType = false };
};

template <class T>
class Conversion<T, T>
{
public:
	enum { exists = 1, sameType = 1 };
};

#define SUPERSUBCLASS(T, U) \
	(Conversion<const U*, const T*>::exists && \
	!Conversion<const T*, const void*>::sameType)

template <class F, class T>
T* checked_cast( F* p )
{
	if( SUPERSUBCLASS( T, F ) )
		return p;

	return nullptr;
}

class s2
{
	int refs;
public:
	s2() : refs(0){}

	virtual ~s2()
	{
	}

	void upcount()
	{
		refs++;
	}

	void downcount()
	{
		if( --refs == 0 )
			deallocate();
	}

	int getNumRef()
	{
		return refs;
	}

	virtual void deallocate()
	{
		delete this;
	}
};

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4522 )
#endif

template <class C>
class s3
{
	C* p;
public:
	s3( C* p_ ) : p(p_)
	{
		if(p)
			p->upcount();
	}

	s3(C& p_) 
	{
		p = &p_;
		if(p)
			p->upcount();
	}

	s3()
	{
		p = NULL;
	}

	s3(const s3<C> &pn)
	{
		p = pn.p;
		if(p)
			pn.p->upcount();
	}

	template<class D> 
	s3( s3<D> &pn )
	{
		p = nullptr;
		if ( SUPERSUBCLASS( C, D ) ) {
			p = (C*)pn;
			if(p)
				p->upcount();
		}
	}

	~s3()
	{
		detach();
	}


	operator C*() { return p; }

	C& operator*() { return *p; }

	const C& operator*()const { return *p; }

	C* operator->() { return p; }

	const C* operator->()const { return p; }

	s3& operator=( s3<C> &p_ ) { return operator=(( C* ) p_); }

	s3& operator=( const s3<C> &p_ ) { return operator=( (s3<C>& ) p_ ); }

	s3& operator=( C* p_ )
	{
		if(p){
			p->downcount();
		}

		p = p_;
		if(p)
			p->upcount();
		return *this;
	}


	template<class B>
	operator B*()
	{
		return checked_cast<C, B>( p );
	}

	template<class D>
	s3& operator=( const s3<D> &p_ )
	{
		if ( SUPERSUBCLASS( C, D ) )
			return operator=( ( C* ) p_);
	}

	template<class ToType>
	ToType scast() const
	{
		return static_cast<ToType>( p );
	}
	
	template<class ToType>
	ToType dcast() const
	{
		return dynamic_cast<ToType>( p );
	}

	bool IsNull()const { return p == nullptr; }

	bool IsValid()const { return p != nullptr; }

	void detach()
	{
		if(p){
			p->downcount();
			p = nullptr;
		}
	}

	bool operator ==(const s3<C>&ptr){return ((INT_PTR)ptr.p == (INT_PTR)p);}
	bool operator !=(const s3<C>&ptr){return ((INT_PTR)ptr.p != (INT_PTR)p);}
	bool operator ==(const C& val){return ( *p == val );}
	bool operator !=(const C& val){return ( *p != val );}
};

#if defined (_MSC_VER)
#pragma warning( pop )
#endif

} // namespace s1

namespace s4 {

void kexceptInit();

class s5
								: public std::exception
{
	static uint32_t s16;

	std::list<std::wstring> s15;

	uint32_t id; 
public:
	enum class s6 : uint32_t
	{
		s7 = 0,
		s8,
		s9,
		s10,
		s11,
		s12,
		s13,
		s14
	};

	s5( const std::wstring& s, uint32_t n = (uint32_t)s6::s8 )
		: id( n )
	{
		s15.push_back( s );
	}

	s5( uint32_t n )
		: id( n )
	{
		s15.push_back( L"Exception" );
	}

	static void s233()
	{
		s16 = (uint32_t)s6::s14 + 1;
	}

	static uint32_t nextId()
	{
		return s16++;
	}

	const wchar_t* Msg() const
	{
		if( s15.size() )
			return s15.back().c_str();
		return NULL;
	}

	const char* what() const noexcept
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes( s15.back() ).c_str();
	}
	uint32_t which() const { return id; }

	void addInfo( const std::wstring& s )
	{
		s15.push_back( s );
	}

};

} // namespace s4
namespace s17 {

bool s18( const std::wstring& _dir, const std::wstring& s388, std::wstring& res );
void s19( const std::wstring& _dir, const std::wstring& s388, std::vector<std::wstring>& files,
	std::vector<std::wstring>& dirs );
time_t s20( const std::wstring& f );
uint8_t s21( const std::wstring& f );
bool s22( const std::wstring& s387 );
bool s23( const std::wstring& s387 );
std::wstring s24();


} // namespace s17
namespace s25 
{
extern std::wstring s26;
extern std::string s27;
extern std::wstring s28;
enum s6 { s12 };

std::wstring s29( const std::wstring& l, const std::wstring& s, const std::wstring& r );
std::wstring s30( const std::wstring& s, const std::wstring& c );
bool s31( const std::wstring& s, const std::wstring& ss );
void s32( const std::wstring& s, size_t s392, wchar_t left, wchar_t right, 
		size_t& start, size_t& end);
void s33( const std::wstring& s, size_t s392, wchar_t c, size_t& start, 
		size_t& end );
int s34( const std::wstring& sf, wchar_t s393, size_t& pos );
void s35( const std::wstring& s, const std::wstring& s393, std::vector<std::wstring>& res );
std::wstring s463( const std::wstring& sf, size_t& pos );
void s36( const std::wstring& sf, const std::wstring& s393, 
		std::pair<std::wstring, std::wstring>& pr, size_t& pos );
void s37( const std::wstring& sf, const std::wstring& s393,
		std::vector<std::pair<std::wstring, std::wstring>>& v );
std::wstring s38( const std::wstring& sf );
void s39( std::wstring& dir );
bool s40( const wchar_t *str, long *val, wchar_t **end = NULL );
bool s41( const wchar_t *str, double *val, wchar_t **end = NULL );

std::string s29( const std::string& l, const std::string& s, const std::string& r );
std::string s30( const std::string& s, const std::string& c );
void s32( const std::string& s, size_t s392, char left, char right, 
						size_t& start, size_t& end);
int s34( const std::string& sf, char s393, size_t& pos );
void s35( const std::string& s, const std::string& s393, std::vector<std::string>& res );
void s37( const std::string& sf, std::vector<std::pair<std::string, std::string>>& v );
std::string s38( const std::string& sf );

struct fill_n_ch {
	size_t len;
	wchar_t ch;
	typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t> > wostream;

	fill_n_ch( wchar_t c, size_t l ): len(l), ch(c) {}

	friend wostream& operator<< ( wostream& os, fill_n_ch fb )
	{
		for ( size_t i = 0; i < fb.len; i++ )
			os << fb.ch;

		return os;
	}
};

#define FILL(x) fill_n_ch( L' ', x )

template <typename UINT> 
constexpr UINT rol(UINT val, size_t n) {
    static_assert(std::is_unsigned<UINT>::value,
                  "Rotate Left only makes sense for unsigned types");
    return (val << n) | (val >> (sizeof(UINT)*8-n));
}

void s42( const std::wstring& msg );



} // namespace s25
namespace s43 {
extern std::locale locloc;
extern std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
}

#define U8(x) s43::conv.to_bytes( x )
#define U16(x) s43::conv.from_bytes( x )
#ifdef __LINUX__
#define KCHAR char
#define TOFS(x) s43::conv.to_bytes(x)
#define FROMTERM(x) s43::conv.from_bytes(x)
#else
#define KCHAR wchar_t
#define TOFS(x) x
#define FROMTERM(x) x
#endif
namespace s44
{

enum s45{ s46, s47, s48, s53, 
	s49, s50, s54, s51, s52 };


class s55;

typedef s1::s3<s55> s56;

class s57
							: public s1::s2
{
	std::multimap<std::wstring, s56> s58;

public:
	enum s59 { s60, s61, s62 };
	enum s6 { s7, s63 };
	const static s55 s64; // similar to string::npos
	s57(){}

	void s65( const std::wstring& sf );
	void s66( std::wostream* pd, bool strict=false ) const;

	bool s67( const std::wstring& key ) const;
	void s68( const std::wstring& key, std::vector<s56>& res ) const;
	const s56 s69( const std::wstring& key ) const;
	void s70( const std::wstring& nam, s56 val, s59 ap=s60 );
	void s71( std::vector<std::pair<std::wstring, s56>>& res ) const;

	bool operator==( const s57& r )  const;

	void s72( std::wostream* pd, int ind ) const;
};

bool operator==( const std::vector<s1::s3<s55>>& l,
	const std::vector<s1::s3<s55>>& r );

class s55
							: public s1::s2
{
	s45 var;
	s57 obj;
	std::wstring str;
	std::vector<s56> s73;
	double s74;
	bool b;
public:
	s55():var(s46){}
	s55( const std::wstring& s ):var(s48),str(s){}
	s55( const std::vector<s56>& a ): var(s49), s73(a){}

	s55( const std::wstring& sf, size_t& pos );

	s45 Var() const { return var; }

	bool s75() const { return var == s46; }

	const s57& s76() const
	{
		if( var == s47 )
			return obj;
		throw std::runtime_error( "wrong variant" );
	}

	const std::vector<s56>& s77() const
	{
		if( var == s49 )
			return s73;
		throw std::runtime_error( "wrong variant" );
	}

	double s78() const
	{
		if( var == s50 )
			return s74;
		throw std::runtime_error( "wrong variant" );
	}

	int s34() const
	{
		if( var == s54 )
			return (int)s74;
		throw std::runtime_error( "wrong variant" );
	}

	const std::wstring& s79() const
	{
		if( var == s48 || var == s53 )
			return str;
		throw std::runtime_error( "wrong variant" );
	}

	bool operator==( const s55& r ) const
	{
		return var == r.var && obj == r.obj && str == r.str && s73 == r.s73 && b == r.b && s74 == r.s74;
	}

	bool is( const s55& r ) const
	{
		return this == &r;
	}

	void s72( std::wostream* pd, int ind ) const;
	void s66( std::wostream* pd, bool strict=false ) const;
};


void s80( const std::wstring& s399, 
	std::vector<s1::s3<s57>>& res, 
	const std::wstring& sheb = std::wstring(L"") );
void s81( const std::wstring& sf, size_t& pos, s1::s3<s57> res );
void s82( const std::wstring& s399, 
	const std::vector<s1::s3<s57>>& objs );
} // namespace s44

#define DEFCONST extern const std::wstring

namespace transd {
namespace s96 {

DEFCONST s97;
DEFCONST s98;
DEFCONST s99;
DEFCONST s100;
DEFCONST s101;
DEFCONST s102;
DEFCONST s103;
DEFCONST s104;
DEFCONST s105;
DEFCONST s106;
DEFCONST s107;
DEFCONST s108;
DEFCONST s109;


DEFCONST s110;
DEFCONST s111;
DEFCONST s112;
DEFCONST s113;
DEFCONST s114;

DEFCONST s459;
DEFCONST s460;
DEFCONST s461;


DEFCONST s115;
DEFCONST s116;


DEFCONST s117;
DEFCONST s118;
DEFCONST s119;

DEFCONST s120;
DEFCONST s121;

DEFCONST s122;
DEFCONST s123;
DEFCONST s124;
DEFCONST s125;
DEFCONST s126;
DEFCONST s127;
DEFCONST s129;


#define INDSTEP 3
#define VECELSEP L','

DEFCONST s454;

} // namespace s96
enum s130 { s131, s132, s133, s134 };
extern std::wstring s135[];
extern const std::wstring s136, s137, s138,
s139,  s140,     s141,    s142,      s143,
s144,  s145, s146, s147,
s148, s149,     s150,     s151,
s152, s153,     s154,   s155,
s156,   s157, s158, s159,
s160, s161;

class s162;
class s163;
class s164;
class s165;
class s166;
typedef s164* ( *s167 )( const std::wstring& s172, const std::vector<s165*>& s168 );


class s163
{
public:
	typedef std::multimap<std::wstring, s163*> s169;
private:
	static std::map<std::wstring, s167> s170;

	std::wstring s171;
	std::wstring s172;
	s130 s173;
	std::wstring s174;
	s169 s175;

	void s176( const std::wstring& sf );
	void s177( const s44::s57& s47 );
	void s178( const std::wstring& sf );
	void s179( const std::wstring& sf );
	void s180( const std::wstring& type, const s44::s55& obj );

	static std::wstring s181( const std::wstring& sf, size_t &pos );

public:
	enum s6 { s7, s63 };
	s163(){ }
	s163( s130 at, const std::wstring& qpre, const std::wstring& nam )
		: s173( at ), s171( qpre ), s172( nam ) { }
	s163( const std::wstring& nam, const std::wstring& val )
		: s173( s131 ), s172( nam ), s174( val ) { }
	s163( const std::wstring& type );
	s163( const std::wstring& nam, const s44::s55& s190 );
	s163( const s44::s57& s47 );

	s130 s191() const { return s173; }
	std::wstring s192() const;
	const std::wstring& s193() const { return s172; }

	static void s182( const std::wstring& sf, size_t& pos, std::vector<std::wstring>& res );
	void s183( s44::s57& s47 );
	void s184( s162* s194, size_t s195 = 0 );
	const std::wstring& s185() const { return s174; }
	const s163& s186( const std::wstring& s172 ) const;
	s169& s187() { return s175; }
	void s188( const std::wstring& s ) { s172 = s; }


	static std::wstring s189( const s44::s57& obj,
		const std::wstring& default = std::wstring( L"" ) );
	static std::wstring s196( const s44::s57& obj,
		const std::wstring& s197,
		const std::wstring& default = std::wstring( L"" ) );

	void s198( std::wostream* pd, int ind = 0 ) const;
};
extern const std::wstring s199;

enum s200 { s201, s202, s203, s204, s205, 
							s206, s207, s208, s209 };
class s162;
class s210;
class s211;
class s166;
class s165;
typedef s165 s212;
typedef s165 TDType;
typedef s1::s3<s212> PProj;

class s165
							: public s1::s2
{
	friend class s213;
	static std::map<const s212*, bool> s214;
protected:
	s200 s215;
	size_t s216;
	s162* s194;
public:
	s165() : s215( s201 ), s216(0){ }
	s165( s200 pt_ ) : s215( pt_ ), s216(0) { }
	s165( s162* s468, s200 pt_ ) : s194( s468 ), s215( pt_ ),
		s216(0) { }
	virtual ~s165();
	s200 s439() const { return s215; } // s165 category
	virtual size_t s217() const { return s216; }
	virtual s211* s218();
	virtual s211* s219( s211** s168 = NULL, size_t s220 = 0 );
	virtual size_t s221() const { return 0; } // s185/result type
	virtual bool isTypeOf( const s211* classObj ) const;
	virtual void s223( const std::wstring& s222 );
	bool s224() const;
	virtual const s211* s225() const;
	virtual s165* s226() const;

	static s165* s227( s163& s372, s162* s194, s166* obj, const std::wstring& s228 );

	virtual operator bool() const;
	virtual operator int() const;
	virtual operator double() const;
	virtual std::wstring to_wstring() const;

/*	static void s182( const std::wstring& sf, size_t& pos,
		std::vector<std::wstring>& res );

	*/
	virtual void s198( std::wostream* pd, int ind = 0 ) const = 0;
};

typedef std::multimap<std::wstring, s212*> s229;
s212* s509( s229& m, const std::wstring& key );
void s517( const std::wstring& s, std::wstring& modName, std::wstring& s359 );

class s211;
class s230;

class s211
							: public s165
{
	friend class s213;

protected:
	std::wstring s231; // DEBUG

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) = 0;

public:
	s211( s162* s194, s200 pc ) : s165( s194, pc ) { }
	virtual ~s211();



	static void s233( s162* s194 );
	virtual void s234() = 0;
	virtual size_t s235( const std::wstring& s236 ) const = 0;
	virtual void s237( size_t type ) = 0;
	virtual const std::wstring& s238() const = 0;
	virtual s211* s239() const = 0;
	virtual void s240( s212* p ) const = 0;
	virtual const s211* s225() const override = 0;
	void s241( const std::wstring& s ) { s231 = s; }


	virtual s211* s219( s211** s168, size_t s220 ) override { return this; }
	virtual s211* s218() override;
	virtual bool s242( const std::wstring& s172, s230* res,
		const std::wstring& s228 ) const
	{
		return false;
	};
};

typedef s1::s3<s211> PVal;
enum s455 { s456, s457, s458 };
enum s248 { s249, s250, s251 };

class s210;

class s213
{
	typedef std::map<std::wstring, s211*> s252;
	s252 m;
	std::vector<std::wstring> v;
public:
	s213() { }

	void s274() { m.clear(); v.clear();  }
	void s184();
	void s253( const s210& st );
	void s254( s211* p );
	void s255( s211* p, const std::wstring& s256 );
	s211* s257( s162* s194, const s44::s55& val, const std::wstring& s256 );
	s211* s257( s162* s194, s163& s372, const std::wstring& s256 );
	s211* s257( s162* s194, size_t type, const std::wstring& s256 );
	s211* s257( s162* s194, const std::wstring& val, const std::wstring& s256 );
	s211* get( const std::wstring& nam, bool thr = true ) const;
	bool s67( const std::wstring& nam ) const;

	void s198( std::wostream* pd, int ind = 0 ) const;
};

class s258
								: public s1::s2
{
	std::vector<std::wstring> patterns;
	s248 pv;
public:
	s258() : pv( s249 ) { }
	s258( s248 pv, const std::vector<std::wstring>& s259 );
	s258( const std::wstring& ag, const std::wstring& sf );
	s248 s260( const std::wstring& s222 ) const;

	static std::vector<std::wstring> s261;
};

class s262
								: public s1::s2
{
	typedef std::map<std::wstring, s258*> s263;
	s263 acls;
public:
	s262( ) { }
	void s264( s248 pv, s163* );
	void s265( const std::wstring& s, s258& pl );
	void s266( const std::wstring& s, const std::wstring& s388 );
	s248 s267( const std::wstring& mem, const std::wstring& s222 );
	void s268( const std::wstring& s228,
		std::vector<std::wstring>& res );
};

class s210
{
public:
	enum s59 { s60, s269, s61, s62 };
private:
	typedef std::map<std::wstring, s211*> s270;
	s270 s410;
	s455 s462;

	s262* s271;
	bool s272( const std::wstring& nam, s59 ap = s60 );
public:
	s210( ): s462( s457 ), s271( NULL ) {}
	s210( s455 am ) : s462( am ), s271( NULL ) {}
	s210( const s210& s466 );
	void s223( const std::wstring& s222, bool s273 = true );
	void s274();
	void s275( const std::wstring& nam, s211* s411, s59 ap = s60 );
	void s275( const std::wstring& nam, s44::s56 val, s59 ap = s60 );
	void s276( const std::wstring& nam );
	void s277( const s44::s57& obj, s59 ap = s60 );
	void s183( const s210& t, s59 ap = s60 );
	bool s67( const std::wstring& nam ) const;
	s211* get( const std::wstring& nam, bool thr = true ) const;
	void s278( std::vector<std::wstring>& res ) const;
	void s279( std::vector<std::wstring>& res ) const;
	void s280( std::vector<std::wstring>& res ) const;
	void s281( const s210& t, std::vector<std::wstring>& res ) const;
	void copy( s210& t, s166* s431, s166* s354, s59 ap = s60 ) const;
	void s282( s210& t ) const;
	void s283( s248 pv, s163* block );
	void s268( const std::wstring& s228,
		std::vector<std::wstring>& res );
	bool s285( const std::wstring& s172, const std::wstring& s228 ) const;

	void s198( std::wostream* pd, int ind = 0 ) const;
};

class s286
{
	typedef std::map<std::wstring, s211*> s270;
	s270 s410;
	std::vector<std::wstring> s476;
public:
	s286();
	void s287( const std::wstring& s );
	void s223( s162* s194, const std::wstring& s420 );
	void s484( const std::wstring& nam, s162* s194, const std::wstring& s420 );
	bool s242( const std::wstring& s411, s230* ref );
	void copy( s286& t ) const;

};

extern s1::s3<s262> s288;
class s289;
class s162;
typedef void ( *s294 )( s211** s168, size_t n );

class s290
							: public s165
{
	std::wstring str;
	std::wstring s228;

public:
	s290( s162* s194, const std::wstring& s, const std::wstring& s291 )
		: s165( s194, s205 ), str( s ), s228( s291 ) { }
	s290( const s290& right );
	const std::wstring& Str() const { return str; }
	const std::wstring& s473() const { return s228; }
	virtual s165* s226() const override;

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};

class s293
								: public s211
{
protected:
	s294 s295;

	struct s296

	{
		std::wstring s172;
		size_t s297;
		s294 s414;
		s296( const std::wstring& s298, s294 fp, size_t type_ )
			: s172( s298 ), s297( type_ ), s414( fp )	{}
		s296()
			: s414( NULL )	{}
	};
	typedef std::map<std::wstring, s296*> s299;
	virtual const s299& s300() const = 0;
public:
	s293( s162* s194 ) : s211( s194, s202 ) {}
	virtual size_t s235( const std::wstring& s236 ) const override;
	s294 s301( const std::wstring& s236 )const;
	virtual const s211* s225() const override;
	virtual void* addr() = 0;
	virtual s211* s219( s211** s168, size_t s220 ) override;
	static s211* s303( s162* s194, const std::wstring& val, const std::wstring& s228 );
};

class s304;

class s304
						: public s293
{
	static size_t type;

	static const std::wstring s409;
	static s299 s413;
	int val;

	static void set( s211** s168, size_t n );
	static void sum( s211** s168, size_t n );
	static void dif( s211** s168, size_t n );
	static void mul( s211** s168, size_t n );
	static void div( s211** s168, size_t n );
	static void pow( s211** s168, size_t n );
	static void sumeq( s211** s168, size_t n );
	static void difeq( s211** s168, size_t n );
	static void muleq( s211** s168, size_t n );
	static void diveq( s211** s168, size_t n );
	static void poweq( s211** s168, size_t n );
	static void eq( s211** s168, size_t n );
	static void less( s211** s168, size_t n );
	static void greater( s211** s168, size_t n );
	static void leq( s211** s168, size_t n );
	static void geq( s211** s168, size_t n );
	static void neq( s211** s168, size_t n );
	static void abs( s211** s168, size_t n );
	static void mod( s211** s168, size_t n );
	static void sqrt( s211** s168, size_t n );
	static void nroot( s211** s168, size_t n );
	static void toDouble( s211** s168, size_t n );

	virtual void s234() override;
	virtual const s299& s300() const override { return s413; }
	virtual void s237( size_t type ) override;

public:
	s304( const s304& );
	s304( s162* s194, int i );
	s304( s162* s194, const std::wstring& s, size_t& pos );

	static size_t s305() { return type; }

	virtual const std::wstring& s238() const override { return s409; }

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) override;
	virtual s211* s239() const override;

	virtual size_t s221() const override;


	virtual void s223( const std::wstring& s228 ) override;	
	virtual s211* s226() const override;
	virtual void s240( s212* p ) const override;

	int* Ptr() { return &val; }
	virtual void* addr() override { return (void*)&val; }
	virtual operator bool() const { return val != 0; }
	virtual operator int() const { return val; }
	virtual operator double() const { return (double)val; }
	virtual std::wstring to_wstring() const { return std::to_wstring( val ); }

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};

class s306
								: public s293
{
	static size_t type;

	static const std::wstring s409;
	static s299 s413;
	double val;

	static void set( s211** s168, size_t n );
	static void sum( s211** s168, size_t n );
	static void dif( s211** s168, size_t n );
	static void mul( s211** s168, size_t n );
	static void div( s211** s168, size_t n );
	static void pow( s211** s168, size_t n );
	static void sumeq( s211** s168, size_t n );
	static void difeq( s211** s168, size_t n );
	static void muleq( s211** s168, size_t n );
	static void diveq( s211** s168, size_t n );
	static void poweq( s211** s168, size_t n );
	static void eq( s211** s168, size_t n );
	static void less( s211** s168, size_t n );
	static void greater( s211** s168, size_t n );
	static void leq( s211** s168, size_t n );
	static void geq( s211** s168, size_t n );
	static void neq( s211** s168, size_t n );
	static void dcmp( s211** s168, size_t n );
	static void dcmpr( s211** s168, size_t n );
	static void abs( s211** s168, size_t n );
	static void sqrt( s211** s168, size_t n );
	static void nroot( s211** s168, size_t n );
	static void toInt( s211** s168, size_t n );

	virtual void s234() override;
	virtual const s299& s300() const override { return s413; }
	virtual void s237( size_t type ) override;

public:
	s306( const s306& r );
	s306( s162* s194, double d );
	s306( s162* s194, const std::wstring& s, size_t& pos );

	static size_t s305() { return type; }
	virtual const std::wstring& s238() const override { return s409; }

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) override;
	virtual s211* s239() const override;

	virtual size_t s221() const;
	virtual void s223( const std::wstring& s228 ) override;

	virtual s211* s226() const override;
	virtual void s240( s212* p ) const override;

	double* Ptr() { return &val; }
	virtual void* addr() override { return (void*)&val; }
	virtual operator bool() const { return val != 0.0; }
	virtual operator int() const { return (int)val; }
	virtual operator double() const { return val; }
	virtual std::wstring to_wstring() const;

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};

class s307;

class s307
								: public s293
{
	static size_t type;

	static const std::wstring s409;
	static s299 s413;
	std::wstring val;

	static void set( s211** s168, size_t n );
	static void sum( s211** s168, size_t n );
	static void eq( s211** s168, size_t n );
	static void less( s211** s168, size_t n );
	static void greater( s211** s168, size_t n );
	static void leq( s211** s168, size_t n );
	static void geq( s211** s168, size_t n );
	static void neq( s211** s168, size_t n );

	virtual void s234() override;
	virtual const s299& s300() const override { return s413; }
	virtual void s237( size_t type ) override;

public:
	s307( const s307& );
	s307( s162* s194, const std::wstring& s = std::wstring( L"" ) );
	s307( s162* s194, const std::wstring& s, size_t& pos );

	static size_t s305() { return type; }
	virtual const std::wstring& s238() const override { return s409; }

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) override;
	virtual s211* s239() const override;

	virtual size_t s221() const;

	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s226() const override;
	virtual void s240( s212* p ) const override;

	std::wstring* Ptr() { return &val; }
	virtual void* addr() override { return (void*)&val; }
	virtual operator bool() const { return !val.empty(); }
	virtual operator int() const { return std::stoi( val ); }
	virtual operator double() const { return std::stod(val); }
	virtual std::wstring to_wstring() const { return val; }

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};

class s308
							: public s293
{
	static size_t type;

	static const std::wstring s409;
	static s299 s413;
	virtual void s234() override {}
	virtual const s299& s300() const override { return s413; }
	virtual void s237( size_t type ) override;
public:
	s308( s162* s194 ) : s293( s194 ) { s216 = s308::s305(); }
	static size_t s305() { return type; }

	virtual const std::wstring& s238() const override { return s409; }

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) override;
	virtual s211* s239() const override;

	virtual size_t s221() const;
	virtual void* addr() override { return (void*)0; }

	virtual void s240( s212* p ) const override { }
	virtual void s223( const std::wstring& s228 ) override;
	virtual operator bool() const { return false; }

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};
extern s308 nullobject;

class s309
						: public s293
{
	static size_t type;
	static const std::wstring s409;
	static s299 s413;
	size_t s474;
	std::vector<s165*> val;
	std::wstring s222;

	static void set( s211** s168, size_t n );
	static void get( s211** s168, size_t n );
	static void add( s211** s168, size_t n );
	static void size( s211** s168, size_t n );
	static void resize( s211** s168, size_t n );
	static void eq( s211** s168, size_t n );

	virtual void s234() override;
	virtual const s299& s300() const override { return s413; }
	virtual void s237( size_t type ) override;

public:
	s309( s162* s194, const std::wstring& sf );
	s309( const s309& right );
	s309( s162* s194, const std::wstring& s, size_t& pos );

	static size_t s305() { return type; }
	virtual const std::wstring& s238() const override { return s409; }
	virtual size_t s217() const override;

	virtual s211* s232( const std::wstring& val, const std::wstring& s228 ) override;
	virtual s211* s239() const override;

	virtual size_t s221() const override;
	size_t s312() const { return s474; }
	virtual bool isTypeOf( const s211* classObj ) const override;
	virtual void* addr() override { return (void*)&val; }

	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s226() const override;
	virtual void s240( s212* p ) const override;

	std::vector<s165*>& Ref() { return val; }

	virtual std::wstring to_wstring() const;

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;

};

typedef s304 TDInt;
typedef s306 TDDouble;
typedef s307 TDString;
typedef s309 TDVector;

class s230
						: public s165
{
	std::wstring s231;
	s211* s313;
	const s211* s314;
	std::vector<size_t> s315;
	size_t s316;
public:
	s230( const std::wstring& s, s162* s194 ) : s212( s194, s203 ), s231( s ), s316( 0 )/*, off(NULL)*/ { }
	s230( const s230& right );

	virtual bool isTypeOf( const s211* classObj ) const override;

	virtual s211* s218() override;
	virtual const s211* s225() const override { return s314; }
	const s211* Root() { return s313; }

	const std::wstring& ValName() const { return s231; }

	virtual s211* s219( s211** s168, size_t s220 ) override;

	s211* s317( s166* ob );
	s211* s317( s211** s318 );
	s211* s320( const s166* ob ) const;
	s211* s321( s166* ob );
	void s319( size_t off, const s211* s475 );
	size_t s323() const { return s316; }
	virtual s165* s226() const override;

	void s223( const s166* s354, const std::wstring& s222 );
	void s324( s211* p ) { s313 = p; }
	void s325( s211* p ) { s314 = p; }

	virtual void s198( std::wostream* pd, int ind = 0 ) const override;
};
class s326;
class s166;
typedef s1::s3<s166> PObject;
typedef std::vector<std::pair<std::wstring, std::wstring>> s479;
typedef std::map<std::wstring, s166*> s478;
typedef std::map<std::wstring, s163*> s480;

class s166
						: public s211
{
protected:
	const s166* s327;
	const s166* s510;
	s166* ns;
	size_t type;
	const std::wstring& s409;
	const std::wstring s172;
	std::wstring s483;

	s210 s328;
	s286* s329;
	std::vector<s212*> s330;
	std::vector<std::wstring> s422;


	s326* s333;

	virtual void s234() override { }
	virtual void s237( size_t type ) override;
public:

	s166( const s166& s421 );
	s166( s162* s194, size_t type_, s200 pc/* = s201*/ );
	s166( const std::wstring& s172, s162* s194, s200 pc );
	s166( const std::wstring& s409, const std::wstring& s172, s162* s194, s210& s332,
		const s210& s328 );
	s166( s163& s372, s162* s194, s200 pc, s166* ns, s455 am, 
		s478& cl, s479& cnl, s480& al );
	const std::wstring& s193() const { return s172; }
	std::wstring s192() const;

	static s165* s477( const s44::s57& obj, const std::wstring& s172, s162* s194 );
	static s165* s477( std::multimap<std::wstring, s212*>& v, s212* s354 );
	s211* s424( s163& s372, const std::wstring& s228 );
	s211* s424( const s44::s55& val );

	virtual s211* s239() const override;
	size_t s305() { return type; }
	virtual size_t s235( const std::wstring& s236 ) const;
	virtual bool isTypeOf( const s211* classObj ) const override;

	virtual s211* s232( s163& s372, const std::wstring& s256 );
	virtual s211* s232( const std::wstring& val, const std::wstring& s256 ) override;


	virtual size_t s221() const override;
	virtual const std::wstring& s238() const override { return s409; }
	virtual const s211* s225() const override { return s327; }
	const s166* s481() const { return s510; }
	const std::wstring& s482() const { return s483; }
	virtual s211* s219( s211** s168, size_t s220 );
	s326* s301( const std::wstring& s172, const std::wstring& s228,
		bool thr = true ) const;
	void s425( s163& s372 );
	void s484( const std::wstring& nam, const std::wstring& s420 );
	void s485( s163& s372 );
	void s426( const std::wstring& s228,	std::vector<std::wstring>& res );

	virtual bool s242( const std::wstring& s172, s230* res,
		const std::wstring& s228 ) const override;
	size_t s335( const std::wstring& s172 ) const;
	s211* s336( const std::wstring& s172, const std::wstring& s228 );
	virtual s165* s337( size_t off );
	s165* s338( const std::wstring& s172, const std::wstring& s228,
		bool thr=true ) const;
	bool s339( const std::wstring& s411, const std::wstring& s222 ) const;
	bool s267( const std::wstring& s411 );


	virtual void s223( const std::wstring& s222 ) override;	
	void s486( const s210& s168 ); 
	void s65( s163& s372 );
	virtual s165* s226() const  override;
	virtual void s240( s212* p ) const override;
	void s487( s166* desc ) const;

	virtual std::wstring to_wstring() const override;

	virtual void s198( std::wostream* pd, int ind = 0 ) const;
};
class s326;
class s162;
typedef s326 TD;
class s293;
class s428;

typedef s326 Transd;

class s326
								: public s166
{
protected:
	std::wstring s172;

	s294 s340;
	enum s341 { s342, s343, s344, s345, s346, s347 };
	enum s488 { s489, s490, s491, s492 };
	s341 s348;
	s488 virtType;

	s211** s349;
	s212** s350;
	size_t s220, s464;

	s210 s351;
	std::vector<s211*> s494;
	std::vector<s212*> s168;
	std::vector<s211*> s352;
	std::vector<s212*> s353;
	std::vector<std::vector<s212*>> s493;
	s211* s449;
	s166* s354;
	s166* s431;
	virtual void s237( size_t type ) override { }
	void s495(const std::wstring& s228);
	void s496(const std::wstring& s228);


public:
	s326( s162* s194, s166* s443, s166* s444, s341 ct, 
							const std::wstring& s298 )
		: s166( s194, 0, s204 ), s449( NULL ), s172( s298 ),
		s431( s443 ), s354( s444 ), s340( NULL ), s348( ct ) { }
	s326( const s326& right, s166* s431, s166* s354,
		const std::vector<s212*>& s168 );
	s326( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s326( s162* s194, s166* s354, const std::vector<s212*>& l,
		s294 s340, size_t s216 );
	virtual ~s326();
	void s499( s488 vt ) { virtType = vt; }
	s488 s500() const { return virtType; }
	virtual const std::wstring& s193() const { return s172; }
	virtual s211* s218() override { return s449; }
	virtual void s223( const std::wstring& s228 ) override;
	void s497( const std::vector<s212*>& l );
	void s498( const std::wstring& s228 );
	virtual bool s242( const std::wstring& s172, s230* ref,
		const std::wstring& s228 ) const override;
	virtual s165* s337( size_t off ) override;
	virtual s211* s219( s211** s168 = NULL, size_t s220 = 0 ) override;
	virtual s165* s226() const override;
	static Transd* s356( s163& s372, s162* s194, s166* obj,
		const std::wstring& s172, const std::wstring& s420 );
	static s428* s357( s163& s372, s162* s194, s166* obj, const std::wstring& s420 );
	static Transd* s358( s428* cs, s162* s194, s166* s431, 
		s166* s432, const std::wstring& s420 );

	virtual void s198( std::wostream* pd, int ind = 0 ) const override { }
};

class s428
							: public s165
{
	const std::wstring s359;
	std::vector<s212*> s168;

	s163* s469;

public:
	s428( s162* s468, s163* s467 )
		: s165( s468, s206 ), s469( s467 )
	{
	}
	s428( const std::wstring& s465, const std::vector<s212*>& l );
	s428( const s428& s466 );

	const std::wstring& s193() const { return s359; }
	std::vector<s212*>& s360() { return s168; }
	s163* s361() { return s469; }
	virtual void s223( const std::wstring& s222 ) override;
	virtual s165* s226() const override;
	virtual void s198( std::wostream* pd, int ind = 0 ) const override { }
};

class s501
								: public s326
{
	std::vector<size_t> types;
	std::vector<s326*> ptrs;
public:
	s501( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	void s502( size_t type, s326* pOvr );
	virtual s211* s219( s211** s168, size_t s220 ) override;
};


class s362
						: public s326
{
public:
	s362( s162* s194, s166* s443, s166* s444, const std::vector<s212*>& l );
	virtual const std::wstring& s193() const override { return s139; }
	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s219( s211** s168, size_t s220 ) override;

	s165* s447( const std::wstring& s172, s230* ref,
		const std::wstring& s228 ) const;
	virtual s211* s226() const override;

};

class s364
					: public s326
{
public:
	s364( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s364( s162* s194 );
	virtual const std::wstring& s193() const override { return s150; }
	virtual void s223( const std::wstring& s228 ) override { }
	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;

};

class s365
					: public s326
{
public:
	s365( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s365( s162* s194 );
	virtual const std::wstring& s193() const override { return s149; }

	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;

};

class s366
					: public s326
{
public:
	s366( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s366( s162* s194 );
	virtual const std::wstring& s193() const override { return s136; }
	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;

};

class s367
							: public s326
{
public:
	s367( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s367( s162* s194 );
	virtual const std::wstring& s193() const override { return s157; }
	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;
};

class s368
						: public s326
{
	s326* condition;
	s326* step;
public:
	s368( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s368( s162* s194 );
	virtual const std::wstring& s193() const override { return s160; }
	virtual void s223( const std::wstring& s228 ) override;
	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;

};

class s369
						: public s326
{
public:
	s369( s162* s194, s166* s431, s166* s354, const std::vector<s212*>& l );
	s369( s162* s194 );
	virtual const std::wstring& s193() const override { return s157; }

	virtual s211* s219( s211** s168, size_t s220 ) override;
	virtual s211* s226() const override;
};
typedef int HPROG;

class s428;

class s162
							: public s166
{
	std::vector<std::wstring> s371;
	s163 s372;
	std::wstring s373;

	void s375( const std::wstring& s172 );
	void s376( const s44::s57& obj );

	s213 s451;
	s210 s470;
	s210 s471;

public:
	s162();
	~s162();
	void s223();
	void s274();
	void s503( s163& _ast );
	void s504( const std::wstring& s387 );
	void s506( s44::s57& obj );
	void s507( const std::wstring& s387 );
	s211* run();
	s211* callFunc( const std::wstring& s359, std::vector<std::wstring>& l );
	s326* getProc( const std::wstring& s359 );

	s326* s379( const std::wstring& s359, std::vector<s212*>& s168, s166* s431, s166* s432 );
	s165* s380( const std::wstring& val, const std::wstring& s228 );
	s211* s381( size_t type, const std::wstring& s228 );
	s211* s381( const std::wstring& val, const std::wstring& s228 );
	s211* s381( s163& s372, const std::wstring& s228 );
	s211* s381( const s290& ds );
	void s382( const std::wstring& s );
	void s254( s211* p );
	void s384( const std::wstring& val, const std::wstring& s228,
		std::vector<std::wstring>& res );
	s211* s385( const std::wstring& val, const std::wstring& s228 );
	s165* s386( const std::wstring& s411, const std::wstring& s222,
		bool thr = true );
	void* getExportVariable( const std::wstring& s172 );

};
void evaluateExpression( const std::wstring& sf );
HPROG createAssembly();
void loadProgram( HPROG handle, const std::wstring& s396 );
void s506( HPROG handle, s44::s57& obj );
s211* run( HPROG handle );
void* callFunc( HPROG handle, const std::wstring& sf, std::vector<std::wstring>& l );
TDType* getProc( HPROG handle, const std::wstring& s359 );

void* getExportVariable( HPROG handle, const std::wstring& s172 );
void* execute( TDType* func );
void importSymbol( HPROG handle, const std::wstring& modName, const std::wstring& symName,
										const std::wstring& s228 );
void importFile( HPROG handle, const std::wstring& modName, const std::wstring& s390,
	const std::wstring& s228 );
HPROG initShell( const std::wstring& modName, const std::wstring& modText );


typedef s4::s5 TDException;
} // namespace transd
