// DrawView.cpp : 实现文件
//

#include "stdafx.h"
#include "LiboReader.h"
#include "DrawView.h"
#include <comdef.h>
#include "..\include\MemDC.h"
#include ".\drawview.h"



// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

CDrawView::CDrawView():m_dImageZoom(1),m_pDoc(NULL)
{
}

CDrawView::~CDrawView()
{
	delete m_pLiboImageCache;
}


BEGIN_MESSAGE_MAP(CDrawView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDrawView 绘图

void CDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	UpdataView();
}

CLiboReaderDoc* CDrawView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLiboReaderDoc)));
	return (CLiboReaderDoc*)m_pDocument;
}

void CDrawView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CMemDC dc(pDC);
	if (m_pDoc==NULL)
	{
		m_pDoc=(CLiboReaderDoc*)pDoc;
	}
	// TODO: 在此添加绘制代码
	//LoadCurImage();
	dc->SetStretchBltMode( HALFTONE );
	if (m_imgOriginal!=NULL)
	{
		m_imgOriginal->StretchBlt(dc,0,0,m_imgOriginal->GetWidth()* m_dImageZoom,m_imgOriginal->GetHeight()*m_dImageZoom,SRCCOPY);
		SetScrollsize();
	}
}


// CDrawView 诊断

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

HRESULT CDrawView::LoadCurImage()
{
	HRESULT hResult;

	m_csFilePath="C:\\Documents and Settings\\Administrator\\桌面\\Data1\\001014B.tif";
	m_imgOriginal->Destroy();//如果有与此对象相连的位图，使之分离并删除
	hResult = m_imgOriginal->Load(m_csFilePath);
	
	if (FAILED(hResult)) 
	{
		CString fmt;
		fmt.Format(_T("Load image failed:\n%x - %s"), hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return hResult;
	}
	
	return hResult;
}

void CDrawView::SetScrollsize()
{
	if (m_imgOriginal!=NULL)
	{
		CSize sizeImageScrollSize(m_imgOriginal->GetWidth()*m_dImageZoom,m_imgOriginal->GetHeight()*m_dImageZoom);

		SetScrollSizes(MM_TEXT,sizeImageScrollSize);
	}
}

// CDrawView 消息处理程序

BOOL CDrawView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CDrawView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint==WM_CUSTOM_OPEN_DOCUMENT)
	{
		UpdataView();
	}
}

void CDrawView::UpdataView()
{
	m_pLiboImageCache=new CLiboImageCache;
	m_pLiboImageCache->Initialize(theApp.GetApi(),GetDocument()->m_nMetadataID,false);
	m_imgOriginal=m_pLiboImageCache->getCurImage(GetDocument()->m_csImageID);
	SetScrollsize();
	Invalidate();
}