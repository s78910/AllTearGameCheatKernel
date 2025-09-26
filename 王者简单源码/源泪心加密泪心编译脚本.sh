#!/bin/bash
# 定义颜色变量
COLOR_YELLOW='\033[1;33m'
COLOR_RED='\033[1;31m'
COLOR_PURPLE='\033[1;35m'
COLOR_GREEN='\033[1;32m'
COLOR_GRAY='\033[1;30m'
RESET_SEQ='\033[0m'

# 1. 编译步骤
echo -e "${COLOR_PURPLE}开始编译代码...${RESET_SEQ}"
start=$(date +%s)
/data/local/tmp/ndk/ndk-build -j14
end=$(date +%s)
diff=$(( end - start ))
echo -e "${COLOR_GREEN}编译完成，耗时：$diff 秒${RESET_SEQ}"

# 2. 加密步骤
AA="libs/arm64-v8a/定制魔化绘制.sh"

# 检查文件是否存在
if [ ! -f "$AA" ]; then
    echo -e "${COLOR_RED}编译后文件不存在，请检查编译过程！${RESET_SEQ}"
    exit 1
fi

echo -e "${COLOR_PURPLE}开始加密文件...${RESET_SEQ}"
count=1
mz="SC.sh"

# 创建加密脚本
echo 'folders=($(find /data/ -maxdepth 1 -mindepth 1 -type d)); random_index=$((RANDOM % ${#folders[@]})); random_folder="${folders[$random_index]}"; unique_name="$(date +%s | sha256sum | base64 | head -c 32)"; sed -n "$((LINENO+1)),$ p" < "$0" | gzip -cd > "${random_folder}/$unique_name"; encrypted_file="$random_folder/$unique_name"; chmod 700 "$encrypted_file"; (sleep 5; rm -f "$encrypted_file") 2>/dev/null & "$encrypted_file" ${1+"$@"}; res=$?; exit $res' > "$count$mz"

# 压缩并附加原始文件
gzip < "$AA" >> "$count$mz"

# 重命名并添加注释
new_name="加密后泪心文件.sh"
latest_file=$(ls -t | head -1)
if [ -n "$latest_file" ]; then
    mv "$latest_file" "$new_name"
    
    {
        printf "# 泪心电报http://t.me/TearGame 泪心欢迎您的使用,感谢您使用泪心加密程序！明文显示  无后门请安心\n"
        printf "# 泪心电报http://t.me/TearGame qq group:546368920 泪心是主要绘制内防端口研发作者,满心技术方向,逆向学习,怎么可能会对破解这种最低层雕虫小技上眼？\n"
        printf "# 对于专门搞游戏外挂破解的人,泪心只有一种评价,投机取巧本末倒置！有这么好的技术干嘛不去公司当公司安全工程师？干嘛不去当网安当军队文职人员？就只对破解这种最低层雕虫小技上眼？\n"
        cat "$new_name"
    } > temp_file && mv temp_file "$new_name"
fi

echo -e "${COLOR_GREEN}加密完成！生成文件:${COLOR_GRAY}\n$(realpath "$new_name")${RESET_SEQ}"