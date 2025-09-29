#!/bin/bash

FILE="data_store_config.h"

# 创建输出文件
USER_FILE="data_store_user_config.h"
echo "#warning \"<this file is generate by default_config_to_user_config.sh>, \
your are useing default config, \
please define DATA_STORE_USER_CONFIG_FILE to your own config file,\
or delete this warning, edit this file accord your project\"" > "$USER_FILE"

# 处理文件：保留整个文件结构，只移除指定区域内的#ifndef/#endif
sed -E '
/\/\/ script process start/,/\/\/ script process end/{
    /^#ifndef [A-Z_]+$/,/^#endif$/{
        /^#ifndef [A-Z_]+$/d
        /^#endif$/d
    }
}
' "$FILE" >> "$USER_FILE"

echo "处理完成，结果保存在: $USER_FILE"