/**
******************************************************************************
* @file         ftpc.h                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        ftp¿Í»§¶ËµÄÓ¦ÓÃ×é°üº¯Êý        
*
* @company      ÉîÛÚì¿ÊÀ¿Æ¼¼ÓÐÏÞ¹«Ë¾
* @information  WIZnet W5100S¹Ù·½´úÀíÉÌ£¬È«³Ì¼¼ÊõÖ§³Ö£¬¼Û¸ñÓÅÊÆ´ó£¡
* @website      www.wisioe.com 
* @forum        www.w5100s.com
* @qqGroup      579842114                                                     
******************************************************************************
*/
#include "ftpc.h"
#include "w5100s.h"

#define ABOR    1    //ÖÐÖ¹ÃüÁî
#define ACCT    2    //ÕËºÅÃüÁî
#define ALLO    3    //·¢ËÍÎÄ¼þÇ°ÔÚ·þÎñÆ÷ÉÏ·ÖÅäX¸ö×Ö½ÚÃüÁ
#define APPE    4    //¸½¼Ó´´½¨ÃüÁî
#define CDUP    5    //½øÈë¸ùÄ¿Â¼
#define CWD     6    //¸Ä±ä¹¤×÷Ä¿Â¼
#define DELE    7    //É¾³ýÃüÁî
#define HELP    8    //°ïÖúÃüÁî
   
#define LIST    9    //ÈÃ·þÎñÆ÷¸ø¿Í»§·¢ËÍÒ»·ÝÁÐ±í
#define MKD     10   //´´½¨Ä¿Â¼
#define MODE    11   //Ö¸¶¨´«ÊäÄ£Ê½
#define NLIST   12   //Ãû³ÆÁÐ±í
#define NOOP    13   //ÎÞ²Ù×÷ÃüÁî

#define PASS    14   //Ïò·þÎñÆ÷·¢ËÍÓÃ»§µÄÃÜÂë
#define PASV    15   //±»¶¯ÃüÁî
#define PORT    16   //Îª·þÎñÆ÷Ö¸¶¨±¾µØIPµØÖ·ºÍ±¾µØ¶Ë¿Ú
#define PWD     17   //´òÓ¡µ±Ç°¹¤×÷Ä¿Â¼
#define QUIT    18   //×¢ÏúÃüÁî
#define REIN    19   //ÖØÐÂ³õÊ¼»¯ÃüÁî
#define REST    20   //ÖØÐÂÆô¶¯ÃüÁî
#define RETR    21   //ÈÃ·þÎñÆ÷·¢ËÍÒ»·Ý
#define RMD     22   //É¾³ýÄ¿Â¼
#define RNFR    23   //°Ñ..ÖØÃüÃûÓëRNTOÁ¬ÓÃ
#define RNTO    24   //ÖØÃüÃûÎª
#define SITE    25   //Õ¾µã²ÎÊýÃüÁî
#define SMNT    26   //½á¹¹×°ÅäÃüÁî
#define STAT    27   //×´Ì¬ÃüÁî
#define STOR    28   //ÎÄ¼þ´«Êäµ½·þÎñÆ÷
#define STOU    29   //´æÎªÎ¨Ò»ÎÄ¼þ
#define STRU    30   //´«´ïÊý¾ÝµÄ½á¹¹ÀàÐÍ
#define SYST    31   //ÏµÍ³ÃüÁî
#define TYPE    32   //Êý¾ÝµÄ´«Êä·½Ê½
#define USER    33   //ÓÃ»§Ãû³Æ ÓëPASSÃüÁîÒ»ÆðÓÃÀ´µÇÂ½
#define FTP_GET 0
#define FTP_PUT 1

typedef union _un_l2cval 
{
 unsigned int lVal;
 unsigned char cVal[4];
}un_l2cval;

un_l2cval fremote_ip;
uint16_t  fremote_port;
un_l2cval flocal_ip;
uint16_t  flocal_port =1267;
uint8_t   connect_state_control_ftpc = 0;
uint8_t   connect_state_data_ftpc = 0;
uint8_t   gModeActivePassiveflag = 0;
uint8_t   FTP_destip[4] = {192,168,1,100};  // For FTP client examples; destination network info
uint16_t  FTP_destport = 21;                    // For FTP client examples; destination network info
uint8_t   gMenuStart = 0;
uint8_t   gDataSockReady = 0;
uint8_t   gDataPutGetStart = 0;
uint8_t   send_ok=0;
uint8_t   count = 0;
static uint8_t gMsgBuf[20]={0,};

