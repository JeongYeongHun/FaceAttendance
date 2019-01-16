
// eDeuProjectDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <iostream>
#include "eDeuProject.h"
#include "eDeuProjectDlg.h"
#include "afxdialogex.h"
#include "opencv2\objdetect.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CeDeuProjectDlg 대화 상자


CeDeuProjectDlg::CeDeuProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDEUPROJECT_DIALOG, pParent)
	
	, m_strID(_T(""))
	, m_strMessage(_T(""))
	, m_iPort(0)
	, m_strServer(_T(""))
	, m_iType(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strID = _T("");
}

CeDeuProjectDlg::~CeDeuProjectDlg() //소멸자
{
	if (m_ConCheck == 1)  //서버 또는 클라이언트가 활성화 되잇을 경우
	{
		if (m_iType == 0)
		{
			delete m_pServerSock;  //서버용  리슨을 받기위해 만들었던 m_pServerSock 클래스를 반환
			POSITION pos = m_ClientSock.GetHeadPosition();    // 서버용  만들어지는 클라이언트마다 주소값을 다르게 한 포지션 값만큼 (포지션 값 * ClientSock Class)
			while (pos != NULL) {         //서버용             //계속 반복 pos 값이 사라질때까지
				delete m_ClientSock.GetNext(pos);   //서버용          // 클라이언트 숫자만큼 반환
			}
		}
		else {
			m_ConCheck = 0;   //종료 체크
			SendInfo(m_ConCheck); //종료메시지 전달 함수
			delete m_pClientSock;   // 클라이언트 클라스 할당해제   // 리시브용?
		}
	}
}

void CeDeuProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM, m_cam); // 1번 캠스크린
	DDX_Control(pDX, IDC_DETECT, m_dscreen2); // 2번 얼굴영역 스크린 
	DDX_Control(pDX, IDC_DECTSCREEN, m_DectFaceScreen); // 3번 얼굴만 나오는 스크린
	DDX_Control(pDX, IDC_STATUS, m_Listbox);
	DDX_Control(pDX, IDC_DBLIST, m_Listbox3);
	DDX_Control(pDX, IDC_FACECHECK, m_facecheck);
	DDX_Control(pDX, IDC_LOICHECK, m_roicheck);
	DDX_Control(pDX, IDC_onscreen2, m_screen2onoff);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_DBNAME, m_name2);
	//DDX_Control(pDX, IDC_CAPTURE, m_Screen4);
	//  DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iPort);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Control(pDX, IDC_EDIT_TIME, m_Time);
	DDX_Control(pDX, IDC_LIST_CHATRECEIVE, m_lChatReceive);
	//DDX_Radio(pDX, IDC_RADIO_TYPE_S, m_iType);
	DDX_Control(pDX, IDC_CLIENT1, m_client1);
	DDX_Control(pDX, IDC_CLIENT2, m_client2);
	DDX_Control(pDX, IDC_CLIENT3, m_client3);
	DDX_Control(pDX, IDC_CLIENT4, m_client4);
}

BEGIN_MESSAGE_MAP(CeDeuProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CULCHECK, &CeDeuProjectDlg::OnBnClickedCulcheck)
	ON_BN_CLICKED(IDC_BUTT_CAPTURE, &CeDeuProjectDlg::OnBnClickedButtCapture)
	ON_BN_CLICKED(IDC_onscreen2, &CeDeuProjectDlg::OnBnClickedonscreen2)
	ON_BN_CLICKED(IDC_FACECHECK, &CeDeuProjectDlg::OnBnClickedFacecheck)
	//ON_BN_CLICKED(IDC_EYECHECK, &CeDeuProjectDlg::OnBnClickedEyecheck)
	ON_BN_CLICKED(IDC_LOICHECK, &CeDeuProjectDlg::OnBnClickedLoicheck)
	ON_BN_CLICKED(IDC_BUTT_EXIT, &CeDeuProjectDlg::OnBnClickedButtExit)
	ON_BN_CLICKED(IDC_BUTT_INPUT, &CeDeuProjectDlg::OnBnClickedButtInput)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTT_DB, &CeDeuProjectDlg::OnBnClickedButtDb)
	ON_BN_CLICKED(IDC_B_MATCHING, &CeDeuProjectDlg::OnBnClickedBMatching)
	//ON_BN_CLICKED(IDC_RADIO_TYPE_S, &CeDeuProjectDlg::OnBnClickedRadioTypeS)
	//ON_BN_CLICKED(IDC_RADIO_TYPE_C, &CeDeuProjectDlg::OnBnClickedRadioTypeC)
	//ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CeDeuProjectDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CeDeuProjectDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_MB_SERVER, &CeDeuProjectDlg::OnBnClickedMbServer)
	ON_BN_CLICKED(IDC_MB_CLIENT, &CeDeuProjectDlg::OnBnClickedMbClient)
	ON_BN_CLICKED(IDC_HEXIT, &CeDeuProjectDlg::OnBnClickedHexit)
END_MESSAGE_MAP()


// CeDeuProjectDlg 메시지 처리기

