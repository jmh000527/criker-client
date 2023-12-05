#pragma once

#include "thirdparty/include/nlohmann/json.hpp"

enum class MsgType {
    LOGIN_MSG = 1,      //登陆消息
    REG_MSG,            //注册消息
    LOGIN_MSG_ACK,      //登陆响应消息
    REG_MSG_ACK,        //注册响应消息
    ONE_CHAT_MSG,       //聊天消息
    ADD_FRIEND_MSG,     //添加好友消息

    CREATE_GROUP_MSG,   //创建群组消息
    ADD_GROUP_MSG,      //加入群组消息
    GROUP_CHAT_MSG,     //群聊天消息

    LOGOUT_MSG,         //注销登陆消息
};

template<typename T>
T getValueFromJson(const nlohmann::json& json, const std::string& key) {
    // 如果是对象，直接获取值
    if (json.is_object()) {
        return json.value(key, T{});
    }
    // 如果是数组，获取第一个元素的值
    else if (json.is_array() && !json.empty()) {
        return json[0].value(key, T{});
    }
    // 其他情况返回默认值
    else {
        return T{};
    }
}