struct ftpc ftpc;
struct Command Command;

/*****
*±¾²âÊÔÀý³ÌFTP·þÎñÆ÷²ÉÓÃµÄÈí¼þÎªTYPSoft·þÎñÆ÷,IPµØÖ·Îª192.168.1.100¶Ë¿ÚºÅÉèÖÃÎª21
*ÓÃ»§ÃûÎªwiznetÃÜÂëÎª123£¬·þÎñÆ÷²ÉÓÃµÄÖ÷¶¯Ä£Ê½
*±¾Àý³ÌÖ»¼òµ¥ÊµÏÖÁËÎÄ¼þÏÂÔØ£¬ÎÄ¼þÉÏ´«ÆäËûÐèÇó×ÔÐÐÐÞ¸Ä
*¾ßÌå´úÂë¿É²Î¿¼https://github.com/Wiznet/ioLibrary_Driver/tree/master/Internet/FTPClient
*******************************************************/
uint8_t ftpc_run(uint8_t * dbuf)
{

  uint16_t size = 0;
  long ret = 0;
  volatile uint32_t send_byte, recv_byte;
  volatile uint32_t blocklen;
  volatile uint32_t remain_filesize;
  volatile uint32_t remain_datasize;
  volatile uint8_t msg_c;
  uint8_t dat[50]={0,};
  volatile uint32_t totalSize = 0, availableSize = 0;

/*********Êý¾Ý´«Êä*******************************/
  switch(getSn_SR(DATA_SOCK))
  {
    case SOCK_ESTABLISHED :

#if FTP_GET      
    /**********ÏÂÔØÊý¾Ý****************/
    if((size = getSn_RX_RSR(DATA_SOCK)) > 0)
    { // ¼ì²âÊÇ·ñÊÕµ½Êý¾Ý
      memset(dbuf, 0, _MAX_SS);
      if(size > _MAX_SS) 
        size = _MAX_SS - 1;
      ret = recv(DATA_SOCK,dbuf,size);
      dbuf[ret] = '\0';
      printf("Rcvd Command data: %s\r\n", dbuf);//´òÓ¡ÊÕµ½µÄÊý¾Ý
      proc_ftpc((char *)dbuf);
    }
#endif      
#if FTP_PUT
    /********ÉÏ´«Êý¾Ý********/
    send(DATA_SOCK,"hello,wiznet\r\n",14);//ÉÏ´«Êý¾Ý
    send_ok =1;
    close(DATA_SOCK);//¹Ø±ÕÊý¾Ý´«Êäsocket
#endif   
      break;
    
    case SOCK_CLOSE_WAIT :
      if((size = getSn_RX_RSR(DATA_SOCK)) > 0)
      {
        memset(dbuf, 0, _MAX_SS);
        if(size > _MAX_SS) 
          size = _MAX_SS - 1;
        ret = recv(DATA_SOCK,dbuf,size);
        dbuf[ret] = '\0';
          
        printf("Rcvd Command data: %s\r\n", dbuf);
        proc_ftpc((char *)dbuf);
      }
      disconnect(DATA_SOCK);
        break;
      
    case SOCK_CLOSED :
      if((size = getSn_RX_RSR(DATA_SOCK)) > 0)
      { 
        memset(dbuf, 0, _MAX_SS);
        if(size > _MAX_SS)  
          size = _MAX_SS - 1;
        ret = recv(DATA_SOCK,dbuf,size);
        dbuf[ret] = '\0';
          
        printf("Rcvd Command data: %s\r\n", dbuf);
        proc_ftpc((char *)dbuf);
      }
      socket(DATA_SOCK, Sn_MR_TCP,flocal_port+1, 0x00);
        break;
      
    case SOCK_INIT :
      listen(DATA_SOCK);
      break;
    default :
      break;
    }  
    /*****ÃüÁî´«Êä*********/
    switch(getSn_SR(CTRL_SOCK))
    {
      case SOCK_ESTABLISHED :
        if(!connect_state_control_ftpc)
        {
          printf("%d:FTP Connected\r\n", CTRL_SOCK);
          strcpy(ftpc.workingdir, "/");
          connect_state_control_ftpc = 1;
        }
        if(gMenuStart==1&&gModeActivePassiveflag==0 )
        {//ÉèÖÃ·þÎñÆ÷ÎªÖ÷¶¯Ä£Ê½
          sprintf((char*)dat,"PORT %d,%d,%d,%d,%d,%d\r\n", 192, 168, 1, 150, (uint8_t)((flocal_port+1)>>8), (uint8_t)((flocal_port+1)&0x00ff));
          send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
          Command.First = f_dir;
          gMenuStart=0;
          gModeActivePassiveflag = 1;
        }
        if(gModeActivePassiveflag == 1&&count==0)
        {
          count=1;
#if FTP_GET 
          sprintf(dat,"RETR ./wiznet.txt\r\n");//·¢ËÍÏÂÔØwiznet.txtÎÄ¼þµÄÃüÁî£¬ftpserverÖÐÐèÒªÓÐ¸ÃÎÄ¼þ
          send(CTRL_SOCK, (uint8_t *)dat, strlen(dat));
#endif
#if FTP_PUT            
          sprintf((char*)dat,"STOR ./wiznet2.txt\r\n");//ÉÏ´«wiznet.txtÎÄ¼þµÄÃüÁî£¬ftpserverÖÐÐèÒªÓÐ¸ÃÎÄ¼þ
          send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
#endif          
        }  
#if FTP_PUT  
        if(send_ok==1)
        {
          sprintf((char*)dat,"ABOR\r\n");//Í£Ö¹ÉÏ´«
          send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
          send_ok=0;
        }
#endif                    
        if((size = getSn_RX_RSR(CTRL_SOCK)) > 0)
        {// Don't need to check SOCKERR_BUSY because it doesn't not occur.
          memset(dbuf, 0, _MAX_SS);
          if(size > _MAX_SS) 
            size = _MAX_SS - 1;
          ret = recv(CTRL_SOCK,dbuf,size);
          dbuf[ret] = '\0';
          if(ret != size)
          {
            if(ret==0) 
              return 0;
            if(ret < 0)
            {
              printf("%d:recv() error:%ld\r\n",CTRL_SOCK,ret);
              close(CTRL_SOCK);
              return ret;
            }
          }
          printf("Rcvd Command: %s\r\n", dbuf);
          proc_ftpc((char *)dbuf);
        }
        break;
        
      case SOCK_CLOSE_WAIT :
        printf("%d:CloseWait\r\n",CTRL_SOCK);
        disconnect(CTRL_SOCK);
        printf("%d:Closed\r\n",CTRL_SOCK);
        break;
      
      case SOCK_CLOSED :
        printf("%d:FTPStart\r\n",CTRL_SOCK);
        socket(CTRL_SOCK, Sn_MR_TCP, flocal_port, 0x00);
        break;
      
      case SOCK_INIT :
        printf("%d:Opened\r\n",CTRL_SOCK);
        connect(CTRL_SOCK, FTP_destip, FTP_destport);
        connect_state_control_ftpc = 0;
        printf("%d:Connectting...\r\n",CTRL_SOCK);
        break;
      default :
        break;
    }
    return 0;
}

