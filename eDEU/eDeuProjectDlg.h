
// eDeuProjectDlg.h : ��� ����
//

#pragma once
#include "opencv2\opencv.hpp"
#include "afxwin.h"
#include "CvvImage.h"
#include "ServerSock.h"
#include "ClientSockServer.h"
#include "ClientSockClient.h"

struct ThreadArg           //�������� �������� �޾ƿ��� ���� ����ü
{
	int type;              //�������� Ŭ�󸮾�Ʈ���� ����
	CString str;           //ä�ù��ڿ�
};

using namespace cv;
// CeDeuProjectDlg ��ȭ ����
class CeDeuProjectDlg : public CDialogEx
{
// �����Դϴ�.
public:
	static UINT OnLogtext(LPVOID log);
	ThreadArg log1;
	void SendInfo(int con);
	int m_ConCheck;
	void OnReceiveC();
	void Send(char *buf);
	void OnReceiveS(CClientSockServer* pSock);
	void OnAccept();

	CeDeuProjectDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CeDeuProjectDlg();	//�Ҹ���

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDEUPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	CServerSock* m_pServerSock;
	CClientSockClient* m_pClientSock;
	CTypedPtrList<CObList, CClientSockServer*> m_ClientSock;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	/// ���� �ۼ� �Լ� ///
	static void DisplayImage(Mat *image, CDC *pDC, CRect rect);
	static void DrawCvMat(CDC* pDC, cv::Mat& origin, CRect rect);
	int TemplateMatching(Mat oriImg, Mat tempImg, int label);
	void setup();
	void MatchingMethod(int, void*);
	bool cursorstop = FALSE;
	char* image_window = "Source Image";
	char* result_window = "Result window";
	Mat result;

	/// ��ķ & �̹��� ���� ���� �����///
	VideoCapture cam; // 0��° ķ ĸ�Ľ���, capture�� ķ ��ȯ�� ������
	CImage cimage_mfc; // ĸ��ȭ���� ����� �̹���
//	int invcount = 0;

	/// HAAR CASCADE �� xml����  ���� ///
	const String haarcascade_face = "haarcascade_frontalface_default.xml";
	const String haarcascade_eye = "haarcascade_eye.xml";
	CascadeClassifier cascade_face;
	CascadeClassifier cascade_eye;
	CvMemStorage* storage1 = cvCreateMemStorage(0);
	CvMemStorage* storage2 = cvCreateMemStorage(0);
	
	/// ����ó�� ����� /// 
	cv::Mat frame; // ��ķ���� ĸ��ȭ���� �޾ƿ� mat�ν��Ͻ�( Ÿ�̸ӷ� ���⿡ ��� ����)
	IplImage* m_pImage = NULL; // MATimage ->lplimage ->cvvimage ���Ͽ� ���� m_pimage 
	IplImage* roi_image=NULL;
	CvvImage m_cImage;
	std::vector<Rect> faces; // ���νĵ� �簢��
	std::vector<Rect> eyes; // ���νĵ� ��
	Mat faceROI;
	Mat image_roi; // roi ���� �����Ǿ� �߶����ִ� �̹����� �� mat�� �̹���
	int roi_i;
	IplImage copy;
	Rect roi_area;
	Mat captureimage;
	int count = 0;

	/// ��Ī ����� ///
	Mat templ;
	Mat img;
	int match_method;
	int max_Trackbar = 5;
	bool ismatch = false;
	/// DB �̹��� ����� ///
	IplImage* captureimg;
	CEdit m_Name3;
	int db_count = 0;
	
	std::string dbfilename;
	CString DBstr[10];
	int dbstridx = 0;
	CString username;
	//// ĸ�Ŀ� ///
	std::string filename;

	/// MFC ���̾�α� ��Ʈ�� ���� ����� ///
	CStatic m_cam;
	CStatic m_dscreen2;
	CStatic m_DectFaceScreen;
//	CButton m_facetoface;
//	bool m_face;
//	bool m_eye;

	/// �޽��� �Լ� ����� ///	
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
