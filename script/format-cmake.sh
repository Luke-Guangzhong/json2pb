#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

# === 配置部分 ===
WORK_DIR="/home/luke/Projects/json-pb-convertor"
IGNORE_LIST=("build" "bin" "doc" ".vscode" "script" "template" "tmp")  # 只忽略 WORK_DIR/下的一级目录

# === 工作开始 ===
cd "$WORK_DIR" || {
    echo "无法进入目录: $WORK_DIR"
    exit 1
}

echo "当前目录: $WORK_DIR"
echo "忽略目录: ${IGNORE_LIST[*]}"

# 构造一级目录下忽略路径的 -path 语句
prune_args=()
for dir in "${IGNORE_LIST[@]}"; do
    if [[ -d "$dir" ]]; then
        prune_args+=( -path "./$dir" -prune -o )
    fi
done

# 添加查找目标文件的部分
# 找到 CMakeLists.txt 或 *.cmake，忽略一级目录中指定的子目录
find . "${prune_args[@]}" \
    \( -name "CMakeLists.txt" -o -name "*.cmake" \) -type f -print |
while IFS= read -r file; do
    echo "格式化: $file"
    cmake-format -c "${WORK_DIR}/.cmake-format.yaml" -i "$file"
done

echo "所有格式化完成。"
