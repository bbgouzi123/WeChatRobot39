#pragma once
// 微信 3.9.5.81 版本偏移地址 (基于 wxhelper 项目)
// 注意：由于实现方式不同，部分偏移地址需要根据实际情况调整

// 接收消息相关 (3.9.5.81)
#define WxReciveMessage 0x1010d80			//接收消息 Hook点 (kDoAddMsg)
#define WxReciveMessageCall 0x1010d80		//接收消息 Call (kDoAddMsg)
#define WxSyncMsg 0xc39680					//同步消息 (kSyncMsg)

// 好友列表相关 (3.9.5.81)
#define WxFriendList 0xeab270				//好友列表 (kGetContactList)
#define WxFriendListCall 0xeab270			//好友列表 Call
#define WxGoToQrCode1 0x264830				//跳转到二维码 1
#define WxGoToQrCode2 0x3ADE40				//跳转到二维码 1
#define WxQrCodeOffset 0x266AAC				//获取二维码 1
#define WxQrCodeOffsetCall 0x58BD30			//获取二维码 1
// 发送消息相关 (3.9.5.81)
#define WxSendMessage 0xfcd8d0				//发送文本消息 (kSendTextMsg)
#define WxGetSendMessageMgr 0x8c00e0		//获取发送消息管理器 (kGetSendMessageMgr)
#define WxFreeChatMsg 0x8aaa00				//释放聊天消息 (kFreeChatMsg)
#define WxLogout 0x4E7210						//退出 1
// 发送文件消息相关 (3.9.5.81)
#define WxSendFileMsg 0xdd27f0				//发送文件消息 (kSendFileMsg)
#define WxGetAppMsgMgr 0x8c33f0				//获取应用消息管理器 (kGetAppMsgMgr)
// 注意：以下偏移地址在 3.9.5.81 版本中可能已改变，需要重新分析
#define WxFileMessage1 0x58DA10				//发送文件消息 Call1 (旧版本，需要更新)
#define WxFileMessage2 0x58DA50				//发送文件消息 Call2 (旧版本，需要更新)
#define WxFileMessage3 0x68D00				//发送文件消息 Call3 (旧版本，需要更新)
#define WxFileMessage4 0x2C1960				//发送文件消息 Call4 (旧版本，需要更新)
#define WxFileMessageParam 0x17FFE10		//发送文件消息参数 (旧版本，需要更新)
// 发送图片消息相关 (3.9.5.81)
#define WxSendImageMsg 0xfc3d30				//发送图片消息 (kSendImageMsg)
#define WxChatMsgInstanceCounter 0x8c7fd0	//聊天消息实例计数器 (kChatMsgInstanceCounter)
// 注意：以下偏移地址在 3.9.5.81 版本中可能已改变，需要重新分析
#define WxSendImageCall1 0x639F0			//发送图片消息 Call1 (旧版本，需要更新)
#define WxSendImageCall2 0x3B5C70			//发送图片消息 Call2 (旧版本，需要更新)
#define WxSendImageCall3 0x58DA50			//发送图片消息 Call3 (旧版本，需要更新)
#define WxPatchAddr 0x3B2AE6				//发送图片消息 Patch地址 (旧版本，需要更新)
#define WxSetRoomAnnouncement 0x3074E0			//发送群公告 1
#define WxDeleteUser 0x325C80					//删除好友 1
#define WxQuitChatRoom 0x3045B0					//退出群聊 1
#define WxAddGroupMemberCall1 0x827B0			//添加群成员 1
#define WxAddGroupMemberCall2 0x58DB60			//添加群成员 1
#define WxAddGroupMemberCall3 0x932D0			//添加群成员 1
#define WxAddGroupMemberCall4 0x2FF220			//添加群成员 1
#define WxAddGroupMemberParam1 0x1AD1B34		//添加群成员 1
#define WxAddGroupMemberParam2 0x1AD2358		//添加群成员 1
#define WxSendXmlCard 0x3B63B0				//发送名片 1
#define WxGetRoomUserWxidCall1 0x512DD0			//获取群成员ID 1
#define WxGetRoomUserWxidCall2 0x379A90			//获取群成员ID 1
#define WxGetRoomUserWxidCall3 0x5193F0			//获取群成员ID 1
#define WxGetRoomUserWxidCall4 0x5137F0			//获取群成员ID 1
#define WxGetUserInfoWithNoNetworkCall1 0x58DDD0		 //根据微信ID获取用户信息 1
#define WxGetUserInfoWithNoNetworkCall2 0x3245E0		 //根据微信ID获取用户信息 1
#define WxGetUserInfoWithNoNetworkCall3 0x4FD8F0		 //根据微信ID获取用户信息 1
#define WxGetUserInfoByWxidCall1 0x58DA50		//根据微信ID获取用户信息 1
#define WxGetUserInfoByWxidCall2 0x63930		//根据微信ID获取用户信息 1
#define WxGetUserInfoByWxidCall3 0x3245E0		//根据微信ID获取用户信息 1
#define WxAddWxUserParam1 0x1730C30				//添加好友 1
#define WxAddWxUserCall1 0x57860				//添加好友 1
#define WxAddWxUserCall2 0x83780				//添加好友 1
#define WxAddWxUserCall3 0x58DA10				//添加好友 1
#define WxAddWxUserCall4 0x58DA50				//添加好友 1
#define WxAddWxUserCall5 0x322B20				//添加好友 1
#define WxSetRoomName 0x303A50					//修改群名称 1
#define WxCllectMoneyCall1 0x9C5310				//收款 1
#define WxCllectMoneyCall2 0x9C5390				//收款 1
#define WxAgreeUserRequestCall1 0x1F0360		//同意好友请求 1
#define WxAgreeUserRequestCall2 0x5D860			//同意好友请求 1
#define WxAgreeUserRequestCall3 0x10FFA0		//同意好友请求 1
#define WxAgreeUserRequestCall4 0x1D7180		//同意好友请求 1
#define WxAgreeUserRequestParam 0x1AB2E98		//同意好友请求 1
#define WxGetExpressionsAddr 0x2D3C68			//提取微信表情  没啥用 去除
#define WxGetExpressionsCallAddr 0x2D7E60		//提取微信表情  没啥用 去除
#define WxDelRoomMemberCall1 0x58DA50			//删除群成员 1
#define WxDelRoomMemberCall2 0x7CD80			//删除群成员 1
#define WxDelRoomMemberCall3 0x2FF410			//删除群成员 1
#define WxOpenUrlCall1 0x58DA50					//打开浏览器 1
#define WxOpenUrlCall2 0x9C9CA0					//打开浏览器 1
#define WxPackageMsgData 0x827B0		//组装艾特消息数据结构 1
#define WxAntiRevoke 0x3DED99			//防撤回 1
//聊天记录偏移
#define MsgTypeOffset 0x30				//消息类型的偏移 
#define MsgContentOffset 0x68			//消息内容的偏移 
#define MsgSourceOffset 0x1B8			//消息来源的偏移 
#define WxidOffset 0x40					//微信ID/群ID偏移 	
#define GroupMsgSenderOffset 0x164		//群消息发送者偏移 
// 登录和个人信息相关 (3.9.5.81)
#define WxGetAccountServiceMgr 0x8c1230		//获取账户服务管理器 (kGetAccountServiceMgr)
#define LoginSign_Offset 0x7F8				//登录标志位偏移 (在账户服务中)
// 个人信息基址 - 需要通过账户服务获取
#define WxSelfInfoBase 0x0					// 需要通过账户服务获取，不再使用固定基址
//微信ID ASCII指针
#define WxID WxSelfInfoBase+0x44	//
//大头像 ASCII指针
#define WxBigHeader WxSelfInfoBase+0x384	//
//小头像 ASCII指针
#define WxSmallHeader WxSelfInfoBase+0x39C	//
//国家 ASCII字符
#define WxNation WxSelfInfoBase+0x298		//
//微信号 ASCII字符
#define WxCount WxSelfInfoBase+0x220		//
//城市 ASCII字符
#define WxProvince WxSelfInfoBase+0x1A8		//
//地区  ASCII字符
#define WxCity WxSelfInfoBase+0x1C0			//
//手机号 ASCII字符
#define WxPhoneNumber WxSelfInfoBase+0xF0   //
//昵称 ASCII字符
#define WxNickName WxSelfInfoBase+0xBC		//
//缓存目录  Unicode指针
#define WxCacheDir WxSelfInfoBase+0x10		//
//登陆设备 ASCII字符
#define WxDevice WxSelfInfoBase+0x510		//
//性别 
#define WxSex WxSelfInfoBase+0x1A4			//