BOOL CeDeuProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	cam.open(0);

	if (!cam.isOpened())
		MessageBox(_T("캠을 열수 없습니다. \n"));
	else 
		MessageBox(_T("카메라 연결 되었습니다"));

	if (!cascade_face.load(haarcascade_face)) //Face Cascade.xml 체크
	{
		std::cout << "ERROR: Could not load face_classifier cascade" << std::endl;
	}
	else
	{
		MessageBox(_T("haarcascade_face 열림"));
	}
	if (!cascade_eye.load(haarcascade_eye)) //Eye Cascade.xml 체크
	{
		std::cout << "ERROR: Could not load eye_classifier cascade" << std::endl;
	}
	else
	{
		MessageBox(_T("haarcascade_eye 열림"));
	}
	
	m_ConCheck = 0;     //처음 생성 될때  초기화
	GetDlgItem(IDC_CAM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DETECT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DECTSCREEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_onscreen2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FACECHECK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOICHECK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DBLIST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DBTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DBNAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTT_DB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SYSTEM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATUS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_S_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTT_INPUT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_B_MATCHING)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTT_CAPTURE)->ShowWindow(SW_HIDE);

	

	GetDlgItem(IDC_TIP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_CHATRECEIVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MESSAGE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_HEXIT)->ShowWindow(SW_HIDE); //강
	
	m_Listbox.AddString(_T("***** 시작하기전에 좌측하단에서 이름을 입력해주세요 *****"));



	UpdateData(TRUE);
	m_strServer = "127.0.0.1";   //IP
	m_iPort = 700;    //PORT
	m_strID = "";   //ID
	UpdateData(FALSE);    //에딧박스 셋팅

	SetTimer(2, 1000, NULL);   // Timer(시계)

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	GetDlgItem(IDC_CLIENT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLIENT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLIENT3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CLIENT4)->ShowWindow(SW_HIDE);
	
	icon1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
	m_client1.SetBitmap(icon1);
	icon2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
	m_client2.SetBitmap(icon2);
	icon3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
	m_client1.SetBitmap(icon3);
	icon4 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
	m_client2.SetBitmap(icon4);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CeDeuProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CeDeuProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CDC* pDC;
		CRect rect;

		CDC* pDC2;
		CRect rect2;

		CDC* pDC3;
		CRect rect3;
		
		pDC3 = m_DectFaceScreen.GetDC(); // 얼굴영역만뿌려줄  스크린의 DC를 가져온다
		m_DectFaceScreen.GetClientRect(&rect3); // rect3에 m_dectface 의 픽쳐컨트롤 크기 저장

		pDC2 = m_dscreen2.GetDC(); // 얼굴 영역 인식하는 과정을 나타냄
		m_dscreen2.GetClientRect(&rect2); // rect2 에 m_dscreen2 의 픽쳐컨트롤 크기 저장

		pDC = m_cam.GetDC(); // 캠이 나오는 픽쳐컨트롤 DC를 가져온다.
		m_cam.GetClientRect(&rect); // rect 에 픽쳐컨트롤 크기 저장

		UpdateData(TRUE);

		if (this->cam.isOpened()) { // 캠이켜져있으면? 
			cam >> frame; // frame이라는 matimage 형으로 이미지 복사
			SetTimer(1, 30, NULL);
		}
		DisplayImage(&frame, pDC, rect); // mat 형 이미지를 해당 컨트롤에 뿌려줄수있게 임의로 작성한 함수
		
		if(m_screen2onoff.GetCheck()==BST_CHECKED){  // 스크린 2 켜지면
			UpdateData(TRUE);
			Mat frame_gray; // 이진화된 이미지가 들어갈 mat이미지변수선언
			cvtColor(frame,frame_gray,COLOR_BGR2GRAY); // 이진화 시켜서 frame_gray에 담기
			equalizeHist(frame_gray, frame_gray); // 이미지를 히스토그램화 시킨다(강도측정)
			
			if (m_facecheck.GetCheck() == BST_CHECKED) { // 얼굴인식 켜지면
				UpdateData(TRUE);
				cascade_face.detectMultiScale(frame_gray, faces, 1.1, 3, 3 | CASCADE_SCALE_IMAGE, Size(30, 30));
				//HAAR cascade 라이브러리를 이용한 얼굴 영억 인식하여 faces라는 이름의 rect에 담는다.
				cascade_eye.detectMultiScale(frame_gray, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(3, 3));
				// HAAR cascade 라이브러리를 이용한 눈 영억 인식하여 eyes라는 이름의 rect에 담는다.

				for (int roi_i = 0; roi_i < faces.size(); roi_i++) { // 얼굴인식 검출영역 개수만큼 사각형 그리기
						//Point center(faces[roi_i].x + faces[roi_i].width / 2, faces[roi_i].y + faces[roi_i].height / 2); // 얼굴인식타원의 중앙포인트
						//ellipse(frame, center, Size(faces[roi_i].width / 2, faces[roi_i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
					rectangle(frame, faces[roi_i], Scalar(0, 0, 0), 2); // 실제 frame(캠영상)에 사각형이 그려지는 부분
					/*cvRectangle(&frame, Point(faces[roi_i].x , faces[roi_i].y),
					Point(faces[roi_i].x +faces[roi_i].width,
					faces[roi_i].y + faces[roi_i].height), cvScalar(255, 255, 255), 1, 0, 0);*/
				}			//타원그리기,frame위에.
		/*if(m_eyecheck.GetCheck()==BST_CHECKED){
			if (m_eye == TRUE) { // 눈,입 인식 활성화
				Mat faceROI = frame_gray(faces[roi_i]);  // 얼굴 인식된 Mat형 faceROI 이미지 생성.
				cascade_eye.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(3, 3));
					for (size_t j = 0; j < eyes.size(); j++) {
					Point eye_center(eyes[j].x + eyes[j].x + eyes[j].width / 2, eyes[roi_i].y + eyes[j].y + eyes[j].height / 2);
					int radius = cvRound((eyes[roi_i].width + eyes[roi_i].height)*0.25);
					circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
					DisplayImage(&frame, pDC2, rect2); // mat 형 이미지를 해당 컨트롤에 뿌려줄수있게 임의로 작성한 함수
					}
			}
}*/			
				DisplayImage(&frame, pDC2, rect2); // mat 형 이미지를 해당 컨트롤에 뿌려줄수있게 임의로 작성한 함수
				}
			else {
				DisplayImage(&frame_gray, pDC2, rect2); // mat 형 이미지를 해당 컨트롤에 뿌려줄수있게 임의로 작성한 함수
			}
			
			if (m_roicheck.GetCheck() == BST_CHECKED) // 얼굴영역 캡쳐시작
			{
				UpdateData(TRUE);
				roi_i = 0;
				Rect roi_area(faces[roi_i].x, faces[roi_i].y, faces[roi_i].width, faces[roi_i].height);
				image_roi = frame(roi_area);
				DrawCvMat(pDC3, image_roi, rect3);
				}
		}
			
		roi_i = 0;
		ReleaseDC(pDC);
		ReleaseDC(pDC2);
		ReleaseDC(pDC3);
		
		if (cursorstop == TRUE) {
			SetCursorPos(30, 30);
		}
	}
	
}
		////////////////////////////////////////얼굴 인식 영역 끝 ////////////////////////////////////

	
	
	


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CeDeuProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CeDeuProjectDlg::DisplayImage(Mat *image, CDC *pDC, CRect rect)
{
	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(bitmapInfo));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biWidth = image->cols;
	bitmapInfo.bmiHeader.biHeight = -image->rows;

	IplImage *tempImage;

	if (image->channels() == 1)
	{
		tempImage = cvCreateImage(image->size(), IPL_DEPTH_8U, 3);
		cvCvtColor(&IplImage(*image), tempImage, CV_GRAY2BGR);
	}
	else if (image->channels() == 3)
	{
		tempImage = cvCloneImage(&IplImage(*image));
	}

	bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top,
		rect.right, rect.bottom,
		0, 0, tempImage->width, tempImage->height,
		tempImage->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	cvReleaseImage(&tempImage);
}


