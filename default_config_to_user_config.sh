#!/bin/bash


FILE="$1"

if [ $# -ne 1 ]; then
    echo "用法: $0 <文件名>"
    exit 1
fi

if [ ! -f "$FILE" ]; then
    echo "错误: 文件 '$FILE' 不存在"
    exit 1
fi

# 创建临时文件
USER_FILE=$(touch data_store_user_config.h)

# 处理文件
sed -E '
/#ifndef DATA_STORE_CONFIG_H/d
/#define DATA_STORE_CONFIG_H/d
/#endif.*DATA_STORE_CONFIG_H/d
/#ifndef [A-Z_]+_H/{:a;N;/#endif/!ba;d}
' "$FILE" > "$USER_FILE"

