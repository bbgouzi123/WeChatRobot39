# -*- coding: utf-8 -*-
"""
修复 ATL 转换宏问题
"""

import os
import re

def fix_atl_issues():
    """修复 ATL 相关问题"""
    
    # 1. 修复 ChatRoomOperate.cpp
    chat_room_file = "WeChatHelper/ChatRoomOperate.cpp"
    if os.path.exists(chat_room_file):
        with open(chat_room_file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # 替换 USES_CONVERSION 和 A2W
        old_code = """		USES_CONVERSION;
		//2.ͨ΢IDȡȺԱϢ
		GetUserInfoByWxid(A2W(tempWxidA));"""
        
        new_code = """		// 使用 Windows API 转换函数替代 ATL 转换宏
		int len = MultiByteToWideChar(CP_ACP, 0, tempWxidA, -1, NULL, 0);
		wchar_t* tempWxidW = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, tempWxidA, -1, tempWxidW, len);
		//2.通过微信ID获取群成员信息
		GetUserInfoByWxid(tempWxidW);
		delete[] tempWxidW;"""
        
        if old_code in content:
            content = content.replace(old_code, new_code)
        else:
            # 使用正则表达式替换
            pattern = r'USES_CONVERSION;\s*//.*?\n\s*GetUserInfoByWxid\(A2W\(tempWxidA\)\);'
            content = re.sub(pattern, new_code, content, flags=re.DOTALL)
        
        with open(chat_room_file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"[OK] 已修复 {chat_room_file}")
    
    # 2. 修复 CAutoFunction.cpp
    cauto_file = "WeChatHelper/CAutoFunction.cpp"
    if os.path.exists(cauto_file):
        with open(cauto_file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # 注释掉 atlconv.h
        content = content.replace('#include <atlconv.h>', '// #include <atlconv.h>  // 暂时注释掉，如果后续需要 ATL 功能再启用')
        
        with open(cauto_file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"[OK] 已修复 {cauto_file}")
    
    print("\n修复完成！")

if __name__ == "__main__":
    fix_atl_issues()