void CeDeuProjectDlg::DrawCvMat(CDC* pDC, cv::Mat& origin, CRect rect) { // 이미지를 dc크기에 맞게 조절
	CImage mfcImg;
	cv::Mat outImg;
	cv::flip(origin, outImg, 0);
	//흑백이면 채널을3으로
	if (outImg.channels() == 1) {
		cv::cvtColor(outImg, outImg, CV_GRAY2BGR);
	}
	//이미지의 크기를 rect에 들어가게 수정합니다.
	if (outImg.cols != rect.Width() || outImg.rows != rect.Height()) {
		int newWidth = outImg.cols*(rect.Height() / (double)outImg.rows);
		int newHeight = rect.Height();
		if (newWidth > rect.Width()) {
			newWidth = rect.Width();
			newHeight = outImg.rows*(rect.Width() / (double)outImg.cols);
		}
		cv::resize(outImg, outImg, cv::Size(newWidth, newHeight), 0, 0, CV_INTER_NN);
	}
	mfcImg.Create(outImg.cols, outImg.rows, 24);
	BITMAPINFO bitInfo = { { sizeof(BITMAPINFOHEADER),outImg.cols,outImg.rows,1,24 },0 };

	void* vp = outImg.data;
	IplImage* iplimage = nullptr;
	if (outImg.cols % 4 != 0 && outImg.isContinuous()) {
		iplimage = cvCreateImage(outImg.size(), 8, outImg.channels());
		outImg.copyTo(cv::cvarrToMat(iplimage));
		vp = iplimage->imageData;
	}
	pDC->SetStretchBltMode(HALFTONE);
	StretchDIBits(mfcImg.GetDC(), 0, 0, outImg.cols, outImg.rows, 0, 0, outImg.cols, outImg.rows, vp, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	if (iplimage != nullptr) {
		cvReleaseImage(&iplimage);
	}
	mfcImg.BitBlt(*pDC, (rect.Width() - outImg.cols) / 2 + rect.left, (rect.Height() - outImg.rows) / 2 + rect.top);
	mfcImg.ReleaseDC();
	outImg.release();
}


void CeDeuProjectDlg::OnBnClickedCheckFace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CeDeuProjectDlg::OnBnClickedButton2()
{

 }

/*
void CeDeuProjectDlg::OnBnClickedButtonDb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Listbox.InsertString(0, _T("USER : Save to DB Image..."));
	m_Listbox.InsertString(0, _T("Saved"));

	dbfilename = cv::format("base%d.jpg", db_count);
	cvSaveImage(dbfilename.c_str(), roi_image);

	GetDlgItemText(IDC_STATUS, DBstr[dbstridx]);
	if (!DBstr[dbstridx])
	{
		exit(-1);
	}
	m_Listbox3.AddString(DBstr[dbstridx]);
	

	db_count++;
	dbstridx++;
	captureimg = cvLoadImage("dbfilename");
	UpdateData(FALSE);
}
*/

void CeDeuProjectDlg::OnBnClickedButtCapture() // 매칭하기위한 캡쳐
{
	m_Listbox.AddString(_T("USER : Capture 버튼 클릭"));
	Mat captureimage;
	cam >> captureimage;
	filename = cv::format("capture%d.jpg", count); // 
	imwrite(filename.c_str(),captureimage);
	m_Listbox.AddString(_T("저장완료"));
}

void CeDeuProjectDlg::OnBnClickedCheck5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CeDeuProjectDlg::OnBnClickedonscreen2()
{
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CeDeuProjectDlg::OnBnClickedFacecheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CeDeuProjectDlg::OnBnClickedEyecheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CeDeuProjectDlg::OnBnClickedLoicheck() // 얼굴 검출 했을때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CeDeuProjectDlg::OnBnClickedButtExit()
{
	OnOK();
}

void CeDeuProjectDlg::OnBnClickedButtInput() // 이름입력 버튼 눌렀을때
{
	GetDlgItem(IDC_CAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_DETECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DECTSCREEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC)->EnableWindow(TRUE);
	GetDlgItem(IDC_onscreen2)->EnableWindow(TRUE);
	GetDlgItem(IDC_FACECHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOICHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DBLIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_DBTEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DBNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTT_DB)->EnableWindow(TRUE);
	GetDlgItem(IDC_SYSTEM)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATUS)->EnableWindow(TRUE);
	GetDlgItem(IDC_S_NAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTT_INPUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_B_MATCHING)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIME)->EnableWindow(TRUE);
	
	
	//GetDlgItem(IDC_CULCHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTT_CAPTURE)->EnableWindow(TRUE);

	GetDlgItemText(IDC_EDIT1, username); // 이름입력칸의 에디트컨트롤의 텍스트를 받아와서 DBstr에 저장한다
	MessageBox(_T("접속한 사용자 이름 :")+username);
	m_Listbox.AddString(_T("***** 준비가 완료되었습니다. 다음 메뉴얼을 따르세요. *****"));
	m_Listbox.AddString(_T("    1. [얼굴 인식 스크린 준비]를 체크하여 스크린을 켜십시오."));
	m_Listbox.AddString(_T("    2. [얼굴 인식 작동]을 체크하여 얼굴을 인식하십시오."));
	m_Listbox.AddString(_T("    3. 얼굴인식이 되는 중이라면 사용자의 얼굴에 사각형이 표시됩니다."));
	m_Listbox.AddString(_T("    4. 사각형이 얼굴에 알맞게 표시되었다면 [인식된 관심 영역 보기]를 체크하세요."));
	m_Listbox.AddString(_T("    5. 정확한 매칭을 위하여 관심영역에서 얼굴이 벗어나면 안됩니다."));
	m_Listbox.AddString(_T("    6. 관심영역에 자신의 얼굴이 잘 나왔다면, [DB에 저장] 버튼을 3번 눌러주세요"));
	m_Listbox.AddString(_T("    7. 3번 저장하셨다면 [얼굴인식작동] [인식된 관심영역보기] 체크를 해제 해주세요."));
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CeDeuProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		Invalidate(FALSE);
		break;
	case 2:
		CTime timer;
		timer = CTime::GetCurrentTime();
		CString str;
		str = timer.Format("%H:%M:%S");
		m_Time.SetWindowText(str);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CeDeuProjectDlg::OnBnClickedButtDb() //DB저장 버튼 누를때마다 , userDBfile 생성 
{
	if (db_count < 3) {
		m_Listbox.AddString(_T("사용자 이미지 DB 저장중..."));
		dbfilename = cv::format("userDBfile%d.jpg", db_count); // 저장될 파일이름 지정
		imwrite(dbfilename.c_str(), image_roi);// 파일저장
		GetDlgItemText(IDC_DBNAME, DBstr[dbstridx]); // DB 이름에 적혀진 텍스트를 DBSTR로 저장,DBSTRIDX는 저장된 개수를 의미.
		if (!DBstr[dbstridx])
		{
			exit(-1);
		}
		m_Listbox3.AddString(DBstr[dbstridx]); // DB 스트링박스에 저장,
		UpdateData(TRUE);
		db_count++;
		dbstridx++;
		captureimg = cvLoadImage("dbfilename"); // 캡쳐화면
		m_Listbox.AddString(_T("저장완료"));
		UpdateData(TRUE);
	}
	else{ // DB에 3개 이상의 이미지를 저장하려고 시도할시,
		m_Listbox.AddString(_T("DB에 공간이 없습니다"));
		UpdateData(TRUE);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CeDeuProjectDlg::OnBnClickedBMatching()
{
	// 창 띄우기위한 윈도우 이름 지정
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;
	int lx, ly, rx, ry;

	templ = imread("userDBfile0.jpg"); // Mat 형 이미지매칭파일에 DB파일 불러옴 (잘린얼굴)
	if (templ.empty()) {
		MessageBox(_T("캡쳐파일 불러오기 실패"));
	}
	img = imread("capture0.jpg"); // MAT형 소스 파일에 전체얼굴 캡쳐 파일 불러옴(전체 화면얼굴)
	if (img.empty()) {
		MessageBox(_T("DB파일 불러오기 실패"));
	}
	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	Mat img_display;
	img.copyTo(img_display);
	int result_cols = 0;// img.cols - templ.cols + 1;
	int result_rows = 0; //img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	matchTemplate(img, templ, result, TM_CCORR);//원본이미지,템플릿이미지 , 결과,match_method (트랙바))
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat()); // 결과값 정규화
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = maxLoc;
	//rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	lx = matchLoc.x+templ.cols;
	ly = matchLoc.y; +templ.rows;
	CString mestr;
	//mestr.Format(_T("lx: %d ly:%d "), lx,ly);
	//MessageBox(mestr);
	imshow(image_window, img_display);
	imshow(result_window, result);

	if ( lx<200 || lx > 400) {
		m_Listbox.AddString(_T("매칭실패"));
		GetDlgItem(IDC_CULCHECK)->EnableWindow(FALSE);

	}
	else {
		m_Listbox.AddString(_T("매칭성공"));
		GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE);
		ismatch = true;
	}
	waitKey(0);
	//DB 카운트의 크기만큼 이미지 생성하고 다 알맞게 불러와야함.
//	 machingFile1,2,3 이 DB에 저장된 이미지를 불러올 변수이다.(MAT형)
	/*
	char* image_window = "Source Image";
	char* result_window = "Result window";
	// 창 띄우기위한 윈도우 이름 지정
	
	machingFile1 = imread("userDBfile.jpg"); // Mat 형 이미지매칭파일에 DB파일 불러옴 (잘린얼굴)
	if (machingFile1.empty()) {
		MessageBox(_T("DB파일 불러오기 실패"));}
	Mat srcImage = imread("capture.jpg"); // MAT형 소스 파일에 전체얼굴 캡쳐 파일 불러옴(전체 화면얼굴)
		if (srcImage.empty()) {
			MessageBox(_T("캡쳐파일 불러오기 실패"));}

	Mat result;
	int match_method;
	int max_Trackbar = 5;
	
	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	Mat dstImage;

	cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	
	 //정규화된 제곱차 매칭
	matchTemplate(image_roi, machingFile1, result, CV_TM_SQDIFF);
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);
	rectangle(dstImage, maxLoc,
	Point(maxLoc.x + machingFile1.cols, maxLoc.y + machingFile1.rows), Scalar(0, 0, 255), 2);
	imshow("dstImage", dstImage);
	waitKey();

	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	
	for (y = 0; y < result.rows; y++)
	{
		for (x = 0; x < result.cols; x++)
		{
			value = result.at<float>(y, x);

			if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
			{
				threshold = minVal;

				if (value < threshold)
				{
					printf("label[%d] [%d,%d] : %f\n", label, x, y, value);
					matchLoc.x = x;
					matchLoc.y = y;
					// Show me what you got
					rectangle(m_matching_img, matchLoc, Point(matchLoc.x + tempImg.cols, matchLoc.y + tempImg.rows), S_GREEN, 2, 8, 0);
					putText(m_matching_img, to_string(label), Point(matchLoc.x - 10, matchLoc.y - 10), FONT_HERSHEY_SIMPLEX, 1, S_RED, 2);
				}
			}
			else
			{
				threshold = maxVal;

				if (value > threshold)
				{
					printf("label[%d] [%d,%d] : %f\n", label, x, y, value);
					matchLoc.x = x;
					matchLoc.y = y;
					// Show me what you got
					rectangle(m_matching_img, matchLoc, Point(matchLoc.x + tempImg.cols, matchLoc.y + tempImg.rows), S_GREEN, 2, 8, 0);
					putText(m_matching_img, to_string(label), Point(matchLoc.x - 10, matchLoc.y - 10), FONT_HERSHEY_SIMPLEX, 1, S_RED, 2);
				}
			}
		}
	}
	*/


	

}

/*
int CeDeuProjectDlg::TemplateMatching(Mat oriImg, Mat tempImg, int label)
{
	if (!oriImg.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	if (!tempImg.data) // Check for invalid input
	{
		cout << "Could not open or find the templet image" << endl;
		return -1;
	}

	//0 SQDIFF
	//1 SQDIFF NORMED
	//2 TM CCORR
	//3 TM CCORR NORMED
	//4 TM COEFF
	//5 TM COEFF NORMED
	// Load Method
	int match_method = 1;
	Mat result;
	// Create the result matrix
	int result_cols = oriImg.cols - tempImg.cols + 1;
	int result_rows = oriImg.rows - tempImg.rows + 1;

	//result.create(result_rows, result_cols, CV_32FC1);
	result.create(result_rows, result_cols, CV_32FC1);

	// Do the Matching and Normalize
	matchTemplate(m_working_img, tempImg, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	float threshold, minVal, maxVal;
	float value;
	int x, y;
	Point matchLoc;

	minVal = 0.1;
	maxVal = 0.9;

	for (y = 0; y < result.rows; y++)
	{
		for (x = 0; x < result.cols; x++)
		{
			value = result.at<float>(y, x);

			if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
			{
				threshold = minVal;

				if (value < threshold)
				{
					printf("label[%d] [%d,%d] : %f\n", label, x, y, value);
					matchLoc.x = x;
					matchLoc.y = y;
					// Show me what you got
					rectangle(m_matching_img, matchLoc, Point(matchLoc.x + tempImg.cols, matchLoc.y + tempImg.rows), S_GREEN, 2, 8, 0);
					putText(m_matching_img, to_string(label), Point(matchLoc.x - 10, matchLoc.y - 10), FONT_HERSHEY_SIMPLEX, 1, S_RED, 2);
				}
			}
			else
			{
				threshold = maxVal;

				if (value > threshold)
				{
					printf("label[%d] [%d,%d] : %f\n", label, x, y, value);
					matchLoc.x = x;
					matchLoc.y = y;
					// Show me what you got
					rectangle(m_matching_img, matchLoc, Point(matchLoc.x + tempImg.cols, matchLoc.y + tempImg.rows), S_GREEN, 2, 8, 0);
					putText(m_matching_img, to_string(label), Point(matchLoc.x - 10, matchLoc.y - 10), FONT_HERSHEY_SIMPLEX, 1, S_RED, 2);
				}
			}
		}
	}

	BOOL chk = IsDlgButtonChecked(IDC_CHECK_MATCH);
	if (chk != 0)
	{
		namedWindow("Matching Image", WINDOW_AUTOSIZE);     // Create a window for display
		imshow("Matching Image", m_matching_img);           // Show our image inside it
	}

	return 1;
}
*/

void CeDeuProjectDlg::OnBnClickedCulcheck() //출석체크 (서버에 접속요청)
{
	UpdateData(TRUE);
	if (m_iType == 0) //서버일경우 , 강의시작(서버열기)
	{
		m_pServerSock = new CServerSock();    //m_pServerSock Class 만큼의 동적할당
		m_pServerSock->Create(m_iPort);    // 서버를 만든다.  (포트)
		m_pServerSock->Listen();            //수신감지모드
		m_ConCheck = 1;
		GetDlgItem(IDC_LIST_CHATRECEIVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MESSAGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_HEXIT)->ShowWindow(SW_SHOW); //강

		GetDlgItem(IDC_CLIENT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT4)->ShowWindow(SW_SHOW);

		SetWindowPos(NULL, 1340, 380, 600, 670, SWP_NOREPOSITION);
		
	}
	else if (m_iType == 1)  //클라이언트 일경우(접속시도)
	{
			GetDlgItemText(IDC_EDIT1, m_strID); // 입력한 이름이 ID로 들어간다.
			GetDlgItem(IDC_LIST_CHATRECEIVE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MESSAGE)->ShowWindow(SW_SHOW);
			m_pClientSock = new CClientSockClient();      //m_ClientSock 만큼의 동적할당
			m_pClientSock->Create();              // 만든다. 클라이언트를
			m_pClientSock->Connect(m_strServer, m_iPort);    //  접속 (서버, 포트)
			m_ConCheck = 1;
			SendInfo(m_ConCheck);
			GetDlgItem(IDC_CAM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DETECT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DECTSCREEN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_onscreen2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FACECHECK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LOICHECK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DBLIST)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DBTEXT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DBNAME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTT_DB)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SYSTEM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_S_NAME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTT_INPUT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_B_MATCHING)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TIME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTT_CAPTURE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_HEXIT)->ShowWindow(SW_SHOW); //강
			SetWindowPos(NULL, 1555,400, 370, 650, SWP_NOREPOSITION);
			

	}
}

