#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define RadianToDegree(radian) ( radian * (180.0f / L_PI))
#define DegreeToRadian(radian) ( radian * (L_PI / 180.0f ))
#define L_PI (3.141592f)
#define L_EPSILON (0.0001f)
#define GRAVITY (9.8f)
#define AIR_RESISTANCE (0.1f)
// Sub texture types
#define ID_TBASIS_AM 0   // ambient
#define ID_TBASIS_DI 1   // diffuse
#define ID_TBASIS_SP 2   // specular
#define ID_TBASIS_SH 3   // shininesNs
#define ID_TBASIS_SS 4   // shininess strength
#define ID_TBASIS_SI 5   // self-illumination
#define ID_TBASIS_OP 6   // opacity
#define ID_TBASIS_FI 7   // filter color
#define ID_TBASIS_BU 8   // bump 
#define ID_TBASIS_RL 9   // reflection
#define ID_TBASIS_RR 10  // refraction 
#define ID_TBASIS_DP 11  // displacement
#define ID_TBASIS_GN 12  // Generic
#define ID_TBASIS_EV 13  // EnvMap
#define SimpleVertex PNCT_VERTEX

#define LWRITE  LWrite::GetInstance()
#define LINPUT	LInput::GetInstance()

//마우스 좌표
#define MOUSEwX  LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).x
#define MOUSEwY  LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).y
#define MOUSEsX  LINPUT.GetMousePos().x - LGlobal::g_WindowWidth / 2
#define MOUSEsY  -(LINPUT.GetMousePos().y - LGlobal::g_WindowHeight / 2)

#include <windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <codecvt>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
//#include "LUtils.h"
#include "TMath.h"

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "DirectXTex.h"
//#include <d3dx11effect.h>


#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectXTex.lib")

//#pragma comment(lib, "Effects11d.lib")

//ImGUI
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "ImGuiFileDialog.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;


const enum OBJECTCLASSTYPE {
    CLASS_GEOM = 0,
    CLASS_BONE,
    CLASS_DUMMY,
    CLASS_BIPED,
};

const enum FILETYPE {
    NULLFILE = 0,
    ASEFILE,
    TBSFILE,
    SKMFILE,
    MATFILE,
};

struct TScene
{
    int iVersion;		// 버전
    int	iFirstFrame;    // 시작 프레임
    int	iLastFrame;     // 마지막 프레임
    int	iFrameSpeed;    // 1초당 프레임 개수(30)
    int	iTickPerFrame;  // 1프레임의 틱 값(160)
    int iNumMesh;		// 메쉬오브젝트 개수
    int iMaxWeight;		// 정점 당 가중치
    int iBindPose;		// 바인딩 포즈 에니메이션 여부
};
struct VersionMark
{
    int		iVersion;
    TCHAR	description[128];
};

//--------------------------------------------------------------------------------------
// Gun
//--------------------------------------------------------------------------------------
enum class WeaponState
{
	NONE,
	PISTOL,
	ASSAULTRIFLE,
	SHOTGUN,
	ONEHANDSWORD,
	TWOHANDSWORD,
};

enum class PlayerType
{
	NONE,
	GUN,
	SWORD,
};

