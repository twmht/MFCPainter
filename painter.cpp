#include <afxwin.h>
#include <afxtempl.h>
#include "resource.h"
#include <afxext.h>
#include <atlimage.h>

class MyModelDialog :public CDialog  //Model對話盒
{private:
	int width;
	int height;
public:
	int GetWidth(){return width;}
	int GetHeight(){return height;}
	MyModelDialog(CWnd *);
	
void DoDataExchange(CDataExchange *pDX)
	{
		DDX_Text(pDX,IDC_EDIT1,width);  //與width變數作連結
		DDX_Text(pDX,IDC_EDIT2,height); //與height變數作連結
		
	}
};

class MyModelessDialog :public CDialog  //Modeless對話盒
{private:
	
	int isFill;
	int isDash;
	int LineWidth;
public:
	MyModelessDialog()

	{
		isFill=0;
		isDash=0;
		LineWidth=6;  //index

	}

	
void DoDataExchange(CDataExchange *pDX)

	{
		
		DDX_Check(pDX,IDC_CHECK1,isFill);
		DDX_Check(pDX,IDC_CHECK2,isDash);
		DDX_CBIndex(pDX,IDC_COMBO1,LineWidth); 
	 
	}

BOOL OnCommand(WPARAM wParam,LPARAM lParam);//當使用者選擇選單的選項或改變其他控制項的內容時驅動
};
class MyFrame :public CFrameWnd
{private:
	CMenu* pMenu;
	int width;  //點陣圖寬度
	int height;
	int isFill;
	int SolidDash;
	int LineWidth;
	CToolBar toolBar;
	MyModelessDialog* pdlg;
	int selective;
	CImage bitmap;
	CImage oldBit;
	CImage newBit;
	CPoint bPoint;
	COLORREF Linecolor;
	int LineRValue,LineGValue,LineBValue;
	COLORREF Fillcolor;
	int FillRValue,FillGValue,FillBValue;

	bool undoAble;
	bool redoAble;


	CStatusBar statusBar;//狀態列

public:
	MyFrame()
	{ 
	  Create(NULL, "Hello MFC");
	  pMenu=new CMenu();
	  pMenu->LoadMenu(IDR_MENU1);
	  SetMenu(pMenu);

	  width=500;
	  height=300;
	  
	  toolBar.Create(this);  //產生工具列
	  toolBar.LoadToolBar(IDR_TOOLBAR1);
	  toolBar.SetBarStyle(CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY);

	  pdlg= new MyModelessDialog();  //產生modeless對話盒
	  pdlg->Create(IDD_DIALOGBAR,this);
	  pdlg->UpdateData(FALSE);
	  pdlg->ShowWindow(SW_SHOW);

	    //產生點陣圖
	  oldBit.Create(width,height,24);
	  newBit.Create(width,height,24);
	  undoAble=false;
	  redoAble=false;
	  bitmap.Create(width,height,24);
	  CDC* aDC=CDC::FromHandle(bitmap.GetDC());
	  CBrush brush(RGB(255,255,255));
	  aDC->FillRect(CRect(0,0,width,height),&brush);
	  aDC->Rectangle(0,0,width,height);
	  bitmap.ReleaseDC();

	  SetIcon(AfxGetApp()->LoadIcon(IDI_ICON1),TRUE);



	  SolidDash=PS_SOLID;
	  Linecolor=RGB(0,0,0);
	  Fillcolor=RGB(255,255,255);

	  static UINT indicators []=
		{	
			ID_LINE_COLOR,
			ID_FILL_COLOR,
		};
		statusBar.Create(this);
		statusBar.SetIndicators(indicators,2);
		statusBar.SetPaneInfo(0,ID_LINE_COLOR,SBPS_STRETCH,50);
		statusBar.SetPaneInfo(1,ID_FILL_COLOR,SBPS_STRETCH,50);


	}
	void SetSolid(int s){SolidDash=s;}
	void SetLineWidth(int w){LineWidth=w;}
	void SetFill(int s){isFill=s;}

