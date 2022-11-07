/*********************************************************************
*							公共头文件
**********************************************************************/
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#pragma warning(disable:4267)
#pragma warning(disable:4819)

#if defined(_MSC_VER) && defined(PMDLL)
#define EXPORT_API _declspec(dllexport)
#else
#define EXPORT_API
#endif

// *********************************************************************
// 宏定义
// *********************************************************************
#ifdef WIN
#define PRINT(msg)			MYTRACE(msg)//{std::stringstream ss; ss << "\n> " << msg; printf(ss.str().c_str());}
#else
#define PRINT(msg)			{}
#endif
#define PRINTV(v)			PRINT(#v << " = " << v)
#define PRINTVEC(vm, v)		PRINT(vm << " = " << v.x << "," << v.y << "," << v.z)
#define PRINTVEC2(v)		PRINT(#v << " = " << v.x << "," << v.y)
#define PRINTV2(v)			PRINTVEC2(v)
#define PRINTVEC3(v)		PRINT(#v << " = " << (v).x << "," << (v).y << "," << (v).z)
#define PRINTV3(v)			PRINTVEC3(v)
#define PRINTVEC4(v)		PRINT(#v << " = " << v.x << "," << v.y << "," << v.z << "," << v.w)
#define PRINTTRI(p1,p2,p3)	PRINT("tri" << "(" << p1.x << "," << p1.y << "," << p1.z << "," << p2.x << "," << p2.y << "," << p2.z << "," << p3.x << "," << p3.y << "," << p3.z << ")")
#define PRINTCVP(p)			PRINT("cvp(" << 50 * p.x + 500 << "," << 50 * p.y + 500 << "," << 50 * p.z + 500 << ")")

extern std::stringstream logmessage;
inline void savelog(std::stringstream& log = logmessage, const std::string& logfile = "log.txt")
{
	std::ofstream	out(logfile, std::ofstream::app);
	out << log.str();
	out << std::endl;

	log.str("");
}
#ifdef WIN
#define MSGBOX(msg)		{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0,ss.str().c_str(),"MSG", 0);}
#define MYTRACE(msg)	{std::stringstream ss; ss << "\n" << msg; printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());LOGMSG(msg)}
#define LOGMSG(msg)		{if(gblog) {logmessage << msg; savelog();}}
#define event(obj, msg)	{if(gblog) logmessage << "\n" << obj << " : " << msg;}
#define ASSERT(x)		{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; ::MessageBoxA(0, ss.str().c_str(), "ASSERT", 0); throw;} }
#define ASSERT_RET(x)	{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; ::MessageBoxA(0, ss.str().c_str(), "ASSERT", 0); return 0;} }
#define ERRORMSG(msg)	{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0, ss.str().c_str(), "ERROR", 0);}
#else
#define MSGBOX(msg)		PRINT(msg)//{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0,ss.str().c_str(),"MSG", 0);}
#define MYTRACE(msg)	{}//{std::stringstream ss; ss << "\n" << msg; printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());LOGMSG(msg)}
#define LOGMSG(msg)		{}//{if(gblog) {logmessage << msg; savelog();}}
#define event(obj, msg)	{}//{if(gblog) logmessage << "\n" << obj << " : " << msg;}
#define ASSERT(x)		{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; PRINT(ss.str());} }
#define ASSERT_RET(x)	{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; PRINT(ss.str()); return 0;} }
#define ERRORMSG(msg)	{std::stringstream ss; ss << "\n" << msg; PRINT(ss.str());} }
#endif
#define COND_THROW(cond, msg)  if(cond){std::stringstream ss; ss << msg; throw(std::runtime_error(ss.str()));}

// cc keywords
#define crstr			const std::string&
#define uint			unsigned int
#define uchar			unsigned char
#define crauto			const auto&
#define rauto			auto&

#define form			struct
#define scope			namespace

#define PI				3.14159265358979323846264338327950288

#define delta_d			0.001f		// 空间微分精度
#define delta_t			0.001f		// 时间微分精度

#define	FIMAGESCALE		((real)IMAGESCALE)
#define	UNIT			(1.0 / FIMAGESCALE)
#define DRAWSCALE		1

#define _HAS_STD_BYTE 1

#define RGB(r, g, b)	(0xFF000000 | ((unsigned int)((((unsigned char)(r))) | (((unsigned int)(unsigned char)(g)) << 8) | (((unsigned int)(unsigned char)(b)) << 16))))
inline unsigned int _RGB(unsigned char r, unsigned char g, unsigned char b) { return RGB(r, g, b); }

#define RGBA(r,g,b,a)   ((unsigned int)(((unsigned char)(r)) | (((unsigned int)(unsigned char)(g)) << 8) | (((unsigned int)(unsigned char)(b)) << 16) |(((unsigned int)(unsigned char)(a)) << 24)))

#define GetRValue(rgb)	((unsigned char)(0x000000ff & (rgb)))
#define GetGValue(rgb)	((unsigned char)((0x0000ff00 & (rgb))>>8))
#define GetBValue(rgb)	((unsigned char)((0x00ff0000 & (rgb))>>16))
#define GetAValue(rgb)	((unsigned char)((0xff000000 & (rgb))>>24))

#define MIN(a, b)		((a) < (b) ? (a) : (b))
inline real _MIN(real a, real b) { return ((a) < (b) ? (a) : (b)); }
inline int _MINi(int a, int b) { return ((a) < (b) ? (a) : (b)); }

#define MAX(a, b)		((a) > (b) ? (a) : (b))
inline real _MAX(real a, real b) { return ((a) > (b) ? (a) : (b)); }

#define vec				vector3
#define vec3			vector3
#define rvec			vector3&
#define crvec			const vector3&
#define vec2			vector2
#define rvec2			vector2&
#define crvec2			const vector2&
#define ivec2			point_t
#define pnt				point_t
#define vec4			vector4
#define rvec4			vector4&
#define crvec4			const vector4&

#define DEFAULTVEC3		vec3::ZERO
#define itovec2(ix, iy)	vector2((ix / FIMAGESCALE - 0.5f) * 2.0f, (iy / FIMAGESCALE - 0.5f) * 2.0f)
#define pixel2d(p, cor)	pixel(int((p.x / 2 + 0.5) * FIMAGESCALE), int((p.y / 2 + 0.5) * FIMAGESCALE), cor)
#define vec2i(p)		int((p.x / 2 + 0.5) * FIMAGESCALE), int((p.y / 2 + 0.5) * FIMAGESCALE)

#define mix				lerp

#define LOOP0(i, szvar, szval) const int szvar = szval; for(int i = 0; i <= szvar; i ++){real a##i = i / real(szvar);	
#define LOOP(i, szvar, szval) const int szvar = szval; for(int i = 0; i < szvar; i ++){real a##i = i / real(szvar);	
#define FOR(i, szvar, szval) const int szvar = szval; for(int i = 0; i < szvar; i ++){real a##i = i / real(szvar);	
#define END	}
#define RNDBW	blendcor(1, 0xFF808080, rrnd(0.25, 1))
#define RNDCOR	RGB(rrnd(0, 255), rrnd(0, 255), rrnd(0, 255))
#define VECLIST	std::vector<vertex>
#define VERLIST	std::vector<vertex>
#define ADDE(__e, __v)	__e.push_back(__v)

#define E_	0.61828183
#define E1	(E_)
#define E2	(E1 * E_)
#define E3	(E2 * E_)
#define E4	(E3 * E_)
#define E5	(E4 * E_)
#define E6	(E5 * E_)
#define E7	(E6 * E_)
#define E8	(E7 * E_)
#define E9	(E8 * E_)

#define PUSH	push_back
#define	CIRCLE(d_i, r, n, ang, ang0)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * ang + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE2(d_i, r, n, ang0)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE3(d_i, r, n, ang0)	LOOP0(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE4(d_i, r, n, ang, ang0)	LOOP0(d_i, len##d_i, n)real ang##d_i = a##d_i * ang + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));

#define BEGINDRAW renderstate = 1;
#define ENDDRAW renderstate = 2;
#define DEF_SCALES(s) 	real s1 = s  * E_ * E_;real s2 = s1 * E_ * E_;real s3 = s2 * E_ * E_;real s4 = s3 * E_ * E_;real s5 = s4 * E_ * E_;real s6 = s5 * E_ * E_;real s7 = s6 * E_ * E_;real s8 = s7 * E_ * E_;
#define DEF_SCALES2(s) 	real s1 = s  * E_;real s2 = s1 * E_;real s3 = s2 * E_;real s4 = s3 * E_;real s5 = s4 * E_;real s6 = s5 * E_;real s7 = s6 * E_;real s8 = s7 * E_;
#define START_MESH	resetsm();color = 0xFFFFFFFF;
#define END_MESH //exportsm();
#define	VEC2STR(v)	(v).x << ", " << (v).y << ", " << (v).z

// ----------------------------------------------------------------------
#define LUA_API_CC(fun)	static int fun(lua_State* L)
#define LUA_REGIST(funname, fun)	lua_register(L, funname, fun);

