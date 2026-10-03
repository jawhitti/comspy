/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jan 20 15:25:04 2000
 */
/* Compiler settings for C:\projects\comspy\ctlspysrv\ctlspysrv.idl:
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

const IID IID_IDebuggerPrivate = {0xF10764D6,0x342A,0x11D3,{0x8B,0xF3,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_IObjectEvents = {0xF10764D5,0x342A,0x11D3,{0x8B,0xF3,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_IObjectBreakpointEvents = {0xF10764D4,0x342A,0x11D3,{0x8B,0xF3,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_IDebuggedObject = {0xF10764D3,0x342A,0x11D3,{0x8B,0xF3,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


const IID IID_ISpyAccessor = {0xF10764D2,0x342A,0x11D3,{0x8B,0xF3,0x00,0x10,0x5A,0x6D,0xC0,0x77}};


#ifdef __cplusplus
}
#endif