void CeDeuProjectDlg::setup() {

	
	
}


/*
void CeDeuProjectDlg::MatchingMethod(int,void*)
{
	
	result.create(result_rows, result_cols, CV_32FC1);
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) { 
		matchLoc = minLoc; }
	else {
		matchLoc = maxLoc;
	} 
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0); 
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0); 
	imshow(image_window, img_display); imshow(result_window, result);
	return;

}

*/















/*
void CeDeuProjectDlg::OnBnClickedRadioTypeS() // 라디오 버튼
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_iType == 0)   //서버선택시
	{	
		SetDlgItemText(IDC_CULCHECK, "강의 시작");    //버튼이 리슨으로 바뀜
		GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE); //강의시작버튼 활성화
		GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_SHOW); //강의시작버튼 활성화
	}
	m_ConCheck = 0;
}


void CeDeuProjectDlg::OnBnClickedRadioTypeC()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_iType == 1)  //클라이언트 선택시
	{
		//SetDlgItemText(IDC_CULCHECK, "출석체크");  //버튼이 컨넥으로 바뀜
	}
	m_ConCheck = 0;
}
*/
/*
void CeDeuProjectDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_iType == 0) //서버일경우
	{
		m_pServerSock = new CServerSock();    //m_pServerSock Class 만큼의 동적할당
		m_pServerSock->Create(m_iPort);    // 서버를 만든다.  (포트)
		m_pServerSock->Listen();            //수신감지모드

		GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_S)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_C)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);

		m_ConCheck = 1;
	}
	else if (m_iType == 1)  //클라이언트 일경우
	{
		if (m_strID == "")
		{
			AfxMessageBox("ID를 입력해주세요");
		}
		else
		{
			// m_strID += " ";
			m_pClientSock = new CClientSockClient();      //m_ClientSock 만큼의 동적할당
			m_pClientSock->Create();              // 만든다. 클라이언트를
			m_pClientSock->Connect(m_strServer, m_iPort);    //  접속 (서버, 포트)
			GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPE_S)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPE_C)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
			m_ConCheck = 1;
			SendInfo(m_ConCheck);
		}
	}
	else
	{
		AfxMessageBox("서버 또는 클라이언트를 체크하세요");
	}
}
*/



void CeDeuProjectDlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	char buf[200];   //버퍼를 만들고

	if (m_ConCheck == 1)  //접속 활성화 여부를 확인후
	{
		if (m_iType == 0)    //서버라면
		{
			TRACE("CeDeuProject::OnSend()\n");
			strcpy(buf, "교사 : ");  //버퍼에 서버 글자를 붙이고
			strcat(buf, (const char*)m_strMessage);  //메시지와 합쳐서
			Send(buf);   // 센드 함수로 버퍼를 보낸다.
			m_lChatReceive.AddString(buf);  // 리스트박스에 집어넣는다.
		}

		else if (m_iType == 1)  //클라이언트라면
		{
			strcpy(buf, (const char*)m_strID);   // 버퍼에 아이디를 붙이고
			strcat(buf, " : ");      // : 붙이고
			strcat(buf, (const char*)m_strMessage);  // 메시지를 붙여서
			m_pClientSock->Send(buf, strlen(buf) + 1);    //클라이언트 소켓의 Send 함수 호출
			m_lChatReceive.AddString(buf);
		}
		else
		{
			AfxMessageBox("Type Server/Client를 체크해주세요.");
			m_ConCheck = 0;
		}
	}
	else
	{
		AfxMessageBox("Listen 또는 Connect 버튼을 눌러주세요.");
		m_ConCheck = 0;
	}
}