	int GetWidth(){ return width;}
	int GetHeight(){return height;}
    afx_msg void OnOptions()
	{	MyModelDialog dlg(this);
        if(dlg.DoModal()==IDOK)

		{
		
			CImage tempBit;

			int oldwidth=bitmap.GetWidth(),oldheight=bitmap.GetHeight();
			width=dlg.GetWidth();  //點陣圖的長跟寬
			height=dlg.GetHeight();

			tempBit.Create(width,height,24);  //新的點陣圖
			CDC* aDC=CDC::FromHandle(tempBit.GetDC());
			CBrush brush(RGB(255,255,255));
	        aDC->FillRect(CRect(0,0,width,height),&brush); //清空
			tempBit.ReleaseDC();


			bitmap.BitBlt(tempBit.GetDC(),0,0,oldwidth,oldheight,0,0,SRCCOPY);//把bitmap複製到tempBit上
			tempBit.ReleaseDC();

			

			CPen pen(SolidDash,1,RGB(255,255,255));  //把舊的點陣圖的外圍矩形用白色清除
			CPen pen1(SolidDash,1,RGB(0,0,0));
			aDC=CDC::FromHandle(tempBit.GetDC());
			aDC->SelectObject(&pen);
			aDC->MoveTo(0,0);
			aDC->LineTo(oldwidth-1,0);

			aDC->MoveTo(0,0);
			aDC->LineTo(0,oldheight-1);

			aDC->MoveTo(oldwidth-1,0);
			aDC->LineTo(oldwidth-1,oldheight-1);

			aDC->MoveTo(0,oldheight-1);
			aDC->LineTo(oldwidth-1,oldheight-1);

	//-------------------------------------------------//
			aDC->SelectObject(&pen1);
			aDC->MoveTo(0,0);
			aDC->LineTo(width-1,0);

			aDC->MoveTo(0,0);
			aDC->LineTo(0,height-1);

			aDC->MoveTo(width-1,0);
			aDC->LineTo(width-1,height-1);

			aDC->MoveTo(0,height-1);
			aDC->LineTo(width-1,height-1);
			
			tempBit.ReleaseDC();
			
		
			bitmap.Destroy();

			bitmap.Create(width,height,24);
			tempBit.BitBlt(bitmap.GetDC(),0,0,width,height,0,0,SRCCOPY);//把tempBit複製到bitmap身上
			bitmap.ReleaseDC();
			

			
			
			Invalidate();

		}	
	}
	afx_msg void OnLine()

	{
		selective=0;

	}

	afx_msg void OnRectangle()

	{
		selective=1;

	}

	afx_msg void OnCircle()

	{
		selective=2;

	}

	afx_msg void OnPencil()

	{
		selective=3;

	}

	afx_msg void OnEraser()

	{
		selective=4;

	}

