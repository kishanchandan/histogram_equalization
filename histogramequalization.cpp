#include "stdafx.h"
#include "kingimage.h"
#include "resource.h"
#include "kingimageDoc.h"
#include "kingimageView.h"
#include <array>
#include <fstream>
#include <algorithm>
#include <list> 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKingimageView

IMPLEMENT_DYNCREATE(CKingimageView, CScrollView)

BEGIN_MESSAGE_MAP(CKingimageView, CScrollView)
	//{{AFX_MSG_MAP(CKingimageView)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_ANSWER1_CREATENEGATIVEIMAGE, OnAnswer1Createnegativeimage)
	ON_COMMAND(ID_ANSWER1_APPLYHISTOGRAMEQUALIZATION, &CKingimageView::OnAnswer1Applyhistogramequalization)
	ON_COMMAND(ID_ANSWER2_LABELSHAPES, &CKingimageView::OnAnswer2Labelshapes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKingimageView construction/destruction

CKingimageView::CKingimageView()
{
	// TODO: add construction code here

}

CKingimageView::~CKingimageView()
{
}

BOOL CKingimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView drawing

void CKingimageView::OnDraw(CDC* pDC)
{
	CKingimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	int x,y,bytes;
	if (pDoc->imagetype==PCX)
		(pDoc->_pcx)->draw(pDC);
	if (pDoc->imagetype==BMP)
		(pDoc->_bmp)->draw(pDC);
	if (pDoc->imagetype==GIF)
		(pDoc->_gif)->draw(pDC);
	if (pDoc->imagetype==JPG)
		(pDoc->_jpg)->draw(pDC);

}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView printing

BOOL CKingimageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKingimageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKingimageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView diagnostics

#ifdef _DEBUG
void CKingimageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CKingimageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CKingimageDoc* CKingimageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKingimageDoc)));
	return (CKingimageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKingimageView message handlers



void CKingimageView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize totalSize=CSize(::GetSystemMetrics(SM_CXSCREEN),
		                  ::GetSystemMetrics(SM_CYSCREEN));
	CSize pageSize=CSize(totalSize.cx/2,totalSize.cy/2);
	CSize lineSize=CSize(totalSize.cx/10,totalSize.cy/10);

	SetScrollSizes(MM_TEXT,totalSize,pageSize,lineSize);
}

void CKingimageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);
}




void CKingimageView::OnAnswer1Applyhistogramequalization()
{
	// TODO: Add your command handler code here
	CKingimageDoc* pDoc = GetDocument();


	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;
	BYTE  *poriginal = pDoc->_bmp->point;


	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		AfxMessageBox(_T("Image is not a 24 Bit Image"));

	}
	else if (iBitPerPixel == 24)  ////True color 24bits image
	{
		int pImgavg = 0;
		int intensityfrequency[256] = { 0 };
		float totalpixels = 0;
		float probabilityfrequency[256] = { 0 };
		float cdf[256] = { 0 };
		int newequalizedintensity[256] = { 0 };
		for (int i = 0; i<iHeight; i++)
			for (int j = 0; j<iWidth; j++)
			{
				pImgavg = (int(pImg[i*Wp + j * 3]) + int(pImg[i*Wp + j * 3 + 1]) + int(pImg[i*Wp + j * 3 + 2])) / 3;
				intensityfrequency[pImgavg] = intensityfrequency[pImgavg] + 1;

			}
		for (int k = 0; k < 256; k++)
		{
			totalpixels = totalpixels + intensityfrequency[k];
		}
		for (int k = 0; k < 256; k++)
		{
			probabilityfrequency[k] = float(intensityfrequency[k] / totalpixels);
		}

		for (int k = 0; k < 256; k++)
		{
			cdf[k] = probabilityfrequency[k] + cdf[k - 1];
		}

		for (int k = 0; k < 256; k++)
		{
			newequalizedintensity[k] = cdf[k] * 255;
		}
				for (int i = 0; i<iHeight; i++)
			for (int j = 0; j<iWidth; j++)
			{
				pImg[i*Wp + j * 3] = newequalizedintensity[(pImg[i*Wp + j * 3])];      //B
				pImg[i*Wp + j * 3 + 1] = newequalizedintensity[(pImg[i*Wp + j * 3 + 1])];      //G
				pImg[i*Wp + j * 3 + 2] = newequalizedintensity[(pImg[i*Wp + j * 3 + 2])];      //R 
			}
	}



	////redraw the screen
	OnDraw(GetDC());


}