void CeDeuProjectDlg::OnAccept()  
{
	CString str1;
	str1 = "Connect Success";   //접속 성공 문구

	TRACE("CeDeuProjectDlg::OnAccept()\n");
	CClientSockServer* p = new CClientSockServer;       // pointer p 에다가 Socket Class 만큼 할당해준다.
	m_pServerSock->Accept(*p);      //서버소켓에 접속
	p->Send(str1, strlen(str1) + 1);   //클라이언트에 보낸다. 문자열과 문자열길이를 파악해서
	m_ClientSock.AddTail(p);        //마지막 노드에 새로운 노드를 추가한다.?? 클라이언트가 쭈욱 새로접속을 시도 할테니까?
}

void CeDeuProjectDlg::OnReceiveS(CClientSockServer* pSock)  //서버 Receive (서버가 문자를 받을때)
{
	TRACE("CeDeuProjectDlg::OnReceiveS()\n");
	char buf[200];     // 버퍼를 만들고
	pSock->Receive(buf, 200);         // 클라이언트에서 받은 문자열을 버퍼에 저장
	int chk = 0;
	if (strncmp(buf, "******", 5) == 0) {
		count_cli[c] = (CString)buf;
		c = c + 1;
		for (int j = 0; j < c+1; j++) {
			if (strcmp(buf, count_cli[j]) == 0 && strlen(buf) < 35) {
				switch (j) {
				case 0:
					icon1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_CON));
					m_client1.SetBitmap(icon1);
					chk = 1;
					break;
				case 1:
					icon2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_CON));
					m_client2.SetBitmap(icon2);
					chk = 1;
					break;
				case 2:
					icon3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_CON));
					m_client3.SetBitmap(icon3);
					chk = 1;
					break;
				case 3:
					icon4 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_CON));
					m_client4.SetBitmap(icon4);
					chk = 1;
					break;
				}
			}
		}
		if (chk == 0) {
			for (int i = 0; i < c+1; i++) {
				if (strncmp(count_cli[i], buf, 13) == 0) {
					switch (i) {
					case 0:
						icon1 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
						m_client1.SetBitmap(icon1);
						break;
					case 1:
						icon2 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
						m_client2.SetBitmap(icon2);
						break;
					case 2:
						icon3 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
						m_client3.SetBitmap(icon3);
						chk = 1;
						break;
					case 3:
						icon4 = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DIS));
						m_client4.SetBitmap(icon4);
						chk = 1;
						break;
					}
					break;
				}
			}
		}
		}

		m_lChatReceive.AddString(buf);       //저장된 문자열을 서버 리스트박스에 추가하고
											 //Send(buf);     //클라이언트로부터 받은 메시지를 Send 함수를 통해서 각 클라이언트에 다시 뿌린다.

											 //스레드를 활용한 로그 파일 기록
		log1.str = buf;        // 스레드 인자 1 (메시지)
		log1.type = m_iType;   // 스레드 인자 2  (서버,클라이언트 타입 구분)
		AfxBeginThread(OnLogtext, (LPVOID)&log1);      // 스레드 선언 (OnLogText 함수를 , log1 변수 사용);
		
		chk = 0;
		Invalidate(FALSE);
}

