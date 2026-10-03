/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Jul 24 12:41:57 1999
 */
/* Compiler settings for C:\projects\ctlspy2\debugger\debugger.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IDebugger = {0x3F209DC0,0x3314,0x11D3,{0x8B,0xF2,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_IDebuggerEvents = {0x3F209DC1,0x3314,0x11D3,{0x8B,0xF2,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_IClientAccessor = {0x3F209DC2,0x3314,0x11D3,{0x8B,0xF2,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID LIBID_DEBUGGERLib = {0xC72A9B90,0x3311,0x11D3,{0x8B,0xF2,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const CLSID CLSID_Debugger = {0x58C849A0,0x3314,0x11D3,{0x8B,0xF2,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const CLSID CLSID_ClientAccessor = {0x664E10F0,0x3F2B,0x11D3,{0x8B,0xFA,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


#ifdef __cplusplus
}
#endif

