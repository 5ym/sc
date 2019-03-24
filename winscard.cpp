//  Put the following files in the same folder:
//      Crypto.cpp, Crypto.h, Decoder-ECM.cpp, Decoder.h, Global.h
//      Keys.cpp, Keys.h, Keyset.cpp, Keyset.h
//  You should modify "Size" in Decoder-ECM.cpp l42 to "Size-3"

#include <string.h>
#include <PCSC/winscard.h>
#define htons htons_
#define ntohs ntohs_

#include "Global.h"
#include "Keys.h"
#include "Decoder.h"

#define CHAR	char

extern"C"{

void __attribute__ ((constructor)) SCardVCasInit(void) {
    BCAS::Keys::RegisterAll();
}

LONG SCardEstablishContext(DWORD dwScope, LPCVOID pvReserved1,LPCVOID pvReserved2, LPSCARDCONTEXT phContext)
{
	*phContext=(SCARDCONTEXT)0x35313239;
	return SCARD_S_SUCCESS;
}

LONG SCardListReaders(SCARDCONTEXT hContext, LPCSTR mszGroups, LPSTR mszReaders, LPDWORD pcchReaders)
{
    CHAR r[]="@OishiiSlurper\0";
    if(mszReaders)memcpy(mszReaders,r,sizeof r);
    *pcchReaders=sizeof r/sizeof r[0];
    return SCARD_S_SUCCESS;
}


LONG SCardConnect(SCARDCONTEXT hContext, LPCSTR szReader, DWORD dwShareMode, DWORD dwPreferredProtocols, LPSCARDHANDLE phCard, LPDWORD pdwActiveProtocol)
{
    *phCard=(SCARDHANDLE)0x35313239;
    *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}


LONG SCardTransmit(SCARDHANDLE hCard, LPCSCARD_IO_REQUEST pioSendPci, LPCBYTE pbSendBuffer, DWORD cbSendLength, LPSCARD_IO_REQUEST pioRecvPci, LPBYTE pbRecvBuffer, LPDWORD pcbRecvLength)
{
    switch(pbSendBuffer[1])
    {
    case 0x30:{
        BYTE r[]={
            0x00,0x39,0x00,0x00,0x21,0x00,0x00,0x05, 0xc0,0xff,0xee,0xc0,0xff,0xee,0x01,0x50,
            0x36,0x31,0x04,0x66,0x4b,0x17,0xea,0x5c, 0x32,0xdf,0x9c,0xf5,0xc4,0xc3,0x6c,0x1b,
            0xec,0x99,0x39,0x21,0x68,0x9d,0x4b,0xb7, 0xb7,0x4e,0x40,0x84,0x0d,0x2e,0x7d,0x98,
            0xfe,0x27,0x19,0x99,0x19,0x69,0x09,0x11, 0x01,0x02,0x01,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x32:{
        BYTE r[]={
            0x00,0x0f,0x00,0x00,0x21,0x00,0x01,0x54, 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x34:{
        BYTE r[]={
            0x00,0x15,0x00,0x00,0x08,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x90, 0x00};
        BCAS::Decoder::DecodeECM(pbSendBuffer+5,pbSendBuffer[4],r+6,NULL);
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x80:{
	BYTE r[]={
		0x00,0x10,0x00,0x00,0xA1,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x00};
	memcpy(pbRecvBuffer,r,sizeof r);
	*pcbRecvLength=sizeof r;
	break;}
    default:{
        BYTE r[]={
            0x00,0x04,0x00,0x00,0xa1,0xfe,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    }
    return SCARD_S_SUCCESS;
}

LONG SCardDisconnect(SCARDHANDLE hCard, DWORD dwDisposition){return SCARD_S_SUCCESS;}

LONG SCardFreeMemory(SCARDCONTEXT,LPCVOID){return SCARD_S_SUCCESS;}

LONG SCardReleaseContext(SCARDCONTEXT){return SCARD_S_SUCCESS;}

LONG SCardGetStatusChange(SCARDCONTEXT hContext, DWORD dwTimeout,LPSCARD_READERSTATE rgReaderStates, DWORD cReaders){return SCARD_S_SUCCESS; }

SCARD_IO_REQUEST g_rgSCardT1Pci_;
const SCARD_IO_REQUEST g_rgSCardT1Pci = { SCARD_PROTOCOL_T1, sizeof(SCARD_IO_REQUEST) };

}