void CeDeuProjectDlg::Send(char *buf)   // 서버가 클라이언트들에게 메시지를 전달
{
	POSITION pos = m_ClientSock.GetHeadPosition();    // Client Sock Position을 확인한다.   (각 클라리언트의 주소 위치?)
	while (pos != NULL)         // 클라이언트의 갯수에 따라?
	{
		CClientSockServer* p = m_ClientSock.GetNext(pos);   // 각 클라이언트의 포지션에 총 갯수만큼   GetNext가 다 떨어질때까지???
		p->Send(buf, strlen(buf) + 1);                    // 보낸다.
	}
}

void CeDeuProjectDlg::OnReceiveC()       // 클라이언트가 서버에서 메시지를 받을때마다 자동으로 호출되어
{
	char buf[200];       //버퍼를 만들고
	m_pClientSock->Receive(buf, 200);         // 버퍼에 메시지를 저장하고
	m_lChatReceive.AddString(buf);  // 리스트박스에 집어넣는다.
	
	if (strcmp(buf,"교사 : stop") == 0) {
		cursorstop = TRUE;
	}
	else if (strcmp(buf, "교사 : start") == 0) {
		cursorstop = FALSE;
	}
}

void CeDeuProjectDlg::SendInfo(int con)    //서버에 접속 메시지 전달
{
	CString str1;
	str1 = "******";
	if (con)
	{
		str1 += m_strID;
		str1 += " 학생 출석 확인******";
		m_pClientSock->Send(str1, strlen(str1) + 1);
	}
	else
	{
		str1 += m_strID;
		str1 += " 학생이 퇴장했습니다******";
		m_pClientSock->Send(str1, strlen(str1) + 1);
	}
}

