/// @file \BusinessLayer\constDefine_BusinessLayer.h

#ifndef __BUSINESSLAYER_CONSTDEFINE_BUSINESSLAYER_H__
#define __BUSINESSLAYER_CONSTDEFINE_BUSINESSLAYER_H__

#define TIMER_DELAY_PLAY 100

#define SERVICE_NAME_W L"ls_test_service"
#define LOG_FILE_NAME_DLG_NOTIFY L"log_dlg_notify.txt"
#define LOG_FILE_NAME_LSSERVICEFORTEST L"log_lsServiceForTest.txt"

#define FILE_NAME_dlgNotify L"dlgNotify.exe"
#define PE_INSTANCE_MUTEX_QUIT_dlgNotify L"{EBFA31FA-6951-4330-A89A-7C6145C26C36}"

#define FILE_NAME_ObjProgInfo FILE_NAME_dlgNotify

/// 单实例互斥锁
#define PE_INSTANCE_NAME_DLG_NOTIFY L"{E5287660-EB54-4BB9-A947-2985C82B4D55}"

/// 激活事件
#define EVENT_NAME_INSTANCE_ACTIVE_DLG_NOTIFY L"{199262D1-AE97-4376-83B4-54355BD9A948}"

 ///< 单实例互斥锁名称-让DlgNotify退出
#define PE_INSTANCE_NAME_CMD_LET_DLG_NOTIFY_QUIT L"{4B564F50-0794-46A5-9907-EACFF4D010CA}"


/// xml资源
#define XML_FILE_NAME_dlgHint_MainDlg L"Hintskin.xml"
#define WND_CLASS_NAME_digHint_MainDlg L"wcn_dlgHintShow_MainDlg"


#define XML_FILE_NAME_dlgNotify_MainDlg L"dlgNotify_MainDlg.xml"
#define WND_CLASS_NAME_dlgNotify_MainDlg L"wcn_FzNotify_MainDlg"
#define WND_DISP_NAME_dlgNotify_MainDlg L"wdn_FzNotify_MainDlg"

#define XML_FILE_NAME_dlgSlidingShow_MainDlg L"SlidingDlg.xml"
#define WND_CLASS_NAME_dlgSlidingShow_MainDlg L"wcn_dlgSlidingShow_MainDlg"
#define WND_DISP_NAME_dlgSlidingShow_MainDlg L"wdn_dlgSlidingShow_MainDlg"

#define WM_USER_SELF_DEFINE_BEGIN WM_APP + 1000
#define WM_TAB_KEY_PRESS                WM_USER_SELF_DEFINE_BEGIN + 1
#define WM_ENTER_KEY_PRESS              WM_USER_SELF_DEFINE_BEGIN + 2
#define WM_UP_KEY_PRESS                 WM_USER_SELF_DEFINE_BEGIN + 3
#define WM_DOWN_KEY_PRESS               WM_USER_SELF_DEFINE_BEGIN + 4
#define WM_CANCEL_KEY_PRESS             WM_USER_SELF_DEFINE_BEGIN + 5
#define WM_SWITCH_VIEW                  WM_USER_SELF_DEFINE_BEGIN + 6
#define WM_SLIDING_SHOW                 WM_USER_SELF_DEFINE_BEGIN + 7
#define WM_SLIDING_QUIT					WM_USER_SELF_DEFINE_BEGIN + 8

/// ! 一定使 WM_OUR_MSG_LIMIT_DOWN 等于 最后一条自定义消息的值
/// ! WM_OUR_MSG_LIMIT_BEGIN 不要改
#define WM_OUR_MSG_LIMIT_BEGIN                  WM_USER_SELF_DEFINE_BEGIN + 1 ///< 自定义消息的开始值
#define WM_OUR_MSG_LIMIT_END                    WM_USER_SELF_DEFINE_BEGIN + 7 ///< 自定义消息的结束值

#endif // #ifndef __BUSINESSLAYER_CONSTDEFINE_BUSINESSLAYER_H__