/***
*@brief ½âÎö·þÎñÆ÷µÄÏìÓ¦Êý¾Ý
*@param: buf ftp·þÎñÆ÷µÄÏìÓ¦Êý¾Ý
************************/
char proc_ftpc(char * buf)
{
  
  uint16_t Responses;
  uint8_t dat[30]={0,};
  memset(dat,0,30);
  Responses =(buf[0]-'0')*100+(buf[1]-'0')*10+(buf[2]-'0');

  switch(Responses){
    case R_220:  /* Service ready for new user. */
      printf("\r\nInput your User ID > ");
      sprintf((char*)dat,"USER %s\r\n", "wiznet");//ÓÃ»§Ãû
      printf("\r\n");
      send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
      break;

    case R_331:  /* User name okay, need password. */
      printf("\r\nInput your Password > ");
      sprintf((char*)dat,"PASS %s\r\n", "123");
      printf("\r\n");
      send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
      break;
    case R_230:  /* User logged in, proceed */
      printf("\r\nUser logged in, proceed\r\n");

      sprintf((char*)dat,"TYPE %c\r\n", TransferAscii);
      ftpc.type = ASCII_TYPE;
      send(CTRL_SOCK, (uint8_t *)dat, strlen((const char*)dat));
      break;
    case R_200:
      if((ftpc.dsock_mode==ACTIVE_MODE)&&gModeActivePassiveflag){
        ftpc.dsock_state = DATASOCK_READY;
        gModeActivePassiveflag = 0;
      }
      else{
        gMenuStart = 1;
      }
      break;
    case R_150:
      switch(Command.First){
      case f_dir:
        Command.First = f_nocmd;
        Command.Second = s_dir;
        gDataPutGetStart = 1;
        break;
      case f_get:
        Command.First = f_nocmd;
        Command.Second = s_get;
        gDataPutGetStart = 1;
        break;
      case f_put:
        Command.First = f_nocmd;
        Command.Second = s_put;
        gDataPutGetStart = 1;
        break;
      default :
        printf("Command.First = default\r\n");
        break;
      }
      break;
    case R_226:
      gMenuStart = 1;
      break;
    case R_227:
      if (pportc(buf) == -1){
        printf("Bad port syntax\r\n");
      }
      else{
        printf("Go Open Data Sock...\r\n ");
        ftpc.dsock_mode = PASSIVE_MODE;
        ftpc.dsock_state = DATASOCK_READY;
      }
      break;
    default:
      printf("\r\nDefault Status = %d\r\n",(uint16_t)Responses);
      gDataSockReady = 1;
      break;
    }
  return 1;
}
int pportc(char * arg)
{
  int i;
  char* tok=0;
  strtok(arg,"(");
  for (i = 0; i < 4; i++)
  {
    if(i==0) tok = strtok(NULL,",\r\n");
    else   tok = strtok(NULL,",");
    fremote_ip.cVal[i] = (uint8_t)atoi(tok, 10);
    if (!tok){
      printf("bad pport : %s\r\n", arg);
      return -1;
    }
  }
  fremote_port = 0;
  for (i = 0; i < 2; i++){
    tok = strtok(NULL,",\r\n");
    fremote_port <<= 8;
    fremote_port += atoi(tok, 10);
    if (!tok){
      printf("bad pport : %s\r\n", arg);
      return -1;
    }
  }
  printf("ip : %d.%d.%d.%d, port : %d\r\n", fremote_ip.cVal[0], fremote_ip.cVal[1], fremote_ip.cVal[2], fremote_ip.cVal[3], fremote_port);
  return 0;
}
uint8_t* User_Keyboard_MSG(void)
{
  uint8_t i=0;
  do{
    gMsgBuf[i] = 1;
    i++;
  }while(gMsgBuf[i-1]!=0x0d);
  gMsgBuf[i-1]=0;
  return gMsgBuf;
}

