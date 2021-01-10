//---------------------------------------------------------------------------

#ifndef TelemeterServer_MainH
#define TelemeterServer_MainH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <dir.h>
#include <io.h>
#include "cspin.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//#include <algorithm.h>
//#include <Math.hpp>


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdTCPServer.hpp>
#include <System.Win.ScktComp.hpp>
//---------------------------------------------------------------------------
typedef struct
{
 double pv;
 String xName;
 bool xEnable;
 bool increment;
 bool random;
 double random_range_min;
 double random_range_max;
 int digit;
 double random_denominator;
}TelemetryDataType;

#define ENTER_KEY 13
#define MAX_TELEMETRY_DATA 256
#define LAT 0
#define LONG 1
#define ALT_AGL 2
#define ALT_MSL 3
#define HEADING 4
#define TIME_TELEM 5
#define TIME_MSG 6
#define THE_CURRENT_MAX_ 7
#define MAX_SOCKET_DATA_BUFFER 2048


class TForm_Telemetry_Server : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo_Log;
	TPanel *Panel1;
	TBitBtn *BitBtn_Close;
	TButton *Button_Start;
	TPageControl *PageControl1;
	TTabSheet *TabSheet_Telemetry_Data_Group1;
	TGroupBox *GroupBox2;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *Edit_AltitudeAglMeters;
	TEdit *Edit_Latitude;
	TEdit *Edit_AltitudeMslMeters;
	TEdit *Edit_Longitude;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *Edit_HeadingDegrees;
	TEdit *Edit_TimestampTelem;
	TEdit *Edit_TimeStampMsg;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TTabSheet *TabSheet_Data_Group2;
	TTabSheet *TabSheet_Setup;
	TCheckBox *CheckBox_Latitude_Enable;
	TLabel *Label11;
	TCSpinEdit *CSpinEdit_Interval;
	TTimer *Timer_Main;
	TButton *Button_Stop;
	TCheckBox *CheckBox_Latitude_Increment;
	TCheckBox *CheckBox_Latitude_Random;
	TCheckBox *CheckBox_Longitude_Enable;
	TCheckBox *CheckBox_AltitudeAglMeters_Enable;
	TCheckBox *CheckBox_AltitudeMslMeters_Enable;
	TCheckBox *CheckBox_TimeStampTelem_Enable;
	TCheckBox *CheckBox_TimeStampMsg_Enable;
	TCheckBox *CheckBox_Longitude_Increment;
	TCheckBox *CheckBox_AltitudeAglMeters_Increment;
	TCheckBox *CheckBox_AltitudeMslMeters_Increment;
	TCheckBox *CheckBox_HeadingDegrees_Increment;
	TCheckBox *CheckBox_TimeStampTelem_Increment;
	TCheckBox *CheckBox_TimeStampMsg_Increment;
	TCheckBox *CheckBox_Longitude_Random;
	TCheckBox *CheckBox_AltitudeAglMeters_Radom;
	TCheckBox *CheckBox_AltitudeMslMeters_Random;
	TCheckBox *CheckBox_HeadingDegrees_Random;
	TCheckBox *CheckBox_TimeStampTelem_Random;
	TCheckBox *CheckBox_TimeStampMsg_Radom;
	TLabel *Label12;
	TCheckBox *CheckBox_Enable_All;
	TCheckBox *CheckBox_Increment_All;
	TCheckBox *CheckBox_Random_All;
	TLabel *Label13;
	TEdit *Edit_MinLatitude;
	TEdit *Edit_MaxLatitude;
	TEdit *Edit_MinLongitude;
	TEdit *Edit_MinAltitudeAglMeters;
	TEdit *Edit_MinAltitudeMslMeters;
	TEdit *Edit_MinHeadingDegrees;
	TEdit *Edit_MinTimestampTelem;
	TEdit *Edit_MinTimestampMsg;
	TEdit *Edit_MaxLongitude;
	TEdit *Edit_MaxAltitudeAglMeters;
	TEdit *Edit_MaxAltitudeMslMeters;
	TEdit *Edit_MaxHeadingDegrees;
	TEdit *Edit_MaxTimestampTelem;
	TEdit *Edit_MaxTimestampMsg;
	TCSpinEdit *CSpinEdit_TCP_Port;
	TLabel *Label14;
	TCheckBox *CheckBox_HeadingDegrees_Enable;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label_display;
	TLabel *Label_socket_connection;
	TButton *Button_StartServer;
	TButton *Button_StopServer;
	TButton *r;
	TRichEdit *RichEdit1;
	TRichEdit *RichEdit2;
        void __fastcall BitBtn_CloseClick(TObject *Sender);
		void __fastcall Button_StartClick(TObject *Sender);
	void __fastcall Timer_MainTimer(TObject *Sender);
	void __fastcall Button_StopClick(TObject *Sender);
	void __fastcall CheckBox_Latitude_IncrementClick(TObject *Sender);
	void __fastcall CheckBox_Latitude_RandomClick(TObject *Sender);
	void __fastcall Edit_LatitudeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CheckBox_Latitude_EnableClick(TObject *Sender);
	void __fastcall CheckBox_Enable_AllClick(TObject *Sender);
	void __fastcall CheckBox_Increment_AllClick(TObject *Sender);
	void __fastcall CheckBox_Random_AllClick(TObject *Sender);
	void __fastcall Edit_Min_KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit_Max_KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ServerSocket1Listen(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Connecting(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall ClientSocket1Write(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Button_StartServerClick(TObject *Sender);
	void __fastcall Button_StopServerClick(TObject *Sender);
	void __fastcall rClick(TObject *Sender);











private:	// User declarations
public:		// User declarations

		TelemetryDataType TelemetryData[MAX_TELEMETRY_DATA];
		TCheckBox *CheckBox_Enable[MAX_TELEMETRY_DATA];
		TCheckBox *CheckBox_Increment[MAX_TELEMETRY_DATA];
		TCheckBox *CheckBox_Random[MAX_TELEMETRY_DATA];
		TEdit *TEdit_TelemetryData[MAX_TELEMETRY_DATA];
		TEdit *TEdit_MinRange[MAX_TELEMETRY_DATA];
		TEdit *TEdit_MaxRange[MAX_TELEMETRY_DATA];

		void __fastcall write_log(String log_info);
		int  __fastcall Send_A_Message(AnsiString Msg);


		__fastcall TForm_Telemetry_Server(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Telemetry_Server *Form_Telemetry_Server;
//---------------------------------------------------------------------------
#endif