	afx_msg void OnPaint()
	{	
		CPaintDC dc(this);
	    bitmap.Draw(dc,0,100,width,height);	
	}
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	
	{
		
		

		CRect rect(10,110,width-10,height+90);

		

		if(rect.PtInRect(point))
		
		{
			
			SetCapture();
	        bPoint=point;

			if(selective==3)  //畫小圓點

			{
				CClientDC dc(this);

				bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
			    undoAble=true;
			    oldBit.ReleaseDC();
				
				CDC* pDC = CDC::FromHandle(bitmap.GetDC());
			    CPen pen(SolidDash,LineWidth,Linecolor);
			    CBrush brush(Linecolor);
;
				pDC->SelectObject(&pen);
			    pDC->SelectObject(&brush);
			    pDC->Ellipse(point.x-LineWidth-2,point.y-LineWidth-2-100,point.x+LineWidth+2,point.y+LineWidth+2-100);
				bitmap.ReleaseDC();

				dc.SelectObject(&pen);
			    dc.SelectObject(&brush);
			    dc.Ellipse(point.x-LineWidth-2,point.y-LineWidth-2,point.x+LineWidth+2,point.y+LineWidth+2);

				bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();
				

				
					

			}

			if(selective==4) //橡皮擦

			{
				bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
			    undoAble=true;
			    oldBit.ReleaseDC();
				CClientDC dc(this);
				CDC* pDC = CDC::FromHandle(bitmap.GetDC());
				CBrush brush(RGB(255,255,255));
				pDC->FillRect(CRect(point.x-LineWidth-2,point.y-LineWidth-2-100,point.x+LineWidth+2,point.y+LineWidth+2-100),&brush);
				dc.FillRect(CRect(point.x-LineWidth-2,point.y-LineWidth-2,point.x+LineWidth+2,point.y+LineWidth+2),&brush);		
				bitmap.ReleaseDC();

				bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();
				

			}
			

		}
				
		
	}
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point)  //這邊只處理畫線、矩形、圓形
	{
	
	
	CRect rect(10,110,width-10,height+90);	

	ReleaseCapture();
	if(rect.PtInRect(point))
		
	{
	  

	  CClientDC dc(this);
	  CDC* pDC = CDC::FromHandle(bitmap.GetDC());
	  CPen pen(SolidDash,LineWidth,Linecolor);
	  
	  CBrush brush(Fillcolor);
	  if(isFill)
	  {
	  dc.SelectObject(&brush);
	  pDC->SelectObject(&brush);
	  }
	  pDC->SelectObject(&pen);
	  dc.SelectObject(&pen);
	  
	  switch(selective)

	  {
		  
		  case 0:
			  bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
	          undoAble=true;
	          oldBit.ReleaseDC();
			 pDC->MoveTo(bPoint.x,bPoint.y-100);
			 pDC->LineTo(point.x,point.y-100);
			 dc.MoveTo(bPoint);
			 dc.LineTo(point);
			 bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();
			 
			  break;
		  case 1:
			  bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
	          undoAble=true;
	          oldBit.ReleaseDC();
	
	          pDC->Rectangle(bPoint.x,bPoint.y-100,point.x,point.y-100);
			  dc.Rectangle(bPoint.x,bPoint.y,point.x,point.y);

			  bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();
			
	          break;
		  case 2:
			  bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
	          undoAble=true;
	          oldBit.ReleaseDC();
			  pDC->Ellipse(bPoint.x,bPoint.y-100,point.x,point.y-100);
			  dc.Ellipse(bPoint.x,bPoint.y,point.x,point.y);

			  bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();
			  
			  break;
		  

	  }

	  bitmap.ReleaseDC();
	  


	}
	

	}

	afx_msg void OnMouseMove(UINT nFlags, CPoint point)

    {
		
        CRect rect(10,110,width-10,height+90);
		
	    if(rect.PtInRect(point))
		
	  {
		  
		if ((selective==3) && this==GetCapture())

		{
			bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);  //畫圖之前先存起來
	        undoAble=true;
		    oldBit.ReleaseDC();
			CDC* pDC = CDC::FromHandle(bitmap.GetDC());
			CPen pen(SolidDash,LineWidth,Linecolor);
			CBrush brush(Linecolor);
			pDC->SelectObject(&pen);
			pDC->SelectObject(&brush);
			pDC->Ellipse(point.x-LineWidth-2,point.y-LineWidth-2-100,point.x+LineWidth+2,point.y+LineWidth+2-100);


			bitmap.ReleaseDC();

			CClientDC dc(this); 
			dc.SelectObject(&pen);
			dc.SelectObject(&brush);
			dc.Ellipse(point.x-LineWidth-2,point.y-LineWidth-2,point.x+LineWidth+2,point.y+LineWidth+2);

			bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();

			 
			
	

		}

		else if((selective==4) && this==GetCapture())

		{
			bitmap.BitBlt(oldBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
	        undoAble=true;
		    oldBit.ReleaseDC();
			CDC* pDC = CDC::FromHandle(bitmap.GetDC());
			CBrush brush(RGB(255,255,255));
			pDC->FillRect(CRect(point.x-LineWidth-2,point.y-LineWidth-2-100,point.x+LineWidth+2,point.y+LineWidth+2-100),&brush); //橡皮擦

		    bitmap.ReleaseDC();

			CClientDC dc(this); 
			dc.FillRect(CRect(point.x-LineWidth-2,point.y-LineWidth-2,point.x+LineWidth+2,point.y+LineWidth+2),&brush);

			bitmap.BitBlt(newBit.GetDC(),0,0,width,height,0,0,SRCCOPY);
				newBit.ReleaseDC();

					

		}


		}


	}

	afx_msg void UpdateLine(CCmdUI* aCmdUI)
	
	{	
		aCmdUI->SetCheck(selective==0);
	}

	afx_msg void UpdateRectangle(CCmdUI* aCmdUI)
	
	{	
		aCmdUI->SetCheck(selective==1);
	}

	afx_msg void UpdateCircle(CCmdUI* aCmdUI)
	{	aCmdUI->SetCheck(selective==2);
	}
	afx_msg void UpdatePencil(CCmdUI* aCmdUI)
	{	aCmdUI->SetCheck(selective==3);
	}
	afx_msg void UpdateEraser(CCmdUI* aCmdUI)
	{	aCmdUI->SetCheck(selective==4);
	}

	afx_msg void OnColorLine()  //改變Line顏色
	{	CColorDialog cdlg(RGB(0,0,0),CC_FULLOPEN);
		if (cdlg.DoModal()==IDOK)

		{
			char string[200];
			Linecolor=cdlg.GetColor();		
			LineRValue=GetRValue(Linecolor);
			LineGValue=GetGValue(Linecolor);
			LineBValue=GetBValue(Linecolor);
			sprintf(string,"                                        Line Color: (Red) %d (Green) %d (Blue) %d",LineRValue,LineGValue,LineBValue);
			statusBar.SetPaneText(0,string);

		}
	}

	afx_msg void OnColorFill()  //改變Fill顏色
	{	
		CColorDialog cdlg(RGB(255,255,255),CC_FULLOPEN);
		if (cdlg.DoModal()==IDOK)

		{
			char string[200];
			Fillcolor=cdlg.GetColor();
			FillRValue=GetRValue(Fillcolor);
			FillGValue=GetGValue(Fillcolor);
			FillBValue=GetBValue(Fillcolor);
			sprintf(string,"                                        Fill Color: (Red) %d (Green) %d (Blue) %d",FillRValue,FillGValue,FillBValue);
			statusBar.SetPaneText(1,string);
			
			

		}

	}

	afx_msg void OnSave()
	{	CFileDialog fdlg(false,NULL,"*gif",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"GIF(*gif)|*.gif||");
		if (fdlg.DoModal()==IDOK)
		{	
			bitmap.Save("picture.gif");
		}
	}

	afx_msg void OnLoad()
	{	CFileDialog fdlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"GIF(*gif)|*.gif||");
		if (fdlg.DoModal()==IDOK)
		{	bitmap.Detach();
			bitmap.Load("picture.gif");
			Invalidate();
		}
	}

	afx_msg void OnExit()
	{
		DestroyWindow();
	}

	afx_msg void OnUnDo()
	{	oldBit.BitBlt(bitmap.GetDC(),0,0,width,height,0,0,SRCCOPY);
		undoAble=false;  //undo之後變不能在undo了
		redoAble=true;  //可以作redo
		bitmap.ReleaseDC();
		Invalidate();
	}
	afx_msg void UpdateUnDo(CCmdUI* aCmdUI)
	{	aCmdUI->Enable(undoAble);
	}

	afx_msg void OnReDo()
	{	newBit.BitBlt(bitmap.GetDC(),0,0,width,height,0,0,SRCCOPY);
		redoAble=false;
		bitmap.ReleaseDC();
		Invalidate();
	}

	afx_msg void UpdateReDo(CCmdUI* aCmdUI)
	
	{
		aCmdUI->Enable(!undoAble && redoAble);  //當不能undo且可以redo的時候
	}



	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(MyFrame,CFrameWnd)
	ON_COMMAND(ID_IMAGE_OPTIONS,OnOptions)
	ON_COMMAND(ID_LINE,OnLine)
	ON_COMMAND(ID_CIRCLE,OnCircle)
	ON_COMMAND(ID_RECTANGLE,OnRectangle)
	ON_COMMAND(ID_ERASER,OnEraser)
	ON_COMMAND(ID_PENCIL,OnPencil)
	ON_UPDATE_COMMAND_UI(ID_LINE,UpdateLine)
	ON_UPDATE_COMMAND_UI(ID_RECTANGLE,UpdateRectangle)
	ON_UPDATE_COMMAND_UI(ID_CIRCLE,UpdateCircle)
	ON_UPDATE_COMMAND_UI(ID_PENCIL,UpdatePencil)
	ON_UPDATE_COMMAND_UI(ID_ERASER,UpdateEraser)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_COMMAND(ID_COLOR_LINE,OnColorLine)
	ON_COMMAND(ID_COLOR_FILL,OnColorFill)
	ON_COMMAND(ID_FILE_SAVE40001,OnSave)
	ON_COMMAND(ID_FILE_LOAD,OnLoad)
	ON_COMMAND(ID_FILE_EXIT,OnExit)

	ON_COMMAND(ID_EDIT_UNDO,OnUnDo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO,UpdateUnDo)
	ON_COMMAND(ID_EDIT_REDO40008,OnReDo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO40008,UpdateReDo)
END_MESSAGE_MAP()

MyModelDialog::MyModelDialog(CWnd *parent)
    :CDialog(IDD_DIALOG1,parent)
   {
	  width=((MyFrame*)parent)->GetWidth();
	  height=((MyFrame*)parent)->GetHeight();
	  
   }
	

 BOOL MyModelessDialog::OnCommand(WPARAM wParam,LPARAM lParam)
	{	UpdateData();
		((MyFrame *) AfxGetMainWnd())->SetSolid(isDash);

		((MyFrame *) AfxGetMainWnd())->SetFill(isFill);

		((MyFrame *)AfxGetMainWnd())->SetLineWidth(LineWidth);
		return CDialog::OnCommand(wParam,lParam);
	}

class MyApp :public CWinApp
{public:
	BOOL InitInstance()
	{	CFrameWnd *frame= new MyFrame();
		m_pMainWnd=frame;
		frame->ShowWindow(SW_SHOW);
		return true;
	}
};
MyApp app;
