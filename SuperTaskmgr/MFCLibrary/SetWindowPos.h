
// SetWindowPos.h
#pragma once

inline BOOL SetWindowPosition(CWnd &wnd,int x,int y){
	return wnd.SetWindowPos(0,x,y,0,0,SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER);
}

inline BOOL SetWindowSize(CWnd &wnd,int cx,int cy){
	return wnd.SetWindowPos(0,0,0,cx,cy,SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
}

inline BOOL SetWindowLayout(CWnd &wnd,int x,int y,int cx,int cy){
	return wnd.SetWindowPos(0,x,y,cx,cy,SWP_NOOWNERZORDER|SWP_NOZORDER);
}