#define LUA_FUN_BEGIN(fun)	static int fun(lua_State* L){ PRINT("call:" << #fun);int n = lua_gettop(L);for (int i = 0; i < n; i++) {luaparam[i] = lua_tonumber(L, 1 + i);PRINT("luaparam" << i << ":" << luaparam[i])}
#define LUA_FUN_BEGIN_NOPPRAM(fun)	static int fun(lua_State* L){ PRINT(#fun);
#define LUA_FUN_END	return 0;}
#define LUA_FUN_END_RET(ret)	lua_pushnumber(L, ret); return 1;}

// ----------------------------------------------------------------------
inline real rrnd(real min = 0.0, real max = 1.0) { return (max) == (min) ? (min) : (min)+((real)(rand()) / (RAND_MAX + 1.0)) * ((max)-(min)); }
inline int rndi(int min, int max) { return min == max ? min : rand() % (max - min + 1) + min; }
inline int RGB2(int r, int g, int b) { return RGB(r, g, b); }
inline int RGBA2(int r, int g, int b, int a) { return RGBA(r, g, b, a); }
inline bool iszero(real x, real tor = 1e-5) { return fabs(x) < tor; }

// **********************************************************************
// 全局状态
int renderstate = 0;			// 渲染模式: 0 -3D构型， 1 -正常渲染 2 -网格渲染, >=3 -2d绘图，-1 not rendering 
real frametime = 0;
bool gblog = true;
std::stringstream logmessage;	// 日志

// **********************************************************************
#include "vector.hpp"
#include "quaternion.hpp"
#include "_numbers.hpp"

// ----------------------------------------
void v2vxvy(const vec& v, vec& vx, vec& vy)
{
	vec uz = vec::UZ;
	if (v.x < EPSINON && v.x > -EPSINON && v.y < EPSINON && v.y > -EPSINON)
	{
		uz = vec::UX;
	}
	vx = uz.cross(v); vx.norm();
	vy = v.cross(vx);
}
void vz2vxvy(const vec& v, vec& vx, vec& vy)
{
	vec uy = vec::UY;
	if (v.x < EPSINON && v.x > -EPSINON && v.z < EPSINON && v.z > -EPSINON)
	{
		uy = vec::UZ;
	}
	vx = v.cross(uy); vx.norm();
	vy = vx.cross(v);
}

void vz2vxvy(const vec& v, const vec& vx, vec& vy)
{
	vec uy = vec::UY;
	if (v.x < EPSINON && v.x > -EPSINON && v.z < EPSINON && v.z > -EPSINON)
	{
		uy = vec::UZ;
	}
	vy = vx.cross(v);
}
vec v2vx(const vec& v)
{
	vec uz = vec::UZ;
	if (v.x < EPSINON && v.x > -EPSINON && v.y < EPSINON && v.y > -EPSINON)
	{
		uz = vec::UX;
	}
	vec vx = uz.cross(v); vx.norm();
	return vx;
}
// ----------------------------------------
inline vec scaleby(crvec v, crvec o, real s)
{
	return (v - o) * s + o;
}
// ----------------------------------------
void vector3::rot(real angle, const vector3& ax = vec::UZ)
{
	quaternion q(angle, ax);
	*this = q * (*this);
}
// ----------------------------------------
vector3 vector3::rotcopy(real angle, const vector3& ax = vec::UZ) const
{
	quaternion q(angle, ax);
	return q * (*this);
}

vector3 vector3::rotcpy(real angle, const vector3& ax = vec::UZ) const
{
	quaternion q(angle, ax);
	return q * (*this);
}
// **********************************************************************
// ray3d
// **********************************************************************
struct ray3
{
	vector3 o;
	vector3 dir;
	ray3() {}
	ray3(const vector3& p1, const vector3& p2)
	{
		o = p1;
		dir = (p2 - p1).normcopy();
	}

};
// **********************************************************************
// segment
// **********************************************************************
struct segment
{
	vector3 s;
	vector3 e;
	segment(const vector3& _s, const vector3& _e)
	{
		s = _s;
		e = _e;
	}
};
// **********************************************************************
// plane
// **********************************************************************
struct plane_t
{
	vector3 o;
	vector3 n;
	plane_t(const vector3& _o, const vector3& _n)
	{
		o = _o;
		n = _n;
		n.norm();
	}
	plane_t(const vector3& p1, const vector3& p2, const vector3& p3)
	{
		n = (p2 - p1).cross(p3 - p1);
		n.norm();
		o = p1;
	}
};
// **********************************************************************
// 2D point
// **********************************************************************
struct point_t
{
	int x = 0, y = 0;
	point_t()
	{
		x = 0;
		y = 0;
	}
	point_t(int ix, int iy)
	{
		x = ix; y = iy;
	}
	int len()
	{
		return sqrt((x) * (x)+(y) * (y));
	}
	vec2 tovec() const
	{
		return vec2(x, y);
	}
	bool operator == (const point_t& rp) const
	{
		return (x == rp.x && y == rp.y);
	}
	bool operator != (const point_t& rp) const
	{
		return (x != rp.x || y != rp.y);
	}
	point_t operator + (const point_t& rp) const
	{
		point_t p;
		p.x = x + rp.x;
		p.y = y + rp.y;
		return  p;
	}
	point_t operator - (const point_t& rp) const
	{
		point_t p;
		p.x = x - rp.x;
		p.y = y - rp.y;
		return  p;
	}
	point_t operator / (real factor) const
	{
		point_t p;
		p.x = x / factor;
		p.y = y / factor;
		return  p;
	}
	static real dis(const point_t& p1, const point_t& p2)
	{
		return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}
	static real Mdis(const point_t& p1, const point_t& p2)
	{
		return abs(p1.x - p2.x) + abs(p1.y - p2.y);
	}
	static real dis(int x1, int y1, int x2, int y2)
	{
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}
	vec2 tovec()
	{
		vec2 p;
		p.x = x;
		p.y = y;
		return p;
	}
	/*void fromvec(const vec2& v)
	{
		x = v.x;
		y = v.y;
	}*/
	static point_t fromvec(const vec2& v)
	{
		point_t p;
		p.x = v.x;
		p.y = v.y;
		return p;
	}
};
// **********************************************************************
// 2D rect
// **********************************************************************
struct rect_t
{
	int x = 0, y = 0, width = 0, height = 0;
};
// **********************************************************************
// 3D point
// **********************************************************************
struct point3_t
{
	int x = 0, y = 0, z = 0;
	point3_t()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	point3_t(int ix, int iy, int iz)
	{
		x = ix; y = iy; z = iz;
	}
	int llen() const
	{
		return ((x) * (x)+(y) * (y)+(z) * (z));
	}

	bool operator == (const point3_t& rp) const
	{
		return (x == rp.x && y == rp.y && z == rp.z);
	}
	point3_t operator + (const point3_t& rp) const
	{
		point3_t p;
		p.x = x + rp.x;
		p.y = y + rp.y;
		p.z = z + rp.z;
		return  p;
	}
	point3_t operator - (const point3_t& rp) const
	{
		point3_t p;
		p.x = x - rp.x;
		p.y = y - rp.y;
		p.z = z - rp.z;
		return  p;
	}
	point3_t operator / (real factor) const
	{
		point3_t p;
		p.x = x / factor;
		p.y = y / factor;
		p.z = z / factor;
		return  p;
	}
	vec3 tovec() const
	{
		return vec3(x, y, z);
	}
};