UINT CeDeuProjectDlg::OnLogtext(LPVOID log)
{
	CTime timer;                          // 타이머를 불러옴
	timer = CTime::GetCurrentTime();     //시간 집어넣고
	CString strTime;                      //시간을 넣은 문자열
	CString str;                          //메시지 문자열
	int type;                             //서버에 있는것만 구분해서 저장해야 하기 때문에 type 구별용 int 변수
	strTime = timer.Format(" [%H:%M:%S]\n");      // ex)   "안녕하세요 이건 문자열입니다. [12:30:13]"  처럼 문자열 뒤에 시간을 붙이려고 만듬

	ThreadArg* test;                      //구조체를 포인터로 하나 만들어서
	test = (ThreadArg*)log;                //스레드에 딸려온 구조체 변수를 형변환 해서

	str = test->str;                      //문자열 변수에 집어넣고
	type = test->type;                    //type에 집어 넣는다.

	if (type == 0)   //서버일 경우
	{
		CStdioFile log1;   //표준파일입출력
		if (log1.Open("log1.txt", CFile::modeCreate | CFile::typeText | CFile::modeNoTruncate | CFile::modeWrite))  //open(파일이름,  생성모드, 글자, 수정, 만들기?)  정도로 이해햇음
		{
			str += strTime;        // 메시지에 시간을 붙임
								   //str.Replace("\r\n","\n");
			log1.SeekToEnd();    //텍스트파일 맨끝으로 가서
			log1.WriteString(str);   //이어 붙임
			log1.Close();   //닫음
		}
		else
			AfxMessageBox("Log Write Fail");   //에러일 경우 메시지박스
	}

	return 0;   // 스레드는 반환 타입이 있어야 하므로
}

void CeDeuProjectDlg::OnBnClickedMbServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iType = 0;
	SetDlgItemText(IDC_CULCHECK, "강의 시작");    //버튼이 리슨으로 바뀜
	GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE); //강의시작버튼 활성화
	GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_SHOW); //강의시작버튼 활성화
	GetDlgItem(IDC_MB_SERVER)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_MB_CLIENT)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_EDIT_SERVER)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_HIDE); //

	
	m_ConCheck = 0;
}


void CeDeuProjectDlg::OnBnClickedMbClient()
{
	UpdateData(TRUE);
	m_iType = 1;  //클라이언트 선택시
	GetDlgItem(IDC_CAM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DETECT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DECTSCREEN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_onscreen2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FACECHECK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOICHECK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DBLIST)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DBTEXT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DBNAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTT_DB)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SYSTEM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATUS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_S_NAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTT_INPUT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_B_MATCHING)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_TIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTT_CAPTURE)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_MB_SERVER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MB_CLIENT)->ShowWindow(SW_HIDE);
	m_ConCheck = 0;
}


void CeDeuProjectDlg::OnBnClickedHexit()
{
	OnOK();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
