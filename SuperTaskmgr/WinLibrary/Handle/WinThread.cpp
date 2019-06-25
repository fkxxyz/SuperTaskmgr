
// WinThread.cpp

#include "stdafx.h"
#include "WinThread.h"

DWORD CThreadProc::StartProc(LPVOID lpParam){
	ASSERT(lpParam);
	PARAM_THIS &stParamThis=*(PPARAM_THIS)lpParam;

	ASSERT((CThreadProc*)stParamThis.pThis);
	CThreadProc &Me=*(CThreadProc*)stParamThis.pThis;

	return Me.ThreadProc(stParamThis.lpParam);
}


BOOL CMsgThread::PostCallBackMessage(PCALLBACK_MSG pmsg){
	if (pmsg->hwnd==NULL){
		if (pmsg->dwThreadID!=0)
			return ::PostThreadMessage(pmsg->dwThreadID,pmsg->message,pmsg->wParam,pmsg->lParam);
		else
			return TRUE;
	} else
		return ::PostMessage(pmsg->hwnd,pmsg->message,pmsg->wParam,pmsg->lParam);
}

typedef struct _MSG_THREADPROC_PARAM {
	CEvent *pCreateFinished;
	HWND hCallbackWnd;
	DWORD dwCallbackThreadID;
	UINT message;
} MSG_THREADPROC_PARAM, *PMSG_THREADPROC_PARAM;

DWORD CMsgThread::ThreadProc(LPVOID lpParam){
	ASSERT(lpParam);
	MSG_THREADPROC_PARAM &stParam=*(PMSG_THREADPROC_PARAM)lpParam;
	CEvent &CreateFinished=*stParam.pCreateFinished;

	//������Ϣ����
	MSG msg;
	PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);

	//����¼�
	if (!CreateFinished.Set()){
		ASSERT(0);
		return 0;
	}

	//��ʼ��Ϣѭ��
	CALLBACK_MSG msgCallBack;
	msgCallBack.hwnd=stParam.hCallbackWnd;
	msgCallBack.dwThreadID=stParam.dwCallbackThreadID;
	msgCallBack.message=stParam.message;
	while (GetMessage(&msg,NULL,0,0)){
		if (msg.message==0) continue;

		//����Ϊæµ״̬
		MsgFinished.Reset();

		//������Ϣ
		TranslateMessage(&msg);

		//ִ����Ϣ����
		OnMsg(msg.message,msg.wParam,msg.lParam,&msgCallBack);

		//���ͻص���Ϣ
		PostCallBackMessage(&msgCallBack);

		//����Ƿ�����Ϣ�����û��������Ϊ����״̬��
		if (!PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
			MsgFinished.Set();
		}
	}
	return 0;
}

BOOL CMsgThread::Create(PCALLBACK_MSG pCallBackFinishedMsg,SIZE_T dwStackSize){
	CLastErrorFunction result;

	//�����ȴ���Ϣ���д������¼�
	CEvent CreateFinished;
	if (!(CreateFinished.Create(TRUE,FALSE))){
		result.SetErrorCode();
		goto end;
	}

	//�����ȴ���Ϣ����¼�
	if (!(MsgFinished.Create(TRUE,TRUE))){
		result.SetErrorCode();
		goto end;
	}

	{
		MSG_THREADPROC_PARAM stParam;//={NULL,NULL,0,0};
		stParam.pCreateFinished=&CreateFinished;
		if (pCallBackFinishedMsg){
			stParam.hCallbackWnd=pCallBackFinishedMsg->hwnd;
			stParam.dwCallbackThreadID=pCallBackFinishedMsg->dwThreadID;
			stParam.message=pCallBackFinishedMsg->message;
		}
		if (!CThreadProc::Create(&stParam,FALSE,dwStackSize)){
			result.SetErrorCode();
			goto end;
		}
	}

	//�ȴ��¼���������Ϣ���У����
	if (!CreateFinished.Wait()){
		result.SetErrorCode();
		VERIFY(Terminate());
		goto end;
	}

end:
	return result;
}