// **********************************************************************
// boundingbox
// **********************************************************************
struct boundingbox
{
	vector3	a;
	vector3 b;
	boundingbox() {}
	boundingbox(const vector3& pt)
	{
		a = pt;
		b = pt;
	}
	boundingbox(const vector3& _min, const vector3& _max)
	{
		a = _min;
		b = _max;
	}
	inline bool intersection(const vector3& pt, real tolerance = 0) const
	{
		return
			pt.x >= a.x - tolerance && pt.x < b.x + tolerance &&
			pt.y >= a.y - tolerance && pt.y < b.y + tolerance &&
			pt.z >= a.z - tolerance && pt.z < b.z + tolerance;
	}
	std::pair<bool, real> intersection(const ray3& ray) const
	{
		const boundingbox& box = *this;
		real lowt = 0.0f;
		real t;
		bool hit = false;
		vector3 hitpoint;
		const vector3& min = box.a;
		const vector3& max = box.b;
		const vector3& rayorig = ray.o;
		const vector3& raydir = ray.dir;

		// Check origin inside first
		if (rayorig > min && rayorig < max)
		{
			return std::pair<bool, real>(true, 0.);
		}

		// Check each face in turn, only check closest 3
		// Min x
		if (rayorig.x < min.x && raydir.x > 0)
		{
			t = (min.x - rayorig.x) / raydir.x;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max x
		if (rayorig.x > max.x && raydir.x < 0)
		{
			t = (max.x - rayorig.x) / raydir.x;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min y
		if (rayorig.y < min.y && raydir.y > 0)
		{
			t = (min.y - rayorig.y) / raydir.y;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max y
		if (rayorig.y > max.y && raydir.y < 0)
		{
			t = (max.y - rayorig.y) / raydir.y;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min z
		if (rayorig.z < min.z && raydir.z > 0)
		{
			t = (min.z - rayorig.z) / raydir.z;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max z
		if (rayorig.z > max.z && raydir.z < 0)
		{
			t = (max.z - rayorig.z) / raydir.z;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}

		return std::pair<bool, real>(hit, lowt);
	}
	/** Returns whether or not this box intersects another. */
	inline bool intersection(const boundingbox& b2) const
	{
		// Use up to 6 separating planes
		if (b.x < b2.a.x)
			return false;
		if (b.y < b2.a.y)
			return false;
		if (b.z < b2.a.z)
			return false;

		if (a.x > b2.b.x)
			return false;
		if (a.y > b2.b.y)
			return false;
		if (a.z > b2.b.z)
			return false;

		// otherwise, must be intersecting
		return true;

	}
	void centerzero(const vector3& pt)
	{
		a = pt;
		b = pt;
	}
	void resize(const vector3& pt)
	{
		pt.x < a.x ? a.x = pt.x : 0;
		pt.x > b.x ? b.x = pt.x : 0;
		pt.y < a.y ? a.y = pt.y : 0;
		pt.y > b.y ? b.y = pt.y : 0;
		pt.z < a.z ? a.z = pt.z : 0;
		pt.z > b.z ? b.z = pt.z : 0;
	}
	vector3 getcenter()
	{
		return (a + b) * 0.5;
	}
};

// 整数boundingbox
struct boundingboxi
{
	point3_t a = point3_t(100000, 100000, 100000);
	point3_t b = point3_t(-100000, -100000, -100000);

	boundingboxi() {}

	boundingboxi(const point3_t& _min, const point3_t& _max)
	{
		a = _min;
		b = _max;
	}
	inline bool intersection(const point3_t& pt, int tolerance = 0)
	{
		return
			pt.x >= a.x - tolerance && pt.x < b.x + tolerance &&
			pt.y >= a.y - tolerance && pt.y < b.y + tolerance &&
			pt.z >= a.z - tolerance && pt.z < b.z + tolerance;
	}
	inline bool intersection(const boundingboxi& b2) const
	{
		// Use up to 6 separating planes
		if (b.x < b2.a.x)
			return false;
		if (b.y < b2.a.y)
			return false;
		if (b.z < b2.a.z)
			return false;

		if (a.x > b2.b.x)
			return false;
		if (a.y > b2.b.y)
			return false;
		if (a.z > b2.b.z)
			return false;

		// otherwise, must be intersecting
		return true;
	}
};
// **********************************************************************
// CTexture
// **********************************************************************
#define TEXSIZEX		1024
#define TEXSIZEY		1024
inline unsigned int widthByte32(unsigned int width, unsigned int depth)
{
	// 32 bits alignment (4 bytes)
	int rest = (width * depth / 8) % 4;
	if (rest != 0)
		return (width * depth / 8 + 4 - rest);
	else
		return (width * depth / 8);
}

struct CTexture
{
	unsigned char data[TEXSIZEX * TEXSIZEY * 3]; // 24位数据
	unsigned int at(int x, int y)
	{
		if (x >= TEXSIZEX || x < 0 ||
			y >= TEXSIZEY || y < 0)
			return 0;

		int BytePerPixel = 24 / 8;
		int i = x, j = y;
		int m_WidthByte32 = widthByte32(TEXSIZEX, 24);
		return
			(data[m_WidthByte32 * j + i * BytePerPixel + 0] << 16) |
			(data[m_WidthByte32 * j + i * BytePerPixel + 1] << 8) |
			data[m_WidthByte32 * j + i * BytePerPixel + 2]
			;
	}
};
// **********************************************************************
// HSL
// **********************************************************************
typedef struct
{
	int hue;              // [0,255]
	int saturation;       // [0,255]
	int luminance;        // [0,255]
}COLOR_HSL;

// Converts HSL to RGB
int HSLtoRGB(const COLOR_HSL* hsl)
{
	real h = hsl->hue / 255.f * 360.f;     // h must be [0, 360]
	real s = hsl->saturation / 255.f;		// s must be [0, 1]
	real l = hsl->luminance / 255.f;        // l must be [0, 1]
	real R, G, B;
	if (hsl->saturation == 0)
	{
		// achromatic color (gray scale)
		R = G = B = l * 255.f;
	}
	else
	{
		real q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - (l * s));
		real p = (2.0f * l) - q;
		real Hk = h / 360.0f;
		real T[3];
		T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
		T[1] = Hk;              // Tb
		T[2] = Hk - 0.3333333f; // Tg
		for (int i = 0; i < 3; i++)
		{
			if (T[i] < 0) T[i] += 1.0f;
			if (T[i] > 1) T[i] -= 1.0f;
			if ((T[i] * 6) < 1)
			{
				T[i] = p + ((q - p) * 6.0f * T[i]);
			}
			else if ((T[i] * 2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
			{
				T[i] = q;
			}
			else if ((T[i] * 3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
			{
				T[i] = p + (q - p) * ((2.0f / 3.0f) - T[i]) * 6.0f;
			}
			else T[i] = p;
		}
		R = T[0] * 255.0f;
		G = T[1] * 255.0f;
		B = T[2] * 255.0f;
	}
	unsigned char red = (unsigned char)((R > 255) ? 255 : ((R < 0) ? 0 : R));
	unsigned char green = (unsigned char)((G > 255) ? 255 : ((G < 0) ? 0 : G));
	unsigned char blue = (unsigned char)((B > 255) ? 255 : ((B < 0) ? 0 : B));
	return RGB(red, green, blue);
}
// **********************************************************************
// vector math
// **********************************************************************

inline real fract(real x)
{
	return x - floor(x);
}
inline vec3 floor(vec3 v)
{
	return vec3(floor(v.x), floor(v.y), floor(v.z));
}
inline vec3 fract(vec3 v)
{
	return vec3(fract(v.x), fract(v.y), fract(v.z));
}
inline real lerp(real h1, real h2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return h1 * (1.0f - alpha) + h2 * alpha;
}
inline vector2 lerp(const vector2& v1, const vector2& v2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
inline vector3 lerp(const vector3& v1, const vector3& v2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
// **********************************************************************
// globles
// **********************************************************************
real 				rndmap[1024][1024];
real				fmin1 = 0;
real				fmax1 = 0;

real				fheightmap0[IMAGESCALE][IMAGESCALE] = { 0 };
real(*fheightmap)[IMAGESCALE] = fheightmap0;

real				depthmap0[IMAGESCALE][IMAGESCALE] = { 0 };
real(*depthmap)[IMAGESCALE] = depthmap0;

int					drawmap0[IMAGESCALE][IMAGESCALE] = { 0 };
int(*drawmap)[IMAGESCALE] = drawmap0;

// **********************************************************************
// Perlin 2d random
// **********************************************************************
// Ken Perlin's Map Gennor
// ------------------------------------------------
int myfloor(real value) { return (value >= 0 ? (int)value : (int)value - 1); } // 取整
inline real fade(real x) { return (x * x * x * (x * (6 * x - 15) + 10)); } // this equates to 6x^5 - 15x^4 + 10x^3  // 插值算法

// ------------------------------------------------
inline real dot(real x1, real y1, real x2, real y2)
{
	return x1 * x2 + y1 * y2;
}
// ------------------------------------------------
extern real blend(real h1, real h2, real alpha, real power);
void perlinmap(real map[IMAGESCALE][IMAGESCALE], int size, real* min1, real* max1, int octaves)
{
	//set up some variables
	int i, j, k, x, y, grad11, grad12, grad21, grad22;

	real pixel_value,
		fracX, fracY,
		noise11, noise12, noise21, noise22,
		interpolatedx1, interpolatedx2, interpolatedxy,
		amplitude, frequency,
		gain = 0.50f, lacunarity = 2.0f;
	real gradients[8][2];
	int permutations[256];

	*min1 = 1000000;
	*max1 = -1000000;

	//梯度
	for (i = 0; i < 8; ++i)
	{
		gradients[i][0] = cos(0.785398163f * (real)i);
		gradients[i][1] = sin(0.785398163f * (real)i);
	}

	// 置换表
	{
		for (i = 0; i < 256; ++i)
			permutations[i] = i;
		for (i = 0; i < 256; ++i)
		{
			j = rand() % 256;
			k = permutations[i];
			permutations[i] = permutations[j];
			permutations[j] = k;
		}
	}

	// 生成MAP
	for (i = 0; i < size; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			//get the value for this pixel by adding successive layers
			amplitude = 1.0f;
			frequency = 1.0f / (real)size;
			pixel_value = 0.0f;
			map[j][i] = 0;
			for (k = 0; k < octaves; ++k)
			{
				// 整数
				x = (int)((real)j * frequency);
				y = (int)((real)i * frequency);

				// 小数
				fracX = (real)j * frequency - (real)x;
				fracY = (real)i * frequency - (real)y;

				// 得到梯度索引
				grad11 = permutations[(x + permutations[y % 255]) % 255] % 8;
				grad12 = permutations[(x + 1 + permutations[y % 255]) % 255] % 8;
				grad21 = permutations[(x + permutations[(y + 1) % 255]) % 255] % 8;
				grad22 = permutations[(x + 1 + permutations[(y + 1) % 255]) % 255] % 8;

				// 四个角的梯度投影
				noise11 = dot(gradients[grad11][0], gradients[grad11][1], fracX, fracY);
				noise12 = dot(gradients[grad12][0], gradients[grad12][1], fracX - 1.0f, fracY);
				noise21 = dot(gradients[grad21][0], gradients[grad21][1], fracX, fracY - 1.0f);
				noise22 = dot(gradients[grad22][0], gradients[grad22][1], fracX - 1.0f, fracY - 1.0f);

				// 插值算法
				fracX = fade(fracX);
				fracY = fade(fracY);

				interpolatedx1 = blend(noise11, noise12, fracX, 1);
				interpolatedx2 = blend(noise21, noise22, fracX, 1);
				interpolatedxy = blend(interpolatedx1, interpolatedx2, fracY, 1);

				//	叠加
				pixel_value += interpolatedxy * amplitude;
				amplitude *= gain;

				// 缩小区域
				frequency *= lacunarity;
			}

			//put it in the map
			map[j][i] = pixel_value;

			//do some quick checks
			if (pixel_value < *min1)
				*min1 = pixel_value;
			else if (pixel_value > *max1)
				*max1 = pixel_value;
		}
	}
}

// ------------------------------------------------
// perlinnosie 3d
// ------------------------------------------------
struct ImprovedNoise
{
	static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	static double lerp(double t, double a, double b) { return a + t * (b - a); }
	static double grad(int hash, double x, double y, double z) {
		int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
		double u = h < 8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	static double noise(double x, double y, double z)
	{
		static int p[512] = { -1 };
		if (p[0] == -1)
		{
			static int permutation[512] = { 151,160,137,91,90,15,
				131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
				190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
				88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
				77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
				102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
				135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
				5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
				223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
				129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
				251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
				49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
				138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
			};

			for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
		}


		int X = (int)floor(x) & 255,                  	// FIND UNIT CUBE THAT
			Y = (int)floor(y) & 255,                  		// CONTAINS POINT.
			Z = (int)floor(z) & 255;
		x -= floor(x);                                	// FIND RELATIVE X,Y,Z
		y -= floor(y);                                	// OF POINT IN CUBE.
		z -= floor(z);
		double u = fade(x),                              // COMPUTE FADE CURVES
			v = fade(y),                                // FOR EACH OF X,Y,Z.
			w = fade(z);
		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),  // AND ADD
			grad(p[BA], x - 1, y, z)), // BLENDED
			lerp(u, grad(p[AB], x, y - 1, z),  // RESULTS
				grad(p[BB], x - 1, y - 1, z))),// FROM  8
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),  // CORNERS
				grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}
};
real perlin(real x, real y, real z)
{
	return ImprovedNoise::noise(x, y, z);
}

double fBm(double x, double y, double z, int octaves)
{
	double frequency = 1.0f;
	double amplitude = 0.5f;
	double sum = 0.0f;
	double gain = 0.50f;
	double lacunarity = 2.0f;

	for (int i = 0; i < octaves; i++)
	{
		sum += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
		//sum += amplitude * (1-fabs(perlin(x * frequency, y * frequency, z * frequency)));
		frequency *= lacunarity;
		amplitude *= gain;
	}
	return sum;
}
inline double ridge(double h, real offset)
{
	h = fabs(h);
	h = offset - h;
	h = h * h;
	return h;
}
double RidgedMF(double x, double y, double z, int octaves, real offset)
{
	double sum = 0;
	real amplitude = 0.5f;
	real frequency = 1.0f;
	double prev = 1.0f;

	double gain = 0.50f;
	double lacunarity = 2.5f;

	for (int i = 0; i < octaves; i++)
	{
		double n = ridge(perlin(x * frequency, y * frequency, z * frequency), offset);
		sum += n * amplitude * prev;
		prev = n;
		frequency *= lacunarity;
		amplitude *= gain;
	}

	return sum;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××
// Polygonising a scalar field
// 代码来自： http://paulbourke.net/geometry/polygonise/
// ××××××××××××××××××××××××××××××××××××××××××××××××××××××
#define XYZ vec
typedef struct {
	XYZ p[3];
} TRIANGLE;

typedef struct {
	XYZ p[8];
	double val[8];
} GRIDCELL;

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/

XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2)
{
	double mu;
	XYZ p;

	if (fabs(isolevel - valp1) < 0.00001)
		return(p1);
	if (fabs(isolevel - valp2) < 0.00001)
		return(p2);
	if (fabs(valp1 - valp2) < 0.00001)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return(p);
}
/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
	0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int Polygonise(GRIDCELL grid, double isolevel, TRIANGLE* triangles)
{
	int i, ntriang;
	int cubeindex;
	XYZ vertlist[12];

	static const int edgeTable[256] = {
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };
	static const int triTable[256][16] =
	{ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} };

	/*
	   Determine the index into the edge table which
	   tells us which vertices are inside of the surface
	*/
	cubeindex = 0;
	if (grid.val[0] < isolevel) cubeindex |= 1;
	if (grid.val[1] < isolevel) cubeindex |= 2;
	if (grid.val[2] < isolevel) cubeindex |= 4;
	if (grid.val[3] < isolevel) cubeindex |= 8;
	if (grid.val[4] < isolevel) cubeindex |= 16;
	if (grid.val[5] < isolevel) cubeindex |= 32;
	if (grid.val[6] < isolevel) cubeindex |= 64;
	if (grid.val[7] < isolevel) cubeindex |= 128;

	/* Cube is entirely in/out of the surface */
	if (edgeTable[cubeindex] == 0)
		return(0);

	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeindex] & 1)
		vertlist[0] =
		VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
	if (edgeTable[cubeindex] & 2)
		vertlist[1] =
		VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
	if (edgeTable[cubeindex] & 4)
		vertlist[2] =
		VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
	if (edgeTable[cubeindex] & 8)
		vertlist[3] =
		VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
	if (edgeTable[cubeindex] & 16)
		vertlist[4] =
		VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
	if (edgeTable[cubeindex] & 32)
		vertlist[5] =
		VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
	if (edgeTable[cubeindex] & 64)
		vertlist[6] =
		VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
	if (edgeTable[cubeindex] & 128)
		vertlist[7] =
		VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
	if (edgeTable[cubeindex] & 256)
		vertlist[8] =
		VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
	if (edgeTable[cubeindex] & 512)
		vertlist[9] =
		VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
	if (edgeTable[cubeindex] & 1024)
		vertlist[10] =
		VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
	if (edgeTable[cubeindex] & 2048)
		vertlist[11] =
		VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

	/* Create the triangle */
	ntriang = 0;
	for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
		triangles[ntriang].p[0] = vertlist[triTable[cubeindex][i]];
		triangles[ntriang].p[1] = vertlist[triTable[cubeindex][i + 1]];
		triangles[ntriang].p[2] = vertlist[triTable[cubeindex][i + 2]];
		ntriang++;
	}

	return(ntriang);
}

//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
// otaviogood's noise from https://www.shadertoy.com/view/ld2SzK
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
// This spiral noise works by successively adding and rotating sin waves while increasing frequency.
// It should work the same on all computers since it's not based on a hash function like some other noises.
// It can be much faster than other noise functions if you're ok with some repetition.
const float nudge = 0.739513;	// size of perpendicular vector
float normalizer = 1.0 / sqrt(1.0 + nudge * nudge);	// pythagorean theorem on that perpendicular to maintain scale
float SpiralNoiseC(vec3 p)
{
	float n = 0.0;	// noise amount
	float iter = 1.0;
	for (int i = 0; i < 8; i++)
	{
		// add sin and cos scaled inverse with the frequency
		n += -abs(sin(p.y * iter) + cos(p.x * iter)) / iter;	// abs for a ridged look
		// rotate by adding perpendicular and scaling down
		p.xy(p.xy() + vec2(p.y, -p.x) * nudge);
		p.xy(p.xy() * normalizer);
		// rotate on other axis
		p.xz(p.xz() + vec2(p.z, -p.x) * nudge);
		p.xz(p.xz() * normalizer);
		// increase the frequency
		iter *= 1.733733;
	}
	return n;
}

float SpiralNoise3D(vec3 p)
{
	float n = 0.0;
	float iter = 1.0;
	for (int i = 0; i < 5; i++)
	{
		n += (sin(p.y * iter) + cos(p.x * iter)) / iter;
		p.xz(p.xz() + vec2(p.z, -p.x) * nudge);
		p.xz(p.xz() * normalizer);
		iter *= 1.33733;
	}
	return n;
}

float NebulaNoise(vec3 p)
{
	float final = p.y + 4.5;
	final -= SpiralNoiseC(p);   // mid-range noise
	final += SpiralNoiseC(p.zxy() * 0.5123 + vec3(100.0, 100.0, 100.0)) * 4.0;   // large scale features
	final -= SpiralNoise3D(p);   // more large scale features, but 3d

	return final;
}

float map(vec3 p)
{
#ifdef ROTATION
	R(p.xz, iMouse.x * 0.008 * pi + iTime * 0.1);
#endif

	float NebNoise = abs(NebulaNoise(p / 0.5) * 0.5);

	return NebNoise + 0.03;
}

//--------------------------------------------------------------
// iq's noise
//--------------------------------------------------------------
float hash0(float n) { return fract(sin(n) * 753.5453123); }
float noise(vec3 x)
{
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f * f * (vec3(3.0, 3.0, 3.0) - f * 2.0);

	float n = p.x + p.y * 157.0 + 113.0 * p.z;
	return mix(mix(mix(hash0(n + 0.0), hash0(n + 1.0), f.x),
		mix(hash0(n + 157.0), hash0(n + 158.0), f.x), f.y),
		mix(mix(hash0(n + 113.0), hash0(n + 114.0), f.x),
			mix(hash0(n + 270.0), hash0(n + 271.0), f.x), f.y), f.z);
}

// **********************************************************************
// shape blend
// **********************************************************************
int blendi(int h1, int h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return int(h1 * (1.0f - alpha) + h2 * alpha);
}
real blend(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return h1 * (1.0f - alpha) + h2 * alpha;
}
vector2 blend(const vector2& v1, const vector2& v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
vector3 blend(const vector3& v1, const vector3& v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}

// ------------------------------------------------
real blend2(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
real blendn(real h1, real h2, real alpha, int n, real power)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	/*
	alpha = alpha * n - int(alpha * n);
	if (alpha < 0.5)
	{
		alpha = (0.5 - alpha) / 0.5;

	}
	else
	{
		alpha = (alpha - 0.5) / 0.5;
	}*/
	alpha = pow(fabs(cos(alpha * n * PI)), power);

	return h2 * (1 - alpha) + h1 * alpha;
}
// ------------------------------------------------
vector2 blend2(vector2 h1, vector2 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
vector3 blend2(vector3 h1, vector3 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

real blend3(real h1, real h2, real alpha, real mid = 0.5, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < mid)
	{
		alpha = (mid - alpha) / mid;
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - mid) / (1 - mid);
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

real blend4(real h1, real h2, real alpha, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	real mid = 0.5;
	if (alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}

real blend5(real h1, real h2, real alpha, real mid = 0.5, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}
inline real clamp(real v, real lo, real hi)
{
	ASSERT(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
inline int clampi(int v, int lo, int hi)
{
	ASSERT(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
// ------------------------------------------------------------------------------------------------
vec3 bezier2(vec3 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec3 bezier3(vec3 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier2(vec2 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier2(vec2 p1, vec2 p2, vec2 p3, real t)
{
	return p1 * ((1 - t) * (1 - t)) + p2 * (2 * t * (1 - t)) + p3 * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier3(vec2 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
vec bezier2(vec p1, vec p2, vec p3, real t)
{
	return p1 * ((1 - t) * (1 - t)) + p2 * (2 * t * (1 - t)) + p3 * (t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier2(real cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier3(real cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// -----------------------------------------------------------------------
inline real roundblend(real h1, real h2, real alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	//alpha = sqrt(1 - (1-alpha)*(1-alpha));
	alpha = sqrt(1 - (alpha) * (alpha));
	return h2 * (1.0f - alpha) + h1 * alpha;

}
// -----------------------------------------------------------------------
inline real roundblend2(real h1, real h2, real alpha, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (power != 1)
		alpha = pow(alpha, power);

	if (alpha < 0.5)
	{
		alpha = alpha * 2;
		alpha = sqrt(1 - (1 - alpha) * (1 - alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5) * 2;
		alpha = sqrt(1 - (alpha) * (alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}

}

// -----------------------------------------------------------------------
// 三角函数插值
inline real BlendSin(real h1, real h2, real alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = sin(alpha * PI / 2);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
// 傅立叶级数
inline real FT(real angle, real t[] = 0, real dt = 0)
{
	if (t == 0)
	{
		static real s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		t = s_t0;
	}

	real yy = 0;
	yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.5 * sin(2 * angle + (t[1] + dt) * PI);
	yy += 0.25 * sin(4 * angle + (t[2] + dt) * PI);
	yy += 0.125 * sin(8 * angle + (t[3] + dt) * PI);

	return yy;
}
// -----------------------------------------------------------------------
inline real FTU(real ang, real t[] = 0, real dt = 0)
{
	real ft = FT(ang, t, dt);
	real max1 = (1 + 0.5 + 0.25 + 0.125);
	real min1 = -max1;
	return (ft - min1) / (max1 - min1);
}
// -----------------------------------------------------------------------
inline real BlendFT(real h1, real h2, real alpha, real t[] = 0, real dt = 0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = FTU(alpha, t, dt);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
inline real FT2D1(real anglex, real angley, real tx[] = 0, real ty[] = 0, real dtx = 0, real dty = 0)
{
	if (tx == 0)
	{
		static real s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static real s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	real yy = 0;
	yy += 1 * sin(1 * anglex + (tx[0] + dtx) * PI) + 1 * sin(1 * angley + (ty[0] + dty) * PI);
	yy += 0.5 * sin(2 * anglex + (tx[1] + dtx) * PI) + 0.5 * sin(2 * angley + (ty[1] + dty) * PI);
	yy += 0.25 * sin(4 * anglex + (tx[2] + dtx) * PI) + 0.25 * sin(4 * angley + (ty[2] + dty) * PI);
	yy += 0.125 * sin(8 * anglex + (tx[3] + dtx) * PI) + 0.125 * sin(8 * angley + (ty[3] + dty) * PI);

	return yy;
}
// ------------------------------------------------------------------------------------------------
real FFT(real ax, real ay, real rndmap[1024][1024])
{
	real ft = 0;
	for (int i = 0; i < 8; i++)
	{
		real dz = blend(ax, ay, rndmap[100][i], 2);
		{
			real A = 2 * rndmap[0][i];
			real F = blend(20, 50, rndmap[1][i]);
			real T0 = 2 * PI * rndmap[2][i];
			ft += A * (0.5 + 0.5 * sin(T0 + F * dz * PI * 2));
		}
	}
	return ft / (8);
}

// -----------------------------------------------------------------------
inline real FT2D(real anglex, real angley, real tx[] = 0, real ty[] = 0, real dtx = 0, real dty = 0)
{
	static real s_fx[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	static real s_fy[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	if (tx == 0)
	{
		static real s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static real s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	real yy = 0;
	yy += 1 * sin(s_fx[0] * anglex + (tx[0] + dtx) * PI + s_fy[0] * angley + (ty[0] + dty) * PI);
	yy += .5 * sin(s_fx[1] * anglex + (tx[1] + dtx) * PI + s_fy[1] * angley + (ty[1] + dty) * PI);
	yy += .25 * sin(s_fx[2] * anglex + (tx[2] + dtx) * PI + s_fy[2] * angley + (ty[2] + dty) * PI);
	yy += .125 * sin(s_fx[3] * anglex + (tx[3] + dtx) * PI + s_fy[3] * angley + (ty[3] + dty) * PI);

	return yy;
}

// ------------------------------------------------
// 2d
real blend2d(real h1, real h2, real alphaX, real alphaY)
{
	int size;
	real alpha;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;
	size = IMAGESCALE;
	alpha = fheightmap[(int)(alphaX * size + 0.5) % size][(int)(alphaY * size + 0.5) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return h1 * (1 - alpha) + h2 * alpha;
}
// **********************************************************************
// color blend
// **********************************************************************
inline int blendcor(int c1, int c2, real alpha, real power = 1.0)
{
	//alpha = alpha != 1 ? abs(alpha) - (int)(alpha) : 1;	
	alpha = alpha > 1 ? 1 : alpha;
	alpha = alpha < 0 ? 0 : alpha;

	if (power != 1.0)
		alpha = pow(alpha, power);
	return _RGB(GetRValue(c2) * alpha + GetRValue(c1) * (1 - alpha),
		GetGValue(c2) * alpha + GetGValue(c1) * (1 - alpha),
		GetBValue(c2) * alpha + GetBValue(c1) * (1 - alpha)
	);

}
inline int blendcorRGBA(int c1, int c2, real alpha, real power = 1.0)
{
	//alpha = alpha != 1 ? abs(alpha) - (int)(alpha) : 1;
	alpha = alpha > 1 ? 1 : alpha;
	alpha = alpha < 0 ? 0 : alpha;

	if (power != 1.0)
		alpha = pow(alpha, power);
	return RGBA(GetRValue(c2) * alpha + GetRValue(c1) * (1 - alpha),
		GetGValue(c2) * alpha + GetGValue(c1) * (1 - alpha),
		GetBValue(c2) * alpha + GetBValue(c1) * (1 - alpha),
		GetAValue(c2) * alpha + GetAValue(c1) * (1 - alpha)
	);

}

// ------------------------------------------------
// 2d
inline int blendcor2d(int c1, int c2, real alphaX, real alphaY, real power = 1.0)
{
	real alpha;
	int size = IMAGESCALE;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;

	alpha = fheightmap[(int)(alphaX * size) % size][(int)(alphaY * size) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return blendcor(c1, c2, alpha, power);
}

// color add

inline int addcor(int c1, int c2)
{
	int r = GetRValue(c1) + GetRValue(c2);
	int g = GetGValue(c1) + GetGValue(c2);
	int b = GetBValue(c1) + GetBValue(c2);
	return RGB(r > 255 ? 255 : r,
		g > 255 ? 255 : g,
		b > 255 ? 255 : b);

}
// mixorder

inline void mixorder(char ar[4])
{
	for (int i = 0; i < 4; i++)
	{
		char a = rand() % 4;
		char b = rand() % 4;

		char tm = ar[b];
		ar[b] = ar[a];
		ar[a] = tm;
	}
}

inline int rgb(const vec3& v)
{
	return _RGB(int(v.x * 255), int(v.y * 255), int(v.z * 255));
}
// **********************************************************************
// projection
// **********************************************************************
inline vector2 viewprjold(const vector3& p)
{
	real vpf = blend(1, 0.1, p.z / 16.0);
	return vector2(
		0.5 + (p.x - 0.5) * vpf,
		0.95 + (p.y - 0.95) * vpf
	);
}
extern inline void viewprj(const vector3& rp, vector2& p2d, real& depth);
// **********************************************************************
// common draw
// **********************************************************************
// pixel
inline bool pixel(int ix, int iy, int color, real alpha = 1, real depth = 0)
{
	if (ix < 0 || iy < 0 || ix >= IMAGESCALE || iy >= IMAGESCALE)
		return false;

	if (//depth >= 0 && 
		(depthmap[ix][iy] == 0 || depth <= depthmap[ix][iy])
		)
	{
		//if (alpha >= 1)
		//{
		drawmap[ix][iy] = color;
		depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
	//}
	//else
	//{
		//drawmap[ix][iy] = blendcor(drawmap[ix][iy], color, alpha, 1);
		//depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
	//}
		return true;
	}

	return false;
}
inline bool pixeladd(int ix, int iy, int color)
{
	if (ix < 0 || iy < 0 || ix >= IMAGESCALE || iy >= IMAGESCALE)
		return false;

	drawmap[ix][iy] |= color;

	return true;
}

inline bool pixel(real x, real y, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int((x + 0.5) * IMAGESCALE), int((y + 0.5) * IMAGESCALE), color, alpha, depth);
}
inline bool pixel(const vector2& p, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int((p.x + 0.5) * IMAGESCALE + 0.5), int((p.y + 0.5) * IMAGESCALE + 0.5), color, alpha, depth);
}
inline bool pixel(const vector3& p, int color = 0xFFFFFFFF, real alpha = 1.0)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixel(p2d, color, alpha, depth);
}
// 抗锯齿算法
inline bool pixelAA(real x, real y, int color = 0xFFFFFFFF, real depth = 0)
{
	x = x * IMAGESCALE;
	y = y * IMAGESCALE;
	int ix = int(x);
	int iy = int(y);
	real fx = x - ix;
	real fy = y - iy;

	if (ix < 0 || iy < 0 || ix >= IMAGESCALE - 1 || iy >= IMAGESCALE - 1)
		return false;
	real& rdepthval = depthmap[int(x + 0.5)][int(y + 0.5)];
	if (depth >= 0 &&
		(rdepthval == 0 || depth <= rdepthval)
		)
	{
		drawmap[ix][iy] = blendcor(drawmap[ix][iy], color, (1 - fx) * (1 - fy));
		drawmap[ix + 1][iy] = blendcor(drawmap[ix + 1][iy], color, (fx) * (1 - fy));
		drawmap[ix][iy + 1] = blendcor(drawmap[ix][iy + 1], color, (1 - fx) * (fy));
		drawmap[ix + 1][iy + 1] = blendcor(drawmap[ix + 1][iy + 1], color, (fx) * (fy));

		drawmap[int(x + 0.5)][int(y + 0.5)] = color;
		rdepthval = depth < 0 ? 0 : depth;
		return true;
	}
	return false;
}
inline bool pixelAA(const vector2& p, int color, real depth = 0)
{
	return pixelAA(p.x, p.y, color, depth);
}
inline bool pixelAA(const vector3& p, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixelAA(p2d, color, depth);
}

// point
inline void pointi(int x0, int y0, int size, int color, real alpha = 1, real depth = 0)
{
	if (size == 0)
		return;

	//if (!pixel(x0, y0, color, 1, depth))
	//	return;

	if (size == 1)
	{
		pixel(x0, y0, color, 1, depth);
		return;
	}

	int halfsize = size / 2;
	real dhalfsize = halfsize * halfsize;// blend(0, halfsize * halfsize, alpha1, alpha1 * 4);
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
		{
			real sqrdis = (i - x0) * (i - x0) + (j - y0) * (j - y0);
			real ds = (1 - sqrdis / dhalfsize);
			if (ds > 0)
			{
				//if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
					//drawmap[i][j] = blendcor(drawmap[i][j], color, ds * alpha);
				drawmap[i][j] = color;
			}
		}
}
inline void point(real x0, real y0, int size, int color, real alpha = 1, real depth = 0)
{
	pointi(((x0 + 0.5) * IMAGESCALE), ((y0 + 0.5) * IMAGESCALE), (size), color, alpha, depth);
}
inline void point(const pnt& p, int size = 4, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void pointi(const pnt& p, int size = 4, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void pointi(const vector2& p, int size, int color, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void point(const vector2& p, int size, int color, real alpha = 1, real depth = 0)
{
	point(p.x, p.y, size, color, alpha, depth);
}
inline void point(const vector3& p, int size, int color, real alpha = 1)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	point(p2d, size, color, alpha, depth);
}

inline void pointi_sharp(int x0, int y0, int size, int color, real depth = 0)
{
	if (size == 0)
		return;

	if (!pixel(x0, y0, color, 1, depth))
		return;

	if (size == 1)
		return;

	int halfsize = size / 2;
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
		{
			real sqrdis = (i - x0) * (i - x0) + (j - y0) * (j - y0);
			real ds = (1 - sqrdis / (halfsize * halfsize));
			if (ds > 0)
			{
				if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
				{
					drawmap[i][j] = color;// blendcor(drawmap[i][j], color, ds, 0.25 + (1 - ds) * 2);
					depthmap[i][j] = depth;
				}
			}
		}
}
inline void point_sharp(real x0, real y0, int size, int color, real depth = 0)
{
	pointi_sharp(int(x0 * IMAGESCALE + 0.5), int(y0 * IMAGESCALE + 0.5), size, color, depth);
}
inline void point_sharp(const vector2& p, int size, int color, real depth = 0)
{
	point_sharp(p.x, p.y, size, color, depth);
}
inline void point_sharp(const vector3& p, int size, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	point_sharp(p2d, size, color, depth);
}
// -------------------------------------------------
// viewprj
// -------------------------------------------------
inline void viewprj(const vector3& rp, vector2& p2d, real& depth)
{
	//vec p = rp;
	//p -= CAM_POS;
	//p.rot(PI * 0.25, vec::UY);
	//p.rot(PI * 0.15, vec::UX);
	//p += CAM_POS;
	//p.z += MAXZ / 3;

	real z = rp.z + 10;
	depth = z;
	real vpf = blend(1, 0.01, z / 100.0f);
	p2d.x = rp.x * vpf;
	p2d.y = rp.y * vpf;
}
inline real getviewdis(const vec& p1, const vec& p2)
{
	real depth;
	vec2 pp1, pp2;
	viewprj(p1, pp1, depth);
	viewprj(p2, pp2, depth);
	return (pp1 - pp2).len() * (IMAGESCALE);
}
// line
void line3d(vec p1, vec p2, int cor = 0xFFFFFFFF)
{
	int len = getviewdis(p1, p2);
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}
void line2d(vec2 p1, vec2 p2, int cor = 0xFFFFFFFF)
{
	int len = (p2 - p1).len() * IMAGESCALE;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}
void line2di(const pnt& p1, const pnt& p2, int size, int cor = 0xFFFFFFFF)
{
	int len = (p2 - p1).len();
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pointi(blend(p1.x, p2.x, ai), blend(p1.y, p2.y, ai), size, cor);
	}
}
void circle(vec2 o, real r, int cor = 0xFFFFFFFF)
{
	int len = (r * 2 * PI) * IMAGESCALE;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = ai * PI * 2;
		pixel(o + vec2(r * cos(ang), r * sin(ang)), cor);
	}
}
void drawrect(const pnt& p1, const pnt& p2, int cor = 0xFFFFFFFF)
{
	for (int i = p1.x; i <= p2.x; i++)
		for (int j = p1.y; j <= p2.y; j++)
		{
			pixel(i, j, cor);
		}
}
void drawrect(int x1, int y1, int x2, int y2, int cor = 0xFFFFFFFF)
{
	for (int i = x1; i <= x2; i++)
	{
		pixel(i, y1, cor);
		pixel(i, y2, cor);
	}
	for (int j = y1; j <= y2; j++)
	{
		pixel(x1, j, cor);
		pixel(x2, j, cor);
	}
}
// **********************************************************************
// 几何体造型基础
// **********************************************************************
struct vertex
{
	vec3 p;
	vec3 n;
	vec2 uv;

	int cor = 0;
	int ind = -1;

	// 标记
	int tag = 0;
	bool bread = false;

	// connect to next
	int next = -1; // -1 没有链接 -2 下一点 >= 0 连接位置

	// deform
	real deformwgt = 0;
	vec3 bone;

	real val = 0;

	vec3 preD; // 前差（p - p0)

	vertex()
	{
		cor = 0;
		ind = -1;
	}
	vertex(const vec& _p)
	{
		p = _p;
		ind = -1;
	}
	vertex(real x, real y, real z)
	{
		p.x = x, p.y = y, p.z = z;
		ind = -1;
	}
	vertex(const vec& _p, const vec& _n)
	{
		p = _p;
		n = _n;
		ind = -1;
	}
	vertex(const vec& _p, int _ind)
	{
		p = _p;
		ind = _ind;
	}
	vertex(const vec& _p, real _val)
	{
		p = _p;
		val = _val;
	}
	vertex(const vec& _p, int _ind, int _tag)
	{
		p = _p;
		ind = _ind;
		tag = _tag;
	}
	vertex(const vec& _p, const vec2& _uv)
	{
		p = _p;
		uv = _uv;
		ind = -1;
	}
	operator const vec& ()
	{
		return p;
	}
	bool operator==(const vertex& v) const
	{
		return p == v.p;
	}
	vec operator + (const vertex& v) const
	{
		return p + v.p;
	}
	vec operator + (const vec3& _p) const
	{
		return p + _p;
	}
	vec operator - (const vertex& v) const
	{
		return p - v.p;
	}
	vec operator - (const vec3& _p) const
	{
		return p - _p;
	}
	vertex operator * (real s) const
	{
		return vertex(p * s, ind);
	}
};
struct triangle
{
	int	vertexIndex[3];
	int	normalIndex[3]; // 读入数据时使用

	bool bread = false;
};
struct submesh
{
	std::vector<vertex> vertices;
	std::vector<int> cors;
	std::vector<triangle> tris;

	// 后边两项在读入模型时才会使用
	std::vector<vector3> normals;
	std::vector<vector2> uvs;

	submesh() {}
	submesh(const submesh& sm)
	{
		PRINT("submesh copy:" << sm.vertices.size())
	}
	void operator = (const submesh& sm)
	{
		PRINT("[submesh=" << sm.vertices.size() << "]")
			vertices = sm.vertices;
		cors = sm.cors;
		tris = sm.tris;
		normals = sm.normals;
		uvs = sm.uvs;
	}
};
// -------------------------------------------------
inline void drawline(vec3 p1, vec3 p2, int  cor = 0xFFFFFFFF)
{
	for (int i = 0; i < 1000; i++)
	{
		vec3 p = blend(p1, p2, i / 1000.0);
		vec2 p2d;
		real depth;
		viewprj(p, p2d, depth);
		pixel(p2d, cor, depth);
	}
}
inline void drawline(crvec2 p1, crvec2 p2, int cor = 0xFFFFFFFF)
{
	for (int i = 0; i < 1000; i++)
	{
		vec2 p = blend(p1, p2, i / 1000.0);
		pixel(p, cor);
	}
}

inline void drawlinei(crvec2 p1, crvec2 p2, int cor = 0xFFFFFFFF)
{
	int len = (p1 - p2).len();
	for (int i = 0; i < len - 8; i++)
	{
		vec2 p = blend(p1, p2, i / real(len));
		pointi(p, 2, cor);
	}
}
// -------------------------------------------------
void drawtriangle(const vec& p1, const vec& p2, const vec& p3, int cor = 1)
{
	if (renderstate == 1)
	{
		static vec light(0.25, 1, -1);
		vec norm = (p2 - p1).cross(p3 - p1);
		norm = norm.normcopy();
		if (norm.sqrlen() == 0)
			return; // 无效三角形	

		int cor0 = rrnd(50, 80);//0x000000FF & cor;
		{
			int lum = blend(cor0, 255, norm.dot(light));
			cor = RGB(lum, lum, lum);
		}

		LOOP(i, len, getviewdis(p1, p2))
			vec p12 = blend(p1, p2, ai);
		LOOP(j, wid, getviewdis(p12, p3))
			vec p = blend(p12, p3, aj);
		pixelAA(p, cor);
		END
			END
	}
	//else if(renderstate == 2)
	{
		line3d(p1, p2, 0xFFFFFFFF);
		line3d(p2, p3, 0xFFFFFFFF);
		line3d(p3, p1, 0xFFFFFFFF);
	}
}
// -------------------------------------------------
void rendersubmesh(const submesh& sm)
{
	const std::vector<vertex>& vertices = sm.vertices;
	const std::vector<triangle>& tris = sm.tris;

	for (int i = 0; i < tris.size(); i++)
	{
		const triangle& tri = tris[i];
		//if(!tri.enable)
		//	continue;
		vec pos[3];
		for (int ii = 0; ii < 3; ii++)
		{
			int ind = tri.vertexIndex[ii];
			pos[ii] = vertices[tri.vertexIndex[ii]].p;
		}
		drawtriangle(pos[0], pos[2], pos[1]);
	}
}

// -------------------------------------------------
bool loadsubmesh3DS(const char* fileName, submesh& sm)
{
	FILE* file;
	if (file = (fopen(fileName, "r")))
	{
		printf("File can't be opened\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0)
		{
			vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			sm.vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			sm.uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			sm.normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			triangle tri;
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2]);
			if (matches != 6)
				//int matches = fscanf(file, "%d//%d//%d %d//%d//%d %d//%d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.uvIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.uvIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2], &tri.uvIndex[2]);
				//if (matches != 9)			
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			//PRINT(tri.vertexIndex[2])
			sm.tris.push_back(tri);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);

	return true;
}

// -------------------------------------------------
bool savesubmesh3DS(const char* fileName, submesh& sm)
{
	FILE* file;
	if (file = (fopen(fileName, "wb")))
	{
		printf("File can't be opened\n");
		return false;
	}
	std::stringstream ss;
	ss << "o Submesh\n";
	for (auto v : sm.vertices)
	{
		ss << "v " << v.p.x << " " << v.p.y << " " << v.p.z << "\n";
	}
	for (auto v : sm.vertices)
	{
		ss << "vn " << v.n.x << " " << v.n.y << " " << v.n.z << "\n";
	}
	ss << "s off\n";

	for (auto f : sm.tris)
	{
		ss << "f "
			<< f.vertexIndex[0] + 1 << "//" << f.vertexIndex[0] + 1 << " "
			<< f.vertexIndex[1] + 1 << "//" << f.vertexIndex[1] + 1 << " "
			<< f.vertexIndex[2] + 1 << "//" << f.vertexIndex[2] + 1
			<< "\n";
	}

	fprintf(file, ss.str().c_str());

	fclose(file);

	return true;
}

// **********************************************************************
// 三角形导出
// **********************************************************************
#define SMCATCH_START(wid)	submesh sm;	triangle tri1, tri2; int verIndex = 0;int lowerind[wid + 1];	
#define SMCATCH_ING(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[2] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[0] = verIndex - 1;tri2.vertexIndex[2] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[0] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SMCATCH_ING2(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[0] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[2] = verIndex - 1;tri2.vertexIndex[0] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[2] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SWITCHSM(sm)	gsubmesh = &sm
#define RESTORESM		gsubmesh = &gsubmesh0
#define SUBMESH			(*gsubmesh)

submesh gsubmesh0;	// 默认模型
submesh gimportsm;	// unity导入的模型
submesh* gsubmesh = &gsubmesh0;

bool gcommonvertex = false;
bool gsearchcomvertex = false;
int gverindfindstart = 0;
bool gcalcnorm = false;

struct targetpos_t
{
	targetpos_t(crvec p)
	{
		pos = p;
	}
	targetpos_t(crvec p, crvec v)
	{
		pos = p;
		dir = v;
	}
	vec pos;
	vec dir;
};
std::vector<targetpos_t>	gtargets;
extern uint color;

// -------------------------------------------------
inline void resetsm(submesh* sm = gsubmesh)
{
	sm->vertices.clear();
	sm->cors.clear();
	sm->tris.clear();

	// 后边两项在读入模型时才会使用
	sm->normals.clear();
	sm->uvs.clear();

	gtargets.clear();
}
inline void clearedgeind(VECLIST& e)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].ind = -1;
	}
}
inline int getverind(vertex& v)
{
	if (gcommonvertex && v.ind != -1)
	{
		gsubmesh->cors[v.ind] = color;
		return v.ind;
	}

	if (gcommonvertex && gsearchcomvertex)
	{
		// 寻找同样的顶点
		for (int i = gsubmesh->vertices.size() - 1; i >= gverindfindstart; i--)
		{
			if (gsubmesh->vertices[i] == v.p)
			{
				gsubmesh->cors[i] = color;
				v.ind = i;
				return i;
			}
		}
	}

	int ind = gsubmesh->vertices.size();
	v.ind = ind;
	gsubmesh->vertices.push_back(v);
	gsubmesh->cors.push_back(color);
	return ind;
}

inline void addtri2sm(vertex& p1, vertex& p2, vertex& p3)
{
	//if (*gsubmesh->vertices.size() >= 60000)
	//{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//
	//	return;
	//}

	//vec norm = (p2 - p1).cross(p3 - p1);
	//real llen = norm.sqrlen();
	//if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind(p1);
		tri.vertexIndex[1] = getverind(p2);
		tri.vertexIndex[2] = getverind(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline int getverind0(const vertex& v)
{
	gsubmesh->vertices.push_back(v.p);
	gsubmesh->cors.push_back(color);
	return gsubmesh->vertices.size() - 1;
}
inline void addtri2sm0(const vertex& p1, const vertex& p2, const vertex& p3)
{
	//if (gsubmesh->vertices.size() >= 60000)
	//{
	//	//LOGMSG("添加失败 三角形数量超过60000!")
	//	//
	//	return;
	//}

	vec norm = (p2 - p1).cross(p3 - p1);
	real llen = norm.sqrlen();
	if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind0(p1);
		tri.vertexIndex[1] = getverind0(p2);
		tri.vertexIndex[2] = getverind0(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline void addtri2smi(int t1, int t2, int t3)
{
	if (t1 >= gsubmesh->vertices.size() || t2 >= gsubmesh->vertices.size() || t3 >= gsubmesh->vertices.size())
		return;

	if (gsubmesh->vertices.size() >= 60000)
	{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//
		return;
	}

	triangle tri;
	tri.vertexIndex[0] = t1;
	tri.vertexIndex[1] = t2;
	tri.vertexIndex[2] = t3;

	gsubmesh->tris.push_back(tri);
}
inline void exportsm()
{
	//exportdat(*gsubmesh);
}
inline void rendersubmesh()
{
	rendersubmesh(*gsubmesh);
}

// -------------------------------------------------
// 法线逻辑
// -------------------------------------------------
inline void uniformnorm(const vec& p, const vec& n)
{
	for (int i = 0; i < gsubmesh->vertices.size(); i++)
	{
		if (gsubmesh->vertices[i] == p)
		{
			gsubmesh->vertices[i].n = n;
		}
	}
}
inline void closeedgenorm(VECLIST& e)
{
	e[e.size() - 1].n = e[0].n;
}
void calcroundnorm(VECLIST& e, crvec o)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].n = (e[i].p - o).normcopy();
	}
}

// **********************************************************************
#ifdef PMDLL
// ----------------------------------------------------------------------
// Export
// **********************************************************************
extern submesh* gsubmesh;
extern void resetsm(submesh* sm);
extern void exportsm();
extern vec getedgenorm(const VECLIST& e);
extern vec getedgecenter(const VECLIST& e, int n);
extern void drawScene(const char* script);
extern void init();
extern void run(const char* script);
extern void triang0(const vertex& p1, const vertex& p2, const vertex& p3);
extern void plane0(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4);
extern int editface(int type, VECLIST& e, const vec& startpos, const vec& updir, real hitdis);

vec gettrinorm(const vec& p1, const vec& p2, const vec& p3)
{
	return (p2 - p1).cross((p3 - p1)).normcopy();
}

// ---------------------------------------------------------
// 三角形雕塑
// ---------------------------------------------------------
vec getpointnorm(vec* v, int* t, int tcnt, vec p1)
{
	VECLIST normlist;
	for (int i = 0; i < tcnt; i += 3)
	{// 每一个三角形
		vec& _p1 = v[t[i]];
		vec& _p2 = v[t[i] + 1];
		vec& _p3 = v[t[i] + 2];
		{// 寻找位置相同的点并移动它
			if ((_p1 == p1) || (_p2 == p1) || (_p3 == p1))
			{
				normlist.push_back((_p2 - _p1).cross(_p3 - _p1).normcopy());
			}
		}
	}
	vec norm = vec::ZERO;
	for (int i = 0; i < normlist.size(); i++)
	{
		norm += normlist[i];
	}
	norm /= normlist.size();
	return norm.normcopy();
}
// **********************************************************************
#include "export.hpp"
#ifdef DYM
// **********************************************************************
// PNG
// **********************************************************************
struct PNG
{
	CDymPng png;
	uint getcolor(int x, int y)
	{
		if (png.Width() > 0)
		{
			x = clampi(x, 0, png.Width() - 1);
			y = clampi(y, 0, png.Height() - 1);
			y = png.Height() - 1 - y; // 上下颠倒

			unsigned int cor = png.gifGetPixel(x, y);
			return GetRValue(cor);
		}
		return 0;
	}
	int getcolor(const pnt& p)
	{
		if (png.Width() > 0)
		{
			int x = clampi(p.x, 0, png.Width() - 1);
			int y = clampi(p.y, 0, png.Height() - 1);

			return getcolor(x, y);
		}

		return 0;
	}
	int getcolor(const vec2& p)
	{
		if (png.Width() > 0)
		{
			int x = clampi(p.x, 0, png.Width() - 1);
			int y = clampi(p.y, 0, png.Height() - 1);

			return getcolor(x, y);
		}

		return 0;
	}
	bool load(crstr filename)
	{
		PRINT("loading " << filename)
			if (png.Width() == 0)
				return 0 != png.LoadFromFileA(filename.c_str());
		return true;
	}
};
#endif
// **********************************************************************
// read/write BMP
// **********************************************************************
int bmp_read(unsigned char* image, const char* filename)
{
	FILE* fp;
	unsigned char header[54];

	if (fp = (fopen(filename, "rb")))
	{
		PRINT("file not exist!\n");
		return -1;
	}

	fread(header, sizeof(unsigned char), 54, fp);
	fread(image, sizeof(unsigned char), (size_t)(long)2048 * 2048 * 3, fp);

	fclose(fp);
	return 0;
}
int bmp_write(unsigned char* image, int xSize, int ySize, const char* fileName)
{
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};

	// sizeof(long) = 4  
	long fileSize = (long)xSize * (long)ySize * 3 + 54;     // image array size + 54  
	long temp = 0;                      // temp for byte convertion  
	long width = xSize;                 // image width  
	long height = ySize;                // image height 
	int i = 0;                          // loop variable  

	FILE* fp;

	for (i = 0; i != 4; ++i)                // write fileSize from byte2 ~ byte 6  
	{
		header[2 + i] = (unsigned char)(fileSize >> (8 * i)) & 0x000000ff;
	}
	for (i = 0; i != 4; ++i)                // write width from byte18 ~ byte 22  
	{
		header[18 + i] = (unsigned char)(width >> (8 * i)) & 0x000000ff;
	}
	for (i = 0; i != 4; ++i)                // write height from byte22 ~ byte 26  
	{
		header[22 + i] = (unsigned char)(height >> (8 * i)) & 0x000000ff;
	}


	if (!(fp = fopen(fileName, "wb")))
	{
		PRINT("bmp_write openfile " << fileName << " failed!\n")
			return -1;
	}

	fwrite(header, sizeof(unsigned char), 54, fp);                              // write header  
	fwrite(image, sizeof(unsigned char), xSize * ySize * 3, fp);       // write image array  

	fclose(fp);

	return 0;
}

// **********************************************************************
// DRAW
// **********************************************************************
#ifdef  WIN
inline std::string getExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	return std::string(szFilePath);
}
#else
//
//int GetModuleFileName1(char* sModuleName, char* sFileName, int nSize)
//{
//	int ret = -1;
//	if (strchr(sModuleName, '/') != NULL)
//		strcpy(sFileName, sModuleName);
//	else
//	{
//		char* sPath = getenv("PATH");
//		char* pHead = sPath;
//		char* pTail = NULL;
//		while (pHead != NULL && *pHead != '/x0')
//		{
//			pTail = strchr(pHead, ':');
//			if (pTail != NULL)
//			{
//				strncpy(sFileName, pHead, pTail - pHead);
//				sFileName[pTail - pHead] = '/x0';
//				pHead = pTail + 1;
//			}
//			else
//			{
//				strcpy(sFileName, pHead);
//				pHead = NULL;
//			}
//
//			int nLen = strlen(sFileName);
//			if (sFileName[nLen] != '/')sFileName[nLen] = '/';
//			strcpy(sFileName + nLen + 1, sModuleName);
//			if (0 == access(sFileName, F_OK))
//			{
//				ret = 0;
//				break;
//			}
//		}
//	}
//	return ret;
//}
//inline std::string getExePath(void)
//{
//	char szFilePath[260 + 1] = { 0 };
//	GetModuleFileName1(NULL, szFilePath, 260);
//	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
//	return std::string(szFilePath);
//}
#endif //  WIN
#ifdef  SERVER_WIN
extern int servermain(int num);
#endif
void drawScene(const char* script)
{
#ifdef  SERVER_WIN
	PRINT("######### start http-server: 127.0.0.1:8080 #########");
	std::thread serverthread = std::thread{ servermain, 100 };
	SetCurrentDirectoryA(getExePath().c_str());
#else
	//system((std::string("cd ") + getExePath()).c_str());
#endif //  WIN

	char imgname[256];
	srand(18858146);

	init();

	int steps = 1;
	for (int i = 1; i <= steps; i++)
	{
		PRINT("@@@@@@@@@@@@@@@@@@@@@@ ROUND(" << i << "/" << steps << ") @@@@@@@@@@@@@@@@@@@@@@");
		if (renderstate > 0)
		{
			// clear
			memset(drawmap0, 0, sizeof(drawmap0));
			memset(depthmap0, 0, sizeof(depthmap0));

			//printf("generating image ... ");
		}

		run(script);

		//PRINT("\n");

		if (renderstate > 0)
		{
			//printf("\ndone!\n");

			printf("\nsaving image ... ");
			sprintf(imgname, "image_%d.bmp", i);
			//sprintf_s(imgname, "image.bmp");

			static unsigned char buf[IMAGESCALE * IMAGESCALE * 3];
			for (int ii = 0; ii < IMAGESCALE; ii++)
				for (int jj = 0; jj < IMAGESCALE; jj++)
				{
					buf[3 * (jj * IMAGESCALE + ii) + 0] = GetBValue(drawmap[ii][jj]);
					buf[3 * (jj * IMAGESCALE + ii) + 1] = GetGValue(drawmap[ii][jj]);
					buf[3 * (jj * IMAGESCALE + ii) + 2] = GetRValue(drawmap[ii][jj]);
				}
			bmp_write(buf, IMAGESCALE, IMAGESCALE, imgname);

			printf("\ndone!\n");

			system(imgname);
		}
		if (renderstate >= 3)
		{
			//break;
		}
		printf("\nany key to continue ...\n");
		getchar();
	}
}
#endif