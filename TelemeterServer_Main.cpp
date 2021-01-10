//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TelemeterServer_Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "cspin"
#pragma resource "*.dfm"
TForm_Telemetry_Server *Form_Telemetry_Server;
//---------------------------------------------------------------------------
__fastcall TForm_Telemetry_Server::TForm_Telemetry_Server(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::BitBtn_CloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------



void __fastcall TForm_Telemetry_Server::Button_StartClick(TObject *Sender)
{
 write_log("Program is starting up");
 Timer_Main->Interval=CSpinEdit_Interval->Value;
 Timer_Main->Enabled = true;
 write_log("Timer has started. Interval = " + IntToStr(__int64(Timer_Main->Interval)));

}
//---------------------------------------------------------------------------

// A function that determines the number of interesctions and the coordinates for those points
void __fastcall TForm_Telemetry_Server::write_log(String log_info)
{
Memo_Log->Lines->Add(DateTimeToStr(Now())+" "+log_info);
}

//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::Timer_MainTimer(TObject *Sender)
{
 int i;
 String xs;
 int err;
 String MsgF1=" ";
 String MsgF2=": ";
 String MsgF3=", ";
 String MsgF4="{\n";
 String MsgF5="\n\r}\n\r";

 write_log("timer has reset");

 xs="";
 MsgF1[1]='"';

 for ((i = LAT); i < THE_CURRENT_MAX_; i++)
 {
  if (!TelemetryData[i].xEnable)
	continue;
 if (TelemetryData[i].increment)
 {
  TelemetryData[i].pv = TelemetryData[i].pv + 1;
 }
 else if (TelemetryData[i].random)
 {
  Randomize();
  TelemetryData[i].pv = TelemetryData[i].random_range_min +
  ((float)(random(TelemetryData[i].random_range_max -
  TelemetryData[i].random_range_min)*1.0)/TelemetryData[i].random_denominator);
 }
 //====prepare message, message is a string
 if (TEdit_TelemetryData[i] != Null)
  {
   TEdit_TelemetryData[i]->Text =
   FloatToStrF(TelemetryData[i].pv, ffFixed, 15, TelemetryData[i].digit);
  }


 xs=xs+MsgF1+TelemetryData[i].xName
	  +MsgF1+MsgF2+MsgF1
	  +FloatToStrF(TelemetryData[i].pv, ffFixed, 15, TelemetryData[i].digit)
	  +MsgF1+MsgF3;
 } // end of loop

 xs= xs.Delete(xs.Length()-1, 2);
 xs= MsgF4 + xs + MsgF5;

 write_log("The message is " +xs);

 Send_A_Message(xs);

}//end of timer

//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::Button_StopClick(TObject *Sender)
{
 write_log("Program has stopped");
 Timer_Main->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Latitude_IncrementClick(TObject *Sender)
{
  TCheckBox *TheCurrentCheckBox=(TCheckBox *)(Sender);
  int xTag;
  double xData;

  if(TheCurrentCheckBox==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentCheckBox->Tag;
	}
  if((xTag<0)||(xTag>=THE_CURRENT_MAX_))
	{
		write_log("Invalid Data Tag");
		return;
	}
  TelemetryData[xTag].increment=TheCurrentCheckBox->Checked;
  write_log(TelemetryData[xTag].xName+" increment setting is updated to "+
			IntToStr(__int64(TelemetryData[xTag].increment)));

  if((TheCurrentCheckBox->Checked)&&(CheckBox_Random[xTag]!=NULL))
	{
		if(CheckBox_Random[xTag]->Checked)
		{
		  CheckBox_Random[xTag]->Checked = false;
		  TelemetryData[xTag].random=false;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Latitude_RandomClick(TObject *Sender)
{
  TCheckBox *TheCurrentCheckBox=(TCheckBox *)(Sender);
  int xTag;
  double xData;

  if(TheCurrentCheckBox==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentCheckBox->Tag;
	}
  if((xTag<0)||(xTag>=THE_CURRENT_MAX_))
	{
		write_log("Invalid Data Tag");
		return;
	}
  TelemetryData[xTag].random=TheCurrentCheckBox->Checked;
  write_log(TelemetryData[xTag].xName+" random setting is updated to "+
			IntToStr(__int64(TelemetryData[xTag].random)));

  if((TheCurrentCheckBox->Checked)&&(CheckBox_Increment[xTag]!=NULL))
	{
		if(CheckBox_Increment[xTag]->Checked)
		{
		  CheckBox_Increment[xTag]->Checked = false;
		  TelemetryData[xTag].increment=false;
		}
	}
}
//---------------------------------------------------------------------------



void __fastcall TForm_Telemetry_Server::Edit_LatitudeKeyUp(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
  TEdit *TheCurrentEdit=(TEdit *)(Sender);
  int xTag;
  double xData;

  if(TheCurrentEdit==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentEdit->Tag;
	}
  if((xTag<0)||(xTag>=THE_CURRENT_MAX_))
	{
		write_log("Invalid Data Tag");
		return;
	}
  if(Key==ENTER_KEY)
	{
	  try
	  {
		  xData=TheCurrentEdit->Text.ToDouble();
		  /*
		  may need to check data is valid/reasonable or not
		  */
		  TelemetryData[xTag].pv =xData;
		  write_log(TelemetryData[xTag].xName+" data is updated to "+TheCurrentEdit->Text);
	  }
	  catch(...)
	  {
	   MessageBox(Handle, "invalid data input", "Alert", MB_OK | MB_ICONWARNING);
	  }
	}

}
//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::FormCreate(TObject *Sender)
{

//*initialize all names and checkboxes
   TelemetryData[LAT].random_denominator= 100.0;
   TelemetryData[LAT].xName="latitude";
   TelemetryData[LAT].pv=0;
   TelemetryData[LAT].random_range_min=0;
   TEdit_MinRange[LAT]= Edit_MinLatitude;
   TEdit_MinRange[LAT]->Tag =LAT;
   TelemetryData[LAT].random_range_max=100;
   TEdit_MaxRange[LAT]= Edit_MaxLatitude;
   TEdit_MaxRange[LAT]->Tag = LAT;
   TelemetryData[LAT].digit =6;
   CheckBox_Enable[LAT]=CheckBox_Latitude_Enable;
   CheckBox_Enable[LAT]->Tag=LAT;
   CheckBox_Increment[LAT]=CheckBox_Latitude_Increment;
   CheckBox_Increment[LAT]->Tag =LAT;
   CheckBox_Random[LAT]=CheckBox_Latitude_Random;
   CheckBox_Random[LAT]->Tag =LAT;
   TEdit_TelemetryData[LAT]=Edit_Latitude;
   TEdit_TelemetryData[LAT]->Tag =LAT;


   TelemetryData[LONG].random_denominator= 100.0;
   TelemetryData[LONG].xName="longitude";
   TelemetryData[LONG].random_range_min=0;
   TEdit_MinRange[LONG]= Edit_MinLongitude;
   TEdit_MinRange[LONG]->Tag=LONG;
   TelemetryData[LONG].random_range_max=100;
   TEdit_MaxRange[LONG]= Edit_MaxLongitude;
   TEdit_MaxRange[LONG]->Tag= LONG;
   TelemetryData[LONG].digit =6;
   TelemetryData[LONG].pv=0;
   CheckBox_Enable[LONG]=CheckBox_Longitude_Enable;
   CheckBox_Enable[LONG]->Tag=LONG;
   CheckBox_Increment[LONG]=CheckBox_Longitude_Increment;
   CheckBox_Increment[LONG]->Tag =LONG;
   CheckBox_Random[LONG]=CheckBox_Longitude_Random;
   CheckBox_Random[LONG]->Tag =LONG;
   TEdit_TelemetryData[LONG]=Edit_Longitude;
   TEdit_TelemetryData[LONG]->Tag =LONG;

   TelemetryData[ALT_AGL].random_denominator= 10.0;
   TelemetryData[ALT_AGL].xName="altitude_agl_meters";
   TelemetryData[ALT_AGL].pv=0;
   TelemetryData[ALT_AGL].random_range_min=0;
   TEdit_MinRange[ALT_AGL]= Edit_MinAltitudeAglMeters;
   TEdit_MinRange[ALT_AGL]->Tag= ALT_AGL;
   TelemetryData[ALT_AGL].random_range_max=100;
   TEdit_MaxRange[ALT_AGL]= Edit_MaxAltitudeAglMeters;
   TEdit_MaxRange[ALT_AGL]->Tag= ALT_AGL;
   TelemetryData[ALT_AGL].digit =1;
   CheckBox_Enable[ALT_AGL]=CheckBox_AltitudeAglMeters_Enable;
   CheckBox_Enable[ALT_AGL]->Tag=ALT_AGL;
   CheckBox_Increment[ALT_AGL]=CheckBox_AltitudeAglMeters_Increment;
   CheckBox_Increment[ALT_AGL]->Tag =ALT_AGL;
   CheckBox_Random[ALT_AGL]=CheckBox_AltitudeAglMeters_Radom;
   CheckBox_Random[ALT_AGL]->Tag =ALT_AGL;
   TEdit_TelemetryData[ALT_AGL]=Edit_AltitudeAglMeters;
   TEdit_TelemetryData[ALT_AGL]->Tag =ALT_AGL;

   TelemetryData[ALT_MSL].random_denominator= 10.0;
   TelemetryData[ALT_MSL].xName="altitude_msl_meters";
   TelemetryData[ALT_MSL].pv=0;
   TelemetryData[ALT_MSL].random_range_min=0;
   TEdit_MinRange[ALT_MSL]= Edit_MinAltitudeMslMeters;
   TEdit_MinRange[ALT_MSL]->Tag= ALT_MSL;
   TelemetryData[ALT_MSL].random_range_max=100;
   TEdit_MaxRange[ALT_MSL]= Edit_MaxAltitudeMslMeters;
   TEdit_MaxRange[ALT_MSL]->Tag= ALT_MSL;
   TelemetryData[ALT_MSL].digit =1;
   CheckBox_Enable[ALT_MSL]=CheckBox_AltitudeMslMeters_Enable;
   CheckBox_Enable[ALT_MSL]->Tag=ALT_MSL;
   CheckBox_Increment[ALT_MSL]=CheckBox_AltitudeMslMeters_Increment;
   CheckBox_Increment[ALT_MSL]->Tag =ALT_MSL;
   CheckBox_Random[ALT_MSL]=CheckBox_AltitudeMslMeters_Random;
   CheckBox_Random[ALT_MSL]->Tag =ALT_MSL;
   TEdit_TelemetryData[ALT_MSL]=Edit_AltitudeMslMeters;
   TEdit_TelemetryData[ALT_AGL]->Tag =ALT_AGL;

	TelemetryData[HEADING].random_denominator= 10.0;
   TelemetryData[HEADING].xName="heading_degrees";
   TelemetryData[HEADING].pv=0;
   TelemetryData[HEADING].random_range_min=0;
   TEdit_MinRange[HEADING]= Edit_MinHeadingDegrees;
   TEdit_MinRange[HEADING]->Tag = HEADING;
   TelemetryData[HEADING].random_range_max=100;
   TEdit_MaxRange[HEADING]= Edit_MaxHeadingDegrees;
   TEdit_MaxRange[HEADING]->Tag= HEADING;
   TelemetryData[HEADING].digit =1;
   CheckBox_Enable[HEADING]=CheckBox_HeadingDegrees_Enable;
   CheckBox_Enable[HEADING]->Tag=HEADING;
   CheckBox_Increment[HEADING]=CheckBox_HeadingDegrees_Increment;
   CheckBox_Increment[HEADING]->Tag =HEADING;
   CheckBox_Random[HEADING]=CheckBox_HeadingDegrees_Random;
   CheckBox_Random[HEADING]->Tag =HEADING;
   TEdit_TelemetryData[HEADING]=Edit_HeadingDegrees;
   TEdit_TelemetryData[HEADING]->Tag =HEADING;

	TelemetryData[TIME_TELEM].random_denominator= 1.0;
   TelemetryData[TIME_TELEM].xName="timestamp_telem";
   TelemetryData[TIME_TELEM].pv=0;
   TelemetryData[TIME_TELEM].random_range_min=0;
   TEdit_MinRange[TIME_TELEM]= Edit_MinTimestampTelem;
   TEdit_MinRange[TIME_TELEM]->Tag = TIME_TELEM;
   TelemetryData[TIME_TELEM].random_range_max=1000;
   TEdit_MaxRange[TIME_TELEM]= Edit_MaxTimestampTelem;
   TEdit_MaxRange[TIME_TELEM]->Tag = TIME_TELEM;
   TelemetryData[TIME_TELEM].digit =0;
   CheckBox_Enable[TIME_TELEM]=CheckBox_TimeStampTelem_Enable;
   CheckBox_Enable[TIME_TELEM]->Tag=TIME_TELEM;
   CheckBox_Increment[TIME_TELEM]=CheckBox_TimeStampTelem_Increment;
   CheckBox_Increment[TIME_TELEM]->Tag =TIME_TELEM;
   CheckBox_Random[TIME_TELEM]=CheckBox_TimeStampTelem_Random;
   CheckBox_Random[TIME_TELEM]->Tag =TIME_TELEM;
   TEdit_TelemetryData[TIME_TELEM]=Edit_TimestampTelem;
   TEdit_TelemetryData[TIME_TELEM]->Tag =TIME_TELEM;

    TelemetryData[TIME_MSG].random_denominator= 1.0;
   TelemetryData[TIME_MSG].xName="timestamp_msg";
   TelemetryData[TIME_MSG].pv=0;
   TelemetryData[TIME_MSG].random_range_min=0;
   TEdit_MinRange[TIME_MSG]= Edit_MinTimestampMsg;
   TEdit_MinRange[TIME_MSG]->Tag =TIME_MSG;
   TelemetryData[TIME_MSG].random_range_max=1000;
   TEdit_MaxRange[TIME_MSG]= Edit_MaxTimestampMsg;
   TEdit_MaxRange[TIME_MSG]->Tag = TIME_MSG;
   TelemetryData[TIME_MSG].digit =0;
   CheckBox_Enable[TIME_MSG]=CheckBox_TimeStampMsg_Enable;
   CheckBox_Enable[TIME_MSG]->Tag=TIME_MSG;
   CheckBox_Increment[TIME_MSG]=CheckBox_TimeStampMsg_Increment;
   CheckBox_Increment[TIME_MSG]->Tag =TIME_MSG;
   CheckBox_Random[TIME_MSG]=CheckBox_TimeStampMsg_Radom;
   CheckBox_Random[TIME_MSG]->Tag =TIME_MSG;
   TEdit_TelemetryData[TIME_MSG]=Edit_TimeStampMsg;
   TEdit_TelemetryData[TIME_MSG]->Tag =TIME_MSG;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Latitude_EnableClick(TObject *Sender)

{
  TCheckBox *TheCurrentCheckBox=(TCheckBox *)(Sender);
  int xTag;
  double xData;

  if(TheCurrentCheckBox==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentCheckBox->Tag;
	}
  if((xTag<0)||(xTag>=THE_CURRENT_MAX_))
	{
		write_log("Invalid Data Tag");
		return;
	}
  TelemetryData[xTag].xEnable =TheCurrentCheckBox->Checked;
  write_log(TelemetryData[xTag].xName+" Enabled is updated to "+
			IntToStr(__int64(TelemetryData[xTag].xEnable)));
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Enable_AllClick(TObject *Sender)

{
int i;
for (i=LAT; i < THE_CURRENT_MAX_; i++)
{
 CheckBox_Enable[i]->Checked = CheckBox_Enable_All->Checked;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Increment_AllClick(TObject *Sender)

{
int i;
for (i=LAT; i < THE_CURRENT_MAX_; i++)
{
if(CheckBox_Increment[i] !=NULL)
 CheckBox_Increment[i]->Checked = CheckBox_Increment_All->Checked;
}
if (CheckBox_Increment_All->Checked)
{
  CheckBox_Random_All->Checked=false;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::CheckBox_Random_AllClick(TObject *Sender)

{
int i;
for (i=LAT; i < THE_CURRENT_MAX_; i++)
{
 if (CheckBox_Random[i] != NULL)
   CheckBox_Random[i]->Checked = CheckBox_Random_All->Checked;
 }

 if (CheckBox_Random_All->Checked)
 {
   CheckBox_Increment_All->Checked=false;
 }
}
//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::Edit_Min_KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
 TEdit *TheCurrentMin=(TEdit *)(Sender);
  int xTag;
  double xData;

  if(TheCurrentMin==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentMin->Tag;
	}

  if(Key==ENTER_KEY)
	{
	   try
	   {
	   if (TheCurrentMin->Text.ToDouble() < TEdit_MaxRange[xTag]->Text.ToDouble())
		 {
		  xData=TheCurrentMin->Text.ToDouble();
		  /*
		  may need to check data is valid/reasonable or not
		  */
		  TelemetryData[xTag].random_range_min =xData;
		  write_log(TelemetryData[xTag].xName+" minimum value is updated to "+TheCurrentMin->Text);
		 }
	   else
		 {
		  MessageBox(Handle, "invalid range", "Alert", MB_OK | MB_ICONWARNING);
		 }
	   }
	   catch(...)
	   {
	   MessageBox(Handle, "invalid data input", "Alert", MB_OK | MB_ICONWARNING);
	   }
	 }
}
//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::Edit_Max_KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
 TEdit *TheCurrentMax=(TEdit *)(Sender);
  int xTag;
  double xData;
  String xs;

  if(TheCurrentMax==NULL)
	{
		return;
	}
	else
	{
	  xTag=TheCurrentMax->Tag;
	}

  if(Key==ENTER_KEY)
	{

	   try
	   {
	   if (TheCurrentMax->Text.ToDouble() > TEdit_MinRange[xTag]->Text.ToDouble())
		 {
		  xData=TheCurrentMax->Text.ToDouble();
		  /*
		  may need to check data is valid/reasonable or not
		  */
		  TelemetryData[xTag].random_range_max =xData;
		  write_log(TelemetryData[xTag].xName+" maximum value is updated to "+TheCurrentMax->Text);
		 }
	   else
		 {
		  xs="invalid range:"+TheCurrentMax->Text+" > "+TEdit_MinRange[xTag]->Text+" ?";
		  write_log(xs);
		  MessageBox(Handle, "invalid range", "Alert", MB_OK | MB_ICONWARNING);
		 }
	   }
	   catch(...)
	   {
	   MessageBox(Handle, "invalid data input", "Alert", MB_OK | MB_ICONWARNING);
	   }



	  }

}

void __fastcall TForm_Telemetry_Server::ServerSocket1Accept(TObject *Sender, TCustomWinSocket *Socket)

{
in_addr localaddr;
  char s[255],s1[255];
  String xs="";

 localaddr=ServerSocket1->Socket->LookupName(ServerSocket1->Socket->LocalHost);
 itoa(localaddr.S_un.S_un_b.s_b1,s,10);
 strcat(s,".");
 itoa(localaddr.S_un.S_un_b.s_b2,s1,10);
 strcat(s,s1);
 strcat(s,".");
 itoa(localaddr.S_un.S_un_b.s_b3,s1,10);
 strcat(s,s1);
 strcat(s,".");
 itoa(localaddr.S_un.S_un_b.s_b4,s1,10);
 strcat(s,s1);
 RichEdit1->Lines->Add("Server("+StrPas(s)+") is onAccept: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ServerSocket1ClientConnect(TObject *Sender,
          TCustomWinSocket *Socket)
{

RichEdit1->Lines->Add("Server is onClientConnect: "+Socket->RemoteAddress);

}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ServerSocket1ClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket)
{
RichEdit1->Lines->Add("Server is onClientDisconnect: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ServerSocket1ClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)

{
String es;

   if (ErrorEvent==eeGeneral)
	es="The socket received an error message that does not fit into any of the following categories.";
   else
   if (ErrorEvent==eeSend)
    es="An error occurred when trying to write to the socket connection.";
   else
   if (ErrorEvent==eeReceive)
	es="An error occurred when trying to read from the socket connection.";
   else
   if (ErrorEvent==eeConnect)
    es="A connection request that was already accepted could not be completed.";
   else
   if (ErrorEvent==eeDisconnect)
	es="An error occurred when trying to close a connection.";
   else
   if (ErrorEvent==eeAccept)
	es="A problem occurred when trying to accept a client connection request.";

   RichEdit1->Lines->Add("Server(TCP/IP: "+
				Socket->LocalAddress+"/"+IntToStr(Socket->LocalPort)+
				" has encountered an error to "+
                Socket->RemoteAddress+"/"+IntToStr(Socket->RemotePort)
                );
   RichEdit1->Lines->Add(es+" EC="+IntToStr(ErrorCode));
   ErrorCode=0;
   ServerSocket1ClientDisconnect(Sender, Socket);
   Socket->Disconnect(Socket->SocketHandle);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket)

{
RichEdit1->Lines->Add("Server is onClientRead: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------


void __fastcall TForm_Telemetry_Server::ServerSocket1Listen(TObject *Sender, TCustomWinSocket *Socket)

{
RichEdit1->Lines->Add("Server is onListen: "+Socket->LocalAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)

{
RichEdit2->Lines->Add("Local Client is Connected: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Connecting(TObject *Sender, TCustomWinSocket *Socket)

{
RichEdit2->Lines->Add("Local Client is Connecting: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket)

{
RichEdit2->Lines->Add("Local Client is Disconnected: "+Socket->RemoteAddress);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket)

{
  int rd_length,rd_num,i;
  Byte rd_buffer[MAX_SOCKET_DATA_BUFFER];  //2048
  String xs="";

  try{
	  rd_num=Socket->ReceiveLength();
	  do{
		  rd_length=(rd_num>=MAX_SOCKET_DATA_BUFFER)?(MAX_SOCKET_DATA_BUFFER):(rd_num);
		  Socket->ReceiveBuf(rd_buffer,rd_length);
		  for(i=0;i<rd_length;++i)
			{
			  xs=xs+char(rd_buffer[i]);
			}
		  rd_num -= rd_length;
		}while(rd_num>0);
	  if(xs!="")
		 //RichEdit2->Lines->Add("C1 received:"+xs);
		 RichEdit2->Lines->Insert(0,"Local Client received:"+xs);
	 }
	 catch(...)
	 {
	   RichEdit2->Lines->Add("Local Client is reading failed");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
RichEdit2->Lines->Add("Local Client is onClientError="+IntToStr(ErrorCode)+" : "+Socket->RemoteAddress);
ErrorCode=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::ClientSocket1Write(TObject *Sender, TCustomWinSocket *Socket)

{
;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


int  __fastcall TForm_Telemetry_Server::Send_A_Message(AnsiString Msg)
{
int n;

char Buf[MAX_TELEMETRY_DATA+1];
	Label_display->Caption=Msg;
	try
	{
	  if((ServerSocket1->Active)&&(ServerSocket1->Socket->Connected))
        {
		  n=ServerSocket1->Socket->ActiveConnections;

		  Label_socket_connection->Caption="Connection count = "+IntToStr(n);

		  StrPLCopy(Buf, Msg, MAX_TELEMETRY_DATA);
		  for(int i=0;i<n;i++)
		   {
			  ServerSocket1->Socket->Connections[i]->SendBuf(Buf,(Msg.Length()>=MAX_TELEMETRY_DATA)? (MAX_TELEMETRY_DATA): (Msg.Length()));
		   }
        }
	}
	catch(...)
	{
	write_log("error found when server is sending data.");
	}

}

//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::Button_StartServerClick(TObject *Sender)
{
ServerSocket1->Port=CSpinEdit_TCP_Port->Value;
 ServerSocket1->Active=true;

 CSpinEdit_TCP_Port->Enabled = false;
}


//---------------------------------------------------------------------------

void __fastcall TForm_Telemetry_Server::Button_StopServerClick(TObject *Sender)
{
 ClientSocket1->Active=false;
 ServerSocket1->Active=false;

 CSpinEdit_TCP_Port->Enabled = true;
}

//---------------------------------------------------------------------------



void __fastcall TForm_Telemetry_Server::rClick(TObject *Sender)

 {

 try {
		ClientSocket1->Host="127.0.0.1";
		ClientSocket1->Port=CSpinEdit_TCP_Port->Value;
		ClientSocket1->Active=true;
	}
 catch (...)
	{
		String wrnmsg = L"Local Client Failed to connect to Server";
        MessageDlg(wrnmsg, mtError, TMsgDlgButtons() << mbOK, 0);
	}

}








