
// eDeuProjectDlg.h : 헤더 파일
//

#pragma once
#include "opencv2\opencv.hpp"
#include "afxwin.h"
#include "CvvImage.h"
#include "ServerSock.h"
#include "ClientSockServer.h"
#include "ClientSockClient.h"

struct ThreadArg           //스레드의 변수값을 받아오기 위한 구조체
{
	int type;              //서버인지 클라리언트인지 구분
	CString str;           //채팅문자열
};

using namespace cv;
// CeDeuProjectDlg 대화 상자
class CeDeuProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	static UINT OnLogtext(LPVOID log);
	ThreadArg log1;
	void SendInfo(int con);
	int m_ConCheck;
	void OnReceiveC();
	void Send(char *buf);
	void OnReceiveS(CClientSockServer* pSock);
	void OnAccept();

	CeDeuProjectDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CeDeuProjectDlg();	//소멸자

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDEUPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	CServerSock* m_pServerSock;
	CClientSockClient* m_pClientSock;
	CTypedPtrList<CObList, CClientSockServer*> m_ClientSock;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	/// 임의 작성 함수 ///
	static void DisplayImage(Mat *image, CDC *pDC, CRect rect);
	static void DrawCvMat(CDC* pDC, cv::Mat& origin, CRect rect);
	int TemplateMatching(Mat oriImg, Mat tempImg, int label);
	void setup();
	void MatchingMethod(int, void*);
	bool cursorstop = FALSE;
	char* image_window = "Source Image";
	char* result_window = "Result window";
	Mat result;

	/// 웹캠 & 이미지 제어 변수 선언부///
	VideoCapture cam; // 0번째 캠 캡쳐시작, capture는 캠 반환값 포인터
	CImage cimage_mfc; // 캡쳐화면이 저장될 이미지
//	int invcount = 0;

	/// HAAR CASCADE 및 xml파일  선언 ///
	const String haarcascade_face = "haarcascade_frontalface_default.xml";
	const String haarcascade_eye = "haarcascade_eye.xml";
	CascadeClassifier cascade_face;
	CascadeClassifier cascade_eye;
	CvMemStorage* storage1 = cvCreateMemStorage(0);
	CvMemStorage* storage2 = cvCreateMemStorage(0);
	
	/// 영상처리 선언부 /// 
	cv::Mat frame; // 웹캠에서 캡쳐화면을 받아올 mat인스턴스( 타이머로 여기에 계속 받음)
	IplImage* m_pImage = NULL; // MATimage ->lplimage ->cvvimage 위하여 만든 m_pimage 
	IplImage* roi_image=NULL;
	CvvImage m_cImage;
	std::vector<Rect> faces; // 얼굴인식된 사각형
	std::vector<Rect> eyes; // 얼굴인식된 눈
	Mat faceROI;
	Mat image_roi; // roi 영역 설정되어 잘라져있는 이미지가 들어갈 mat형 이미지
	int roi_i;
	IplImage copy;
	Rect roi_area;
	Mat captureimage;
	int count = 0;

	/// 매칭 선언부 ///
	Mat templ;
	Mat img;
	int match_method;
	int max_Trackbar = 5;
	bool ismatch = false;
	/// DB 이미지 저장용 ///
	IplImage* captureimg;
	CEdit m_Name3;
	int db_count = 0;
	
	std::string dbfilename;
	CString DBstr[10];
	int dbstridx = 0;
	CString username;
	//// 캡쳐용 ///
	std::string filename;

	/// MFC 다이얼로그 컨트롤 변수 선언부 ///
	CStatic m_cam;
	CStatic m_dscreen2;
	CStatic m_DectFaceScreen;
//	CButton m_facetoface;
//	bool m_face;
//	bool m_eye;

	/// 메시지 함수 선언부 ///	
	afx_msg void OnBnClickedCheckFace();
	afx_msg void OnBnClickedButton2();

	void CeDeuProjectDlg::OnBnClickedButtonDb();
	CListBox m_Listbox;
//	CButton m_culcheck;
	afx_msg void OnBnClickedCulcheck();
//	bool m_culcheck;
	CListBox m_Listbox3;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtCapture();
//	bool m_capture;
//	CButton m_Roi;
	int sorry = 0;
//	CButton m_Screenonoff;
	afx_msg void OnBnClickedCheck5();
//	CButton m_eyecheck;
	CButton m_facecheck;
	CButton m_roicheck;
//	CButton m_screenonoff;
	CButton m_screen2onoff;
	afx_msg void OnBnClickedonscreen2();
	afx_msg void OnBnClickedFacecheck();
	afx_msg void OnBnClickedEyecheck();
	afx_msg void OnBnClickedLoicheck();
	afx_msg void OnBnClickedButtExit();
	afx_msg void OnBnClickedButtInput();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_name;
	afx_msg void OnBnClickedButtDb();
	CEdit m_name2;
	CStatic m_Screen4;
	afx_msg void OnBnClickedBMatching();
//	CString m_strID;
	CString m_strMessage;
	int m_iPort;
	CString m_strServer;
	CEdit m_Time;
	CListBox m_lChatReceive;
	int m_iType;
	afx_msg void OnBnClickedRadioTypeS();
	afx_msg void OnBnClickedRadioTypeC();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
	CString m_strID;
	afx_msg void OnBnClickedMbServer();
	afx_msg void OnBnClickedMbClient();
	afx_msg void OnBnClickedHexit();
	
	CString count_cli[200];
	int c = 0;

	HBITMAP icon1, icon2, icon3, icon4;
	CStatic m_client1;
	CStatic m_client2;
	CStatic m_client3;
	CStatic m_client4;

};
