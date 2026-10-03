/////////////////////////////////////////////////////////////
// CoDelegator.cpp - COMSpy
//
// Copyright 2000, Jason Whittington
//
// COMSpy uses a really prmitive method for looking up
// method names.  Simon Fell has a better scheme that
// should obsolesce this whole thing with a better
// implementation of GetMethodName().
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "methodNames.h"

#include <docobj.h>

static LPOLESTR Generic_MethodNames[] = 
{
L"Method4",L"Method5",L"Method6",
L"Method7",L"Method8",L"Method9",
L"Method10",L"Method11",L"Method12",
L"Method13",L"Method14",L"Method15",
L"Method16",L"Method17",L"Method18",
L"Method19", L"Method20", L"Method21",
L"Method22", L"Method23", L"Method24",
L"Method25", L"Method26", L"Method27",
L"Method28", L"Method29", L"Method30",
L"Method31", L"Method32", L"Method33",
L"Method34", L"Method35", L"Method36",
L"Method37", L"Method38", L"Method39",
L"Method40", L"Method41", L"Method42",
L"Method43", L"Method44", L"Method45",
L"Method46", L"Method47", L"Method48",
L"Method49", L"Method50", L"Method51",
};

static LPOLESTR IOleObject_MethodNames[] = 
{
L"SetClientSite",L"GetClientSite",L"SetHostNames",
L"Close",L"SetMoniker",L"GetMoniker",L"InitFromData",
L"GetClipboardData",L"DoVerb",L"EnumVerbs",L"Update",
L"IsUpToDate",L"GetUserClassID",L"GetUserType",L"SetExtent",
L"GetExtent",L"Advise",L"Unadvise",L"EnumAdvise",L"GetMiscStatus",
L"GetColorScheme"
};

static LPOLESTR IViewObjectEx_MethodNames[] = 
{
L"Draw",L"GetColorSet",L"Freeze",L"Unfreeze",L"SetAdvise",
L"GetAdvise",L"GetExtent",L"GetRect",L"GetViewStatus",
L"QueryHitPoint",L"QueryHitRect",L"GetNaturalExtent"

};
static LPOLESTR IOleControlSite_MethodNames[] = 
{
L"OnControlInfoChanged",L"LockInPlaceActive",L"GetExtendedControl",
L"TransformCoords",L"TranslateAccelerator",L"OnFocus",L"ShowPropertyFrame"
};

static LPOLESTR IOleClientSite_MethodNames[] = 
{
L"SaveObject",L"GetMoniker",L"GetContainer",L"ShowObject",
L"OnShowWindow",L"RequestNewObjectLayout"
};
static LPOLESTR IOleInPlaceSiteWindowless_MethodNames[] = 
{
L"GetWindow", L"ContextSensitiveHelp", L"CanInPlaceActive", L"OnInPlaceActivate",
L"OnUIActivate",L"GetWindowContext",L"Scroll",L"OnUIDeactivate",L"OnInPlaceDeactivate",
L"DiscardUndoState",L"DeactivateAndUndo",L"OnPosRectChanged",L"OnInPlaceActivateEx",
L"OnInPlaceDeactivateEx",L"RequestUIActivate",
L"CanWindowlessActivate",L"GetCapture",L"SetCapture",
L"GetFocus",L"SetFocus",L"GetDC",L"ReleaseDC",
L"InvalidateRect",L"InvalidateRgn",L"ScrollRect",
L"AdjustRect",L"OnDefWindowMessage"
};

static LPOLESTR IOleControl_MethodNames[] = 
{
L"GetControlInfo",L"OnMnemonic",L"OnAmbientPropertyChange",L"FreezeEvents"
};

static LPOLESTR IOleInPlaceObject_MethodNames[] = 
{
L"GetWindow",L"ContextSensitiveHelp",
L"InPlaceDeactivate",L"UIDeactivate",
L"SetObjectRects",L"ReactivateAndUndo"
};

static LPOLESTR IOleInPlaceActiveObject_MethodNames[] = 
{
L"GetWindow",L"ContextSensitiveHelp",
L"TranslateAccelerator",L"OnFrameWindowActivate",
L"OnDocWindowActivate",L"ResizeBorder",
L"EnableModeless"
};


static LPOLESTR IAdviseSink2_MethodNames[] = 
{ 
L"OnDataChange",L"OnViewChange",L"OnRename",L"OnSave",L"OnClose",L"OnLinkSrcChange"
};

static LPOLESTR IAdviseSinkEx_MethodNames[] = 
{ 
L"OnDataChange",L"OnViewChange",L"OnRename",L"OnSave",L"OnClose",L"OnViewStatusChange"
};

static LPOLESTR IPointerInactive_MethodNames[] = 
{
L"GetActivationPolicy",L"OnInactiveMouseMove",L"OnInactiveSetCursor"
};

static LPOLESTR IDispatch_MethodNames[] = 
{
L"GetTypeInfoCount",L"GetTypeInfo",L"GetIDsOfNames",L"Invoke"
};

static LPOLESTR IDataObject_MethodNames[] = 
{
L"GetData",L"GetDataHere",L"QueryGetData",L"GetCanonicalFormatEtc",
L"SetData",L"EnumFormatEtc",L"DAdvise",L"DUnadvise",L"EnumDAdvise"
};