enum class ComboType
{
	NONE,
	INWARD,
	OUTWARD,
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct PCT_VERTEX
{
	TVector3 p;
	TVector4 c;
	TVector2 t;
	bool operator == (const PCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PCT_VERTEX() {}
	PCT_VERTEX(TVector3		vp,
		TVector4		vc,
		TVector2     vt)
	{
		p = vp, c = vc, t = vt;
	}
};
struct PCT2_VERTEX
{
	TVector3 p;
	TVector4 c;
	TVector2 t0;
	TVector2 t1;
	bool operator == (const PCT2_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c && t0 == Vertex.t0 && t1 == Vertex.t1)
		{
			return true;
		}
		return  false;
	}
	PCT2_VERTEX() {}
	PCT2_VERTEX(TVector3		vp,
		TVector4		vc,
		TVector2     vt0,
		TVector2     vt1)
	{
		p = vp, c = vc, t0 = vt0, t1 = vt1;
	}
};
struct PC_VERTEX
{
	TVector3 p;
	TVector4 c;
	bool operator == (const PC_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(TVector3		vp,
		TVector4		vc)
	{
		p = vp, c = vc;
	}
};
struct PNC_VERTEX
{
	TVector3 p;
	TVector3 n;
	TVector4 c;
	bool operator == (const PNC_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PNC_VERTEX() {}
	PNC_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc)
	{
		p = vp, n = vn, c = vc;
	}
};
struct PNCT_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2     t;
	bool operator == (const PNCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};
struct PNCT2_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2     t;
	TVector3		vTangent;
	bool operator == (const PNCT2_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && vTangent == Vertex.vTangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector3		tangent)
	{
		p = vp, n = vn, c = vc, t = vt, vTangent = tangent;
	}
};
// ─────────────────────────────────
//  쉐이더 사용시( 가중치 4개 제한 사용 )
// ─────────────────────────────────
struct PNCT3_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2		t;
	TVector4		i;	// I0, I1, I2, I3
	TVector4		w;// W0, fW1, fW2, fNumWeight;
	bool operator == (const PNCT3_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT3_VERTEX() {
		w.x = w.y = w.z = w.w = 0.0f;
		i.x = i.y = i.z = i.w = 0.0f;
	}
	PNCT3_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector4		vw,
		TVector4		vi)
	{
		p = vp, n = vn, c = vc, t = vt;
		w = vw, i = vi;
	}
};
// ─────────────────────────────────
//  쉐이더 사용시( 가중치 8개 제한 사용 )
// ─────────────────────────────────
struct PNCT5_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2		t;
	TVector4		w0;// W0, fW1, fW2, fW3;
	TVector4		i0;	// I0, I1, I2, I3
	TVector4		w1;// W4, fW5, fW6, fNumWeight;	
	TVector4		i1;	// I4, I5, I6, I7
	bool operator == (const PNCT5_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT5_VERTEX() {
		w0.x = w0.y = w0.z = w0.w = 0.0f;
		i0.x = i0.y = i0.z = i0.w = 0.0f;
		w1.x = w1.y = w1.z = w1.w = 0.0f;
		i1.x = i1.y = i1.z = i1.w = 0.0f;
	}
	PNCT5_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector4		vw0,
		TVector4		vi0,
		TVector4		vw1,
		TVector4		vi1)
	{
		p = vp, n = vn, c = vc, t = vt;
		w0 = vw0, i0 = vi0, w1 = vw1, i1 = vi1;
	}
};
struct IW_VERTEX
{
	float       t[3];
	float 		i1[4];
	float 		w1[4];
	IW_VERTEX()
	{
		i1[0] = i1[1] = i1[2] = i1[3] = 0.0f;
		w1[0] = w1[1] = w1[2] = w1[3] = 0;
	}
};
//struct TBipedMesh
//{
//	T_STR		m_strName;
//	int					m_iNumFace;
//	TMatrix			m_matWorld;
//	vector<PNC_VERTEX>	m_VertexList;
//	PNC_VERTEX* m_pDrawVertex;
//};

struct VS_CONSTANT_BUFFER
{
	TMatrix matWorld;// c0						
	TMatrix matView;	// c4						
	TMatrix matProj;	// c8						
	TVector4 Color; // 12
};

struct TObjFrame
{
	vector<TMatrix>  m_matWorldList;
	int  				m_iIndex;
	TObjFrame(int iNumMesh, int iIndex)
	{
		m_matWorldList.resize(iNumMesh);
		m_iIndex = iIndex;
	}
};

typedef struct
{
	double x, y, z;
} Point3;




using InstanceID = std::pair<UINT, UINT>;


enum class CollisionType
{
    NONE, // 아무것도 충돌이 없을때
    MAP,
    NPC,
    PLAYER,
};
struct T_PLANE
{
	float	fA, fB, fC, fD;
	bool	CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
	{
		TVector3 vEdge0 = v1 - v0;
		TVector3 vEdge1 = v2 - v0;
		TVector3 vNormal;
		D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
		D3DXVec3Normalize(&vNormal, &vNormal);

		fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
		fD = -(fA * v0.x + fB * v0.y + fC * v0.z);
		return true;
	}
	bool	CreatePlane(TVector3 vNormal, TVector3 v0)
	{
		D3DXVec3Normalize(&vNormal, &vNormal);

		fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
		fD = -(fA * v0.x + fB * v0.y + fC * v0.z);
		return true;
	}
	void	Normalize()
	{
		float fMag = sqrt(fA * fA + fB * fB + fC * fC);
		fA = fA / fMag;
		fB = fB / fMag;
		fC = fC / fMag;
		fD = fD / fMag;
	}

};
struct T_SPHERE
{
	TVector3		vCenter;
	float			fRadius;
};

struct T_BOX
{
    TVector3   vPos[8];
    TVector3   vMax;
    TVector3   vMin;
    TVector3   vCenter;
    TVector3   vAxis[3];
    float      fExtent[3];
};

static void L_DebugString(const WCHAR* msg)
{
#ifdef _DEBUG
	OutputDebugString(msg);
#endif
}

static std::wstring mtw(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(str);
}

static std::string wtm(std::wstring str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

//struct SimpleVertex
//{
//    TVector3 p;
//    TVector3 n;
//    TVector4 c;
//    TVector2 t;
//
//    SimpleVertex() = default;
//    SimpleVertex(const TVector3 & pos, const TVector2 & uv, const TVector4 & color, const TVector3 & normal) : p(pos), t(uv), n(normal), c(color) {}
//};





//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
