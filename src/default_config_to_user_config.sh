#!/bin/bash

FILE="data_store_config.h"

# 创建输出文件
USER_FILE="data_store_user_config.h"
echo "#warning \"<this file is generate by default_config_to_user_config.sh>, \
your are useing default config, \
please define DATA_STORE_USER_CONFIG_FILE to your own config file,\
or delete this warning, edit this file accord your project\"" > "$USER_FILE"

echo "#ifndef XXX_USER_CONFIG_FILE" >> "$USER_FILE"
echo "#define XXX_USER_CONFIG_FILE" >> "$USER_FILE"


# 处理文件：只提取//script process start 与 //script process end之间的内容
sed -n '/\/\/ script process start/,/\/\/ script process end/p' "$FILE" | \
sed '/\/\/ script process start/d; /\/\/ script process end/d' >> "$USER_FILE"



echo "#endif" >> "$USER_FILE"


echo "处理完成，结果保存在: $USER_FILE"