/**
*@brief:ftpcommand  Æ´½ÓftpËùÐèµÄÃüÁî
*@param:cmd FTP ÃüÁî
*@para:sbuf ÃüÁîÄÚÈÝ
*@param:dbuf Ä¿±êbuffer
*******************************/

void ftp_cmd(uint8_t cmd,char* sbuf,char*dbuf)
{
  switch (cmd)
  {  
    case ABOR://ÖÐÖ¹ÃüÁî
      sprintf(dbuf,"ABOR\r\n");
      break;
    case ACCT://Ö¸¶¨ÓÃ»§µÄÕËºÅÐÅÏ¢ Ö»ÄÜÔÚPASSÃüÁîºóÊÕµ½332´úÂëºó·¢ËÍ:
      sprintf(dbuf,"ACCT %s\r\n",sbuf);
      break;
    case ALLO: //·¢ËÍÎÄ¼þÇ°ÔÚ·þÎñÆ÷·ÖÅäX¸ö×Ö½Ú
      sprintf(dbuf,"ALLO %s\r\n",sbuf);
      break;
    case APPE://ÈÃ·þÎñÆ÷×¼±¸½ÓÊÕÎÄ¼þ²¢½«Êý¾Ý¸½¼Óµ½Ö¸¶¨µÄÎÄ¼þ
      sprintf(dbuf,"APPE %s\r\n",sbuf);
      break;
    case CDUP://°Ñµ±Ç°Ä¿Â¼¸ÄÎª¸ùÎÄ¼þÄ¿Â¼
      sprintf(dbuf,"CDUP \r\n");
      break;
    case CWD://¸Ä±ä¹¤×÷Ä¿Â¼µ½Ö¸¶¨Â·¾¶
      sprintf(dbuf,"CWD %s\r\n",sbuf);
      break;
    case DELE://É¾³ý·þÎñÆ÷ÉÏÖ¸¶¨µÄÎÄ¼þ
      sprintf(dbuf,"DELE %s\r\n",sbuf);
      break;
    case HELP://°ïÖúÃüÁî
      sprintf(dbuf,"HELP %s\r\n",sbuf);
      break;
    case LIST://ÁÐ±íÃüÁî·þÎñÆ÷·¢ËÍ¸ø¿Í»§¶Ë
      sprintf(dbuf,"LIST %s\r\n",sbuf);
      break;
    case MKD://´´½¨Ä¿Â¼
      sprintf(dbuf,"MKD %s\r\n",sbuf);
       break;
    case MODE://´«ÊäÄ£Ê½ÃüÁî S , B, C
       sprintf(dbuf,"MODE %s\r\n",sbuf);
       break;
    case NLIST://·þÎñÆ÷·¢ËÍÎÄ¼þÃû³ÆÁÐ±í
       sprintf(dbuf,"NLISTT %s\r\n",sbuf);
       break;
    case NOOP://Ê²Ã´¶¼²»×ö
       sprintf(dbuf,"NOOP\r\n");
       break;
    case PASS://ÃÜÂëÃüÁî
        sprintf(dbuf,"PASS %s\r\n",sbuf);
       break;
    case PASV://·þÎñÆ÷±»¶¯Ä£Ê½
        sprintf(dbuf,"PASV %s\r\n",sbuf);
       break;
    case PORT://·þÎñÆ÷Ö÷¶¯Ä£Ê½
        sprintf(dbuf,"PORT %s\r\n",sbuf);
       break;
    case PWD://´òÓ¡¹¤×÷Ä¿Â¼
        sprintf(dbuf,"PWD\r\n");
       break;
    case QUIT://×¢ÏúÃüÁî
        sprintf(dbuf,"QUIT\r\n");
       break;
    case REIN://ÖØÐÂ³õÊ¼»¯ÃüÁî
        sprintf(dbuf,"REIN\r\n");
       break;
    case REST://ÖØÆôÃüÁî
        sprintf(dbuf,"REST %s\r\n",sbuf);
       break;
    case RETR://ÏÂÔØÃüÁî
      sprintf(dbuf,"RETR %s\r\n",sbuf);
      break;
     case RMD://É¾³ýÄ¿Â¼
        sprintf(dbuf,"RMD %s\r\n",sbuf);
       break;
     case RNFR://°ÑÎÄ¼þÖØÃüÃû
        sprintf(dbuf,"RNFR %s\r\n",sbuf);
       break;
     case RNTO://°ÑÎÄ¼þÖØÃüÃûÎª
        sprintf(dbuf,"RNTO %s\r\n",sbuf);
       break;
     case SITE://Õ¾µã²ÎÊýÃüÁî
        sprintf(dbuf,"SITE %s\r\n",sbuf);
       break;
     case SMNT://½á¹¹¹ÒÔØ
        sprintf(dbuf,"SMNT %s\r\n",sbuf);
       break;
     case STAT://×´Ì¬ÃüÁî
        sprintf(dbuf,"STAT %s\r\n",sbuf);
       break;
     case STRU://Ö¸¶¨´«ÊäÊý¾ÝµÄ½á¹¹ÀàÐÍ F ÎÄ¼þ½á¹¹ R¼ÇÂ¼½á¹¹ P Ò³½á¹¹
        sprintf(dbuf,"STRU %s\r\n",sbuf);
       break;
     case SYST://·þÎñÆ÷²Ù×÷ÏµÍ³ÀàÐÍ
        sprintf(dbuf,"SYST\r\n");
       break;
     case TYPE://Êý¾Ý´«Êä·½Ê½
        sprintf(dbuf,"TYPE %s\r\n",sbuf);
       break;
     case USER://µÇÂ½·þÎñÆ÷ÓÃ»§Ãû³Æ
        sprintf(dbuf,"USER %s\r\n",sbuf);
       break;
    default:
      break;
  }
}