static LPOLESTR IOleDocument_MethodNames[] = 
{
L"CreateView", L"GetDocMiscStatus", L"EnumViews"
};

static LPOLESTR IOleDocumentSite_MethodNames[] = 
{
L"ActivateMe"
};

static LPOLESTR IOleDocumentView_MethodNames[] = 
{
L"SetInPlaceSite",L"GetInPlaceSite",L"GetDocument",
L"SetRect",L"GetRect",L"SetRectComplex",L"Show",
L"UIActivate",L"Open",L"CloseView",L"SaveViewState",
L"ApplyViewState",L"Clone"
};

static LPOLESTR IPersistStreamInit_MethodNames[] = 
{
L"GetClassID",L"IsDirty",L"Load",L"Save",L"GetSizeMax",L"InitNew"
};

static LPOLESTR IPersistStorage_MethodNames[] = 
{
L"GetClassID",L"IsDirty",L"InitNew",L"Load",L"Save",L"SaveCompleted",L"HandsOffStorage"
};

static LPOLESTR IPerPropertyBrowsing_MethodNames[] = 
{
L"GetDisplayString",L"MapPropertyToPage",L"GetPredefinedStrings",L"GetPredefinedValue"
};
static LPOLESTR IConnectionPointContainer_MethodNames[] = 
{
L"EnumConnectionPoints", L"FindConnectionPoint"
};


static LPOLESTR IOleInPlaceObjectWindowless_MethodNames[] = 
{
L"GetWindow", L"ContextSensitiveHelp", L"InPlaceDectivate", L"UIDeactivate",
 L"SetObjectRects", L"ReactivateAndUndo", L"OnWindowMessage", L"GetDropTarget" 
};



HRESULT GetMethodName(REFIID riid, long vtblIndex, BSTR *pbstrMethodName)
    {
	// TODO:Actually try and figure out the method name based
    //on the IID passed in.

	if(vtblIndex == 0) {*pbstrMethodName = ::SysAllocString(L"QueryInterface"); return S_OK;}
	if(vtblIndex == 1) {*pbstrMethodName = ::SysAllocString(L"AddRef"); return S_OK;}
	if(vtblIndex == 2) {*pbstrMethodName = ::SysAllocString(L"Release"); return S_OK;}

#define USE_INTERFACE_TABLE(iid)\
	else if(riid == IID_##iid) \
       *pbstrMethodName = ::SysAllocString(iid##_MethodNames[vtblIndex]);
#define USE_INTERFACE_TABLE2(iid, tbl)\
	else if(riid == IID_##iid) \
       *pbstrMethodName = ::SysAllocString(tbl##_MethodNames[vtblIndex]);

	//For any other interface we'll grab something out of our tables...
	vtblIndex -= 3;
	if((riid == IID_IViewObject) || (riid == IID_IViewObject2) || (riid == IID_IViewObjectEx))
       *pbstrMethodName = ::SysAllocString(IViewObjectEx_MethodNames[vtblIndex]);
	
	USE_INTERFACE_TABLE(IPointerInactive)
	USE_INTERFACE_TABLE(IOleClientSite)
	USE_INTERFACE_TABLE(IOleControlSite)
	USE_INTERFACE_TABLE(IOleInPlaceObjectWindowless)
	USE_INTERFACE_TABLE(IOleInPlaceSiteWindowless)
	USE_INTERFACE_TABLE2(IOleInPlaceSiteEx,IOleInPlaceSiteWindowless)
	USE_INTERFACE_TABLE2(IOleInPlaceSite,IOleInPlaceSiteWindowless)
	USE_INTERFACE_TABLE(IOleControl)
	USE_INTERFACE_TABLE(IOleInPlaceObject)
	USE_INTERFACE_TABLE(IOleInPlaceActiveObject)
	USE_INTERFACE_TABLE(IOleObject)
	USE_INTERFACE_TABLE2(IOleWindow,IOleInPlaceActiveObject)
	USE_INTERFACE_TABLE(IAdviseSink2)
	USE_INTERFACE_TABLE2(IAdviseSink,IAdviseSink2)
	USE_INTERFACE_TABLE(IAdviseSinkEx)
	USE_INTERFACE_TABLE(IDispatch)
	USE_INTERFACE_TABLE(IDataObject)
	USE_INTERFACE_TABLE(IOleDocument)
	USE_INTERFACE_TABLE(IOleDocumentSite)
	USE_INTERFACE_TABLE(IOleDocumentView)
	USE_INTERFACE_TABLE(IPersistStreamInit)
	USE_INTERFACE_TABLE2(IPersistStream,IPersistStreamInit)
	USE_INTERFACE_TABLE2(IPersist,IPersistStreamInit)
	USE_INTERFACE_TABLE(IPersistStorage)
	USE_INTERFACE_TABLE(IConnectionPointContainer)
	//USE_INTERFACE_TABLE(IPerPropertyBrowsing)

	else
		*pbstrMethodName = ::SysAllocString(Generic_MethodNames[vtblIndex]);	
	return S_OK;
    }

