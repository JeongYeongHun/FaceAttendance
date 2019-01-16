
// eDeuProjectDlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CeDeuProjectDlg ��ȭ ����


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

CeDeuProjectDlg::~CeDeuProjectDlg() //�Ҹ���
{
	if (m_ConCheck == 1)  //���� �Ǵ� Ŭ���̾�Ʈ�� Ȱ��ȭ ������ ���
	{
		if (m_iType == 0)
		{
			delete m_pServerSock;  //������  ������ �ޱ����� ������� m_pServerSock Ŭ������ ��ȯ
			POSITION pos = m_ClientSock.GetHeadPosition();    // ������  ��������� Ŭ���̾�Ʈ���� �ּҰ��� �ٸ��� �� ������ ����ŭ (������ �� * ClientSock Class)
			while (pos != NULL) {         //������             //��� �ݺ� pos ���� �����������
				delete m_ClientSock.GetNext(pos);   //������          // Ŭ���̾�Ʈ ���ڸ�ŭ ��ȯ
			}
		}
		else {
			m_ConCheck = 0;   //���� üũ
			SendInfo(m_ConCheck); //����޽��� ���� �Լ�
			delete m_pClientSock;   // Ŭ���̾�Ʈ Ŭ�� �Ҵ�����   // ���ú��?
		}
	}
}

void CeDeuProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM, m_cam); // 1�� ķ��ũ��
	DDX_Control(pDX, IDC_DETECT, m_dscreen2); // 2�� �󱼿��� ��ũ�� 
	DDX_Control(pDX, IDC_DECTSCREEN, m_DectFaceScreen); // 3�� �󱼸� ������ ��ũ��
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


// CeDeuProjectDlg �޽��� ó����

BOOL CeDeuProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	cam.open(0);

	if (!cam.isOpened())
		MessageBox(_T("ķ�� ���� �����ϴ�. \n"));
	else 
		MessageBox(_T("ī�޶� ���� �Ǿ����ϴ�"));

	if (!cascade_face.load(haarcascade_face)) //Face Cascade.xml üũ
	{
		std::cout << "ERROR: Could not load face_classifier cascade" << std::endl;
	}
	else
	{
		MessageBox(_T("haarcascade_face ����"));
	}
	if (!cascade_eye.load(haarcascade_eye)) //Eye Cascade.xml üũ
	{
		std::cout << "ERROR: Could not load eye_classifier cascade" << std::endl;
	}
	else
	{
		MessageBox(_T("haarcascade_eye ����"));
	}
	
	m_ConCheck = 0;     //ó�� ���� �ɶ�  �ʱ�ȭ
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

	GetDlgItem(IDC_HEXIT)->ShowWindow(SW_HIDE); //��
	
	m_Listbox.AddString(_T("***** �����ϱ����� �����ϴܿ��� �̸��� �Է����ּ��� *****"));



	UpdateData(TRUE);
	m_strServer = "127.0.0.1";   //IP
	m_iPort = 700;    //PORT
	m_strID = "";   //ID
	UpdateData(FALSE);    //�����ڽ� ����

	SetTimer(2, 1000, NULL);   // Timer(�ð�)

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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CeDeuProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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
		
		pDC3 = m_DectFaceScreen.GetDC(); // �󱼿������ѷ���  ��ũ���� DC�� �����´�
		m_DectFaceScreen.GetClientRect(&rect3); // rect3�� m_dectface �� ������Ʈ�� ũ�� ����

		pDC2 = m_dscreen2.GetDC(); // �� ���� �ν��ϴ� ������ ��Ÿ��
		m_dscreen2.GetClientRect(&rect2); // rect2 �� m_dscreen2 �� ������Ʈ�� ũ�� ����

		pDC = m_cam.GetDC(); // ķ�� ������ ������Ʈ�� DC�� �����´�.
		m_cam.GetClientRect(&rect); // rect �� ������Ʈ�� ũ�� ����

		UpdateData(TRUE);

		if (this->cam.isOpened()) { // ķ������������? 
			cam >> frame; // frame�̶�� matimage ������ �̹��� ����
			SetTimer(1, 30, NULL);
		}
		DisplayImage(&frame, pDC, rect); // mat �� �̹����� �ش� ��Ʈ�ѿ� �ѷ��ټ��ְ� ���Ƿ� �ۼ��� �Լ�
		
		if(m_screen2onoff.GetCheck()==BST_CHECKED){  // ��ũ�� 2 ������
			UpdateData(TRUE);
			Mat frame_gray; // ����ȭ�� �̹����� �� mat�̹�����������
			cvtColor(frame,frame_gray,COLOR_BGR2GRAY); // ����ȭ ���Ѽ� frame_gray�� ���
			equalizeHist(frame_gray, frame_gray); // �̹����� ������׷�ȭ ��Ų��(��������)
			
			if (m_facecheck.GetCheck() == BST_CHECKED) { // ���ν� ������
				UpdateData(TRUE);
				cascade_face.detectMultiScale(frame_gray, faces, 1.1, 3, 3 | CASCADE_SCALE_IMAGE, Size(30, 30));
				//HAAR cascade ���̺귯���� �̿��� �� ���� �ν��Ͽ� faces��� �̸��� rect�� ��´�.
				cascade_eye.detectMultiScale(frame_gray, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(3, 3));
				// HAAR cascade ���̺귯���� �̿��� �� ���� �ν��Ͽ� eyes��� �̸��� rect�� ��´�.

				for (int roi_i = 0; roi_i < faces.size(); roi_i++) { // ���ν� ���⿵�� ������ŭ �簢�� �׸���
						//Point center(faces[roi_i].x + faces[roi_i].width / 2, faces[roi_i].y + faces[roi_i].height / 2); // ���ν�Ÿ���� �߾�����Ʈ
						//ellipse(frame, center, Size(faces[roi_i].width / 2, faces[roi_i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
					rectangle(frame, faces[roi_i], Scalar(0, 0, 0), 2); // ���� frame(ķ����)�� �簢���� �׷����� �κ�
					/*cvRectangle(&frame, Point(faces[roi_i].x , faces[roi_i].y),
					Point(faces[roi_i].x +faces[roi_i].width,
					faces[roi_i].y + faces[roi_i].height), cvScalar(255, 255, 255), 1, 0, 0);*/
				}			//Ÿ���׸���,frame����.
		/*if(m_eyecheck.GetCheck()==BST_CHECKED){
			if (m_eye == TRUE) { // ��,�� �ν� Ȱ��ȭ
				Mat faceROI = frame_gray(faces[roi_i]);  // �� �νĵ� Mat�� faceROI �̹��� ����.
				cascade_eye.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(3, 3));
					for (size_t j = 0; j < eyes.size(); j++) {
					Point eye_center(eyes[j].x + eyes[j].x + eyes[j].width / 2, eyes[roi_i].y + eyes[j].y + eyes[j].height / 2);
					int radius = cvRound((eyes[roi_i].width + eyes[roi_i].height)*0.25);
					circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
					DisplayImage(&frame, pDC2, rect2); // mat �� �̹����� �ش� ��Ʈ�ѿ� �ѷ��ټ��ְ� ���Ƿ� �ۼ��� �Լ�
					}
			}
}*/			
				DisplayImage(&frame, pDC2, rect2); // mat �� �̹����� �ش� ��Ʈ�ѿ� �ѷ��ټ��ְ� ���Ƿ� �ۼ��� �Լ�
				}
			else {
				DisplayImage(&frame_gray, pDC2, rect2); // mat �� �̹����� �ش� ��Ʈ�ѿ� �ѷ��ټ��ְ� ���Ƿ� �ۼ��� �Լ�
			}
			
			if (m_roicheck.GetCheck() == BST_CHECKED) // �󱼿��� ĸ�Ľ���
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
		////////////////////////////////////////�� �ν� ���� �� ////////////////////////////////////

	
	
	


// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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


void CeDeuProjectDlg::DrawCvMat(CDC* pDC, cv::Mat& origin, CRect rect) { // �̹����� dcũ�⿡ �°� ����
	CImage mfcImg;
	cv::Mat outImg;
	cv::flip(origin, outImg, 0);
	//����̸� ä����3����
	if (outImg.channels() == 1) {
		cv::cvtColor(outImg, outImg, CV_GRAY2BGR);
	}
	//�̹����� ũ�⸦ rect�� ���� �����մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CeDeuProjectDlg::OnBnClickedButton2()
{

 }

/*
void CeDeuProjectDlg::OnBnClickedButtonDb()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

void CeDeuProjectDlg::OnBnClickedButtCapture() // ��Ī�ϱ����� ĸ��
{
	m_Listbox.AddString(_T("USER : Capture ��ư Ŭ��"));
	Mat captureimage;
	cam >> captureimage;
	filename = cv::format("capture%d.jpg", count); // 
	imwrite(filename.c_str(),captureimage);
	m_Listbox.AddString(_T("����Ϸ�"));
}

void CeDeuProjectDlg::OnBnClickedCheck5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CeDeuProjectDlg::OnBnClickedonscreen2()
{
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CeDeuProjectDlg::OnBnClickedFacecheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CeDeuProjectDlg::OnBnClickedEyecheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CeDeuProjectDlg::OnBnClickedLoicheck() // �� ���� ������
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CeDeuProjectDlg::OnBnClickedButtExit()
{
	OnOK();
}

void CeDeuProjectDlg::OnBnClickedButtInput() // �̸��Է� ��ư ��������
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

	GetDlgItemText(IDC_EDIT1, username); // �̸��Է�ĭ�� ����Ʈ��Ʈ���� �ؽ�Ʈ�� �޾ƿͼ� DBstr�� �����Ѵ�
	MessageBox(_T("������ ����� �̸� :")+username);
	m_Listbox.AddString(_T("***** �غ� �Ϸ�Ǿ����ϴ�. ���� �޴����� ��������. *****"));
	m_Listbox.AddString(_T("    1. [�� �ν� ��ũ�� �غ�]�� üũ�Ͽ� ��ũ���� �ѽʽÿ�."));
	m_Listbox.AddString(_T("    2. [�� �ν� �۵�]�� üũ�Ͽ� ���� �ν��Ͻʽÿ�."));
	m_Listbox.AddString(_T("    3. ���ν��� �Ǵ� ���̶�� ������� �󱼿� �簢���� ǥ�õ˴ϴ�."));
	m_Listbox.AddString(_T("    4. �簢���� �󱼿� �˸°� ǥ�õǾ��ٸ� [�νĵ� ���� ���� ����]�� üũ�ϼ���."));
	m_Listbox.AddString(_T("    5. ��Ȯ�� ��Ī�� ���Ͽ� ���ɿ������� ���� ����� �ȵ˴ϴ�."));
	m_Listbox.AddString(_T("    6. ���ɿ����� �ڽ��� ���� �� ���Դٸ�, [DB�� ����] ��ư�� 3�� �����ּ���"));
	m_Listbox.AddString(_T("    7. 3�� �����ϼ̴ٸ� [���ν��۵�] [�νĵ� ���ɿ�������] üũ�� ���� ���ּ���."));
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CeDeuProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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


void CeDeuProjectDlg::OnBnClickedButtDb() //DB���� ��ư ���������� , userDBfile ���� 
{
	if (db_count < 3) {
		m_Listbox.AddString(_T("����� �̹��� DB ������..."));
		dbfilename = cv::format("userDBfile%d.jpg", db_count); // ����� �����̸� ����
		imwrite(dbfilename.c_str(), image_roi);// ��������
		GetDlgItemText(IDC_DBNAME, DBstr[dbstridx]); // DB �̸��� ������ �ؽ�Ʈ�� DBSTR�� ����,DBSTRIDX�� ����� ������ �ǹ�.
		if (!DBstr[dbstridx])
		{
			exit(-1);
		}
		m_Listbox3.AddString(DBstr[dbstridx]); // DB ��Ʈ���ڽ��� ����,
		UpdateData(TRUE);
		db_count++;
		dbstridx++;
		captureimg = cvLoadImage("dbfilename"); // ĸ��ȭ��
		m_Listbox.AddString(_T("����Ϸ�"));
		UpdateData(TRUE);
	}
	else{ // DB�� 3�� �̻��� �̹����� �����Ϸ��� �õ��ҽ�,
		m_Listbox.AddString(_T("DB�� ������ �����ϴ�"));
		UpdateData(TRUE);
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CeDeuProjectDlg::OnBnClickedBMatching()
{
	// â �������� ������ �̸� ����
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;
	int lx, ly, rx, ry;

	templ = imread("userDBfile0.jpg"); // Mat �� �̹�����Ī���Ͽ� DB���� �ҷ��� (�߸���)
	if (templ.empty()) {
		MessageBox(_T("ĸ������ �ҷ����� ����"));
	}
	img = imread("capture0.jpg"); // MAT�� �ҽ� ���Ͽ� ��ü�� ĸ�� ���� �ҷ���(��ü ȭ���)
	if (img.empty()) {
		MessageBox(_T("DB���� �ҷ����� ����"));
	}
	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	Mat img_display;
	img.copyTo(img_display);
	int result_cols = 0;// img.cols - templ.cols + 1;
	int result_rows = 0; //img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	matchTemplate(img, templ, result, TM_CCORR);//�����̹���,���ø��̹��� , ���,match_method (Ʈ����))
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat()); // ����� ����ȭ
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
		m_Listbox.AddString(_T("��Ī����"));
		GetDlgItem(IDC_CULCHECK)->EnableWindow(FALSE);

	}
	else {
		m_Listbox.AddString(_T("��Ī����"));
		GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE);
		ismatch = true;
	}
	waitKey(0);
	//DB ī��Ʈ�� ũ�⸸ŭ �̹��� �����ϰ� �� �˸°� �ҷ��;���.
//	 machingFile1,2,3 �� DB�� ����� �̹����� �ҷ��� �����̴�.(MAT��)
	/*
	char* image_window = "Source Image";
	char* result_window = "Result window";
	// â �������� ������ �̸� ����
	
	machingFile1 = imread("userDBfile.jpg"); // Mat �� �̹�����Ī���Ͽ� DB���� �ҷ��� (�߸���)
	if (machingFile1.empty()) {
		MessageBox(_T("DB���� �ҷ����� ����"));}
	Mat srcImage = imread("capture.jpg"); // MAT�� �ҽ� ���Ͽ� ��ü�� ĸ�� ���� �ҷ���(��ü ȭ���)
		if (srcImage.empty()) {
			MessageBox(_T("ĸ������ �ҷ����� ����"));}

	Mat result;
	int match_method;
	int max_Trackbar = 5;
	
	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	Mat dstImage;

	cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	
	 //����ȭ�� ������ ��Ī
	matchTemplate(image_roi, machingFile1, result, CV_TM_SQDIFF);
	minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);
	rectangle(dstImage, maxLoc,
	Point(maxLoc.x + machingFile1.cols, maxLoc.y + machingFile1.rows), Scalar(0, 0, 255), 2);
	imshow("dstImage", dstImage);
	waitKey();

	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	
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

void CeDeuProjectDlg::OnBnClickedCulcheck() //�⼮üũ (������ ���ӿ�û)
{
	UpdateData(TRUE);
	if (m_iType == 0) //�����ϰ�� , ���ǽ���(��������)
	{
		m_pServerSock = new CServerSock();    //m_pServerSock Class ��ŭ�� �����Ҵ�
		m_pServerSock->Create(m_iPort);    // ������ �����.  (��Ʈ)
		m_pServerSock->Listen();            //���Ű������
		m_ConCheck = 1;
		GetDlgItem(IDC_LIST_CHATRECEIVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MESSAGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_HEXIT)->ShowWindow(SW_SHOW); //��

		GetDlgItem(IDC_CLIENT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLIENT4)->ShowWindow(SW_SHOW);

		SetWindowPos(NULL, 1340, 380, 600, 670, SWP_NOREPOSITION);
		
	}
	else if (m_iType == 1)  //Ŭ���̾�Ʈ �ϰ��(���ӽõ�)
	{
			GetDlgItemText(IDC_EDIT1, m_strID); // �Է��� �̸��� ID�� ����.
			GetDlgItem(IDC_LIST_CHATRECEIVE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MESSAGE)->ShowWindow(SW_SHOW);
			m_pClientSock = new CClientSockClient();      //m_ClientSock ��ŭ�� �����Ҵ�
			m_pClientSock->Create();              // �����. Ŭ���̾�Ʈ��
			m_pClientSock->Connect(m_strServer, m_iPort);    //  ���� (����, ��Ʈ)
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
			GetDlgItem(IDC_HEXIT)->ShowWindow(SW_SHOW); //��
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
void CeDeuProjectDlg::OnBnClickedRadioTypeS() // ���� ��ư
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_iType == 0)   //�������ý�
	{	
		SetDlgItemText(IDC_CULCHECK, "���� ����");    //��ư�� �������� �ٲ�
		GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE); //���ǽ��۹�ư Ȱ��ȭ
		GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_SHOW); //���ǽ��۹�ư Ȱ��ȭ
	}
	m_ConCheck = 0;
}


void CeDeuProjectDlg::OnBnClickedRadioTypeC()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_iType == 1)  //Ŭ���̾�Ʈ ���ý�
	{
		//SetDlgItemText(IDC_CULCHECK, "�⼮üũ");  //��ư�� �������� �ٲ�
	}
	m_ConCheck = 0;
}
*/
/*
void CeDeuProjectDlg::OnBnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_iType == 0) //�����ϰ��
	{
		m_pServerSock = new CServerSock();    //m_pServerSock Class ��ŭ�� �����Ҵ�
		m_pServerSock->Create(m_iPort);    // ������ �����.  (��Ʈ)
		m_pServerSock->Listen();            //���Ű������

		GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_S)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_C)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);

		m_ConCheck = 1;
	}
	else if (m_iType == 1)  //Ŭ���̾�Ʈ �ϰ��
	{
		if (m_strID == "")
		{
			AfxMessageBox("ID�� �Է����ּ���");
		}
		else
		{
			// m_strID += " ";
			m_pClientSock = new CClientSockClient();      //m_ClientSock ��ŭ�� �����Ҵ�
			m_pClientSock->Create();              // �����. Ŭ���̾�Ʈ��
			m_pClientSock->Connect(m_strServer, m_iPort);    //  ���� (����, ��Ʈ)
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
		AfxMessageBox("���� �Ǵ� Ŭ���̾�Ʈ�� üũ�ϼ���");
	}
}
*/



void CeDeuProjectDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	char buf[200];   //���۸� �����

	if (m_ConCheck == 1)  //���� Ȱ��ȭ ���θ� Ȯ����
	{
		if (m_iType == 0)    //�������
		{
			TRACE("CeDeuProject::OnSend()\n");
			strcpy(buf, "���� : ");  //���ۿ� ���� ���ڸ� ���̰�
			strcat(buf, (const char*)m_strMessage);  //�޽����� ���ļ�
			Send(buf);   // ���� �Լ��� ���۸� ������.
			m_lChatReceive.AddString(buf);  // ����Ʈ�ڽ��� ����ִ´�.
		}

		else if (m_iType == 1)  //Ŭ���̾�Ʈ���
		{
			strcpy(buf, (const char*)m_strID);   // ���ۿ� ���̵� ���̰�
			strcat(buf, " : ");      // : ���̰�
			strcat(buf, (const char*)m_strMessage);  // �޽����� �ٿ���
			m_pClientSock->Send(buf, strlen(buf) + 1);    //Ŭ���̾�Ʈ ������ Send �Լ� ȣ��
			m_lChatReceive.AddString(buf);
		}
		else
		{
			AfxMessageBox("Type Server/Client�� üũ���ּ���.");
			m_ConCheck = 0;
		}
	}
	else
	{
		AfxMessageBox("Listen �Ǵ� Connect ��ư�� �����ּ���.");
		m_ConCheck = 0;
	}
}

void CeDeuProjectDlg::OnAccept()  
{
	CString str1;
	str1 = "Connect Success";   //���� ���� ����

	TRACE("CeDeuProjectDlg::OnAccept()\n");
	CClientSockServer* p = new CClientSockServer;       // pointer p ���ٰ� Socket Class ��ŭ �Ҵ����ش�.
	m_pServerSock->Accept(*p);      //�������Ͽ� ����
	p->Send(str1, strlen(str1) + 1);   //Ŭ���̾�Ʈ�� ������. ���ڿ��� ���ڿ����̸� �ľ��ؼ�
	m_ClientSock.AddTail(p);        //������ ��忡 ���ο� ��带 �߰��Ѵ�.?? Ŭ���̾�Ʈ�� �޿� ���������� �õ� ���״ϱ�?
}

void CeDeuProjectDlg::OnReceiveS(CClientSockServer* pSock)  //���� Receive (������ ���ڸ� ������)
{
	TRACE("CeDeuProjectDlg::OnReceiveS()\n");
	char buf[200];     // ���۸� �����
	pSock->Receive(buf, 200);         // Ŭ���̾�Ʈ���� ���� ���ڿ��� ���ۿ� ����
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

		m_lChatReceive.AddString(buf);       //����� ���ڿ��� ���� ����Ʈ�ڽ��� �߰��ϰ�
											 //Send(buf);     //Ŭ���̾�Ʈ�κ��� ���� �޽����� Send �Լ��� ���ؼ� �� Ŭ���̾�Ʈ�� �ٽ� �Ѹ���.

											 //�����带 Ȱ���� �α� ���� ���
		log1.str = buf;        // ������ ���� 1 (�޽���)
		log1.type = m_iType;   // ������ ���� 2  (����,Ŭ���̾�Ʈ Ÿ�� ����)
		AfxBeginThread(OnLogtext, (LPVOID)&log1);      // ������ ���� (OnLogText �Լ��� , log1 ���� ���);
		
		chk = 0;
		Invalidate(FALSE);
}

void CeDeuProjectDlg::Send(char *buf)   // ������ Ŭ���̾�Ʈ�鿡�� �޽����� ����
{
	POSITION pos = m_ClientSock.GetHeadPosition();    // Client Sock Position�� Ȯ���Ѵ�.   (�� Ŭ�󸮾�Ʈ�� �ּ� ��ġ?)
	while (pos != NULL)         // Ŭ���̾�Ʈ�� ������ ����?
	{
		CClientSockServer* p = m_ClientSock.GetNext(pos);   // �� Ŭ���̾�Ʈ�� �����ǿ� �� ������ŭ   GetNext�� �� ������������???
		p->Send(buf, strlen(buf) + 1);                    // ������.
	}
}

void CeDeuProjectDlg::OnReceiveC()       // Ŭ���̾�Ʈ�� �������� �޽����� ���������� �ڵ����� ȣ��Ǿ�
{
	char buf[200];       //���۸� �����
	m_pClientSock->Receive(buf, 200);         // ���ۿ� �޽����� �����ϰ�
	m_lChatReceive.AddString(buf);  // ����Ʈ�ڽ��� ����ִ´�.
	
	if (strcmp(buf,"���� : stop") == 0) {
		cursorstop = TRUE;
	}
	else if (strcmp(buf, "���� : start") == 0) {
		cursorstop = FALSE;
	}
}

void CeDeuProjectDlg::SendInfo(int con)    //������ ���� �޽��� ����
{
	CString str1;
	str1 = "******";
	if (con)
	{
		str1 += m_strID;
		str1 += " �л� �⼮ Ȯ��******";
		m_pClientSock->Send(str1, strlen(str1) + 1);
	}
	else
	{
		str1 += m_strID;
		str1 += " �л��� �����߽��ϴ�******";
		m_pClientSock->Send(str1, strlen(str1) + 1);
	}
}

UINT CeDeuProjectDlg::OnLogtext(LPVOID log)
{
	CTime timer;                          // Ÿ�̸Ӹ� �ҷ���
	timer = CTime::GetCurrentTime();     //�ð� ����ְ�
	CString strTime;                      //�ð��� ���� ���ڿ�
	CString str;                          //�޽��� ���ڿ�
	int type;                             //������ �ִ°͸� �����ؼ� �����ؾ� �ϱ� ������ type ������ int ����
	strTime = timer.Format(" [%H:%M:%S]\n");      // ex)   "�ȳ��ϼ��� �̰� ���ڿ��Դϴ�. [12:30:13]"  ó�� ���ڿ� �ڿ� �ð��� ���̷��� ����

	ThreadArg* test;                      //����ü�� �����ͷ� �ϳ� ����
	test = (ThreadArg*)log;                //�����忡 ������ ����ü ������ ����ȯ �ؼ�

	str = test->str;                      //���ڿ� ������ ����ְ�
	type = test->type;                    //type�� ���� �ִ´�.

	if (type == 0)   //������ ���
	{
		CStdioFile log1;   //ǥ�����������
		if (log1.Open("log1.txt", CFile::modeCreate | CFile::typeText | CFile::modeNoTruncate | CFile::modeWrite))  //open(�����̸�,  �������, ����, ����, �����?)  ������ ��������
		{
			str += strTime;        // �޽����� �ð��� ����
								   //str.Replace("\r\n","\n");
			log1.SeekToEnd();    //�ؽ�Ʈ���� �ǳ����� ����
			log1.WriteString(str);   //�̾� ����
			log1.Close();   //����
		}
		else
			AfxMessageBox("Log Write Fail");   //������ ��� �޽����ڽ�
	}

	return 0;   // ������� ��ȯ Ÿ���� �־�� �ϹǷ�
}

void CeDeuProjectDlg::OnBnClickedMbServer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_iType = 0;
	SetDlgItemText(IDC_CULCHECK, "���� ����");    //��ư�� �������� �ٲ�
	GetDlgItem(IDC_CULCHECK)->EnableWindow(TRUE); //���ǽ��۹�ư Ȱ��ȭ
	GetDlgItem(IDC_CULCHECK)->ShowWindow(SW_SHOW); //���ǽ��۹�ư Ȱ��ȭ
	GetDlgItem(IDC_MB_SERVER)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_MB_CLIENT)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_EDIT_SERVER)->ShowWindow(SW_HIDE); //
	GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_HIDE); //

	
	m_ConCheck = 0;
}


void CeDeuProjectDlg::OnBnClickedMbClient()
{
	UpdateData(TRUE);
	m_iType = 1;  //Ŭ���̾�Ʈ ���ý�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
